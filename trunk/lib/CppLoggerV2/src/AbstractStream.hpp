#ifndef ABSTRACT_STREAM_HPP_
#define ABSTRACT_STREAM_HPP_
#include <string>
#include <cstdio>
//#include <fstream>
#include <unistd.h>
//#include <errno.h>
//#include <string.h>
//#include <iostream>
#include <errno.h>
#include <thread_safe_pointer.hpp>
/*
 * Author: Brandon Chisham
 * Created: May 26, 2006
 * Modified May 1, 2007
 * Copyright (c) 2006
 * See: COPYING for license details.
 */

/**
 * Provides a trivial base for all of the following wrapped IO classes.
 */
class AbstractStream{};
/**
 * Provides the base interface for all output adapters.
 */
class AbstractOutputStream : public AbstractStream {
public:
  virtual ~AbstractOutputStream(){}
  /**
   * Write the string data to the file handle associated with
   * the concrete instance.
   */
  virtual void write(const std::string& data)=0;
  virtual void close()=0;
  /**
   * Convert a bool into its string representation.
   */
  static std::string convert(const bool in){ return in?"1":"0"; }
  static std::string convert(const char in);
  /**
   * Convert an int into this ASCII representation.
   */
  static std::string convert(const int);
  /**
   * Convert into an ASCII representation.
   */
  static std::string convert(const unsigned int);
  /**
   * Convert into an ASCII representation.
   */
  static std::string convert(const long);
  /**
   * Convert into an ASCII representation.
   */
  static std::string convert(const unsigned long);
  static std::string convert(const long long);
  /**
   * Convert into an ASCII representation.
   */
  static std::string convert(const double);
  /**
   * Convert into an ASCII representation.
   */
  static std::string convert(const void*);
};

/**
 * Provides an interface allowing clients to use std::ostream style stream insert operators to write data.
 */
class TypedOutputStream : public AbstractOutputStream {
public:
  virtual ~TypedOutputStream(){}
  /**
   * Default implementation provided for compatibility with STL containers.
   */
  TypedOutputStream():instance_(this){}
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const bool& val){ instance_->write( AbstractOutputStream::convert(val) ); return *this; }
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const short& val){ instance_->write( AbstractOutputStream::convert(val)  ); return *this; }
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const unsigned short& val){ instance_->write( AbstractOutputStream::convert(val)  ); return *this; }
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const int& val){instance_->write( AbstractOutputStream::convert(val)  ); return *this; }
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const unsigned int& val){ instance_->write( AbstractOutputStream::convert(val) ); return *this; }
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const long& val){ instance_->write( AbstractOutputStream::convert(val) ); return *this; }
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const unsigned long& val){ instance_->write( AbstractOutputStream::convert(val) ); return *this; }
  TypedOutputStream& operator<<(const long long& val){ instance_->write( AbstractOutputStream::convert(val) ); return *this; }
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const double& val){ instance_->write( AbstractOutputStream::convert(val) ); return *this; }
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const void* val){ instance_->write( AbstractOutputStream::convert(val) ); return *this; }
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const std::string& val){ instance_->write( val ); return *this;}
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const char* val){ instance_->write( std::string(val) );  return *this;}
  /**
   * Write the specified value to the specified stream. The library will handle conversion to ASCII format.
   */
  TypedOutputStream& operator<<(const char& val){ instance_->write( AbstractOutputStream::convert(val) ); return *this; }
  /**
   * Write the specified data using the concrete implementation.
   */
  void write(const std::string& in){ instance_->write(in); return; }
  /**
   * Close the stream.
   */
  void close(){instance_->close(); return;}
protected:
  /**
   * Set pointer to concrete instance.  
   */
  TypedOutputStream(TypedOutputStream* inst):instance_(inst){}
private:
  /**
   * Pointer to the concrete instance of the interface.
   */
  TypedOutputStream* instance_;
};


/**
 * AbstractInputStream provides a common base analogous
 * to AbstactOutputStream to unify standard input libraries.
 */
class AbstractInputStream : public AbstractStream {
public:
  virtual ~AbstractInputStream(){}
  /**
   * Clients must provide a an implementation for reading from a stream.
   */
  virtual std::string read(size_t max_read_size)=0;
  /**
   * Retrieves the next character from the stream without extracting it.
   */
  virtual int peek()=0;
  virtual int get() = 0;
  virtual void close()=0;
};
/**
 * Allows extraction of builtin datatypes from the stream.
 * Note: If this is being used over a network the size of some data types
 *       may differ on separate machines. The library as of yet does not 
 *       guard against this!
 */
class TypedInputStream : public AbstractInputStream {
public:
  /**
   * Initializes the input stream with a pointer to the specified concrete implemenation.
   */
  TypedInputStream(TypedInputStream* instance):instance_(instance){}
  virtual ~TypedInputStream(){ /*this->close(); */}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual std::string read(size_t max_read_size){ return instance_->read(max_read_size);}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual int peek(){return instance_->peek();}
  virtual int get(){ return instance_->get(); }
  virtual bool eof(){ return instance_->eof();}
  virtual void close(){ instance_->close();}
  /**
   * Use the concrete instance to read the referenced value.
   */
  TypedInputStream& operator>>(bool& val){ val = instance_->readBool(); return *this; }
  /**
   * Use the concrete instance to read the referenced value.
   */
  TypedInputStream& operator>>(short& val){ val = instance_->readShort(); return *this;}
  /**
   * Use the concrete instance to read the referenced value.
   */
  TypedInputStream& operator>>(unsigned short& val){ val = instance_->readUShort(); return *this; }
  /**
   * Use the concrete instance to read the referenced value.
   */
  TypedInputStream& operator>>(int& val){ val = instance_->readInt(); return *this; }
  /**
   * Use the concrete instance to read the referenced value.
   */
  TypedInputStream& operator>>(unsigned int& val){ val = instance_->readUInt(); return *this; }
  /**
   * Use the concrete instance to read the referenced value.
   */
  TypedInputStream& operator>>(long& val){ val = instance_->readLong(); return *this; }
  TypedInputStream& operator>>(long long& val){ val = instance_->readLong(); return *this; }
  /**
   * Use the concrete instance to read the referenced value.
   */
  TypedInputStream& operator>>(unsigned long& val){ val = instance_->readULong(); return *this; }
  /**
   * Use the concrete instance to read the referenced value.
   */
  TypedInputStream& operator>>(float& val){ val = instance_->readFloat(); return *this; }
  /**
   * Use the concrete instance to read the referenced value.
   */
  TypedInputStream& operator>>(double& val){ val = instance_->readDouble(); return *this;}
  /**
   * Use the concrete instance to read the referenced value.
   */
  TypedInputStream& operator>>(long double& val){ val = instance_->readLongDouble(); return *this; }
  /**
   * Use the concrete instance to read the referenced value.
   */
  TypedInputStream& operator>>(void* val){ val = instance_->readVoidStar(); return *this; }
  
  TypedInputStream& operator>>(std::string& val){ val = instance_->readString(); return *this;  }
  //TypedInputStream& operator>>(char*& val){ val = instance_->readString(); return *this;  }
protected:
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual bool readBool(){return false;}
  /** 
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual short readShort(){ return 0;}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual unsigned short readUShort(){return 0;}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual int readInt(){return 0;}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual unsigned int readUInt(){return 0;}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual long readLong(){return 0;}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual unsigned long readULong(){return 0;}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual long long readLLong(){return 0;}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual float readFloat(){ return 0; }
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual double readDouble(){return 0;}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual long double readLongDouble(){return 0;}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual char readChar(){return '\0';}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual void* readVoidStar(){ return NULL;}
  /**
   * Provides a dummy implementation for this pseudo-abstract class. Each concrete implemenation
   * must over-ride this method.
   */
  virtual std::string readString(){return "";}
private:
  /*
   * Pointer to the concrete instance. 
   * This member is used to simulate virtual method
   * behavior with stack based instances of the class.
   */
  TypedInputStream* instance_;
};



class StreamFactory {
public:
  static thread_safe_ptr<TypedOutputStream> getOutputInstance(std::ostream&);
  static thread_safe_ptr<TypedOutputStream> getOutputInstance(FILE*);
  static thread_safe_ptr<TypedOutputStream> getOutputInstance(int);
  static thread_safe_ptr<TypedInputStream> getInputInstance(std::istream&);
  static thread_safe_ptr<TypedInputStream> getInputInstance(FILE*);
  static thread_safe_ptr<TypedInputStream> getInputInstance(int);
};

#endif
