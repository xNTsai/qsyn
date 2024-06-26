/****************************************************************************
  PackageName  [ qcir ]
  Synopsis     [ Define class QCir Writer functions ]
  Author       [ Design Verification Lab ]
  Copyright    [ Copyright(c) 2023 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#include <fmt/ostream.h>

#include <filesystem>
#include <fstream>
#include <string>

#include "./operation.hpp"
#include "./qcir.hpp"
#include "./qcir_gate.hpp"
#include "./qcir_io.hpp"
#include "util/sysdep.hpp"
#include "util/tmp_files.hpp"

namespace qsyn::qcir {

/**
 * @brief Write QASM
 *
 * @param filename
 * @return true if successfully write
 * @return false if path or file not found
 */
bool QCir::write_qasm(std::filesystem::path const& filepath) const {
    std::ofstream ofs(filepath);
    if (!ofs) {
        spdlog::error("Cannot open file {}", filepath.string());
        return false;
    }
    ofs << to_qasm(*this);

    return true;
}

/**
 * @brief Draw a quantum circuit onto terminal or into a file using Qiskit
 *
 * @param drawer `text`, `mpl`, `latex`, or `latex_source`. Here `mpl` means Python's MatPlotLib
 * @param outputPath If specified, output to this path; else output to terminal. Must be specified for `mpl` and `latex` drawer.
 * @return true if succeeds drawing;
 * @return false if not.
 */
bool QCir::draw(QCirDrawerType drawer, std::filesystem::path const& output_path, float scale) const {
    namespace dv = dvlab::utils;
    namespace fs = std::filesystem;

    if (!dv::python_package_exists("qiskit")) {
        spdlog::error("qiskit is not installed in the system!!");
        spdlog::error("Please install qiskit first or check if you have used the correct python environment!!");
        return false;
    }

    if (drawer == QCirDrawerType::mpl || drawer == QCirDrawerType::latex) {
        if (dv::python_package_exists("pylatexenc") == 0) {
            spdlog::error("pylatexenc is not installed in the system!!");
            spdlog::error("Please install pylatexenc first or check if you have used the correct python environment!!");
            return false;
        }
    }

    if (drawer == QCirDrawerType::latex) {
        if (!dvlab::utils::pdflatex_exists()) {
            spdlog::error("pdflatex is not installed in the system. Please install pdflatex first!!");
            return false;
        }
    }

    // check if output_path is valid
    if (!output_path.string().empty()) {
        if (!std::ofstream{output_path}) {
            spdlog::error("Cannot open file {}", output_path.string());
            return false;
        }
    }

    dv::TmpDir const tmp_dir;
    fs::path const tmp_qasm = tmp_dir.path() / "tmp.qasm";

    this->write_qasm(tmp_qasm.string());

    auto const path_to_script = "scripts/qccdraw_qiskit_interface.py";

    auto cmd = fmt::format("python3 {} -input {} -drawer {} -scale {}", path_to_script, tmp_qasm.string(), drawer, scale);
    // auto cmd = fmt::format("python3 {} -input {} -drawer {} -scale {}", path_to_script, tmp_qasm.string(), drawer, scale);

    if (!output_path.string().empty()) {
        cmd += " -output " + output_path.string();
    }

    return system(cmd.c_str()) == 0;
}

std::string to_qasm(QCir const& qcir) {
    std::string qasm = "OPENQASM 2.0;\n";
    qasm += "include \"qelib1.inc\";\n";
    qasm += fmt::format("qreg q[{}];\n", qcir.get_num_qubits());

    for (auto const* gate : qcir.get_gates()) {
        using namespace std::literals;
        auto const qubits = gate->get_qubits();
        auto repr         = gate->get_operation().get_repr();
        // if encountering "π", replace it with "pi"
        size_t pos = 0;
        while ((pos = repr.find("π"s, pos)) != std::string::npos) {
            if (pos == 0 || !std::isdigit(repr[pos - 1])) {
                repr.replace(pos, "π"s.size(), "pi");
            } else {
                repr.replace(pos, "π"s.size(), "*pi");
            }
        }

        qasm += fmt::format("{} {};\n",
                            repr,
                            fmt::join(qubits | std::views::transform([](auto pin) { return fmt::format("q[{}]", pin); }), ", "));
    }
    return qasm;
}

}  // namespace qsyn::qcir
