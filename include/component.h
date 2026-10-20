#pragma once
#include <cstddef>
#include "matrix.h"

class Component{
    protected:
        Component(std::size_t n1, std::size_t n2): node1_id(n1), node2_id(n2){}

    public:
        virtual ~Component() = default;
        virtual void stamp(Matrix& matrix) const = 0;
        virtual void setup(const Matrix& matrix) = 0;
        std::size_t getNode1() const {return node1_id;}
        std::size_t getNode2() const {return node2_id;}

    private:
        std::size_t node1_id;
        std::size_t node2_id;

};

