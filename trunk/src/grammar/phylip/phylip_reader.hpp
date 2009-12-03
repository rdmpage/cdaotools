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
      PhylipEnvironment(istream& in, interleaved ):in_(in), interleaved_(interleaved){}
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
       
     private:
       //unsigned ntax_;
       //unsigned ntraits_;
       std::vector< std::string > matrix_data_;
       std::vector< std::string > taxon_labels_;
  };


  DataRepresentation* phylipparse();

}
#endif
