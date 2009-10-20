#include "RawStream.hpp"
#include <assert.h>

size_t writeWrapper(int fdes, const void* dat, size_t count) {return write(fdes, dat, count); fsync(fdes);}

/*
 * Returns the current character from the buffer
 * without moving the currentCharacter pointer. 
 * If the buffer is empty another block read using
 * the file descriptor is attempted. 
 */
char TypedRawInputStream::Buffer::currentChar(){
  //see if the buffer is empty
  if (dataAvailable()){
    return buff_[currentPos_];
  }
  else {
    //fill the buffer
    readFromSource();
    if (!eof()){ 
      return buff_[currentPos_]; 
    }
    else{ return '\0'; }
  }
}

/*
 * Extract a character from the buffer, and move
 * the currentCharacter pointer.
 */
char TypedRawInputStream::Buffer::extractChar(){
  char ret = currentChar();
  currentPos_++;
  return ret;
}
/*
 * Extracts a contiguous block of non-whitespace characters from
 * the buffer.
 */
std::string TypedRawInputStream::Buffer::extractToken(){
  std::string ret = "";
  char current;
  while(!eof() && isspace(currentChar())){extractChar();} //remove leading space
  while(!(eof() || isspace(currentChar()))){ 
    current = extractChar(); 
    ret += current; 
  }
  return ret;
}
/*
 * Reads a new block from the file descriptor.
 */
void TypedRawInputStream::Buffer::readFromSource(){
  currentPos_ = 0;
  endOfData_ = 0;
  assert (RawStream::getDescriptor() != RawStream::INVALID_DES);
  lastReadReturnValue_ = readWrapper(RawStream::getDescriptor(), buff_, buffSize_);
  if (!(lastReadReturnValue_ == EAGAIN || lastReadReturnValue_ == EBADF)){
    endOfData_ = lastReadReturnValue_;
  } 
  return;
}


TypedRawInputStream::Buffer& TypedRawInputStream::Buffer::operator=(const Buffer& rhs){
  if (this != &rhs){
    delete [] buff_;
    file_descriptor_ = rhs.file_descriptor_;
    buff_ = new char[ rhs.buffSize_ ];
    currentPos_ = rhs.currentPos_;
    endOfData_ = rhs.endOfData_;
    lastReadReturnValue_ = rhs.lastReadReturnValue_;
  }
  return *this;
}
