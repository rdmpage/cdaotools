#include "util.hpp"
#include <string>
#include <iostream>
/**
 * Convert to a wide string applying the standard libray widen function
 * to each element in the string. (yes this is most likey not the most efficient.
 */
std::wstring str_to_wstr( const std::string& src){
    std::wstring ret = L"";
    //std::copy( src.begin(), src.end(), ret.begin() );
    for (std::string::const_iterator i = src.begin(); i != src.end(); ++i ){
        ret += std::cin.widen( *i );
    }
    return ret;
}
/**
 * Convert a to a standard string, using the standard library "narrow" function.
 */
std::string wstr_to_str( const std::wstring& src ){
    std::string ret = "";
    //std::copy( src.begin(), src.end(), ret.begin() );
    for (std::wstring::const_iterator i = src.begin(); i != src.end(); ++i){
        ret += std::cin.narrow(*i, '\0');
    }
    return ret;
}
