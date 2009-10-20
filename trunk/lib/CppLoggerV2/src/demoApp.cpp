/*
 * Author: Brandon Chisham
 * Created: May 25, 2006
 * Short demo that illustrates the capabilites of the logging facility.
 * Copyright (c) 2006 
 * This file is licensed under the terms of the LGPL, see the
 * file ../COPYING
 */
//#include <pthread.h>
//#include <unistd.h>
#include <Loggable.hpp>
#include <Logger.hpp>
#include <LoggerFactory.hpp>
//#include "CppLogger.hpp"
//#include "SysLogger.hpp"
//#include "CLogger.hpp"
#include <LogManager.hpp>
#include <thread_safe_pointer.hpp>
#include <iostream>
#include <list>
//#include <cstdio>
using namespace std;

class HelloWorld : public Loggable {
  virtual string toString()const{ return "Hello World";}
};
int main(){
  //Get a local instance of the logger to add listeners to it.
  //Starts a new thread for the logger the first time this method is called.
  LogManager lmgr = LogManager::getInstance();
  //Setup argument list.
  list<thread_safe_ptr<Loggable> > logList = list<thread_safe_ptr<Loggable> >();
  //Add a loggable item to the list.
  logList.push_back( thread_safe_ptr<Loggable>(new HelloWorld()));
  //Register the listers with the logger singleton.
  lmgr.registerListener( LoggerFactory::getInstance(cout, ALL_MESSAGES_LR) );
  lmgr.registerListener( LoggerFactory::getInstance("demoApp"));
  lmgr.registerListener( LoggerFactory::getInstance(stdout) );
  //generate some log messages.
  for (unsigned int i = 0; i < 10; i++){
    lmgr.log(0,  "Here's a message from the logger!\n" );
    lmgr.log(0, logList);
    //usleep(500);
  }
  //kill the logger.(this will terminate the logging thread).
  lmgr.stopLogger();
  return 0;
}
