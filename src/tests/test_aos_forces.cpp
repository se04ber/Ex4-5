#define BOOST_TEST_MODULE TestAoSForces
#include <boost/test/included/unit_test.hpp>  //single-header
#include <cmath>
#include <random>
#include <utility>

#include "../lib/forces.hpp"
#include "../lib/particle3d.hpp"
#include "../lib/particlecontainer.hpp"
#include "../lib/vector3d.hpp"

typedef double FP_TYPE;
std::mt19937 rng;
std::uniform_real_distribution<FP_TYPE> real_dist(-10, 10);

BOOST_AUTO_TEST_CASE(LJ_force_vector) {
  rng.seed(42);

  // create particles
  const auto particle1 = Particle3D<FP_TYPE>(0.0, 0.0, 0.0, real_dist(rng),
                                             real_dist(rng), real_dist(rng));
  const auto particle2 = Particle3D<FP_TYPE>(1.0, 0.0, 0.0, real_dist(rng),
                                             real_dist(rng), real_dist(rng));
  const auto particle3 = Particle3D<FP_TYPE>(0.0, 1.0, 0.0, real_dist(rng),
                                             real_dist(rng), real_dist(rng));
  const auto particle4 = Particle3D<FP_TYPE>(0.0, 0.0, 1.0, real_dist(rng),
                                             real_dist(rng), real_dist(rng));

  // correct force vector
  const auto correct_force_1_2 = Vector3D<FP_TYPE>(24.0, 0.0, 0.0);
  const auto correct_force_1_3 = Vector3D<FP_TYPE>(0.0, 24.0, 0.0);
  const auto correct_force_1_4 = Vector3D<FP_TYPE>(0.0, 0.0, 24.0);

  // allocate vector to store the computed forces into
  auto computed_force_1_2 = Vector3D<FP_TYPE>();
  auto computed_force_1_3 = Vector3D<FP_TYPE>();
  auto computed_force_1_4 = Vector3D<FP_TYPE>();

  // compute the forces
  LJForce<FP_TYPE>::force_vector(particle1, particle2, computed_force_1_2);
  LJForce<FP_TYPE>::force_vector(particle1, particle3, computed_force_1_3);
  LJForce<FP_TYPE>::force_vector(particle1, particle4, computed_force_1_4);

  // compare force vectors
  BOOST_CHECK(correct_force_1_2 == computed_force_1_2);
  BOOST_CHECK(correct_force_1_3 == computed_force_1_3);
  BOOST_CHECK(correct_force_1_4 == computed_force_1_4);
}

BOOST_AUTO_TEST_CASE(apply_particle_container) {
  // create particles
  const auto particle1 = Particle3D<FP_TYPE>(0.0, 0.0, 0.0, real_dist(rng),
                                             real_dist(rng), real_dist(rng));
  const auto particle2 = Particle3D<FP_TYPE>(0.0, 1.0, 0.0, real_dist(rng),
                                             real_dist(rng), real_dist(rng));

  // put particles in container
  auto cont = ParticleContainer<FP_TYPE>(3);
  cont.insert(particle1);
  cont.insert(particle2);

  // apply force
  LJForce<FP_TYPE>::apply(cont);

  // expected forces
  const auto f_p1 = Vector3D<FP_TYPE>(0.0, 24.0, 0.0);
  const auto f_p2 = f_p1 * (-1.0);

  // check
  BOOST_CHECK(cont.get(0).get_current_force() == f_p1);
  BOOST_CHECK(cont.get(1).get_current_force() == f_p2);
}
