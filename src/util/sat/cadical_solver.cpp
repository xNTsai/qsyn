/****************************************************************************
  PackageName  [ util ]
  Synopsis     [ Define the interface of SAT solver ]
  Author       [ Design Verification Lab ]
  Copyright    [ Copyright(c) 2023 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cadical/cadical.hpp>
#include <ranges>

#include "./sat_solver.hpp"

namespace dvlab::sat {

void CaDiCalSolver::reset() {
    _solver   = std::make_unique<CaDiCaL::Solver>();
    _next_var = Variable(1);
}

void CaDiCalSolver::add_clause(std::vector<Literal> const& clause) {
    for (auto const& lit : clause) {
        _solver->add(lit.get());
    }
    _solver->add(0);
}

void CaDiCalSolver::assume(Literal lit) {
    _solver->assume(lit.get());
}

Result CaDiCalSolver::solve() {
    switch (_solver->solve()) {
        case 10:
            return Result::sat;
        case 20:
            return Result::unsat;
        case 0:
        default:
            return Result::unknown;
    }
}

std::optional<Solution> CaDiCalSolver::get_solution() {
    if (!(_solver->state() & CaDiCaL::SATISFIED)) {
        return std::nullopt;
    }

    Solution solution(_next_var.get());
    for (auto const& i : std::views::iota(0, (int)solution.size())) {
        auto var = Variable(i + 1);
        solution.set(var, _solver->val(var.get()) > 0);
    }
    return solution;
}

}  // namespace dvlab::sat
