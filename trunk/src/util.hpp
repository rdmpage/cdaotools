#ifndef UTIL_HPP_
#define UTIL_HPP_
#include <string>

#include <Logger.hpp>
#include <AbstractStream.hpp>
#include <LogManager.hpp>



namespace CDAO {

class GlobalState {
public:
  //static init(std::istream* in=&std::cin, std::istream* out=&std::cout):in_(in), out_(out){}
  static void setInfile( std::istream* infile){ in_ = infile;  }
  static void setOutfile( std::ostream* outfile){ out_ = outfile; }
  static void setErrorfile( std::ostream* errorfile ){ err_ = errorfile; }
  static void setInterleaved( bool i ){ interleaved_ = i; }
  static std::istream* getInfile(){return in_; }
  static std::ostream* getOutfile(){ return out_;}
  static std::ostream* getErrorfile(){ return err_; }
  static bool isInterleaved(){ return interleaved_; }
private:
  static std::istream* in_;
  static std::ostream* out_;
  static std::ostream* err_;
  static bool interleaved_;
  
};



  /*
   * Generate a document-wide unique identifier
   */
  std::string labelMaker( std::string tag_type );
  /*
   * Convert numbers to strings.
   */
  std::string number_to_string( int number );

  static const std::string INFILE_ARG  = "-i";
  static const std::string OUTFILE_ARG = "-o";
  static const std::string INTERLEAVED_ARG = "-il";
  static const std::string VERBOSE_1   = "-v";
  static const std::string VERBOSE_2   = "-vv";
  static const std::string VERBOSE_3   = "-vvv";
  static const std::string VERBOSE_4   = "-vvvv";
  static const std::string VERBOSE_5   = "-vvvvv";

  /*
   * Process the argument list and setup the environment.
   */
   void processArgs(int argc, char** argv, char** env);

   inline bool isOption( const char* str){ return str ? *str == '-' : false; }
   
  /*
   * Initializes the logging system. 
   */
   void initLog(LogManager& lmgr, const char* out, const int level);
   
   std::string getInputFile();
   std::string getOutputFile();

   std::istream* getInputStream();
   std::ostream* getOutputStream();
   
}
#endif
