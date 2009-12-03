#ifndef PHYLIP_TRANSLATOR_HPP_
#define PHYLIP_TRANSLATOR_HPP_
#include <vector>
#include <string>
#include <data_representation.hpp>
#include <Logger.hpp>
#include <LogManager.hpp>

namespace CDAO {
  
  class PhylipEnvironment {
    public:
      PhylipEnvironment(istream& in, bool intl ):in_(in), interleaved_(intl){}
      ~PhylipEnvironment(){}
      istream& getinput(){ return in_; }
      bool isInterleaved()const{ return interleaved_; }
    private:
      istream& in_;
      bool interleaved_;
  };

  class PhylipDataRepresentation : public DataRepresentation {
     public:
       PhylipDataRepresentation( std::vector< std::string > data, std::vector< std::string > labels );
       ~PhylipDataRepresentation();
       const unsigned getNTraits()const{ return matrix_data_.size(); }
       const unsigned getNTax()const{ return matrix_data_.at(0).size(); }
       const Node* getParseTree( unsigned int )const{ return NULL; }
       const std::string getTaxonLabel( unsigned int tax )const{ return taxon_labels_.at( tax ); }
       const std::string getTreeLabel(unsigned int ){ return ""; }
       const std::string getMatrixLabel()const{ return matrix_label_; }
       void setMatrixLabel( const std::string& ml ){ matrix_label_ = ml; }
       const char getTraitState( unsigned int tax, unsigned int trait )const{ return matrix_data_.at( tax ).at( trait ); }
       const bool isTrait( unsigned int )const{ return false; }
       void setDataType(unsigned int dtype){ data_type_ = dtype;  }
       const unsigned int getDataType()const{ return data_type_; }
       const unsigned int getNumTrees()const{ return 0; }
       static const char GAP_CHAR = '-';
       static const char MISSING_CHAR = '*';
       const bool isGap( unsigned int tax, unsigned int trait )const{ return getTraitState(tax,trait) == GAP_CHAR; }
       const bool isGap( char state_datum )const{ return state_datum == GAP_CHAR; }
       const std::string getTreeLabel( unsigned int )const{ return ""; }
       const bool hasGap( unsigned int trait )const{ return true; }
       const Node* findNode( unsigned int, unsigned int )const{ return NULL; }
       const bool isMissing( char datum )const{ return datum == MISSING_CHAR; }
       const unsigned int getTaxonNumber(const std::string& label )const;

     private:
       std::string matrix_label_;
       unsigned int data_type_;
       //unsigned ntax_;
       //unsigned ntraits_;
       std::vector< std::string > matrix_data_;
       std::vector< std::string > taxon_labels_;
  };


  DataRepresentation* phylipparse(PhylipEnvironment* env);

}
#endif
