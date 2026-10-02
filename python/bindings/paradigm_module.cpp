/***************************************************************************
 *            paradigm_module.cpp
 *
 *  Copyright  2026  Pieter Collins
 *
 ****************************************************************************/

#include "pybind11.hpp"

void paradigm_submodule(pybind11::module& module);

PYBIND11_MODULE(pyariadne, module) {
    paradigm_submodule(module);
}
