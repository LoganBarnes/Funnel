#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace ssf {

template <typename T>
struct TestData
{
    std::vector<glm::tvec2<T>> input;
    std::vector<glm::tvec2<T>> expected_output;
};

template <typename T>
TestData<T> empty_test()
{
    TestData<T> data;

    data.input = {};
    data.expected_output = {};

    return data;
}

template <typename T>
TestData<T> single_point_test()
{
    TestData<T> data;

    data.input = {{21, -7}};
    data.expected_output = {{21, -7}};

    return data;
}

template <typename T>
TestData<T> single_line_test()
{
    TestData<T> data;

    data.input = {{13, 5}, {-15, 3}};
    data.expected_output = {{13, 5}, {-15, 3}};

    return data;
}

template <typename T>
TestData<T> single_ccw_triangle_test()
{
    TestData<T> data;

    data.input = {{1, 0}, {5, 3}, {3, 5}};
    data.expected_output = {{1, 0}, {3, 5}};

    return data;
}

template <typename T>
TestData<T> single_cw_triangle_test()
{
    TestData<T> data;

    data.input = {{1, 0}, {-2, 3}, {3, 5}};
    data.expected_output = {{1, 0}, {3, 5}};

    return data;
}

template <typename T>
TestData<T> two_triangles_starting_ccw_test()
{
    TestData<T> data;

    data.input = {{1, 0}, {5, 3}, {3, 5}, {8, 3}};
    data.expected_output = {{1, 0}, {5, 3}, {8, 3}};

    return data;
}

template <typename T>
TestData<T> two_triangles_starting_cw_test()
{
    TestData<T> data;

    data.input = {{1, 0}, {-2, 3}, {3, 5}, {5, 7}};
    data.expected_output = {{1, 0}, {3, 5}, {5, 7}};

    return data;
}

template <typename T>
struct Test
{
    decltype(&ssf::empty_test<T>) func;
    std::string name;
};

#define MAKE_TEST(test_func)                                                                                           \
    Test<T> { test_func, #test_func }

template <typename T>
std::vector<Test<T>> test_cases()
{
    static std::vector<Test<T>> test_funcs{
        MAKE_TEST(empty_test<T>),
        MAKE_TEST(single_point_test<T>),
        MAKE_TEST(single_line_test<T>),
        MAKE_TEST(single_ccw_triangle_test<T>),
        MAKE_TEST(single_cw_triangle_test<T>),
        MAKE_TEST(two_triangles_starting_ccw_test<T>),
        MAKE_TEST(two_triangles_starting_cw_test<T>),
    };
    return test_funcs;
}

} // namespace ssf
