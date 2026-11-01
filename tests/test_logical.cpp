/***************************************************************************
 *            test_logical.cpp
 *
 *  Copyright  2015-20  Pieter Collins
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

#include "utility/metaprogramming.hpp"
#include "utility/array.hpp"
#include "foundation/paradigm.hpp"
#include "foundation/logical.hpp"

#include "ariadne-testing.hpp"

#include <limits>

using namespace Ariadne;


namespace {

bool same_logical_value(LogicalValue lhs, LogicalValue rhs) {
    return static_cast<ComparableEnumerationType>(lhs) == static_cast<ComparableEnumerationType>(rhs);
}

class DelayedLogical final : public LogicalInterface {
    Nat _threshold;
    LogicalValue _value;
  public:
    DelayedLogical(Nat threshold, LogicalValue value)
        : _threshold(threshold), _value(value) { }
  private:
    LogicalInterface* _copy() const override { return new DelayedLogical(*this); }
    LogicalValue _check(Effort effort) const override {
        return effort.work() >= _threshold ? _value : LogicalValue::INDETERMINATE;
    }
    OutputStream& _write(OutputStream& os) const override {
        return os << "delayed(" << _threshold << ")";
    }
};

class TrackedLogical final : public LogicalInterface {
    Int& _destructions;
    LogicalValue _value;
  public:
    TrackedLogical(Int& destructions, LogicalValue value)
        : _destructions(destructions), _value(value) { }
    ~TrackedLogical() override { ++_destructions; }
  private:
    LogicalInterface* _copy() const override { return new TrackedLogical(*this); }
    LogicalValue _check(Effort) const override { return _value; }
    OutputStream& _write(OutputStream& os) const override { return os << "tracked"; }
};

} // namespace


class TestLogical : public Ariadne::Testing::ContextFixture
{
  public:
    using ContextFixture::ContextFixture;
    Void test_concept();
    Void test_conversion_to_bool();
    Void test_conversion();
    Void test_effort();
    Void test_logical_values();
    Void test_handles();
    Void test_owned_pointer_conversion();
    Void test_types();
    Void test_nondeterminism();
    Void test_class_names();
};


int main() {
    std::cout<<std::setprecision(20);
    std::cerr<<std::setprecision(20);
    ARIADNE_TESTING_SUITE("logical");
    ARIADNE_TESTING_ADD_TEST(TestLogical, test_conversion_to_bool);
    ARIADNE_TESTING_ADD_TEST(TestLogical, test_conversion);
    ARIADNE_TESTING_ADD_TEST(TestLogical, test_effort);
    ARIADNE_TESTING_ADD_TEST(TestLogical, test_logical_values);
    ARIADNE_TESTING_ADD_TEST(TestLogical, test_handles);
    ARIADNE_TESTING_ADD_TEST(TestLogical, test_owned_pointer_conversion);
    ARIADNE_TESTING_ADD_TEST(TestLogical, test_types);
    ARIADNE_TESTING_ADD_TEST(TestLogical, test_nondeterminism);
    ARIADNE_TESTING_ADD_TEST(TestLogical, test_class_names);
    return ARIADNE_TESTING_RUN_TESTS();
}


Void
TestLogical::test_concept()
{
    // Check to see if we can perform operations on computational and specification logical types
    LogicalType<ExactTag> xl(true);
    LogicalType<EffectiveTag> el(true);
    LogicalType<ValidatedTag> vl(true);
    Effort eff(0);

    vl=el.check(eff);
    vl=check(el,eff);

    vl=indeterminate;
    vl=LogicalType<ValidatedTag>(LogicalValue::LIKELY);

    xl = xl && xl;
    el = xl && el;
    vl = xl && vl;
    el = el && el;
    vl = vl && vl;
}

Void
TestLogical::test_conversion_to_bool()
{
    ARIADNE_TESTING_PROVE((Convertible<Boolean,Bool>));
    ARIADNE_TESTING_PROVE((not Convertible<Sierpinskian,Bool>));
    ARIADNE_TESTING_PROVE((not Convertible<NegatedSierpinskian,Bool>));
    ARIADNE_TESTING_PROVE((not Convertible<Kleenean,Bool>));
    ARIADNE_TESTING_PROVE((not Convertible<LowerKleenean,Bool>));
    ARIADNE_TESTING_PROVE((not Convertible<UpperKleenean,Bool>));
    ARIADNE_TESTING_PROVE((not Convertible<ValidatedKleenean,Bool>));
    ARIADNE_TESTING_PROVE((not Convertible<ValidatedUpperKleenean,Bool>));
    ARIADNE_TESTING_PROVE((not Convertible<ValidatedLowerKleenean,Bool>));
    ARIADNE_TESTING_PROVE((not Convertible<ApproximateKleenean,Bool>));
}

Void
TestLogical::test_conversion()
{
    if(Convertible<LogicalType<EffectiveTag>,LogicalType<ValidatedTag>>) {
        this->ctx().note("EffectiveTag logical types may be converted to values using default Effort.");
    } else if(Convertible<LogicalType<EffectiveTag>,LogicalType<ValidatedTag>>) {
        this->ctx().note("EffectiveTag logical types may be explicitly converted to values using default Effort.");
    } else {
        this->ctx().note("EffectiveTag logical types cannot be converted to values; the Effort used must be specified.");
    }

    try {
        if(decide(indeterminate)) {
            this->ctx().note("decide(...) is true on INDETERMINATE value.");
        } else {
            this->ctx().note("decide(...) is false on INDETERMINATE value.");
        }
    } catch(...) {
        this->ctx().note("decide(...) is throws error on INDETERMINATE value.");
    }

    ARIADNE_TESTING_PROVE((not Convertible<Indeterminate,Boolean>));
    ARIADNE_TESTING_PROVE((Convertible<Indeterminate,Sierpinskian>));
    ARIADNE_TESTING_PROVE((Convertible<Indeterminate,NegatedSierpinskian>));
    ARIADNE_TESTING_PROVE((Convertible<Indeterminate,Kleenean>));
    ARIADNE_TESTING_PROVE((Convertible<Indeterminate,LowerKleenean>));
    ARIADNE_TESTING_PROVE((Convertible<Indeterminate,UpperKleenean>));
//    ARIADNE_TESTING_PROVE((Same<decltype(indeterminate and true),Kleenean>));
//    ARIADNE_TESTING_PROVE((Same<decltype(indeterminate and Boolean(true)),Kleenean>));
    ARIADNE_TESTING_PROVE((Same<decltype(indeterminate and Sierpinskian(true)),Sierpinskian>));
    ARIADNE_TESTING_PROVE((Same<decltype(indeterminate and Kleenean(true)),Kleenean>));

    LogicalType<ValidatedTag> vl(LogicalValue::LIKELY); this->ctx().inspect("vl", vl);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(vl),false);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vl),true);
    ARIADNE_TESTING_EXPECT_EQUAL(decide(vl),true);

    LogicalType<ValidatedTag> vi(LogicalValue::INDETERMINATE); this->ctx().inspect("vi", vi);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(vl),false);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vl),true);
}


Void
TestLogical::test_effort()
{
    Effort::set_default(3u);
    ARIADNE_TESTING_EXPECT_EQUAL(Effort::get_default().work(), Nat(3u));

    Effort effort(2u);
    ARIADNE_TESTING_EXPECT_EQUAL(effort.work(), Nat(2u));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<Nat>(effort), Nat(2u));
    ++effort;
    ARIADNE_TESTING_EXPECT_EQUAL(effort.work(), Nat(3u));
    effort += 2u;
    ARIADNE_TESTING_EXPECT_EQUAL(effort.work(), Nat(5u));
    effort *= 2u;
    ARIADNE_TESTING_EXPECT_EQUAL(effort.work(), Nat(10u));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(effort), String("Effort(10)"));

    Effort literal = 7_eff;
    ARIADNE_TESTING_EXPECT_EQUAL(literal.work(), Nat(7u));
    ARIADNE_TESTING_EXPECT_THROWS(std::overflow_error, operator""_eff(static_cast<unsigned long long int>(std::numeric_limits<Nat>::max()) + 1ull));
    Effort::set_default(0u);
}

Void
TestLogical::test_logical_values()
{
    using Detail::decide;
    using Detail::definitely;
    using Detail::is_determinate;
    using Detail::is_indeterminate;
    using Detail::make_logical_value;
    using Detail::possibly;
    using Detail::probably;

    ARIADNE_TESTING_PROVE(same_logical_value(make_logical_value(true), LogicalValue::TRUE));
    ARIADNE_TESTING_PROVE(same_logical_value(make_logical_value(false), LogicalValue::FALSE));

    ARIADNE_TESTING_EXPECT_EQUAL(Ariadne::definitely(true), true);
    ARIADNE_TESTING_EXPECT_EQUAL(Ariadne::probably(false), false);
    ARIADNE_TESTING_EXPECT_EQUAL(Ariadne::decide(true), true);
    ARIADNE_TESTING_EXPECT_EQUAL(Ariadne::possibly(false), false);

    ARIADNE_TESTING_EXPECT_EQUAL(definitely(LogicalValue::TRUE), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(LogicalValue::LIKELY), false);
    ARIADNE_TESTING_EXPECT_EQUAL(probably(LogicalValue::LIKELY), true);
    ARIADNE_TESTING_EXPECT_EQUAL(probably(LogicalValue::INDETERMINATE), false);
    ARIADNE_TESTING_EXPECT_EQUAL(decide(LogicalValue::LIKELY), true);
    ARIADNE_TESTING_EXPECT_EQUAL(decide(LogicalValue::UNLIKELY), false);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(LogicalValue::FALSE), false);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(LogicalValue::UNLIKELY), true);
    ARIADNE_TESTING_EXPECT_EQUAL(is_determinate(LogicalValue::TRUE), true);
    ARIADNE_TESTING_EXPECT_EQUAL(is_determinate(LogicalValue::FALSE), true);
    ARIADNE_TESTING_EXPECT_EQUAL(is_determinate(LogicalValue::INDETERMINATE), false);
    ARIADNE_TESTING_EXPECT_EQUAL(is_indeterminate(LogicalValue::INDETERMINATE), true);
    ARIADNE_TESTING_EXPECT_EQUAL(is_indeterminate(LogicalValue::UNLIKELY), true);
    ARIADNE_TESTING_EXPECT_EQUAL(is_indeterminate(LogicalValue::LIKELY), true);
    ARIADNE_TESTING_EXPECT_EQUAL(is_indeterminate(LogicalValue::TRUE), false);

    ARIADNE_TESTING_PROVE(same_logical_value(!LogicalValue::TRUE, LogicalValue::FALSE));
    ARIADNE_TESTING_PROVE(same_logical_value(!LogicalValue::LIKELY, LogicalValue::UNLIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::LIKELY && LogicalValue::UNLIKELY, LogicalValue::UNLIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::LIKELY || LogicalValue::UNLIKELY, LogicalValue::LIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::TRUE ^ LogicalValue::FALSE, LogicalValue::TRUE));

    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::TRUE == LogicalValue::LIKELY, LogicalValue::LIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::LIKELY == LogicalValue::TRUE, LogicalValue::LIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::LIKELY == LogicalValue::FALSE, LogicalValue::UNLIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::LIKELY == LogicalValue::INDETERMINATE, LogicalValue::INDETERMINATE));
    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::INDETERMINATE == LogicalValue::TRUE, LogicalValue::INDETERMINATE));
    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::UNLIKELY == LogicalValue::TRUE, LogicalValue::UNLIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::UNLIKELY == LogicalValue::FALSE, LogicalValue::LIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::UNLIKELY == LogicalValue::LIKELY, LogicalValue::UNLIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(LogicalValue::FALSE == LogicalValue::LIKELY, LogicalValue::UNLIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(static_cast<LogicalValue>(99) == LogicalValue::TRUE, LogicalValue::INDETERMINATE));

    ARIADNE_TESTING_EXPECT_EQUAL(to_string(LogicalValue::TRUE), String("true"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(LogicalValue::LIKELY), String("likely"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(LogicalValue::INDETERMINATE), String("indeterminate"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(LogicalValue::UNLIKELY), String("unlikely"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(LogicalValue::FALSE), String("false"));
    ARIADNE_TESTING_EXPECT_THROWS(std::runtime_error, to_string(static_cast<LogicalValue>(99)));
}

Void
TestLogical::test_handles()
{
    Effort effort(0u);
    LogicalHandle t = LogicalHandle::constant(LogicalValue::TRUE);
    LogicalHandle f = LogicalHandle::constant(LogicalValue::FALSE);
    LogicalHandle l = LogicalHandle::constant(LogicalValue::LIKELY);

    ARIADNE_TESTING_PROVE(same_logical_value(t.check(effort), LogicalValue::TRUE));
    ARIADNE_TESTING_PROVE(same_logical_value(Detail::check(l, effort), LogicalValue::LIKELY));
    ARIADNE_TESTING_EXPECT_EQUAL(Detail::definitely(t, effort), true);
    ARIADNE_TESTING_EXPECT_EQUAL(Detail::probably(l, effort), true);
    ARIADNE_TESTING_EXPECT_EQUAL(Detail::decide(l, effort), true);
    ARIADNE_TESTING_EXPECT_EQUAL(Detail::possibly(f, effort), false);
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(t), String("true"));

    LogicalHandle a = t && f;
    LogicalHandle o = t || f;
    LogicalHandle e = t == l;
    LogicalHandle x = t ^ f;
    LogicalHandle n = !l;

    ARIADNE_TESTING_PROVE(same_logical_value(a.check(effort), LogicalValue::FALSE));
    ARIADNE_TESTING_PROVE(same_logical_value(o.check(effort), LogicalValue::TRUE));
    ARIADNE_TESTING_PROVE(same_logical_value(e.check(effort), LogicalValue::LIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(x.check(effort), LogicalValue::TRUE));
    ARIADNE_TESTING_PROVE(same_logical_value(n.check(effort), LogicalValue::UNLIKELY));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(a), String("and(true,false)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(o), String("or(true,false)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(e), String("equal(true,likely)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(x), String("xor(true,false)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(n), String("not(likely)"));

    LogicalHandle ca = Detail::conjunction(t, f);
    LogicalHandle co = Detail::disjunction(t, f);
    LogicalHandle ce = Detail::equality(t, l);
    LogicalHandle cx = Detail::exclusive(t, f);
    LogicalHandle cn = Detail::negation(l);
    ARIADNE_TESTING_PROVE(same_logical_value(ca.check(effort), LogicalValue::FALSE));
    ARIADNE_TESTING_PROVE(same_logical_value(co.check(effort), LogicalValue::TRUE));
    ARIADNE_TESTING_PROVE(same_logical_value(ce.check(effort), LogicalValue::LIKELY));
    ARIADNE_TESTING_PROVE(same_logical_value(cx.check(effort), LogicalValue::TRUE));
    ARIADNE_TESTING_PROVE(same_logical_value(cn.check(effort), LogicalValue::UNLIKELY));

    LogicalInterface* constant = Detail::new_logical_pointer_from_value(LogicalValue::LIKELY);
    ARIADNE_TESTING_PROVE(same_logical_value(Detail::logical_value_from_pointer(constant), LogicalValue::LIKELY));
    LogicalInterface* constant_copy = constant->_copy();
    ARIADNE_TESTING_PROVE(same_logical_value(constant_copy->_check(effort), LogicalValue::LIKELY));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(LogicalHandle(constant_copy)), String("likely"));
    delete constant;

    LogicalInterface* expression_copy = a.pointer()->_copy();
    ARIADNE_TESTING_PROVE(same_logical_value(expression_copy->_check(effort), LogicalValue::FALSE));
    ARIADNE_TESTING_EXPECT_THROWS(std::runtime_error, Detail::logical_value_from_pointer(expression_copy));
    delete expression_copy;

    LogicalInterface* unary_copy = n.pointer()->_copy();
    ARIADNE_TESTING_PROVE(same_logical_value(unary_copy->_check(effort), LogicalValue::UNLIKELY));
    delete unary_copy;

    ARIADNE_TESTING_EXPECT_THROWS(std::runtime_error, Detail::logical_type_from_pointer<EffectiveTag>(nullptr));

    LogicalInterface* effective_ptr = Detail::new_logical_pointer_from_value(LogicalValue::TRUE);
    Kleenean effective = Detail::logical_type_from_pointer<EffectiveTag>(effective_ptr);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(effective, effort), true);

    LogicalInterface* validated_ptr = Detail::new_logical_pointer_from_value(LogicalValue::LIKELY);
    ValidatedKleenean validated = Detail::logical_type_from_pointer<ValidatedTag>(validated_ptr);
    ARIADNE_TESTING_EXPECT_EQUAL(probably(validated), true);
    delete validated_ptr;
}

Void
TestLogical::test_owned_pointer_conversion()
{
    Effort effort(0u);
    Int destructions=0;
    {
        Kleenean owned = Detail::logical_type_from_owned_pointer<EffectiveTag>(
            new TrackedLogical(destructions, LogicalValue::TRUE));
        ARIADNE_TESTING_EXPECT_EQUAL(definitely(owned,effort),true);
        ARIADNE_TESTING_EXPECT_EQUAL(destructions,0);
    }
    ARIADNE_TESTING_EXPECT_EQUAL(destructions,1);

    // A value conversion releases a concrete logical object after extraction.
    ValidatedKleenean value = Detail::logical_type_from_owned_pointer<ValidatedTag>(
        Detail::new_logical_pointer_from_value(LogicalValue::LIKELY));
    ARIADNE_TESTING_EXPECT_EQUAL(probably(value),true);

    // Failed conversions must also release the original allocation.
    ARIADNE_TESTING_EXPECT_THROWS(std::runtime_error, Detail::logical_type_from_owned_pointer<ValidatedTag>(
            new TrackedLogical(destructions,LogicalValue::TRUE)));
    ARIADNE_TESTING_EXPECT_EQUAL(destructions,2);
}

Void
TestLogical::test_types()
{
    Effort effort(0u);
    Effort::set_default(0u);

    Boolean bt(true);
    Boolean bf(false);
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<bool>(bt), true);
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<bool>(bf), false);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(!bt), false);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(bt && bt), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(bt || bf), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(bt ^ bf), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(bt == bt), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(bt != bf), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(bt && true), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(true && bt), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(bf || true), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(true || bf), true);
    ARIADNE_TESTING_EXPECT_EQUAL(is_determinate(bt), true);
    ARIADNE_TESTING_EXPECT_EQUAL(is_indeterminate(bt), false);
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(bt), String("true"));

    Sierpinskian s_true(true);
    Sierpinskian s_ind(indeterminate);
    NegatedSierpinskian ns_false(false);
    NegatedSierpinskian ns_ind(indeterminate);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(s_true, effort), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(s_ind, effort), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(ns_false, effort), false);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(ns_ind, effort), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(check(s_true, effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(check(ns_ind, effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(s_true), true);
    ARIADNE_TESTING_EXPECT_EQUAL(probably(s_true), true);
    ARIADNE_TESTING_EXPECT_EQUAL(decide(s_true), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(s_true), true);
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(!s_true), String("not(true)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(s_true && Sierpinskian(false)), String("and(true,false)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(s_true || Sierpinskian(false)), String("or(true,false)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(s_true || bt), String("true"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(bt || s_true), String("true"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(ns_false && bt), String("false"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(bt && ns_false), String("false"));

    Kleenean k_true(bt);
    Kleenean k_s(s_true);
    Kleenean k_ns(ns_ind);
    Kleenean k_ind(indeterminate);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(k_true.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(check(k_s, effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(k_ns.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(k_ind.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(!k_true), String("not(true)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(k_true && Kleenean(false)), String("and(true,false)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(k_true || Kleenean(false)), String("or(true,false)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(k_true ^ Kleenean(false)), String("xor(true,false)"));

    LowerKleenean lower_true(bt);
    LowerKleenean lower_s(s_true);
    LowerKleenean lower_k(k_ind);
    LowerKleenean lower_ind(indeterminate);
    UpperKleenean upper_false(bf);
    UpperKleenean upper_ns(ns_ind);
    UpperKleenean upper_k(k_ind);
    UpperKleenean upper_ind(indeterminate);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(lower_true.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(lower_s.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(lower_k.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(lower_ind.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(upper_false.check(effort)), false);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(upper_ns.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(upper_k.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(upper_ind.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(check(lower_true, effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(check(upper_false, effort)), false);
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(!lower_true), String("not(true)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(!upper_false), String("not(false)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(lower_true && LowerKleenean(false)), String("and(true,false)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(lower_true || LowerKleenean(false)), String("or(true,false)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(upper_false && UpperKleenean(true)), String("and(false,true)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(upper_false || UpperKleenean(true)), String("or(false,true)"));

    NaiveKleenean naive_bool(bt);
    NaiveKleenean naive_ind(indeterminate);
    NaiveKleenean naive_s(s_true);
    NaiveKleenean naive_ns(ns_ind);
    NaiveKleenean naive_k(k_ind);
    NaiveKleenean naive_lower(lower_true);
    NaiveKleenean naive_upper(upper_false);
    ARIADNE_TESTING_EXPECT_EQUAL(probably(naive_bool.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(naive_ind.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(probably(naive_s.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(naive_ns.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(naive_k.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(probably(naive_lower.check(effort)), true);
    ARIADNE_TESTING_EXPECT_EQUAL(probably(naive_upper.check(effort)), false);
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(!naive_bool), String("not(true)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(naive_bool && NaiveKleenean(false)), String("and(true,false)"));
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(naive_bool || NaiveKleenean(false)), String("or(true,false)"));

    ValidatedSierpinskian vs_true(true);
    ValidatedSierpinskian vs_false(false);
    ValidatedSierpinskian vs_from(s_true, effort);
    ValidatedNegatedSierpinskian vns_true(true);
    ValidatedNegatedSierpinskian vns_false(false);
    ValidatedNegatedSierpinskian vns_from(ns_ind, effort);
    ValidatedKleenean vk_default;
    ValidatedKleenean vk_bool(bt);
    ValidatedKleenean vk_vs(vs_true);
    ValidatedKleenean vk_vns(vns_from);
    ValidatedKleenean vk_k(k_ind, effort);
    ValidatedLowerKleenean vl_direct_bool(true);
    ValidatedLowerKleenean vl_bool(bt);
    ValidatedLowerKleenean vl_vs(vs_true);
    ValidatedLowerKleenean vl_vk(vk_k);
    ValidatedLowerKleenean vl_lower(lower_ind, effort);
    ValidatedUpperKleenean vu_direct_bool(false);
    ValidatedUpperKleenean vu_bool(bf);
    ValidatedUpperKleenean vu_vk(vk_k);
    ValidatedUpperKleenean vu_vns(vns_from);
    ValidatedUpperKleenean vu_upper(upper_ind, effort);

    ARIADNE_TESTING_EXPECT_EQUAL(definitely(vs_true), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(vs_false), false);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(vs_from), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vns_true), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vns_false), false);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vns_from), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(vk_default), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(vk_bool), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(vk_vs), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vk_vns), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vk_k), true);
    ValidatedKleenean vk_and = vk_bool && vk_k;
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vk_and), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(vl_direct_bool), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(vl_bool), true);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(vl_vs), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vl_vk), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vl_lower), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vu_direct_bool), false);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vu_bool), false);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vu_vk), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vu_vns), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(vu_upper), true);

    ApproximateKleenean ak_direct_bool(false);
    ApproximateKleenean ak_bool(bt);
    ApproximateKleenean ak_vk(vk_k);
    ApproximateKleenean ak_vl(vl_lower);
    ApproximateKleenean ak_vu(vu_upper);
    ApproximateKleenean ak_k(k_ind, effort);
    ARIADNE_TESTING_EXPECT_EQUAL(probably(ak_direct_bool), false);
    ARIADNE_TESTING_EXPECT_EQUAL(probably(ak_bool), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(ak_vk), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(ak_vl), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(ak_vu), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(ak_k), true);

    ValidatedSierpinskian ind_s = indeterminate;
    ValidatedKleenean ind_k = indeterminate;
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(ind_s), true);
    ARIADNE_TESTING_EXPECT_EQUAL(possibly(ind_k), true);
    ARIADNE_TESTING_EXPECT_EQUAL(decide(indeterminate, effort), false);
    ARIADNE_TESTING_EXPECT_EQUAL(decide(indeterminate), false);

    Case<ValidatedKleenean,Int> logical_case(vk_bool, 17);
    ARIADNE_TESTING_EXPECT_EQUAL(definitely(logical_case.condition()), true);
    ARIADNE_TESTING_EXPECT_EQUAL(logical_case.term(), 17);
}

Void
TestLogical::test_nondeterminism()
{
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<bool>(choose(LowerKleenean(true), LowerKleenean(false))), true);
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<bool>(choose(LowerKleenean(false), LowerKleenean(true))), false);

    LowerKleenean delayed_true(LogicalHandle(new DelayedLogical(2u, LogicalValue::TRUE)));
    LowerKleenean delayed_false(LogicalHandle(new DelayedLogical(1u, LogicalValue::TRUE)));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<bool>(choose(delayed_true, LowerKleenean(false))), true);
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<bool>(choose(LowerKleenean(false), delayed_false)), false);

    Array<LowerKleenean> immediate { LowerKleenean(false), LowerKleenean(true) };
    ARIADNE_TESTING_EXPECT_EQUAL(nondeterministic_choose_index(immediate), SizeType(1u));

    Array<LowerKleenean> delayed {
        LowerKleenean(false),
        LowerKleenean(LogicalHandle(new DelayedLogical(2u, LogicalValue::TRUE)))
    };
    ARIADNE_TESTING_EXPECT_EQUAL(nondeterministic_choose_index(delayed), SizeType(1u));
}

Void
TestLogical::test_class_names()
{
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<ExactTag>(), String("Exact"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<EffectiveTag>(), String("Effective"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<ValidatedTag>(), String("Validated"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<ApproximateTag>(), String("Approximate"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<Bool>(), String("Bool"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<Boolean>(), String("Boolean"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<Sierpinskian>(), String("Sierpinskian"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<NegatedSierpinskian>(), String("NegatedSierpinskian"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<Kleenean>(), String("Kleenean"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<LowerKleenean>(), String("LowerKleenean"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<UpperKleenean>(), String("UpperKleenean"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<ValidatedSierpinskian>(), String("ValidatedSierpinskian"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<ValidatedNegatedSierpinskian>(), String("ValidatedNegatedSierpinskian"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<ValidatedKleenean>(), String("ValidatedKleenean"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<ValidatedLowerKleenean>(), String("ValidatedLowerKleenean"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<ValidatedUpperKleenean>(), String("ValidatedUpperKleenean"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<ApproximateKleenean>(), String("ApproximateKleenean"));
}
