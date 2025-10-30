#define BOOST_TEST_MODULE TestAoSBoundaries
#include <boost/test/included/unit_test.hpp>  //single-header
#include <random>
#include <utility>

#include "../lib/boundaries.hpp"
#include "../lib/particle3d.hpp"
#include "../lib/particlecontainer.hpp"
#include "../lib/vector3d.hpp"

typedef double FP_TYPE;
std::mt19937 rng;
std::uniform_real_distribution<FP_TYPE> real_dist(0, 1);

BOOST_AUTO_TEST_CASE(inner_unchanged) {
  for (int dim : {1, 2, 3}) {  // test for all dimensions
    const auto boundary = ReflectiveBoundary<FP_TYPE>(0.0, 1.0, dim);
    // populate container with some examples
    auto cont = ParticleContainer<FP_TYPE>(2);
    cont.insert(Particle3D<FP_TYPE>(0.1, 0.3, 0.5, 0.2, 0.4, 0.6));
    cont.insert(Particle3D<FP_TYPE>(0.15, 0.35, 0.55, 0.25, 0.45, 0.65));
    // apply boundary
    boundary.apply_container(cont);
    // check that components are unchanged
    BOOST_CHECK(cont.get(0).get_position() == Vector3D<FP_TYPE>(0.1, 0.3, 0.5));
    BOOST_CHECK(cont.get(1).get_position() ==
                Vector3D<FP_TYPE>(0.15, 0.35, 0.55));
    // leaves velocity of in-domain particles unchanged
    BOOST_CHECK(cont.get(0).get_velocity() == Vector3D<FP_TYPE>(0.2, 0.4, 0.6));
    BOOST_CHECK(cont.get(1).get_velocity() ==
                Vector3D<FP_TYPE>(0.25, 0.45, 0.65));
  }
};

BOOST_AUTO_TEST_CASE(outer_updated) {
  auto cont = ParticleContainer<FP_TYPE>(6);

  const auto boundary_x = ReflectiveBoundary<FP_TYPE>(0.0, 1.0, 0);
  const auto boundary_y = ReflectiveBoundary<FP_TYPE>(0.0, 1.0, 1);
  const auto boundary_z = ReflectiveBoundary<FP_TYPE>(0.0, 1.0, 2);

  // insert lower out-of-bounds
  cont.insert(Particle3D<FP_TYPE>(-0.1, 0.3, 0.5, 0.2, 0.4, 0.6));
  cont.insert(Particle3D<FP_TYPE>(0.1, -0.3, 0.5, 0.2, 0.4, 0.6));
  cont.insert(Particle3D<FP_TYPE>(0.1, 0.3, -0.4, 0.2, 0.4, 0.6));
  // insert upper out of bounds
  cont.insert(Particle3D<FP_TYPE>(1.1, 0.3, 0.5, 0.2, 0.4, 0.6));
  cont.insert(Particle3D<FP_TYPE>(0.1, 1.3, 0.5, 0.2, 0.4, 0.6));
  cont.insert(Particle3D<FP_TYPE>(0.1, 0.3, 1.2, 0.2, 0.4, 0.6));

  // apply x-boundary
  boundary_x.apply_container(cont);
  BOOST_CHECK(cont.get(0).get_position().approx(
      Vector3D<FP_TYPE>(0.1, 0.3, 0.5), 1e-7));
  BOOST_CHECK(cont.get(0).get_velocity().approx(
      Vector3D<FP_TYPE>(-0.2, 0.4, 0.6), 1e-7));
  BOOST_CHECK(cont.get(3).get_position().approx(
      Vector3D<FP_TYPE>(0.9, 0.3, 0.5), 1e-7));
  BOOST_CHECK(cont.get(3).get_velocity().approx(
      Vector3D<FP_TYPE>(-0.2, 0.4, 0.6), 1e-7));

  // apply y boundary
  boundary_y.apply_container(cont);
  BOOST_CHECK(cont.get(1).get_position().approx(
      Vector3D<FP_TYPE>(0.1, 0.3, 0.5), 1e-7));
  BOOST_CHECK(cont.get(1).get_velocity().approx(
      Vector3D<FP_TYPE>(0.2, -0.4, 0.6), 1e-7));
  BOOST_CHECK(cont.get(4).get_position().approx(
      Vector3D<FP_TYPE>(0.1, 0.7, 0.5), 1e-7));
  BOOST_CHECK(cont.get(4).get_velocity().approx(
      Vector3D<FP_TYPE>(0.2, -0.4, 0.6), 1e-7));

  // apply z boundary
  boundary_z.apply_container(cont);
  BOOST_CHECK(cont.get(2).get_position().approx(
      Vector3D<FP_TYPE>(0.1, 0.3, 0.4), 1e-7));
  BOOST_CHECK(cont.get(2).get_velocity().approx(
      Vector3D<FP_TYPE>(0.2, 0.4, -0.6), 1e-7));
  BOOST_CHECK(cont.get(5).get_position().approx(
      Vector3D<FP_TYPE>(0.1, 0.3, 0.8), 1e-7));
  BOOST_CHECK(cont.get(5).get_velocity().approx(
      Vector3D<FP_TYPE>(0.2, 0.4, -0.6), 1e-7));
};
