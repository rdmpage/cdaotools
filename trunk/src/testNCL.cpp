//First attempt at using NCL to read NEXUS files
//
//Ben Wright
//  Sept. 8, 2009
//  CDAO Developers Group

#include "ncl.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <wstring>

NxsTaxaBlock*		taxa        = NULL;//new NxsTaxaBlock();
NxsTreesBlock*		trees       =NULL;  //          = new NxsTreesBlock(taxa);
NxsAssumptionsBlock*	assumptions =NULL;//             = new NxsAssumptionsBlock(taxa);
NxsCharactersBlock*	wchar_tacters  =NULL;      //= new NxsCharactersBlock(taxa, assumptions);
NxsDistancesBlock*	distances   =NULL;    //        = new NxsDistancesBlock(taxa);
NxsDataBlock*		data        =NULL;  //          = new NxsDataBlock(taxa, assumptions);
//NxsUnalignedBlock*	unaligned   =NULL;//            = new NxsUnalignedBlock(taxa);

class MyReader : public NxsReader
	{
	public:
		ifstream inf;
		ofstream outf;

		MyReader(wchar_t *infname, wchar_t *outfname) : NxsReader()
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
		cout << L"Reading \"" << blockName << L"\" block..." << endl;
		outf << L"Reading \"" << blockName << L"\" block..." << endl;

		// Returning true means it is ok to delete any data associated with 
		// blocks of this type read in previously
		//
		return true;	
		}

	void SkippingBlock(NxsString blockName)
		{
		cout << L"Skipping unknown block (" << blockName << L")..." << endl;
		outf << L"Skipping unknown block (" << blockName << L")..." << endl;
		}

	void SkippingDisabledBlock(NxsString blockName) {}

	void OutputComment(const NxsString &msg)
		{
		outf << msg;
		}

	void NexusError(NxsString msg, file_pos pos, unsigned line, unsigned col)
		{
		cerr << endl;
		cerr << L"Error found at line L" << line;
		cerr << L", column L" << col;
		cerr << L" (file position L" << pos << L"):" << endl;
		cerr << msg << endl;

		outf << endl;
		outf << L"Error found at line L" << line;
		outf << L", column L" << col;
		outf << L" (file position L" << pos << L"):" << endl;
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



int main(int argc, wchar_t* argv[])
{
  // ./testNCL <NexusFile>
  // ./testNCL
  // the second will prompt for NeXUS file.

  wchar_t* file;
  wstring strFile;
  if(argc >= 2)
  {
    file = argv[1];
    strFile = file;//new wstring( (const wchar_t*) argv[1]);
  }
  else
  {
    cout << L"Nexus file to be read? :\n";
    getline(cin, strFile);
    //wchar_t* cfile = strFile.c_str();
    //file = cfile;
  }
  
  //file now contains the filename to be used.
  
  //it is being assumed that the file is already in a proper NEXUS format.

  
  cout <<"File to be read:("<<strFile<<")\n";

  
	taxa                    = new NxsTaxaBlock();
	trees                   = new NxsTreesBlock(taxa);
	assumptions             = new NxsAssumptionsBlock(taxa);
	wchar_tacters              = new NxsCharactersBlock(taxa, assumptions);
	distances               = new NxsDistancesBlock(taxa);
	data                    = new NxsDataBlock(taxa, assumptions);
	//unaligned               = new NxsUnalignedBlock(taxa);

 	wchar_t* outfile = L"MyOutput.out";

	MyReader nexus( (wchar_t*) strFile.c_str(), outfile);
	nexus.Add(taxa);
	nexus.Add(trees);
	nexus.Add(assumptions);
	nexus.Add(wchar_tacters);
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
	if(!wchar_tacters->IsEmpty())
		wchar_tacters->Report(nexus.outf);
	else
		cout<<"wchar_tacters is Empty().\n";
	if(!distances->IsEmpty())
		distances->Report(nexus.outf);
	else
		cout<<"distances is Empty().\n";
	if(!data->IsEmpty())
		data->Report(nexus.outf);
	else
		cout<<"data is Empty().\n";
	//unaligned->Report(nexus.outf);

	cout << L"File read and Reported to L" << outfile <<endl;

  return 0;
}
