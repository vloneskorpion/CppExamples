#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
#include <type_traits>

template <typename T> 
concept numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;

template <typename T> 
requires std::is_integral_v<T> || std::is_floating_point_v<T>
constexpr double Average(std::vector<T> const &vec) {
    const double sum = std::accumulate(vec.begin(), vec.end(), 0.0);        
    return sum / static_cast<double>(vec.size());
}

template <typename T> 
requires numeric<T>
constexpr double Average2(std::vector<T> const &vec) {
    const double sum = std::accumulate(vec.begin(), vec.end(), 0.0);        
    return sum / static_cast<double>(vec.size());
}

constexpr double Average3(std::vector<numeric auto> const &vec) {
    const double sum = std::accumulate(vec.begin(), vec.end(), 0.0);        
    return sum / static_cast<double>(vec.size());
}

template <typename T> 
std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, double>
Average4(std::vector<T> const &vec) {
    const double sum = std::accumulate(vec.begin(), vec.end(), 0.0);        
    return sum / static_cast<double>(vec.size());
}

int main() {
    std::vector ints { 1, 2, 3, 4, 5};
    std::cout << Average(ints) << '\n';

    std::vector floats { 1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    std::cout << Average4(floats) << '\n';

    //std::vector strings {"abc", "xyz"};
    //auto test = Average4(strings);

    return 0;                                        
}