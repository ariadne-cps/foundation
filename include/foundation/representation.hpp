/***************************************************************************
 *            foundation/representation.hpp
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

/** @file foundation/representation.hpp
 *  @brief Representation wrappers and printable representations of values.
 */

#ifndef ARIADNE_FOUNDATION_REPRESENTATION_HPP
#define ARIADNE_FOUNDATION_REPRESENTATION_HPP

#include "utility/typedefs.hpp"

namespace Ariadne {

/** @brief Non-owning wrapper selecting representation output for a value. */
template<class T>
struct Representation {
    const T* pointer;
    /** @brief Construct a \c Representation from value. */
    explicit Representation(const T& value) : pointer(&value) {}
    /** @brief Return a reference to the represented object. */
    const T& reference() const { return *pointer; }
};

/** @brief Return a wrapper for printing the value representation. */
template<class T>
inline Representation<T> representation(const T& value) {
    return Representation<T>(value);
}

template<class T>
concept HasRepresentation = requires(OutputStream& os, const T& value) {
    value._repr(os);
};

/** @brief Write the value to the output stream. */
template<class T>
inline OutputStream& operator<<(OutputStream& os, const Representation<T>& object) {
    if constexpr (HasRepresentation<T>) {
        object.reference()._repr(os);
        return os;
    } else {
        return os << object.reference();
    }
}

} // namespace Ariadne

#endif /* ARIADNE_FOUNDATION_REPRESENTATION_HPP */
