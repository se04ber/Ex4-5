#pragma once
#include <cmath>
#include <vector>

#include "particle3d.hpp"
#include "particlecontainer.hpp"
#include "soacontainer.hpp"
//#include "linkedcellcontainer.hpp"

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
      static SoAContainer<T> get_soa_problem(
  const std::size_t n_x, const std::size_t n_y, const std::size_t n_z,
      const T v_particles, const T max_x, const T min_x, const T max_y,
      const T min_y, const T max_z, const T min_z);
  //static LinkedCellContainer<T> get_lc_problem(
  //    const std::size_t n_x, const std::size_t n_y, const std::size_t n_z,
  //    const T v_particles, const T min_x, const T min_y, const T min_z,
  //    const T r_cut, const std::size_t x_cells, const std::size_t y_cells,
  //    const std::size_t z_cells);
};

template class RandomParticles<double>;
template class RandomParticles<float>;
