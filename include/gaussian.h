#pragma once
#include <vector>
#include "matrix.h"

// Solves the linear system in an augmented n x (n + 1) matrix [A | b]
// using Gaussian elimination with partial pivoting, then back substitution.
// Returns x such that A·x = b.

std::vector<double> solve(Matrix augmented); 