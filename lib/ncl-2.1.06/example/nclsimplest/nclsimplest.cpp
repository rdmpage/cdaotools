#include "ncl/ncl.h"

NxsTaxaBlock	*taxa	= NULL;
NxsTreesBlock	*trees	= NULL;

class MyReader : public NxsReader
	{
	public:
		wifstream inf;
		wofstream outf;

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
		wcout << L"Reading \"" << blockName << L"\" block..." << endl;
		outf << L"Reading \"" << blockName << L"\" block..." << endl;

		// Returning true means it is ok to delete any data associated with 
		// blocks of this type read in previously
		//
		return true;	
		}

	void SkippingBlock(NxsString blockName)
		{
		wcout << L"Skipping unknown block (" << blockName << L")..." << endl;
		outf << L"Skipping unknown block (" << blockName << L")..." << endl;
		}

	void SkippingDisabledBlock(NxsString ) 
		{
		}

	void OutputComment(const NxsString &msg)
		{
		outf << msg;
		}

	void NexusError(NxsString msg, file_pos pos, long line, long col)
		{
		wcerr << endl;
		wcerr << L"Error found at line " << line;
		wcerr << L", column " << col;
		wcerr << L" (file position " << pos << L"):" << endl;
		wcerr << msg << endl;

		outf << endl;
		outf << L"Error found at line " << line;
		outf << L", column " << col;
		outf << L" (file position " << pos << L"):" << endl;
		outf << msg << endl;

		exit(0);
		}
	};

class MyToken : public NxsToken
	{
	public:

		MyToken(wistream &is, wostream &os)
			:NxsToken(is),
			out(os)
			{
			}

		void OutputComment(const NxsString &msg)
			{
			wcout << msg << endl;
			out << msg << endl;
			}

	private:
		wostream &out;
	};

int main(int , char *argv[])
	{
	taxa = new NxsTaxaBlock();
	trees = new NxsTreesBlock(taxa);

	MyReader nexus(argv[1], argv[2]);
	nexus.Add(taxa);
	nexus.Add(trees);

	MyToken token(nexus.inf, nexus.outf);
	nexus.Execute(token);

	taxa->Report(nexus.outf);
	trees->Report(nexus.outf);

	return 0;
	}

