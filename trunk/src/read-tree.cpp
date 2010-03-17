//#include "grammar/nexus/tree_description_parser2.hpp"
#include "data_representation.hpp"
#include <RawStream.hpp>

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
#include <sys/wait.h>
#include <strings.h>

extern int yyparse();

using namespace std;
using namespace CDAO;
Node* pTree = NULL;
class TreeRepresentation : public DataRepresentation {
  public:
    TreeRepresentation( vector< wstring > taxa, map< wstring, Node* > trees);
    virtual ~TreeRepresentation();
    virtual const Node* getParseTree( unsigned int i ) const;
    virtual const std::wstring getTaxonLabel(unsigned int) const;
    virtual const std::wstring getTreeLabel(unsigned int) const;
    virtual const std::wstring getMatrixLabel() const;
    virtual void setMatrixLabel(const std::wstring&);
    virtual const wchar_t getTraitState(unsigned int, unsigned int) const;
    virtual const bool isTrait(unsigned int) const;
    virtual const unsigned int getNTax() const;
    virtual const unsigned int getNTraits() const;
    virtual const unsigned int getDataType() const;
    virtual const unsigned int getNumTrees() const;
    virtual const bool hasGap(unsigned int) const;
    virtual const bool isGap(unsigned int, unsigned int) const;
    virtual const Node* findNode(unsigned int, unsigned int) const;
    virtual const unsigned int getTaxonNumber(const std::wstring&) const;
    virtual const bool isGap(wchar_t) const;
    virtual const bool isMissing(wchar_t) const;

  private:
    wstring matrix_label_;
    vector< wstring > taxa_;
    vector< wstring > tree_names_;
    map< wstring, Node* > trees_;

};


DataRepresentation* populate_model( vector< wstring >& TUs, map< wstring, Node* >& trees );

void read_input( vector< wstring >& taxa, map< wstring, Node* >& trees );

int main( int argc, char** argv ){
   
   vector< wstring > taxa = vector< wstring >();
   map< wstring, Node* > trees = map< wstring, Node* >();
   
   //wcerr << "Read tree processing args\n";

   processArgs(argc, argv, NULL);

   //wcerr << "Read tree processed args\n";

   map_segment();

   read_input( taxa, trees );
   
  // wcerr << "Read tree read input\n";

   DataRepresentation* model = populate_model( taxa, trees );
   
  // wcerr << "Read tree populated model\n";
   
   model->setMatrixLabel( CDAO::str_to_wstr( getInputFile() ) );

  // wcerr << "Read tree set matrix label\n";

   CodeGenerator gen( model );

  // wcerr << "Read tree initialized output formatter\n";

   gen.generate( *(GlobalState::getOutfile()) );

 //  wcerr << "Read tree" << getpid() << " generated output\n";

   unmap_segment();

  return 0;
}

DataRepresentation* populate_model( vector< wstring >& TUs, map< wstring, Node* >& trees ){

  return new TreeRepresentation( TUs, trees );
}

extern FILE* yyin;

void read_input( vector< wstring >& taxa, map< wstring, Node* >& trees ){
   //TypedRawInputStream pin( pipefd[ READ_END ] );
   wstring command;
   wstring taxon_name;
   wstring tree_name;
   wstring rooted;
   wstring newick_data;

  // vector< wstring > taxa(0);
   //map< wstring, Node* > trees = map< wstring, Node* >();
   while (!wcin.eof()){
     
    // wcerr << "Preparing to read the command\n";
     //cout << (wchar_t)pin.get();
     wcin >> command;

     //wcerr << "Read command: \"" << command << "\"\n";

     //cout << command;
     if (command == L"TU"){
       wcin >> taxon_name;
       // wcerr << L"read taxon id: " << taxon_name << endl;
        taxa.push_back( taxon_name );
        taxon_name = L"";
     }
     else if ( command == L"TREE"){
        wcin >> tree_name;
        wcin >> rooted;
        int ch;
        while (!wcin.eof() && (ch =wcin.get()) != ';' ){
            newick_data += (wchar_t)ch;
        }
        //wcerr << L"Newick data: " << newick_data << endl;
	string nwkdatatemplate = "temp.newickXXXXXX";
        char* nwkdataf = (char*)calloc( nwkdatatemplate.size(), sizeof(char)  );
	strncpy(nwkdataf, nwkdatatemplate.c_str(), nwkdatatemplate.size());
        int tmpnkwfd = mkstemp( nwkdataf  );
	write( tmpnkwfd, CDAO::wstr_to_str(newick_data).c_str(), newick_data.size()  );
	close( tmpnkwfd );
        FILE* nkwf = fopen( nwkdataf , "r");
	//fputs( CDAO::wstr_to_str(newick_data).c_str(), nkwf  );
	yyin = nkwf;
	//wcerr << "Parsing tree: " << newick_data << endl;
	yyparse(); 
	fclose( nkwf );
	unlink( nwkdataf );
	free( nwkdataf );
	//wcerr << "writing pTree:" << pTree << " to the parent\n";
        trees[ tree_name ] =  pTree; //TreeDescriptionParser( newick_data ).getParseTree();

        //wcerr << L"read tree: " << tree_name << L" rooted: " << rooted << L" description: " << newick_data << endl;
        tree_name = rooted = newick_data = L"";
     }
     else if ( command == L"" ){/* skip empty line */}
     else {
        wcerr << L"Unrecognized command: \"" << command << L"\"" << endl;
        //pin.close();
        exit( 1 );
     }
     command = L"";
   }
      //pin.close();
     // for (unsigned i = 0; i < taxa.size(); ++i){
     //    cout << L"<cdao:TU rdf:ID=\"" + taxa.at( i ) + L"\"/>\n";
     // }
   return;
}

TreeRepresentation::TreeRepresentation( vector< wstring > taxa, map< wstring, Node* > trees ):taxa_(taxa), trees_(trees){
  tree_names_ = vector< wstring >();
  for (map< wstring, Node* >::const_iterator i = trees_.begin(); i != trees_.end(); ++i){
      tree_names_.push_back( i->first );
  }

}
TreeRepresentation::~TreeRepresentation(){
  for ( map< wstring, Node* >::iterator i = trees_.begin(); i != trees_.end(); ++i ){
     delete (i->second);
  }
}
const CDAO::Node* TreeRepresentation::getParseTree( unsigned int i ) const{
   map< wstring, Node* >::const_iterator  ret = trees_.find( tree_names_.at( i ) );
   return ret == trees_.end() ? NULL : ret->second;
}
const std::wstring TreeRepresentation::getTaxonLabel(unsigned int i) const{
   return taxa_.at( i );  
}
const std::wstring TreeRepresentation::getTreeLabel( unsigned int i ) const{ return tree_names_.at( i ); }
const std::wstring TreeRepresentation::getMatrixLabel() const{ return matrix_label_; }
void TreeRepresentation::setMatrixLabel(const std::wstring& l){ matrix_label_ = l; }
const wchar_t TreeRepresentation::getTraitState(unsigned int, unsigned int) const{ return '\0'; }
const bool TreeRepresentation::isTrait(unsigned int) const{ return false; }
const unsigned int TreeRepresentation::getNTax() const{ return taxa_.size(); }
const unsigned int TreeRepresentation::getNTraits() const{ return 0; }
const unsigned int TreeRepresentation::getDataType() const{ return 0; }
const unsigned int TreeRepresentation::getNumTrees() const{ return tree_names_.size(); }
const bool TreeRepresentation::hasGap(unsigned int) const{ return false; }
const bool TreeRepresentation::isGap(unsigned int, unsigned int) const{ return false; }
const CDAO::Node* TreeRepresentation::findNode(unsigned int, unsigned int) const{ return NULL; }
const unsigned int TreeRepresentation::getTaxonNumber(const std::wstring& target) const{ 
        unsigned int ret = 0;
        for (; ret < taxa_.size() && taxa_.at( ret ) != target; ++ret );
        return ret;
}
const bool TreeRepresentation::isGap(wchar_t) const{ return false; }
const bool TreeRepresentation::isMissing(wchar_t) const{ return false; }

