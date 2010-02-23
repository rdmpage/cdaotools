//	Copyright (C) 2007-2008 Mark T. Holder
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

/*******************************************************************************
 * This file contains code for 4 executables:
 *		NEXUSnormalizer, NEXUSvalidator, NEXUSinspector, and NEX_us2ml
 *	with conditional compilation used to determine the behavior.
 *
 *		* NEXUSnormalizer - writes a NEXUS version of the file with consistent
 *			ordering of blocks and commands. Ideally 2 equivalent files will 
 *			produce the same normalized output. This version of tthe program is
 *			less ambitious. The goal is to be able to run (for any valid NEXUS 
 *			in.nex file):
 *				$ NEXUSnormalizer in.nex > outOrig.nex
 *				$ NEXUSnormalizer outOrig.nex > outSecond.nex
 *				$ diff outOrig.nex outSecond.nex
 *			and find no differences.
 *		* NEXUSvalidator - reports errors and warnings to stderr. Invalid files
 *			cause exit with a non-zero return code
 *		* NEXUSinspector - writes a brief report of every block parsed
 *		* NEXUS_us2ml - writes a nexml version of the input (partially 
 *			implemented, note that the code to write nexml is in us2ml.cpp).
 * See the processFilepath() function for an example of how to deal with NCL
 *	to read a file using the new MultiFormatReader class. When the file
 *	is correctly read, the processContent() function is called.
 * 
 * All other code has to do with reading command line arguments and other
 * 	user-interface concerns.
 */
#include "ncl/ncl.h"
#include "ncl/nxsblock.h"
#include "ncl/nxspublicblocks.h"
#include "ncl/nxsmultiformat.h"

//#include "ncl/nxscxxdiscretematrix.h"

#if defined(TO_NEXML_CONVERTER) && TO_NEXML_CONVERTER
void	writeAsNexml(PublicNexusReader & nexusReader, wostream & os);
#endif

long gStrictLevel = 2;
bool gTreesViaInMemoryStruct = true;
long gInterleaveLen = -1;
bool blocksReadInValidation = false;

enum ProcessActionsEnum
	{
	REPORT_BLOCKS,
	OUTPUT_NORMALIZED_NEXUS,
	OUTPUT_NEXML,
	VALIDATE_ONLY
	};
	
	
void processContent(PublicNexusReader & nexusReader, wostream *os, ProcessActionsEnum currentAction);
MultiFormatReader * instantiateReader();

#	if defined(MULTIFILE_NEXUS_READER) && MULTIFILE_NEXUS_READER
	MultiFormatReader * gNexusReader = NULL;
#	endif


void reportNexusStats(const PublicNexusReader & nexusReader, wostream *os)
{
	if (!os)
		return;

	const unsigned nTaxaBlocks = nexusReader.GetNumTaxaBlocks();
	*os <<  nTaxaBlocks << L" taxa block(s) read.\n";
	for (unsigned t = 0; t < nTaxaBlocks; ++t)
		{
		NxsTaxaBlock * tb = nexusReader.GetTaxaBlock(t);
		*os << L"Taxa block #" << t + 1 << L".\n";
		tb->Report(*os);
		const unsigned nCharBlocks = nexusReader.GetNumCharactersBlocks(tb);
		*os <<  nCharBlocks << L" Characters/Data block(s) read that link to this Taxa block.\n";
		for (unsigned i = 0; i < nCharBlocks; ++i)
			{
			NxsCharactersBlock * cb = nexusReader.GetCharactersBlock(tb, i);

			//NxsCXXDiscreteMatrix mat(*cb, true);

			*os << L"Character block #" << i + 1 << L" for this Taxa block.\n";
			cb->Report(*os);
			const unsigned nAssumpBlocks = nexusReader.GetNumAssumptionsBlocks(cb);
			*os <<  nAssumpBlocks << L" Assumptions block(s) read that link to this Characters block.\n";
			for (unsigned j= 0; j < nAssumpBlocks; ++j)
				{
				NxsAssumptionsBlock * ab = nexusReader.GetAssumptionsBlock(cb, j);
				*os << L"Assumptions block #" << j + 1 << L" for this Characters block.\n";
				ab->Report(*os);
				}
			}
		const unsigned nTreesBlocks = nexusReader.GetNumTreesBlocks(tb);
		*os <<  nTreesBlocks << L" Trees/Data block(s) read that link to this Taxa block.\n";
		for (unsigned i = 0; i < nTreesBlocks; ++i)
			{
			NxsTreesBlock * cb = nexusReader.GetTreesBlock(tb, i);
			*os << L"Trees block #" << i + 1 << L" for this Taxa block.\n";
			cb->Report(*os);
			const unsigned nAssumpBlocks = nexusReader.GetNumAssumptionsBlocks(cb);
			*os <<  nAssumpBlocks << L" Assumptions block(s) read that link to this Trees block.\n";
			for (unsigned j= 0; j < nAssumpBlocks; ++j)
				{
				NxsAssumptionsBlock * ab = nexusReader.GetAssumptionsBlock(cb, j);
				*os << L"Assumptions block #" << j + 1 << L" for this Trees block.\n";
				ab->Report(*os);
				}
			}
		const unsigned nAssumpBlocks = nexusReader.GetNumAssumptionsBlocks(tb);
		*os <<  nAssumpBlocks << L" Assumptions block(s) read that link to this Taxa block.\n";
		for (unsigned j= 0; j < nAssumpBlocks; ++j)
			{
			NxsAssumptionsBlock * ab = nexusReader.GetAssumptionsBlock(tb, j);
			*os << L"Assumptions block #" << j + 1 << L" for this Taxa block.\n";
			ab->Report(*os);
			}
		const unsigned nDistancesBlocks = nexusReader.GetNumDistancesBlocks(tb);
		*os <<  nDistancesBlocks << L" Distances block(s) read that link to this Taxa block.\n";
		for (unsigned j= 0; j < nDistancesBlocks; ++j)
			{
			NxsDistancesBlock * ab = nexusReader.GetDistancesBlock(tb, j);
			*os << L"Distances block #" << j + 1 << L" for this Taxa block.\n";
			ab->Report(*os);
			}
		const unsigned nUnalignedBlocks = nexusReader.GetNumUnalignedBlocks(tb);
		*os <<  nUnalignedBlocks << L" Unaligned block(s) read that link to this Taxa block.\n";
		for (unsigned j= 0; j < nUnalignedBlocks; ++j)
			{
			NxsUnalignedBlock * ab = nexusReader.GetUnalignedBlock(tb, j);
			*os << L"Unaligned block #" << j + 1 << L" for this Taxa block.\n";
			ab->Report(*os);
			}
		*os << "\n\n";
		}
	const unsigned nUnknown = nexusReader.GetNumUnknownBlocks();
	*os <<  nUnknown << L" private block(s) read.\n";
	for (unsigned t = 0; t < nUnknown; ++t)
		{
		NxsStoreTokensBlockReader * ub = nexusReader.GetUnknownBlock(t);
		*os << L"Private block #" << t + 1 << L" is a " << ub->GetID() << L" block.\n";
		}
}


////////////////////////////////////////////////////////////////////////////////
// Takes NxsReader that has successfully read a file, and processes the
//	information stored in the reader. 
//
// The caller is responsibel for calling DeleteBlocksFromFactories() to clean
//	up (if the reader uses the factory API).
////////////////////////////////////////////////////////////////////////////////
void processContent(PublicNexusReader & nexusReader, wostream *os, ProcessActionsEnum currentAction)
	{
	BlockReaderList blocks = nexusReader.GetUsedBlocksInOrder();

	if (currentAction == REPORT_BLOCKS)
		reportNexusStats(nexusReader, os);
	else if (OUTPUT_NORMALIZED_NEXUS == currentAction && os)
		{
		*os << L"#NEXUS\n";
		for (BlockReaderList::const_iterator bIt = blocks.begin(); bIt != blocks.end(); ++bIt)
			{
			NxsBlock * b = *bIt;
			if (b)
				b->WriteAsNexus(*os);
			}
		}
	else if (OUTPUT_NEXML == currentAction && os)
		{
#		if defined(TO_NEXML_CONVERTER) && TO_NEXML_CONVERTER
			writeAsNexml(nexusReader, *os);
#		else
			cerr << L"Error nexml conversion not implemented\n";
			exit(1);
#		endif			
		}
	else if (VALIDATE_ONLY == currentAction)
		{
		if (!blocks.empty())
			blocksReadInValidation = true;
		}
	}


MultiFormatReader * instantiateReader()
{
	MultiFormatReader * nexusReader = new MultiFormatReader(-1, NxsReader::WARNINGS_TO_STDERR);
	if (gStrictLevel != 2)
		nexusReader->SetWarningToErrorThreshold((int)NxsReader::FATAL_WARNING + 1 - (int) gStrictLevel);
	NxsCharactersBlock * charsB = nexusReader->GetCharactersBlockTemplate();
	NxsDataBlock * dataB = nexusReader->GetDataBlockTemplate();
	charsB->SetAllowAugmentingOfSequenceSymbols(true);
	dataB->SetAllowAugmentingOfSequenceSymbols(true);
	if (gInterleaveLen > 0)
		{
		assert(charsB);
		charsB->SetWriteInterleaveLen(gInterleaveLen);
		dataB->SetWriteInterleaveLen(gInterleaveLen);
		}
	
	NxsTreesBlock * treesB = nexusReader->GetTreesBlockTemplate();
	assert(treesB);
	if (gStrictLevel < 2)
		treesB->SetAllowImplicitNames(true);
	treesB->SetWriteFromNodeEdgeDataStructure(gTreesViaInMemoryStruct);
	if (gStrictLevel < 2)
		{
		NxsStoreTokensBlockReader *storerB =  nexusReader->GetUnknownBlockTemplate();
		assert(storerB);
		storerB->SetTolerateEOFInBlock(true);  
		}
	return nexusReader;
}

////////////////////////////////////////////////////////////////////////////////
// Creates a NxsReader, and tries to read the file `filename`.  If the
//	read succeeds, then processContent will be called.
////////////////////////////////////////////////////////////////////////////////
void processFilepath(
	const char * filename, // name of the file to be read
	wostream *os, // output stream to use (NULL for no output). Not that cerr is used to report errors.
	MultiFormatReader::DataFormatType fmt, // enum indicating the file format to expect.
	ProcessActionsEnum currentAction) // enum that is passed on to processContent to indicate what should be done with the content of the file.
	{
	assert(filename);
	try
		{
		MultiFormatReader * nexusReader;
#		if defined(MULTIFILE_NEXUS_READER) && MULTIFILE_NEXUS_READER
			nexusReader = gNexusReader;
#		else
			nexusReader = instantiateReader();
#		endif

		cerr << "Executing" <<endl;
		try {
			nexusReader->ReadFilepath(filename, fmt);
#			if !defined(MULTIFILE_NEXUS_READER) ||  !MULTIFILE_NEXUS_READER
				processContent(*nexusReader, os, currentAction);
#			endif
			}
		catch(...) 
			{
			nexusReader->DeleteBlocksFromFactories();
			throw;
			}
#		if !defined(MULTIFILE_NEXUS_READER) ||  !MULTIFILE_NEXUS_READER
			nexusReader->DeleteBlocksFromFactories();
			delete nexusReader;
#		endif
		}
	catch (const NxsException &x)
		{
		wcerr << L"Error:\n " << x.msg << endl;
		if (x.line >=0)
			wcerr << L"at line " << x.line << L", column (approximately) " << x.col << L" (and file position "<< x.pos << L")" << endl;
		exit(2);
		}
	}

void readFilepathAsNEXUS(const char *filename, MultiFormatReader::DataFormatType fmt, ProcessActionsEnum currentAction)
	{
	cerr << "[Reading " << filename << "	 ]" << endl;
	try {
		wostream * outStream = 0L;
		if (currentAction != VALIDATE_ONLY)
			outStream = &wcout;
		processFilepath(filename, outStream, fmt, currentAction);
		}
	catch (...) 
		{
		cerr << "Normalizing of " << filename << " failed (with an exception)" << endl;
		exit(1);
		}
	}	

void readFilesListedIsFile(const char *masterFilepath, MultiFormatReader::DataFormatType fmt, ProcessActionsEnum currentAction)
	{
	ifstream masterStream(masterFilepath, ios::binary);
	if (masterStream.bad())
		{
		cerr << "Could not open " << masterFilepath << "." << endl;
		exit(3);
		}
	char filename[1024];
	while ((!masterStream.eof())  && masterStream.good())
		{
		masterStream.getline(filename, 1024);
		if (strlen(filename) > 0 && filename[0] != '#')
			readFilepathAsNEXUS(filename, fmt, currentAction);
		}
	}

void printHelp(wostream & out)
	{
#	if defined(JUST_VALIDATE_NEXUS) && JUST_VALIDATE_NEXUS
		out << L"NEXUSvalidator takes reads a file and exits with a success (return code 0) if the file is valid.\n";
#	elif defined(JUST_REPORT_NEXUS) && JUST_REPORT_NEXUS
		out << L"NEXUSinspector takes reads a file and writes a report of the content to standard out.\n";
#	else
#		if defined(MULTIFILE_NEXUS_READER) && MULTIFILE_NEXUS_READER
			out << L"NEXUSunion reads a series of NEXUS file and writes the union of all of their content to standard out (using the NEXUSnormalizer conventions of indentation and syntax).\n";
#		else
			out << L"NEXUSnormalizer takes reads a file and rewrites the file to standard out with consistent indentation and syntax.\n";
#		endif
# 	endif
	out << L"\nThe most common usage is simply:\n    NEXUSnormalizer <path to NEXUS file>\n";
	out << L"\nCommand-line flags:\n\n";
	out << L"    -h on the command line shows this help message\n\n";
	out << L"    -l<path> reads a file and treats each line of the file as a path to NEXUS file\n\n";
	out << L"    -s<non-negative integer> controls the NEXUS strictness level.\n";
	out << L"        the default level is equivalent to -s2 invoking the program with \n";
	out << L"        -s3 or a higher number will convert some warnings into fatal errors.\n";
	out << L"        Running with -s1 will cause the parser to accept dangerous constructs,\n";
	out << L"        and running with -s0 will cause the parser make every attempt to finish\n";
	out << L"        parsing the file (warning about very serious errors).\n\n";
	out << L"        Note that when -s0 strictness level is used, and the parser fails to\n";
	out << L"        finish, it will often be the result of an earlier error than the \n";
	out << L"        error that is reported in the last message.\n";
#	if defined(JUST_VALIDATE_NEXUS) && JUST_VALIDATE_NEXUS
		//passs
#	elif defined(JUST_REPORT_NEXUS) && JUST_REPORT_NEXUS
		//passs
#	else
		out << L"    -i<number> specifies the length of the interleaved pages to create\n";
#	endif
	out << L"    -f<format> specifies the input file format expected:\n";
	out << L"            -fnexus     NEXUS (this is also the default)\n";
	out << L"            -faafasta   Amino acid data in fasta\n";
	out << L"            -fdnafasta  DNA data in fasta\n";
	out << L"            -frnafasta  RNA data in fasta\n";
	out << L"        The complete list of format names that can follow the -f flag is:\n";
	std::vector<std::wstring> fmtNames =  MultiFormatReader::getFormatNames();
	for (std::vector<std::wstring>::const_iterator n = fmtNames.begin(); n != fmtNames.end(); ++n)
		{
		out << L"            "<< *n << L"\n";
		}
	}

int main(int argc, char *argv[])
	{
	MultiFormatReader::DataFormatType f(MultiFormatReader::NEXUS_FORMAT);
#	if defined(JUST_VALIDATE_NEXUS) && JUST_VALIDATE_NEXUS
		ProcessActionsEnum currentAction = VALIDATE_ONLY;
#	elif defined(JUST_REPORT_NEXUS) && JUST_REPORT_NEXUS
		ProcessActionsEnum currentAction = REPORT_BLOCKS;
#	elif defined(TO_NEXML_CONVERTER) && TO_NEXML_CONVERTER
		ProcessActionsEnum currentAction = OUTPUT_NEXML;
#	else
		ProcessActionsEnum currentAction = OUTPUT_NORMALIZED_NEXUS;
# 	endif

	for (int i = 1; i < argc; ++i)
		{
		const char * filepath = argv[i];
                wstring wfilepath = str_to_wstr( filepath );
//                string  sfilepath( filepath );
//                std::copy( sfilepath.begin(), sfilepath.end(), wfilepath.begin() );
		const unsigned slen = strlen(filepath);
		if (slen < 2 || filepath[0] != '-')
			continue;
		if (filepath[1] == 'h')
			printHelp(wcout);
		else if (filepath[1] == 's')
			{
			if ((slen == 2) || (!NxsString::to_long(wfilepath.c_str() + 2, &gStrictLevel)))
				{
				wcerr << L"Expecting an integer after -s\n" << endl;
				printHelp(wcerr);
				return 2;
				}
			}
#	if defined(JUST_VALIDATE_NEXUS) && JUST_VALIDATE_NEXUS
		//pass
#	elif defined(JUST_REPORT_NEXUS) && JUST_REPORT_NEXUS
		//pass
#	else
		else if (filepath[1] == 'i')
			{
			if ((slen == 2) || (!NxsString::to_long(wfilepath.c_str() + 2, &gInterleaveLen)) || gInterleaveLen < 1)
				{
				wcerr << L"Expecting a positive integer after -i\n" << endl;
				printHelp(wcerr);
				return 2;
				}
			}		
#	endif
		else if (filepath[1] == 'f')
			{
			f = MultiFormatReader::UNSUPPORTED_FORMAT;
			if (slen > 2)
				{
				std::string fmtName(filepath + 2, slen - 2);
                                std::wstring wfmtName = str_to_wstr( fmtName );
                                //std::copy( fmtName.begin(), fmtName.end(), wfmtName.begin() );
				f =  MultiFormatReader::formatNameToCode(wfmtName);
				}
			if (f == MultiFormatReader::UNSUPPORTED_FORMAT)
				{
				wcerr << "Expecting a format after -f\n" << endl;
				printHelp(wcerr);
				return 2;
				}
			}
		}

#	if defined(MULTIFILE_NEXUS_READER) && MULTIFILE_NEXUS_READER
		gNexusReader = instantiateReader();
		gNexusReader->cullIdenticalTaxaBlocks(true);
#	endif
	bool readfile = false;
	for (int i = 1; i < argc; ++i)
		{
		const char * filepath = argv[i];
		const unsigned slen = strlen(filepath);
		if (slen < 1)
			continue;
		if (strlen(filepath) > 2 && filepath[0] == '-' && filepath[1] == 'l')
			{
			readfile = true;
			readFilesListedIsFile(filepath+2, f, currentAction);
			}
		else if (filepath[0] != '-')
			{
			readfile = true;
			readFilepathAsNEXUS(filepath, f, currentAction);
			}
		}
#	if defined(MULTIFILE_NEXUS_READER) && MULTIFILE_NEXUS_READER
		if (gNexusReader)
			{
			processContent(*gNexusReader, &std::wcout, OUTPUT_NORMALIZED_NEXUS);
			gNexusReader->DeleteBlocksFromFactories();
			delete gNexusReader;
			}
#	endif

	if (!readfile)
		{
		wcerr << L"Expecting the path to NEXUS file as the only command line argument!\n" << endl;
		printHelp(wcerr);
		return 1;
		}
#	if defined(JUST_VALIDATE_NEXUS) && JUST_VALIDATE_NEXUS
		if (blocksReadInValidation)
			return  0;
		std::wcerr << L"No blocks read\n";
		return 1;
#	else
		return 0;
#	endif
	}

