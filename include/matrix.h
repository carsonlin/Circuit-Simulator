#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

// Dense matrix of doubles, stored row-major in one contiguous vector
// (element (r, c) is at r * cols + c)
// n rows by n + 1 columns, with the right-hand side in the last column

class Matrix {
public:
    // Empty 0x0 matrix, so classes holding a Matrix can be default-constructed.
    Matrix() : m_rows(0), m_cols(0) {}

    // rows x cols matrix with every entry set to 0.0.
    Matrix(std::size_t rows, std::size_t cols);

    // Element access by (row, col).
    double& operator()(std::size_t row, std::size_t col) {
        assert(row < m_rows && col < m_cols);
        return m_table[index(row, col)];
    }

    double operator()(std::size_t row, std::size_t col) const {
        assert(row < m_rows && col < m_cols);
        return m_table[index(row, col)];
    }

    // Element access by precomputed flat index (row * cols + col).
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

    // Sets every entry to 0.0 without changing the size. Called before each re-stamp.
    void zeroMatrix();

    // Prints the matrix to stdout, for debugging.
    void print() const;

    // Swaps two entire rows, including the right-hand side column. Used for pivoting.
    void swapRows(std::size_t r1, std::size_t r2);

private:
    std::size_t index(std::size_t row, std::size_t col) const {
        return row * m_cols + col;
    }

    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<double> m_table;
};