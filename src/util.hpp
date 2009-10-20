#ifndef UTIL_HPP_
#define UTIL_HPP_
#include <string>

namespace CDAO {
  /*
   * Generate a document-wide unique identifier
   */
  std::string labelMaker( std::string tag_type );
  /*
   * Convert numbers to strings.
   */
  std::string number_to_string( int number );
}
#endif
