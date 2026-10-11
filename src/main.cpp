#include <iostream>
#include <stdexcept>

int main() {
    try {
        // std::vector<double> x = solve(M);
        // print results
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}