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

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};

    auto taken_vector = v | take(4);
    auto taken_vector_next = taken_vector | take(2);
    auto removed = v | filter([](int i) { return i % 2; });
    auto result = taken_vector_next | filter([](int i) { return i % 3; });
    auto transformed = result | transform([](int i) { return i * i; });
    auto result1 = transformed | filter([](int i) { return i % 3; });

    std::cout << "Take 4, take 2, can't be divided by 3, squared, can't be divided by 3," << "\n";
    for (auto i: result1) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    auto reversed = v | reverse();
    std::cout << "Reversed" << "\n";
    for (auto i: reversed) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "Smart cycle" << "\n";
    for (int i: v |
                filter([](int i) { return i % 2; }) |
                transform([](int i) { return i * i; })) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::map<std::string, int> m = {{"aaa", 1},
                                    {"bbb", 2},
                                    {"ccc", 3},
                                    {"ddd", 4}};

    auto map_keys = m | keys();

    std::cout << "Keys" << "\n";
    for (auto i: map_keys) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    auto map_values = m | values();

    std::cout << "Values" << "\n";
    for (auto i: map_values) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    return 0;
}
