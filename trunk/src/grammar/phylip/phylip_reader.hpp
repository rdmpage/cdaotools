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
 /**
  * Data representation implementation for phylip format data
  */
  class PhylipDataRepresentation : public DataRepresentation {
     public:
       /**
	* Create a new phylip data representation with the specified contents.
	*/
       PhylipDataRepresentation( std::vector< std::wstring > data, std::vector< std::wstring > labels );
       ~PhylipDataRepresentation();
       /**
	* Get the number of columns/traits in the matrix
	*/
       const unsigned getNTraits()const{ return matrix_data_.size(); }
       /**
	* Get the number of rows/taxa in the matrix
	*/
       const unsigned getNTax()const{ return matrix_data_.at(0).size(); }
       /**
	* NA
	*/
       const Node* getParseTree( unsigned int )const{ return NULL; }
       /**
	* Get the specified taxon label
	*/
       const std::wstring getTaxonLabel( unsigned int tax )const{ return taxon_labels_.at( tax ); }
       /**
	* NA
	*/
       const std::wstring getTreeLabel(unsigned int ){ return L""; }
       /**
	* Return an identifier associated with the matrix.
	*/
       const std::wstring getMatrixLabel()const{ return matrix_label_; }
       /**
	* Specify an identifer for the matrix.
	*/
       void setMatrixLabel( const std::wstring& ml ){ matrix_label_ = ml; }
       /**
	* Get the state of a particular trait for the speicifed taxon.
	*/
       const wchar_t getTraitState( unsigned int tax, unsigned int trait )const{ return matrix_data_.at( tax ).at( trait ); }
       /**
	* True of all taxa have the same state for the specified character.
	*/
       const bool isTrait( unsigned int )const{ return false; }
       /**
	* Set the type of data in the matrix.
	*/
       void setDataType(unsigned int dtype){ data_type_ = dtype;  }
       /**
	* Get the datatype of the matrix.
	*/
       const unsigned int getDataType()const{ return data_type_; }
       /**
	* NA
	*/
       const unsigned int getNumTrees()const{ return 0; }
       static const wchar_t GAP_CHAR = '-';
       static const wchar_t MISSING_CHAR = '*';
       /**
	* True if the specified state is a gap state.
	*/
       const bool isGap( unsigned int tax, unsigned int trait )const{ return getTraitState(tax,trait) == GAP_CHAR; }
       /**
	* True if the specified character is a gap.
	*/
       const bool isGap( wchar_t state_datum )const{ return state_datum == GAP_CHAR; }
       /**
	* NA
	*/
       const std::wstring getTreeLabel( unsigned int )const{ return L""; }
       /**
	* True if at least one taxa in the column has a gap state.
	*/
       const bool hasGap( unsigned int trait )const{ return true; }
       /**
	* NA
	*/
       const Node* findNode( unsigned int, unsigned int )const{ return NULL; }
       /**
	* True if the specified character is a missing code
	*/
       const bool isMissing( wchar_t datum )const{ return datum == MISSING_CHAR; }
       /**
	* Get the number associated with a taxon name.
	*/
       const unsigned int getTaxonNumber(const std::wstring& label )const;
       /**
	* Get the gap character that is being used for this matrix.
	*/
       const char getGapChar()const{ return GAP_CHAR; }
       /**
	* Get the character that is being used to represent the "missing" state.
	*/
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
