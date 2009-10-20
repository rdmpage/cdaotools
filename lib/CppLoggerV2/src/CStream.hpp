#ifndef C_STREAM_HPP_
#define C_STREAM_HPP_
#include "AbstractStream.hpp"
#include <cstdio>
/**
 * Basic C style stream wrapper.
 */
class COutputStream : public AbstractOutputStream {
public:
  virtual ~COutputStream(){ this->close();}
  /**
   * Create an output stream tied to the specified file stream.
   */
  COutputStream(FILE* handle):handle_(handle){}
  /**
   * Write preformatted data to the stream.
   */
  virtual void write(const std::string& data){ fprintf(handle_,"%s",data.c_str());  }
  virtual void close(){ fclose(handle_); return; }
private:
  FILE* handle_;
};

/**
 * This class provides a compatibility adaptor for c-style FILE*.
 */
class TypedCOutputStream : public COutputStream, public TypedOutputStream {
public:
  /**
   * Initialize the TypedCOutput stream with the specified file pointer.
   */
  TypedCOutputStream(FILE* out):COutputStream(out), TypedOutputStream(this){}
  virtual ~TypedCOutputStream(){}
  virtual void write(const std::string& data){ COutputStream::write( data  ); return; }
  virtual void close(){ COutputStream::close(); return;}
};

/**
 * CInputStream provides an adapter for C-style FILE* streams.
 */
class CInputStream : public TypedInputStream {
public:
  /**
   * Initialize the stream with the specified c-stream.
   */
  CInputStream(FILE* handle):TypedInputStream(this),handle_(handle){}
  virtual ~CInputStream(){}
  /**
   * Read up to the specified number of bytes from the stream.
   */
  virtual std::string read(size_t max_read_size){ 
    char* pRet = new char[ max_read_size ]; 
    fgets(pRet, max_read_size, handle_); 
    std::string ret = std::string(pRet);
    delete[] pRet;
    return ret;
  }
  virtual bool eof(){ return peek() == EOF;}
protected:
  /**
   * Return the next character in the stream without extracting it.
   */
  virtual int peek(){ char next = fgetc(handle_); ungetc(next, handle_); return next; }
  virtual int get(){ return fgetc(handle_); }
  /**
   * Extract a short from the stream.
   */
  virtual short readShort(){ short ret; return genericRead<short>("%d", ret); }
  /**
   * Extract an unsigned short from the stream.
   */
  virtual unsigned short readUShort(){ unsigned short ret;  return genericRead<unsigned short>("%ud", ret);}
  /**
   * Extract an int from the stream.
   */
  virtual int readInt(){ int ret; return genericRead<int>("%d", ret); }
  /**
   * Extract an unsigned int from the stream.
   */
  virtual unsigned int readUInt(){ unsigned int ret; return genericRead<unsigned int>("%ud", ret); }
  /**
   * Extract a long int from the stream.
   */
  virtual long readLong(){ long ret; return genericRead<long>("%ld", ret); }
  /**
   * Extract a unsigned long int from the stream.
   */
  virtual unsigned long readULong(){ unsigned long ret; return genericRead<unsigned long>("%uld", ret); }
  /**
   * Extract a long long int from the stream.
   */
  virtual long long readLLong(){ long long ret; return genericRead<long long>("%ld", ret); }
  /**
   * Extract a float from the stream.
   */
  virtual float readFloat(){ float ret; return genericRead<float>("%f", ret);}
  /**
   * Extract a double from the stream.
   */
  virtual double readDouble(){ double ret; return genericRead<double>("%g", ret); }
  /**
   * Extract a long from the stream.
   */
  virtual long double readLongDouble(){ long double ret; return genericRead<long double>("%g", ret); }
  /**
   * Extract a character from the stream.
   */
  virtual char readChar(){ char ret; return genericRead<char>("%c", ret); }
  /**
   * Extract a pointer from the stream.
   */
  virtual void* readVoidStar(){ void* ret; return genericRead<void*>("%p", ret);}
  /**
   * Extract a string from the stream.
   */
  virtual std::string readString();
private:
  template <typename T> T genericRead(const char* format, T& ret){ fscanf(handle_, format, &ret); return ret; }
  FILE* handle_;
};
#endif
