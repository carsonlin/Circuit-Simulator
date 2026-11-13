#include "voltagesource.h"
#include <utility>

VoltageSource::VoltageSource(std::string name, std::size_t n1, std::size_t n2, double v)
    : Component(std::move(name), n1, n2), voltage(v) {}

// Node 1 is the + terminal, node 2 is the - terminal.
// k is this source's extra matrix index
// column k is its unknown current, row k is its equation V(n1) - V(n2) = voltage.
// Node rows/columns are node ID - 1, since ground has no row.
void VoltageSource::setup(const Matrix& matrix) {
    const std::size_t cols = matrix.cols();
    const std::size_t node1_id = getNode1();
    const std::size_t node2_id = getNode2();
    const std::size_t k = index;

    if (node1_id != 0) {
        const std::size_t row1 = node1_id - 1;
        n1k = row1 * cols + k;   // source current in node 1's KCL row
        kn1 = k * cols + row1;   // V(n1) in the source's equation
    }
    if (node2_id != 0) {
        const std::size_t row2 = node2_id - 1;
        n2k = row2 * cols + k;   // source current in node 2's KCL row
        kn2 = k * cols + row2;   // V(n2) in the source's equation
    }
    krhs = k * cols + (cols - 1);   // right-hand side of the source's equation
}

// The unknown current I flows from node 1 into the source and out at node 2,
// so it leaves node 1 (+I in its KCL row) and enters node 2 (-I in its row).
void VoltageSource::stamp(Matrix& matrix) const {
    const std::size_t node1_id = getNode1();
    const std::size_t node2_id = getNode2();

    if (node1_id != 0) {
        matrix.flat(n1k) += 1.0;
        matrix.flat(kn1) += 1.0;
    }
    if (node2_id != 0) {
        matrix.flat(n2k) -= 1.0;
        matrix.flat(kn2) -= 1.0;
    }
    matrix.flat(krhs) += voltage;
}