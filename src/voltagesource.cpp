#include "voltagesource.h"

VoltageSource::VoltageSource(std::size_t n1, std::size_t n2, double v)
    : Component(n1, n2), voltage(v) {}


void VoltageSource::setup(const Matrix& matrix) {
    const std::size_t cols = matrix.cols();
    const std::size_t node1_id = getNode1();
    const std::size_t node2_id = getNode2();
    const std::size_t k = index;

    if (node1_id != 0) {
        const std::size_t row1 = node1_id - 1;
        n1k = row1 * cols + k;
        kn1 = k * cols + row1;
    }
    if (node2_id != 0) {
        const std::size_t row2 = node2_id - 1;
        n2k = row2 * cols + k;
        kn2 = k * cols + row2;
    }
    krhs = k * cols + (cols - 1);
}

void VoltageSource::stamp(Matrix& matrix) const {
    const std::size_t node1_id = getNode1();
    const std::size_t node2_id = getNode2();

    if (node1_id != 0) {
        matrix.flat(n1k)+=1.0;
        matrix.flat(kn1)+=1.0;
    }
    if (node2_id != 0) {
        matrix.flat(n2k)-=1.0;
        matrix.flat(kn2)-=1.0;
    }
    matrix.flat(krhs)+=voltage;
}
