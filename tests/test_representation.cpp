/***************************************************************************
 *            test_representation.cpp
 *
 *  Copyright  2026
 *
 ****************************************************************************/

#include "foundation/representation.hpp"
#include "utility/string.hpp"
#include "utility/test.hpp"

using namespace Ariadne;

namespace {

struct WithRepresentation {
    Int value;
    OutputStream& _repr(OutputStream& os) const { return os << "repr(" << value << ")"; }
};

struct PlainRepresentation {
    Int value;
};

OutputStream& operator<<(OutputStream& os, PlainRepresentation const& object) {
    return os << "plain(" << object.value << ")";
}

} // namespace

class TestRepresentation
{
  public:
    Void test();
};

Int main() {
    ARIADNE_TEST_CLASS(TestRepresentation,TestRepresentation());
    return ARIADNE_TEST_FAILURES;
}

Void
TestRepresentation::test()
{
    WithRepresentation with_repr { 4 };
    Representation<WithRepresentation> wrapped = representation(with_repr);
    ARIADNE_TEST_EQUAL(wrapped.reference().value, 4);
    ARIADNE_TEST_EQUAL(to_string(wrapped), String("repr(4)"));

    PlainRepresentation plain { 9 };
    Representation<PlainRepresentation> plain_wrapped = representation(plain);
    ARIADNE_TEST_EQUAL(plain_wrapped.reference().value, 9);
    ARIADNE_TEST_EQUAL(to_string(plain_wrapped), String("plain(9)"));
}
