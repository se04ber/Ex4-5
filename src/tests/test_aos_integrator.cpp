#define BOOST_TEST_MODULE TestAoSIntegrator
#include <boost/test/included/unit_test.hpp>  //single-header
#include <random>

#include "../lib/forces.hpp"
#include "../lib/integrator.hpp"
#include "../lib/particle3d.hpp"
#include "../lib/particlecontainer.hpp"
#include "../lib/vector3d.hpp"

typedef double FP_TYPE;
std::mt19937 rng;
std::uniform_real_distribution<FP_TYPE> real_dist(-10, 10);

BOOST_AUTO_TEST_CASE(aos_position_update) {
  // create particles
  const auto particle1 =
      Particle3D<FP_TYPE>(real_dist(rng), real_dist(rng), real_dist(rng),
                          real_dist(rng), real_dist(rng), real_dist(rng));
  const auto particle2 =
      Particle3D<FP_TYPE>(real_dist(rng), real_dist(rng), real_dist(rng),
                          real_dist(rng), real_dist(rng), real_dist(rng));

  // get some force vectors (current and old)
  const auto old_vec =
      Vector3D<FP_TYPE>(real_dist(rng), real_dist(rng), real_dist(rng));
  // time step
  const FP_TYPE dt = real_dist(rng) + 11.0;

  // correct results
  const auto correct_position_1 = Vector3D<FP_TYPE>(
      particle1.get_position().get(0) + dt * particle1.get_velocity().get(0) +
          dt * dt * old_vec.get(0) / 2.0,
      particle1.get_position().get(1) + dt * particle1.get_velocity().get(1) +
          dt * dt * old_vec.get(1) / 2.0,
      particle1.get_position().get(2) + dt * particle1.get_velocity().get(2) +
          dt * dt * old_vec.get(2) / 2.0);
  const auto correct_position_2 = Vector3D<FP_TYPE>(
      particle2.get_position().get(0) + dt * particle2.get_velocity().get(0) +
          dt * dt * old_vec.get(0) / 2.0,
      particle2.get_position().get(1) + dt * particle2.get_velocity().get(1) +
          dt * dt * old_vec.get(1) / 2.0,
      particle2.get_position().get(2) + dt * particle2.get_velocity().get(2) +
          dt * dt * old_vec.get(2) / 2.0);

  // put particles in container
  auto cont = ParticleContainer<FP_TYPE>(2);
  cont.insert(particle1);
  cont.insert(particle2);
  // insert to particles
  for (std::size_t p_idx = 0; p_idx < cont.size(); ++p_idx) {
    cont.get(p_idx).add_current_force_inplace(old_vec);
  }
  // apply integration
  const auto integrator = VelocityStoermerVerlet<FP_TYPE>(dt);
  integrator.update_positions(cont);

  BOOST_CHECK(cont.get(0).get_position().approx(correct_position_1, 1e-7));
  BOOST_CHECK(cont.get(1).get_position().approx(correct_position_2, 1e-7));
}

BOOST_AUTO_TEST_CASE(aos_velocity_update) {
  // create particles
  const auto particle1 =
      Particle3D<FP_TYPE>(real_dist(rng), real_dist(rng), real_dist(rng),
                          real_dist(rng), real_dist(rng), real_dist(rng));
  const auto particle2 =
      Particle3D<FP_TYPE>(real_dist(rng), real_dist(rng), real_dist(rng),
                          real_dist(rng), real_dist(rng), real_dist(rng));

  // get some force vectors (current and old)
  const auto old_vec =
      Vector3D<FP_TYPE>(real_dist(rng), real_dist(rng), real_dist(rng));
  const auto new_vec =
      Vector3D<FP_TYPE>(real_dist(rng), real_dist(rng), real_dist(rng));
  // time step
  const FP_TYPE dt = real_dist(rng) + 11.0;

  // correct results
  const auto correct_velocity_1 =
      Vector3D<FP_TYPE>(particle1.get_velocity().get(0) +
                            dt * (old_vec.get(0) + new_vec.get(0)) / 2.0,
                        particle1.get_velocity().get(1) +
                            dt * (old_vec.get(1) + new_vec.get(1)) / 2.0,
                        particle1.get_velocity().get(2) +
                            dt * (old_vec.get(2) + new_vec.get(2)) / 2.0);

  const auto correct_velocity_2 =
      Vector3D<FP_TYPE>(particle2.get_velocity().get(0) +
                            dt * (old_vec.get(0) + new_vec.get(0)) / 2.0,
                        particle2.get_velocity().get(1) +
                            dt * (old_vec.get(1) + new_vec.get(1)) / 2.0,
                        particle2.get_velocity().get(2) +
                            dt * (old_vec.get(2) + new_vec.get(2)) / 2.0);

  // put particles in container
  auto cont = ParticleContainer<FP_TYPE>(2);
  cont.insert(particle1);
  cont.insert(particle2);
  // insert to particles
  for (std::size_t p_idx = 0; p_idx < cont.size(); ++p_idx) {
    cont.get(p_idx).add_current_force_inplace(old_vec);
    cont.get(p_idx).swap_and_flush_forces();
    cont.get(p_idx).add_current_force_inplace(new_vec);
  }

  const auto integrator = VelocityStoermerVerlet<FP_TYPE>(dt);
  integrator.update_velocities(cont);

  BOOST_CHECK(cont.get(0).get_velocity().approx(correct_velocity_1, 1e-7));
  BOOST_CHECK(cont.get(1).get_velocity().approx(correct_velocity_2, 1e-7));
}
