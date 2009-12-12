#include "grammar/nexus/tree_description_parser.hpp"
#include "data_representation.hpp"
//#include <RawStream.hpp>

#include "util.hpp"
#include "node.hpp"
#include "codegen/codegen.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <unistd.h>

using namespace std;
using namespace CDAO;

class TreeRepresentation : public DataRepresentation {
  public:
    TreeRepresentation( vector< string > taxa, map< string, Node* > trees);
    virtual ~TreeRepresentation();
    virtual const Node* getParseTree( unsigned int i ) const;
    virtual const std::string getTaxonLabel(unsigned int) const;
    virtual const std::string getTreeLabel(unsigned int) const;
    virtual const std::string getMatrixLabel() const;
    virtual void setMatrixLabel(const std::string&);
    virtual const char getTraitState(unsigned int, unsigned int) const;
    virtual const bool isTrait(unsigned int) const;
    virtual const unsigned int getNTax() const;
    virtual const unsigned int getNTraits() const;
    virtual const unsigned int getDataType() const;
    virtual const unsigned int getNumTrees() const;
    virtual const bool hasGap(unsigned int) const;
    virtual const bool isGap(unsigned int, unsigned int) const;
    virtual const Node* findNode(unsigned int, unsigned int) const;
    virtual const unsigned int getTaxonNumber(const std::string&) const;
    virtual const bool isGap(char) const;
    virtual const bool isMissing(char) const;

  private:
    string matrix_label_;
    vector< string > taxa_;
    vector< string > tree_names_;
    map< string, Node* > trees_;

};


DataRepresentation* populate_model( vector< string >& TUs, map< string, Node* >& trees );

void read_input( vector< string >& taxa, map< string, Node* >& trees );

int main( int argc, char** argv ){

   vector< string > taxa = vector< string >();
   map< string, Node* > trees = map< string, Node* >();
   processArgs(argc, argv, NULL);
   read_input( taxa, trees );
   DataRepresentation* model = populate_model( taxa, trees );
   model->setMatrixLabel( getInputFile() );
   CodeGenerator gen( model );
   gen.generate( *(getOutputStream()) );

  return 0;
}

DataRepresentation* populate_model( vector< string >& TUs, map< string, Node* >& trees ){

  return new TreeRepresentation( TUs, trees );
}

void read_input( vector< string >& taxa, map< string, Node* >& trees ){
   //TypedRawInputStream pin( pipefd[ READ_END ] );
   string command;
   string taxon_name;
   string tree_name;
   string rooted;
   string newick_data;
  // vector< string > taxa(0);
   //map< string, Node* > trees = map< string, Node* >();
   while (!cin.eof()){
     //cout << (char)pin.get();
     cin >> command;
     //cout << command;
     if (command == "TU"){
        cin >> taxon_name;
        taxa.push_back( taxon_name );
     }
     else if ( command == "TREE"){
        cin >> tree_name;
        cin >> rooted;
        cin >> newick_data;
        trees[ tree_name ] = TreeDescriptionParser( newick_data ).getParseTree();
     }
     else {
        cerr << "Unrecognized command: \"" << command << "\"" << endl;
        //pin.close();
        exit( 1 );
     }
   }
      //pin.close();
     // for (unsigned i = 0; i < taxa.size(); ++i){
     //    cout << "<cdao:TU rdf:ID=\"" + taxa.at( i ) + "\"/>\n";
     // }
   return;
}

TreeRepresentation::TreeRepresentation( vector< string > taxa, map< string, Node* > trees ):taxa_(taxa), trees_(trees){
  tree_names_ = vector< string >();
  for (map< string, Node* >::const_iterator i = trees_.begin(); i != trees_.end(); ++i){
      tree_names_.push_back( i->first );
  }

}
TreeRepresentation::~TreeRepresentation(){
  for ( map< string, Node* >::iterator i = trees_.begin(); i != trees_.end(); ++i ){
     delete (i->second);
  }
}
const CDAO::Node* TreeRepresentation::getParseTree( unsigned int i ) const{
   map< string, Node* >::const_iterator  ret = trees_.find( tree_names_.at( i ) );
   return ret == trees_.end() ? NULL : ret->second;
}
const std::string TreeRepresentation::getTaxonLabel(unsigned int i) const{
   return taxa_.at( i );  
}
const std::string TreeRepresentation::getTreeLabel( unsigned int i ) const{ return tree_names_.at( i ); }
const std::string TreeRepresentation::getMatrixLabel() const{ return matrix_label_; }
void TreeRepresentation::setMatrixLabel(const std::string& l){ matrix_label_ = l; }
const char TreeRepresentation::getTraitState(unsigned int, unsigned int) const{ return '\0'; }
const bool TreeRepresentation::isTrait(unsigned int) const{ return false; }
const unsigned int TreeRepresentation::getNTax() const{ return taxa_.size(); }
const unsigned int TreeRepresentation::getNTraits() const{ return 0; }
const unsigned int TreeRepresentation::getDataType() const{ return 0; }
const unsigned int TreeRepresentation::getNumTrees() const{ return tree_names_.size(); }
const bool TreeRepresentation::hasGap(unsigned int) const{ return false; }
const bool TreeRepresentation::isGap(unsigned int, unsigned int) const{ return false; }
const CDAO::Node* TreeRepresentation::findNode(unsigned int, unsigned int) const{ return NULL; }
const unsigned int TreeRepresentation::getTaxonNumber(const std::string& target) const{ 
        unsigned int ret = 0;
        for (; ret < taxa_.size() && taxa_.at( ret ) != target; ++ret );
        return ret;
}
const bool TreeRepresentation::isGap(char) const{ return false; }
const bool TreeRepresentation::isMissing(char) const{ return false; }

