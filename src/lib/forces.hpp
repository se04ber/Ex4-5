#pragma once

#include <vector>

#include "particle3d.hpp"
#include "particlecontainer.hpp"
#include "vector3d.hpp"
#include "soacontainer.hpp"

template <typename T>
class LJForce {
 public:
  // iterates sequentially over all pairs of particles
  // in a container and performs the respective force update
  static void apply(ParticleContainer<T>& container);
  // iterates sequentially over all pairs of particles
  // in a container and performs the respective force update
  static void apply(SoAContainer<T>& container);
  // computes force vector from left particle to right particle
  // and writes it to argument vector F
  static void force_vector(const Particle3D<T>& p1, const Particle3D<T>& p2,
                           Vector3D<T>& F);

 private:
  static constexpr T m_one = 1.0;
  static constexpr T m_48 = 48.0;
  static constexpr T m_24 = 24.0;
};

// utility function that may be used by the student for later exercises
inline std::size_t abs_diff(std::size_t i, std::size_t j) {
  return (i > j) * (i - j) + (j > i) * (j - i);
}

// specify template specializations
template class LJForce<double>;
template class LJForce<float>;
