//First attempt at using NCL to read NEXUS files
//
//Ben Wright
//  Sept. 8, 2009
//  CDAO Developers Group

#include "ncl.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

NxsTaxaBlock*		taxa        = NULL;//new NxsTaxaBlock();
NxsTreesBlock*		trees       =NULL;  //          = new NxsTreesBlock(taxa);
NxsAssumptionsBlock*	assumptions =NULL;//             = new NxsAssumptionsBlock(taxa);
NxsCharactersBlock*	characters  =NULL;      //= new NxsCharactersBlock(taxa, assumptions);
NxsDistancesBlock*	distances   =NULL;    //        = new NxsDistancesBlock(taxa);
NxsDataBlock*		data        =NULL;  //          = new NxsDataBlock(taxa, assumptions);
//NxsUnalignedBlock*	unaligned   =NULL;//            = new NxsUnalignedBlock(taxa);

class MyReader : public NxsReader
	{
	public:
		ifstream inf;
		ofstream outf;

		MyReader(char *infname, char *outfname) : NxsReader()
			{
			inf.open(infname, ios::binary);
			outf.open(outfname);
			}

		~MyReader()
			{
			inf.close();
			outf.close();
			}

	void ExecuteStarting() {}
	void ExecuteStopping() {}

	bool EnteringBlock(NxsString blockName)
		{
		cout << "Reading \"" << blockName << "\" block..." << endl;
		outf << "Reading \"" << blockName << "\" block..." << endl;

		// Returning true means it is ok to delete any data associated with 
		// blocks of this type read in previously
		//
		return true;	
		}

	void SkippingBlock(NxsString blockName)
		{
		cout << "Skipping unknown block (" << blockName << ")..." << endl;
		outf << "Skipping unknown block (" << blockName << ")..." << endl;
		}

	void SkippingDisabledBlock(NxsString blockName) {}

	void OutputComment(const NxsString &msg)
		{
		outf << msg;
		}

	void NexusError(NxsString msg, file_pos pos, unsigned line, unsigned col)
		{
		cerr << endl;
		cerr << "Error found at line " << line;
		cerr << ", column " << col;
		cerr << " (file position " << pos << "):" << endl;
		cerr << msg << endl;

		outf << endl;
		outf << "Error found at line " << line;
		outf << ", column " << col;
		outf << " (file position " << pos << "):" << endl;
		outf << msg << endl;

		exit(0);
		}
	};

class MyToken : public NxsToken
	{
	public:

		MyToken(istream &is, ostream &os) : out(os), NxsToken(is) {}

		void OutputComment(const NxsString &msg)
			{
			cout << msg << endl;
			out << msg << endl;
			}

	private:
		ostream &out;
	};



int main(int argc, char* argv[])
{
  // ./testNCL <NexusFile>
  // ./testNCL
  // the second will prompt for NeXUS file.

  char* file;
  string strFile;
  if(argc >= 2)
  {
    file = argv[1];
    strFile = file;//new string( (const char*) argv[1]);
  }
  else
  {
    cout << "Nexus file to be read? :\n";
    getline(cin, strFile);
    //char* cfile = strFile.c_str();
    //file = cfile;
  }
  
  //file now contains the filename to be used.
  
  //it is being assumed that the file is already in a proper NEXUS format.

  
  cout <<"File to be read:("<<strFile<<")\n";

  
	taxa                    = new NxsTaxaBlock();
	trees                   = new NxsTreesBlock(taxa);
	assumptions             = new NxsAssumptionsBlock(taxa);
	characters              = new NxsCharactersBlock(taxa, assumptions);
	distances               = new NxsDistancesBlock(taxa);
	data                    = new NxsDataBlock(taxa, assumptions);
	//unaligned               = new NxsUnalignedBlock(taxa);

 	char* outfile = "MyOutput.out";

	MyReader nexus( (char*) strFile.c_str(), outfile);
	nexus.Add(taxa);
	nexus.Add(trees);
	nexus.Add(assumptions);
	nexus.Add(characters);
	nexus.Add(distances);
	nexus.Add(data);
	//nexus.Add(unaligned);
	//nexus.Add(this);

	cout<<"Done with .Add() MyReader is setup.  Onto tokens...\n";

	
	MyToken token(nexus.inf, nexus.outf);

	cout<<"MyToken set up, executing MyReader.\n";

	nexus.Execute(token);

	cout<<"MyReader executed completely.\n";

	if(!taxa->IsEmpty())
        	taxa->Report(nexus.outf);
	else
		cout<<"taxa is Empty().\n";
	if(!trees->IsEmpty())
		trees->Report(nexus.outf);
	else
		cout<<"trees is Empty().\n";
	if(!assumptions->IsEmpty())
		assumptions->Report(nexus.outf);
	else
		cout<<"assumptions is Empty().\n";
	if(!characters->IsEmpty())
		characters->Report(nexus.outf);
	else
		cout<<"characters is Empty().\n";
	if(!distances->IsEmpty())
		distances->Report(nexus.outf);
	else
		cout<<"distances is Empty().\n";
	if(!data->IsEmpty())
		data->Report(nexus.outf);
	else
		cout<<"data is Empty().\n";
	//unaligned->Report(nexus.outf);

	cout << "File read and Reported to " << outfile <<endl;

  return 0;
}
