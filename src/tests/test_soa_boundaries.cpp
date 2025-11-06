#define BOOST_TEST_MODULE TestSoABoundaries
#include <boost/test/included/unit_test.hpp>  //single-header
#include <cmath>
#include <random>
#include <utility>

#include "../lib/boundaries.hpp"
#include "../lib/soacontainer.hpp"

typedef double FP_TYPE;
std::mt19937 rng;
std::uniform_real_distribution<FP_TYPE> real_dist(0, 1);

BOOST_AUTO_TEST_CASE(inner_unchanged) {
  for (int dim : {1, 2, 3}) {  // test for all dimensions
    const auto boundary = ReflectiveBoundary<FP_TYPE>(0.0, 1.0, dim);
    // populate container with some examples
    auto cont = SoAContainer<FP_TYPE>(2);
    cont.insert(0.1, 0.3, 0.5, 0.2, 0.4, 0.6);
    cont.insert(0.15, 0.35, 0.55, 0.25, 0.45, 0.65);
    // apply boundary
    boundary.apply_container(cont);
    // check that components are unchanged
    BOOST_CHECK(cont.get_x(0) == 0.1);
    BOOST_CHECK(cont.get_y(0) == 0.3);
    BOOST_CHECK(cont.get_z(0) == 0.5);
    BOOST_CHECK(cont.get_x(1) == 0.15);
    BOOST_CHECK(cont.get_y(1) == 0.35);
    BOOST_CHECK(cont.get_z(1) == 0.55);
    // leaves velocity of in-domain particles unchanged
    BOOST_CHECK(cont.get_vx(0) == 0.2);
    BOOST_CHECK(cont.get_vy(0) == 0.4);
    BOOST_CHECK(cont.get_vz(0) == 0.6);
    BOOST_CHECK(cont.get_vx(1) == 0.25);
    BOOST_CHECK(cont.get_vy(1) == 0.45);
    BOOST_CHECK(cont.get_vz(1) == 0.65);
  }
};

BOOST_AUTO_TEST_CASE(outer_updated) {
  auto cont = SoAContainer<FP_TYPE>(6);

  const auto boundary_x = ReflectiveBoundary<FP_TYPE>(0.0, 1.0, 0);
  const auto boundary_y = ReflectiveBoundary<FP_TYPE>(0.0, 1.0, 1);
  const auto boundary_z = ReflectiveBoundary<FP_TYPE>(0.0, 1.0, 2);

  // insert lower out-of-bounds
  cont.insert(-0.1, 0.3, 0.5, 0.2, 0.4, 0.6);
  cont.insert(0.1, -0.3, 0.5, 0.2, 0.4, 0.6);
  cont.insert(0.1, 0.3, -0.4, 0.2, 0.4, 0.6);
  // insert upper out of bounds
  cont.insert(1.1, 0.3, 0.5, 0.2, 0.4, 0.6);
  cont.insert(0.1, 1.3, 0.5, 0.2, 0.4, 0.6);
  cont.insert(0.1, 0.3, 1.2, 0.2, 0.4, 0.6);

  // apply x-boundary
  boundary_x.apply_container(cont);
  BOOST_CHECK(abs(cont.get_x(0) - 0.1) < 1e-12);
  BOOST_CHECK(abs(cont.get_y(0) - 0.3) < 1e-12);
  BOOST_CHECK(abs(cont.get_z(0) - 0.5) < 1e-12);
  BOOST_CHECK(abs(cont.get_vx(0) + 0.2) < 1e-12);
  BOOST_CHECK(abs(cont.get_vy(0) - 0.4) < 1e-12);
  BOOST_CHECK(abs(cont.get_vz(0) - 0.6) < 1e-12);
  BOOST_CHECK(abs(cont.get_x(3) - 0.9) < 1e-12);
  BOOST_CHECK(abs(cont.get_y(3) - 0.3) < 1e-12);
  BOOST_CHECK(abs(cont.get_z(3) - 0.5) < 1e-12);
  BOOST_CHECK(abs(cont.get_vx(3) + 0.2) < 1e-12);
  BOOST_CHECK(abs(cont.get_vy(3) - 0.4) < 1e-12);
  BOOST_CHECK(abs(cont.get_vz(3) - 0.6) < 1e-12);

  // apply y boundary
  boundary_y.apply_container(cont);
  BOOST_CHECK(abs(cont.get_x(1) - 0.1) < 1e-12);
  BOOST_CHECK(abs(cont.get_y(1) - 0.3) < 1e-12);
  BOOST_CHECK(abs(cont.get_z(1) - 0.5) < 1e-12);
  BOOST_CHECK(abs(cont.get_vx(1) - 0.2) < 1e-12);
  BOOST_CHECK(abs(cont.get_vy(1) + 0.4) < 1e-12);
  BOOST_CHECK(abs(cont.get_vz(1) - 0.6) < 1e-12);
  BOOST_CHECK(abs(cont.get_x(4) - 0.1) < 1e-12);
  BOOST_CHECK(abs(cont.get_y(4) - 0.7) < 1e-12);
  BOOST_CHECK(abs(cont.get_z(4) - 0.5) < 1e-12);
  BOOST_CHECK(abs(cont.get_vx(4) - 0.2) < 1e-12);
  BOOST_CHECK(abs(cont.get_vy(4) + 0.4) < 1e-12);
  BOOST_CHECK(abs(cont.get_vz(4) - 0.6) < 1e-12);

  // apply z boundary
  boundary_z.apply_container(cont);
  BOOST_CHECK(abs(cont.get_x(2) - 0.1) < 1e-12);
  BOOST_CHECK(abs(cont.get_y(2) - 0.3) < 1e-12);
  BOOST_CHECK(abs(cont.get_z(2) - 0.4) < 1e-12);
  BOOST_CHECK(abs(cont.get_vx(2) - 0.2) < 1e-12);
  BOOST_CHECK(abs(cont.get_vy(2) - 0.4) < 1e-12);
  BOOST_CHECK(abs(cont.get_vz(2) + 0.6) < 1e-12);
  BOOST_CHECK(abs(cont.get_x(5) - 0.1) < 1e-12);
  BOOST_CHECK(abs(cont.get_y(5) - 0.3) < 1e-12);
  BOOST_CHECK(abs(cont.get_z(5) - 0.8) < 1e-12);
  BOOST_CHECK(abs(cont.get_vx(5) - 0.2) < 1e-12);
  BOOST_CHECK(abs(cont.get_vy(5) - 0.4) < 1e-12);
  BOOST_CHECK(abs(cont.get_vz(5) + 0.6) < 1e-12);
};
