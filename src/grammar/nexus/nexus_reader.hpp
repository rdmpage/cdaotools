#ifndef NEXUS_READER_HPP_
#define NEXUS_READER_HPP_


#include <ncl/ncl.h>
#include <iostream>
#include <fstream>
#include <data_representation.hpp>
//#include <Logger.hpp>
//#include <LogManager.hpp>


namespace CDAO {

//class DataRepresentation;
  /*
   * Concrete representation for the nexus data format.
   */
  class NexusDataRepresentation : public DataRepresentation {
  public:
    /*
     * Builds a nexus data representation.
     */
    NexusDataRepresentation(std::vector<const Node* > parse_tree, 
			    NxsTaxaBlock* taxa, 
			    NxsTreesBlock* trees, 
			    NxsAssumptionsBlock* assumptions,
			    NxsCharactersBlock* characters, 
			    NxsDataBlock* data,
			    NxsDistancesBlock* distances):parse_tree_( parse_tree ),
							  taxa_( taxa ),
							  trees_( trees ),
							  assumptions_( assumptions ),
							  characters_( characters ),
							  data_( data ),
							  distances_( distances ){}
    /*
     * Tear down the nexus data representation.
     */
    virtual ~NexusDataRepresentation();
    /*
     * Retrieve the parse tree. 
     */
    virtual const Node* getParseTree(const unsigned int i = 0)const{ return parse_tree_.at( i ); }
    /*
     * Retrieve the taxon label
     */
    virtual const std::string getTaxonLabel( const unsigned int i)const{ 
      //assert( taxa_ ); 
      //string label;
      //try { label =  return label; }
      //catch (NxsX_NoSuchTaxon e){
      //	cerr << "No Taxon: " << e.getContents() << "\n";
      //}
      return taxa_ && !taxa_->IsEmpty() ? taxa_->GetTaxonLabel( i ) : "";
    }
    virtual const std::string  getTreeLabel( const unsigned int i)const{
      //assert(  trees_  );
      return trees_ && !trees_->IsEmpty() && i < trees_->GetNumTrees() ? trees_->GetTreeName( i ) : "" ;;
    }

    virtual const std::string getMatrixLabel()const{ return matrix_label_; }
    virtual void setMatrixLabel( const std::string& label){ matrix_label_ = label;}

    /*
     * Get the state of a particular trait for a particular taxon.
     */
    virtual const char getTraitState( const unsigned int taxon, const unsigned int trait)const;
    /*
     * True if all taxa have the same state for the given trait.
     */
    virtual const bool  isTrait( const unsigned int character)const{ return characters_ && !characters_->IsEmpty() ? characters_->GetObsNumStates( character ) == 1 : false;  }
    /*
     * Returns the number of taxa in the set.
     */
    virtual const unsigned int getNTax()const{ return taxa_ && !taxa_->IsEmpty() ? taxa_->GetNumTaxonLabels() : 0; }
    /*
     * Returns the number of characters/traits in the set.
     */
    virtual const unsigned int getNTraits()const{ return  characters_ &&  !characters_->IsEmpty() ? characters_->GetNumMatrixCols() : 0; }
    /*
     * Returns the datatype of the set.
     */
    virtual const unsigned int getDataType()const{ return characters_ && !characters_->IsEmpty() ? characters_->GetDataType() : 0 ;  }
    /*
     * True if some taxon has a gap for the specified trait.
     */
    virtual const bool hasGap( const unsigned int trait)const;
    /*
     * True of the specified taxon and traic is in a gap state.
     */
    virtual const bool isGap( const unsigned int taxon, const unsigned int trait)const{ 
      return characters_ && !characters_->IsEmpty() && taxon < this->getNTax() && trait < this->getNTraits()?
          CDAO::NXS_GAP == characters_->GetInternalRepresentation( taxon, trait, 0) : false;  
    }
    /*
     * Finds the node associated with the specifed taxon number. 
     */
    virtual const Node* findNode( const unsigned int taxon, const unsigned int tree  )const;
    /*
     * Finds the taxon number associated with the specified label.
     */
    virtual const unsigned int getTaxonNumber( const string& label)const{ 
      int ret = NO_TAXON;
      if ( taxa_ && !taxa_->IsEmpty() ){ 
        try {
	  ret = taxa_->FindTaxon( NxsString(label.c_str()) ); 
	  return ret;
        } catch (NxsTaxaBlock::NxsX_NoSuchTaxon e){
	  //cerr << "No taxon: " << label << "\n";
	  //return NO_TAXON;
	//exit ( 1 );
        }
       }
      return ret;
    }
    virtual const bool isGap( const char ch )const{ return !characters_ || characters_->IsEmpty() ? false : characters_->GetGapSymbol() == ch; }
    
    virtual const bool isMissing( const char ch )const{ return !characters_ || characters_->IsEmpty() ? false : characters_->GetMissingSymbol() == ch; }

    virtual const unsigned int getNumTrees()const{ return trees_ && !trees_->IsEmpty() ? trees_->GetNumTrees() : 0; }
  private:
    const Node* findNode(const string& key, const Node* current )const;
    std::string matrix_label_;
    std::vector<const Node* > parse_tree_;
    NxsTaxaBlock* taxa_;
    NxsTreesBlock* trees_;
    NxsAssumptionsBlock* assumptions_;
    NxsCharactersBlock* characters_;
    NxsDataBlock* data_;
    NxsDistancesBlock* distances_;
    
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
