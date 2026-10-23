#include "circuit.h"
#include "matrix.h"
#include "component.h"
#include <algorithm>
#include "voltagesource.h"
#include "gaussian.h"

Circuit::Circuit(std::vector<std::unique_ptr<Component>> components) : m_components(std::move(components)){
    std::size_t max_id = 0;
    for (const auto& component : m_components) {
        max_id = std::max({max_id, component->getNode1(), component->getNode2()});
        if (auto* source = dynamic_cast<VoltageSource*>(component.get())) {
            m_voltage_sources.push_back(source);
        }
    }

    m_node_count = max_id;

    for (std::size_t i = 0; i < m_voltage_sources.size(); ++i) {
        m_voltage_sources[i]->setIndex(max_id + i);
    }
    const std::size_t n = max_id + m_voltage_sources.size();

    m_matrix = Matrix(n, n + 1);

    for (const auto& component : m_components) {
        component->setup(m_matrix);
    }
}

Results Circuit::analyze(){
    m_matrix.zeroMatrix();

    for(const auto& component: m_components){
        component->stamp(m_matrix);
    }

    std::vector<double> solution = solve(m_matrix);

    std::vector<double> node_voltages{};
    std::vector<double> source_currents{};

    node_voltages.push_back(0.0);
    for(std::size_t i = 0; i < solution.size(); ++i){
        if(i < m_node_count){
            node_voltages.push_back(solution[i]);
        }
        else{
            source_currents.push_back(solution[i]);
        }
    }

    return Results{std::move(node_voltages), std::move(source_currents)};

}