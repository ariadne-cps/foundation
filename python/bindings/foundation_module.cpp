/***************************************************************************
 *            foundation_module.cpp
 *
 *  Copyright  2026  Pieter Collins
 *
 ****************************************************************************/

#include "pybind11.hpp"

void foundation_submodule(pybind11::module& module);

void foundation_module(pybind11::module& module) {
    foundation_submodule(module);
}
