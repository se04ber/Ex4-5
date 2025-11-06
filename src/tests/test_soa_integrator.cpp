#define BOOST_TEST_MODULE TestSoAIntegrator
#include <boost/test/included/unit_test.hpp>  //single-header
#include <cmath>
#include <random>
#include <utility>

#include "../lib/forces.hpp"
#include "../lib/integrator.hpp"
#include "../lib/soacontainer.hpp"

typedef double FP_TYPE;
std::mt19937 rng;
std::uniform_real_distribution<FP_TYPE> real_dist(-10, 10);

BOOST_AUTO_TEST_CASE(aos_position_update) {
  // put particles in container
  auto cont = SoAContainer<FP_TYPE>(2);
  cont.insert(real_dist(rng), real_dist(rng), real_dist(rng), real_dist(rng),
              real_dist(rng), real_dist(rng));
  cont.insert(real_dist(rng), real_dist(rng), real_dist(rng), real_dist(rng),
              real_dist(rng), real_dist(rng));
  // create force
  FP_TYPE f_x = real_dist(rng);
  FP_TYPE f_y = real_dist(rng);
  FP_TYPE f_z = real_dist(rng);
  // time step
  const FP_TYPE dt = real_dist(rng) + 11.0;
  // correct results
  const FP_TYPE x_1 = cont.get_x(0) + dt * cont.get_vx(0) + dt * dt * f_x / 2.0;
  const FP_TYPE y_1 = cont.get_y(0) + dt * cont.get_vy(0) + dt * dt * f_y / 2.0;
  const FP_TYPE z_1 = cont.get_z(0) + dt * cont.get_vz(0) + dt * dt * f_z / 2.0;
  const FP_TYPE x_2 = cont.get_x(1) + dt * cont.get_vx(1) + dt * dt * f_x / 2.0;
  const FP_TYPE y_2 = cont.get_y(1) + dt * cont.get_vy(1) + dt * dt * f_y / 2.0;
  const FP_TYPE z_2 = cont.get_z(1) + dt * cont.get_vz(1) + dt * dt * f_z / 2.0;
  // insert to particles
  for (std::size_t p_idx = 0; p_idx < cont.size(); ++p_idx) {
    cont.set_Fx(p_idx, f_x);
    cont.set_Fy(p_idx, f_y);
    cont.set_Fz(p_idx, f_z);
  }

  // position update
  const auto integrator = VelocityStoermerVerlet<FP_TYPE>(dt);
  integrator.update_positions(cont);

  BOOST_CHECK(abs(cont.get_x(0) - x_1) < 1e-12);
  BOOST_CHECK(abs(cont.get_y(0) - y_1) < 1e-12);
  BOOST_CHECK(abs(cont.get_z(0) - z_1) < 1e-12);
  BOOST_CHECK(abs(cont.get_x(1) - x_2) < 1e-12);
  BOOST_CHECK(abs(cont.get_y(1) - y_2) < 1e-12);
  BOOST_CHECK(abs(cont.get_z(1) - z_2) < 1e-12);
}

BOOST_AUTO_TEST_CASE(aos_velocity_update) {
  // put particles in container
  auto cont = SoAContainer<FP_TYPE>(2);
  cont.insert(real_dist(rng), real_dist(rng), real_dist(rng), real_dist(rng),
              real_dist(rng), real_dist(rng));
  cont.insert(real_dist(rng), real_dist(rng), real_dist(rng), real_dist(rng),
              real_dist(rng), real_dist(rng));
  // create force
  FP_TYPE f_x = real_dist(rng);
  FP_TYPE f_y = real_dist(rng);
  FP_TYPE f_z = real_dist(rng);
  // create old force
  FP_TYPE f_x_old = real_dist(rng);
  FP_TYPE f_y_old = real_dist(rng);
  FP_TYPE f_z_old = real_dist(rng);
  const FP_TYPE dt = real_dist(rng) + 11.0;

  // correct results
  const auto vx_1 = cont.get_vx(0) + dt * (f_x + f_x_old) / 2.0;
  const auto vy_1 = cont.get_vy(0) + dt * (f_y + f_y_old) / 2.0;
  const auto vz_1 = cont.get_vz(0) + dt * (f_z + f_z_old) / 2.0;
  const auto vx_2 = cont.get_vx(1) + dt * (f_x + f_x_old) / 2.0;
  const auto vy_2 = cont.get_vy(1) + dt * (f_y + f_y_old) / 2.0;
  const auto vz_2 = cont.get_vz(1) + dt * (f_z + f_z_old) / 2.0;

  // insert to particles
  for (std::size_t p_idx = 0; p_idx < cont.size(); ++p_idx) {
    cont.set_Fx(p_idx, f_x);
    cont.set_Fy(p_idx, f_y);
    cont.set_Fz(p_idx, f_z);
  }
  cont.flush_forces();
  for (std::size_t p_idx = 0; p_idx < cont.size(); ++p_idx) {
    cont.set_Fx(p_idx, f_x_old);
    cont.set_Fy(p_idx, f_y_old);
    cont.set_Fz(p_idx, f_z_old);
  }

  const auto integrator = VelocityStoermerVerlet<FP_TYPE>(dt);
  integrator.update_velocities(cont);

  // check correctness
  BOOST_CHECK(abs(cont.get_vx(0) - vx_1) < 1e-12);
  BOOST_CHECK(abs(cont.get_vy(0) - vy_1) < 1e-12);
  BOOST_CHECK(abs(cont.get_vz(0) - vz_1) < 1e-12);
  BOOST_CHECK(abs(cont.get_vx(1) - vx_2) < 1e-12);
  BOOST_CHECK(abs(cont.get_vy(1) - vy_2) < 1e-12);
  BOOST_CHECK(abs(cont.get_vz(1) - vz_2) < 1e-12);
}
