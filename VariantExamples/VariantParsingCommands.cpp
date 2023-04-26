#include <iostream>
#include <variant>
#include <map>
#include <charconv>
#include <cstring>
#include <optional>

class CmdLine
{
public:
    using Arg = std::variant<int, std::string>;
    
private:
    std::map<std::string, Arg> mParsedArgs;

public:
    explicit CmdLine(int argc, char** argv) { ParseArgs(argc, argv); }
    
    std::optional<Arg> Find(const std::string& name) const 
    {
        auto it = mParsedArgs.find(name);
        if (it != mParsedArgs.end())
            return it->second;
            
        return { };
    }
    
    void ParseArgs(int argc, char** argv);
};

// at the moment from_chars for float/double is not supported, so only ints
CmdLine::Arg TryParseString(char* arg)
{
    // try with int first
    int iResult = 0;
    auto res = std::from_chars(arg, arg+strlen(arg), iResult);
    if (res.ec == std::errc::invalid_argument)
    {
        // if not possible, then just assume it's a string
        return std::string(arg);
    }
    
    return iResult;
}

void CmdLine::ParseArgs(int argc, char** argv)
{
    // the form: -argName value -argName value
    // unnamed? later...
    for (int i = 1; i < argc; i+=2)
    {
        if (argv[i][0] != '-')
            throw std::runtime_error("wrong command name");
        
        mParsedArgs[argv[i]+1] = TryParseString(argv[i+1]);
    }
}

int main(int argc, char** argv) 
{
    // run with -paramInt 10 -textParam "Hello World"
    try
    {
        CmdLine cmdLine(argc, argv);
        
        auto arg = cmdLine.Find("paramInt");
        if (arg && std::holds_alternative<int>(*arg))
            std::cout << "paramInt is " << std::get<int>(*arg) << "\n";
        
        arg = cmdLine.Find("textParam");
        if (arg && std::holds_alternative<std::string>(*arg))
            std::cout << "textParam is " << std::get<std::string>(*arg) << "\n";    
    }
    catch (std::runtime_error &err)
    {
        std::cout << err.what() << "\n";
    }
    
    return 0;
}