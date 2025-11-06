#include <cstdlib>
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
  // number of timesteps to perform
  const unsigned int num_timesteps = 10;
  // time step size
  const FP_TYPE dt = 1e-4;

  // number of particles in x,y,z direction
  const std::size_t n_x = 25;
  const std::size_t n_y = 25;
  const std::size_t n_z = 25;
  // speed at initialization
  const FP_TYPE v_particles = 0.1;

  // create specified problem
  CONTAINER_TYPE cont = PROBLEM;
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
  // compute initial forces
  LJForce<FP_TYPE>::apply(cont);
  // simulate specified number of timesteps
  for (std::size_t t_idx = 0; t_idx <= num_timesteps; ++t_idx) {
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
  }
  return EXIT_SUCCESS;
}
