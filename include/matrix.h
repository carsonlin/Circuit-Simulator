#pragma once

#include <cstddef>
#include <vector>

class Matrix{
    public:
        Matrix(std::size_t rows, std::size_t cols);
        double& operator()(std::size_t row, std::size_t col);
        double  operator()(std::size_t row, std::size_t col) const;
        std::size_t rows() const;
        std::size_t cols() const;
        void print() const;

        void swapRows(std::size_t r1, std::size_t r2);
    
    private:
        std::size_t index(std::size_t row, std::size_t col) const {return row * m_cols + col; }
        std::size_t m_rows;
        std::size_t m_cols;
        std::vector<double> m_table;
};