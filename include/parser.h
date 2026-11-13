#pragma once

#include <cstddef>
#include <istream>
#include <memory>
#include <string>
#include <vector>

#include "component.h"

// Netlist format
//
//     NAME NODE1 NODE2 VALUE [TOLERANCE%]
//
//     V1 1 0 9          voltage source: + at node 1, - at node 0, 9 volts
//     R1 1 2 1000 5%    resistor: 1000 ohms, 5% tolerance
//     R2 2 0 2000       resistor: no tolerance given, so 0%
//
// The first letter of NAME selects the type (R = resistor, V = voltage source).
// Node 0 is always ground.

// The result of parsing a netlist.
struct Netlist {
    // Components using packed node IDs: 0 = ground, others numbered 1..N
    std::vector<std::unique_ptr<Component>> components;

    // Maps each packed ID back to the node number written in the netlist:
    // original_node_names[id] is the original number.
    std::vector<std::size_t> original_node_names;
};


Netlist parseNetlist(const std::string& path);

