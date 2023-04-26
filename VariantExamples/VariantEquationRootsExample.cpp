#include <iostream>
#include <variant>
#include <cmath>

using DoublePair = std::pair<double, double>;
using EquationRoots = std::variant<DoublePair, double, std::monostate>; 

EquationRoots FindRoots(double a, double b, double c)
{
    if (a == 0.0)
        return std::nan("");

    auto d = b*b-4*a*c; // discriminant
    if (d > 0.0)
    {
        double root1 = (-b + std::sqrt(d)) / (2*a);
        double root2 = (-b - std::sqrt(d)) / (2*a);
        return std::make_pair(root1, root2);
    }
    else if (d == 0.0)
        return (-1*b)/(2*a);
    
    return std::monostate();
}

struct RootPrinterVisitor
{
    void operator()(const DoublePair& arg) 
    { 
        std::cout << "2 roots found: " << arg.first << " " << arg.second << '\n'; 
    }
    void operator()(double arg) 
    { 
        if (std::isnan(arg))
            std::cout << "not a quadratic equation!" << arg << '\n'; 
        else
            std::cout << "1 root found: " << arg << '\n'; 
    }
    void operator()(std::monostate) 
    { 
        std::cout << "No real roots found.\n"; 
    }
};

int main() 
{
    std::visit(RootPrinterVisitor{}, FindRoots(10,0,-2));
    std::visit(RootPrinterVisitor{}, FindRoots(2,0,-1));
    std::visit(RootPrinterVisitor{}, FindRoots(1,2,1));
    std::visit(RootPrinterVisitor{}, FindRoots(1,2,3));
    std::visit(RootPrinterVisitor{}, FindRoots(0,2,3));
}