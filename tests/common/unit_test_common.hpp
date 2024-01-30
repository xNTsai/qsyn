/**
 * @file
 * @package common
 * @copyright Copyright(c) 2024 DVLab, GIEE, NTU, Taiwan
 */

#pragma once

#include <random>
#include <string>
bool stop_requested();

std::mt19937& get_rand_gen();
std::string get_random_string(size_t length, std::string_view charset =
                                                 "0123456789"
                                                 "abcdefghijklmnopqrstuvwxyz"
                                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
