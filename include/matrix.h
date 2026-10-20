#pragma once
 
#include <cassert>
#include <cstddef>
#include <vector>
 
// Dense matrix stored row-major in one contiguous vector.
class Matrix {
public:
    Matrix(std::size_t rows, std::size_t cols);
 
    double& operator()(std::size_t row, std::size_t col) {
        assert(row < m_rows && col < m_cols);
        return m_table[index(row, col)];
    }
 
    double operator()(std::size_t row, std::size_t col) const {
        assert(row < m_rows && col < m_cols);
        return m_table[index(row, col)];
    }
 
    double& flat(std::size_t flat_index) {
        assert(flat_index < m_table.size());
        return m_table[flat_index];
    }
 
    double flat(std::size_t flat_index) const {
        assert(flat_index < m_table.size());
        return m_table[flat_index];
    }
 
    std::size_t rows() const { return m_rows; }
    std::size_t cols() const { return m_cols; }
 
    void zeroMatrix();
    void print() const;
    void swapRows(std::size_t r1, std::size_t r2);
 
private:
    std::size_t index(std::size_t row, std::size_t col) const {
        return row * m_cols + col;
    }
 
    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<double> m_table;
};