#pragma once
#include "component.h"
 
// Each voltage source adds index k: column k is its unknown current
// (entering the KCL rows of its two nodes), and row k is its equation V(n+) - V(n-) = value.

class VoltageSource : public Component {
public:
    VoltageSource(std::string name, std::size_t n1, std::size_t n2, double v);
 
    void setup(const Matrix& matrix) override;
    void stamp(Matrix& matrix) const override;
 
    void setIndex(std::size_t k) { index = k; }
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
 