#include "SysLogger.hpp"
#include <cstdlib>
#include <cstring>
#include <cstdio>
/*
 * Author: Brandon Chisham
 * Copyright (c) 2008 
 * This file is licensed under the terms of the LGPL, see the
 * file ../COPYING
 */
using namespace std; 
void SysLogger::log(int level, const string& message)const{
  syslog( level, "%s", message.c_str());
  return;
}


#define MAX_LEN_  8192

//#ifndef strnlen
//size_t strnlen( const char* str, size_t max );
//size_t strnlen( const char* str, size_t max ){
 //   size_t ret = 0;
//    for (const char*  i = str; i && *i && ret < max; ++ret, ++i);
//    return ret;
//}
//#endif

char* initBuff(unsigned int size = SysLogger::BUFF_ENTRY_SIZE_){ return  (char*)calloc(size + 1, sizeof(char)); }
void  freeBuff(char* buff){ free( buff );  }
/**
 * Collect message parts and write to the log. 
 */
void     SysLogger::leaveSection( const level_t nLevel ){
  string message = ""; 
  for (vector<char*>::iterator i = buff_.begin(); i != buff_.end(); ++i ){
    message += string(*i);
    freeBuff( *i );
  }
  buff_ = vector<char*>();
  log( blockLevel_, message);
  changeLevel( nLevel );
  pthread_mutex_unlock( &lock_ );
  return;
}

Logger& SysLogger::operator<<(const char& in){ 
  char* nbuff = initBuff(); 
  nbuff[0] = in; 
  buff_.push_back( nbuff );   
  return *this;  
}
Logger& SysLogger::operator<<(const short& in){ 
  char* nbuff = initBuff(); 
  snprintf( nbuff, BUFF_ENTRY_SIZE_, "%d", in ); 
  buff_.push_back( nbuff );  
  return *this;  
}
Logger& SysLogger::operator<<(const unsigned short& in){ 
  char* nbuff = initBuff(); 
  snprintf( nbuff, BUFF_ENTRY_SIZE_, "%ud", in ); 
  buff_.push_back( nbuff );  
  return *this;   
}
Logger& SysLogger::operator<<(const int& in){ 
  char* nbuff = initBuff(); 
  snprintf( nbuff, BUFF_ENTRY_SIZE_, "%d", in ); 
  buff_.push_back( nbuff ); 
  return *this;  
}
Logger& SysLogger::operator<<(const unsigned int& in){
  char* nbuff = initBuff(); 
  snprintf( nbuff, BUFF_ENTRY_SIZE_, "%ud", in ); 
  buff_.push_back( nbuff );  return *this;   
}
Logger& SysLogger::operator<<(const long& in){ 
  char* nbuff = initBuff(); 
  snprintf( nbuff, BUFF_ENTRY_SIZE_, "%ld", in ); 
  buff_.push_back( nbuff ); 
  return *this;  
}
Logger& SysLogger::operator<<(const unsigned long& in){
  char* nbuff = initBuff(); 
  snprintf( nbuff, BUFF_ENTRY_SIZE_, "%uld", in ); 
  buff_.push_back( nbuff );  
  return *this;   
}
Logger& SysLogger::operator<<(const long long& in){ 
  char* nbuff = initBuff(); 
  snprintf( nbuff, BUFF_ENTRY_SIZE_, "%lld", in ); 
  buff_.push_back( nbuff ); 
  return *this; 
}
Logger& SysLogger::operator<<(const float& in){
  char* nbuff = initBuff(); 
  snprintf( nbuff, BUFF_ENTRY_SIZE_, "%f", in ); 
  buff_.push_back( nbuff );  
  return *this;  
}
Logger& SysLogger::operator<<(const double& in){  
  char* nbuff = initBuff(); 
  snprintf( nbuff, BUFF_ENTRY_SIZE_, "%g", in ); 
  buff_.push_back( nbuff ); 
  return *this;  
}

Logger& SysLogger::operator<<(const char*& in){ 
  size_t len = strnlen(in, MAX_LEN_);  
  char* nbuff = initBuff(len); 
  strncpy(nbuff, in, len); 
  buff_.push_back( nbuff );  
  return *this; 
}
Logger& SysLogger::operator<<(const std::string& in){ 
  return *this << in.c_str(); 
}
Logger& SysLogger::operator<<(const void*& in){ 
  char* nbuff = initBuff(); 
  snprintf( nbuff, BUFF_ENTRY_SIZE_, "%p", in ); 
  buff_.push_back( nbuff );  
  return *this;  
}
