#pragma once

#include <algorithm>
#include "check.h"

namespace details {
    template<typename R, typename Difference>
    class TakeView {
    public:
        using iterator = typename std::remove_reference_t<R>::iterator;
        using value_type = typename std::iterator_traits<iterator>::value_type;

        TakeView() = default;

        TakeView(R base, Difference count)
                : base_{base}, count_{count} {
        }

        auto begin() const {
            return base_.begin();
        }

        auto end() const {
            return base_.begin() + count_;
        }

    private:
        R base_;
        Difference count_;
    };

    template<typename Difference>
    struct TakeAdaptorClosure {
        Difference count;

        template<typename R>
        friend auto operator|(R r, TakeAdaptorClosure closure) {
            check<R>();
            return TakeView<R, Difference>{r, closure.count};
        }
    };

    struct TakeAdaptor {
        template<typename T>
        auto operator()(T count) {
            return TakeAdaptorClosure<T>{count};
        }
    };
}  // namespace details

details::TakeAdaptor take;