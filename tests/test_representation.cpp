/***************************************************************************
 *            test_representation.cpp
 *
 *  Copyright  2026
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

#include "foundation/representation.hpp"
#include "utility/string.hpp"
#include "ariadne-testing.hpp"

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

class TestRepresentation : public Ariadne::Testing::ContextFixture
{
  public:
    using ContextFixture::ContextFixture;
    Void test();
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("representation");
    ARIADNE_TESTING_ADD_TEST(TestRepresentation, test);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}

Void
TestRepresentation::test()
{
    WithRepresentation with_repr { 4 };
    Representation<WithRepresentation> wrapped = representation(with_repr);
    ARIADNE_TESTING_EXPECT_EQUAL(wrapped.reference().value, 4);
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(wrapped), String("repr(4)"));

    PlainRepresentation plain { 9 };
    Representation<PlainRepresentation> plain_wrapped = representation(plain);
    ARIADNE_TESTING_EXPECT_EQUAL(plain_wrapped.reference().value, 9);
    ARIADNE_TESTING_EXPECT_EQUAL(to_string(plain_wrapped), String("plain(9)"));
}
