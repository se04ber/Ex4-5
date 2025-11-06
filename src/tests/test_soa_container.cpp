#define BOOST_TEST_MODULE TestSoAContainer
#include <boost/test/included/unit_test.hpp>  //single-header
#include <random>
#include <vector>

#include "../lib/soacontainer.hpp"
#include "../lib/vector3d.hpp"

typedef double FP_TYPE;
std::mt19937 rng;
std::uniform_int_distribution<std::size_t> int_dist(2, 10);
std::uniform_real_distribution<FP_TYPE> float_dist(-10, 10);

BOOST_AUTO_TEST_CASE(soa_constructor_capacity) {
  rng.seed(42);
  std::size_t cap;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // sample some capacity
    cap = int_dist(rng);
    // create SoAContainer with this capacity
    const auto cont = SoAContainer<FP_TYPE>(cap);
    // and assert that capacity() returns the correct value
    BOOST_CHECK(cont.capacity() == cap);
  }
}

BOOST_AUTO_TEST_CASE(soa_reserve_capacity) {
  rng.seed(42);
  std::size_t cap;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // create container of capacity 1
    auto cont = SoAContainer<FP_TYPE>(1);
    // sample and reserve capacity
    cap = int_dist(rng);
    cont.reserve(cap);
    // assert value
    BOOST_CHECK(cont.capacity() == cap);
  }
}

BOOST_AUTO_TEST_CASE(soa_size_insert) {
  rng.seed(42);
  std::size_t cap;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // sample capacity
    cap = int_dist(rng);
    // create container of appropriate capacity
    auto cont = SoAContainer<FP_TYPE>(cap);
    // assert that current size of (empty) container is 0
    BOOST_CHECK(cont.size() == 0);
    // insert particles
    for (std::size_t j = 0; j < cap; ++j) {
      cont.insert(float_dist(rng), float_dist(rng), float_dist(rng),
                  float_dist(rng), float_dist(rng), float_dist(rng));
    }
    // and check that size increases appropriately
    BOOST_CHECK(cont.size() == cap);
  }
}

BOOST_AUTO_TEST_CASE(soa_flush_force) {
  rng.seed(42);
  FP_TYPE fx1, fx2, fy1, fy2, fz1, fz2;
  for (int i = 0; i < 10; ++i) {
    // create container and insert particles
    auto cont = SoAContainer<FP_TYPE>(2);
    cont.insert(float_dist(rng), float_dist(rng), float_dist(rng),
                float_dist(rng), float_dist(rng), float_dist(rng));
    cont.insert(float_dist(rng), float_dist(rng), float_dist(rng),
                float_dist(rng), float_dist(rng), float_dist(rng));
    // define new forces
    fx1 = float_dist(rng);
    fx2 = float_dist(rng);
    fy1 = float_dist(rng);
    fy2 = float_dist(rng);
    fz1 = float_dist(rng);
    fz2 = float_dist(rng);
    // update forces
    cont.set_Fx(0, fx1);
    cont.set_Fy(0, fy1);
    cont.set_Fz(0, fz1);
    cont.set_Fx(1, fx2);
    cont.set_Fy(1, fy2);
    cont.set_Fz(1, fz2);
    // flush container
    cont.flush_forces();
    // verify flush
    BOOST_CHECK(cont.get_F_old_x(0) == fx1);
    BOOST_CHECK(cont.get_F_old_y(0) == fy1);
    BOOST_CHECK(cont.get_F_old_z(0) == fz1);
    BOOST_CHECK(cont.get_F_old_x(1) == fx2);
    BOOST_CHECK(cont.get_F_old_y(1) == fy2);
    BOOST_CHECK(cont.get_F_old_z(1) == fz2);
    BOOST_CHECK(cont.get_Fx(0) == 0.0);
    BOOST_CHECK(cont.get_Fy(0) == 0.0);
    BOOST_CHECK(cont.get_Fz(0) == 0.0);
    BOOST_CHECK(cont.get_Fx(1) == 0.0);
    BOOST_CHECK(cont.get_Fy(1) == 0.0);
    BOOST_CHECK(cont.get_Fz(1) == 0.0);
  }
}
