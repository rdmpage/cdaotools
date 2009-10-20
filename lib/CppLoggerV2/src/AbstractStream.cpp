/*
 * Author: Brandon Chisham
 * Date: May 26, 2006
 * Copyright (c) 2006
 * See file COPYING for details. 
 */
/*
 * Description: This file contains implementation of non-trivial methods for AbstractStream
 * and its descendants. 
 */
#include "AbstractStream.hpp"
#include "CStream.hpp"
#include "CPPStream.hpp"
#include "RawStream.hpp"
#include <ctype.h>
#include <assert.h>
using namespace std;
const size_t ABS_STREAM_TEMP_BUFF_SIZE_ = 24;

void initBuffer(char* buff){
  for (unsigned int i = 0; i < ABS_STREAM_TEMP_BUFF_SIZE_; ++i){ buff[ i ] = '\0';  }
}



std::string AbstractOutputStream::convert(const char in){ 
  char buf[] = "\0";
  buf[ 0 ] = in;
  return string( buf );
}
/*
 * Convert() methods expand their arguments into a string representation.
 */
string AbstractOutputStream::convert(const int in){
  char* buff = new char[ ABS_STREAM_TEMP_BUFF_SIZE_ ];
  initBuffer( buff );
  snprintf(buff, ABS_STREAM_TEMP_BUFF_SIZE_ - 1,"%d",in);
  return string(buff);
}

string AbstractOutputStream::convert(const unsigned int in){
  char* buff = new char[ ABS_STREAM_TEMP_BUFF_SIZE_ ];
  initBuffer( buff );
  snprintf(buff, ABS_STREAM_TEMP_BUFF_SIZE_ - 1,"%ud",in);
  return string(buff);
}

string AbstractOutputStream::convert(const long in){
  char* buff = new char[ ABS_STREAM_TEMP_BUFF_SIZE_ ];
  initBuffer( buff );
  snprintf(buff, ABS_STREAM_TEMP_BUFF_SIZE_ - 1,"%ld",in);
  return string(buff);
}

string AbstractOutputStream::convert(const unsigned long in){
  char* buff = new char[ ABS_STREAM_TEMP_BUFF_SIZE_ ];
  initBuffer( buff );
  snprintf(buff, ABS_STREAM_TEMP_BUFF_SIZE_ - 1,"%ld",in);
  return string(buff);
}


string AbstractOutputStream::convert(const long long in){
  char* buff = new char[ ABS_STREAM_TEMP_BUFF_SIZE_ ];
  initBuffer( buff );
  snprintf(buff, ABS_STREAM_TEMP_BUFF_SIZE_ - 1,"%lld",in);
  return string(buff);
}

string AbstractOutputStream::convert(const double in){
  char* buff = new char[ ABS_STREAM_TEMP_BUFF_SIZE_ ];
  initBuffer( buff );
  snprintf(buff, ABS_STREAM_TEMP_BUFF_SIZE_ - 1,"%g",in);
  return string(buff);
}

string AbstractOutputStream::convert(const void* in){
  char* buff = new char[ ABS_STREAM_TEMP_BUFF_SIZE_ ];
  initBuffer( buff );
  snprintf(buff, ABS_STREAM_TEMP_BUFF_SIZE_ - 1,"%p",in);
  return string(buff);
}


thread_safe_ptr<TypedOutputStream>  StreamFactory::getOutputInstance(ostream& out){
  return thread_safe_ptr<TypedOutputStream>(new TypedCppOutputStream(out));
}
thread_safe_ptr<TypedOutputStream>  StreamFactory::getOutputInstance(FILE* out){
  return thread_safe_ptr<TypedOutputStream>(new TypedCOutputStream(out));
}
thread_safe_ptr<TypedOutputStream>  StreamFactory::getOutputInstance(int out){
  return thread_safe_ptr<TypedOutputStream>(new TypedRawOutputStream(out));
}
thread_safe_ptr<TypedInputStream>  StreamFactory::getInputInstance(istream& in){
  return thread_safe_ptr<TypedInputStream>(new CppInputStream(in));
}
thread_safe_ptr<TypedInputStream> StreamFactory::getInputInstance(FILE* in){
  return thread_safe_ptr<TypedInputStream>(new CInputStream(in));
}
thread_safe_ptr<TypedInputStream>  StreamFactory::getInputInstance(int in){
  return thread_safe_ptr<TypedInputStream>(new TypedRawInputStream(in));
}
