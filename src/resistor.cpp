#include "resistor.h"
#include <stdexcept>

Resistor::Resistor(std::size_t n1, std::size_t n2, double r, double tol): Component(n1,n2), target_resistance(r), tolerance(tol), actual_resistance(r){
    if (r <= 0.0) {
        throw std::invalid_argument("Resistance must be positive");
    }

    if (tol < 0.0 || tol >= 1.0) {
        throw std::invalid_argument("Tolerance must be in the range [0, 1)");
    }
}

void Resistor::setup(const Matrix& matrix) {
    const std::size_t cols = matrix.cols();
    const std::size_t node1_id = getNode1();
    const std::size_t node2_id = getNode2();

    if (node1_id != 0) {
        const std::size_t row1 = node1_id - 1;
        n1n1 = row1 * cols + row1;
    }
    if (node2_id != 0) {
        const std::size_t row2 = node2_id - 1;
        n2n2 = row2 * cols + row2;
    }
    if (node1_id != 0 && node2_id != 0) {
        const std::size_t row1 = node1_id - 1;
        const std::size_t row2 = node2_id - 1;
        n1n2 = row1 * cols + row2;
        n2n1 = row2 * cols + row1;
    }
}

void Resistor::stamp(Matrix& matrix) const{
    const std::size_t node1_id = getNode1();
    const std::size_t node2_id = getNode2();
    const double cond = 1.0 / actual_resistance;

    if (node1_id != 0) {
        matrix.flat(n1n1) += cond;
    }
    if (node2_id != 0) {
        matrix.flat(n2n2) += cond;
    }
    if (node1_id != 0 && node2_id != 0) {
        matrix.flat(n1n2) -= cond;
        matrix.flat(n2n1) -= cond;
    }
    
}



