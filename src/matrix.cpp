#include "Matrix.h"
#include <iostream>
#include <iomanip>

Matrix::Matrix(std::size_t rows, std::size_t cols)
    : m_rows(rows), m_cols(cols), m_table(rows * cols, 0.0)
{
}

void Matrix::print() const {
    for (std::size_t r = 0; r < m_rows; ++r) {
        for (std::size_t c = 0; c < m_cols; ++c) {
            std::cout << std::setw(5) << m_table[index(r, c)];
        }
        std::cout << '\n';
    }
}

void Matrix::swapRows(std::size_t r1, std::size_t r2){
    if(r1 == r2){
        return;
    }
    for(std::size_t i = 0; i < m_cols; ++i){
        double temp = m_table[index(r1, i)];
        m_table[index(r1, i)] = m_table[index(r2, i)];
        m_table[index(r2, i)] = temp;
    }
}

void Matrix::reset() {
    for (double& value : m_table) {
        value = 0.0;
    }
}