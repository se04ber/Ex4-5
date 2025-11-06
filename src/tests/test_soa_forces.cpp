#define BOOST_TEST_MODULE TestSoAForce
#include <boost/test/included/unit_test.hpp>  //single-header
#include <cmath>
#include <random>

#include "../lib/forces.hpp"
#include "../lib/soacontainer.hpp"

typedef double FP_TYPE;
std::mt19937 rng;
std::uniform_real_distribution<FP_TYPE> real_dist(-10, 10);

BOOST_AUTO_TEST_CASE(apply_soa_container) {
  // put particles in container
  auto cont = SoAContainer<FP_TYPE>(2);
  cont.insert(0.0, 0.0, 0.0, real_dist(rng), real_dist(rng), real_dist(rng));
  cont.insert(0.0, 1.0, 0.0, real_dist(rng), real_dist(rng), real_dist(rng));

  // apply force
  LJForce<FP_TYPE>::apply(cont);

  // check
  BOOST_CHECK(abs(cont.get_Fx(0)) < 1e-12);
  BOOST_CHECK(abs(cont.get_Fy(0) - 24.0) < 1e-12);
  BOOST_CHECK(abs(cont.get_Fz(0)) < 1e-12);
  BOOST_CHECK(abs(cont.get_Fx(1)) < 1e-12);
  BOOST_CHECK(abs(cont.get_Fy(1) + 24.0) < 1e-12);
  BOOST_CHECK(abs(cont.get_Fz(1)) < 1e-12);
}
