/* 
 * File:   util.hpp
 * Author: bchisham
 *
 * Created on February 24, 2010, 11:43 PM
 */
#include <string>
#ifndef _UTIL_HPP
#define	_UTIL_HPP
 /**
  * Convert a std::string into a wstring.
  */
 std::wstring str_to_wstr( const std::string& src);
 /**
  * Convert a std::wstring into a std::string.
  */
  std::string wstr_to_str( const std::wstring& src );

#endif	/* _UTIL_HPP */

