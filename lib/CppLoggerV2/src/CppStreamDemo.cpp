/*
 * Author: Brandon Chisham
 * Date: Sept. 11, 2006
 * Description: Example of using the CppStream library.
 */

#include "AbstractStream.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

struct Foo_t {
private:
  char* bar;
public:
  Foo_t(){bar = "Foo";}
  friend TypedOutputStream& operator<<(TypedOutputStream& out, const Foo_t& rhs){ out << rhs.bar; }
};

int main(){
  
  TypedCppOutputStream out = TypedCppOutputStream(cout);
  CppInputStream in = CppInputStream(cin);
  
  int i = 0;
  short s = 1;
  string str = "Hello World";
  float f = 0.9;
  double d = 0.75;
  long l = 34;
  long long ll = 69;
  int* vs = &i;
  Foo_t foo; 
  
  out << "Enter an integer: " ;
  in >> i;
  out << "Enter a short: ";
  in >> s;
  out << "Enter a float: ";
  in >> f;
  out << "Enter a string: ";
  in >> str;
  out << "Enter a double: ";
  in >> d;
  out << "Enter a  long: ";
  in >> l;
  out << "Enter a long long: ";
  in >> ll;
  out << "Enter a hex number: ";
  in >> vs;
  out << "***Displaying the results***\n";
  
  out << "int " << i << '\n';
  out << "short " << s << '\n';
  out << "float " << f << '\n';
  out << "string " << str << '\n';
  out << "double " << d << '\n';
  out << "long " << l << '\n';
  out << "long long " << ll << '\n';
  out << "void* " << vs << '\n';
  out << "Foo_t " << foo << "\n";
  
  return 0;
}
