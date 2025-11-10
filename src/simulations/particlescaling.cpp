#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "../lib/boundaries.hpp"
#include "../lib/forces.hpp"
#include "../lib/integrator.hpp"
#include "../lib/particle3d.hpp"
#include "../lib/particlecontainer.hpp"
#include "../lib/problem.hpp"
#include "../lib/soacontainer.hpp"
//#include "../lib/linkedcellcontainer.hpp"
#include "../lib/vector3d.hpp"

//typedef double FP_TYPE;
typedef float FP_TYPE;

typedef ParticleContainer<FP_TYPE> CONTAINER_TYPE;
//typedef SoAContainer<FP_TYPE> CONTAINER_TYPE;
//typedef LinkedCellContainer<FP_TYPE> CONTAINER_TYPE;

#define PROBLEM RandomParticles<FP_TYPE>::get_aos_problem(n_x, n_y, n_z, v_particles, 55.0, -55.0, 55.0, -55.0, 55.0, -55.0)
//#define PROBLEM RandomParticles<FP_TYPE>::get_soa_problem(n_x, n_y, n_z, v_particles, 55.0, -55.0, 55.0, -55.0, 55.0, -55.0);
//#define PROBLEM RandomParticles<FP_TYPE>::get_lc_problem(n_x, n_y, n_z, v_particles, -55.0, -55.0, -55.0, 10.0, 11, 11, 11);


//#define FILENAME "/double_AoS.csv"
#define FILENAME "/float_AoS.csv"
//#define FILENAME "/double_SoA.csv"
//#define FILENAME "/float_SoA.csv"
//#define FILENAME "/double_LinkedCell.csv"
//#define FILENAME "/float_LinkedCell.csv"


int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Simulation needs to be invoked as './program "
                 "path/to/output/directory'"
              << std::endl;
    return EXIT_FAILURE;
  }

  // number of timesteps to perform
  const unsigned int num_repetitions = 10;
  // particle step size
  const std::size_t particle_stepsize = 3;
  // number of particle steps
  const std::size_t num_particle_steps = 10;
  // number of warmup steps
  const std::size_t num_warmup_steps = 25;
  // time step size
  const FP_TYPE dt = 1e-4;

  // number of particles in x,y,z direction
  std::size_t n_x = 3;
  std::size_t n_y = 3;
  std::size_t n_z = 3;
  // speed at initialization
  FP_TYPE v_particles = 0.1;

  // variables for timing the computations
  using timer = std::chrono::steady_clock;
  timer::time_point begin_comp = timer::now();
  timer::time_point end_comp = end_comp;
  auto simulation_time = begin_comp - begin_comp;

  // create reflective boundaries that span a large box around the simulation
  // domain
  
  const ReflectiveBoundary<FP_TYPE> x_boundary =
      ReflectiveBoundary((FP_TYPE)-55.0+(FP_TYPE)1e-3, (FP_TYPE)55.0-(FP_TYPE)1e-3, 0);
  const ReflectiveBoundary<FP_TYPE> y_boundary =
      ReflectiveBoundary((FP_TYPE)-55.0+(FP_TYPE)1e-3, (FP_TYPE)55.0-(FP_TYPE)1e-3, 1);
  const ReflectiveBoundary<FP_TYPE> z_boundary =
      ReflectiveBoundary((FP_TYPE)-55.0+(FP_TYPE)1e-3, (FP_TYPE)55.0-(FP_TYPE)1e-3, 2);
   
  // create integrator
  
  const VelocityStoermerVerlet<FP_TYPE> integrator = VelocityStoermerVerlet(dt);
  
  // open csv file
  std::string filename(argv[1]);
  filename += FILENAME;
  std::ofstream file_handle(filename.c_str());
  std::cout << "Writing results to " << filename << std::endl;
  file_handle << "No. Particles" << "," << "Time [s]" << std::endl;
  for(std::size_t step = 0; step < num_particle_steps; ++step){
    // create specified problem
    CONTAINER_TYPE cont = PROBLEM;
    std::cout << "Simulating " << cont.size() << " particles" << std::endl;
    // apply forces once
    LJForce<FP_TYPE>::apply(cont);
    for(std::size_t iteration_counter = 0; iteration_counter < num_repetitions + num_warmup_steps; ++iteration_counter){
        begin_comp = std::chrono::steady_clock::now();
        // update position
        integrator.update_positions(cont);
        // apply boundaries
        x_boundary.apply_container(cont);
        y_boundary.apply_container(cont);
        z_boundary.apply_container(cont);
        // flush forces
        cont.flush_forces();
        // compute new forces
        LJForce<FP_TYPE>::apply(cont);
        // update velocities
        integrator.update_velocities(cont);
        end_comp = std::chrono::steady_clock::now();
        // write timing to file
        if(iteration_counter >= num_warmup_steps){
        file_handle << cont.size() << "," << std::chrono::duration<double>(std::chrono::duration_cast<std::chrono::microseconds>(end_comp-begin_comp)).count()  << std::endl;
        }
    }
    // increase number of particles
    n_x += particle_stepsize;
    n_y += particle_stepsize;
    n_z += particle_stepsize;
  }
  file_handle.close();
  return EXIT_SUCCESS;
}
