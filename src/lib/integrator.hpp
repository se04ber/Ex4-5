#pragma once
#include "particlecontainer.hpp"
#include "soacontainer.hpp"

template <typename T>
class VelocityStoermerVerlet {
 public:
  // constructor
  VelocityStoermerVerlet(T dt) : m_dt{dt}, m_a{dt / (T)2.0} {};
  // updates positions of all particles in particle container
  void update_positions(ParticleContainer<T>& container) const;
  // updates velocity of all particles in particle container
  void update_velocities(ParticleContainer<T>& container) const;
  // updates positions of all particles in SoA container
  void update_positions(SoAContainer<T>& container) const;
  // updates velocity of all particles in SoA container
  void update_velocities(SoAContainer<T>& container) const;

 private:
  const T m_dt;
  const T m_a;
};

// template specializations
template class VelocityStoermerVerlet<double>;
template class VelocityStoermerVerlet<float>;
