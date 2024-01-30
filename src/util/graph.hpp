/****************************************************************************
  PackageName  [ util ]
  Synopsis     [ Define generic graph class ]
  Author       [ Design Verification Lab ]
  Copyright    [ Copyright(c) 2023 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#pragma once

#include "util/ordered_hashmap.hpp"
#include "util/ordered_hashset.hpp"

namespace dvlab {

template <typename T>
class DirectedGraph {
public:
    DirectedGraph() = default;

    // accessors and attributes
    [[nodiscard]] auto const& get_vertices() const {
        return _vertices;
    }

    [[nodiscard]] auto const& get_out_neighbors(size_t id) const {
        return _out_neighbors.at(id);
    }

    [[nodiscard]] auto const& get_in_neighbors(size_t id) const {
        return _in_neighbors.at(id);
    }

    [[nodiscard]] auto const& get_inputs() const {
        return _inputs;
    }

    [[nodiscard]] auto const& get_outputs() const {
        return _outputs;
    }

    [[nodiscard]] bool has_vertex(size_t id) const {
        return _vertices.contains(id);
    }

    [[nodiscard]] bool has_edge(size_t from, size_t to) const {
        return _out_neighbors.at(from).contains(to);
    }

    [[nodiscard]] size_t n_vertices() const {
        return _vertices.size();
    }

    [[nodiscard]] size_t num_edges() const {
        size_t num_edges = 0;
        for (auto const& [_, adjacencies] : _out_neighbors) {
            num_edges += adjacencies.size();
        }
        return num_edges;
    }

    [[nodiscard]] size_t num_inputs() const {
        return _inputs.size();
    }

    [[nodiscard]] size_t num_outputs() const {
        return _outputs.size();
    }

    [[nodiscard]] size_t num_in_neighbors(size_t id) const {
        return _in_neighbors.at(id).size();
    }

    [[nodiscard]] size_t num_out_neighbors(size_t id) const {
        return _out_neighbors.at(id).size();
    }

    void add_vertex(size_t id, T const& data) {
        _vertices.emplace(id, data);
    }

    void add_edge(size_t from, size_t to) {
        _out_neighbors[from].emplace(to);
        _in_neighbors[to].emplace(from);
    }

    void remove_vertex(size_t id) {
        _vertices.erase(id);
        _out_neighbors.erase(id);
        _in_neighbors.erase(id);
        for (auto& [_, adjacencies] : _out_neighbors) {
            adjacencies.erase(id);
        }
        for (auto& [_, adjacencies] : _in_neighbors) {
            adjacencies.erase(id);
        }
    }

    void remove_edge(size_t from, size_t to) {
        _out_neighbors[from].erase(to);
        _in_neighbors[to].erase(from);
    }

private:
    utils::ordered_hashmap<size_t, utils::ordered_hashset<size_t>> _out_neighbors;
    utils::ordered_hashmap<size_t, utils::ordered_hashset<size_t>> _in_neighbors;
    std::unordered_map<size_t, T> _vertices;
    std::unordered_map<size_t, size_t> _inputs;
    std::unordered_map<size_t, size_t> _outputs;
};

}  // namespace dvlab
