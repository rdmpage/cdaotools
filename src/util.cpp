#include "util.hpp"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <data_representation.hpp>
#include <grammar/nexus/nexus_reader.hpp>
#include <codegen/constants.hpp>
#include <codegen/codegen.hpp>

#include <fstream>
#include <iostream>

#include <Logger.hpp>
#include <AbstractStream.hpp>
#include <LogManager.hpp>

#define SHARED_HEAP_SIZE (1024 * 1024 * 128 )

using namespace std;
namespace CDAO {

  wistream* GlobalState::in_ = &std::wcin;
 // istream* GlobalState::narrow_in_ = &std::cin;
  wostream* GlobalState::out_ = &std::wcout;
//  ostream* GlobalState::narrow_out_ = &std::cout;
  wostream* GlobalState::err_ = &std::wcerr;
 // ostream* GlobalState::narrow_err_ = &std::cerr;
  bool GlobalState::interleaved_ = false;

static std::string input_file = "";
static std::string output_file = "";
static MmappedHeapManager* segment_;
static HeapManager* previous_default_;
//static int fd;
   /*
    * Input and output streams.
    */
//static istream* in;
//static ostream* out;
TypedOutputStream tocerr = *StreamFactory::getOutputInstance( cerr );


void setInputFile( const std::string& s){ input_file = s; }
void setOutputFile( const std::string& s){ output_file = s;  }

string getInputFile(){ return input_file; }
string getOutputFile(){ return output_file;}

//wistream* getInputStream(){ return GlobalState::getInfile(); }
//wostream* getOutputStream(){ return GlobalState::getOutfile(); }


wstring labelMaker(  wstring tag_type ){
	static int serial_number = 0;

	return tag_type + number_to_wstring( ++serial_number );
}

wstring number_to_wstring( int number ){
   
	const int NUM_SIZE = 13;
	const int NUM_ASIZE = NUM_SIZE -1;
 	char str_number[NUM_SIZE];
	snprintf( str_number, NUM_ASIZE, "%d", number );

	return str_to_wstr( string(str_number) );
}

void processArgs(int argc, char** argv, char** env){

  //wcerr << L"processArgs( argc: " << argc << ", argv: " << argv << ", env: " << env << ")\n";
  GlobalState::setInfile( &wcin );
  GlobalState::setOutfile( &wcout );
  GlobalState::setErrorfile( &wcerr ); 

  //wcerr << L"Set default stream pointers\n";

  //LogManager lmgr = LogManager::getInstance();
  for (int i = 1; i < argc  - 1; ++i){
    
    //wcerr << L"processing argument, " << i << L", value: " << str_to_wstr( argv[i] ) << endl;

    if ( argv[ i ] == INFILE_ARG){
	//input_file = argv[ i + 1 ];
                setInputFile( argv[i + 1 ] );
                //wcerr << L"Set input file: " << str_to_wstr(getInputFile()) << endl;

                wifstream* winf = new wifstream( getInputFile().c_str() );

                assert( NULL != winf );
  //      ifstream* inf = new ifstream( input_file.c_str() );
	        GlobalState::setInfile( winf );
      //  GlobalState::setNarrowInfile( inf );
    }
    else if ( argv[ i ] == OUTFILE_ARG ){
        setOutputFile(  argv[i + 1] );
        //wcerr << L"Set output file: " << str_to_wstr( getOutputFile() ) << endl;
	wofstream* woutf = new wofstream( getOutputFile().c_str()  );
        assert( NULL != woutf );
   //     ofstream* outf = new ofstream( argv[ i + 1 ] );
        //output_file = argv[ i + 1 ];
      	GlobalState::setOutfile( woutf );
   //     GlobalState::setNarrowOutfile( outf );
	Codegen::Imports::setBaseURI( str_to_wstr( string( argv[ i + 1 ] ) ) );
    }
    else if (argv[ i ] == INTERLEAVED_ARG){
        GlobalState::setInterleaved( true );
    }
    /*
    else if ( argv[ i ] == VERBOSE_1){
      //initLog( lmgr, argv[ i + 1], ALERT_MESSAGES_LR ); 
    }
    else if ( argv[ i ] == VERBOSE_2){  
      //initLog( lmgr, argv[ i + 1], CRITICAL_MESSAGES_LR ); 
    }
    else if ( argv[ i ] == VERBOSE_3){ 
      //initLog( lmgr, argv[ i + 1], ERROR_MESSAGES_LR ); 
    }
    else if ( argv[ i ] == VERBOSE_4){
      //initLog( lmgr, argv[ i + 1], INFO_MESSAGES_LR );
    }
    else if ( argv[ i ] == VERBOSE_5){ 
      //initLog( lmgr, argv[ i + 1], ALL_MESSAGES_LR );
    }*/
  }

  assert( GlobalState::getInfile() );
  assert( GlobalState::getOutfile() );

 // cerr << L"exiting processArgs()\n";
  return;
}

void initLog(LogManager& lmgr, const char* out, const int level){

  if (isOption( out )){
    
    lmgr.registerListener( LoggerFactory::getLogger( tocerr, level  ) );
  }
  else {
    ofstream logStream( out );
    lmgr.registerListener( LoggerFactory::getLogger( logStream, level  ) );
  } 
}

std::wstring str_to_wstr( const std::string& src){ 
    std::wstring ret = L"";
    //std::copy( src.begin(), src.end(), ret.begin() );
    for (std::string::const_iterator i = src.begin(); i != src.end(); ++i ){
        ret +=  std::cin.widen( *i );
    }
    return ret;
}
std::string wstr_to_str( const std::wstring& src ){
    string ret = "";
    //std::copy( src.begin(), src.end(), ret.begin() );
    for (std::wstring::const_iterator i = src.begin(); i != src.end(); ++i){
        ret += std::cin.narrow( *i, '\0' );
    }
    
    return ret;
}

void map_segment(){
    //void* ret = NULL;
    HeapManagerManager* shm  = HeapManagerManager::getInstance();
    segment_ = new MmappedHeapManager( SHARED_HEAP_SIZE );
    previous_default_ = shm->setDefault( segment_ );
    
    return;
}

void unmap_segment( ){
  //ShmHeap::shm_destroy( );
  HeapManagerManager* shm = HeapManagerManager::getInstance();
   shm->setDefault( previous_default_ );
   delete segment_;
   //close( fd );
} 

void* get_segment(){ return segment_; }

}


