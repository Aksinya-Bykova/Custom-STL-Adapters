#pragma once

#include <iterator>
#include <algorithm>
#include <iostream>
#include "check.h"

namespace details {
    template<typename Func, typename R, typename DiffType>
    class TransformIter {
    public:
        using iter_type = typename std::remove_reference_t<R>::iterator;
        using value_type = typename std::iterator_traits<iter_type>::value_type;
        using difference_type = DiffType;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        TransformIter() = default;

        TransformIter(Func func, R base, DiffType pos, DiffType pos_end)
                : func_(func), base_(base), pos_(pos), pos_end_(pos_end) {}

        TransformIter& operator++() {
            ++pos_;
            return *this;
        }

        TransformIter& operator--() {
            ---pos_;
            return *this;
        }

        TransformIter operator++(int) {
            TransformIter temp(*this);
            ++pos_;
            return temp;
        }

        TransformIter operator--(int) {
            TransformIter temp(*this);
            --pos_;
            return temp;
        }

        value_type operator*() const {
            auto it = base_.begin();
            std::advance(it, pos_);
            return func_(*it);
        }

        friend bool operator==(const TransformIter& first, const TransformIter& second) {
            return first.pos_ == second.pos_;
        }

        friend bool operator!=(const TransformIter& first, const TransformIter& second) {
            return !(first == second);
        }

    private:
        Func func_;
        R base_;
        DiffType pos_;
        DiffType pos_end_;
    };

    template<typename IterType>
    class TransformView {
    public:
        using value_type = typename IterType::value_type;
        using iterator = IterType;

        TransformView() = default;

        TransformView(IterType begin, IterType end)
                : begin_(begin), end_(end) {
        }

        IterType begin() const {
            return begin_;
        }

        IterType end() const {
            return end_;
        }

    private:
        IterType begin_;
        IterType end_;
    };

    template<typename Func>
    struct TransformAdaptorClosure {
        Func func;

        template<typename R>
        friend auto operator|(R r, TransformAdaptorClosure closure) {
            check<R>();
            using iter_type = typename std::remove_reference_t<R>::iterator;
            using diff_type = typename std::iterator_traits<iter_type>::difference_type;
            using value_type = typename std::iterator_traits<iter_type>::value_type;
            TransformIter<Func, R, diff_type> begin_iter{closure.func, r, 0, std::distance(r.begin(), r.end())};
            TransformIter<Func, R, diff_type> end_iter{closure.func, r, std::distance(r.begin(), r.end()), std::distance(r.begin(), r.end())};
            return TransformView<TransformIter<Func, R, diff_type>>{begin_iter, end_iter};
        }
    };

    struct TransformAdaptor {
        template<typename Func>
        auto operator()(Func func) {
            return TransformAdaptorClosure<Func>{func};
        }
    };
}  // namespace details

details::TransformAdaptor transform;
