//	Copyright (C) 1999-2002 Paul O. Lewis
//
//	This file is part of NCL (Nexus Class Library).
//
//	NCL is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	NCL is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with NCL; if not, write to the Free Software Foundation, Inc., 
//	59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//

#include "ncl/ncl.h"
#include "basiccmdline.h"
#include <sstream>

/*----------------------------------------------------------------------------------------------------------------------
|	The constructor simply passes along `i' to the base class constructor. Nothing else needs to be done.
*/
MyNexusToken::MyNexusToken(
  wistream & i)	/* is the input file stream attached to the NEXUS file to be read */
  : NxsToken(i)
	{
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Overrides the NxsToken::OutputComment virtual function (which does nothing) to display output comments [!comments
|	like this one beginning with an exclamation point]. The output comment contained in `msg' is simply sent to the
|	standard output stream cout.
*/
void MyNexusToken::OutputComment(
  const NxsString & msg)	/* is the output comment to be displayed */
	{
	wcout << msg << endl;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Initializes the `id' data member to "BASICCMDLINE" and calls the FactoryDefaults member function to perform the 
|	remaining initializations. The data member `next_command' is set to NULL so that memory will be allocated for it in
|	FactoryDefaults.
*/
BASICCMDLINE::BASICCMDLINE()
	{
	id				= L"BASICCMDLINE";

	// Make sure all data members that are pointers are initialized to NULL!
	// Failure to do this will result in problems because functions such as
	// FactoryDefaults() will try to delete an object if it is non-NULL.
	taxa			= NULL;
	trees			= NULL;
	assumptions		= NULL;
	distances		= NULL;
	characters		= NULL;
	data			= NULL;
	unaligned		= NULL;
	next_command	= NULL;

	FactoryDefaults();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Closes `logf' if it is open and deletes memory allocated to `next_command'.
*/
BASICCMDLINE::~BASICCMDLINE()
	{
	assert(next_command != NULL);
	delete [] next_command;

	if (logf_open)
		logf.close();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	The code here is identical to the base class version (simply returns 0), so the code here should either be 
|	modified or this derived version eliminated altogether. Under what circumstances would you need to modify the 
|	default code, you ask? This function should be modified to something meaningful if this derived class needs to 
|	construct and run a NxsSetReader object to read a set involving characters. The NxsSetReader object may need to 
|	use this function to look up a character label encountered in the set. A class that overrides this method should 
|	return the character index in the range [1..`nchar']; i.e., add one to the 0-offset index.
*/
unsigned BASICCMDLINE::CharLabelToNumber(
  NxsString) NCL_COULD_BE_CONST	/* is the character label to be translated to character number */
	{
	return 0;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called by the NxsReader object when a block named `blockName' is entered. Allows program to notify user of 
|	progress in parsing the NEXUS file. Also gives program the opportunity to ask user if it is ok to purge data 
|	currently contained in this block. If user is asked whether existing data should be deleted, and the answer comes 
|	back no, then then return false, otherwise return true. Overrides pure virtual function in class NxsReader. 
*/
bool BASICCMDLINE::EnteringBlock(
  NxsString blockName)	/* is the name of the block just entered */
	{
	message = L"Reading ";
	message += blockName;
	message += L" block...";
	PrintMessage();

	return true;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called by the NxsReader object when exiting a block named `blockName'. Allows program to notify user of progress 
|	in parsing the NEXUS file. Virtual function that overrides the pure virtual function in the base class NxsReader.
*/
void BASICCMDLINE::ExitingBlock(
  NxsString )	/* is the name of the block just exited */
	{
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Sets all data members to their factory default settings: `inf_open', `logf_open' and `quit_now' are set to false; 
|	`message' to the null string, and the pointers `data', `characters', `assumptions', `taxa' and `trees' 
|	are all set to NULL. The C-string `next_command' is allocated COMMAND_MAXLEN + 1 bytes if it is currently NULL, 
|	and its first byte is set to the null character to create an empty `next_command' string.
*/
void BASICCMDLINE::FactoryDefaults()
	{
	isEmpty = true;
	inf_open = false;
	logf_open = false;
	quit_now = false;
	message.clear();

	if (trees != NULL)
		{
		Detach(trees);
		delete trees;
		trees = NULL;
		}

	if (taxa != NULL)
		{
		Detach(taxa);
		delete taxa;
		taxa = NULL;
		}

	if (assumptions != NULL)
		{
		Detach(assumptions);
		delete assumptions;
		assumptions = NULL;
		}

	if (distances != NULL)
		{
		Detach(distances);
		delete distances;
		distances = NULL;
		}

	if (characters != NULL)
		{
		Detach(characters);
		delete characters;
		characters = NULL;
		}

	if (data != NULL)
		{
		Detach(data);
		delete data;
		data = NULL;
		}

	if (unaligned != NULL)
		{
		Detach(unaligned);
		delete unaligned;
		unaligned = NULL;
		}

	if (next_command == NULL)
		{
		next_command = new wchar_t[COMMAND_MAXLEN + 1];
		}
	next_command[0] = '\0';
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if file named `fn' already exists, false otherwise.
*/
bool BASICCMDLINE::FileExists(
  const char * fn)	/* is the name of the file to check */
	{
	bool exists = false;

	FILE *fp = fopen(fn, "r");
	if (fp != NULL)
		{
		fclose(fp);
		exists = true;
		}

	return exists;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called whenever a file name needs to be read from either the command line or a file. Expects next token to be "=" 
|	followed by the token representing the file name. Call this function after, say, the keyword "file" has been read 
|	in the following LOG command:
|>
|	log file=doofus.txt start replace;
|>
|	Note that this function will read only the "=doofus.txt " leaving "start replace;" in the stream for reading at 
|	a later time.
*/
NxsString BASICCMDLINE::GetFileName(
  NxsToken & token)	/* is the token used to read from `in' */
	{
	// Eat the equals sign
	token.GetNextToken();

	if (!token.Equals(L"="))
		{
		errormsg = L"Expecting an equals sign, but found ";
		errormsg += token.GetToken();
		errormsg += L" instead";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}

	// Now get the filename itself
	token.GetNextToken();

	return token.GetToken();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called when the END or ENDBLOCK command needs to be parsed from within the BASICCMDLINE block. Basically just 
|	checks to make sure the next token in the data file is a semicolon.
*/
void BASICCMDLINE::HandleEndblock(
  NxsToken & token)	/* is the token used to read from `in' */
	{
	// Get the semicolon following END or ENDBLOCK token
	token.GetNextToken();

	if (!token.Equals(L";"))
		{
		errormsg = L"Expecting ';' to terminate the END or ENDBLOCK command, but found ";
		errormsg += token.GetToken();
		errormsg += L" instead";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Handles everything after the EXECUTE keyword and the terminating semicolon. Purges all blocks before executing 
|	file specified, and no warning is given of this.
*/
void BASICCMDLINE::HandleExecute(
  NxsToken & token)	/* is the token used to read from `in' */
	{
	// Issuing the EXECUTE command from within a file is a no-no (at least in this program)
	if (inf_open)
		{
		errormsg = L"Cannot issue execute command from within a BASICCMDLINE block";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}

	// Get the file name to execute (note: if filename contains underscores, these will be
	// automatically converted to spaces; user should surround such filenames with single quotes)
	token.GetNextToken();

	NxsString fn = token.GetToken();
        string strfn = "";
        //wstring wcsfn = fn.c_str();
        strfn = wstr_to_str( fn );
//        std::copy( wcsfn.begin(), wcsfn.end(), strfn.begin() );

	// Get the semicolon terminating the EXECUTE command
	token.GetNextToken();

	if (!token.Equals(L";"))
		{
		errormsg = L"Expecting ';' to terminate the EXECUTE command, but found ";
		errormsg += token.GetToken();
		errormsg += L" instead";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}

	if (FileExists(strfn.c_str()))
		{
		cerr << endl;
		wcerr << L"Opening " << fn << L"..." << endl;

		PurgeBlocks();

		wifstream inf(strfn.c_str(), ios::binary | ios::in);

		inf_open = true;

		MyNexusToken ftoken(inf);

		try
			{
			Execute(ftoken);
			}
		catch(NxsException x) 
			{
			NexusError(errormsg, x.pos, x.line, x.col);
			}

		if (inf_open)
			inf.close();
		inf_open = false;

		// Users are allowed to put DATA blocks in their NEXUS files, but internally the data is always
		// stored in a NxsCharacterBlock object.
		if (characters->IsEmpty() && !data->IsEmpty())
			{
			data->TransferTo(*characters);
			}

		}	// if (FileExists(fn.c_str()))

	else
		{
		wcerr << endl;
		wcerr << L"Oops! Could not find specified file: " << fn << endl;
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called when the HELP command needs to be parsed from within the BASICCMDLINE block.
*/
void BASICCMDLINE::HandleHelp(
  NxsToken & token)	/* is the token used to read from `in' */
	{
	// Retrieve all tokens for this command, stopping only in the event
	// of a semicolon or an unrecognized keyword
	for (;;)
		{
		token.GetNextToken();

		if (token.Equals(L";"))
			{
			break;
			}
		else
			{
			errormsg = L"Unexpected keyword (";
			errormsg += token.GetToken();
			errormsg += L") encountered reading HELP command";
			throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
			}
		}

	message = L"\nExamples of use of available commands:";
	message += L"\n  help                     -> shows this message";
	message += L"\n  log file=mylog.txt start -> opens log file named mylog.txt";
	message += L"\n  log stop                 -> closes current log file";
	message += L"\n  exe mydata.nex           -> executes nexus file mydata.nex";
	message += L"\n  show                     -> reports on blocks currently stored";
	message += L"\n  quit                     -> terminates application";
	message += L"\n";
	PrintMessage();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called when the HELP command needs to be parsed from within the BASICCMDLINE block.
*/
void BASICCMDLINE::HandleShow(
  NxsToken & token)	/* is the token used to read from `in' */
	{
	// Retrieve all tokens for this command, stopping only in the event
	// of a semicolon or an unrecognized keyword
	for (;;)
		{
		token.GetNextToken();

		if (token.Equals(L";"))
			break;
		else
			{
			errormsg = L"Unexpected keyword (";
			errormsg += token.GetToken();
			errormsg += L") encountered reading HELP command";
			throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
			}
		}

	bool taxaStored			= !taxa->IsEmpty();
	bool treesStored		= !trees->IsEmpty();
	bool assumptionsStored	= !assumptions->IsEmpty();
	bool distancesStored	= !distances->IsEmpty();
	bool charactersStored	= !characters->IsEmpty();
	bool dataStored			= !data->IsEmpty();
	bool unalignedStored	= !unaligned->IsEmpty();
	bool isAnythingStored = (taxaStored || treesStored || assumptionsStored || distancesStored || charactersStored || dataStored || unalignedStored);

	if (isAnythingStored)
		message = L"\nNexus blocks currently stored:";
	else
		message = L"\nNo Nexus blocks are currently stored.";
	PrintMessage();

	if (!taxa->IsEmpty())
		{
		cerr << "\n  TAXA block found" << endl;
		taxa->Report(wcerr);
		if (logf_open)
			taxa->Report(logf);
		}

	if (!trees->IsEmpty())
		{
		cerr << "\n  TREES block found" << endl;
		trees->Report(wcerr);
		if (logf_open)
			trees->Report(logf);
		}

	if (!assumptions->IsEmpty())
		{
		cerr << "\n  ASSUMPTIONS block found" << endl;
		assumptions->Report(wcerr);
		if (logf_open)
			assumptions->Report(logf);
		}

	if (!distances->IsEmpty())
		{
		cerr << "\n  DISTANCES block found" << endl;
		distances->Report(wcerr);
		if (logf_open)
			distances->Report(logf);
		}

	if (!characters->IsEmpty())
		{
		cerr << "\n  CHARACTERS block found" << endl;
		characters->Report(wcerr);
		if (logf_open)
			characters->Report(logf);
		}

	if (!data->IsEmpty())
		{
		cerr << "\n  DATA block found" << endl;
		data->Report(wcerr);
		if (logf_open)
			data->Report(logf);
		}

	if (!unaligned->IsEmpty())
		{
		cerr << "\n  UNALIGNED block found" << endl;
		unaligned->Report(wcerr);
		if (logf_open)
			unaligned->Report(logf);
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called when the LOG command needs to be parsed from within the BASICCMDLINE block.
*/
void BASICCMDLINE::HandleLog(
  NxsToken & token)	/* is the token used to read from `in' */
	{
	bool starting = false;
	bool stopping = false;
	bool appending = false;
	bool replacing = false;
	bool name_provided = false;
	NxsString logfname;

	// Retrieve all tokens for this command, stopping only in the event
	// of a semicolon or an unrecognized keyword
	for (;;)
		{
		token.GetNextToken();

		if (token.Equals(L";"))
			{
			break;
			}
		else if (token.Abbreviation(L"STOp"))
			{
			stopping = true;
			}
		else if (token.Abbreviation(L"STArt"))
			{
			starting = true;
			}
		else if (token.Abbreviation(L"Replace"))
			{
			replacing = true;
			}
		else if (token.Abbreviation(L"Append"))
			{
			appending = true;
			}
		else if (token.Abbreviation(L"File"))
			{
			logfname = GetFileName(token);
			name_provided = true;
			}
		else
			{
			errormsg = L"Unexpected keyword (";
			errormsg += token.GetToken();
			errormsg += L") encountered reading LOG command";
			throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
			}
		}

	// Check for incompatible combinations of keywords
	if (stopping && (starting || appending || replacing || name_provided))
		{
		errormsg = L"Cannot specify STOP with any of the following START, APPEND, REPLACE, FILE";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}

	if (appending && replacing)
		{
		errormsg = L"Cannot specify APPEND and REPLACE at the same time";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}

	if (logf_open && (starting || name_provided || appending || replacing))
		{
		errormsg = L"Cannot start log file since log file is already open";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}

	// Is user closing an open log file?
	if (stopping)
		{
		logf.close();
		logf_open = false;

		message = L"\nLog file closed";
		PrintMessage();

		return;
		}

	// If this far, must be attempting to open a log file
	if (!name_provided)
		{
		errormsg = L"Must provide a file name when opening a log file\n";
		errormsg += L"e.g., log file=doofus.txt start replace;";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}   

	if (appending)
		{
		logf_open = true;
                string strlog = wstr_to_str( logfname );
                //std::copy( logfname.begin(), logfname.end(), strlog.begin() );
		logf.open( strlog.c_str(), ios::out | ios::app);

		message = L"\nAppending to log file ";
		message += logfname;
		PrintMessage();
		}

	else if (replacing)
		{
		logf_open = true;
                string strlog = wstr_to_str( logfname );
                //std::copy( logfname.begin(), logfname.end(), strlog.begin() );
		logf.open( strlog.c_str() );

		message = L"\nReplacing log file ";
		message += logfname;
		PrintMessage();
		}

	else 
		{
                  string strlog = "";
                  std::copy( strlog.begin(), strlog.end(), strlog.begin() );
		bool exists = FileExists(strlog.c_str());
		bool userok = true;
		if (exists && !UserQuery(L"Ok to replace?", L"Log file specified already exists", BASICCMDLINE::UserQueryEnum(BASICCMDLINE::uq_yes | BASICCMDLINE::uq_no)))
			userok = false;

		if (userok)
			{
			logf_open = true;

			logf.open(strlog.c_str());
			}

		if (exists && userok)
			{
			message = L"\nReplacing log file ";
			message += logfname;
			}

		else if (userok)
			{
			message = L"\nLog file ";
			message += logfname;
			message += L" opened";
			}

		else
			{
			message = L"\nLog command aborted";
			}

		PrintMessage();
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Accepts a string in the form of a BASICCMDLINE block containing one command and processes it just like a real 
|	BASICCMDLINE block in a NEXUS data file.
*/
void BASICCMDLINE::HandleNextCommand()
	{
	std::wistringstream cmdin(next_command);

	MyNexusToken token(cmdin);
	try
		{
		Read(token);
		}
	catch(NxsException x) 
		{
		NexusError(errormsg, x.pos, x.line, x.col);
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called when an error is encountered in a NEXUS file. Allows program to give user details of the error as well as 
|	the precise location of the error.
*/
void BASICCMDLINE::NexusError(
  NxsString msg,	/* is the error message */
  file_pos ,		/* is the point in the NEXUS file where the error occurred */
  long line,		/* is the line in the NEXUS file where the error occurred */
  long col)			/* is the column in the NEXUS file where the error occurred */
	{
	message = L"\n";
	message += msg;
	PrintMessage();

	if (inf_open)
		{
		message = L"Line:   ";
		message += line;
		PrintMessage();

		message = L"Column: ";
		message += col;
		PrintMessage();
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Begins with the command just entered by the user, which is stored in the data member `next_command', adds a 
|	semicolon (if the user failed to supply one), and then adds the string "end;" so the whole bundle looks like a 
|	very short BASICCMDLINE block. This is then passed to HandleNextCommand, which processes it just like a real 
|	BASICCMDLINE block in a NEXUS data file.
*/
void BASICCMDLINE::PreprocessNextCommand()
	{
	// If user failed to add the terminating semicolon, we'll do it now. We will also remove the line feed
	// at the end and add the command "end;" to the end of the line (see explanation below). 
	unsigned len = (unsigned)wcslen(next_command);
	assert(len > 0);

	// Remove any whitespace characters from end of string entered by user
	//
	unsigned i = len;
	while (i > 0 && next_command[i-1] == ' ' || next_command[i-1] == '\t' || next_command[i-1] == '\n')
		i--;

	// If character at position i - 1 is a semicolon, put '\0' terminator at position i;
	// otherwise, put a semicolon at position i and terminator at i + 1
	if (next_command[i-1] != ';')
		{
		next_command[i] = ';';
		i++;
		}
	assert(i <= COMMAND_MAXLEN);
	next_command[i] = '\0';

	// Now add a semicolon at the beginning and terminate with an "END;" command 
	// so that we can pretend this is simply a very short private NEXUS block
	// containing only one command.  This allows us to simply use the Read 
	// function we inherited from the base class BstBase to process the command.
	len = (unsigned)wcslen(next_command);
	assert(len < COMMAND_MAXLEN-2);
	NxsString tmp = L";";
	tmp += next_command;
	tmp += L"end;";
	wcscpy(next_command, tmp.c_str());
	}

/*----------------------------------------------------------------------------------------------------------------------
|	All output is funneled through here. Writes string currently stored in `message' (a NxsString data member) to the 
|	output file stream, if open, and also to the console via cerr. Places a newline after the string if `linefeed' is 
|	true.
*/
void BASICCMDLINE::PrintMessage(
  bool linefeed) NCL_COULD_BE_CONST /* if true, places newline character after message */
	{
	wcerr << message;
	if (linefeed)
		cerr << endl;

	if (logf_open)
		{
		logf << message;
		if (linefeed)
			logf << endl;
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Detaches all blocks, deletes them, creates new blocks, and finally adds the new blocks. Call this function if
|	you want to be sure that there is no data currently stored in any blocks.
*/
void BASICCMDLINE::PurgeBlocks()
	{
	if (blockList != NULL)
		{
		Detach(taxa);
		Detach(trees);
		Detach(assumptions);
		Detach(distances);
		Detach(characters);
		Detach(data);
		Detach(unaligned);
		}

	delete taxa;
	delete trees;
	delete assumptions;
	delete distances;
	delete characters;
	delete data;
	delete unaligned;

	taxa		= new NxsTaxaBlock();
	trees		= new NxsTreesBlock(taxa);
	assumptions	= new NxsAssumptionsBlock(taxa);
	distances	= new NxsDistancesBlock(taxa);
	characters	= new NxsCharactersBlock(taxa, assumptions);
	data		= new NxsDataBlock(taxa, assumptions);
	unaligned	= new NxsUnalignedBlock(taxa);

	Add(taxa);
	Add(trees);
	Add(assumptions);
	Add(distances);
	Add(characters);
	Add(data);
	Add(unaligned);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	This function provides the ability to read everything following the block name (which is read by the NxsReader 
|	object) to the END or ENDBLOCK statement. Characters are read from the input stream `in'. Overrides the virtual 
|	function in the base class.
*/
void BASICCMDLINE::Read(
  NxsToken & token)	/* is the token used to read from `in' */
	{
	isEmpty = false;

	// This should be the semicolon after the block name
	token.GetNextToken();

	if (!token.Equals(L";"))
		{
		errormsg = L"Expecting ';' after ";
		errormsg += id;
		errormsg += L" block name, but found ";
		errormsg += token.GetToken();
		errormsg += L" instead";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}

	for (;;)
		{
		token.GetNextToken();

		if (token.Abbreviation(L"ENdblock"))
			{
			HandleEndblock(token);
			break;
			}
		else if (token.Abbreviation(L"Help"))
			{
			HandleHelp(token);
			}
		else if (token.Abbreviation(L"Log"))
			{
			HandleLog(token);
			}
		else if (token.Abbreviation(L"EXecute"))
			{
			HandleExecute(token);
			}
		else if (token.Abbreviation(L"Show"))
			{
			HandleShow(token);
			}
		else if (token.Abbreviation(L"Quit"))
			{
			quit_now = true;

			message = L"\nBASICCMDLINE says goodbye\n";
			PrintMessage();

			break;
			}
		else
			{
			SkippingCommand(token.GetToken());
			do
				{
				token.GetNextToken();
				}
			while (!token.AtEOF() && !token.Equals(L";"));

			if (token.AtEOF())
				{
				errormsg = L"Unexpected end of file encountered";
				throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
				}
			}
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Overrides the NxsBlock virtual function. This function does nothing because the BASICCMDLINE block is simply a
|	private command block and does not store any data.
*/
void BASICCMDLINE::Reset()
	{
	NxsBlock::Reset();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	This function outputs a brief report of the contents of this BASICCMDLINE block. Overrides the virtual function 
|	in the NxsBlock base class.
*/
void BASICCMDLINE::Report(
  wostream & out) NCL_COULD_BE_CONST /* is the output stream to which to write the report */
	{
	message.clear();
	PrintMessage();
	out << message << '\n';
	message = id;
	message += L" block contains...";
	PrintMessage();
	out << message << '\n';
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Runs the command line interpreter, allowing BASICCMDLINE to interact with user. Typically, this is the only 
|	function called in main after a BASICCMDLINE object is created. If `infile_name' is non-NULL, the first command 
|	executed by the command interpreter will be "EXECUTE `infile_name'".
*/
void BASICCMDLINE::Run(
  const wchar_t * infile_name)	/* is the name of the NEXUS data file to execute (can be NULL) */
	{
	taxa			= new NxsTaxaBlock();
	trees			= new NxsTreesBlock(taxa);
	assumptions		= new NxsAssumptionsBlock(taxa);
	characters		= new NxsCharactersBlock(taxa, assumptions);
	distances		= new NxsDistancesBlock(taxa);
	data			= new NxsDataBlock(taxa, assumptions);
	unaligned		= new NxsUnalignedBlock(taxa);

	Add(taxa);
	Add(trees);
	Add(assumptions);
	Add(characters);
	Add(distances);
	Add(data);
	Add(unaligned);
	Add(this);

	if (infile_name != NULL)
		{
		wcscpy(next_command, L"exe \'");
		wcsncat(next_command, infile_name, 252);
		wcsncat(next_command, L"\'", 252);
		PreprocessNextCommand();
		HandleNextCommand();
		}

	quit_now = false;
	while (!quit_now) 
		{
		wcerr << endl;
		wcerr << L"BASICCMDLINE> ";
		//cin.getline(next_command, COMMAND_MAXLEN);
		unsigned i = 0;
		for(;;)
			{
			int ch = wcin.get();
			if (i > COMMAND_MAXLEN)
				{
				wcerr << endl;
				wcerr << L"Error: the length of any one command cannot exceed ";
				wcerr << COMMAND_MAXLEN << L" characters" << endl;
				break;
				}
			else if (ch == 10 || ch == 13)
				break;
			next_command[i++] = (char)ch;
			next_command[i] = '\0';
			}
		PreprocessNextCommand();
		HandleNextCommand();
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called when program does not recognize a block name encountered in a NEXUS file. Virtual function that overrides 
|	the virtual function in the base class NxsReader.
*/
void BASICCMDLINE::SkippingBlock(
  NxsString blockName)	/* is the unrecognized block name */
	{
	message = L"Skipping unknown block (";
	message += blockName;
	message += L")";
	PrintMessage();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	This function is called when an unknown command named `commandName' is about to be skipped. This version of the 
|	function (which is identical to the base class version) does nothing (i.e., no warning is issued that a command 
|	was unrecognized). Modify this virtual function to provide such warnings to the user (or eliminate it altogether 
|	since the base class version already does what this does).
*/
void BASICCMDLINE::SkippingCommand(
  NxsString commandName)	/* is the name of the command being skipped */
	{
	message = L"Skipping unknown command (";
	message += commandName;
	message += L")";
	PrintMessage();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called by the NxsReader object when skipping a block named blockName that has been disabled. Allows program to 
|	notify user of progress in parsing the NEXUS file. Virtual function that overrides the virtual function in the 
|	base class NxsReader.
*/
void BASICCMDLINE::SkippingDisabledBlock(
  NxsString )	/* is the name of the block just exited */
	{
	}

/*----------------------------------------------------------------------------------------------------------------------
|	The code here is identical to the base class version (simply returns 0), so the code here should either be modified 
|	or this derived version eliminated altogether. Under what circumstances would you need to modify the default code, 
|	you ask? This function should be modified to something meaningful if this derived class needs to construct and run 
|	a NxsSetReader object to read a set involving taxa. The NxsSetReader object may need to use this function to look 
|	up a taxon label encountered in the set. A class that overrides this method should return the taxon index in the 
|	range [1..ntax]; i.e., add one to the 0-offset index.
*/
unsigned BASICCMDLINE::TaxonLabelToNumber(
  NxsString ) const /* is the taxon label to be translated to a taxon number */
	{
	return 0;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if response is either "ok" or "yes", and returns false if response is either "no" or "cancel".
|	This is a general query function that can handle many situations. The possible responses are enumerated in 
|	BASICCMDLINE::UserQueryEnum: uq_cancel, uq_ok, uq_yes, and uq_no. Not yet fully implemented: only handles uq_ok
|	alone or the (uq_yes | uq_no) combination.
*/
bool BASICCMDLINE::UserQuery(
  NxsString mb_message,						/* is the question posed to the user */
  NxsString mb_title,						/* is the title of the message box */
  BASICCMDLINE::UserQueryEnum mb_choices)	/* is the bit combination of uq_xx values indicating which buttons to show */
	{
	const bool yes_no			= (mb_choices == (BASICCMDLINE::uq_yes | BASICCMDLINE::uq_no));
	const bool ok_only		= (mb_choices == BASICCMDLINE::uq_ok);
	assert(ok_only || yes_no); // Still working on other choices

	if (ok_only)
		{
		wcerr << endl;
		wcerr << mb_title << endl;
		wcerr << L"  " << mb_message;
		wcerr << L" (press return to acknowledge) ";
		wcin.getline(next_command, COMMAND_MAXLEN);
		return true;
		}
	wcerr << endl;
	wcerr << mb_title << endl;
	wcerr << L"  " << mb_message;
	wcerr << L" (y/n) ";

	wcin.getline(next_command, COMMAND_MAXLEN);

	// This could be made much simpler by just checking first letter: if 'y' then
	// assume yes, if 'n' assume no.
	bool yep  = (next_command[0] == 'y' && next_command[1] == '\0');
	bool nope = (next_command[0] == 'n' && next_command[1] == '\0');

	while (!yep && !nope)
		{
		wcerr << endl;
		wcerr << L"Must answer by typing either y or n and then pressing the Enter key" << endl;
		wcerr << endl;
		wcerr << mb_title << endl;
		wcerr << L"  " << mb_message;
		wcerr << L" (y/n) ";

		wcin.getline(next_command, COMMAND_MAXLEN);
		yep  = (next_command[0] == 'y' && next_command[1] == '\0');
		nope = (next_command[0] == 'n' && next_command[1] == '\0');
		}

	return yep;
	}

int main(int argc, char *argv[])
	{
	const wchar_t * infile_name = NULL;

	if (argc > 2)
		{
		wcerr << L"Sorry, this program can accept at most one command" << endl;
		wcerr << L"line argument, which must be the name of a NEXUS" << endl;
		wcerr << L"data file." << endl;
		wcerr << endl;
		exit(0);
		}

	else if (argc > 1)
		{
                  //string ifname = argv[1];
                  wstring wifname = str_to_wstr( argv[1] );
                  //std::copy( ifname.begin(), ifname.end(), wifname.begin() );
                  
		infile_name = wifname.c_str();
		}

	BASICCMDLINE basiccmdline;
	basiccmdline.Run(infile_name);

	return 0;
	}
