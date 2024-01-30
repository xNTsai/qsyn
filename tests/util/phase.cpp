/****************************************************************************
  PackageName  [ util ]
  Synopsis     [ tests for phase class ]
  Author       [ Design Verification Lab ]
  Copyright    [ Copyright(c) 2023 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#include "util/phase.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "util/rational.hpp"

using namespace dvlab;

TEST_CASE("phase constructors", "[util]") {
    SECTION("default constructor") {
        auto p = Phase{};
        REQUIRE(p.numerator() == 0);
        REQUIRE(p.denominator() == 1);
    }
    SECTION("construct from numerator") {
        auto p1 = Phase{1};
        REQUIRE(p1.numerator() == 1);
        REQUIRE(p1.denominator() == 1);
        auto p2 = Phase{0};
        REQUIRE(p2.numerator() == 0);
        REQUIRE(p2.denominator() == 1);
    }
    SECTION("construct from numerator and denominator") {
        auto p1 = Phase{1, 2};
        REQUIRE(p1.numerator() == 1);
        REQUIRE(p1.denominator() == 2);
        auto p2 = Phase{0, 2};
        REQUIRE(p2.numerator() == 0);
        REQUIRE(p2.denominator() == 1);
        auto p3 = Phase{3, 2};
        REQUIRE(p3.numerator() == -1);
        REQUIRE(p3.denominator() == 2);
    }
    SECTION("construct from floating point numbers") {
        auto p1 = Phase{1.57f, 1e-2f};
        auto p2 = Phase{1.5708};
        auto p3 = Phase{1.570796l, 1e-6l};
        REQUIRE(p1.numerator() == 1);
        REQUIRE(p1.denominator() == 2);
        REQUIRE(p2.numerator() == 1);
        REQUIRE(p2.denominator() == 2);
        REQUIRE(p3.numerator() == 1);
        REQUIRE(p3.denominator() == 2);
    }

    SECTION("construct with denominator should fail") {
        REQUIRE_THROWS(Phase{1, 0});
    }
}

TEST_CASE("phase arithmetics", "[util]") {
    SECTION("unary plus and minus") {
        auto p1 = Phase{1, 2};
        auto p2 = +p1;
        auto p3 = -p1;
        REQUIRE(p2.numerator() == 1);
        REQUIRE(p2.denominator() == 2);
        REQUIRE(p3.numerator() == -1);
        REQUIRE(p3.denominator() == 2);
    }

    SECTION("addition and subtraction") {
        Phase p1{1, 2};
        Phase p2{1, 3};
        Phase p3 = p1 + p2;
        Phase p4 = p1 - p2;
        REQUIRE(p3.numerator() == 5);
        REQUIRE(p3.denominator() == 6);
        REQUIRE(p4.numerator() == 1);
        REQUIRE(p4.denominator() == 6);
    }

    SECTION("addition and subtraction with wrap-around") {
        auto p1 = Phase{1, 2};
        auto p2 = Phase{-1, 3};
        auto p3 = Phase{2, 3};
        auto p4 = p1 + p3;
        auto p5 = p2 - p3;
        REQUIRE(p4.numerator() == -5);
        REQUIRE(p4.denominator() == 6);
        REQUIRE(p5.numerator() == 1);
        REQUIRE(p5.denominator() == 1);
    }

    SECTION("multiplication and division") {
        auto p1 = Phase{1, 2};
        auto p2 = p1 * 2;
        auto p3 = p1 / 2;
        REQUIRE(p2.numerator() == 1);
        REQUIRE(p2.denominator() == 1);
        REQUIRE(p3.numerator() == 1);
        REQUIRE(p3.denominator() == 4);

        // commutation
        REQUIRE(2 * p1 == p1 * 2);

        // divide by rational
        REQUIRE(p2 == p1 / Rational{1, 2});
        REQUIRE(p3 == p1 * Rational{1, 2});

        // divide two phases
        REQUIRE((p2 / p3) == Rational(4));
    }

    SECTION("multiplication and division with wrap-around") {
        auto p1 = Phase{1, 2};
        auto p2 = p1 * 3;
        REQUIRE(p2.numerator() == -1);
        REQUIRE(p2.denominator() == 2);
    }

    SECTION("equality and inequality") {
        auto p1 = Phase{1, 2};
        auto p2 = Phase{1, 2};
        auto p3 = Phase{1, 3};
        REQUIRE(p1 == p2);
        REQUIRE(p1 != p3);
    }
}

TEST_CASE("phase printing", "[util]") {
    REQUIRE(fmt::format("{}", Phase{1, 2}) == "π/2");
    REQUIRE(fmt::format("{}", Phase{1, 1}) == "π");
    REQUIRE(fmt::format("{}", Phase{-1, 2}) == "-π/2");
    REQUIRE(fmt::format("{}", Phase{0, 1}) == "0");
}

TEST_CASE("phase conversions to floating points", "[util]") {
    using Catch::Matchers::WithinAbs;

    for (auto const& p : {Phase{2, 3}, Phase{-1, 4}, Phase{1}, Phase{0}}) {
        REQUIRE_THAT(Phase::to_float(p), WithinAbs(std::numbers::pi_v<float> * p.numerator() / p.denominator(), 1e-4f));
        REQUIRE_THAT(Phase::to_double(p), WithinAbs(std::numbers::pi_v<double> * p.numerator() / p.denominator(), 1e-4f));
        REQUIRE_THAT(Phase::to_long_double(p), WithinAbs(std::numbers::pi_v<long double> * p.numerator() / p.denominator(), 1e-4f));
    }
}
