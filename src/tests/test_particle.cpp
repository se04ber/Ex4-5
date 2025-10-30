#define BOOST_TEST_MODULE TestParticle3D
#include <boost/test/included/unit_test.hpp>  //single-header
#include <random>

#include "../lib/particle3d.hpp"
#include "../lib/vector3d.hpp"

typedef double FP_TYPE;
std::mt19937 rng;
std::uniform_real_distribution<FP_TYPE> uniform_dist(-10, 10);

BOOST_AUTO_TEST_CASE(constructor_and_getters) {
  rng.seed(42);
  FP_TYPE x, y, z, vx, vy, vz;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // sample random position and velocity vectors
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    vx = uniform_dist(rng);
    vy = uniform_dist(rng);
    vz = uniform_dist(rng);
    auto expected_position = Vector3D<FP_TYPE>(x, y, z);
    auto expected_velocity = Vector3D<FP_TYPE>(vx, vy, vz);
    auto expected_force = Vector3D<FP_TYPE>(0.0, 0.0, 0.0);
    auto expected_old_force = Vector3D<FP_TYPE>(0.0, 0.0, 0.0);
    // create the particle
    const auto test_particle = Particle3D<FP_TYPE>(x, y, z, vx, vy, vz);
    // check that position, velocity are as defined by constructor, all other
    // vectors are zero
    BOOST_CHECK(test_particle.get_position() == expected_position);
    BOOST_CHECK(test_particle.get_velocity() == expected_velocity);
    BOOST_CHECK(test_particle.get_current_force() == expected_force);
    BOOST_CHECK(test_particle.get_old_force() == expected_old_force);
  }
}

BOOST_AUTO_TEST_CASE(mutable_getters) {
  rng.seed(42);
  FP_TYPE x, y, z, vx, vy, vz, x_new, y_new, z_new, vx_new, vy_new, vz_new;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // current positions/velocities
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    vx = uniform_dist(rng);
    vy = uniform_dist(rng);
    vz = uniform_dist(rng);
    // new positions / velocities
    x_new = uniform_dist(rng);
    y_new = uniform_dist(rng);
    z_new = uniform_dist(rng);
    vx_new = uniform_dist(rng);
    vy_new = uniform_dist(rng);
    vz_new = uniform_dist(rng);
    // create particle
    auto test_particle = Particle3D<FP_TYPE>(x, y, z, vx, vy, vz);
    // obtain references to positions and velocities
    Vector3D<FP_TYPE>& position = test_particle.get_position_mut();
    Vector3D<FP_TYPE>& velocity = test_particle.get_velocity_mut();
    // set components
    position.set(0, x_new);
    position.set(1, y_new);
    position.set(2, z_new);
    velocity.set(0, vx_new);
    velocity.set(1, vy_new);
    velocity.set(2, vz_new);
    // and assert that particle class members have been mutated
    BOOST_CHECK(test_particle.get_position() ==
                Vector3D<FP_TYPE>(x_new, y_new, z_new));
    BOOST_CHECK(test_particle.get_velocity() ==
                Vector3D<FP_TYPE>(vx_new, vy_new, vz_new));
  }
}

BOOST_AUTO_TEST_CASE(add_position_inplace) {
  rng.seed(42);
  FP_TYPE x, y, z, vx, vy, vz, x_diff, y_diff, z_diff;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // sample current particle position and velocity
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    vx = uniform_dist(rng);
    vy = uniform_dist(rng);
    vz = uniform_dist(rng);
    // the difference vector to add
    x_diff = uniform_dist(rng);
    y_diff = uniform_dist(rng);
    z_diff = uniform_dist(rng);
    // create particle
    auto test_particle = Particle3D<FP_TYPE>(x, y, z, vx, vy, vz);
    // in-place addition
    test_particle.add_position_inplace(
        Vector3D<FP_TYPE>(x_diff, y_diff, z_diff));
    // manual addition for comparison
    auto expected_result =
        Vector3D<FP_TYPE>(x + x_diff, y + y_diff, z + z_diff);
    // compare results
    BOOST_CHECK(test_particle.get_position() == expected_result);
  }
}

BOOST_AUTO_TEST_CASE(add_velocity_inplace) {
  // same as test add_position_inplace but using velocity vector
  rng.seed(42);
  FP_TYPE x, y, z, vx, vy, vz, x_diff, y_diff, z_diff;
  for (int i = 0; i < 10; ++i) {
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    vx = uniform_dist(rng);
    vy = uniform_dist(rng);
    vz = uniform_dist(rng);
    x_diff = uniform_dist(rng);
    y_diff = uniform_dist(rng);
    z_diff = uniform_dist(rng);
    auto test_particle = Particle3D<FP_TYPE>(x, y, z, vx, vy, vz);
    test_particle.add_velocity_inplace(
        Vector3D<FP_TYPE>(x_diff, y_diff, z_diff));
    auto expected_result =
        Vector3D<FP_TYPE>(vx + x_diff, vy + y_diff, vz + z_diff);
    BOOST_CHECK(test_particle.get_velocity() == expected_result);
  }
}

BOOST_AUTO_TEST_CASE(add_current_force_inplace) {
  // same as test add_position_inplace but using current force vector
  rng.seed(42);
  FP_TYPE x, y, z, vx, vy, vz, x_diff, y_diff, z_diff;
  for (int i = 0; i < 10; ++i) {
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    vx = uniform_dist(rng);
    vy = uniform_dist(rng);
    vz = uniform_dist(rng);
    x_diff = uniform_dist(rng);
    y_diff = uniform_dist(rng);
    z_diff = uniform_dist(rng);
    auto test_particle = Particle3D<FP_TYPE>(x, y, z, vx, vy, vz);
    test_particle.add_current_force_inplace(
        Vector3D<FP_TYPE>(x_diff, y_diff, z_diff));
    auto expected_result = Vector3D<FP_TYPE>(x_diff, y_diff, z_diff);
    BOOST_CHECK(test_particle.get_current_force() == expected_result);
  }
}

BOOST_AUTO_TEST_CASE(subtract_current_force_inplace) {
  // same as test add_current_force_inplace but using subtraction
  rng.seed(42);
  FP_TYPE x, y, z, vx, vy, vz, x_diff, y_diff, z_diff;
  for (int i = 0; i < 10; ++i) {
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    vx = uniform_dist(rng);
    vy = uniform_dist(rng);
    vz = uniform_dist(rng);
    x_diff = uniform_dist(rng);
    y_diff = uniform_dist(rng);
    z_diff = uniform_dist(rng);
    auto test_particle = Particle3D<FP_TYPE>(x, y, z, vx, vy, vz);
    test_particle.subtract_current_force_inplace(
        Vector3D<FP_TYPE>(x_diff, y_diff, z_diff));
    auto expected_result = Vector3D<FP_TYPE>(x_diff, y_diff, z_diff);
    expected_result *= -1.0;
    BOOST_CHECK(test_particle.get_current_force() == expected_result);
  }
}

BOOST_AUTO_TEST_CASE(swap_and_flush_forces) {
  rng.seed(42);
  FP_TYPE x, y, z, vx, vy, vz, fx_diff, fy_diff, fz_diff;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // sample position and velocity
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    vx = uniform_dist(rng);
    vy = uniform_dist(rng);
    vz = uniform_dist(rng);
    // force difference
    fx_diff = uniform_dist(rng);
    fy_diff = uniform_dist(rng);
    fz_diff = uniform_dist(rng);
    // create particle
    auto test_particle = Particle3D<FP_TYPE>(x, y, z, vx, vy, vz);
    // add to current force vector
    test_particle.add_current_force_inplace(
        Vector3D<FP_TYPE>(fx_diff, fy_diff, fz_diff));
    // define expected current force vector after swap_and_flush
    auto expected_result_current = Vector3D<FP_TYPE>(0.0, 0.0, 0.0);
    // define expected old force vector after swap_and_flush
    auto expected_result_old = Vector3D<FP_TYPE>(fx_diff, fy_diff, fz_diff);
    // perform operation
    test_particle.swap_and_flush_forces();
    // and compare vectors
    BOOST_CHECK(test_particle.get_current_force() == expected_result_current);
    BOOST_CHECK(test_particle.get_old_force() == expected_result_old);
  }
}

BOOST_AUTO_TEST_CASE(get_distance_vector) {
  rng.seed(42);
  FP_TYPE x1, y1, z1, vx1, vy1, vz1, x2, y2, z2, vx2, vy2, vz2;
  for (int i = 0; i < 10; ++i) {
    x1 = uniform_dist(rng);
    y1 = uniform_dist(rng);
    z1 = uniform_dist(rng);
    vx1 = uniform_dist(rng);
    vy1 = uniform_dist(rng);
    vz1 = uniform_dist(rng);
    x2 = uniform_dist(rng);
    y2 = uniform_dist(rng);
    z2 = uniform_dist(rng);
    vx2 = uniform_dist(rng);
    vy2 = uniform_dist(rng);
    vz2 = uniform_dist(rng);

    auto p1 = Particle3D<FP_TYPE>(x1, y1, z1, vx1, vy1, vz1);
    auto p2 = Particle3D<FP_TYPE>(x2, y2, z2, vx2, vy2, vz2);

    auto diff_p1_p2 = p1.get_distance_vector(p2);
    auto diff_p2_p1 = p2.get_distance_vector(p1);

    BOOST_CHECK(diff_p1_p2 == (diff_p2_p1 * (-1.0)));
    BOOST_CHECK(diff_p1_p2 == (Vector3D<FP_TYPE>(x2, y2, z2) -
                               Vector3D<FP_TYPE>(x1, y1, z1)));
  }
}
