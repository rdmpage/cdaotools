#ifndef SYS_LOGGER_HPP_
#define SYS_LOGGER_HPP_
#include "Logger.hpp"
#include <syslog.h>
#include <vector>

/*
 * Author: Brandon Chisham
 * Copyright (c) 2008 
 * This file is licensed under the terms of the LGPL, see the
 * file ../COPYING
 */
/**
 * This Logger provides an interface to the system logger.
 */
class SysLogger : public Logger {
public:
  /**
   * Allows the user to tune the logger.
   */
  SysLogger(TypedOutputStream dontUse,
	    const char* ident=NULL, 
	    int option = LOG_NDELAY, 
	    int facility  = LOG_USER, 
	    level_t nLogLevel = ERROR_MESSAGES_LR):Logger( dontUse,  nLogLevel )
  {
    openlog(ident, option, facility|nLogLevel);
    buff_ = std::vector< char* >();
  }
  /**
   * Releases syslog connection.
   */
  virtual ~SysLogger(){
    closelog();
  }
  /**
   * Handles receipt of preformatted messages.
   */
  virtual void log(int level=ERROR_MESSAGES_LR, const string& message="")const;
  /**
   * Collect message parts and write to the log. 
   */
  virtual void     leaveSection( const level_t nLevel );
  /**
   * Supply stream insterters for the builtin types.
   */
  virtual Logger& operator<<(const char& in);
  virtual Logger& operator<<(const short& in);
  virtual Logger& operator<<(const unsigned short& in);
  virtual Logger& operator<<(const int& in);
  virtual Logger& operator<<(const unsigned int& in);
  virtual Logger& operator<<(const long& in);
  virtual Logger& operator<<(const unsigned long& in);
  virtual Logger& operator<<(const long long& in);
  virtual Logger& operator<<(const float& in);
  virtual Logger& operator<<(const double& in);
  virtual Logger& operator<<(const char*& in);
  virtual Logger& operator<<(const std::string& in);
  virtual Logger& operator<<(const void*& in);
  static const int BUFF_ENTRY_SIZE_ = 24;
protected:


std::vector<char*> buff_;
  
};
#endif
