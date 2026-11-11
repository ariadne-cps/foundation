/***************************************************************************
 *            foundation/logical.hpp
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

/** @file foundation/logical.hpp
 *  @brief Logical values, predicates, evidence, and decision procedures.
 */

#ifndef ARIADNE_LOGICAL_HPP
#define ARIADNE_LOGICAL_HPP

#include <limits>
#include <memory>
#include <stdexcept>

#include "foundation/paradigm.hpp"
#include "logical.decl.hpp"
#include "utility/handle.hpp"
#include "utility/stdlib.hpp"
#include "utility/string.hpp"
#include "utility/typedefs.hpp"

namespace Ariadne {

template<class T>
String class_name();

//! \ingroup LogicSubModule
//! \brief The amount of work used in performing a calculation or checking a quasidecidable predicate.
//! \details The Effort should roughly reflect the time needed to perform a computation.
//! \sa Accuracy.
class Effort {
    inline static Nat _default = 0u;
    Nat _m;

  public:
    //! \brief Get the default effort. Initially set to the minimum possible effort 0.
    static Effort get_default() { return Effort(_default); }
    //! \brief Set the default effort.
    static Void set_default(Nat m) { _default = m; }

  public:
    //! \brief Construct from a raw positive integer.
    explicit Effort(Nat m) : _m(m) {}
    //! \brief Convert to a raw positive integer.
    operator Nat() const { return _m; }
    /** @brief Return the work budget represented by this effort. */
    Nat work() const { return _m; }
    /** @brief Increment the value or iterator. */
    Effort& operator++() {
        ++_m;
        return *this;
    }
    /** @brief Add the operand in place. */
    Effort& operator+=(Nat m) {
        _m += m;
        return *this;
    }
    /** @brief Multiply by the operand in place. */
    Effort& operator*=(Nat m) {
        _m *= m;
        return *this;
    }
    /** @brief Write the value to the output stream. */
    friend OutputStream& operator<<(OutputStream& os, Effort eff) { return os << "Effort(" << eff._m << ")"; }
};
/** @brief Construct the typed value from the literal. */
inline Effort operator""_eff(unsigned long long int e) {
    if (e > static_cast<unsigned long long int>(std::numeric_limits<Nat>::max())) {
        throw std::overflow_error("Effort literal out of range");
    }
    return Effort(static_cast<Nat>(e));
}

namespace Detail {

//! \ingroup LogicalTypes
//! \brief An enumeration containing the possible values of a logical variable.
enum class LogicalValue : ComparableEnumerationType {
    FALSE = -2,        //!< Definitely not true.
    UNLIKELY = -1,     //!< Considered unlikely to be true.
    INDETERMINATE = 0, //!< Truth is unknown, possibly undecidable.
    LIKELY = +1,       //!< Considered likely to be true.
    TRUE = +2          //!< Definitely true.
};
/** @brief Construct the logical value with the supplied truth information. */
inline LogicalValue make_logical_value(bool b) { return b ? LogicalValue::TRUE : LogicalValue::FALSE; }
/** @brief Test whether truth has been established. */
inline Bool definitely(LogicalValue lv) { return lv == LogicalValue::TRUE; }
/** @brief Return the approximate truth decision. */
inline Bool probably(const LogicalValue& lv) { return lv >= LogicalValue::LIKELY; }
/** @brief Convert the logical predicate to a Boolean decision. */
inline Bool decide(LogicalValue lv) { return lv >= LogicalValue::LIKELY; }
/** @brief Test whether falsity has not been established. */
inline Bool possibly(LogicalValue lv) { return lv != LogicalValue::FALSE; }
/** @brief Test whether the logical value is definitely true or definitely false. */
inline Bool is_determinate(LogicalValue lv) { return lv == LogicalValue::TRUE or lv == LogicalValue::FALSE; }
/** @brief Test whether neither truth nor falsity has been established. */
inline Bool is_indeterminate(LogicalValue lv) { return lv == LogicalValue::INDETERMINATE or lv == LogicalValue::UNLIKELY or lv == LogicalValue::LIKELY; }
/** @brief Test equality of the operands. */
LogicalValue operator==(LogicalValue lv1, LogicalValue lv2);
/** @brief Return the logical negation. */
inline LogicalValue operator!(LogicalValue lv) { return static_cast<LogicalValue>(-static_cast<ComparableEnumerationType>(lv)); }
/** @brief Return the logical conjunction. */
inline LogicalValue operator&&(LogicalValue lv1, LogicalValue lv2) { return std::min(lv1, lv2); }
/** @brief Return the logical disjunction. */
inline LogicalValue operator||(LogicalValue lv1, LogicalValue lv2) { return std::max(lv1, lv2); }
/** @brief Return the exclusive disjunction. */
inline LogicalValue operator^(LogicalValue lv1, LogicalValue lv2) { return not(lv1 == lv2); }
/** @brief Write the value to the output stream. */
OutputStream& operator<<(OutputStream& os, LogicalValue l);

/** @brief Polymorphic interface for logical. */
class LogicalInterface {
  public:
    /** @brief Destroy the object and release its owned resources. */
    virtual ~LogicalInterface() = default;
    /** @brief Return a copy of the object. */
    virtual LogicalInterface* _copy() const = 0;
    /** @brief Check the predicate at the supplied effort or validate the supplied data. */
    virtual LogicalValue _check(Effort) const = 0;
    /** @brief Write the value to the output stream. */
    virtual OutputStream& _write(OutputStream&) const = 0;
};

/** @brief Owning handle to a lazily checked logical predicate. */
class LogicalHandle : public Handle<const LogicalInterface> {
  public:
    using Handle<const LogicalInterface>::Handle;
    /** @brief Construct a constant with the supplied value. */
    static LogicalHandle constant(LogicalValue v);
    /** @brief Check the predicate at the supplied effort or validate the supplied data. */
    LogicalValue check(Effort eff) const { return this->pointer()->_check(eff); }
    /** @brief Write the value to the output stream. */
    friend OutputStream& operator<<(OutputStream& os, LogicalHandle l) {
        return l.pointer()->_write(os);
    }
};

/** @brief Check the predicate at the supplied effort or validate the supplied data. */
inline LogicalValue check(LogicalHandle l, Effort e) { return l.check(e); }
/** @brief Test whether truth has been established. */
inline Bool definitely(LogicalHandle l, Effort e) { return definitely(check(l, e)); }
/** @brief Return the approximate truth decision. */
inline Bool probably(LogicalHandle l, Effort e) { return probably(check(l, e)); }
/** @brief Convert the logical predicate to a Boolean decision. */
inline Bool decide(LogicalHandle l, Effort e) { return decide(check(l, e)); }
/** @brief Test whether falsity has not been established. */
inline Bool possibly(LogicalHandle l, Effort e) { return possibly(check(l, e)); }
/** @brief Return the logical conjunction. */
LogicalHandle operator&&(LogicalHandle v1, LogicalHandle v2);
/** @brief Return the logical disjunction. */
LogicalHandle operator||(LogicalHandle v1, LogicalHandle v2);
/** @brief Test equality of the operands. */
LogicalHandle operator==(LogicalHandle v1, LogicalHandle v2);
/** @brief Return the exclusive disjunction. */
LogicalHandle operator^(LogicalHandle v1, LogicalHandle v2);
/** @brief Return the logical negation. */
LogicalHandle operator!(LogicalHandle v);

/** @brief Return the logical conjunction. */
LogicalHandle conjunction(LogicalHandle l1, LogicalHandle l2);
/** @brief Return the logical disjunction. */
LogicalHandle disjunction(LogicalHandle l1, LogicalHandle l2);
/** @brief Return the logical negation. */
LogicalHandle negation(LogicalHandle l);
/** @brief Return logical equivalence. */
LogicalHandle equality(LogicalHandle l1, LogicalHandle l2);
/** @brief Return the exclusive disjunction. */
LogicalHandle exclusive(LogicalHandle l1, LogicalHandle l2);

/** @brief Obtain truth information from the logical interface pointer. */
LogicalValue logical_value_from_pointer(LogicalInterface* ptr);
/** @brief Allocate a logical interface representing the supplied truth information. */
LogicalInterface* new_logical_pointer_from_value(LogicalValue);

/** @brief Construct the logical type from a borrowed interface pointer. */
template<class P>
LogicalType<P> logical_type_from_pointer(LogicalInterface* ptr) {
    if constexpr (ConstructibleFrom<LogicalType<P>, LogicalHandle>) {
        auto elptr = dynamic_cast<LogicalInterface*>(ptr);
        if (elptr) {
            return LogicalType<P>(LogicalHandle(SharedPointer<LogicalInterface>(elptr)));
        }
    }
    static_assert(ConstructibleFrom<LogicalType<P>, LogicalValue>);
    return LogicalType<P>(logical_value_from_pointer(ptr));
}

// Unlike the legacy conversion above, this API always takes ownership.
// Handle-based results retain the allocation; value-based results release
// it on both success and exception.
/** @brief Construct the logical type taking ownership of the interface pointer. */
template<class P>
LogicalType<P> logical_type_from_owned_pointer(LogicalInterface* ptr) {
    std::unique_ptr<LogicalInterface> owner(ptr);
    if constexpr (ConstructibleFrom<LogicalType<P>, LogicalHandle>) {
        return LogicalType<P>(LogicalHandle(std::shared_ptr<const LogicalInterface>(std::move(owner))));
    } else {
        return LogicalType<P>(logical_value_from_pointer(owner.get()));
    }
}

} // namespace Detail

using Detail::LogicalHandle;
using Detail::LogicalInterface;
using Detail::LogicalValue;

/** @brief Storage and conversion support shared by logical types. */
template<class L, class R>
class LogicalBase;

/** @brief Storage and conversion support shared by logical types. */
template<class L>
class LogicalBase<L, LogicalValue> {
    typedef LogicalValue R;

  protected:
    R _v;
    /** @brief Construct a \c LogicalBase from v. */
    explicit LogicalBase(R v) : _v(v) {}
    /** @brief Disallow this construction to preserve the ownership contract. */
    explicit LogicalBase(bool b) = delete;

  public:
    /** @brief Test whether truth has been established. */
    friend bool definitely(const L& l) { return Detail::definitely(l._v); }
    /** @brief Return the approximate truth decision. */
    friend bool probably(const L& l) { return Detail::probably(l._v); }
    /** @brief Convert the logical predicate to a Boolean decision. */
    friend bool decide(const L& l) { return Detail::decide(l._v); }
    /** @brief Test whether falsity has not been established. */
    friend bool possibly(const L& l) { return Detail::possibly(l._v); }
    /** @brief Test whether the logical value is definitely true or definitely false. */
    friend bool is_determinate(const L& l) { return Detail::is_determinate(l._v); }
    /** @brief Test whether neither truth nor falsity has been established. */
    friend bool is_indeterminate(const L& l) { return Detail::is_indeterminate(l._v); }
};

/** @brief Storage and conversion support shared by logical types. */
template<class L>
class LogicalBase<L, LogicalHandle> {
    typedef LogicalHandle R;

  protected:
    R _v;
    /** @brief Construct a \c LogicalBase from v. */
    explicit LogicalBase(R v) : _v(v) {}
    /** @brief Disallow this construction to preserve the ownership contract. */
    explicit LogicalBase(bool b) = delete;

  public:
    /** @brief Test whether truth has been established. */
    friend bool definitely(const L& l, Effort e) { return definitely(l.check(e)); }
    /** @brief Return the approximate truth decision. */
    friend bool probably(const L& l, Effort e) { return probably(l.check(e)); }
    /** @brief Convert the logical predicate to a Boolean decision. */
    friend bool decide(const L& l, Effort e) { return decide(l.check(e)); }
    /** @brief Test whether falsity has not been established. */
    friend bool possibly(const L& l, Effort e) { return possibly(l.check(e)); }
    /** @brief Test whether truth has been established. */
    friend bool definitely(const L& l) { return definitely(l, Effort::get_default()); }
    /** @brief Return the approximate truth decision. */
    friend bool probably(const L& l) { return probably(l, Effort::get_default()); }
    /** @brief Convert the logical predicate to a Boolean decision. */
    friend bool decide(const L& l) { return decide(l, Effort::get_default()); }
    /** @brief Test whether falsity has not been established. */
    friend bool possibly(const L& l) { return possibly(l, Effort::get_default()); }
};

/** @brief Logical value with the selected information and computational paradigm. */
template<class L, class R, class NL = L, class XL = L>
class Logical : public LogicalBase<L, R> { // FIXME: Use protected inheritence
    template<class LL, class RR, class NNLL, class XXLL>
    friend class Logical;

  protected:
    /** @brief Construct a \c Logical from v. */
    explicit Logical(R v) : LogicalBase<L, R>(v) {}
    /** @brief Disallow this construction to preserve the ownership contract. */
    explicit Logical(bool b) = delete;

  public:
    /** @brief Convert to const r &. */
    explicit operator const R&() const { return this->_v; }
    /** @brief Write the representation of the object. */
    const R& repr() const { return this->_v; }

  public:
    /** @brief Return the logical negation. */
    friend NL operator!(const L& l) { return NL(!l._v); }
    /** @brief Return the logical conjunction. */
    friend L operator&&(const L& l1, const L& l2) { return L(l1._v && l2._v); }
    /** @brief Return the logical disjunction. */
    friend L operator||(const L& l1, const L& l2) { return L(l1._v || l2._v); }

    /** @brief Return the exclusive disjunction. */
    friend XL operator^(const L& l1, const L& l2) { return XL(l1._v ^ l2._v); }
    /** @brief Test equality of the operands. */
    friend XL operator==(const L& l1, const L& l2) { return XL(l1._v == l2._v); }
    /** @brief Test inequality of the operands. */
    friend XL operator!=(const L& l1, const L& l2) { return XL(l1._v ^ l2._v); }

    /** @brief Write the value to the output stream. */
    friend OutputStream& operator<<(OutputStream& os, const L& l) { return os << l._v; }
};

#ifdef DOXYGEN
//! \relatesalso ValidatedKleenean \ingroup LogicalTypes
//! \brief Returns \c true if a finite (non-effective) logical type is definitely true, otherwise \c false.
//! In particular, always returns \c false for a ValidatedUpperKleenean or ApproximateKleenean.
template<class L>
bool definitely(const L&);
//! \relatesalso ValidatedKleenean \ingroup LogicalTypes
//! \brief Returns \c false if a finite (non-effective) logical type is definitely false, otherwise \c true.
//! In particular, always returns \c false for a ValidatedLowerKleenean or ApproximateKleenean.
template<class L>
bool possibly(const L&);
//! \relatesalso ApproximateKleenean \ingroup LogicalTypes
//! \brief Returns an 'educated guess' to the value of a finite (non-effective) logical type.
//! Returns \c true for logical values of \c TRUE and \c LIKELY, and \c false for \c INDETERMINATE, \c UNLIKELY and \c FALSE.
template<class L>
bool probably(const L& l);
//! \relatesalso Kleenean \ingroup LogicalTypes
//! \brief Force a decision of a logical type.
//! Returns \c true for logical values of \c TRUE, and \c LIKELY for a non-effective type, and \c decide(check(l,e)) for the default effort \c e.
template<class L>
bool decide(const L&);
#endif

/** @brief Test whether truth has been established. */
inline bool definitely(bool b) { return b; }
/** @brief Return the approximate truth decision. */
inline bool probably(bool b) { return b; }
/** @brief Convert the logical predicate to a Boolean decision. */
inline bool decide(bool b) { return b; }
/** @brief Test whether falsity has not been established. */
inline bool possibly(bool b) { return b; }

class Boolean;
class Sierpinskian;
class Kleenean;
class LowerKleenean;
class UpperKleenean;
class ValidatedSierpinskian;
class ValidatedNegatedSierpinskian;
class ValidatedKleenean;
class ValidatedLowerKleenean;
class ValidatedUpperKleenean;
class ApproximateKleenean;

/** @brief Tag representing an undecided logical value. */
class Indeterminate {
  public:
    /** @brief Convert to validated sierpinskian. */
    operator ValidatedSierpinskian() const;
    /** @brief Convert to validated kleenean. */
    operator ValidatedKleenean() const;
    /** @brief Convert the logical predicate to a Boolean decision. */
    friend Bool decide(const Indeterminate& l, Effort e);
    /** @brief Convert the logical predicate to a Boolean decision. */
    friend Bool decide(const Indeterminate& l);
    /** @brief Write the value to the output stream. */
    friend OutputStream& operator<<(OutputStream& os, const ValidatedKleenean&);
};

//! \ingroup LogicSubModule
//! \brief A logical class representing the result of a decidable predicate.
//! \sa LogicSubModule
class Boolean : public Logical<Boolean, LogicalValue> {
    typedef Logical<Boolean, LogicalValue> Base;

  public:
    /** @brief Construct a \c Boolean from l. */
    explicit Boolean(LogicalValue l) : Base(l) {}

  public:
    //! \brief Convert from a built-in boolean value, defaulting to \c true.
    Boolean(bool b = true) : Boolean(Detail::make_logical_value(b)) {}
    //! \brief Convert to a built-in boolean value.
    operator bool() const { return definitely(*this); }

    //! \brief Logical negation.
    friend Boolean operator!(const Boolean& b);
    //! \brief Logical conjunction.
    friend Boolean operator&&(const Boolean& b1, const Boolean& b2);
    //! \brief Logical disjunction. <code>b1 or b2</code> is equivalent to <code>not ( (not b1) and (not b2) )</code>
    friend Boolean operator||(const Boolean& b1, const Boolean& b2);
    //! \brief Logical exclusive or.
    //! <code>b1 xor b2</code> is equivalent to <code>(b1 or b2) and ((not b1) or (not b2))</code> and to <code>(b1 or b2) and not (b1 and b2)</code>
    friend Boolean operator^(const Boolean& b1, const Boolean& b2);
    //! \brief Logical equality.
    friend Boolean operator==(const Boolean& b1, const Boolean& b2);

    /** @brief Return the logical conjunction. */
    friend Boolean operator&&(const Boolean& l1, const Bool& l2) { return l1 && Boolean(l2); }
    /** @brief Return the logical conjunction. */
    friend Boolean operator&&(const Bool& l1, const Boolean& l2) { return Boolean(l1) && l2; }
    /** @brief Return the logical disjunction. */
    friend Boolean operator||(const Boolean& l1, const Bool& l2) { return l1 || Boolean(l2); }
    /** @brief Return the logical disjunction. */
    friend Boolean operator||(const Bool& l1, const Boolean& l2) { return Boolean(l1) || l2; }
};

//! \ingroup LogicSubModule
//! \brief A logical class representing the result of a verifyable predicate
//! over a continuous (uncountable) data type which has no robustly \c false instances,
//! such as inequality on a Hausdoff space.
class Sierpinskian : public Logical<Sierpinskian, LogicalHandle, NegatedSierpinskian, Kleenean> {
    typedef Logical<Sierpinskian, LogicalHandle, NegatedSierpinskian, Kleenean> Base;

  public:
    /** @brief Construct a \c Sierpinskian from l. */
    explicit Sierpinskian(LogicalValue l) : Base(LogicalHandle::constant(l)) {}
    /** @brief Construct a \c Sierpinskian from l. */
    explicit Sierpinskian(LogicalHandle l) : Base(l) {}
    /** @brief Construct a \c Sierpinskian from b. */
    explicit Sierpinskian(bool b = true) : Sierpinskian(Detail::make_logical_value(b)) {}
    /** @brief Construct a \c Sierpinskian with the specified characteristics. */
    Sierpinskian(Indeterminate) : Sierpinskian(LogicalValue::INDETERMINATE) {}
    //! \brief Check the value using effort \a e.
    ValidatedSierpinskian check(Effort e) const;
    //! \brief Check the value of \a s using effort \a e.
    friend ValidatedSierpinskian check(const Sierpinskian& s, Effort e);

    //! \brief Logical negation.
    friend NegatedSierpinskian operator!(const Sierpinskian& l);
    //! \brief Logical conjunction.
    friend Sierpinskian operator&&(const Sierpinskian& l1, const Sierpinskian& l2);
    //! \brief Logical disjunction.
    friend Sierpinskian operator||(const Sierpinskian& l1, const Sierpinskian& l2);

    /** @brief Return the logical disjunction. */
    friend const Sierpinskian& operator||(const Sierpinskian& l1, const Boolean&) { return l1; }
    /** @brief Return the logical disjunction. */
    friend const Sierpinskian& operator||(const Boolean&, const Sierpinskian& l2) { return l2; }

    /** @brief Return the logical conjunction. */
    friend Kleenean operator&&(const Kleenean&, const Kleenean&);
    /** @brief Return the logical disjunction. */
    friend Kleenean operator||(const Kleenean&, const Kleenean&);
};

//! \ingroup LogicSubModule
//! \brief A logical class representing the result of a falsifyable predicate
//! over a continuous (uncountable) data type which has no robustly \c true instances,
//! such as equality on a Hausdoff space.
class NegatedSierpinskian : public Logical<NegatedSierpinskian, LogicalHandle, Sierpinskian, Kleenean> {
    typedef Logical<NegatedSierpinskian, LogicalHandle, Sierpinskian, Kleenean> Base;

  public:
    /** @brief Construct a \c NegatedSierpinskian from l. */
    explicit NegatedSierpinskian(LogicalValue l) : Base(LogicalHandle::constant(l)) {}
    /** @brief Construct a \c NegatedSierpinskian from l. */
    explicit NegatedSierpinskian(LogicalHandle l) : Base(l) {}
    /** @brief Construct a \c NegatedSierpinskian from b. */
    explicit NegatedSierpinskian(bool b) : NegatedSierpinskian(Detail::make_logical_value(b)) {}
    /** @brief Construct a \c NegatedSierpinskian with the specified characteristics. */
    NegatedSierpinskian(Indeterminate) : NegatedSierpinskian(LogicalValue::INDETERMINATE) {}
    //! \brief Check the value using effort \a e.
    ValidatedNegatedSierpinskian check(Effort e) const;
    //! \brief Check the value of \a s using effort \a e.
    friend ValidatedNegatedSierpinskian check(const NegatedSierpinskian& s, Effort e);

    //! \brief Logical negation.
    friend Sierpinskian operator!(const NegatedSierpinskian& l);
    //! \brief Logical conjunction.
    friend NegatedSierpinskian operator&&(const NegatedSierpinskian& l1, const NegatedSierpinskian& l2);
    //! \brief Logical disjunction.
    friend NegatedSierpinskian operator||(const NegatedSierpinskian& l1, const NegatedSierpinskian& l2);

    /** @brief Return the logical conjunction. */
    friend const NegatedSierpinskian& operator&&(const NegatedSierpinskian& l1, const Boolean&) { return l1; }
    /** @brief Return the logical conjunction. */
    friend const NegatedSierpinskian& operator&&(const Boolean&, const NegatedSierpinskian& l2) { return l2; }
    /** @brief Return the logical conjunction. */
    friend Kleenean operator&&(const Kleenean&, const Kleenean&);
    /** @brief Return the logical disjunction. */
    friend Kleenean operator||(const Kleenean&, const Kleenean&);
};

//! \ingroup LogicSubModule
//! \brief A logical class representing the result of a quasidecidable predicate
//! over a continuous (uncountable) data type.
//! \details A canonical example is (strict) positivity of a real number,
//! which is undecidable for zero, since positivity cannot be determined from arbitrarily accurate approximations.
class Kleenean : public Logical<Kleenean, LogicalHandle> {
    typedef Logical<Kleenean, LogicalHandle> Base;

  public:
    /** @brief Construct a \c Kleenean from l. */
    explicit Kleenean(const LogicalHandle& l) : Base(l) {}
    /** @brief Construct a \c Kleenean from l. */
    explicit Kleenean(LogicalValue l) : Base(LogicalHandle::constant(l)) {} // FIXME: Remove

  public:
    //! \brief Convert from a built-in boolean value, defaulting to \c true.
    Kleenean(bool b = true) : Kleenean(Boolean(b)) {}
    //! \brief Convert from the %Indeterminate constant value.
    Kleenean(Indeterminate) : Kleenean(LogicalValue::INDETERMINATE) {}
    //! \brief Convert from a %Boolean constant value. May be checked with effort 0.
    Kleenean(Boolean b) : Kleenean(b.repr()) {}
    //! \brief Convert from a %Sierpinskian predicate.
    Kleenean(Sierpinskian s) : Kleenean(s.repr()) {}
    //! \brief Convert from a negated %Sierpinskian predicate.
    Kleenean(NegatedSierpinskian ns) : Kleenean(ns.repr()) {}
    //! \brief Check the value using effort \a e.
    ValidatedKleenean check(Effort e) const;
    //! \brief Check the value of \a k using effort \a e.
    friend ValidatedKleenean check(const Kleenean& k, Effort e);

    //! \brief Logical negation.
    friend Kleenean operator!(const Kleenean& k);
    //! \brief Logical conjunction.
    friend Kleenean operator&&(const Kleenean& k1, const Kleenean& k2);
    //! \brief Logical disjunction.
    friend Kleenean operator||(const Kleenean& k1, const Kleenean& k2);
    //! \brief Logical exclusive or.
    friend Kleenean operator^(const Kleenean& k1, const Kleenean& k2);
};

//! \ingroup LogicSubModule
//! \brief A logical class representing the result of a verifyable predicate
//! over a continuous (uncountable) data type.
//! \details A canonical example is strict positivity of a real number given only lower bounds to its value,
//! since sufficient accurate lower bounds for a strictly positive number are themselves strictly positive,
//! but negativity cannot be deduced given only lower bounds to a negative number.
class LowerKleenean : public Logical<LowerKleenean, LogicalHandle, UpperKleenean> {
    typedef Logical<LowerKleenean, LogicalHandle, UpperKleenean> Base;

  public:
    /** @brief Construct a \c LowerKleenean from l. */
    explicit LowerKleenean(const LogicalHandle& l) : Base(l) {}
    /** @brief Construct a \c LowerKleenean from b. */
    LowerKleenean(bool b) : LowerKleenean(Boolean(b)) {}
    /** @brief Construct a \c LowerKleenean with the specified characteristics. */
    LowerKleenean(Indeterminate) : LowerKleenean(LogicalHandle::constant(LogicalValue::INDETERMINATE)) {}
    /** @brief Construct a \c LowerKleenean from b. */
    LowerKleenean(Boolean b) : LowerKleenean(Kleenean(b)) {}
    /** @brief Construct a \c LowerKleenean from s. */
    LowerKleenean(Sierpinskian s) : LowerKleenean(Kleenean(s)) {}
    //! \brief Convert from a %Kleenean predicate.
    LowerKleenean(Kleenean k) : LowerKleenean(k.repr()) {}
    //! \brief Check the value using effort \a e.
    ValidatedLowerKleenean check(Effort e) const;
    //! \brief Check the value of \a lk using effort \a e.
    friend ValidatedLowerKleenean check(const LowerKleenean& lk, Effort e);

    //! \brief Logical negation.
    friend UpperKleenean operator!(const LowerKleenean& l);
    //! \brief Logical conjunction.
    friend LowerKleenean operator&&(const LowerKleenean& l1, const LowerKleenean& l2);
    //! \brief Logical disjunction.
    friend LowerKleenean operator||(const LowerKleenean& l1, const LowerKleenean& l2);
};

//! \ingroup LogicSubModule
//! \brief A logical class representing the result of a falsifyable predicate
//! over a continuous (uncountable) data type.
class UpperKleenean : public Logical<UpperKleenean, LogicalHandle, LowerKleenean> {
    typedef Logical<UpperKleenean, LogicalHandle, LowerKleenean> Base;

  public:
    /** @brief Construct a \c UpperKleenean from l. */
    explicit UpperKleenean(const LogicalHandle& l) : Base(l) {}
    /** @brief Construct a \c UpperKleenean from b. */
    UpperKleenean(bool b) : UpperKleenean(Kleenean(b)) {}
    /** @brief Construct a \c UpperKleenean with the specified characteristics. */
    UpperKleenean(Indeterminate) : UpperKleenean(LogicalHandle::constant(LogicalValue::INDETERMINATE)) {}
    /** @brief Construct a \c UpperKleenean from b. */
    UpperKleenean(Boolean b) : UpperKleenean(Kleenean(b)) {}
    /** @brief Construct a \c UpperKleenean from ns. */
    UpperKleenean(NegatedSierpinskian ns) : UpperKleenean(Kleenean(ns)) {}
    //! \brief Convert from a %Kleenean predicate.
    UpperKleenean(Kleenean k) : UpperKleenean(k.repr()) {}
    //! \brief Check the value using effort \a e.
    ValidatedUpperKleenean check(Effort e) const;
    //! \brief Check the value of \a uk using effort \a e.
    friend ValidatedUpperKleenean check(const UpperKleenean& uk, Effort e);

    //! \brief Logical negation.
    friend LowerKleenean operator!(const UpperKleenean& l);
    //! \brief Logical conjunction.
    friend UpperKleenean operator&&(const UpperKleenean& l1, const UpperKleenean& l2);
    //! \brief Logical disjunction.
    friend UpperKleenean operator||(const UpperKleenean& l1, const UpperKleenean& l2);
};

//! \ingroup LogicSubModule
//! \brief A logical class representing the result of a undecidable predicate
//! over a continuous (uncountable) data type.
//! \details A canonical example is (strict) positivity of a real number given only a convergent sequence,
//! since without error bounds we can never know whether a given approximation is sufficiently close to
//! deduce any properties of the number.
class NaiveKleenean : public Logical<NaiveKleenean, LogicalHandle> {
    typedef Logical<NaiveKleenean, LogicalHandle> Base;

  public:
    /** @brief Construct a \c NaiveKleenean from l. */
    explicit NaiveKleenean(const LogicalHandle& l) : Base(l) {}
    /** @brief Construct a \c NaiveKleenean from b. */
    NaiveKleenean(bool b) : NaiveKleenean(Kleenean(b)) {}
    /** @brief Construct a \c NaiveKleenean from i. */
    NaiveKleenean(Indeterminate i) : NaiveKleenean(Kleenean(i)) {}
    /** @brief Construct a \c NaiveKleenean from b. */
    NaiveKleenean(Boolean b) : NaiveKleenean(Kleenean(b)) {}
    /** @brief Construct a \c NaiveKleenean from s. */
    NaiveKleenean(Sierpinskian s) : NaiveKleenean(Kleenean(s)) {}
    /** @brief Construct a \c NaiveKleenean from ns. */
    NaiveKleenean(NegatedSierpinskian ns) : NaiveKleenean(Kleenean(ns)) {}
    //! \brief Convert from a %Kleenean predicate.
    NaiveKleenean(Kleenean k) : NaiveKleenean(k.repr()) {}
    //! \brief Convert from a %LowerKleenean predicate.
    NaiveKleenean(LowerKleenean lk) : NaiveKleenean(lk.repr()) {}
    //! \brief Convert from an %UpperKleenean predicate.
    NaiveKleenean(UpperKleenean uk) : NaiveKleenean(uk.repr()) {}
    //! \brief Check the value using effort \a e.
    ApproximateKleenean check(Effort e) const;
    //! \brief Check the value of \a ak using effort \a e.
    friend ApproximateKleenean check(const NaiveKleenean& ak, Effort e);

    //! \brief Logical negation.
    friend NaiveKleenean operator!(const NaiveKleenean& l);
    //! \brief Logical conjunction.
    friend NaiveKleenean operator&&(const NaiveKleenean& l1, const NaiveKleenean& l2);
    //! \brief Logical disjunction.
    friend NaiveKleenean operator||(const NaiveKleenean& l1, const NaiveKleenean& l2);
};

//! \sa LogicSubModule
//! \brief A logical class representing the result of a verifyable predicate, checked to some given effort.
//! \sa Sierpinskian
class ValidatedSierpinskian : public Logical<ValidatedSierpinskian, LogicalValue, ValidatedNegatedSierpinskian> {
    typedef Logical<ValidatedSierpinskian, LogicalValue, ValidatedNegatedSierpinskian> Base;

  public:
    /** @brief Construct a \c ValidatedSierpinskian from lv. */
    explicit ValidatedSierpinskian(LogicalValue lv) : Base(lv || LogicalValue::UNLIKELY) {}
    /** @brief Construct a \c ValidatedSierpinskian from b. */
    ValidatedSierpinskian(bool b) : ValidatedSierpinskian(b ? LogicalValue::TRUE : LogicalValue::UNLIKELY) {}
    /** @brief Construct a \c ValidatedSierpinskian from s, e. */
    ValidatedSierpinskian(Sierpinskian s, Effort e) : ValidatedSierpinskian(s.check(e)) {}
    /** @brief Check the predicate at the supplied effort or validate the supplied data. */
    friend ValidatedSierpinskian check(const Sierpinskian& s, Effort e) { return s.check(e); }
};

//! \ingroup LogicSubModule
//! \brief A logical class representing the result of a falsifyable predicate, checked using some given effort.
//! \sa Sierpinskian, NegatedSierpinskian
class ValidatedNegatedSierpinskian : public Logical<ValidatedNegatedSierpinskian, LogicalValue, ValidatedSierpinskian> {
    typedef Logical<ValidatedNegatedSierpinskian, LogicalValue, ValidatedSierpinskian> Base;

  public:
    /** @brief Construct a \c ValidatedNegatedSierpinskian from lv. */
    explicit ValidatedNegatedSierpinskian(LogicalValue lv) : Base(lv && LogicalValue::LIKELY) {}
    /** @brief Construct a \c ValidatedNegatedSierpinskian from b. */
    ValidatedNegatedSierpinskian(bool b) : ValidatedNegatedSierpinskian(b ? LogicalValue::LIKELY : LogicalValue::FALSE) {}
    /** @brief Construct a \c ValidatedNegatedSierpinskian from ns, e. */
    ValidatedNegatedSierpinskian(NegatedSierpinskian ns, Effort e) : ValidatedNegatedSierpinskian(ns.check(e)) {}
    /** @brief Check the predicate at the supplied effort or validate the supplied data. */
    friend ValidatedNegatedSierpinskian check(const NegatedSierpinskian& ns, Effort e) { return ns.check(e); }
};

//! \ingroup LogicSubModule
//! \brief A logical class representing a value of a quasidecidable predicate,
//! either a Kleenean value \ref Kleenean::check "check"ed using some \ref Effort, or applied directly to a Validated object.
//! \sa Kleenean
class ValidatedKleenean : public Logical<ValidatedKleenean, LogicalValue> {
    typedef Logical<ValidatedKleenean, LogicalValue> Base;

  public:
    /** @brief Construct a \c ValidatedKleenean with the specified characteristics. */
    explicit ValidatedKleenean() : ValidatedKleenean(true) {} // FIXME: Remove
    /** @brief Construct a \c ValidatedKleenean from lv. */
    explicit ValidatedKleenean(LogicalValue lv) : Base(lv) {}
    /** @brief Construct a \c ValidatedKleenean from b. */
    ValidatedKleenean(bool b) : ValidatedKleenean(Boolean(b)) {}
    /** @brief Construct a \c ValidatedKleenean from b. */
    ValidatedKleenean(Boolean b) : ValidatedKleenean(static_cast<LogicalValue>(b)) {}
    /** @brief Construct a \c ValidatedKleenean from s. */
    explicit ValidatedKleenean(ValidatedSierpinskian s) : ValidatedKleenean(static_cast<LogicalValue>(s)) {}
    /** @brief Construct a \c ValidatedKleenean from ns. */
    explicit ValidatedKleenean(ValidatedNegatedSierpinskian ns) : ValidatedKleenean(static_cast<LogicalValue>(ns)) {}
    /** @brief Construct a \c ValidatedKleenean from k, e. */
    ValidatedKleenean(Kleenean k, Effort e) : ValidatedKleenean(k.check(e)) {}
    /** @brief Check the predicate at the supplied effort or validate the supplied data. */
    friend ValidatedKleenean check(const Kleenean& k, Effort e) { return k.check(e); }
};

//! \ingroup LogicSubModule
//! \brief A logical class representing the result of checking a verifyable predicate.
//! \sa LowerKleenean
class ValidatedLowerKleenean : public Logical<ValidatedLowerKleenean, LogicalValue, ValidatedUpperKleenean> {
    typedef Logical<ValidatedLowerKleenean, LogicalValue, ValidatedUpperKleenean> Base;

  public:
    /** @brief Construct a \c ValidatedLowerKleenean from lv. */
    explicit ValidatedLowerKleenean(LogicalValue lv) : Base(lv || LogicalValue::UNLIKELY) {}
    /** @brief Construct a \c ValidatedLowerKleenean from b. */
    ValidatedLowerKleenean(bool b) : ValidatedLowerKleenean(Boolean(b)) {}
    /** @brief Construct a \c ValidatedLowerKleenean from b. */
    ValidatedLowerKleenean(Boolean b) : ValidatedLowerKleenean(static_cast<LogicalValue>(b)) {}
    /** @brief Construct a \c ValidatedLowerKleenean from s. */
    ValidatedLowerKleenean(ValidatedSierpinskian s) : ValidatedLowerKleenean(static_cast<LogicalValue>(s)) {}
    /** @brief Construct a \c ValidatedLowerKleenean from k. */
    ValidatedLowerKleenean(ValidatedKleenean k) : ValidatedLowerKleenean(static_cast<LogicalValue>(k)) {}
    /** @brief Construct a \c ValidatedLowerKleenean from lk, e. */
    ValidatedLowerKleenean(LowerKleenean lk, Effort e) : ValidatedLowerKleenean(lk.check(e)) {}
    /** @brief Check the predicate at the supplied effort or validate the supplied data. */
    friend ValidatedLowerKleenean check(const LowerKleenean& lk, Effort e) { return lk.check(e); }
};

//! \ingroup LogicSubModule
//! \brief A logical class representing the result of checking a falsifyable predicate.
//! \sa UpperKleenean
class ValidatedUpperKleenean : public Logical<ValidatedUpperKleenean, LogicalValue, ValidatedLowerKleenean> {
    typedef Logical<ValidatedUpperKleenean, LogicalValue, ValidatedLowerKleenean> Base;

  public:
    /** @brief Construct a \c ValidatedUpperKleenean from lv. */
    explicit ValidatedUpperKleenean(LogicalValue lv) : Base(lv && LogicalValue::LIKELY) {}
    /** @brief Construct a \c ValidatedUpperKleenean from b. */
    ValidatedUpperKleenean(bool b) : ValidatedUpperKleenean(Boolean(b)) {}
    /** @brief Construct a \c ValidatedUpperKleenean from b. */
    ValidatedUpperKleenean(Boolean b) : ValidatedUpperKleenean(static_cast<LogicalValue>(b)) {}
    /** @brief Construct a \c ValidatedUpperKleenean from k. */
    ValidatedUpperKleenean(ValidatedKleenean k) : ValidatedUpperKleenean(static_cast<LogicalValue>(k)) {}
    /** @brief Construct a \c ValidatedUpperKleenean from ns. */
    ValidatedUpperKleenean(ValidatedNegatedSierpinskian ns) : ValidatedUpperKleenean(static_cast<LogicalValue>(ns)) {}
    /** @brief Construct a \c ValidatedUpperKleenean from uk, e. */
    ValidatedUpperKleenean(UpperKleenean uk, Effort e) : ValidatedUpperKleenean(uk.check(e)) {}
    /** @brief Check the predicate at the supplied effort or validate the supplied data. */
    friend ValidatedUpperKleenean check(const UpperKleenean& uk, Effort e) { return uk.check(e); }
};

//! \ingroup LogicSubModule
//! \brief A logical class representing a value of an undecidable predicate,
//! either a NaiveKleenean value \ref NaiveKleenean::check "check"ed using some Effort, or applied directly to an Approximate object.
//! \details Note that definitely() and possibly() always return \c false and \c true, respectively.
//! \sa NaiveKleenean
class ApproximateKleenean : public Logical<ApproximateKleenean, LogicalValue> {
    typedef Logical<ApproximateKleenean, LogicalValue> Base;

  public:
    /** @brief Construct a \c ApproximateKleenean from lv. */
    explicit ApproximateKleenean(LogicalValue lv) : Base((lv && LogicalValue::LIKELY) || LogicalValue::UNLIKELY) {}
    /** @brief Construct a \c ApproximateKleenean from b. */
    ApproximateKleenean(bool b) : ApproximateKleenean(Boolean(b)) {}
    /** @brief Construct a \c ApproximateKleenean from b. */
    ApproximateKleenean(Boolean b) : ApproximateKleenean(static_cast<LogicalValue>(b)) {}
    /** @brief Construct a \c ApproximateKleenean from k. */
    ApproximateKleenean(ValidatedKleenean k) : ApproximateKleenean(static_cast<LogicalValue>(k)) {}
    /** @brief Construct a \c ApproximateKleenean from lk. */
    ApproximateKleenean(ValidatedLowerKleenean lk) : ApproximateKleenean(static_cast<LogicalValue>(lk)) {}
    /** @brief Construct a \c ApproximateKleenean from uk. */
    ApproximateKleenean(ValidatedUpperKleenean uk) : ApproximateKleenean(static_cast<LogicalValue>(uk)) {}
    /** @brief Construct a \c ApproximateKleenean from k, e. */
    ApproximateKleenean(Kleenean k, Effort e) : ApproximateKleenean(k.check(e)) {}
};

inline Indeterminate::operator ValidatedSierpinskian() const {
    return ValidatedSierpinskian(LogicalValue::INDETERMINATE);
}
inline Indeterminate::operator ValidatedKleenean() const {
    return ValidatedKleenean(LogicalValue::INDETERMINATE);
}
/** @brief Convert the logical predicate to a Boolean decision. */
inline Bool decide(const Indeterminate&, Effort) {
    return decide(ValidatedKleenean(LogicalValue::INDETERMINATE));
}
/** @brief Convert the logical predicate to a Boolean decision. */
inline Bool decide(const Indeterminate&) {
    return decide(ValidatedKleenean(LogicalValue::INDETERMINATE));
}

inline ValidatedSierpinskian Sierpinskian::check(Effort eff) const {
    return ValidatedSierpinskian(this->repr().check(eff));
}
inline ValidatedNegatedSierpinskian NegatedSierpinskian::check(Effort eff) const {
    return ValidatedNegatedSierpinskian(this->repr().check(eff));
}
inline ValidatedKleenean Kleenean::check(Effort e) const {
    return ValidatedKleenean(this->repr().check(e));
}
inline ValidatedLowerKleenean LowerKleenean::check(Effort e) const {
    return ValidatedLowerKleenean(this->repr().check(e));
}
inline ValidatedUpperKleenean UpperKleenean::check(Effort e) const {
    return ValidatedUpperKleenean(this->repr().check(e));
}
inline ApproximateKleenean NaiveKleenean::check(Effort e) const {
    return ApproximateKleenean(this->repr().check(e));
}

/** @brief Boolean decision selected from partially decidable alternatives. */
class NondeterministicBoolean {
    LowerKleenean _pt;
    LowerKleenean _pf;
    Bool _r;

  public:
    /** @brief Construct a \c NondeterministicBoolean from pt, pf. */
    NondeterministicBoolean(LowerKleenean pt, LowerKleenean pf) : _pt(pt), _pf(pf), _r(_choose(pt, pf)) {}
    /** @brief Convert to bool. */
    operator bool() const { return _r; }

  private:
    /** @brief Choose an alternative whose predicate can be established. */
    static Bool _choose(LowerKleenean pt, LowerKleenean pf);
};
/** @brief Choose an alternative whose predicate can be established. */
inline NondeterministicBoolean choose(LowerKleenean pt, LowerKleenean pf) {
    return NondeterministicBoolean(pt, pf);
}

//! \relates LowerKleenean
//! Returns an index \a i such that \c p[i] is definitely \c true, if one exists.
//! Loops infinitely if no \c p[i] can be shown to be definitely \c true.
SizeType nondeterministic_choose_index(const Array<LowerKleenean>& p);

/** @brief Conditional alternative pairing a logical predicate with a term. */
template<class P, class T>
class Case {
    P _p;
    T _t;

  public:
    /** @brief Construct a \c Case from p, t. */
    Case(P p, T t) : _p(p), _t(t) {}
    /** @brief Return the branch predicate. */
    P condition() const { return _p; }
    /** @brief Return the branch value. */
    T term() const { return _t; }
};

} // namespace Ariadne

#endif
