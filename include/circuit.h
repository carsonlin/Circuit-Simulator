#include "matrix.h"
#include <component.h>
#include <memory>

struct Results{
    std::vector<double> node_voltages;
    std::vector<double> source_currents;
};

class Circuit{
    public:
        Circuit(std::vector<std::unique_ptr<Component>> components);

        Results analyze();



    private:
        Matrix matrix;
        std::vector<std::unique_ptr<Component>> m_components;
        std::vector<VoltageSource*> voltage_sources;
};