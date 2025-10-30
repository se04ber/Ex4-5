#define BOOST_TEST_MODULE TestParticleContainer
#include <boost/test/included/unit_test.hpp>  //single-header
#include <random>
#include <utility>
#include <vector>

#include "../lib/particle3d.hpp"
#include "../lib/particlecontainer.hpp"
#include "../lib/vector3d.hpp"

typedef double FP_TYPE;
std::mt19937 rng;
std::uniform_int_distribution<std::size_t> int_dist(2, 10);
std::uniform_real_distribution<FP_TYPE> float_dist(-10, 10);

BOOST_AUTO_TEST_CASE(constructor_capacity) {
  rng.seed(42);
  std::size_t cap;
  // a few repetitions
  for (int i = 0; i < 10; ++i) {
    // sample capacity
    cap = int_dist(rng);
    // create a corresponding container
    const auto cont = ParticleContainer<FP_TYPE>(cap);
    // and check for capacity
    BOOST_CHECK(cont.capacity() == cap);
  }
}

BOOST_AUTO_TEST_CASE(reserve_capacity) {
  rng.seed(42);
  std::size_t cap;
  for (int i = 0; i < 10; ++i) {
    // create particle container
    auto cont = ParticleContainer<FP_TYPE>(1);
    // sample a random capacity
    cap = int_dist(rng);
    // reserve the capacity
    cont.reserve(cap);
    // and check for capacity of the container
    BOOST_CHECK(cont.capacity() == cap);
  }
}

BOOST_AUTO_TEST_CASE(size_insert) {
  rng.seed(42);
  std::size_t cap;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // sample capacity
    cap = int_dist(rng);
    // create corresponding particle container
    auto cont = ParticleContainer<FP_TYPE>(cap);
    // check that the container is empty at the moment
    BOOST_CHECK(cont.size() == 0);
    // insert the correct number of particles
    for (std::size_t j = 0; j < cap; ++j) {
      cont.insert(Particle3D<FP_TYPE>(float_dist(rng), float_dist(rng),
                                      float_dist(rng), float_dist(rng),
                                      float_dist(rng), float_dist(rng)));
    }
    // and check that the size of the container has grown accordingly
    BOOST_CHECK(cont.size() == cap);
  }
}

BOOST_AUTO_TEST_CASE(get_element) {
  rng.seed(42);
  // create an empty particle container
  auto cont = ParticleContainer<FP_TYPE>(0);  // empty container
  // double check that the get() method performs bounds-checking
  BOOST_CHECK_THROW(cont.get(0), std::out_of_range);  // bounds check
  FP_TYPE x, y, z, vx, vy, vz;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // sample positions and velocities
    x = float_dist(rng);
    y = float_dist(rng);
    z = float_dist(rng);
    vx = float_dist(rng);
    vy = float_dist(rng);
    vz = float_dist(rng);
    // insert a corresponding particle
    cont.insert(Particle3D<FP_TYPE>(x, y, z, vx, vy, vz));
    // get the respective particle (as it has been inserted last)
    Particle3D<FP_TYPE>& p_last = cont.get(cont.size() - 1);
    // and check for equality by position and velocity vectors
    BOOST_CHECK(p_last.get_position() == Vector3D<FP_TYPE>(x, y, z));
    BOOST_CHECK(p_last.get_velocity() == Vector3D<FP_TYPE>(vx, vy, vz));
  }
}

BOOST_AUTO_TEST_CASE(flush_force) {
  rng.seed(42);
  FP_TYPE fx1, fx2, fy1, fy2, fz1, fz2;
  for (int i = 0; i < 10; ++i) {
    // create container and insert particles
    auto cont = ParticleContainer<FP_TYPE>(2);
    cont.insert(Particle3D<FP_TYPE>(float_dist(rng), float_dist(rng),
                                    float_dist(rng), float_dist(rng),
                                    float_dist(rng), float_dist(rng)));
    cont.insert(Particle3D<FP_TYPE>(float_dist(rng), float_dist(rng),
                                    float_dist(rng), float_dist(rng),
                                    float_dist(rng), float_dist(rng)));
    // define new forces
    fx1 = float_dist(rng);
    fx2 = float_dist(rng);
    fy1 = float_dist(rng);
    fy2 = float_dist(rng);
    fz1 = float_dist(rng);
    fz2 = float_dist(rng);
    // update forces
    cont.get(0).add_current_force_inplace(Vector3D<FP_TYPE>(fx1, fy1, fz1));
    cont.get(1).add_current_force_inplace(Vector3D<FP_TYPE>(fx2, fy2, fz2));
    // flush container
    cont.flush_forces();
    // verify flush
    BOOST_CHECK(cont.get(0).get_old_force() ==
                Vector3D<FP_TYPE>(fx1, fy1, fz1));
    BOOST_CHECK(cont.get(1).get_old_force() ==
                Vector3D<FP_TYPE>(fx2, fy2, fz2));
    BOOST_CHECK(cont.get(0).get_current_force() ==
                Vector3D<FP_TYPE>(0.0, 0.0, 0.0));
    BOOST_CHECK(cont.get(1).get_current_force() ==
                Vector3D<FP_TYPE>(0.0, 0.0, 0.0));
  }
}
