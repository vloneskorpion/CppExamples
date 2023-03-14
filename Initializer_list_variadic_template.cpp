#include <iostream>
#include <vector>
#include <memory>

struct Shape {
    virtual void render() const {
        std::cout << "rendering shape\n";
    }
};

struct Circle : Shape {
    void render() const override {
        std::cout << "rendering circle\n";
    }
};

struct Rectangle : Shape {
    void render() const override {
        std::cout << "rendering rectangle\n";
    }
};

template<typename T, typename... Args>
auto initHelper(Args&&... args)
{
    std::vector<T> vec;
    vec.reserve(sizeof...(Args)); 
    (vec.emplace_back(std::forward<Args>(args)), ...);
    return vec;
}

int main() {
    /* does not work:
    std::vector<std::unique_ptr<Shape>> shapes {
        std::make_unique<Circle>(),
        std::make_unique<Rectangle>()
    };
    */

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.reserve(2);
    shapes.push_back(std::make_unique<Circle>());
    shapes.emplace_back(std::make_unique<Rectangle>());

    auto shapes2 = initHelper<std::unique_ptr<Shape>>(
        std::make_unique<Circle>(), std::make_unique<Rectangle>());
}