#ifndef YNABConvertException_HPP
#define YNABConvertException_HPP

#include <stdexcept>

class YNABConvertException : public std::exception 
{
    const string _what;
    
    public:
    YNABConvertException(const std::string& what) : _what(what) {};
    const char * what () const noexcept
    {
        return _what.c_str();
    }
    
};

class YNABConvertInvalidBudgetException : public YNABConvertException
{
    public:
    YNABConvertInvalidBudgetException(const std::string& what) : YNABConvertException(what) {};
};

#endif