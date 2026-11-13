#include "parser.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "resistor.h"
#include "voltagesource.h"

Netlist parseNetlist(const std::string& path) {
    std::ifstream input(path);
    if (!input) throw std::runtime_error("could not open " + path);

    Netlist netlist;
    netlist.original_node_names.push_back(0);   // packed ID 0 = ground

    // Original node number -> packed ID. Only needed while parsing, so that a node
    // gets the same ID every time it appears. original_node_names holds the reverse.
    std::unordered_map<std::size_t, std::size_t> packed_ids;

    // Returns the packed ID for a node from the file, assigning the next free ID
    // and recording it in both structures the first time the node is seen.
    auto toPackedId = [&](std::size_t node_name) -> std::size_t {
        if (node_name == 0) return 0;
        // try_emplace only inserts if node_name is new
        auto [it, inserted] = packed_ids.try_emplace(node_name, netlist.original_node_names.size());
        if (inserted) netlist.original_node_names.push_back(node_name);
        return it->second;
    };

    // Each line: name node1 node2 value [tolerance%]
    std::string line;
    while (std::getline(input, line)) {
        std::istringstream tokens(line);
        std::string name;
        double tol;
        std::size_t n1, n2;
        double value;
        if (!(tokens >> name >> n1 >> n2 >> value)) continue;   // blank or unreadable

        if (name[0] == 'R') {
            // Optional tolerance, given in percent; stored as a fraction. Defaults to 0.
            const double tolerance = (tokens >> tol) ? tol / 100.0 : 0.0;
            netlist.components.push_back(std::make_unique<Resistor>(
                name, toPackedId(n1), toPackedId(n2), value, tolerance));
        } else if (name[0] == 'V') {
            netlist.components.push_back(std::make_unique<VoltageSource>(
                name, toPackedId(n1), toPackedId(n2), value));
        }
    }
    return netlist;
}