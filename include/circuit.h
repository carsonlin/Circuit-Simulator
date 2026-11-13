#pragma once

#include "matrix.h"
#include "component.h"
#include "resistor.h"
#include "voltagesource.h"
#include <memory>
#include <vector>


struct Results {
    // Voltage at each node, indexed by packed node ID. Index 0 is ground (always 0 V).
    std::vector<double> node_voltages;
    // Current through each voltage source, in the order the sources appear in the netlist.
    std::vector<double> source_currents;
};

// A circuit built from resistors and voltage sources, solved with
// Modified Nodal Analysis. Node IDs must be packed: ground is 0 and the
// other nodes are numbered 1..n with no gaps produced by parseNetlist.
class Circuit {
public:
    Circuit(std::vector<std::unique_ptr<Component>> components);

    const std::vector<Resistor*>& getResistors() const { return m_resistors; }

    // Builds the MNA system from the components and solves it.
    Results analyze();

private:
    Matrix m_matrix;
    std::size_t m_node_count;
    std::vector<std::unique_ptr<Component>> m_components;
    std::vector<VoltageSource*> m_voltage_sources;
    std::vector<Resistor*> m_resistors;
};