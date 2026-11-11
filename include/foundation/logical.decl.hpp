/***************************************************************************
 *            foundation/logical.decl.hpp
 *
 *  Copyright  2013-20  Pieter Collins
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

/** @file foundation/logical.decl.hpp
 *  @brief Forward declarations and logical types for three-valued and validated computation.
 */

#ifndef ARIADNE_LOGICAL_DECL_HPP
#define ARIADNE_LOGICAL_DECL_HPP

#include "foundation/paradigm.hpp"

namespace Ariadne {

class Indeterminate;
extern const Indeterminate indeterminate;

class Boolean;
class Sierpinskian;
class Kleenean;

/** @brief Type transformation selecting the negated logical type. */
template<class L>
class Negate;
/** @brief Type transformation selecting the lower-information type. */
template<class L>
class Lower;
/** @brief Type transformation selecting the upper-information type. */
template<class L>
class Upper;
/** @brief Type transformation selecting the validated-information type. */
template<class L>
class Validated;
/** @brief Type transformation selecting the approximate-information type. */
template<class L>
class Approximate;

class NegatedSierpinskian;
class LowerKleenean;
class UpperKleenean;
class ValidatedSierpinskian;
class ValidatedNegatedSierpinskian;
class ValidatedKleenean;
class ValidatedLowerKleenean;
class ValidatedUpperKleenean;
class ApproximateKleenean;

using Fuzzy = ApproximateKleenean;

/** @brief Associated type definitions for logical. */
template<class P>
struct LogicalTypedef;
template<class P>
using LogicalType = typename LogicalTypedef<P>::Type;
/** @brief Associated type definitions for logical. */
template<>
struct LogicalTypedef<ExactTag> {
    typedef Boolean Type;
};
/** @brief Associated type definitions for logical. */
template<>
struct LogicalTypedef<EffectiveTag> {
    typedef Kleenean Type;
};
/** @brief Associated type definitions for logical. */
template<>
struct LogicalTypedef<ValidatedTag> {
    typedef ValidatedKleenean Type;
};
/** @brief Associated type definitions for logical. */
template<>
struct LogicalTypedef<ApproximateTag> {
    typedef ApproximateKleenean Type;
};

/** @brief Associated type definitions for lower logical. */
template<class P>
struct LowerLogicalTypedef;
template<class P>
using LowerLogicalType = typename LowerLogicalTypedef<P>::Type;
/** @brief Associated type definitions for lower logical. */
template<>
struct LowerLogicalTypedef<EffectiveTag> {
    typedef LowerKleenean Type;
};
/** @brief Associated type definitions for lower logical. */
template<>
struct LowerLogicalTypedef<ValidatedTag> {
    typedef ValidatedLowerKleenean Type;
};
/** @brief Associated type definitions for lower logical. */
template<>
struct LowerLogicalTypedef<ApproximateTag> {
    typedef ApproximateKleenean Type;
};

/** @brief Associated type definitions for upper logical. */
template<class P>
struct UpperLogicalTypedef;
template<class P>
using UpperLogicalType = typename UpperLogicalTypedef<P>::Type;
/** @brief Associated type definitions for upper logical. */
template<>
struct UpperLogicalTypedef<EffectiveTag> {
    typedef UpperKleenean Type;
};
/** @brief Associated type definitions for upper logical. */
template<>
struct UpperLogicalTypedef<ValidatedTag> {
    typedef ValidatedUpperKleenean Type;
};
/** @brief Associated type definitions for upper logical. */
template<>
struct UpperLogicalTypedef<ApproximateTag> {
    typedef ApproximateKleenean Type;
};

template<class P>
using KleeneanType = LogicalType<P>;
template<class P>
using LowerKleeneanType = LowerLogicalType<P>;
template<class P>
using UpperKleeneanType = UpperLogicalType<P>;

using Decidable = Boolean;
using Quasidecidable = Kleenean;
using Verifyable = Sierpinskian;
using Falsifyable = NegatedSierpinskian;

/** @brief Return the logical negation. */
Boolean operator!(const Boolean &);
/** @brief Return the logical negation. */
NegatedSierpinskian operator!(const Sierpinskian &);
/** @brief Return the logical negation. */
Kleenean operator!(const Kleenean &);

/** @brief Compile-time traits for apartness. */
template<class P>
struct ApartnessTraits;
template<class P>
using ApartnessType = typename ApartnessTraits<P>::Type;
/** @brief Compile-time traits for apartness. */
template<>
struct ApartnessTraits<ExactTag> {
    typedef Boolean Type;
};
/** @brief Compile-time traits for apartness. */
template<>
struct ApartnessTraits<EffectiveTag> {
    typedef Sierpinskian Type;
};
/** @brief Compile-time traits for apartness. */
template<>
struct ApartnessTraits<ValidatedTag> {
    typedef ValidatedSierpinskian Type;
};
/** @brief Compile-time traits for apartness. */
template<>
struct ApartnessTraits<ApproximateTag> {
    typedef ApproximateKleenean Type;
};

template<class P>
using EqualityLogicalType = decltype(not declval<ApartnessType<P>>());
template<class P>
using InequalityLogicalType = ApartnessType<P>;

namespace Detail {

enum class LogicalValue : ComparableEnumerationType;
}
using Detail::LogicalValue;

} // namespace Ariadne

#endif
