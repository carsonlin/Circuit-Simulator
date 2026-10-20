#include "component.h"
#include <cstddef>
#pragma once


class Resistor: public Component{

    public:
        Resistor(std::size_t n1, std::size_t n2, double r, double tol = 0.0);

        void stamp(Matrix& matrix) const override;
        void setup(const Matrix& matrix) override;

        double getTargetResistance() const { return target_resistance; }
        double getActualResistance() const { return actual_resistance; }
        double getTolerance() const { return tolerance; }

    private:
        double target_resistance;
        double actual_resistance;
        double tolerance;
        std::size_t n1n1 = 0;
        std::size_t n1n2 = 0;
        std::size_t n2n2 = 0;
        std::size_t n2n1 = 0;
};