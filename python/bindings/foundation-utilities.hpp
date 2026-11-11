/***************************************************************************
 *            foundation-utilities.hpp
 *
 *  Copyright  2026  Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne Foundation.
 *
 *  Ariadne Foundation is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne Foundation is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne Foundation. If not, see <https://www.gnu.org/licenses/>.
 */

/** @file python/bindings/foundation-utilities.hpp
 *  @brief Utilities for exposing Foundation values and representations to Python.
 */

#ifndef ARIADNE_PYTHON_FOUNDATION_UTILITIES_HPP
#define ARIADNE_PYTHON_FOUNDATION_UTILITIES_HPP

#include "foundation/representation.hpp"
#include "utilities.hpp"
#include "utility/string.hpp"
#include "utility/typedefs.hpp"

namespace Ariadne {

template<class T>
struct PythonClassName {
    static std::string get() { return class_name<T>(); }
};

template<class T>
OutputStream& operator<<(OutputStream& os, const PythonRepresentation<T>& repr) {
    return os << python_class_name<T>() << "(" << repr.reference() << ")";
}

template<class T>
OutputStream& operator<=(OutputStream& os, const T& value) {
    return os << python_representation(value);
}

template<class T>
std::string __crepr__(const T& value) {
    std::stringstream ss;
    ss << representation(value);
    return ss.str();
}

} // namespace Ariadne

#endif /* ARIADNE_PYTHON_FOUNDATION_UTILITIES_HPP */
