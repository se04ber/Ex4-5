#pragma once
#include "particlecontainer.hpp"

template <typename T>
class ReflectiveBoundary {
 public:
  // constructor
  ReflectiveBoundary(T min, T max, int dim);
  // getter functions
  const T get_min() const { return m_min; };
  const T get_max() const { return m_max; };
  // applies this boundary to the entire container
  void apply_container(ParticleContainer<T>& container) const;

 private:
  const T m_min;    // lower boundary
  const T m_max;    // higher boundary
  const int m_dim;  // component perpendicular to wall
};

template class ReflectiveBoundary<double>;
template class ReflectiveBoundary<float>;
