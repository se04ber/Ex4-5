#define BOOST_TEST_MODULE TestVector3D
#include <boost/test/included/unit_test.hpp>  //single-header
#include <random>

#include "../lib/vector3d.hpp"
typedef double FP_TYPE;

std::mt19937 rng;
std::uniform_real_distribution<FP_TYPE> uniform_dist(-10, 10);

BOOST_AUTO_TEST_CASE(constructor_and_getters) {
  rng.seed(42);
  FP_TYPE x;
  FP_TYPE y;
  FP_TYPE z;
  // some repetitions due do randomness
  for (int i = 0; i < 10; ++i) {
    // define some random values
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    // create a new vector with these values
    auto test_vec = Vector3D<FP_TYPE>(x, y, z);
    // and check that the getter returns the correct value
    BOOST_CHECK(test_vec.get(0) == x);
    BOOST_CHECK(test_vec.get(1) == y);
    BOOST_CHECK(test_vec.get(2) == z);
  }
}

BOOST_AUTO_TEST_CASE(getters_and_setters) {
  rng.seed(42);
  // create a vector
  auto test_vec = Vector3D<FP_TYPE>(0.0, 0.0, 0.0);
  FP_TYPE x;
  FP_TYPE y;
  FP_TYPE z;
  // some repetitions due do randomness
  for (int i = 0; i < 10; ++i) {
    // random x,y,z values
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    // test getter and setter pairs
    test_vec.set(0, x);
    BOOST_CHECK(test_vec.get(0) == x);
    test_vec.set(1, y);
    BOOST_CHECK(test_vec.get(1) == y);
    test_vec.set(2, z);
    BOOST_CHECK(test_vec.get(2) == z);
  }
}

BOOST_AUTO_TEST_CASE(vector_length) {
  // a few representative example vectors
  const auto zero_vec = Vector3D<FP_TYPE>(0.0, 0.0, 0.0);
  const auto unit_vec = Vector3D<FP_TYPE>(0.0, 1.0, 0.0);
  const auto other_vec = Vector3D<FP_TYPE>(0.0, 3.0, 4.0);
  // check against manually computed length
  BOOST_CHECK(zero_vec.length() == 0.0);
  BOOST_CHECK(unit_vec.length() == 1.0);
  BOOST_CHECK(other_vec.length() == 5.0);
}

BOOST_AUTO_TEST_CASE(inplace_copy) {
  rng.seed(42);
  // create two vectors for testing
  auto test_vec = Vector3D<FP_TYPE>(0.0, 0.0, 0.0);
  auto new_vec = Vector3D<FP_TYPE>(0.0, 0.0, 0.0);
  FP_TYPE x;
  FP_TYPE y;
  FP_TYPE z;
  // some repetitions due do randomness
  for (int i = 0; i < 10; ++i) {
    // sample new x,y,z values for one of the vectors and fill them in
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    new_vec.set(0, x);
    new_vec.set(1, y);
    new_vec.set(2, z);
    // assign to test vector
    test_vec = new_vec;
    // reset values of the other vectors
    new_vec.set(0, 0.0);
    new_vec.set(1, 0.0);
    new_vec.set(2, 0.0);
    // and check that test vector still has the correct values
    BOOST_CHECK(test_vec.get(0) == x);
    BOOST_CHECK(test_vec.get(1) == y);
    BOOST_CHECK(test_vec.get(2) == z);
  }
}

BOOST_AUTO_TEST_CASE(inplace_addition) {
  rng.seed(42);
  FP_TYPE x;
  FP_TYPE y;
  FP_TYPE z;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // create a zero-initialized vector
    auto test_vec = Vector3D<FP_TYPE>(0.0, 0.0, 0.0);
    // sample some values for the vector to add
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    auto new_vec = Vector3D<FP_TYPE>(x, y, z);
    // inplace addition
    test_vec += new_vec;
    // reset values of RHS vector
    new_vec.set(0, 0.0);
    new_vec.set(1, 0.0);
    new_vec.set(2, 0.0);
    // and check that values are correct
    BOOST_CHECK(test_vec.get(0) == x);
    BOOST_CHECK(test_vec.get(1) == y);
    BOOST_CHECK(test_vec.get(2) == z);
  }
}

BOOST_AUTO_TEST_CASE(inplace_subtraction) {
  // same test concept as for inplace_addition
  rng.seed(42);
  FP_TYPE x;
  FP_TYPE y;
  FP_TYPE z;
  for (int i = 0; i < 10; ++i) {
    auto test_vec = Vector3D<FP_TYPE>(0.0, 0.0, 0.0);
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    auto new_vec = Vector3D<FP_TYPE>(x, y, z);
    test_vec -= new_vec;
    new_vec.set(0, 0.0);
    new_vec.set(1, 0.0);
    new_vec.set(2, 0.0);
    BOOST_CHECK(test_vec.get(0) == -1.0 * x);
    BOOST_CHECK(test_vec.get(1) == -1.0 * y);
    BOOST_CHECK(test_vec.get(2) == -1.0 * z);
  }
}

BOOST_AUTO_TEST_CASE(scalar_multiplication_inplace) {
  rng.seed(42);
  FP_TYPE x;
  FP_TYPE y;
  FP_TYPE z;
  FP_TYPE factor;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // create random vector
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    auto test_vec = Vector3D<FP_TYPE>(x, y, z);
    // sample random factor
    factor = uniform_dist(rng);
    // inplace multiplication
    test_vec *= factor;
    // check elements
    BOOST_CHECK(test_vec.get(0) == x * factor);
    BOOST_CHECK(test_vec.get(1) == y * factor);
    BOOST_CHECK(test_vec.get(2) == z * factor);
  }
}

BOOST_AUTO_TEST_CASE(scalar_multiplication_copy) {
  rng.seed(42);
  FP_TYPE x;
  FP_TYPE y;
  FP_TYPE z;
  FP_TYPE factor;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // same random vector
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    auto test_vec = Vector3D<FP_TYPE>(x, y, z);
    // sample random factor
    factor = uniform_dist(rng);
    // component-wise multiplication
    auto result_vec = test_vec * factor;
    test_vec *= factor;
    // check elements
    BOOST_CHECK(test_vec.get(0) == result_vec.get(0));
    BOOST_CHECK(test_vec.get(1) == result_vec.get(1));
    BOOST_CHECK(test_vec.get(2) == result_vec.get(2));
  }
}

BOOST_AUTO_TEST_CASE(vector_subtraction_copy) {
  rng.seed(42);
  FP_TYPE x1;
  FP_TYPE y1;
  FP_TYPE z1;
  FP_TYPE x2;
  FP_TYPE y2;
  FP_TYPE z2;
  for (int i = 0; i < 10; ++i) {
    // sample two random vectors
    x1 = uniform_dist(rng);
    x2 = uniform_dist(rng);
    y1 = uniform_dist(rng);
    y2 = uniform_dist(rng);
    z1 = uniform_dist(rng);
    z2 = uniform_dist(rng);
    auto test_vec = Vector3D<FP_TYPE>(x1, y1, z1);
    auto other = Vector3D<FP_TYPE>(x2, y2, z2);
    // compute difference (using - and -= operator)
    auto result_vec = test_vec - other;
    test_vec -= other;
    // and check results
    BOOST_CHECK(test_vec.get(0) == result_vec.get(0));
    BOOST_CHECK(test_vec.get(1) == result_vec.get(1));
    BOOST_CHECK(test_vec.get(2) == result_vec.get(2));
  }
}

BOOST_AUTO_TEST_CASE(test_set_zero) {
  rng.seed(42);
  FP_TYPE x;
  FP_TYPE y;
  FP_TYPE z;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // create random vector
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    auto test_vec = Vector3D<FP_TYPE>(x, y, z);
    // set to zero and check values
    test_vec.set_zero();
    BOOST_CHECK(test_vec.get(0) == 0.0);
    BOOST_CHECK(test_vec.get(1) == 0.0);
    BOOST_CHECK(test_vec.get(2) == 0.0);
  }
}

BOOST_AUTO_TEST_CASE(compare_vectors) {
  rng.seed(42);
  FP_TYPE x;
  FP_TYPE y;
  FP_TYPE z;
  // some repetitions
  for (int i = 0; i < 10; ++i) {
    // create some random elements
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    // create two vectors with the same values
    const auto test_vec1 = Vector3D<double>(x, y, z);
    const auto test_vec2 = Vector3D<double>(x, y, z);
    // create three different vectors (one per component)
    const auto test_vec3 = Vector3D<double>(x + 1.0, y, z);
    const auto test_vec4 = Vector3D<double>(x, y + 1.0, z);
    const auto test_vec5 = Vector3D<double>(x, y, z + 1.0);
    // vectors with same elements should be equal
    BOOST_CHECK(test_vec1 == test_vec2);
    BOOST_CHECK(test_vec1 == test_vec1);
    // vectors with at least one unqual component should be unequal
    BOOST_CHECK(!(test_vec1 == test_vec3));
    BOOST_CHECK(!(test_vec1 == test_vec4));
    BOOST_CHECK(!(test_vec1 == test_vec5));
  }
}

BOOST_AUTO_TEST_CASE(test_compare_vectors_tol) {
  rng.seed(42);
  FP_TYPE x;
  FP_TYPE y;
  FP_TYPE z;
  for (int i = 0; i < 10; ++i) {
    // sample a random vector, as well as
    // some vectors with small component-wise differences
    x = uniform_dist(rng);
    y = uniform_dist(rng);
    z = uniform_dist(rng);
    const auto test_vec1 = Vector3D<double>(x, y, z);
    const auto test_vec2 = Vector3D<double>(x + (double)5.0, y, z);
    const auto test_vec3 = Vector3D<double>(x, y + (double)5.0, z);
    const auto test_vec4 = Vector3D<double>(x, y, z + (double)5.0);
    // approx() should be true if difference is smaller than
    // specified tolerance
    BOOST_CHECK(test_vec1.approx(test_vec2, 6.0));
    BOOST_CHECK(test_vec1.approx(test_vec3, 6.0));
    BOOST_CHECK(test_vec1.approx(test_vec4, 6.0));
    // and vice versa
    BOOST_CHECK(!test_vec1.approx(test_vec2, 4.0));
    BOOST_CHECK(!test_vec1.approx(test_vec3, 4.0));
    BOOST_CHECK(!test_vec1.approx(test_vec4, 4.0));
  }
}
