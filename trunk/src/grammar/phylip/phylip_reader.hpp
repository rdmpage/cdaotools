#ifndef PHYLIP_TRANSLATOR_HPP_
#define PHYLIP_TRANSLATOR_HPP_
#include <vector>
#include <string>
#include <data_representation.hpp>
namespace CDAO {
  /*
  class PhylipEnvironment {
    public:
      PhylipEnvironment(istream& in, bool intl ):in_(in), interleaved_(intl){}
      ~PhylipEnvironment(){}
      istream& getinput(){ return in_; }
      bool isInterleaved()const{ return interleaved_; }
    private:
      istream& in_;
      bool interleaved_;
  };*/

  class PhylipDataRepresentation : public DataRepresentation {
     public:
       PhylipDataRepresentation( std::vector< std::wstring > data, std::vector< std::wstring > labels );
       ~PhylipDataRepresentation();
       const unsigned getNTraits()const{ return matrix_data_.size(); }
       const unsigned getNTax()const{ return matrix_data_.at(0).size(); }
       const Node* getParseTree( unsigned int )const{ return NULL; }
       const std::wstring getTaxonLabel( unsigned int tax )const{ return taxon_labels_.at( tax ); }
       const std::wstring getTreeLabel(unsigned int ){ return L""; }
       const std::wstring getMatrixLabel()const{ return matrix_label_; }
       void setMatrixLabel( const std::wstring& ml ){ matrix_label_ = ml; }
       const wchar_t getTraitState( unsigned int tax, unsigned int trait )const{ return matrix_data_.at( tax ).at( trait ); }
       const bool isTrait( unsigned int )const{ return false; }
       void setDataType(unsigned int dtype){ data_type_ = dtype;  }
       const unsigned int getDataType()const{ return data_type_; }
       const unsigned int getNumTrees()const{ return 0; }
       static const wchar_t GAP_CHAR = '-';
       static const wchar_t MISSING_CHAR = '*';
       const bool isGap( unsigned int tax, unsigned int trait )const{ return getTraitState(tax,trait) == GAP_CHAR; }
       const bool isGap( wchar_t state_datum )const{ return state_datum == GAP_CHAR; }
       const std::wstring getTreeLabel( unsigned int )const{ return L""; }
       const bool hasGap( unsigned int trait )const{ return true; }
       const Node* findNode( unsigned int, unsigned int )const{ return NULL; }
       const bool isMissing( wchar_t datum )const{ return datum == MISSING_CHAR; }
       const unsigned int getTaxonNumber(const std::wstring& label )const;
       const char getGapChar()const{ return GAP_CHAR; }
       const char getMissingChar()const{ return MISSING_CHAR; }
     private:
       std::wstring matrix_label_;
       unsigned int data_type_;
       //unsigned ntax_;
       //unsigned ntraits_;
       std::vector< std::wstring > matrix_data_;
       std::vector< std::wstring > taxon_labels_;
  };


  DataRepresentation* phylipparse();

}
#endif
