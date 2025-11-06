#pragma once
#include <cmath>
#include <vector>

#include "particle3d.hpp"
#include "particlecontainer.hpp"

template <typename T>
class RandomParticles {
 /*
 This class represents particles that are initialized on a regular grid
 and at constant absolute velocities, but with randomly defined velocity vectors.
 */
 public:
  static ParticleContainer<T> get_aos_problem(
      const std::size_t n_x, const std::size_t n_y, const std::size_t n_z,
      const T v_particles, const T max_x, const T min_x, const T max_y,
      const T min_y, const T max_z, const T min_z);
};

template class RandomParticles<double>;
template class RandomParticles<float>;
