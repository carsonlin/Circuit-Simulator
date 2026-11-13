#include "resistor.h"
#include <stdexcept>
#include <utility>

Resistor::Resistor(std::string name, std::size_t n1, std::size_t n2, double r, double tol)
    : Component(std::move(name), n1, n2),
      target_resistance(r), actual_resistance(r), tolerance(tol) {
    if (r <= 0.0) {
        throw std::invalid_argument("Resistance must be positive");
    }
    // Tolerance is a fraction. 1.0 or more would allow zero or negative resistance.
    if (tol < 0.0 || tol >= 1.0) {
        throw std::invalid_argument("Tolerance must be in the range [0, 1)");
    }
}

// Matrix row/column for node ID k is k - 1, since ground (ID 0) has no row.
// Only entries for non-ground nodes get an index
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
    // Off-diagonal entries only exist if both nodes are in the matrix.
    if (node1_id != 0 && node2_id != 0) {
        const std::size_t row1 = node1_id - 1;
        const std::size_t row2 = node2_id - 1;
        n1n2 = row1 * cols + row2;
        n2n1 = row2 * cols + row1;
    }
}

// From KCL: the current leaving node 1 through this resistor is g·(v1 - v2).
// So node 1's row gets +g at v1 and -g at v2, and node 2's row gets the mirror image.
void Resistor::stamp(Matrix& matrix) const {
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