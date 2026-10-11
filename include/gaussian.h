#pragma once
#include <vector>
#include "Matrix.h"

// Solves a system: n rows, n+1 columns (last column = right-hand side)

std::vector<double> solve(Matrix augmented);