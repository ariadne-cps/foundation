/***************************************************************************
 *            foundation/representation.hpp
 *
 *  Copyright  2026  Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ARIADNE_FOUNDATION_REPRESENTATION_HPP
#define ARIADNE_FOUNDATION_REPRESENTATION_HPP

#include "utility/typedefs.hpp"

namespace Ariadne {

template<class T>
struct Representation {
    const T* pointer;
    explicit Representation(const T& value) : pointer(&value) {}
    const T& reference() const { return *pointer; }
};

template<class T>
inline Representation<T> representation(const T& value) {
    return Representation<T>(value);
}

template<class T>
concept HasRepresentation = requires(OutputStream& os, const T& value) {
    value._repr(os);
};

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
