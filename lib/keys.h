#pragma once

#include <iterator>
#include <algorithm>
#include <iostream>
#include "check.h"

namespace details {
    template<typename R, typename DiffType>
    class KeysIter {
    public:
        using iter_type = typename std::remove_reference_t<R>::iterator;
        using value_type = typename std::iterator_traits<iter_type>::value_type;
        using difference_type = DiffType;
        using pointer = typename std::iterator_traits<iter_type>::pointer;
        using reference = typename std::iterator_traits<iter_type>::reference;
        using iterator_category = std::forward_iterator_tag;

        KeysIter() = default;

        KeysIter(R base, DiffType pos, DiffType pos_end)
                : base_(base), pos_(pos), pos_end_(pos_end) {}

        KeysIter& operator++() {
            ++pos_;
            return *this;
        }

        KeysIter& operator--() {
            ---pos_;
            return *this;
        }

        KeysIter operator++(int) {
            KeysIter temp(*this);
            ++pos_;
            return temp;
        }

        KeysIter operator--(int) {
            KeysIter temp(*this);
            --pos_;
            return temp;
        }

        typename value_type::first_type operator*() const {
            auto it = base_.begin();
            std::advance(it, pos_);
            return it->first;
        }

        friend bool operator==(const KeysIter& first, const KeysIter& second) {
            return first.pos_ == second.pos_;
        }

        friend bool operator!=(const KeysIter& first, const KeysIter& second) {
            return !(first == second);
        }

    private:
        R base_;
        DiffType pos_;
        DiffType pos_end_;
    };

    template<typename IterType>
    class KeysView {
    public:
        using value_type = typename IterType::value_type::first_type;
        using iterator = IterType;

        KeysView() = default;

        KeysView(IterType begin, IterType end)
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

    struct KeysAdaptorClosure {
        template<typename R>
        friend auto operator|(R r, KeysAdaptorClosure closure) {
            check<R>();
            using iter_type = typename std::remove_reference_t<R>::iterator;
            using diff_type = typename std::iterator_traits<iter_type>::difference_type;
            using value_type = typename std::iterator_traits<iter_type>::value_type;
            KeysIter<R, diff_type> begin_iter{r, 0, std::distance(r.begin(), r.end())};
            KeysIter<R, diff_type> end_iter{r, std::distance(r.begin(), r.end()), std::distance(r.begin(), r.end())};
            return KeysView<KeysIter<R, diff_type>>{begin_iter, end_iter};
        }
    };

    struct KeysAdaptor {
        auto operator()() {
            return KeysAdaptorClosure{};
        }
    };
}  // namespace details

details::KeysAdaptor keys;