#pragma once
#include <cstddef>
#include <string>
#include "matrix.h"

// Base class for circuit elements (resistors, voltage sources).
// Each component connects two nodes, identified by packed node IDs
//
// Building the  matrix is split into two steps:
//   setup() runs once per circuit structure and works out where this
//           component's entries go in the matrix.
//   stamp() runs on every analysis and adds the component's values at those positions.
class Component {
protected:
    // Only derived classes can construct a Component.
    Component(std::string name, std::size_t n1, std::size_t n2)
        : name(std::move(name)), node1_id(n1), node2_id(n2) {}

public:
    virtual ~Component() = default;

    // Adds this component's contribution to the matrix
    // Requires setup() to have been called with a matrix of the same size.
    virtual void stamp(Matrix& matrix) const = 0;

    // Computes and stores this component's matrix positions.
    // Must be called again if the circuit's structure changes.
    virtual void setup(const Matrix& matrix) = 0;

    std::size_t getNode1() const { return node1_id; }
    std::size_t getNode2() const { return node2_id; }
    const std::string& getName() const { return name; }

private:
    std::string name;
    std::size_t node1_id;
    std::size_t node2_id;
};