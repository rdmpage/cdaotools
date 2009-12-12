#ifndef NEXUS_READER_HPP_
#define NEXUS_READER_HPP_


#include <ncl/ncl.h>
#include <iostream>
#include <fstream>
#include <data_representation.hpp>
//#include <Logger.hpp>
//#include <LogManager.hpp>


namespace CDAO {

class DataRepresentation;

  
  class OntNexusReader : public NxsReader, public NxsBlock {
  public:
    
    OntNexusReader(istream& in, ostream& out, ostream&  err):inf(in), outf(out), errf(err){ }
    //NexusReader(ifstream& infile, ofstream& outfile):inf(infile), outf(outfile){}
    void Execute(NxsToken& tok, bool flag){ NxsReader::Execute(tok, flag); return; }
    void Execute(NxsToken& tok){ NxsReader::Execute(tok, true); }
    bool EnteringBlock(NxsString blockName){ /*outf <<"Entering: " << blockName << endl;*/  return true;}
    void SkippingBlock(NxsString blockName){ /*outf << "Leaving: " << blockName << endl;*/ return;}
    void NexusError(NxsString msg, file_pos pos, long line, long col){
     // LogManager lmgr = LogManager::getInstance();
     // lmgr.startMultiPartMessage( ALL_MESSAGES_LR  );
     // lmgr << "Pos:"<< pos << " line:" << line << " col: " << col << " " << (std::string) msg << "\n";
     // lmgr.endMultiPartMessage( );
      return;
    }
    istream& inf;
    ostream& outf;
    ostream& errf;
  };
  
  DataRepresentation*  nexusparse( void );
}
#endif
