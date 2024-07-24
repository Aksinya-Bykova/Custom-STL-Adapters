# STL. Adapters.

* transform - change the values of the elements, similar to how the transform algorithm does it
* filter - filtering by a specific attribute, the attribute is passed as an argument
* take - takes only the first N elements
* drop - skip the first N elements
* reverse - reverse
* keys - keys for associative containers
* values - values for associative containers

Possible application method:

```cpp
std::vector<int> v = {1,2,3,4,5,6};
std::vector<int> result;

std::copy_if(v.begin(), v.end(), std::back_inserter(result), [](int i){return i % 2;});
std::transform(result.begin(), result.end(), result.begin(), [](int i){return i * i;});

for(int i : result)
    std::cout << i << " ";
```

This task could be solved more "elegantly"

```cpp
std::vector<int> v = {1,2,3,4,5,6};

auto removed = v | filter([](int i){return i % 2;});
auto result = removed | transform([](int i){return i * i;});

for(int i : result)
    std::cout << i << " ";
```

or even more briefly, using [a pipeline, similar to how it is accepted in unix systems](https://en.wikipedia.org/wiki/Pipeline_ (Unix))

```cpp
std::vector<int> v = {1,2,3,4,5,6};

for(int i : v | filter([](int i){return i % 2;}) | transform([](int i){return i * i;}))
     std::cout << i << " ";
```

Here is a custom implementation of these adapters with support for range based for
