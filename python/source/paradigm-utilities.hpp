/***************************************************************************
 *            paradigm-utilities.hpp
 *
 *  Copyright  2026  Pieter Collins
 *
 ****************************************************************************/

#ifndef ARIADNE_PYTHON_PARADIGM_UTILITIES_HPP
#define ARIADNE_PYTHON_PARADIGM_UTILITIES_HPP

#include "utilities.hpp"

#include "utility/string.hpp"
#include "utility/typedefs.hpp"

namespace Ariadne {

template<class T> struct PythonClassName {
    static std::string get() { return class_name<T>(); }
};

template<class T>
OutputStream& operator<<(OutputStream& os, const PythonRepresentation<T>& repr) {
    return os << python_class_name<T>() << "(" << repr.reference() << ")";
}

template<class T>
OutputStream& operator<=(OutputStream& os, T const& value) {
    return os << python_representation(value);
}

} // namespace Ariadne

#endif /* ARIADNE_PYTHON_PARADIGM_UTILITIES_HPP */
