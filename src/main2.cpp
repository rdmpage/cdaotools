
/*
 * Brandon Chisham
 * Prototype to read nexus files and translate into the new ontology.
 */
#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <string>
#include <vector>
#include <data_representation.hpp>
#include <grammar/nexus/nexus_reader.hpp>
#include <codegen/constants.hpp>
#include <codegen/codegen.hpp>

#include <fstream>
#include <iostream>

#include <Logger.hpp>
#include <AbstractStream.hpp>
#include <LogManager.hpp>


//using namespace std;
using namespace CDAO;
//static const wstring INFILE_ARG  = L"-i";
//static const wstring OUTFILE_ARG = L"-o";
static const std::string COMPRESS_OUTPUT_ARG = "-z";
//static const wstring VERBOSE_1   = L"-v";
//static const wstring VERBOSE_2   = L"-vv";
//static const wstring VERBOSE_3   = L"-vvv";
//static const wstring VERBOSE_4   = L"-vvvv";
//static const wstring VERBOSE_5   = L"-vvvvv";

//TypedOutputStream tocerr = *StreamFactory::getOutputInstance( cerr );

/*
 * Process the argument list and setup the environment.
 */
void myprocessArgs(int argc, char** argv, char** env);

void doCompressedOutput( const std::string& inname, const std::string& outname );
void doNormalOutput( const std::string& inname, const std::string& outname );

inline bool isOption( const wchar_t* str){ return str ? *str == (wchar_t)'-' : false; }
/*
 * Initializes the logging system. 
 */
//void initLog(LogManager& lmgr, const wchar_t* out, const int level);
/*
 * Input and output streams.
 */
std::wistream* in;
std::wostream* out;

//Logger* logger;

std::vector< std::string > input_files;
std::vector< std::string > output_files;

bool compress_output = false;

int main(int argc, char** argv, char** env){
  //setup environment
  myprocessArgs( argc, argv, env );
  //Logger has been configured.
//  LogManager lmgr = LogManager::getInstance();
  unsigned long int file_no = 1;
  map_segment();  
  for (std::vector< std::string >::iterator i = input_files.begin(); i != input_files.end(); ++i, ++file_no ){
      in = new std::wifstream( (*i).c_str() );
      
      std::wcerr << L"Processing (" << file_no << L"/" << input_files.size() << L" : " << 100*(double)file_no/input_files.size() << L"%):" << CDAO::str_to_wstr( *i ) << std::endl;

      if (compress_output){ doCompressedOutput(*i, *i + ".cdao"); }
      else { doNormalOutput( *i, *i + ".cdao" ); }
                         
  }
  unmap_segment();
  return 0;

}

void myprocessArgs(int argc, char** argv, char** env){
 // cerr << L"processArgs( argc:"  << argc << L", argv:" << argv << L", env: L" << env << L")\n";
  //int default_log_level = NO_MESSAGES_LR;
  //logger = new CppLogger( cerr, default_log_level );
  in = &std::wcin;
  out = &std::wcout;
 
  GlobalState::setInfile( in );
  GlobalState::setOutfile( out );

  input_files = std::vector< std::string >();
  output_files = std::vector< std::string >();
  //LogManager lmgr = LogManager::getInstance();
  //GlobalState::setErrorfile( &cerr );
  for (int i = 1; i < argc  - 1; ++i){
    if ( argv[ i ] == INFILE_ARG){
	input_files.push_back(  std::string( argv[ i + 1 ] )  );
        //ifstream* inf = new ifstream( input_file.c_str() );
        
	//GlobalState::setInfile( in =  dynamic_cast<istream*>(inf) );
    }
    else if ( argv[ i ] == OUTFILE_ARG ){
	//ofstream* outf = new ofstream( argv[ i + 1 ]  );
      	//GlobalState::setOutfile( out =  dynamic_cast<ostream*>(outf) );
	//Codegen::Imports::setBaseURI( argv[ i + 1 ] );
        output_files.push_back(  std::string(argv[ i + 1 ])  );
    }
    else if ( argv[ i ] == COMPRESS_OUTPUT_ARG){
        compress_output = true;
    }
    //else if ( argv[ i ] == VERBOSE_1){
    //  initLog( lmgr, argv[ i + 1], ALERT_MESSAGES_LR ); 
    //}
    //else if ( argv[ i ] == VERBOSE_2){  
    //  initLog( lmgr, argv[ i + 1], CRITICAL_MESSAGES_LR ); 
    //}
    //else if ( argv[ i ] == VERBOSE_3){ 
    //  initLog( lmgr, argv[ i + 1], ERROR_MESSAGES_LR ); 
    //}
    //else if ( argv[ i ] == VERBOSE_4){
    //  initLog( lmgr, argv[ i + 1], INFO_MESSAGES_LR );
    //}
    //else if ( argv[ i ] == VERBOSE_5){ 
    //  initLog( lmgr, argv[ i + 1], ALL_MESSAGES_LR );
    //}
  }
 // cerr << L"exiting processArgs()\n";
  return;
}

//void initLog(LogManager& lmgr, const wchar_t* out, const int level){

//  if (isOption( out )){
    
//    lmgr.registerListener( LoggerFactory::getLogger( tocerr, level  ) );
 // }
 // else {
 //   ofstream logStream( out );
 //   lmgr.registerListener( LoggerFactory::getLogger( logStream, level  ) );
 // } 
//}

void doCompressedOutput( const std::string& inname, const std::string& outname ){
  std::wifstream* in;
  //ofstream* out;
  DataRepresentation* data;
  const unsigned int READ_END = 0;
  const unsigned int WRITE_END = 1;
  int pipe_descriptors[2];
  if (pipe(pipe_descriptors) > -1){
      pid_t child_pid = fork();
      if (0 == child_pid){
        /* child-code */
        if ( close(pipe_descriptors[ WRITE_END ]) > -1 ){
          if (dup2( pipe_descriptors[ READ_END ], STDIN_FILENO ) > -1){
            int outfiledes = open( (outname + ".gz").c_str(), O_CREAT|O_WRONLY|O_TRUNC,0644);
            if (outfiledes > -1){
              if (dup2( outfiledes, STDOUT_FILENO ) > -1){
                  close( outfiledes );
                  execlp("gzip","gzip", (char*)NULL);
              }
              else {
                perror("Unable to dup STDOUT_FILENO L");
                exit( 1 );
              }
            }
            else {
              perror(("unable to open " + outname + ".gz ").c_str());
              exit( 1 );
            }
          }
          else {
             perror("Unable to dup STDIN_FILENO L");
             exit( 1 );
          }

        }
        else {
          perror("Unable to close write end of pipe");
          exit( 1 );
        }
      }
      else {
        if (close( pipe_descriptors[ READ_END ]) > -1 ){
          if (dup2( pipe_descriptors[ WRITE_END ], STDOUT_FILENO) > -1){
            in = new wifstream( inname.c_str() );
            GlobalState::setInfile( in );
            GlobalState::setOutfile( &wcout );

            data = nexusparse();

            std::wcerr << L"Parsed: " << CDAO::str_to_wstr( inname ) << endl;

            data->setMatrixLabel(  CDAO::str_to_wstr( inname )  );
            CodeGenerator outputFormatter( data );

            outputFormatter.generate( wcout );
        
            std::wcerr << L"Wrote output" << endl;

            in->close();
            close( pipe_descriptors[ WRITE_END ] );
            close( STDOUT_FILENO );
            delete in;
            delete data;
            int status; 
            std::wcerr << L"Waiting for child to exit" << endl;
            waitpid(-1, &status, 0);
            std::wcerr << L"Child Exited" << endl;
          }
          else { perror( "Unable to dup STDOUT_FILENO" ); }
        }
        else { perror( "Unable to close read end of pipe" ); }
      }
  }
  else {
      perror("Unable to open pipe: L");
      exit( 1 );

  }
 // delete data;

}

void doNormalOutput( const string& inname, const string& outname ){

  std::wifstream* in;
  std::wofstream* out;
  DataRepresentation* data;

  in = new std::wifstream(  inname.c_str() );
  out = new std::wofstream(  outname.c_str() );

  GlobalState::setInfile( in );
  GlobalState::setOutfile( out );
  
  data = nexusparse();

  data->setMatrixLabel(  CDAO::str_to_wstr( inname ) );

  CodeGenerator outputFormatter( data );

  outputFormatter.generate( *out );

  in->close();
  out->close();
  delete in;
  delete out;
  delete data;


}

