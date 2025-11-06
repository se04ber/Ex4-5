#include "problem.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

#include "particle3d.hpp"
#include "particlecontainer.hpp"
#include "vector3d.hpp"
//#include "linkedcellcontainer.hpp"

template <typename T>
ParticleContainer<T> RandomParticles<T>::get_aos_problem(
    const std::size_t n_x, const std::size_t n_y, const std::size_t n_z,
    const T v_particles, const T max_x, const T min_x, const T max_y,
    const T min_y, const T max_z, const T min_z) {
  std::mt19937 rng;
  std::uniform_real_distribution<T> uniform_dist(-1, 1);

  rng.seed(42);

  ParticleContainer<T> cont = ParticleContainer<T>(n_x * n_y * n_z);

  Vector3D<T> velocity_vec = Vector3D<T>(0.0, 0.0, 0.0);

  const T step_x = ((max_x - 1e-2) - (min_x + 1e-2)) / n_x;
  const T step_y = ((max_y - 1e-2) - (min_y + 1e-2)) / n_y;
  const T step_z = ((max_z - 1e-2) - (min_z + 1e-2)) / n_z;

  for (T x_pos = min_x + 1e-2; x_pos < max_x - 1e-2; x_pos += step_x) {
    for (T y_pos = min_y + 1e-2; y_pos < max_y - 1e-2; y_pos += step_y) {
      for (T z_pos = min_z + 1e-2; z_pos < max_z - 1e-2; z_pos += step_z) {
        // get velocity
        velocity_vec.set(0, uniform_dist(rng));
        velocity_vec.set(1, uniform_dist(rng));
        velocity_vec.set(2, uniform_dist(rng));
        velocity_vec *= v_particles / velocity_vec.length();
        cont.insert(Particle3D<T>(x_pos, y_pos, z_pos, velocity_vec.get(0),
                                  velocity_vec.get(1), velocity_vec.get(2)));
      }
    }
  }
  return cont;
}

/*
template <typename T>
SoAContainer<T> RandomParticles<T>::get_soa_problem(
    const std::size_t n_x, const std::size_t n_y, const std::size_t n_z,
    const T v_particles, const T max_x, const T min_x, const T max_y,
    const T min_y, const T max_z, const T min_z) {
  std::mt19937 rng;
  std::uniform_real_distribution<T> uniform_dist(-1, 1);

  rng.seed(42);

  SoAContainer<T> cont = SoAContainer<T>(n_x * n_y * n_z);

  Vector3D<T> velocity_vec = Vector3D<T>(0.0, 0.0, 0.0);

  const T step_x = ((max_x - 1e-2) - (min_x + 1e-2)) / n_x;
  const T step_y = ((max_y - 1e-2) - (min_y + 1e-2)) / n_y;
  const T step_z = ((max_z - 1e-2) - (min_z + 1e-2)) / n_z;

  for (T x_pos = min_x + 1e-2; x_pos < max_x - 1e-2; x_pos += step_x) {
    for (T y_pos = min_y + 1e-2; y_pos < max_y - 1e-2; y_pos += step_y) {
      for (T z_pos = min_z + 1e-2; z_pos < max_z - 1e-2; z_pos += step_z) {
        // get velocity
        velocity_vec.set(0, uniform_dist(rng));
        velocity_vec.set(1, uniform_dist(rng));
        velocity_vec.set(2, uniform_dist(rng));
        velocity_vec *= v_particles / velocity_vec.length();
        cont.insert(x_pos, y_pos, z_pos, velocity_vec.get(0),
                    velocity_vec.get(1), velocity_vec.get(2));
      }
    }
  }
  return cont;
}
*/
