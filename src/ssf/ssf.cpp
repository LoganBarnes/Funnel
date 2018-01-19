#include "ssf.hpp"

namespace ssf {

template <typename T>
std::vector<glm::tvec2<T>> run_simple_stupid_funnel_algorithm(const std::vector<glm::tvec2<T>> &triangle_strip)
{
    if (triangle_strip.size() < 3) {
        return triangle_strip;
    }
    if (triangle_strip.size() == 3) {
        return {triangle_strip.front(), triangle_strip.back()};
    }

    return {};
}

template std::vector<glm::vec2> run_simple_stupid_funnel_algorithm<float>(const std::vector<glm::vec2> &);
template std::vector<glm::dvec2> run_simple_stupid_funnel_algorithm<double>(const std::vector<glm::dvec2> &);

} // namespace ssf
