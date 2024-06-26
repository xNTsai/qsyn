/****************************************************************************
  PackageName  [ util ]
  Synopsis     [ Define scope guards ]
  Author       [ Design Verification Lab ]
  Copyright    [ Copyright(c) 2023 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#include <functional>

namespace dvlab {

namespace utils {

class scope_exit {  // NOLINT(readability-identifier-naming) // emulate library
public:
    template <typename Callable>
    scope_exit(Callable&& undo_func) try : _undo_func(std::forward<Callable>(undo_func)) {
    } catch (...) {
        undo_func();
        throw;
    }

    ~scope_exit() {
        if (_undo_func) _undo_func();
    }

    scope_exit(scope_exit const&)            = delete;
    scope_exit& operator=(scope_exit const&) = delete;

    scope_exit(scope_exit&& other) noexcept : _undo_func(std::move(other._undo_func)) {
        other._undo_func = nullptr;
    }

    scope_exit& operator=(scope_exit&& other) = delete;

    void release() { _undo_func = nullptr; }

private:
    std::function<void()> _undo_func;
};

}  // namespace utils

}  // namespace dvlab
