/**
 * @file
 * @brief testing RAII temporary files and directories
 * @author Design Verification Lab
 * @copyright Copyright(c) 2024 DVLab, GIEE, NTU, Taiwan
 */

#include "util/tmp_files.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>

#include "../common/unit_test_common.hpp"

using namespace dvlab;
namespace fs = std::filesystem;

TEST_CASE("The temp file should exists after the file is instantiated and destroyed after it goes out of scope", "[util]") {
    fs::path file_path;
    {
        auto const tmp_file = utils::TmpFile();
        file_path           = tmp_file.path();  // memoize the path
        REQUIRE(fs::exists(file_path));
    }
    REQUIRE_FALSE(fs::exists(file_path));
}

TEST_CASE("The temp dir should exists after the file is instantiated and destroyed after it goes out of scope", "[util]") {
    fs::path dir_path;
    {
        auto const tmp_dir = utils::TmpDir();
        dir_path           = tmp_dir.path();  // memoize the path
        REQUIRE(fs::exists(dir_path));
    }
    REQUIRE_FALSE(fs::exists(dir_path));
}

TEST_CASE("The temp file should be created with the given prefix", "[util]") {
    for (auto i = 0; i < 5; ++i) {
        auto const path     = fs::temp_directory_path() / get_random_string(10);
        auto const tmp_file = utils::TmpFile(path.string());
        REQUIRE(tmp_file.path().string().starts_with(path.string()));
    }
}

TEST_CASE("The temp dir should be created with the given prefix", "[util]") {
    for (auto i = 0; i < 5; ++i) {
        auto const path    = fs::temp_directory_path() / get_random_string(10);
        auto const tmp_dir = utils::TmpDir(path.string());
        REQUIRE(tmp_dir.path().string().starts_with(path.string()));
    }
}

TEST_CASE("The temp file path should be readable and writable", "[util]") {
    auto tmp_file = utils::TmpFile();

    auto stream = std::fstream(tmp_file.path());
    REQUIRE(stream.good());
    stream << "Hello World!" << std::endl;
    stream.seekg(0);
    std::string line;
    std::getline(stream, line);
    REQUIRE(line == "Hello World!");
}
