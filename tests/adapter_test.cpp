#include <gtest/gtest.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "../lib/take.h"
#include "../lib/values.h"
#include "../lib/keys.h"
#include "../lib/filter.h"
#include "../lib/transform.h"
#include "../lib/reverse.h"
#include "lib/drop.h"

TEST(TakeCase, FullTake) {
    std::vector<int> v = {5, 6, 239};
    auto x = v | take(3);

    int i = 0;
    for (auto el: x) {
        ASSERT_EQ(el, v[i]);
        ++i;
    }
}

TEST(TakeCase, ManyTakes) {
    std::vector<int> v = {5, 6, 239, 0, 4, 3, 5};
    auto x = v | take(7);
    auto y = x | take(3);

    int i = 0;
    for (auto el: y) {
        ASSERT_EQ(el, v[i]);
        ++i;
    }
}

TEST(FewCases, TakeFilterTransform) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};

    auto taken_vector = v | take(4);
    auto taken_vector_next = taken_vector | take(2);
    auto result = taken_vector_next | filter([](int i) { return i % 3; });
    auto transformed = result | transform([](int i) { return i * i; });

    int i = 0;
    for (auto el: transformed) {
        ASSERT_EQ(el, v[i] * v[i]);
        ++i;
    }
}

TEST(DropCase, FullTake) {
    std::vector<int> v = {5, 6, 239};
    auto x = v | drop(2);

    int i = 2;
    for (auto el: x) {
        ASSERT_EQ(el, v[i]);
        ++i;
    }
}

TEST(ReverseCase, VestorReverse) {
    std::vector<int> v1 = {5, 6, 239, 1, 2, 3};
    std::vector<int> v2 = {3, 2, 1, 239, 6, 5};

    auto x = v1 | reverse();

    int i = 0;
    for (auto el: x) {
        ASSERT_EQ(el, v2[i]);
        ++i;
    }
}

TEST(KeysCase, MapKeys) {
    std::map<std::string, int> m = {{"aaa", 1},
                                    {"bbb", 2},
                                    {"ccc", 3},
                                    {"ddd", 4}};

    auto map_keys = m | keys();

    std::vector<std::string> v = {"aaa", "bbb", "ccc", "ddd"};

    int i = 0;
    for (auto el: map_keys) {
        ASSERT_EQ(el, v[i]);
        ++i;
    }

    std::vector<std::string> v1 = {"xxx", "bbb", "ccc", "ddd"};

    i = 0;
    bool flag = false;
    for (auto el: map_keys) {
        if (el != v1[i]) {
            flag = true;
            break;
        }
        ++i;
    }

    ASSERT_TRUE(flag);
}

TEST(KeysCase, MapValues) {
    std::map<std::string, int> m = {{"aaa", 1},
                                    {"bbb", 2},
                                    {"ccc", 3},
                                    {"ddd", 4}};

    auto map_keys = m | values();

    std::vector<int> v = {1, 2, 3, 4};

    int i = 0;
    for (auto el: map_keys) {
        ASSERT_EQ(el, v[i]);
        ++i;
    }

    std::vector<int> v1 = {666, 36, 239};

    i = 0;
    bool flag = false;
    for (auto el: map_keys) {
        if (el != v1[i]) {
            flag = true;
            break;
        }
        ++i;
    }

    ASSERT_TRUE(flag);
}

