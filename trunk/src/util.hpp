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
  static void setInfile( std::wistream* infile){ in_ = infile;  }
//  static void setNarrowInfile( std::istream* i ){narrow_in_ = i; }
  static void setOutfile( std::wostream* outfile){ out_ = outfile; }
//  static void setNarrowOutfile( std::ostream* o ){ narrow_out_ = o; }
  static void setErrorfile( std::wostream* errorfile ){ err_ = errorfile; }
//  static void setNarrowErrorfile( std::ostream* e ){ narrow_err_ = e;  }
  static void setInterleaved( bool i ){ interleaved_ = i; }
  static std::wistream* getInfile(){return in_; }
//  static std::istream* getNarrowInfile(){ return narrow_in_; }
  static std::wostream* getOutfile(){ return out_;}
//  static std::ostream* getNarrowOutfile(){ return narrow_out_; }
  static std::wostream* getErrorfile(){ return err_; }
//  static std::ostream* getNarrowErrorfile(){ return narrow_err_; }
  static bool isInterleaved(){ return interleaved_; }
private:
  static std::wistream* in_;
 // static std::istream* narrow_in_;
  static std::wostream* out_;
 // static std::ostream* narrow_out_;
  static std::wostream* err_;
 // static std::ostream* narrow_err_;
  static bool interleaved_;
  
};

  std::wstring str_to_wstr( const std::string& src);
  std::string wstr_to_str( const std::wstring& src );


  /*
   * Generate a document-wide unique identifier
   */
  std::wstring labelMaker( std::wstring tag_type );
  /*
   * Convert numbers to wstrings.
   */
  std::wstring number_to_wstring( int number );

  static const std::string INFILE_ARG  =  "-i";
  static const std::string OUTFILE_ARG =  "-o";
  static const std::string INTERLEAVED_ARG = "-il";
  static const std::string VERBOSE_1   =  "-v";
  static const std::string VERBOSE_2   =  "-vv";
  static const std::string VERBOSE_3   =  "-vvv";
  static const std::string VERBOSE_4   =  "-vvvv";
  static const std::string VERBOSE_5   =  "-vvvvv";

  /*
   * Process the argument list and setup the environment.
   */
   void processArgs(int argc, char** argv, char** env);

   inline bool isOption( const char* str){ return str ? *str == '-' : false; }
   
  /*
   * Initializes the logging system. 
   */
   void initLog(LogManager& lmgr, const char* out, const int level);
   void setInputFile( const std::string& inputfile );
   void setOutputFile( const std::string& outputfile );
   std::string getInputFile();
   std::string getOutputFile();

   //std::istream* getInputStream();
   //std::wostream* getOutputStream();
   void map_segment();
   void unmap_segment( );

  void* get_segment();
  
}
//#ifdef SHM_NODE
    void* operator new( size_t );
    void operator delete(void* trash);
  //#endif

#endif
