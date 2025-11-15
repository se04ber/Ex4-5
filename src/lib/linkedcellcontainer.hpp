#pragma once
#include <fstream>
#include <vector>

#include "soacontainer.hpp"
#include "vector3d.hpp"

template <typename T>
class LinkedCellContainer {
  // this class stores multiple SoAContainers as "linked cells"
 public:
  // constructor (cutoff radius, number of cells in x,y,z, lower left front
  // corner, capacity to reserve per cell)
  LinkedCellContainer(T rcut, std::size_t n_x, std::size_t n_y, std::size_t n_z,
                      Vector3D<T> corner, std::size_t default_size);
  // lower left corner is zero
  LinkedCellContainer(T rcut, std::size_t n_x, std::size_t n_y, std::size_t n_z,
                      std::size_t default_size);
  // get fields (pre-implemented)
  const std::size_t get_size_x() const { return m_n_x; };
  const std::size_t get_size_y() const { return m_n_y; };
  const std::size_t get_size_z() const { return m_n_z; };
  const T cutoff() const { return m_rcut; };
  const Vector3D<T> get_corner() const { return m_corner; };
  
  // get container by index (bound-checked, pre-implemented)
  SoAContainer<T>& get(std::size_t idx) { return m_containers.at(idx); };
  const SoAContainer<T>& get_const(std::size_t idx) const {
    return m_containers.at(idx);
  }

  // get number of total containers
  const std::size_t num_containers() const;

  // whether a given position lies within the domain covered by the
  // LinkedCellContainer (boundaries are not included)
  const bool lies_within(T x, T y, T z) const;

  // get total number of particles
  const std::size_t size() const;
  
  // insert particle into the specified cell (do not check for correct index)
  void insert_particle(const std::size_t idx, T x, T y, T z, T vx, T vy, T vz,
                       T Fx, T Fy, T Fz);
  // insert particle into the correct cell (cell index inferred from particle position)
  void insert_particle(T x, T y, T z, T vx, T vy, T vz, T Fx, T Fy, T Fz);
  // get container index for coordinate
  const std::size_t get_container_index(T x, T y, T z) const;
  
  
  // cell indices to vector index
  const std::size_t get_idx(const std::size_t x_idx, const std::size_t y_idx,
                            const std::size_t z_idx) const;
  // vector indices to cell index
  const Vector3D<std::size_t> idx_to_cellnum(const std::size_t idx) const;
  

  // iterate over all particles. For each particle,
  // 1. check, whether it resides in the correct cell based on its position
  // 2. if not 1., insert a copy of it into the correct cell and delete the original
  void resort();
  
  // print all particles to csv-file
  void print_csv(const char* filename) const;

  // move current to old force and set current to zero
  // AND CALL resort()
  void flush_forces();

 private:
  // the cell container
  std::vector<SoAContainer<T>> m_containers{};
  // the cutoff length, represented by the sides of the container
  T m_rcut;
  // number of cells
  std::size_t m_n_x, m_n_y, m_n_z;
  // stores m_n_x*m_n_y; can be used for idx_to_cellnum() to avoid repeated multiplications
  std::size_t m_stride_z;
  // vector representing the front lower left corner
  Vector3D<T> m_corner;
};

template class LinkedCellContainer<double>;
template class LinkedCellContainer<float>;
