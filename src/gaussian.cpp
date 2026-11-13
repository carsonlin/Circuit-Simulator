#include "gaussian.h"
#include <cmath>
#include <stdexcept>
#include <string>


// Pivots smaller than this are treated as zero.
constexpr double threshold = 1e-12;

std::vector<double> solve(Matrix augmented){

    if(augmented.cols() != augmented.rows() + 1){
        throw std::runtime_error("System invalid, matrix must be of dimensions n x (n + 1) ");
    }

    // Forward elimination: reduce to upper-triangular form, one column at a time.
    for(std::size_t diag = 0; diag < augmented.rows(); ++diag){

        // Partial pivoting: swap up the row with the largest value in this column.
        // Dividing by a tiny pivot magnifies rounding errors; the largest one minimizes that.
        std::size_t pivot = diag;
        for(std::size_t row = diag + 1; row < augmented.rows(); ++row){
            if(std::abs(augmented(row, diag)) > std::abs(augmented(pivot, diag))){
                pivot = row;
            }
        }
        if (std::abs(augmented(pivot, diag)) < threshold) {
            throw std::runtime_error("System unsolvable, no valid pivot in column " + std::to_string(diag));
        }
        augmented.swapRows(diag, pivot);

        // Subtract a multiple of the pivot row from each row below, to zero this column.
        double pivot_value = augmented(diag, diag);
        for(std::size_t row = diag + 1; row < augmented.rows(); ++row){
            double ratio = augmented(row, diag) / pivot_value;

            // Set exactly to 0 instead of computing it, which would leave rounding noise.
            // Columns left of diag are already 0 in both rows, so start at diag + 1.
            augmented(row, diag) = 0.0;
            for(std::size_t col = diag + 1; col < augmented.cols(); ++col){
                augmented(row, col) -= (augmented(diag, col) * ratio);
            }
        }
    }

    // Back substitution: solve from the last row up, since each row only
    // depends on unknowns below it.
    std::vector<double> solved_vector(augmented.rows());

    // Counting down with an unsigned index: i-- > 0 tests before decrementing,
    // so the body runs for rows-1 down to 0 and never wraps around.
    for(std::size_t i = augmented.rows(); i-- > 0;){
        double solution = augmented(i, augmented.cols() - 1);
        for(std::size_t j = i + 1; j < augmented.cols() - 1; ++j){
            solution -= solved_vector[j] * augmented(i, j);
        }
        solved_vector[i] = solution / augmented(i, i);
    }

    return solved_vector;
}