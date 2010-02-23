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
#include "basicfactory.h"
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
|	Initializes the `id' data member to "BASICFACTORY" and calls the FactoryDefaults member function to perform the 
|	remaining initializations. The data member `next_command' is set to NULL so that memory will be allocated for it in
|	FactoryDefaults.
*/
BASICFACTORY::BASICFACTORY()
	{
	id				= L"BASICFACTORY";
	Add(this);
	next_command	= NULL;
	FactoryDefaults();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Closes `logf' if it is open and deletes memory allocated to `next_command'.
*/
BASICFACTORY::~BASICFACTORY()
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
unsigned BASICFACTORY::CharLabelToNumber(
  NxsString) NCL_COULD_BE_CONST /* is the character label to be translated to character number */
	{
	return 0;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called by the NxsReader object when a block named `blockName' is entered. Allows program to notify user of 
|	progress in parsing the NEXUS file. Also gives program the opportunity to ask user if it is ok to purge data 
|	currently contained in this block. If user is asked whether existing data should be deleted, and the answer comes 
|	back no, then then return false, otherwise return true. Overrides pure virtual function in class NxsReader. 
*/
bool BASICFACTORY::EnteringBlock(
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
void BASICFACTORY::ExitingBlock(
  NxsString )	/* is the name of the block just exited */
	{
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Sets all data members to their factory default settings: `inf_open', `logf_open' and `quit_now' are set to false; 
|	`message' to the null string.
|	The C-string `next_command' is allocated COMMAND_MAXLEN + 1 bytes if it is currently NULL, 
|	and its first byte is set to the null character to create an empty `next_command' string.
*/
void BASICFACTORY::FactoryDefaults()
	{
	isEmpty = true;
	inf_open = false;
	logf_open = false;
	quit_now = false;
	message.clear();
	if (next_command == NULL)
		{
		next_command = new wchar_t[COMMAND_MAXLEN + 1];
		}
	next_command[0] = '\0';
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if file named `fn' already exists, false otherwise.
*/
bool BASICFACTORY::FileExists(
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
NxsString BASICFACTORY::GetFileName(
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
|	Called when the END or ENDBLOCK command needs to be parsed from within the BASICFACTORY block. Basically just 
|	checks to make sure the next token in the data file is a semicolon.
*/
void BASICFACTORY::HandleEndblock(
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
void BASICFACTORY::HandleExecute(
  NxsToken & token)	/* is the token used to read from `in' */
	{
	// Issuing the EXECUTE command from within a file is a no-no (at least in this program)
	if (inf_open)
		{
		errormsg = L"Cannot issue execute command from within a BASICFACTORY block";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}

	// Get the file name to execute (note: if filename contains underscores, these will be
	// automatically converted to spaces; user should surround such filenames with single quotes)
	token.GetNextToken();

	NxsString fn = token.GetToken();
        string nfn = wstr_to_str( fn );
//        std::copy( fn.begin(), fn.end(), nfn.begin() );
	// Get the semicolon terminating the EXECUTE command
	token.GetNextToken();

	if (!token.Equals(L";"))
		{
		errormsg = L"Expecting ';' to terminate the EXECUTE command, but found ";
		errormsg += token.GetToken();
		errormsg += L" instead";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}
         
	if (FileExists(nfn.c_str()))
		{
		wcerr << endl;
		wcerr << "Opening " << fn << "..." << endl;

		PurgeBlocks();

		wifstream inf(nfn.c_str(), ios::binary | ios::in);

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

		}	// if (FileExists(fn.c_str()))

	else
		{
		wcerr << endl;
		wcerr << "Oops! Could not find specified file: " << fn << endl;
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called when the HELP command needs to be parsed from within the BASICFACTORY block.
*/
void BASICFACTORY::HandleHelp(
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
|	Called when the HELP command needs to be parsed from within the BASICFACTORY block.
*/
void BASICFACTORY::HandleShow(
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

	BlockTypeToBlockList usedBlocksByType = GetUsedBlocks();
	if (usedBlocksByType.size() > 1) //the "BASICFACTORY" key will always be present with a `this` pointer
		message = L"\nNexus blocks currently stored:";
	else
		message = L"\nNo Nexus blocks are currently stored.";
	PrintMessage();
	const wchar_t * blocknames[7] = {L"TAXA", L"TREES", L"ASSUMPTIONS", L"DISTANCES", L"CHARACTERS", L"DATA", L"UNALIGNED"};
	for (int i = 0; i < 7; ++i)
		{
		std::wstring n(blocknames[i]);
		BlockReaderList brl = usedBlocksByType[n];
		for (BlockReaderList::iterator brlIt = brl.begin(); brlIt != brl.end(); ++brlIt)
			{
			wcerr << "\n  " << n << " block found" << endl;
			(*brlIt)->Report(wcerr);
			if (logf_open)
				(*brlIt)->Report(logf);
			}
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called when the LOG command needs to be parsed from within the BASICFACTORY block.
*/
void BASICFACTORY::HandleLog(
  NxsToken & token)	/* is the token used to read from `in' */
	{
	bool starting = false;
	bool stopping = false;
	bool appending = false;
	bool replacing = false;
	bool name_provided = false;
	NxsString logfname;
        string nlgn = "";
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
                        nlgn = wstr_to_str( logfname );
                        //std::copy( logfname.begin(), logfname.end(), nlgn.begin() );
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
                		logf.open(nlgn.c_str(), ios::out | ios::app);

		message = L"\nAppending to log file ";
		message += logfname;
		PrintMessage();
		}

	else if (replacing)
		{
		logf_open = true;
		logf.open(nlgn.c_str());

		message = L"\nReplacing log file ";
		message += logfname;
		PrintMessage();
		}

	else 
		{
		bool exists = FileExists(nlgn.c_str());
		bool userok = true;
		if (exists && !UserQuery(L"Ok to replace?", L"Log file specified already exists", BASICFACTORY::UserQueryEnum(BASICFACTORY::uq_yes | BASICFACTORY::uq_no)))
			userok = false;

		if (userok)
			{
			logf_open = true;
			logf.open(nlgn.c_str());
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
|	Accepts a string in the form of a BASICFACTORY block containing one command and processes it just like a real 
|	BASICFACTORY block in a NEXUS data file.
*/
void BASICFACTORY::HandleNextCommand()
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
void BASICFACTORY::NexusError(
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
|	very short BASICFACTORY block. This is then passed to HandleNextCommand, which processes it just like a real 
|	BASICFACTORY block in a NEXUS data file.
*/
void BASICFACTORY::PreprocessNextCommand()
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
void BASICFACTORY::PrintMessage(
  bool linefeed) NCL_COULD_BE_CONST /* if true, places newline character after message */
	{
	wcerr << message;
	if (linefeed)
		wcerr << endl;

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
void BASICFACTORY::PurgeBlocks()
	{
	std::set<NxsBlock*> ub = GetSetOfAllUsedBlocks();
	ClearUsedBlockList();
	for (std::set<NxsBlock*>::iterator ubIt = ub.begin(); ubIt != ub.end(); ++ubIt)
		{
		if (*ubIt != this)
			delete *ubIt;
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	This function provides the ability to read everything following the block name (which is read by the NxsReader 
|	object) to the END or ENDBLOCK statement. Characters are read from the input stream `in'. Overrides the virtual 
|	function in the base class.
*/
void BASICFACTORY::Read(
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

			message = L"\nBASICFACTORY says goodbye\n";
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
|	Overrides the NxsBlock virtual function. This function does nothing because the BASICFACTORY block is simply a
|	private command block and does not store any data.
*/
void BASICFACTORY::Reset()
	{
	NxsBlock::Reset();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	This function outputs a brief report of the contents of this BASICFACTORY block. Overrides the virtual function 
|	in the NxsBlock base class.
*/
void BASICFACTORY::Report(
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
|	Runs the command line interpreter, allowing BASICFACTORY to interact with user. Typically, this is the only 
|	function called in main after a BASICFACTORY object is created. If `infile_name' is non-NULL, the first command 
|	executed by the command interpreter will be "EXECUTE `infile_name'".
*/
void BASICFACTORY::Run(
  const char * infile_name)	/* is the name of the NEXUS data file to execute (can be NULL) */
	{
	
	if (infile_name != NULL)
		{
                  //string inn( infile_name );
                  wstring win = str_to_wstr( infile_name );
                  //std::copy( inn.begin(), inn.end(), win.begin() );
		wcscpy(next_command, L"exe ");
		wcsncat(next_command, win.c_str(), 252);
		PreprocessNextCommand();
		HandleNextCommand();
		}

	quit_now = false;
	while (!quit_now) 
		{
		wcerr << endl;
		wcerr << L"BASICFACTORY> ";
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
			next_command[i++] = (wchar_t)ch;
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
void BASICFACTORY::SkippingBlock(
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
void BASICFACTORY::SkippingCommand(
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
void BASICFACTORY::SkippingDisabledBlock(
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
unsigned BASICFACTORY::TaxonLabelToNumber(
  NxsString ) const/* is the taxon label to be translated to a taxon number */
	{
	return 0;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if response is either "ok" or "yes", and returns false if response is either "no" or "cancel".
|	This is a general query function that can handle many situations. The possible responses are enumerated in 
|	BASICFACTORY::UserQueryEnum: uq_cancel, uq_ok, uq_yes, and uq_no. Not yet fully implemented: only handles uq_ok
|	alone or the (uq_yes | uq_no) combination.
*/
bool BASICFACTORY::UserQuery(
  NxsString mb_message,						/* is the question posed to the user */
  NxsString mb_title,						/* is the title of the message box */
  BASICFACTORY::UserQueryEnum mb_choices)	/* is the bit combination of uq_xx values indicating which buttons to show */
	{
	const bool yes_no			= (mb_choices == (BASICFACTORY::uq_yes | BASICFACTORY::uq_no));
	const bool ok_only		= (mb_choices == BASICFACTORY::uq_ok);
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
	const char * infile_name = NULL;

	if (argc > 2)
		{
		wcerr << "Sorry, this program can accept at most one command" << endl;
		wcerr << "line argument, which must be the name of a NEXUS" << endl;
		wcerr << "data file." << endl;
		wcerr << endl;
		exit(0);
		}

	else if (argc > 1)
		{
                  //string instr = argv[1];
                  //wstring winstr = L"";
                  //std::copy( instr.begin(), instr.end(), winstr.begin() );
		infile_name = argv[1];//instr.c_str();
		}

	BASICFACTORY BASICFACTORY;
	BASICFACTORY.Run(infile_name);

	return 0;
	}
