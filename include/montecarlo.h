#pragma once

#include <cstddef>
#include <limits>
#include <random>
#include <vector>
#include "circuit.h"

// Running mean, variance, min and max, updated one value at a time
// (Welford's algorithm). Stores no samples, so memory is fixed regardless of run count.
struct RunningStats {
    std::size_t count = 0;
    double mean = 0.0;
    double m2 = 0.0;   // sum of squared differences from the mean
    double min = std::numeric_limits<double>::infinity();
    double max = -std::numeric_limits<double>::infinity();

    // Adds one value to the statistics.
    void add(double x);

    // Sample variance and standard deviation. Return 0 if fewer than 2 values were added.
    double variance() const;
    double stdDev() const;
};

// Per-node statistics from a Monte Carlo analysis indexed by packed node ID.
struct MonteCarloResults {
    std::vector<RunningStats> node_stats;
};

// Monte Carlo tolerance analysis: on each run, every resistor gets a random value
class MonteCarlo {
public:
    // Does not take ownership. The circuit must outlive this object.
    // The same seed gives the same results.
    MonteCarlo(Circuit& circuit, unsigned seed);

    // Performs the given number of runs and returns statistics for every node.
    // Resistors are restored to their target values afterward.
    MonteCarloResults run(std::size_t runs);

private:
    Circuit& m_circuit;
    std::mt19937 m_rng;
};