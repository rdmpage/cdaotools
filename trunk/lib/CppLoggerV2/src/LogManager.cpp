#include "LogManager.hpp"
#include <iterator>
//#include <iostream>
/*
 * This is the main entry point of the asynchronous logger. 
 * The method will run indefinitely until the stopLogger() method
 * of the LogManager singleton is called.
 * This method ignores parameters.
 * Copyright (c) 2008 
 * This file is licensed under the terms of the LGPL, see the
 * file ../COPYING
 */
void* logger_thread_start(void*);

//<--------Declare Storage for Static Class Members----------->
std::list<Logger* > LogManager::listeners_ = std::list<Logger* >();
pthread_mutex_t LogManager::lock_ = PTHREAD_MUTEX_INITIALIZER;
LogManager* LogManager::instance_ = NULL;
sem_t LogManager::referenceCount_;
bool LogManager::continue_ = true;
pthread_t LogManager::loggerThread_;
//<----------------------------------------------------------->
LogManager::LogManager(){    
  //pthread_mutex_init(&lock_, NULL);
 // sem_init(&referenceCount_,0,1);
  //start the logger thread.
  pthread_create(&loggerThread_, NULL, logger_thread_start,NULL );
  //put don't synchronize with the thread on completion.
  pthread_detach(loggerThread_);
}

/**
 * Adds a new listener to the subscriber list.
 */
void LogManager::registerListener(Logger* nLog){
  pthread_mutex_lock(&lock_);
  listeners_.push_back(nLog);
  pthread_mutex_unlock(&lock_);
  return;
}
/**
 * Broadcasts the messages to all the subscribers in the list.
 */
void LogManager::log(level_t level, const string& message)const{
 //block access to the subscriber list while the message is being sent.
  pthread_mutex_lock(&lock_);
  if (listeners_.size()){
    for(list<Logger* >::const_iterator i = listeners_.begin() ; i != listeners_.end(); ++i){
      (*i)->log(level, message);
    }
  }
  pthread_mutex_unlock(&lock_);
  return;
}

void LogManager::startMultiPartMessage( level_t level ){
  if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      (*i)->enterSection( level );
    }
  }
  return;
}

void LogManager::endMultiPartMessage(){
   if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      (*i)->leaveSection( );
    }
  }
  return;
}
  
LogManager& LogManager::operator<<(const char& in){
   if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const unsigned char& in){
 if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
} 
LogManager& LogManager::operator<<(const short& in){
 if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const unsigned short& in){
 if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const int& in){
 if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
     **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const unsigned int& in){
 if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const long& in){
 if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const unsigned long& in){
 if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const long long& in)
{
 if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const float& in){
 if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const double& in){
   if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const char*& in){
   if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const std::string& in){
   if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}
LogManager& LogManager::operator<<(const void*& in){
   if (listeners_.size()){
    for (list< Logger* >::iterator i = listeners_.begin(); i != listeners_.end(); ++i){
      **i << in;
    }
  }
  return *this;
}

/*
 * This is the entry point for the logger thread. This will run until
 * the stopLogger() method is called by an external client.
 */
void* logger_thread_start(void*){
  //get a local handle to the LogManager..
  LogManager lmgr = LogManager::getInstance();
  //run until the logger receives a stop message from another thread.
  while (lmgr.getContinue()){ usleep(500);  }
  return NULL;
}
