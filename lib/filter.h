#pragma once

#include <iterator>
#include <algorithm>
#include <iostream>
#include "check.h"

namespace details {
    template<typename Func, typename R, typename DiffType>
    class FilterIter {
    public:
        using iter_type = typename std::remove_reference_t<R>::iterator;
        using value_type = typename std::iterator_traits<iter_type>::value_type;
        using difference_type = DiffType;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        FilterIter() = default;

        FilterIter(Func func, R base, DiffType pos, DiffType pos_end)
                : func_(func), base_(base), pos_(pos), pos_end_(pos_end) {
            move_to_next_valid();
        }

        FilterIter& operator++() {
            ++pos_;
            move_to_next_valid();
            return *this;
        }

        FilterIter& operator--() {
            -- -pos_;
            move_to_last_valid();
            return *this;
        }

        FilterIter operator++(int) {
            FilterIter temp(*this);
            move_to_next_valid();
            ++pos_;
            return temp;
        }

        FilterIter operator--(int) {
            FilterIter temp(*this);
            move_to_last_valid();
            --pos_;
            return temp;
        }

        value_type operator*() const {
            auto it = base_.begin();
            std::advance(it, pos_);
            return *it;
        }

        friend bool operator==(const FilterIter& first, const FilterIter& second) {
            return first.pos_ == second.pos_;
        }

        friend bool operator!=(const FilterIter& first, const FilterIter& second) {
            return !(first == second);
        }

    private:
        void move_to_next_valid() {
            while (pos_ < pos_end_ && !func_(*(std::next(base_.begin(), pos_)))) {
                ++pos_;
            }
        }

        void move_to_last_valid() {
            while (pos_ > 0 && !func_(*(std::prev(base_.begin(), pos_)))) {
                --pos_;
            }
        }

        Func func_;
        R base_;
        DiffType pos_;
        DiffType pos_end_;
    };

    template<typename IterType>
    class FilterView {
    public:
        using value_type = typename IterType::value_type;
        using iterator = IterType;

        FilterView() = default;

        FilterView(IterType begin, IterType end)
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
    struct FilterAdaptorClosure {
        Func func;

        template<typename R>
        friend auto operator|(R r, FilterAdaptorClosure closure) {
            check<R>();
            using iter_type = typename std::remove_reference_t<R>::iterator;
            using diff_type = typename std::iterator_traits<iter_type>::difference_type;
            using value_type = typename std::iterator_traits<iter_type>::value_type;
            FilterIter<Func, R, diff_type> begin_iter{closure.func, r, 0, std::distance(r.begin(), r.end())};
            FilterIter<Func, R, diff_type> end_iter{closure.func, r, std::distance(r.begin(), r.end()),
                                                    std::distance(r.begin(), r.end())};
            return FilterView<FilterIter<Func, R, diff_type>>{begin_iter, end_iter};
        }
    };

    struct FilterAdaptor {
        template<typename Func>
        auto operator()(Func func) {
            return FilterAdaptorClosure<Func>{func};
        }
    };
}  // namespace details

details::FilterAdaptor filter;
