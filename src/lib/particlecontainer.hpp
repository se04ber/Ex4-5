#pragma once
#include <fstream>
#include <vector>

#include "particle3d.hpp"
#include "vector3d.hpp"

template <typename T>
class ParticleContainer {
 public:
  // create empty container of given capacity
  ParticleContainer(std::size_t my_size);
  // return capacity of container
  const std::size_t capacity() const;
  // reserve memory for our container
  void reserve(std::size_t new_size);
  // return size of container
  const std::size_t size() const;
  // insert particle
  void insert(Particle3D<T> particle);
  // retrieve element at index (should check for out-of-bounds)
  Particle3D<T>& get(std::size_t index);
  // for each particle: move current force to old force and set current force to
  // zero
  void flush_forces();

  // print all particles to csv-file
  // (not to be implemented by student)
  void print_csv(const char* filename) {
    std::ofstream file_handle(filename);
    file_handle << "x,y,z,v" << std::endl;
    for (auto& p : m_particles) {
      auto& pos = p.get_position();
      auto& vel = p.get_velocity();
      file_handle << pos.get(0) << "," << pos.get(1) << "," << pos.get(2) << ","
                  << vel.length() << std::endl;
    }
    file_handle.close();
  };

 private:
  // the vector data structure that stores all particles in this container
  std::vector<Particle3D<T>> m_particles;
};

// specify required templates
template class ParticleContainer<float>;
template class ParticleContainer<double>;
