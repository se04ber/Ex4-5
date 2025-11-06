#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "../lib/boundaries.hpp"
#include "../lib/forces.hpp"
#include "../lib/integrator.hpp"
#include "../lib/particle3d.hpp"
#include "../lib/particlecontainer.hpp"
#include "../lib/problem.hpp"
#include "../lib/vector3d.hpp"

// double-precision simulation
typedef double FP_TYPE;
typedef ParticleContainer<FP_TYPE> CONTAINER_TYPE;

#define PROBLEM RandomParticles<FP_TYPE>::get_aos_problem(n_x, n_y, n_z, v_particles, 55.0, -55.0, 55.0, -55.0, 55.0, -55.0)

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Simulation needs to be invoked as './program "
                 "path/to/output/directory'"
              << std::endl;
    return EXIT_FAILURE;
  }
  // number of timesteps to perform
  const unsigned int num_timesteps = 15;
  // interval at which the particles are logged
  const unsigned int log_timesteps = 1;
  // time step size
  const FP_TYPE dt = 1e-4;

  // number of particles in x,y,z direction
  const std::size_t n_x = 25;
  const std::size_t n_y = 25;
  const std::size_t n_z = 25;
  // speed at initialization
  const FP_TYPE v_particles = 0.5;

  // variables for timing the computations
  using timer = std::chrono::steady_clock;
  timer::time_point begin_comp = timer::now();
  timer::time_point end_comp = end_comp;
  auto simulation_time = begin_comp - begin_comp;

  // create specified problem
  timer::time_point begin_problem_def = timer::steady_clock::now();
  CONTAINER_TYPE cont = PROBLEM;
  timer::time_point end_problem_def = timer::now();
  std::cout << "Filled container with " << cont.size() << " particles in: "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   end_problem_def - begin_problem_def)
                   .count()
            << "[micro sec]" << std::endl;
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
  // start counter
  begin_comp = std::chrono::steady_clock::now();
  // compute initial forces
  LJForce<FP_TYPE>::apply(cont);
  end_comp = std::chrono::steady_clock::now();
  simulation_time += end_comp - begin_comp;
  // simulate specified number of timesteps
  for (std::size_t t_idx = 0; t_idx <= num_timesteps; ++t_idx) {
    // check if we need to log state to csv
    if (t_idx % log_timesteps == 0) {
      // output file
      std::string filename(argv[1]);
      filename += "/collision.";
      filename += std::to_string(t_idx);
      filename += ".csv";
      cont.print_csv(filename.c_str());
      // print timing information so far
      if (t_idx != 0) {
        std::cout << " Entered timestep " << t_idx << "\t\t(average "
                  << std::chrono::duration<double>(
                         std::chrono::duration_cast<std::chrono::microseconds>(
                             simulation_time))
                             .count() /
                         static_cast<FP_TYPE>(t_idx)
                  << "s per timestep)" << std::endl;
      }
    }
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
    // add to time
    end_comp = std::chrono::steady_clock::now();
    simulation_time += end_comp - begin_comp;
  }
  return EXIT_SUCCESS;
}
