#pragma once

#include <cmath>
#include <iostream>
#include <vector>

#include "particle3d.hpp"
#include "utils.hpp"
#include "vector3d.hpp"

template <typename T>
class SoAContainer {
 public:
  // create empty container of given capacity
  SoAContainer(std::size_t my_size);
  // return capacity of container
  const std::size_t capacity() const;
  // reserve memory for our container
  virtual void reserve(std::size_t new_size);
  // return size of container
  const std::size_t size() const;
  // insert particle
  // default: set current and old forces to zero
  void insert(T x, T y, T z, T vx, T vy, T vz);
  // also: insert particles; but here set current forces
  // to specified values
  void insert(T x, T y, T z, T vx, T vy, T vz, T Fx, T Fy, T Fz);
  // move current to old force and set current to zero
  void flush_forces();
  // delete particle from container (only used for linked cell)
  virtual void delete_particle(const std::size_t idx);
  // efficiently delete element from vector
  // Hint: pop-and-swap approach
  void delete_from_vector(std::vector<T>& vec, const std::size_t idx);
  // print all particles to csv-file
  void print_csv(const char* filename) const;

  // retrieve positions at index
  const T get_x(std::size_t index) const { return m_x[index]; };
  const T get_y(std::size_t index) const { return m_y[index]; };
  const T get_z(std::size_t index) const { return m_z[index]; };
  // retrieve velocities at index
  const T get_vx(std::size_t index) const { return m_vx[index]; };
  const T get_vy(std::size_t index) const { return m_vy[index]; };
  const T get_vz(std::size_t index) const { return m_vz[index]; };
  // retrieve current forces at index
  const T get_Fx(std::size_t index) const { return m_Fx[index]; };
  const T get_Fy(std::size_t index) const { return m_Fy[index]; };
  const T get_Fz(std::size_t index) const { return m_Fz[index]; };
  // retrieve old forces at index
  const T get_F_old_x(std::size_t index) const { return m_F_old_x[index]; };
  const T get_F_old_y(std::size_t index) const { return m_F_old_y[index]; };
  const T get_F_old_z(std::size_t index) const { return m_F_old_z[index]; };

  // set positions at index
  void set_x(std::size_t index, T new_val) { m_x.at(index) = new_val; };
  void set_y(std::size_t index, T new_val) { m_y.at(index) = new_val; };
  void set_z(std::size_t index, T new_val) { m_z.at(index) = new_val; };
  // set velocities at index
  void set_vx(std::size_t index, T new_val) { m_vx.at(index) = new_val; };
  void set_vy(std::size_t index, T new_val) { m_vy.at(index) = new_val; };
  void set_vz(std::size_t index, T new_val) { m_vz.at(index) = new_val; };
  // set current forces at index
  void set_Fx(std::size_t index, T new_val) { m_Fx.at(index) = new_val; };
  void set_Fy(std::size_t index, T new_val) { m_Fy.at(index) = new_val; };
  void set_Fz(std::size_t index, T new_val) { m_Fz.at(index) = new_val; };
  // set old forces at index
  void set_F_old_x(std::size_t index, T new_val) {
    m_F_old_x.at(index) = new_val;
  };
  void set_F_old_y(std::size_t index, T new_val) {
    m_F_old_y.at(index) = new_val;
  };
  void set_F_old_z(std::size_t index, T new_val) {
    m_F_old_z.at(index) = new_val;
  };

 protected:
  alignas(64) std::vector<T> m_x, m_y, m_z;                    // positions
  alignas(64) std::vector<T> m_vx, m_vy, m_vz;                 // velocities
  alignas(64) std::vector<T> m_Fx, m_Fy, m_Fz;                 // current forces
  alignas(64) std::vector<T> m_F_old_x, m_F_old_y, m_F_old_z;  // old forces
};

template class SoAContainer<double>;
template class SoAContainer<float>;
