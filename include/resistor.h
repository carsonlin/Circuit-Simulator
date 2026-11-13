#pragma once

#include "component.h"
#include <cstddef>
#include <string>

// resistor between two nodes
// Holds a target value and an actual value. The actual value is
// what gets stamped. For Monte Carlo it can vary within ±tolerance of the target.
class Resistor : public Component {
public:
    // r is in ohms and must be positive. tol is a fraction (0.05 = ±5%).
    // The actual resistance starts equal to r.
    Resistor(std::string name, std::size_t n1, std::size_t n2, double r, double tol = 0.0);

    // Adds conductance 1/R_actual: +g on each node's diagonal, -g on the off-diagonals.
    void stamp(Matrix& matrix) const override;

    // Stores the flat indices of this resistor's matrix entries.
    void setup(const Matrix& matrix) override;

    double getTargetResistance() const { return target_resistance; }
    double getActualResistance() const { return actual_resistance; }
    double getTolerance() const { return tolerance; }

    void setActualResistance(double resistance) { 
        assert(resistance > 0.0);
        actual_resistance = resistance;
    }

private:
    double target_resistance;   // target value from the netlist (ohms)
    double actual_resistance;   // value used in stamping (ohms)
    double tolerance;           // allowed variance, as a fraction of target

    // Flat matrix indices set by setup()
    std::size_t n1n1 = 0;   // node 1's diagonal (+g)
    std::size_t n1n2 = 0;   // row node 1, column node 2 (-g)
    std::size_t n2n2 = 0;   // node 2's diagonal (+g)
    std::size_t n2n1 = 0;   // row node 2, column node 1 (-g)
};