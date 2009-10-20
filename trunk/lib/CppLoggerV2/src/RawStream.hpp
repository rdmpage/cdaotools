#ifndef RAW_STREAM_HPP_
#define RAW_STREAM_HPP_
#include <cstdlib>
#include "AbstractStream.hpp"
/**
 * Compatibility wrapper for RawOutputStream. g++ is unable to resolve write() from unistd.h if the call
 * is made directly from its write(std::string) method.
 */
size_t writeWrapper(int fdes, const void* dat, size_t count);/*{ write(fdes, dat, count  ); }*/
inline int closeWrapper(int fileDes){ return close(fileDes);}

inline ssize_t readWrapper(int fileDes, void* buff, size_t length){ return read(fileDes, buff, length);  }


/**
 * RawStream encapsulates a raw file descriptor. 
 */
class RawStream {
public: 
  /**
     Specifies an file descriptor that can not be encountered as valid in practice
   */
  static const int INVALID_DES = -1;
  /**
     Initialize the descriptor. 
   */
  RawStream(int des=INVALID_DES):file_descriptor_(des){}
  /**
     Release the stream on exit.
   */
  virtual ~RawStream(){ this->close(); }
  /**
     Retrieve the descriptor if needed. 
   */
  virtual int getDescriptor()const{ return file_descriptor_; }
  /**
     Resets the descriptor. Keeps the previous descriptor open (potential resource leak).
   */
  virtual void setDescriptor(int des){ file_descriptor_ = des; }
  /**
     True of the stream is open false otherwise.
   */
  virtual bool isOpen()const{ return file_descriptor_ != INVALID_DES; }
  /**
     Close the stream.
   */
  virtual void close(){ closeWrapper(file_descriptor_); file_descriptor_ = INVALID_DES; }
protected:
  int file_descriptor_;
};


/**
 * Wrapper for low-level IO operators.
 */
class RawOutputStream : public AbstractOutputStream, protected RawStream {
public:
  /**
   * Initialize the output stream with the specified *open* file descriptor.
   */
  RawOutputStream(int file_descriptor):RawStream(file_descriptor){}
  virtual ~RawOutputStream(){ this->close();}
  /**
   * Write preformatted data to the stream.
   */
  virtual void write(const std::string& data){ writeWrapper(file_descriptor_, data.c_str(), data.size()); }
  virtual void close(){ RawStream::close(); return;}
  //static const int INVALID_DES = -1;
protected:  
  //int file_descriptor_;
};

/**
 * Allows the use of stream inserter syntax with file descriptors.
 */
class TypedRawOutputStream: public RawOutputStream, public TypedOutputStream {
public:
  /**
   * Attach the stream to the specified file descriptor.
   */
  TypedRawOutputStream(int file_descriptor):RawOutputStream(file_descriptor), TypedOutputStream(this){}
  virtual ~TypedRawOutputStream(){}
  /**
   * Write the specified data to the stream.
   */
  virtual void write(const std::string& data){ RawOutputStream::write(data); return; }
  /**
   * Close the stream, and the underlying file descriptor.
   */
  virtual void close(){ RawOutputStream::close();  }
  virtual int getDescriptor(){return file_descriptor_;}
  virtual void setDescriptor(int des){ file_descriptor_ = des; return;}
};

/**
 * Type-safe extractors for file descriptors.
 */
class TypedRawInputStream : public TypedInputStream {
public:
  TypedRawInputStream(int file_descriptor):TypedInputStream(this),
					   /*RawStream(file_descriptor), */
					   buff(file_descriptor,DEFAULT_BUFFER_SIZE){ }
  TypedRawInputStream(int file_descriptor, size_t buff_size):TypedInputStream(this),
							     /*RawStream(file_descriptor),*/ 
							     buff(file_descriptor,buff_size){}
  TypedRawInputStream(const TypedRawInputStream& rhs):TypedInputStream(this),
						      /*RawStream(rhs.getDescriptor()),*/
						      buff(rhs.getDescriptor(),
							   rhs.buff.getSize()){}
  virtual ~TypedRawInputStream(){ }
  /**
   * Returns a copy of the next character in the stream without removing it.
   */
  int peek(){return buff.currentChar();}
  int get(){ return readChar(); }
  /**
   * True if the end of file has been reached, false otherwise.
   */
  bool eof(){ return buff.eof(); }
  /**
   * Default size for the internal buffer.
   */
  static const size_t DEFAULT_BUFFER_SIZE = 1024;
  virtual void close(){ buff.close();}
  //virtual int getDescriptor(){return file_descriptor_;}
  //virtual void setDescriptor(int des){ file_descriptor_ = des; return;}
protected:
  /*
    These constructors are protected to prevent their use by a client. 
    They would construct an object with an imcomplete unusuable state.
   */
  TypedRawInputStream():TypedInputStream(this),/*RawStream(INVALID_DES),*/
			buff(RawStream::INVALID_DES,0){}
  //TypedRawInputStream(const TypedRawInputStream& rhs):buff(STDIN_FILENO, 0),TypedInputStream(this){}
  TypedRawInputStream& operator=(const TypedRawInputStream& rhs){
    if (this == &rhs){ return *this;}
    else {
      this->buff = rhs.buff;
    }
    //return *this;
  }
  /**
   * Remove a character from the stream.
   */
  virtual char readChar(){return buff.extractChar();}
  /**
   * Remove a short from the stream.
   */
  virtual short readShort(){ return atoi( (buff.extractToken()).c_str() ); }
  /**
   * Remove an unsigned short from the stream
   */
  virtual unsigned short readUShort(){ return atoi( (buff.extractToken()).c_str() ); }
  /**
   * Remove an int from the stream
   */
  virtual int readInt(){ return atoi( (buff.extractToken()).c_str() ); }
  /**
   * Remove an unsigned int from the stream.
   */
  virtual unsigned int readUInt(){ return atoi( (buff.extractToken()).c_str() ); }
  /**
   * Extract a long from the stream.
   */
  virtual long readLong(){ return atol( (buff.extractToken()).c_str() ); }
  /**
   * Extract an unsigned long from the stream.
   */
  virtual unsigned long readULong(){  return atol( (buff.extractToken()).c_str() ); }
  /**
   * Extract a long long from the stream.
   */
  virtual long long readLLong(){  return atoll( (buff.extractToken()).c_str() ); }
  /**
   * Extract a float from the stream.
   */
  virtual float readFloat(){ return atof( (buff.extractToken()).c_str() ); }
  /**
   * Extract a double from the stream.
   */
  virtual double readDouble(){  return atof( (buff.extractToken()).c_str() ); }
  /**
   * Extract a long double from the stream.
   */
  virtual long double readLongDouble(){  return strtold( (buff.extractToken()).c_str(), NULL ); }
  /**
   * Extract a pointer from the stream.
   */
  virtual void* readVoidStar(){ return (void*)strtol( (buff.extractToken()).c_str(), NULL, 16 ); }
  /**
   * Extract a string from the stream.
   */
  virtual std::string readString(){ return buff.extractToken(); }
 private:
  int getDescriptor()const{ return buff.getDescriptor();}
  /**
   * Buffer handles actual reading (in blocks) from the file descriptor.
   */
  class Buffer : public RawStream {
  public:
    /**
     * Create a buffer with the specified source, and size.
     */
    Buffer(int file_descriptor, size_t size):RawStream(file_descriptor),
					     lastReadReturnValue_(EAGAIN),
					     buffSize_(size), 
					     currentPos_(0), 
					     endOfData_(0)
    { buff_ = new char[ buffSize_ ]; }
    /**
     * Free buffer space.
     */
    ~Buffer(){ delete[] buff_; }
    
    Buffer& operator=(const Buffer& rhs);
    
    /**
     * Returns the current character without removing it from the buffer.
     */
    char currentChar();
    /**
     * Returns the current character and removes it from the buffer.
     */
    char extractChar();
    /**
     * True if the buffer still contains some data.
     */
    bool dataAvailable()const{return currentPos_ < endOfData_; }
    /**
     * True if EOF has been encountered while reading from the descriptor.
     */
    bool eof()const{ return  lastReadReturnValue_ == 0;/*read returns 0 on EOF*/}
    /**
     * Removes a single token (Number or String) stripped of white-space.
     */
    std::string extractToken();
    const size_t getSize()const{ return buffSize_; }
    //void close(){ closeWrapper(file_descriptor_); file_descriptor_ = RawStream::INVALID_DES; }
  protected:
    /**
     * Refills the buffer after all pending characters have been extracted.
     */
    void readFromSource();
  private:
    /*
     * Source used for filling the buffer.
     */
    int file_descriptor_;
    /*
     * Used for EOF and error detection. 
     */
    ssize_t lastReadReturnValue_;
    /*
     * Storage for pending characters.
     */
    char* buff_;
    /*
     * Size of the buffer.
     */
    size_t buffSize_;
    /*
     * Points to the next character to be read.
     */
    size_t currentPos_;
    /*
     * Points to the character just past the end of the block that has been read.
     */
    size_t  endOfData_;
  };
  
  Buffer buff;
  //char putBackChar_;
  size_t BUFF_SIZE_;
  size_t currentPos_;
protected:
  //int file_descriptor_;
private:
};


#endif
