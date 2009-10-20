#include "CStream.hpp"

 std::string  CInputStream::readString(){
    std::string ret;
    char current = fgetc(handle_);
    while (isspace(current = fgetc(handle_)));//remove leading white-space 
    while (!(current == EOF || current == '\n' || current == '\t' || current == ' ' || current == '\r')){
      ret += current;
      current = fgetc(handle_);
    }
    return ret;
  }
