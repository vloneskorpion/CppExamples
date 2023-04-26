#include <string>
#include <iostream>
#include <variant>
#include <vector>

struct MultiplyVisitor
{
    float mFactor;

    MultiplyVisitor(float factor) : mFactor(factor) { }

    void operator()(int& i) const {
        i *= static_cast<int>(mFactor);
    }

    void operator()(float& f) const {
        f *= mFactor;
    }

    void operator()(std::string& ) const {
        // nothing to do here...
    }
};

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

int main() 
{
    // a generic lambda:
    auto PrintVisitor = [](const auto& t) { std::cout << t << "\n"; };
    auto TwiceMoreVisitor = [](auto& t) { t*= 2; };

    std::variant<int, float, std::string> intFloatString { "Hello" };
    std::visit(PrintVisitor, intFloatString);

    std::variant<int, float> intFloat { 20.4f };
    std::visit(PrintVisitor, intFloat);
    std::visit(TwiceMoreVisitor, intFloat);
    std::visit(PrintVisitor, intFloat);
    std::visit(MultiplyVisitor(0.5f), intFloat);
    std::visit(PrintVisitor, intFloat);

    std::visit(overload{
    [](int& i) { i*= 2; },
    [](float& f) { f*= 2.0f; },
    [](std::string& s) { s = s + s; }
    }, intFloatString);
    std::visit(PrintVisitor, intFloatString);

}