#pragma once

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

// computes the length of a 3-component vector with x,y,z
float vector_length(float x, float y, float z);
double vector_length(double x, double y, double z);
// returns the minimum of two std::size_t
std::size_t minimum(std::size_t x, std::size_t y);

// returns indices of a vector such that these indices would sort the vector
// the code uses the elegant answer of Timothy Shields on StackOverflow
// here:
// https://stackoverflow.com/questions/17074324/how-can-i-sort-two-vectors-in-the-same-way-with-criteria-that-uses-only-one-of
// answer was last accessed on 16th of September, 2024
std::vector<std::size_t> sort_permutation(
    const std::vector<unsigned long>& vec);

void apply_permutation_in_place(std::vector<double>& vec,
                                const std::vector<std::size_t>& p,
                                std::vector<bool>& done);

void apply_permutation_in_place(std::vector<unsigned long>& vec,
                                const std::vector<std::size_t>& p,
                                std::vector<bool>& done);

void reset_booleanvec_false(std::vector<bool>& done);
