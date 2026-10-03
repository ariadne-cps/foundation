/***************************************************************************
 *            foundation_module.cpp
 *
 *  Copyright  2026  Pieter Collins
 *
 ****************************************************************************/

#include "pybind11.hpp"

void foundation_submodule(pybind11::module& module);

PYBIND11_MODULE(pyariadne, module) {
    foundation_submodule(module);
}
