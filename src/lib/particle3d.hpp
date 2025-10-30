#pragma once
#include <vector>

#include "vector3d.hpp"

template <typename T>
class Particle3D {
 public:
  // simple constructor for particles, forces shoud be zero-initialized
  Particle3D(T x, T y, T z, T v_x, T v_y, T v_z);
  // constant getters for member vectors
  const Vector3D<T>& get_position() const;
  const Vector3D<T>& get_velocity() const;
  const Vector3D<T>& get_current_force() const;
  const Vector3D<T>& get_old_force() const;
  // mutable getters
  Vector3D<T>& get_position_mut();
  Vector3D<T>& get_velocity_mut();

  // inplace additions / subtractions
  void add_position_inplace(const Vector3D<T>& delta_x);
  void add_velocity_inplace(const Vector3D<T>& delta_v);
  void add_current_force_inplace(const Vector3D<T>& delta_force);
  void subtract_current_force_inplace(const Vector3D<T>& delta_force);
  // shift current force vector into old force vector
  // and set current forces to zero
  void swap_and_flush_forces();
  // gets distance vector from this particle to another
  Vector3D<T> get_distance_vector(const Particle3D<T>& other) const;

 private:
  Vector3D<T> m_x;                                  // position
  Vector3D<T> m_v;                                  // velocity
  Vector3D<T> m_F{Vector3D<T>(0.0, 0.0, 0.0)};      // current forces
  Vector3D<T> m_F_old{Vector3D<T>(0.0, 0.0, 0.0)};  // old forces
};

template class Particle3D<float>;
template class Particle3D<double>;
