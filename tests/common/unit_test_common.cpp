/****************************************************************************
  PackageName  [ common ]
  Synopsis     [ common functions for unit tests ]
  Author       [ Design Verification Lab ]
  Copyright    [ Copyright(c) 2023 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#include "./unit_test_common.hpp"

#include <string>

/**
 * @brief Dummy function for testing functions that takes a custom stop signal
 *
 * @return true
 * @return false
 */
bool stop_requested() { return false; }

/**
 * @brief Get the rand gen object
 *
 * @return std::mt19937&
 */
std::mt19937& get_rand_gen() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

/**
 * @brief Get a random string
 *
 * @param length
 * @return std::string
 */
std::string get_random_string(size_t length, std::string_view charset) {
    std::uniform_int_distribution<> dist(0, static_cast<int>(charset.size() - 1));
    std::string str(length, 0);
    for (auto& c : str) {
        c = charset[dist(get_rand_gen())];
    }
    return str;
}
