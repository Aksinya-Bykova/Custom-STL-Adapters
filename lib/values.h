#pragma once

#include <iterator>
#include <algorithm>
#include <iostream>
#include "check.h"

namespace details {
    template<typename R, typename DiffType>
    class ValuesIter {
    public:
        using iter_type = typename std::remove_reference_t<R>::iterator;
        using value_type = typename std::iterator_traits<iter_type>::value_type;
        using difference_type = DiffType;
        using pointer = typename std::iterator_traits<iter_type>::pointer;
        using reference = typename std::iterator_traits<iter_type>::reference;
        using iterator_category = std::forward_iterator_tag;

        ValuesIter() = default;

        ValuesIter(R base, DiffType pos, DiffType pos_end)
                : base_(base), pos_(pos), pos_end_(pos_end) {}

        ValuesIter& operator++() {
            ++pos_;
            return *this;
        }

        ValuesIter& operator--() {
            ---pos_;
            return *this;
        }

        ValuesIter operator++(int) {
            ValuesIter temp(*this);
            ++pos_;
            return temp;
        }

        ValuesIter operator--(int) {
            ValuesIter temp(*this);
            --pos_;
            return temp;
        }

        typename value_type::second_type operator*() const {
            auto it = base_.begin();
            std::advance(it, pos_);
            return it->second;
        }

        friend bool operator==(const ValuesIter& left, const ValuesIter& right) {
            return left.pos_ == right.pos_;
        }

        friend bool operator!=(const ValuesIter& left, const ValuesIter& right) {
            return !(left == right);
        }

    private:
        R base_;
        DiffType pos_;
        DiffType pos_end_;
    };

    template<typename IterType>
    class ValuesView {
    public:
        using value_type = typename IterType::value_type::second_type;
        using iterator = IterType;

        ValuesView() = default;

        ValuesView(IterType begin, IterType end)
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

    struct ValuesAdaptorClosure {
        template<typename R>
        friend auto operator|(R&& r, ValuesAdaptorClosure closure) {
            check<R>();
            using iter_type = typename std::remove_reference_t<R>::iterator;
            using diff_type = typename std::iterator_traits<iter_type>::difference_type;
            using value_type = typename std::iterator_traits<iter_type>::value_type;
            ValuesIter<R, diff_type> begin_iter{r, 0, std::distance(r.begin(), r.end())};
            ValuesIter<R, diff_type> end_iter{r, std::distance(r.begin(), r.end()), std::distance(r.begin(), r.end())};
            return ValuesView<ValuesIter<R, diff_type>>{begin_iter, end_iter};
        }
    };

    struct ValuesAdaptor {
        auto operator()() {
            return ValuesAdaptorClosure{};
        }
    };
}  // namespace details

details::ValuesAdaptor values;