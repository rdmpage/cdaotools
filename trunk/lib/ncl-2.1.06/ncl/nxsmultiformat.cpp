//	Copyright (C) 1999-2003 Paul O. Lewis and Mark T. Holder
//
//	This file is part of NCL (Nexus Class Library) version 2.0.
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


/* The phylip parser is based on code from PHYLIP which is:
 * version 3.6. (c) Copyright 1993-2004 by the University of Washington.
 * Written by Joseph Felsenstein, Akiko Fuseki, Sean Lamont, Andrew Keeffe,
 * Mike Palczewski, Doug Buxton and Dan Fineman. Permission is granted to
 * copy and use this program provided no fee is charged for it and provided
 * that this copyright notice is not removed.
 */


/*
 * This file is a phylip to NEXUS converter that consists of code from PHYLIP
 * 3.6.5 (see copyright above) tweaked by Mark Holder to output NEXUS.
 *
 * This file was created by concatenating the headers, and .c files:
 *	phylip.h, 
 *	seq.h,
 *	discrete.h 
 *	phylip.c
 *	seq.c, 
 *	pars.c, and 
 *	discrete.c concatenated 
 *	followed by removal of code that is unused in this simple program, and 
 * the addition of routines for printing out NEXUS.
 */


#include <cassert>
#include <fstream>
#include <algorithm>
#include "ncl/nxsmultiformat.h"
#include "ncl/nxsstring.h"

const unsigned MAX_BUFFER_SIZE = 0x80000;







const wchar_t* gFormatNames[] = {	L"nexus",
					L"dnafasta",
					L"aafasta",
					L"rnafasta",
					L"dnaphylip",
					L"rnaphylip",
					L"aaphylip",
					L"discretephylip",
					L"dnaphylipinterleaved",
					L"rnaphylipinterleaved",
					L"aaphylipinterleaved",
					L"discretephylipinterleaved",
					L"dnarelaxedphylip",
					L"rnarelaxedphylip",
					L"aarelaxedphylip",
					L"discreterelaxedphylip",
					L"dnarelaxedphylipinterleaved",
					L"rnarelaxedphylipinterleaved",
					L"aarelaxedphylipinterleaved",
					L"discreterelaxedphylipinterleaved",
					L"phyliptree",
					L"relaxedphyliptree",
							};
const unsigned gNumFormats = 22;
const unsigned PHYLIP_NMLNGTH = 10;

std::vector<std::wstring> MultiFormatReader::getFormatNames()
	{
	std::vector<std::wstring> v(gNumFormats);
	for (unsigned i = 0; i < gNumFormats; ++i)
		{
		v[i] = std::wstring(gFormatNames[i]);
		}
	return v;
	}

FileToCharBuffer::FileToCharBuffer(std::wistream & instream)
	:prevChar('\n'),
	inf(instream),
	pos(0),
	totalSize(0),
	lineNumber(1),
	prevNewlinePos(0),
	buffer(0L)
	{
	std::streampos s = inf.tellg();
	inf.seekg (0, std::ios::end);
	std::streampos e = inf.tellg();
	if (e <= s) 
		{
		inbuffer = 0;
		remaining = 0;
		return;
		}
	inf.seekg(s);
	totalSize = static_cast<unsigned>(e - s);
	inbuffer = std::min(MAX_BUFFER_SIZE, totalSize);
	remaining = totalSize - inbuffer;
	buffer = new wchar_t [inbuffer];
	inf.read(buffer, inbuffer);
	const wchar_t c = current();
	if (c == 13)
		{
		++lineNumber;
		prevNewlinePos = position();
		}
	else if (c == 10)
		{
		if (prev() != 13) 
			++lineNumber;
		prevNewlinePos = position();
		}
	}
	
bool FileToCharBuffer::refillBuffer(unsigned offset)
	{
	if (remaining  == 0)
		return false;
	if (offset == 0)
		prevChar = buffer[inbuffer-1];
	inbuffer = std::min(inbuffer - offset, remaining);
	remaining -= inbuffer;
	inf.read(buffer + offset, inbuffer);
	pos = offset;
	return true;
	}


MultiFormatReader::DataFormatType MultiFormatReader::formatNameToCode(const std::wstring &s)
	{
	std::wstring l(s);
	NxsString::to_lower(l);
	int ind = NxsString::index_in_array(l, gFormatNames, gNumFormats);
	if (ind < 0)
		return UNSUPPORTED_FORMAT;
	assert(ind < UNSUPPORTED_FORMAT);
	return MultiFormatReader::DataFormatType(ind);
	}
	


/* Assumes that `contents` was returned from readFileToMemory() has been called 
	with `inf` and the `len` refers the size of the buffer allocated by 
	readFileToMemory 
*/
bool  MultiFormatReader::readFastaSequences(
	FileToCharBuffer & ftcb,
	const NxsDiscreteDatatypeMapper &dm, 
	std::list<std::wstring> & taxaNames,
	std::list<NxsDiscreteStateRow> & matList,
	size_t & longest)
	{
	wchar_t * contents = ftcb.buffer;
	assert(contents);
	NxsString err;
	for (;;)
		{
		if (ftcb.current() == '>' && ( ftcb.prev() == '\n' ||  ftcb.prev() == '\r'))
			{
			std::wstring n;
			if (!ftcb.advance())
				break;
			for (;;)
				{
				char c = ftcb.current();
				if (c == '\n' || c == '\r')
					break;
				n.append(1, c);
				if (!ftcb.advance())
					break;
				}
			taxaNames.push_back(n);
			matList.push_back(NxsDiscreteStateRow());
			if (!ftcb.advance())
				break;
			NxsDiscreteStateRow & row = *(matList.rbegin());
			row.reserve(longest);
			for (;;)
				{
				char c = ftcb.current();
				if (c == '>' && (ftcb.prev() == '\n' || ftcb.prev() == '\r'))
					break;
				if (isgraph(c))
					{
					int stateCode = dm.GetStateCodeStored(c);
					if (stateCode == NXS_INVALID_STATE_CODE)
						{
						err << L"Illegal state code \"" << c << L"\" found when reading character " << row.size() << L" for taxon " << n;
						throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
						}
					row.push_back(stateCode);
					}
				if (!ftcb.advance())
					break;
				}
			longest = std::max(longest, row.size());
			}
		else if (!ftcb.advance())
			break;
		}
	// pad with missing data to make even rows
	std::list<NxsDiscreteStateRow>::iterator sIt = matList.begin();
	bool allSameLength = true;
	for (; sIt != matList.end(); ++sIt)
		{
		NxsDiscreteStateRow & row = *sIt;
		if (row.size() < longest)
			{
			allSameLength = false;
			break;
			}
		}
	return allSameLength;
	}

std::wstring  MultiFormatReader::readPhylipName(FileToCharBuffer & ftcb, unsigned i, bool relaxedNames)
	{
	NxsString err;
	std::wstring n;
	if (relaxedNames)
		{
		do {
			n.append(1,ftcb.current());
			if (!ftcb.advance())
				{
				err << L"End of file found when reading the name of taxon " << i+1 << L", \"" << n << L"\"";
				throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());	
				}
			}
		while (isgraph(ftcb.current()));
		while (!isgraph(ftcb.current()))
			{
			if (!ftcb.advance())
				{
				err << L"End of file found when expecting the beginning of the data for taxon " << i+1 << L", \"" << n << L"\"";
				throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());	
				}
			}
		}
	else
		{
		std::wstring ws;
		for (unsigned letter = 0; letter < PHYLIP_NMLNGTH; ++letter)
			{
			char c = ftcb.current();
			if (isgraph(c))
				{
				n.append(ws);
				n.append(1,c);
				ws.clear();
				}
			else
				ws.append(1, c);
			if (!ftcb.advance())
				{
				err << L"End of file found when reading the name for taxon " << i+1 << L", \"" << n << L"\"";
				throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());	
				}
			}
		}
	return n;
	}

void  MultiFormatReader::readPhylipData(
	FileToCharBuffer & ftcb,
	const NxsDiscreteDatatypeMapper &dm, 
	std::list<std::wstring> & taxaNames,
	std::list<NxsDiscreteStateRow> & matList,
	const unsigned n_taxa,
	const unsigned n_char,
	bool relaxedNames)
	{
	assert(n_taxa > 0 && n_char > 0);
	NxsString err;
	matList.clear();
	matList.assign(n_taxa, NxsDiscreteStateRow(n_char, NXS_INVALID_STATE_CODE));
	std::list<NxsDiscreteStateRow>::iterator mIt = matList.begin();
	while (!isgraph(ftcb.current()))
		{
		if (!ftcb.advance())
			goto funcExit;
		}
	
	for (unsigned i = 0; i < n_taxa; ++i) 
		{
		std::wstring n = readPhylipName(ftcb, i, relaxedNames);
		taxaNames.push_back(n);
		assert(mIt != matList.end());
		NxsDiscreteStateRow & row = *mIt++;
		for (unsigned j = 0; j < n_char; ++j)
			{
			bool readChar = false;
			for (;;)
				{
				const wchar_t c = ftcb.current();
				if (isgraph(c)) 
					{
					if (isdigit(c))// I don't know why PHYLIP allows digits in the midst of the sequence, but it seems to.
						{
						err << L"Number encountered (and ignored) within sequence for taxon " << n;
						NexusWarn(err, NxsReader::PROBABLY_INCORRECT_CONTENT_WARNING, ftcb.position(), ftcb.line(), ftcb.column());
						err.clear();
						}
					else
						{
						const int stateCode = dm.GetStateCodeStored(c);
						if (stateCode == NXS_INVALID_STATE_CODE)
							{
							if (c == '.')
								{
								if (i == 0)
									{
									err << L"Illegal match character state code  \".\" found in the first taxon for character " << j + 1 ;
									throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
									}
								NxsDiscreteStateRow & firstRow = *(matList.begin());
								row[j] = firstRow.at(j);
								}
							else
								{
								err << L"Illegal state code \"" << c << L"\" found when reading site " << j + 1 << L" for taxon " << n;
								throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
								}
							}
						else
							row[j] = stateCode;
						readChar = true;
						}
					}
				if (!ftcb.advance())
					goto funcExit;
				if (readChar)
					break;
				}
			}
		char f = ftcb.current();
		while (f != '\r' && f != '\n')
			{
			if (isgraph(f))
				{
				err << L"Sequence longer than " << n_char << L" found for taxon " << n << L". The character \""<< f << L"\" was found, and will be ignored. If the file position of this error corresponds to sequences for the next taxon in the matrix, then that is an indication that the sequences for taxon " << n << L" are too short.";
				NexusWarn(err, NxsReader::PROBABLY_INCORRECT_CONTENT_WARNING, ftcb.position(), ftcb.line(), ftcb.column());
				err.clear();
				}
			if (!ftcb.advance())
				goto funcExit;
			f = ftcb.current();
			}
		while (!isgraph(ftcb.current()))
			{
			if (!ftcb.advance())
				goto funcExit;
			}
		}
	funcExit:
		if (matList.size() != n_taxa)
			{
			err << L"Unexpected end of file.\nExpecting data for " << n_taxa << L" taxa, but only found data for " << matList.size();
			throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
			}
		const NxsDiscreteStateRow & lastRow = *matList.rbegin();
		if (lastRow.size() != n_char)
			{
			err << L"Unexpected end of file.\nExpecting " << n_char << L" characters for taxon " <<  *(taxaNames.rbegin()) << L", but only found " << lastRow.size() << L" characters.";
			throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
			}
	}


void  MultiFormatReader::readInterleavedPhylipData(
	FileToCharBuffer & ftcb,
	const NxsDiscreteDatatypeMapper &dm, 
	std::list<std::wstring> & taxaNames,
	std::list<NxsDiscreteStateRow> & matList,
	const unsigned n_taxa,
	const unsigned n_char,
	bool relaxedNames)
	{
	assert(n_taxa > 0 && n_char > 0);
	NxsString err;
	matList.clear();
	matList.assign(n_taxa, NxsDiscreteStateRow(n_char, NXS_INVALID_STATE_CODE));
	std::list<NxsDiscreteStateRow>::iterator mIt = matList.begin();
	unsigned startCharIndex = 0;
	unsigned endCharIndex = n_char;
	while (!isgraph(ftcb.current()))
		{
		if (!ftcb.advance())
			goto funcExit;
		}
	while (startCharIndex < n_char)
		{
		for (unsigned i = 0; i < n_taxa; ++i) 
			{
			if (startCharIndex == 0)
				{
				std::wstring n = readPhylipName(ftcb, i, relaxedNames);
				taxaNames.push_back(n);
				}
			if (i == 0)
				mIt = matList.begin();
			assert(mIt != matList.end());
			NxsDiscreteStateRow & row = *mIt++;
			unsigned j = startCharIndex;
			for (;;)
				{
				const wchar_t c = ftcb.current();
				if (isgraph(c)) 
					{
					if (j >= endCharIndex)
						{
						if (i == 0)
							{
							err << L"Too many characters were found for the taxon " << *(taxaNames.begin());
							throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
							}
						else
							{
							std::list<std::wstring>::const_iterator nIt = taxaNames.begin();
							for (unsigned q = 0; q < i ; ++q)
								++nIt;
							err << L"Illegal character \"" << c << L"\" found, after all of the data for this interleave page has been read for the taxon " << *nIt;
							throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
							}
						}
					if (isdigit(c))// I don't know why PHYLIP allows digits in the midst of the sequence, but it seems to.
						{
						std::list<std::wstring>::const_iterator nIt = taxaNames.begin();
						for (unsigned q = 0; q < i ; ++q)
							++nIt;
						err << L"Number encountered (and ignored) within sequence for taxon " << *nIt;
						NexusWarn(err, NxsReader::PROBABLY_INCORRECT_CONTENT_WARNING, ftcb.position(), ftcb.line(), ftcb.column());
						err.clear();
						}
					else
						{
						const int stateCode = dm.GetStateCodeStored(c);
						if (stateCode == NXS_INVALID_STATE_CODE)
							{
							if (c == '.')
								{
								if (i == 0)
									{
									err << L"Illegal match character state code  \".\" found in the first taxon for character " << j + 1 ;
									throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
									}
								NxsDiscreteStateRow & firstRow = *(matList.begin());
								row[j] = firstRow.at(j);
								}
							else 
								{
								std::list<std::wstring>::const_iterator nIt = taxaNames.begin();
								for (unsigned q = 0; q < i ; ++q)
									++nIt;
								err << L"Illegal state code \"" << c << L"\" found when reading site " << j + 1 << L" for taxon " << *nIt;
								throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
								}
							}
						else
							row[j] = stateCode;
						j++;
						}
					}
				else if (c == '\r' || c == '\n')
					{
					if (i == 0)
						endCharIndex = j;
					else if (j != endCharIndex)
						{
						std::list<std::wstring>::const_iterator nIt = taxaNames.begin();
						for (unsigned q = 0; q < i ; ++q)
							++nIt;
						err << L"Expecting " << endCharIndex -  startCharIndex << L"characters  in this interleave page (based on the number of characters in the first taxon), but only found " << j - startCharIndex << L" for taxon " << *nIt;
						throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
						}
					break;
					}
				if (!ftcb.advance())
					goto funcExit;
				}
			while (!isgraph(ftcb.current()))
				{
				if (!ftcb.advance())
					goto funcExit;
				}
			}
		startCharIndex = endCharIndex;
		endCharIndex = n_char;
		}
	funcExit:
		if (matList.size() != n_taxa)
			{
			err << L"Unexpected end of file.\nExpecting data for " << n_taxa << L" taxa, but only found data for " << matList.size();
			throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
			}
		const NxsDiscreteStateRow & lastRow = *matList.rbegin();
		if (lastRow.size() != n_char)
			{
			err << L"Unexpected end of file.\nExpecting " << n_char << L" characters for taxon " <<  *(taxaNames.rbegin()) << L", but only found " << lastRow.size() << L" characters.";
			throw NxsException(err, ftcb.position(), ftcb.line(), ftcb.column());
			}
	}

void MultiFormatReader::addTaxaNames(const std::list<std::wstring> & taxaNames, NxsTaxaBlockAPI * taxa)
	{
	assert(taxa);
	std::list<std::wstring>::const_iterator nIt = taxaNames.begin();
	for (; nIt != taxaNames.end(); ++nIt)
		taxa->AddTaxonLabel(*nIt);
	}

void MultiFormatReader::moveDataToMatrix(std::list<NxsDiscreteStateRow> & matList,  NxsDiscreteStateMatrix &mat)
	{
	mat.clear();
	mat.resize(matList.size());
	NxsDiscreteStateMatrix::iterator dIt = mat.begin();
	std::list<NxsDiscreteStateRow>::iterator sIt = matList.begin();
	for (; sIt != matList.end(); ++sIt, ++dIt)
		{
		NxsDiscreteStateRow & source = *sIt;
		NxsDiscreteStateRow & dest = *dIt;
		dest.swap(source);
		}
	}

void  MultiFormatReader::moveDataToDataBlock(const std::list<std::wstring> & taxaNames, std::list<NxsDiscreteStateRow> & matList, const unsigned nchar, NxsDataBlock * dataB)
	{
	assert(dataB);
	NxsString d;
	d << L"Dimensions ntax = " << matList.size() << L" nchar = " << nchar << L" ; ";
	std::wistringstream fakeDimStream(d);
	NxsToken fakeDimToken(fakeDimStream);
	NxsString newTaxLabel(L"NewTaxa");
	NxsString ntaxLabel(L"NTax");
	NxsString ncharLabel(L"NChar");
	dataB->HandleDimensions(fakeDimToken, newTaxLabel, ntaxLabel, ncharLabel);

	assert(dataB->taxa);
	addTaxaNames(taxaNames, dataB->taxa);

	moveDataToMatrix(matList, dataB->discreteMatrix);
	}

void  MultiFormatReader::moveDataToUnalignedBlock(const std::list<std::wstring> & taxaNames, std::list<NxsDiscreteStateRow> & matList, NxsUnalignedBlock * uB)
	{
	assert(uB);
	NxsString d;
	d << L"Dimensions NewTaxa ntax = " << matList.size() << L" ; ";
	std::wistringstream fakeDimStream(d);
	NxsToken fakeDimToken(fakeDimStream);
	uB->HandleDimensions(fakeDimToken);

	assert(uB->taxa);
	addTaxaNames(taxaNames, uB->taxa);

	moveDataToMatrix(matList, uB->uMatrix);
	}

void  MultiFormatReader::readFastaFile(std::wistream & inf, NxsCharactersBlock::DataTypesEnum dt)
	{
	NxsString blockID(L"DATA");
	NxsBlock *nb = cloneFactory.GetBlockReaderForID(blockID, this, NULL);
	assert(nb);
	if (!nb)
		return;
	nb->SetNexus(this);

	NxsDataBlock * dataB = static_cast<NxsDataBlock *>(nb); // this should be safe because we know that the PublicNexusReader has a DataBlock assigned to "DATA" -- unless the caller has replaced that clone template (gulp)
	FileToCharBuffer ftcb(inf);
	if (ftcb.buffer)
		{
		dataB->Reset();
		dataB->datatype = dt;
		dataB->ResetSymbols();
		dataB->gap = '-';
		NxsPartition dtParts;
		std::vector<NxsCharactersBlock::DataTypesEnum> dtv;
		dataB->CreateDatatypeMapperObjects(dtParts, dtv);
		
		const NxsDiscreteDatatypeMapper * dm = dataB->GetDatatypeMapperForChar(0);
		
		std::list<std::wstring> taxaNames;
		std::list<NxsDiscreteStateRow> matList;
		size_t longest = 0;
		bool aligned = true;
		try {
			aligned = readFastaSequences(ftcb, *dm, taxaNames, matList, longest);
			}
		catch (...)
			{
			cloneFactory.BlockError(dataB);
			throw;
			}
		
		if (aligned)
			{
			moveDataToDataBlock(taxaNames, matList, longest, dataB);
			BlockReadHook(blockID, dataB);
			}
		else
			{
			cloneFactory.BlockError(dataB);
			blockID.assign(L"UNALIGNED");
			NxsBlock * nub = cloneFactory.GetBlockReaderForID(blockID, this, NULL);
			if (!nub)
				{
				assert(nub);
				return;
				}
			nub->SetNexus(this);

			NxsUnalignedBlock * unalignedB = static_cast<NxsUnalignedBlock *>(nub); // this should be safe because we know that the PublicNexusReader has a DataBlock assigned to "DATA" -- unless the caller has replaced that clone template (gulp)
			unalignedB->Reset();
			unalignedB->datatype = dt;
			unalignedB->ResetSymbols();
			unalignedB->ResetDatatypeMapper();
			moveDataToUnalignedBlock(taxaNames, matList, unalignedB);
			BlockReadHook(blockID, unalignedB);
			}
		}
	else
		{
		cloneFactory.BlockError(dataB);
		NxsString err;
		err << L"No Data read -- file appears to be empty";
		this->NexusError(err, 0, -1, -1);
		}
	}

void  MultiFormatReader::ReadFilepath(const char* filepath, DataFormatType format)
	{
          std::string temp( filepath );
                                std::wstring fp = L"";
                                std::copy( temp.begin(), temp.end(), fp.begin() );

	if (format == NEXUS_FORMAT)
		{
		NxsReader::ReadFilepath(filepath);
		}
	else 
		{
		std::wifstream inf;
		try{
			inf.open(filepath, std::ios::binary);
			if (!inf.good())
				{
				NxsString err;
                                err << L"Could not open the file \"" << fp << L"\"";
				this->NexusError(err, 0, -1, -1);
				}
			}
		catch (...)
			{
			NxsString err;
			err << '\"' << fp <<L"\" does not refer to a valid file." ;
			this->NexusError(err, 0, -1, -1);
			}
		if (format == FASTA_DNA_FORMAT)
			readFastaFile(inf, NxsCharactersBlock::dna);
		else if (format == FASTA_RNA_FORMAT)
			readFastaFile(inf, NxsCharactersBlock::rna);
		else if (format == FASTA_AA_FORMAT)
			readFastaFile(inf, NxsCharactersBlock::protein);
		else if (format == PHYLIP_DNA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::dna, false, false);
		else if (format == PHYLIP_RNA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::rna, false, false);
		else if (format == PHYLIP_AA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::protein, false, false);
		else if (format == PHYLIP_DISC_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::standard, false, false);
		else if (format == INTERLEAVED_PHYLIP_DNA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::dna, false, true);
		else if (format == INTERLEAVED_PHYLIP_RNA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::rna, false, true);
		else if (format == INTERLEAVED_PHYLIP_AA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::protein, false, true);
		else if (format == INTERLEAVED_PHYLIP_DISC_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::standard, false, true);
		else if (format == RELAXED_PHYLIP_DNA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::dna, true, false);
		else if (format == RELAXED_PHYLIP_RNA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::rna, true, false);
		else if (format == RELAXED_PHYLIP_AA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::protein, true, false);
		else if (format == RELAXED_PHYLIP_DISC_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::standard, true, false);
		else if (format == INTERLEAVED_RELAXED_PHYLIP_DNA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::dna, true, true);
		else if (format == INTERLEAVED_RELAXED_PHYLIP_RNA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::rna, true, true);
		else if (format == INTERLEAVED_RELAXED_PHYLIP_AA_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::protein, true, true);
		else if (format == INTERLEAVED_RELAXED_PHYLIP_DISC_FORMAT)
			readPhylipFile(inf, NxsCharactersBlock::standard, true, true);
		else if (format == RELAXED_PHYLIP_TREE_FORMAT)
			readPhylipTreeFile(inf, true);
		else if (format == PHYLIP_TREE_FORMAT)
			readPhylipTreeFile(inf, false);
		else
			{
			NxsString m;
			m << L"The file " << fp << L" is not in a supported format.";
			NexusError(m, 0, -1, -1);
			return;
			}
		PostExecuteHook();
		}
	}
// More tolerant than strict PHYLIP (tolerates any amount of whitespace before or
// between ntax and nchar.  
// throws a NxsException if the header cannot be read.
// returns the file position.
unsigned MultiFormatReader::readPhylipHeader(std::wistream & inf, unsigned & ntax, unsigned & nchar)
	{
	if (inf.good())
		inf >> ntax;
	if (inf.good())
		inf >> nchar;
	if (!inf.good() || ntax == 0 || nchar == 0)
		{
		NxsString err(L"Expecting the file to start with the number of taxa then the number of characters.");
		throw NxsException(err, 0, -1, -1);
		}
	return (unsigned) inf.tellg();
	}

void MultiFormatReader::readPhylipTreeFile(std::wistream & inf, bool relaxedNames)
	{
	NxsString blockID(L"TREES");
	NxsBlock *nb = cloneFactory.GetBlockReaderForID(blockID, this, NULL);
	assert(nb);
	if (!nb)
		return;
	nb->SetNexus(this);

	/* this should be safe because we know that the PublicNexusReader has a 
		NxsTreesBlock assigned to "TREES" -- unless the caller has replaced that 
		clone template (gulp)
	*/
	NxsTreesBlock * treesB = static_cast<NxsTreesBlock *>(nb); 
	NxsString err;
	try {
		treesB->Reset();
		NxsToken inTokens(inf);
		treesB->ReadPhylipTreeFile(inTokens);
		if (!relaxedNames)
			{
			const NxsTaxaBlockAPI * taxa = treesB->GetTaxaBlockPtr(0L);
			if (!taxa)
				{
				err << L"No taxa found in tree description (which probably means that no tree was found).";
				throw NxsException(err, inTokens);
				}
			const std::vector<std::wstring> l = taxa->GetAllLabels();
			for (std::vector<std::wstring>::const_iterator lIt = l.begin(); lIt != l.end(); ++lIt)
				{
				if (lIt->length() > PHYLIP_NMLNGTH)
					{
					err << L"The taxon label " << *lIt << L" has more than the allowed number of charcters (" << PHYLIP_NMLNGTH << ')';
					throw NxsException(err);
					}
				}
			}
		BlockReadHook(blockID, treesB);
		}
	catch (...)
		{
		cloneFactory.BlockError(nb);
		throw;
		}
	}

/* if this returns NULL, then the read failed and gLogMessage will contain
	and error message.
*/
void MultiFormatReader::readPhylipFile(std::wistream & inf, NxsCharactersBlock::DataTypesEnum dt, bool relaxedNames, bool interleaved)
	{
	NxsString blockID(L"DATA");
	NxsBlock *nb = cloneFactory.GetBlockReaderForID(blockID, this, NULL);
	assert(nb);
	if (!nb)
		return;
	nb->SetNexus(this);
	/* this should be safe because we know that the PublicNexusReader has a 
		DataBlock assigned to "DATA" -- unless the caller has replaced that 
		clone template (gulp)
	*/
	NxsDataBlock * dataB = static_cast<NxsDataBlock *>(nb); 
		
	try {
		dataB->Reset();
		dataB->datatype = dt;
		dataB->ResetSymbols();
		dataB->gap = '-';
		NxsPartition dtParts;
		std::vector<NxsCharactersBlock::DataTypesEnum> dtv;
		dataB->CreateDatatypeMapperObjects(dtParts, dtv);
		
		const NxsDiscreteDatatypeMapper * dm = dataB->GetDatatypeMapperForChar(0);
		assert(dm);
		unsigned ntax, nchar;
		unsigned headerLen = readPhylipHeader(inf, ntax, nchar);
		FileToCharBuffer ftcb(inf);
		ftcb.totalSize += headerLen;
		if (ftcb.buffer)
			{
			std::list<std::wstring> taxaNames;
			std::list<NxsDiscreteStateRow> matList;
			if (interleaved)
				readInterleavedPhylipData(ftcb, *dm, taxaNames, matList, ntax, nchar, relaxedNames);
			else
				readPhylipData(ftcb, *dm, taxaNames, matList, ntax, nchar, relaxedNames);
			moveDataToDataBlock(taxaNames, matList, nchar, dataB);
			BlockReadHook(blockID, dataB);
			}
		}
	catch (...)
		{
		cloneFactory.BlockError(nb);
		throw;
		}
}
