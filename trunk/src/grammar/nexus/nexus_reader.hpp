#ifndef NEXUS_READER_HPP_
#define NEXUS_READER_HPP_


#include <ncl.h>
#include <iostream>
#include <fstream>
#include <data_representation.hpp>
#include <Logger.hpp>
#include <LogManager.hpp>


namespace CDAO {

class DataRepresentation;

class NexusState {
public:
  //static init(std::istream* in=&std::cin, std::istream* out=&std::cout):in_(in), out_(out){}
  static void setInfile( std::istream* infile){ in_ = infile;  }
  static void setOutfile( std::ostream* outfile){ out_ = outfile; }
  static void setErrorfile( std::ostream* errorfile ){ err_ = errorfile; }
  static std::istream* getInfile(){return in_; } 
  static std::ostream* getOutfile(){ return out_;}
  static std::ostream* getErrorfile(){ return err_; }
private:
  static std::istream* in_;
  static std::ostream* out_;
  static std::ostream* err_;
  
};
  
  class OntNexusReader : public NxsReader, public NxsBlock {
  public:
    
    OntNexusReader(istream& in, ostream& out, ostream&  err):inf(in), outf(out), errf(err){ }
    //NexusReader(ifstream& infile, ofstream& outfile):inf(infile), outf(outfile){}
    void Execute(NxsToken& tok, bool flag){ NxsReader::Execute(tok, flag); return; }
    void Execute(NxsToken& tok){ NxsReader::Execute(tok, true); }
    bool EnteringBlock(NxsString blockName){ /*outf <<"Entering: " << blockName << endl;*/  return true;}
    void SkippingBlock(NxsString blockName){ /*outf << "Leaving: " << blockName << endl;*/ return;}
    void NexusError(NxsString msg, file_pos pos, long line, long col){
      LogManager lmgr = LogManager::getInstance();
      lmgr.startMultiPartMessage( ALL_MESSAGES_LR  );
      lmgr << "Pos:"<< pos << " line:" << line << " col: " << col << " " << (std::string) msg << "\n";
      lmgr.endMultiPartMessage( );
      return;
    }
    istream& inf;
    ostream& outf;
    ostream& errf;
  };
  
  DataRepresentation*  nexusparse( void );
}
#endif
