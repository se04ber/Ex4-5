#define BOOST_TEST_MODULE TestSoAForce
#include <boost/test/included/unit_test.hpp>  //single-header
#include <cmath>
#include <random>
#include <vector>

#include "../lib/forces.hpp"
#include "../lib/linkedcellcontainer.hpp"
#include "../lib/soacontainer.hpp"
#include "../lib/vector3d.hpp"

typedef double FP_TYPE;
std::mt19937 rng;
std::uniform_real_distribution<FP_TYPE> real_dist(0.1, 4.0);

BOOST_AUTO_TEST_CASE(LJ_force_same_cell) {
  rng.seed(42);
  FP_TYPE cutoff = 5.0;
  // loop over some cells
  for (auto rep_idx = 0; rep_idx < 10; ++rep_idx) {
    // create container
    auto cont = LinkedCellContainer<FP_TYPE>(cutoff, 10, 10, 10, 2);
    // randomly choose a cell
    std::uniform_int_distribution<std::size_t> int_dist(0, 9);
    const auto idx = cont.get_idx(int_dist(rng), int_dist(rng), int_dist(rng));
    // insert into that cell
    cont.insert_particle(idx, 0.0, 0.0, 0.0, real_dist(rng), real_dist(rng),
                         real_dist(rng), 0.0, 0.0, 0.0);
    cont.insert_particle(idx, 0.0, 1.0, 0.0, real_dist(rng), real_dist(rng),
                         real_dist(rng), 0.0, 0.0, 0.0);
    // apply the force
    LJForce<FP_TYPE>::apply(cont);
    // check
    BOOST_CHECK(abs(cont.get(idx).get_Fx(0)) < 1e-12);
    BOOST_CHECK(abs(cont.get(idx).get_Fy(0) - 24.0) < 1e-12);
    BOOST_CHECK(abs(cont.get(idx).get_Fz(0)) < 1e-12);
    BOOST_CHECK(abs(cont.get(idx).get_Fx(1)) < 1e-12);
    BOOST_CHECK(abs(cont.get(idx).get_Fy(1) + 24.0) < 1e-12);
    BOOST_CHECK(abs(cont.get(idx).get_Fz(1)) < 1e-12);
  }
}

BOOST_AUTO_TEST_CASE(LJ_force_neighboring_cell) {
  rng.seed(42);
  FP_TYPE cutoff = 5.0;
  // loop over some cells
  for (auto rep_idx = 0; rep_idx < 10; ++rep_idx) {
    // create container
    auto cont = LinkedCellContainer<FP_TYPE>(cutoff, 5, 5, 5, 2);
    // randomly choose two cells
    std::uniform_int_distribution<std::size_t> int_dist(
        0, 4);  // all allowed indices
    const auto x_cell1 = int_dist(rng);
    const auto x_cell2 = int_dist(rng);
    const auto y_cell1 = int_dist(rng);
    const auto y_cell2 = int_dist(rng);
    const auto z_cell1 = int_dist(rng);
    const auto z_cell2 = int_dist(rng);
    const auto idx1 = cont.get_idx(x_cell1, y_cell1, z_cell1);
    const auto idx2 = cont.get_idx(x_cell2, y_cell2, z_cell2);
    // insert a particle into each of these cells
    cont.insert_particle(idx1, 0.0, 0.0, 0.0, real_dist(rng), real_dist(rng),
                         real_dist(rng), 0.0, 0.0, 0.0);
    cont.insert_particle(idx2, 0.0, 1.0, 0.0, real_dist(rng), real_dist(rng),
                         real_dist(rng), 0.0, 0.0, 0.0);
    // apply the force
    LJForce<FP_TYPE>::apply(cont);

    if ((abs(static_cast<int>(x_cell1) - static_cast<int>(x_cell2)) <= 1) &&
        (abs(static_cast<int>(y_cell1) - static_cast<int>(y_cell2)) <= 1) &&
        (abs(static_cast<int>(z_cell1) - static_cast<int>(z_cell2)) <= 1) &&
        (idx1 != idx2)) {
      // if the two cells were neighboring, forces should have been updated
      BOOST_CHECK(abs(cont.get(idx1).get_Fx(0)) < 1e-10);
      BOOST_CHECK(abs(cont.get(idx1).get_Fy(0) - 24.0) < 1e-10);
      BOOST_CHECK(abs(cont.get(idx1).get_Fz(0)) < 1e-10);
      BOOST_CHECK(abs(cont.get(idx2).get_Fx(0)) < 1e-10);
      BOOST_CHECK(abs(cont.get(idx2).get_Fy(0) + 24.0) < 1e-10);
      BOOST_CHECK(abs(cont.get(idx2).get_Fz(0)) < 1e-10);
    } else if (idx1 != idx2) {
      // if the forces were not neighboring, forces should be left at zero
      BOOST_CHECK(cont.get(idx1).get_Fx(0) == 0.0);
      BOOST_CHECK(cont.get(idx1).get_Fy(0) == 0.0);
      BOOST_CHECK(cont.get(idx1).get_Fz(0) == 0.0);
      BOOST_CHECK(cont.get(idx2).get_Fx(0) == 0.0);
      BOOST_CHECK(cont.get(idx2).get_Fy(0) == 0.0);
      BOOST_CHECK(cont.get(idx2).get_Fz(0) == 0.0);
    }
  }
}
