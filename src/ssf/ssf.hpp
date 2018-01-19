#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <vector>

// enable the printing of glm::tvec2s
namespace glm {
template <typename T>
::std::ostream &operator<<(::std::ostream &os, const glm::tvec2<T> &v)
{
    return os << glm::to_string(v);
}
} // namespace glm

namespace ssf {

template <typename T>
std::vector<glm::tvec2<T>> run_simple_stupid_funnel_algorithm(const std::vector<glm::tvec2<T>> &triangle_strip);

namespace detail {

} // namespace detail

} // namespace ssf
