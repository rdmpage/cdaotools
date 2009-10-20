#ifndef CPP_STREAM_HPP_
#define CPP_STREAM_HPP_
#include "AbstractStream.hpp"
#include <fstream>

/**
 * Template for TypedCppInputStream.
 */
template <typename T> T genericRead(std::istream& handle_, T& ret){  handle_ >> ret; return ret; }
/**
 * Basic ostream wrapper. 
 */
class CppOutputStream : public AbstractOutputStream {
public:
  /**
   * Creates a basic CppOutput stream. This class is a light-weight wrapper
   * around std::ostream, but the client is responsible for converting all types into
   * string form before calling.
   */
  CppOutputStream(std::ostream& handle):handle_(handle){}
  virtual ~CppOutputStream(){ this->close();}
  /**
   * Write the client formatted string data to the wrapped std::ostream.
   */
  virtual void write(const std::string& data){    handle_ << data;  }
  /**
   * Close the output stream. No further can be written after it is close.
   */
  virtual void close(){ (static_cast<std::ofstream&>(handle_)).close();  return;}
  /**
   * Returns an ostream reference tied to the same file as this OutputStream
   */
  std::ostream& getOstream(){return handle_;}
private:
  std::ostream& handle_;
};


/**
 * Provides adaptor for C++ ostream operations. 
 */
class TypedCppOutputStream : public CppOutputStream, public  TypedOutputStream {
public:
  TypedCppOutputStream(std::ostream& out):CppOutputStream(out), TypedOutputStream(this){}
  virtual ~TypedCppOutputStream(){}
  void write(const std::string& dat){ CppOutputStream::write(dat); }
  void close(){ CppOutputStream::close(); return;  }
};

/**
 * Provides an adapter for Cpp-style IOStreams.
 */
class CppInputStream : public TypedInputStream {
public:
  /**
   * Initializes the input stream with the specified istream. 
   */
  CppInputStream(std::istream& handle):TypedInputStream(this),handle_(handle){}
  /**
   * Reads raw input up to the specified number of bytes from the input stream.
   */
  virtual std::string read( size_t max_read_size ){  
    char* buff = new char[ max_read_size ];
    handle_.read( buff, max_read_size );
    std::string ret = std::string( buff );
    delete[] buff;
    return ret;
  }
  virtual bool eof(){ return handle_.eof(); }
protected:
  /**
   * Return the next character in the stream without extracting it.
   */
  virtual int peek(){ return handle_.peek(); }
  virtual int get(){ return handle_.get(); }
  /**
   * Extract a short from the stream.
   */
  virtual short readShort(){ short ret; return genericRead(handle_,ret);}
  /**
   * Extract an unsigned short from the stream.
   */
  virtual unsigned short readUShort(){ unsigned short ret; return genericRead(handle_, ret);}
  /**
   * Extract an int from the stream.
   */
  virtual int readInt(){ int ret; return genericRead(handle_, ret); }
  /**
   * Extract an unsigned int from the stream.
   */
  virtual unsigned int readUInt(){ unsigned int ret; return genericRead(handle_, ret); }
  /**
   * Extract a long int from the stream.
   */
  virtual long readLong(){ long ret; return genericRead(handle_,ret); }
  /**
   * Extract a unsigned long int from the stream.
   */
  virtual unsigned long readULong(){ unsigned long ret; return genericRead(handle_, ret); }
  /**
   * Extract a long long int from the stream.
   */
  virtual long long readLLong(){ long long ret;  return genericRead(handle_, ret); }
  /**
   * Extract a float from the stream.
   */
  virtual float readFloat(){ float ret; return genericRead(handle_, ret); }
  /**
   * Extract a double from the stream.
   */
  virtual double readDouble(){ double ret; return genericRead(handle_, ret);}
  /**
   * Extract a long from the stream.
   */
  virtual long double readLongDouble(){ long double ret;  return genericRead(handle_, ret);}
  /**
   * Extract a character from the stream.
   */
  virtual char readChar(){ char ret; return genericRead(handle_, ret); }
  /**
   * Extract a pointer from the stream.
   */
  virtual void* readVoidStar(){ void* ret;  return genericRead(handle_, ret); }
  /**
   * Extract a string from the stream.
   */
  virtual std::string readString(){std::string ret; return genericRead(handle_, ret); }
private:
  std::istream& handle_;
};


#endif
