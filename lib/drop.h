#pragma once

#include <algorithm>
#include "check.h"

namespace details {
    template<typename R, typename Difference>
    class DropView {
    public:
        using iterator = typename std::remove_reference_t<R>::iterator;
        using value_type = typename std::iterator_traits<iterator>::value_type;

        DropView() = default;

        DropView(R base, Difference count)
                : base_{base}, count_{count} {
        }

        auto begin() const {
            return base_.begin() + count_;
        }

        auto end() const {
            return base_.end();
        }

    private:
        R base_;
        Difference count_;
    };

    template<typename Difference>
    struct DropAdaptorClosure {
        Difference count;

        template<typename R>
        friend auto operator|(R r, DropAdaptorClosure closure) {
            check<R>();
            return DropView<R, Difference>{r, closure.count};
        }
    };

    struct DropAdaptor {
        template<typename T>
        auto operator()(T count) {
            return DropAdaptorClosure<T>{count};
        }
    };
}  // namespace details

details::DropAdaptor drop;