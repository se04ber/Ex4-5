#define BOOST_TEST_MODULE TestLinkedCellContainer
#include <boost/test/included/unit_test.hpp>  //single-header
#include <random>
#include <vector>

#include "../lib/linkedcellcontainer.hpp"
#include "../lib/soacontainer.hpp"
#include "../lib/vector3d.hpp"

typedef double FP_TYPE;
std::mt19937 rng;
std::uniform_int_distribution<std::size_t> int_dist(2, 10);
std::uniform_real_distribution<FP_TYPE> real_dist(0.1, 4.0);

BOOST_AUTO_TEST_CASE(constructor_sizes) {
  rng.seed(42);
  // some repetitions
  for (std::size_t idx = 0; idx <= 5; ++idx) {
    // samples some container sizes and cutoff
    std::size_t n_x = int_dist(rng);
    std::size_t n_y = int_dist(rng);
    std::size_t n_z = int_dist(rng);
    std::size_t default_size = int_dist(rng);
    FP_TYPE cutoff = real_dist(rng);
    // construct corresponding linked cell container
    auto cont =
        LinkedCellContainer<FP_TYPE>(cutoff, n_x, n_y, n_z, default_size);
    // check member variables are set correctly
    BOOST_CHECK(cont.get_size_x() == n_x);
    BOOST_CHECK(cont.get_size_y() == n_y);
    BOOST_CHECK(cont.get_size_z() == n_z);
    // number of SoAContainers is correct
    BOOST_CHECK(cont.num_containers() == n_x * n_y * n_z);
    // and check that the cells have the correct sizes
    for (std::size_t c_idx = 0; c_idx < cont.num_containers(); ++c_idx) {
      BOOST_CHECK(cont.get(c_idx).capacity() == default_size);
    }
  }
}

BOOST_AUTO_TEST_CASE(index_computation) {
  rng.seed(42);
  const auto cont = LinkedCellContainer<FP_TYPE>(5.0, 11, 12, 13, 1);
  // some examples for cell position to vector indices
  BOOST_CHECK(cont.get_idx(1, 3, 6) == 826);
  BOOST_CHECK(cont.get_idx(0, 1, 7) == 935);
  BOOST_CHECK(cont.get_idx(0, 0, 0) == 0);
  // and test inversion
  std::size_t x_temp, y_temp, z_temp, vector_idx;
  for (auto idx = 0; idx < 10; ++idx) {
    // draw some cell numbers (we don't care about out-of-bounds here)
    x_temp = int_dist(rng);
    y_temp = int_dist(rng);
    z_temp = int_dist(rng);
    vector_idx = cont.get_idx(x_temp, y_temp, z_temp);
    Vector3D<std::size_t> coord_vec = cont.idx_to_cellnum(vector_idx);
    BOOST_CHECK(coord_vec.get(0) == x_temp);
    BOOST_CHECK(coord_vec.get(1) == y_temp);
    BOOST_CHECK(coord_vec.get(2) == z_temp);
  }
}

BOOST_AUTO_TEST_CASE(insert_particles_idx) {
  rng.seed(42);
  FP_TYPE cutoff = 5.0;
  auto cont = LinkedCellContainer<FP_TYPE>(cutoff, 3, 5, 2, 1);
  // generate random index
  std::size_t idx = int_dist(rng);
  // and insert into that
  cont.insert_particle(idx, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  // the randomly drawn soa container
  const auto& soacontainer = cont.get(idx);
  // check that it is not empty
  BOOST_CHECK(soacontainer.size() == 1);
}

BOOST_AUTO_TEST_CASE(num_particles) {
  rng.seed(42);
  FP_TYPE cutoff = 5.0;
  // create LinkedCellContainer of empty SoAContainers
  auto cont = LinkedCellContainer<FP_TYPE>(cutoff, 3, 5, 2, 1);
  // distribution for container indices
  std::uniform_int_distribution<std::size_t> test_dist(
      0, cont.num_containers() - 1);
  // expected number of particles
  std::size_t num_p = 0;
  std::size_t container_idx, n_insert;
  // some repetitions
  for (auto rep_idx = 0; rep_idx < 10; ++rep_idx) {
    BOOST_CHECK(cont.size() == num_p);
    // draw a number of particles and an index to insert into
    n_insert = test_dist(rng);
    container_idx = test_dist(rng);
    for (std::size_t particle_idx = 0; particle_idx < n_insert;
         ++particle_idx) {
      cont.insert_particle(container_idx, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1,
                           0.2, 0.3);
    }
    num_p += n_insert;
  }
}

BOOST_AUTO_TEST_CASE(lies_within) {
  // an example container
  auto cont = LinkedCellContainer<FP_TYPE>(
      1.0, 3, 5, 2, Vector3D<FP_TYPE>(-1.1, 0.0, 1.2), 1);
  // and a few representative examples in the container
  BOOST_CHECK(cont.lies_within(-1.09, 0.01, 1.21));
  BOOST_CHECK(cont.lies_within(1.8, 4.9, 3.19));
  // outside of the container (lower bound)
  BOOST_CHECK(!cont.lies_within(-1.2, 0.01, 1.21));
  BOOST_CHECK(!cont.lies_within(-1.0, -0.1, 1.21));
  BOOST_CHECK(!cont.lies_within(-1.0, 0.01, 1.19));
  // outside of the container (upper bound)
  BOOST_CHECK(!cont.lies_within(2.0, 0.1, 1.3));
  BOOST_CHECK(!cont.lies_within(-1, 5.1, 1.3));
  BOOST_CHECK(!cont.lies_within(-1, 0.1, 3.3));
  // outside of the container (on the boundary)
  BOOST_CHECK(!cont.lies_within(-1.1, 0.1, 1.3));
  BOOST_CHECK(!cont.lies_within(-1, 0.0, 1.3));
  BOOST_CHECK(!cont.lies_within(-1, 0.1, 1.2));
}

BOOST_AUTO_TEST_CASE(spatial_index) {
  rng.seed(42);
  FP_TYPE cutoff = 5.0;
  const Vector3D<FP_TYPE> offset =
      Vector3D<FP_TYPE>(real_dist(rng), real_dist(rng), real_dist(rng));
  const auto cont = LinkedCellContainer<FP_TYPE>(cutoff, 3, 5, 2, offset, 1);
  // some repetitions
  for (auto rep_idx = 0; rep_idx < 10; ++rep_idx) {
    // we should start at 0
    BOOST_CHECK(cont.get_container_index(offset.get(0) + real_dist(rng),
                                         offset.get(1) + real_dist(rng),
                                         offset.get(2) + real_dist(rng)) == 0);
    // some simple examples
    BOOST_CHECK(
        cont.get_container_index(offset.get(0) + cutoff + real_dist(rng),
                                 offset.get(1) + 2.0 * cutoff + real_dist(rng),
                                 offset.get(2) + real_dist(rng)) == 7);
    BOOST_CHECK(cont.get_container_index(
                    offset.get(0) + cutoff + real_dist(rng),
                    offset.get(1) + cutoff + real_dist(rng),
                    offset.get(2) + 3.0 * cutoff + real_dist(rng)) == 49);
  }
}

BOOST_AUTO_TEST_CASE(insert_particles_position) {
  rng.seed(42);
  FP_TYPE cutoff = 5.0;
  const Vector3D<FP_TYPE> offset =
      Vector3D<FP_TYPE>(real_dist(rng), real_dist(rng), real_dist(rng));
  auto cont = LinkedCellContainer<FP_TYPE>(cutoff, 3, 5, 2, offset, 1);
  // generate particle
  FP_TYPE x, y, z;
  x = offset.get(0) + 2.0 * cutoff + real_dist(rng);
  y = offset.get(1) + cutoff + real_dist(rng);
  z = offset.get(2) + cutoff + real_dist(rng);
  // and insert
  cont.insert_particle(x, y, z, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  // the soa container
  const auto& soacontainer = cont.get(20);
  BOOST_CHECK(soacontainer.size() == 1);
}

BOOST_AUTO_TEST_CASE(resort) {
  rng.seed(42);
  FP_TYPE cutoff = 5.0;
  // zero offset
  auto cont = LinkedCellContainer<FP_TYPE>(cutoff, 3, 5, 2, 1);
  // insert particle in incorrect containers
  // should be in first container
  cont.insert_particle(12, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  // should be in container 7
  cont.insert_particle(0, cutoff + real_dist(rng),
                       2.0 * cutoff + real_dist(rng), real_dist(rng), 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.0);
  // resort
  cont.resort();
  // check that correct containers are populated
  BOOST_CHECK(cont.get(0).size() == 1);
  BOOST_CHECK(cont.get(7).size() == 1);
  BOOST_CHECK(cont.get(12).size() == 0);
  // and that the right partcicles are in there
  BOOST_CHECK(cont.get(0).get_x(0) == 1.0);
  BOOST_CHECK(cont.get(7).get_x(0) > cutoff);
}

BOOST_AUTO_TEST_CASE(flush_force) {
  rng.seed(42);
  FP_TYPE cutoff = 5.0;
  // zero offset
  auto cont = LinkedCellContainer<FP_TYPE>(cutoff, 3, 5, 2, 1);
  // insert particle in incorrect containers
  // should be in first container
  cont.insert_particle(12, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.2, 0.3);
  // should be in container 7
  cont.insert_particle(0, cutoff + real_dist(rng),
                       2.0 * cutoff + real_dist(rng), real_dist(rng), 0.0, 0.0,
                       0.0, 1.0, 2.0, 3.0);
  // flush (and resort)
  cont.flush_forces();
  // first particle
  BOOST_CHECK(cont.get(0).get_Fx(0) == 0.0);
  BOOST_CHECK(cont.get(0).get_Fy(0) == 0.0);
  BOOST_CHECK(cont.get(0).get_Fz(0) == 0.0);
  BOOST_CHECK(cont.get(0).get_F_old_x(0) == 0.1);
  BOOST_CHECK(cont.get(0).get_F_old_y(0) == 0.2);
  BOOST_CHECK(cont.get(0).get_F_old_z(0) == 0.3);
  // second particle
  BOOST_CHECK(cont.get(7).get_Fx(0) == 0.0);
  BOOST_CHECK(cont.get(7).get_Fy(0) == 0.0);
  BOOST_CHECK(cont.get(7).get_Fz(0) == 0.0);
  BOOST_CHECK(cont.get(7).get_F_old_x(0) == 1.0);
  BOOST_CHECK(cont.get(7).get_F_old_y(0) == 2.0);
  BOOST_CHECK(cont.get(7).get_F_old_z(0) == 3.0);
}
