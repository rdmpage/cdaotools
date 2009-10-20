#ifndef THREAD_SAFE_PTR_
#define THREAD_SAFE_PTR_
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <assert.h>
//#include "safe_pointer.hpp"
class OtherRefsError {
public:
  OtherRefsError(){}
};

/**
 * @author Brandon Chisham
 * Created: May, 14, 2006<br>
 * Revised: April, 17, 2007<br>
 * safe_ptr is an implementation of a reference counted pointers.
 * All instances should be declared as stack objects!
 */
template <typename T> class thread_safe_ptr  {
public:
  thread_safe_ptr(){
    sem_init(refs_, 0, 1);
    pthread_mutex_init(lock_, NULL);
    pthread_mutex_lock(lock_);
    this->value_ = NULL;
    pthread_mutex_unlock(lock_);
  }
  /**
   * Creates safe_ptr pointing to the specified value.
   */
  thread_safe_ptr(T* val){
    lock_ = new pthread_mutex_t;
    refs_ = new sem_t;
    sem_init(refs_, 0, 1);
    pthread_mutex_init(lock_, NULL);
    pthread_mutex_lock(lock_);
    this->value_ = val;
    pthread_mutex_unlock(lock_);
  }
  /**
   * Copies the safe_ptr for the specifed function call incrementing
   * the local count as well. 
   */
  thread_safe_ptr(const thread_safe_ptr& rhs){
    this->lock_ = rhs.lock_;
    this->refs_ = rhs.refs_;
    sem_post(this->refs_);
    pthread_mutex_lock(this->lock_);
    this->value_ = rhs.value_;
    pthread_mutex_unlock(this->lock_); 
  }
  /**
   * Copies the safe_ptr for the specifed function call incrementing
   * the local count as well. 
   */
  const thread_safe_ptr& operator=(const thread_safe_ptr& rhs){
    if (this == &rhs){ return *this;}
    else {
      pthread_mutex_t* oLock = this->lock_;
      bool oLockDestroyed = false;
      pthread_mutex_lock(oLock);
      pthread_mutex_lock(rhs.lock_);
      sem_post(rhs.refs_);
      if (sem_trywait(this->refs_)==EAGAIN){
	//this is the last reference to this pointer.
	pthread_mutex_unlock(oLock);
	delete this->value_;
	pthread_mutex_destroy(oLock);
	delete oLock;
	sem_destroy(this->refs_);
	delete this->refs_;
	oLockDestroyed = true;
      } 
      this->refs_ = rhs.refs_;
      this->lock_ = rhs.lock_;
     
      delete this->value_;
      this->value_ = rhs.value_;
      pthread_mutex_unlock(rhs.lock_);
      if (!oLockDestroyed){ pthread_mutex_unlock(oLock); }
      //pthread_mutex_destroy(oLock);
      return *this;
    }
  }
  /**
   * 
   */
  bool operator==(const thread_safe_ptr& rhs)const{
    return &this->refs_ == &rhs.refs_ ;
  }
  /**
   * Automatically deletes the wrapped pointer if all references have
   * been released. Otherwise does nothing.
   */
  ~thread_safe_ptr(){
    if (EAGAIN == sem_trywait(refs_)){
      //there aren't any more references to this pointer.
      delete value_;
      value_ = NULL;
      pthread_mutex_destroy(lock_);
      sem_destroy(refs_);
      delete lock_;
      delete refs_;
      lock_ = NULL;
      refs_ = NULL;
    }
  }
  /**
   * Allows the safe_ptr to use the traditional derefrence syntax.
   */
   T* operator->()const{ return value_; }
   T operator*()const{ return *value_; }
private:
  sem_t* refs_;
  pthread_mutex_t* lock_;
  T* value_;
protected:
  thread_safe_ptr* operatornew(){ return NULL; }
  void operatordelete(){ return; }
};

#endif
