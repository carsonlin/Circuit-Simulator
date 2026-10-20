#pragma once
#include <cstddef>
#include "component.h"
 
// node1 = positive terminal, node2 = negative terminal: V(node1) - V(node2) = voltage.
// The source's current is an extra unknown at its row and column k in the matrix

class VoltageSource : public Component {
public:
    VoltageSource(std::size_t n1, std::size_t n2, double v);
 
    void setup(const Matrix& matrix) override;
    void stamp(Matrix& matrix) const override;
 
    void setBranchIndex(std::size_t k) { index = k; }
    std::size_t getBranchIndex() const { return index; }
    double getVoltage() const { return voltage; }
 
private:
    double voltage;
    std::size_t index = 0;   // matrix row/column of this source's current (k)
 
    std::size_t n1k = 0;    // row of node1, column k:  +1
    std::size_t kn1 = 0;    // row k, column of node1:  +1
    std::size_t n2k = 0;    // row of node2, column k:  -1
    std::size_t kn2 = 0;    // row k, column of node2:  -1
    std::size_t krhs = 0;   // row k, RHS column:        +voltage
};
 