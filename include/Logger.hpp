/*
 * Description: This module defines a simple logger that clients can use to generate variable verbosity error message.
 * Author: Brandon Chisham
 * Created: Feb. 6, 2005
 * Copyright (c) 2006 
 * This file is licensed under the terms of the LGPL, see the
 * file ../COPYING
 */
#ifndef LOG4CPP_H_
#define LOG4CPP_H_
#include <string>
#include <limits.h>
#include <syslog.h>
#include <iostream>
#include <pthread.h>
#include "AbstractStream.hpp"
//#include "itoa.hpp"
//Available Reporting Levels.
//These levels mirror those provided by syslog(),
//clients are free to define their own. 
const int NO_MESSAGES_LR       = LOG_EMERG;
const int ALERT_MESSAGES_LR    = LOG_ALERT;
const int CRITICAL_MESSAGES_LR = LOG_CRIT;
const int ERROR_MESSAGES_LR    = LOG_ERR;
const int INFO_MESSAGES_LR     = LOG_INFO;
const int ALL_MESSAGES_LR      = LOG_DEBUG;

using namespace std;
typedef int level_t;
/**
 * Logger defines a the variable verbosity logging mechanism.
 */
class Logger  {
public:
  Logger(TypedOutputStream out, 
	 level_t logLevel=NO_MESSAGES_LR): logLevel_(logLevel),out_(out){
    pthread_mutex_init(&lock_, NULL);
  }
  /**
   * Currently does nothing since the Logger does not allocate resources to itself.
   */
  virtual ~Logger(){ out_.close();  pthread_mutex_destroy(&lock_);  }
  /**
   * Tests the log level of the message and reports the message if it has a high enough
   *              priority otherwise the message is suppressed.
   */
  virtual void log(level_t level=ERROR_MESSAGES_LR, const std::string& message="")const{ 
    if (level < logLevel_){
      out_ << message; 
    }
    return;
  }
  /**
   * Changes the reporting level of the logger.
   * It is recommended that the client use one of the provided named constants, but
   * an arbitrary integer will work.
   */
  void changeLevel(const level_t nLevel){ logLevel_= nLevel; }
  /**
   * Changes the log level returning the old level
   */
  virtual level_t  enterSection( const level_t nLevel ){ pthread_mutex_lock(&lock_);  int ret = blockLevel_; blockLevel_ = nLevel; return ret; }
  virtual void     leaveSection(  ){   pthread_mutex_unlock(&lock_); return; }
  /**
   * Supply stream insterters for the builtin types.
   */
  virtual Logger& operator<<(const char& in){ if (blockLevel_ < logLevel_){ out_ << in; } return *this;  }
  virtual Logger& operator<<(const unsigned char& in){ if (blockLevel_ < logLevel_){ out_ << in; } return *this;  }
  virtual Logger& operator<<(const short& in){ if (blockLevel_ < logLevel_){ out_ << in; } return *this;  }
  virtual Logger& operator<<(const unsigned short& in){ if (blockLevel_ < logLevel_){ out_ << in; } return *this;  }
  virtual Logger& operator<<(const int& in){ if (blockLevel_ < logLevel_){ out_ << in; } return *this; }
  virtual Logger& operator<<(const unsigned int& in){ if (blockLevel_ < logLevel_){ out_ << in; } return *this;  }
  virtual Logger& operator<<(const long& in){ if (blockLevel_ < logLevel_){ out_ << in; } return *this; }
  virtual Logger& operator<<(const unsigned long& in){ if (blockLevel_ < logLevel_){ out_ << in; } return *this;  }
  virtual Logger& operator<<(const long long& in){ if (blockLevel_ < logLevel_){ out_ << in; } return *this;  }
  virtual Logger& operator<<(const float& in){ if (blockLevel_ < logLevel_){ out_ << in; } return *this;  }
  virtual Logger& operator<<(const double& in){ if (blockLevel_ < logLevel_){ out_ << in; }return *this;  }
  virtual Logger& operator<<(const char*& in){ if (blockLevel_ < logLevel_){ out_ << in; }return *this;  }
  virtual Logger& operator<<(const std::string& in){ log(blockLevel_, in); return *this; }
  virtual Logger& operator<<(const void*& in){ if (blockLevel_ < logLevel_){ out_ << in;  } return *this;  }
protected:
  /**
   * Specifies the level that this instance will listen for.
   * In the current implementation the level specified indicates
   * the least serious message that the logger will report.
   */
  level_t logLevel_;
  
  level_t blockLevel_; 
  pthread_mutex_t lock_;
  
  TypedOutputStream& out_;
};
#endif 
