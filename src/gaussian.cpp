#include "gaussian.h"
#include <cmath>
#include <stdexcept>
#include <string>

constexpr double threshold = 1e-12;

std::vector<double> solve(Matrix augmented){

    if(augmented.cols() != augmented.rows() + 1){
        throw std::runtime_error("System invalid, matrix must be n x (n + 1) ");
    }

    //iterate diagonal
    for(std::size_t diag = 0; diag < augmented.rows(); ++diag){
        std::size_t pivot = diag;

        //find largest pivot to swap
        for(std::size_t row = diag + 1; row < augmented.rows(); ++row){
            if(std::abs(augmented(row, diag)) > std::abs(augmented(pivot, diag))){
                pivot = row;
            }
        }
        if (std::abs(augmented(pivot, diag)) < threshold) {
            throw std::runtime_error("System unsolvable, no valid pivot in column " + std::to_string(diag));
        }
        augmented.swapRows(diag, pivot);

        //zeroing column underneath pivot
        double pivot_value = augmented(diag, diag);
        for(std::size_t row = diag + 1; row < augmented.rows(); ++row){
            
            double ratio = augmented(row, diag) / pivot_value;

            augmented(row, diag) = 0.0;
            for(std::size_t col = diag + 1; col < augmented.cols(); ++col){
                augmented(row, col) -= (augmented(diag, col) * ratio);
            }
        }

        //back substitution
        

    }
}

