/*
 * Author: Brandon Chisham
 * Created: December 11, 2005
 * Module Description:
 *  This file defines a singleton logging facility.
 * Copyright (c) 2008 
 * This file is licensed under the terms of the LGPL, see the
 * file ../COPYING
 */
#ifndef LOG_MANAGER_HPP_
#define LOG_MANAGER_HPP_
#include <list>
#include <errno.h>
#include <string>
#include <fstream>
#include <iostream>
#include <Logger.hpp>
#include <SysLogger.hpp>
//#include <thread_safe_pointer.hpp>
#include <pthread.h>
//Used to implement reference-counting.
#include <semaphore.h>
/**
 * LogMangager specifies an flexible singleton, publisher/subscriber patterned logging
 *              facility. Loggers subscribe with it and are then informed when 
 *              log events occur. 
 */
class LogManager {
protected:  
  /**
   * Creates a default logger with no listeners.
   */
  LogManager();
public:
  static LogManager& getInstance(){
    if (LogManager::instance_ == NULL){ LogManager::instance_ = new LogManager(); }
    return *LogManager::instance_;
  }
  /**
   * Releases the mutex and heap space to the system when the
   *              last handle to the LogManager instance has gone out of scope.
   */
  virtual ~LogManager(){
    //destroy the object only when no handles remain.
    //check without blocking.
      if(EAGAIN == sem_trywait(&referenceCount_)){
	pthread_mutex_destroy(&lock_);
	delete instance_;
	instance_ = NULL;
	//listeners_.clear();
	sem_destroy(&referenceCount_);
      }
  }
  /**
   * Adds a new log listener to the system. 
   */
  void registerListener(Logger* nLog);
  /**
   * Broadcasts the specified string to all subscribed loggers.
   */
  void log(level_t level=ERROR_MESSAGES_LR, const string& message = "")const;
  bool getContinue(){return continue_;}
  void stopLogger(){continue_ = false;}
  
  void startMultiPartMessage( level_t level );
  void endMultiPartMessage();
  /**
   * Supply stream insterters for the builtin types.
   */
  virtual LogManager& operator<<(const char& in);
  virtual LogManager& operator<<(const unsigned char& in); 
  virtual LogManager& operator<<(const short& in);
  virtual LogManager& operator<<(const unsigned short& in);
  virtual LogManager& operator<<(const int& in);
  virtual LogManager& operator<<(const unsigned int& in);
  virtual LogManager& operator<<(const long& in);
  virtual LogManager& operator<<(const unsigned long& in);
  virtual LogManager& operator<<(const long long& in);
  virtual LogManager& operator<<(const float& in);
  virtual LogManager& operator<<(const double& in);
  virtual LogManager& operator<<(const char*& in);
  virtual LogManager& operator<<(const std::string& in);
  virtual LogManager& operator<<(const void*& in);

private:
  /**
   * Stores the subscribers.
   */
  static std::list<Logger* > listeners_;
  /**
   * Used to provide thread-safe access to the subscriber list.
   */
  static pthread_mutex_t lock_;
  /**
   * Singleton instance.
   */
  static LogManager* instance_;
  /**
   * used to automatically destroy the object when it is no longer referenced.
   */
  static sem_t referenceCount_;
  static bool  continue_;
  //static bool created_ = false;
  static pthread_t loggerThread_;
};


class LoggerFactory {
public:
  static  Logger*  getLogger( FILE* file, level_t level ){ return  new Logger( *StreamFactory::getOutputInstance(file), level  );  }
  static  Logger*  getLogger( ofstream& file, level_t level ){ return   new Logger( *StreamFactory::getOutputInstance(file), level );  }
  static  Logger*  getLogger( int file, level_t level ){ return  new Logger( *StreamFactory::getOutputInstance(file),level ); }
  static  Logger*  getLogger( TypedOutputStream& file, level_t level ){ return  new Logger( file, level ) ; }
  static  Logger*  getLogger(const char* ident, 
			   int option, 
			   int facility, 
			   level_t level){ return  new SysLogger( *StreamFactory::getOutputInstance( std::cerr ), 
								 ident, 
								 option, 
								 facility, 
								 level ); }

};

#endif
