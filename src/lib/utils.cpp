
#include "utils.hpp"

#include <cmath>
float vector_length(float x, float y, float z) {
  return sqrt(x * x + y * y + z * z);
};

double vector_length(double x, double y, double z) {
  return sqrt(x * x + y * y + z * z);
};

std::size_t minimum(std::size_t x, std::size_t y) {
  if (x <= y) {
    return x;
  } else {
    return y;
  }
}

// returns indices of a vector such that these indices would sort the vector
// the code uses the elegant answer of Timothy Shields on StackOverflow
// here:
// https://stackoverflow.com/questions/17074324/how-can-i-sort-two-vectors-in-the-same-way-with-criteria-that-uses-only-one-of
// answer was last accessed on 16th of September, 2024
std::vector<std::size_t> sort_permutation(
    const std::vector<unsigned long>& vec) {
  std::vector<std::size_t> p(vec.size());
  std::iota(p.begin(), p.end(), 0);
  std::sort(p.begin(), p.end(),
            [&](std::size_t i, std::size_t j) { return vec[i] < vec[j]; });
  return p;
}

// applies a given permutation in vector p to vector vec,
// using a vector of boolean as a temp variable;
// again, this uses the code from the same stackoverflow post as before
void apply_permutation_in_place(std::vector<double>& vec,
                                const std::vector<std::size_t>& p,
                                std::vector<bool>& done) {
  for (std::size_t i = 0; i < vec.size(); ++i) {
    if (done[i]) {
      continue;
    }
    done[i] = true;
    std::size_t prev_j = i;
    std::size_t j = p[i];
    while (i != j) {
      std::swap(vec[prev_j], vec[j]);
      done[j] = true;
      prev_j = j;
      j = p[j];
    }
  }
  reset_booleanvec_false(done);
}

void apply_permutation_in_place(std::vector<unsigned long>& vec,
                                const std::vector<std::size_t>& p,
                                std::vector<bool>& done) {
  for (std::size_t i = 0; i < vec.size(); ++i) {
    if (done[i]) {
      continue;
    }
    done[i] = true;
    std::size_t prev_j = i;
    std::size_t j = p[i];
    while (i != j) {
      std::swap(vec[prev_j], vec[j]);
      done[j] = true;
      prev_j = j;
      j = p[j];
    }
  }
  reset_booleanvec_false(done);
}

void reset_booleanvec_false(std::vector<bool>& done) {
  for (auto b : done) {
    b = false;
  }
}
