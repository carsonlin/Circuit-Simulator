#include "montecarlo.h"
#include <cmath>
#include <algorithm>


MonteCarlo::MonteCarlo(Circuit& circuit, unsigned seed)
    : m_circuit(circuit), m_rng(seed) {}


MonteCarloResults MonteCarlo::run(std::size_t runs) {
    MonteCarloResults stats;
    const auto& resistors = m_circuit.getResistors();

    for (std::size_t r = 0; r < runs; ++r) {
        // Give every resistor a random value within its tolerance.
        for (Resistor* resistor : resistors) {
            const double target = resistor->getTargetResistance();
            const double tol = resistor->getTolerance();
            std::uniform_real_distribution<double> dist(target * (1.0 - tol), target * (1.0 + tol));
            resistor->setActualResistance(dist(m_rng));
        }

        Results analysis = m_circuit.analyze();

        // Size the stats on the first run, once the node count is known.
        if (stats.node_stats.empty()) {
            stats.node_stats.resize(analysis.node_voltages.size());
        }

        for (std::size_t node = 0; node < analysis.node_voltages.size(); ++node) {
            stats.node_stats[node].add(analysis.node_voltages[node]);
        }
    }

    // Leave the circuit with its nominal values.
    for (Resistor* resistor : resistors) {
        resistor->setActualResistance(resistor->getTargetResistance());
    }

    return stats;
}

void RunningStats::add(double x) {
    ++count;
    const double delta1 = x - mean;
    mean += (x - mean) / count;

    const double delta2 = x - mean;
    m2 += delta1 * delta2;

    min = std::min(min, x);
    max = std::max(max, x);
}

// Sample variance (divides by count - 1).
double RunningStats::variance() const {
    return count < 2 ? 0.0 : m2 / (count - 1);
}

double RunningStats::stdDev() const {
    return std::sqrt(variance());
}