/**
 * @file
 * @brief testing scope guards
 * @author Design Verification Lab
 * @copyright Copyright(c) 2024 DVLab, GIEE, NTU, Taiwan
 */

#include "util/scope_guard.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace dvlab;

TEST_CASE("The scope_exit guard should be called when the scope ends", "[util]") {
    auto flag = false;
    {
        auto guard = utils::scope_exit([&]() { flag = true; });
        REQUIRE_FALSE(flag);
    }
    REQUIRE(flag);
}

TEST_CASE("The guard should do nothing if the scope_exit is released", "[util]") {
    auto flag = false;
    {
        auto guard = utils::scope_exit([&]() { flag = true; });
        REQUIRE_FALSE(flag);
        guard.release();
    }
    REQUIRE_FALSE(flag);
}
