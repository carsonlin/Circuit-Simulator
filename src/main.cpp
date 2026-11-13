#include "circuit.h"
#include "montecarlo.h"
#include "parser.h"
#include "voltagesource.h"
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Usage: circuit_sim <netlist> [runs] [seed]
int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "usage: " << argv[0] << " <netlist> [runs] [seed]\n";
            return 1;
        }
        const std::string path = argv[1];
        const std::size_t runs = argc > 2 ? std::stoul(argv[2]) : 10000;
        const unsigned seed = argc > 3 ? static_cast<unsigned>(std::stoul(argv[3])) : 42;

        Netlist netlist = parseNetlist(path);
        const auto& names = netlist.original_node_names;

        // Collect source names before the components are moved into the Circuit.
        // Circuit stores sources in the same order, so source_names[i] matches source_currents[i].
        std::vector<std::string> source_names;
        for (const auto& component : netlist.components) {
            if (dynamic_cast<const VoltageSource*>(component.get())) {
                source_names.push_back(component->getName());
            }
        }

        Circuit circuit(std::move(netlist.components));
        std::cout << std::fixed << std::setprecision(4);

        // Nominal analysis (every resistor at its target value)
        Results nominal = circuit.analyze();
        std::cout << "Nominal analysis\n";
        for (std::size_t id = 1; id < nominal.node_voltages.size(); ++id) {   // skip ground
            std::cout << "  node " << names[id] << ": " << nominal.node_voltages[id] << " V\n";
        }
        for (std::size_t i = 0; i < nominal.source_currents.size(); ++i) {
            std::cout << "  " << source_names[i] << ": " << nominal.source_currents[i] << " A\n";
        }

        // Monte Carlo tolerance analysis
        MonteCarlo mc(circuit, seed);
        MonteCarloResults stats = mc.run(runs);

        std::cout << "\nMonte Carlo (" << runs << " runs, seed " << seed << ")\n";
        for (std::size_t id = 1; id < stats.node_stats.size(); ++id) {   // skip ground
            const RunningStats& s = stats.node_stats[id];
            std::cout << "  node " << names[id]
                      << ": mean " << s.mean << " V"
                      << ", std dev " << s.stdDev() * 1000.0 << " mV"
                      << ", min " << s.min << " V"
                      << ", max " << s.max << " V\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}