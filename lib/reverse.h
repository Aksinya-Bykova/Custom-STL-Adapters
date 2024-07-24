#pragma once

#include <iterator>
#include <algorithm>
#include <iostream>
#include "check.h"

struct reverse {
};

template<typename R>
class ReverseView {
public:
    ReverseView(const R& base) : base_(base) {}

    auto base_begin() const {
        return base_.begin();
    }

    auto base_end() const {
        return base_.end();
    }

    auto begin() const {
        return std::make_reverse_iterator(base_end());
    }

    auto end() const {
        return std::make_reverse_iterator(base_begin());
    }

private:
    const R& base_;
};

template<typename R>
ReverseView<R> operator|(const R& base, const reverse&) {
    check<R>();
    return ReverseView<R>(base);
}
