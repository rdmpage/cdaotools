//	Copyright (C) 1999-2003 Paul O. Lewis
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
#include <iomanip>
#include <climits>
#include <cstdlib>

#include "ncl/nxsdistancesblock.h"

#include "ncl/nxsreader.h"
using namespace std;


void NxsDistancesBlock::WriteFormatCommand(std::wostream &out) const
	{
	out << L"    FORMAT Missing = " << missing << L" Triangle = Lower Diagonal;\n";
	}

void NxsDistancesBlock::WriteMatrixCommand(std::wostream &out) const
	{
	if (taxa == NULL)
		return;
	unsigned width = taxa->GetMaxTaxonLabelLength();
	const unsigned ntaxTotal = taxa->GetNTax();
	out << L"MATRIX";
	int prec = out.precision(10);
	for (unsigned i = 0; i < ntaxTotal; i++)
		{
		const std::wstring currTaxonLabel = NxsString::GetEscaped(taxa->GetTaxonLabel(i));
		out << '\n' << currTaxonLabel;
		unsigned currTaxonLabelLen = (unsigned)currTaxonLabel.size();
		unsigned diff = width - currTaxonLabelLen;
		for (unsigned k = 0; k < diff+5; k++)
			out << ' ';
		for (unsigned j = 0; j< i; j++)
			{
			if (IsMissing(i,j))
				out << ' ' << missing << L"         ";
			else
				out << ' '<< GetDistance(i, j);
			}
		out << L" 0.0";
		}
	out << L";\n";
	out.precision(prec);
	}
void NxsDistancesBlock::WriteAsNexus(std::wostream &out) const
	{
	out << L"BEGIN DISTANCES;\n";
	WriteBasicBlockCommands(out);
	if (nchar > 0)
		out << L"    DIMENSIONS NChar = " << nchar << L";\n";
	WriteFormatCommand(out);
	WriteMatrixCommand(out);
	WriteSkippedCommands(out);
	out << L"END;\n";
	}


/*----------------------------------------------------------------------------------------------------------------------
|	See Reset() for defaults
*/
NxsDistancesBlock::NxsDistancesBlock(
  NxsTaxaBlockAPI *t)	/* the NxsTaxaBlockAPI that will keep track of taxon labels */
  : NxsBlock(), 
  NxsTaxaBlockSurrogate(t, NULL)
	{
	id = L"DISTANCES";
	Reset();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Deletes `matrix' and `taxonPos' arrays.
*/
NxsDistancesBlock::~NxsDistancesBlock()
	{
	Reset();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called when DIMENSIONS command needs to be parsed from within the DISTANCES block. Deals with everything after the 
|	token DIMENSIONS up to and including the semicolon that terminates the DIMENSIONS command.
*/
void NxsDistancesBlock::HandleDimensionsCommand(
  NxsToken &token)	/* the token used to read from `in' */
	{
	nchar = 0;
	unsigned ntaxRead = 0;
	for (;;)
		{
		token.GetNextToken();
		if (token.Equals(L"NEWTAXA"))
			newtaxa = true;
		else if (token.Equals(L"NTAX")) 
			{
			DemandEquals(token, L"after NTAX in DIMENSIONS command");
			ntaxRead = DemandPositiveInt(token, L"NTAX");
			}
		else if (token.Equals(L"NCHAR")) 
			{
			DemandEquals(token, L"in DIMENSIONS command");
			nchar = DemandPositiveInt(token, L"NCHAR");
			}
		else if (token.Equals(L";"))
			break;
		}
	if (newtaxa)
		{
		if (ntaxRead == 0)
			{
			errormsg = L"DIMENSIONS command must have an NTAX subcommand when the NEWTAXA option is in effect.";
			throw NxsException(errormsg, token);
			}
		expectedNtax = ntaxRead;
		AssureTaxaBlock(createImpliedBlock, token, L"Dimensions");
		if (!createImpliedBlock)
			{
			taxa->Reset();
			if (nexusReader)
				nexusReader->RemoveBlockFromUsedBlockList(taxa);
			}
		taxa->SetNtax(expectedNtax);
		}
	else
		{
		AssureTaxaBlock(false, token, L"Dimensions");
		const unsigned ntaxinblock = taxa->GetNumTaxonLabels();
		if (ntaxinblock == 0)
			{
			errormsg = L"A TAXA block must be read before character data, or the DIMENSIONS command must use the NEWTAXA.";
			throw NxsException(errormsg, token);
			}
		if (ntaxinblock < ntaxRead)
			{
			errormsg = L"NTAX in ";
			errormsg << id << L" block must be less than or equal to NTAX in TAXA block\nNote: one circumstance that can cause this error is \nforgetting to specify NTAX in DIMENSIONS command when \na TAXA block has not been provided";
			throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
			}
		expectedNtax = (ntaxRead == 0 ? ntaxinblock : ntaxRead);;
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called when FORMAT command needs to be parsed from within the DISTANCES block. Deals with everything after the 
|	token FORMAT up to and including the semicolon that terminates the FORMAT command.
*/
void NxsDistancesBlock::HandleFormatCommand(
  NxsToken &token)	/* the token used to read from `in' */
	{
	for (;;)
		{
		token.GetNextToken();
		if (token.Equals(L";"))
			break;
		if (token.Equals(L"TRIANGLE"))
			{
			DemandEquals(token, L"after TRIANGLE");
			token.GetNextToken();
			if (token.Equals(L"LOWER"))
				triangle = NxsDistancesBlockEnum(lower);
			else if (token.Equals(L"UPPER"))
				triangle = NxsDistancesBlockEnum(upper);
			else if (token.Equals(L"BOTH"))
				triangle = NxsDistancesBlockEnum(both);
			else
				{
				errormsg = L"Expecting UPPER, LOWER, or BOTH but found ";
				errormsg += token.GetToken();
				errormsg += L" instead";
				throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
				}
			}
		else if (token.Equals(L"DIAGONAL"))
			diagonal = true;
		else if (token.Equals(L"NODIAGONAL"))
			diagonal = false;
		else if (token.Equals(L"LABELS"))
			labels = true;
		else if (token.Equals(L"NOLABELS"))
			labels = false;
		else if (token.Equals(L"INTERLEAVE"))
			interleave = true;
		else if (token.Equals(L"NOINTERLEAVE"))
			interleave = false;
		else if (token.Equals(L"MISSING"))
			{
			DemandEquals(token, L"after MISSING");
			token.GetNextToken();
			if (token.GetTokenLength() != 1 || isdigit(token.GetTokenReference()[0]))
				{
				errormsg = L"Missing data symbol specified (";
				errormsg += token.GetToken();
				errormsg += L") is invalid (must be a single character)";
				throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
				}
			missing = token.GetTokenReference()[0];
			}
		else
			{
			errormsg = L"Token specified (";
			errormsg += token.GetToken();
			errormsg += L") is an invalid subcommand for the FORMAT command";
			throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
			}
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called from within HandleMatrix, this function is used to deal with interleaved matrices. It is called once for 
|	each pass through the taxa. The local variable `jmax' records the number of columns read in the current interleaved 
|	page and is used to determine the offset used for j in subsequent pages.
*/
bool NxsDistancesBlock::HandleNextPass(
  NxsToken &token,	/* the token we are using for reading the data file */
  unsigned &offset,	/* the offset */
  vector<unsigned> & fileMatrixCmdOrderToTaxInd, 
  set<unsigned> & taxIndsRead)
	{
	unsigned jmax = 0; 
	bool done = false;

	unsigned i_first = 0;
	if (triangle == NxsDistancesBlockEnum(lower))
		i_first = offset;
	unsigned i_last = expectedNtax;
	errormsg.clear();
	for (unsigned i = i_first; i < i_last; i++)
		{
		// Deal with taxon label if provided. Here are the four situations we need to deal with:
		//   newtaxa  (offset > 0)  handled by
		//      0           0         case 1
		//      0           1         case 1
		//      1           0         case 2
		//      1           1         case 1
		//
		if (labels && (!newtaxa || offset > 0))
			{
			// Case 1: Expecting taxon labels, and also expecting them to already be in taxa
			//
			do
				{
				token.SetLabileFlagBit(NxsToken::newlineIsToken);
				token.GetNextToken();
				}
			while(token.AtEOL());

			try
				{
				// Look up position of taxon in NxsTaxaBlockAPI list
				//
				unsigned k = taxa->FindTaxon(token.GetToken());
				if (k != i && triangle != NxsDistancesBlockEnum(lower))
					{
					errormsg << L"Taxon " << token.GetToken() << L" was not expected in the DISTANCES matrix.\nTaxa should be in the same order as in the Taxon block";
					throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
					}

				// Array taxonPos is initialized to UINT_MAX and filled in as taxa are encountered
				//
				if (fileMatrixCmdOrderToTaxInd[i] == UINT_MAX)
					{
					fileMatrixCmdOrderToTaxInd[i] = k;
					if (taxIndsRead.count(k) > 0)
						{
						errormsg << L"Taxon " << token.GetToken() << L" was encountered more than one time in the Distances Matrix.";
						throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
						}
					taxIndsRead.insert(k);
					}
				else if (fileMatrixCmdOrderToTaxInd[i] != k)
					{
					errormsg << L"Taxon labeled " << token.GetToken() << L" is out of order compared to previous interleave pages";
					throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
					}
				}
			catch (NxsTaxaBlock::NxsX_NoSuchTaxon)
				{
				errormsg = L"Could not find ";
				errormsg += token.GetToken();
				errormsg += L" among taxa previously defined";
				throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
				}
			}

		else if (labels && newtaxa)
			{
			// Case 2: Expecting taxon labels, and also expecting taxa block to be empty
			//
			do
				{
				token.SetLabileFlagBit(NxsToken::newlineIsToken);
				token.GetNextToken();
				}
			while(token.AtEOL());
			const NxsString t(token.GetToken().c_str());
			taxa->AddTaxonLabel(t);
			fileMatrixCmdOrderToTaxInd[i] = i;
			taxIndsRead.insert(i);
			}

		// Now deal with the row of distance values
		//
		unsigned true_j = 0;
		for (unsigned j = 0; j < expectedNtax; j++)
			{
			if (i == expectedNtax - 1)
				{
				if (j == expectedNtax - 1)
					done = true;
				if (true_j == expectedNtax - 1 || (!diagonal && triangle == NxsDistancesBlockEnum(upper)))
					{
					done = true;
					break;
					}
				}
			if (!diagonal && triangle == NxsDistancesBlockEnum(lower) && j == expectedNtax - offset - 1)
				{
				done = true;
				break;
				}

			token.SetLabileFlagBit(NxsToken::newlineIsToken);
			token.GetNextToken();

			if (token.AtEOL())
				{
				if (j > jmax)
					{
					jmax = j;
					if (!diagonal && triangle == NxsDistancesBlockEnum(upper) && i >= offset)
						jmax++;
					if (interleave && triangle == NxsDistancesBlockEnum(upper))
						i_last = jmax + offset;
					}
				break;
				}

			true_j = j + offset;
			if (triangle == NxsDistancesBlockEnum(upper) && i > offset)
				true_j += (i - offset);
			if (!diagonal && triangle == NxsDistancesBlockEnum(upper) && i >= offset)
				true_j++;

			if (true_j == expectedNtax)
				{
				errormsg = L"Too many distances specified in row just read in";
				throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
				}

			wstring t = token.GetToken();
			unsigned corrected_i = fileMatrixCmdOrderToTaxInd.at(i);
			unsigned corrected_j = true_j;
			if (triangle == NxsDistancesBlockEnum(lower))
				corrected_j = fileMatrixCmdOrderToTaxInd.at(true_j);
			if (corrected_i == UINT_MAX || corrected_j == UINT_MAX)
				{
				errormsg = L"Illegal internal row number for taxon in Distance Matrix.";
				throw NxsNCLAPIException(errormsg, token);
				}
			if (token.GetTokenLength() == 1 && t[0] == missing)
				SetMissing(corrected_i, corrected_j);
			else
				SetDistance(corrected_i, corrected_j, wcstod(t.c_str(), NULL));
			}
		}
	offset += jmax;
	return done;
	}

void NxsDistancesBlock::CopyDistancesContents(const NxsDistancesBlock &other)
	{
	expectedNtax = other.expectedNtax;
	nchar = other.nchar;
	diagonal = other.diagonal;
	interleave = other.interleave;
	labels = other.labels;
	triangle = other.triangle;
	missing = other.missing;
	matrix = other.matrix;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called when MATRIX command needs to be parsed from within the DISTANCES block. Deals with everything after the 
|	token MATRIX up to and including the semicolon that terminates the MATRIX command.
*/
void NxsDistancesBlock::HandleMatrixCommand(
  NxsToken &token)	/* the token used to read from `in' */
	{
	errormsg.clear();
	if (expectedNtax == 0 || taxa == NULL)
		{
		AssureTaxaBlock(false, token, L"Matrix");
		expectedNtax = taxa->GetNumTaxonLabels();
		}
	if (expectedNtax == 0)
		{
		errormsg = L"MATRIX command cannot be read if NTAX is zero";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}

	if (triangle == NxsDistancesBlockEnum(both) && !diagonal)
		{
		errormsg = L"Cannot specify NODIAGONAL and TRIANGLE=BOTH at the same time";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}
	if (newtaxa)
		taxa->Reset();

	vector<unsigned> fileMatrixCmdOrderToTaxInd(expectedNtax, UINT_MAX);
	set<unsigned> taxIndsRead;
	unsigned nTaxInTaxBlock = taxa->GetNumTaxonLabels();
	if (nTaxInTaxBlock < expectedNtax)
		{
		errormsg << L"NTAX in " << id << L" block must be less than or equal to NTAX in TAXA block\nNote: one circumstance that can cause this error is \nforgetting to specify NTAX in DIMENSIONS command when \na TAXA block has not been provided";
		throw NxsException(errormsg, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
		}
	NxsDistanceDatumRow row(nTaxInTaxBlock);
	matrix.assign(nTaxInTaxBlock, row);
	unsigned offset = 0;
	for (;;)
		{
		if (HandleNextPass(token, offset, fileMatrixCmdOrderToTaxInd, taxIndsRead))
			break;
		}
	DemandEndSemicolon(token, L"MATRIX");
	}

/*----------------------------------------------------------------------------------------------------------------------
|	This function provides the ability to read everything following the block name (which is read by the NEXUS object)
|	to the end or endblock statement. Characters are read from the input stream in. Overrides the abstract virtual 
|	function in the base class.
*/
void NxsDistancesBlock::Read(
  NxsToken &token)	/* the token used to read from `in' */
	{
	isEmpty = false;

	DemandEndSemicolon(token, L"BEGIN DISTANCES");

	for (;;)
		{
		token.GetNextToken();
		NxsBlock::NxsCommandResult res = HandleBasicBlockCommands(token);
		if (res == NxsBlock::NxsCommandResult(STOP_PARSING_BLOCK))
			return;
		if (res != NxsBlock::NxsCommandResult(HANDLED_COMMAND))
			{
			if (token.Equals(L"DIMENSIONS"))
				HandleDimensionsCommand(token);
			else if (token.Equals(L"FORMAT"))
				HandleFormatCommand(token);
			else if (token.Equals(L"TAXLABELS"))
				HandleTaxLabels(token);
			else if (token.Equals(L"MATRIX"))
				HandleMatrixCommand(token);
			else
				SkipCommand(token);
			}
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	This function outputs a brief report of the contents of this taxa block. Overrides the abstract virtual function in 
|	the base class.
*/
void NxsDistancesBlock::Report(
  std::wostream &out) NCL_COULD_BE_CONST /* the output stream to which to write the report */
	{
	const unsigned ntaxTotal = taxa->GetNumTaxonLabels();

	out << endl;
	out << id << L" block contains ";
	if (ntaxTotal == 0)
		{
		out << L"no taxa" << endl;
		}
	else if (ntaxTotal == 1)
		out << L"one taxon" << endl;
	else
		out << ntaxTotal << L" taxa" << endl;

	if (IsLowerTriangular())
		out << L"  Matrix is lower-triangular" << endl;
	else if (IsUpperTriangular())
		out << L"  Matrix is upper-triangular" << endl;
	else
		out << L"  Matrix is rectangular" << endl;

	if (IsInterleave())
		out << L"  Matrix is interleaved" << endl;
	else 
		out << L"  Matrix is non-interleaved" << endl;

	if (IsLabels())
		out << L"  Taxon labels provided" << endl;
	else
		out << L"  No taxon labels provided" << endl;

	if (IsDiagonal())
		out << L"  Diagonal elements specified" << endl;
	else 
		out << L"  Diagonal elements not specified" << endl;

	out << L"  Missing data symbol is " << missing << endl;

	if (expectedNtax == 0)
		return;

	out.setf(ios::fixed, ios::floatfield);
	out.setf(ios::showpoint);
	for (unsigned i = 0; i < ntaxTotal; i++)
		{
		if (labels)
			out << setw(20) << taxa->GetTaxonLabel(i);
		else
			out << L"        ";

		for (unsigned j = 0; j < ntaxTotal; j++)
			{
			if (triangle == NxsDistancesBlockEnum(upper) && j < i)
				out << setw(12) << L" ";
			else if (triangle == NxsDistancesBlockEnum(lower) && j > i)
				continue;
			else if (!diagonal && i == j)
				{
				out << setw(12) << L" ";
				}
			else if (IsMissing(i, j))
				out << setw(12) << missing;
			else
				out << setw(12) << GetDistance(i, j);
			}

		out << endl;
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Flushes taxonLabels and sets ntax to 0 in preparation for reading a new TAXA block.
|	`triangle' to `NxsDistancesBlockEnum::lower', 
|	`missing' to '?', 
|	`labels' and `diagonal' to true, 
|	`newtaxa' and `interleave' to false, 
|	`expectedNtax' and `nchar' to 0. 
|	and clears the matrix.
*/
void NxsDistancesBlock::Reset()
	{
	NxsBlock::Reset();
	ResetSurrogate();
	matrix.clear();
	expectedNtax        = 0;
	nchar       = 0;
	diagonal    = true;
	labels      = true;
	interleave  = false;
	missing     = '?';
	triangle    = NxsDistancesBlockEnum(lower);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the value of nchar.
*/
unsigned NxsDistancesBlock::GetNchar() NCL_COULD_BE_CONST 
	{
	return nchar;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the value of the (`i', `j')th element of `matrix'. Assumes `i' and `j' are both in the range [0..`ntax') 
|	and the distance stored at `matrix[i][j]' is not missing. Also assumes `matrix' is not NULL.
*/
double NxsDistancesBlock::GetDistance(
  unsigned i,	/* the row */
  unsigned j) const /* the column */
	{
	return GetCell(i,j).value;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the value of `missing'.
*/
char NxsDistancesBlock::GetMissingSymbol() NCL_COULD_BE_CONST 
	{
	return missing;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the value of `triangle'.
*/
unsigned NxsDistancesBlock::GetTriangle() NCL_COULD_BE_CONST 
	{
	return triangle;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if the value of `triangle' is NxsDistancesBlockEnum(both), false otherwise.
*/
bool NxsDistancesBlock::IsRectangular() NCL_COULD_BE_CONST 
	{
	return (triangle == NxsDistancesBlockEnum(both) ? true : false);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if the value of triangle is NxsDistancesBlockEnum(upper), false otherwise.
*/
bool NxsDistancesBlock::IsUpperTriangular() NCL_COULD_BE_CONST 
	{
	return (triangle == NxsDistancesBlockEnum(upper) ? true : false);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if the value of triangle is NxsDistancesBlockEnum(lower), false otherwise.
*/
bool NxsDistancesBlock::IsLowerTriangular() NCL_COULD_BE_CONST 
	{
	return (triangle == NxsDistancesBlockEnum(lower) ? true : false);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the value of diagonal.
*/
bool NxsDistancesBlock::IsDiagonal() NCL_COULD_BE_CONST 
	{
	return diagonal;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the value of interleave.
*/
bool NxsDistancesBlock::IsInterleave() NCL_COULD_BE_CONST 
	{
	return interleave;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the value of labels.
*/
bool NxsDistancesBlock::IsLabels() NCL_COULD_BE_CONST 
	{
	return labels;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if the (`i',`j')th distance is missing. Assumes `i' and `j' are both in the range [0..`ntax') and 
|	`matrix' is not NULL.
*/
bool NxsDistancesBlock::IsMissing(
  unsigned i,	/* the row */
  unsigned j) const	/* the column */
	{
	return (bool)(GetCell(i,j).missing);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Sets the value of the (`i',`j')th matrix element to `d' and `missing' to false . Assumes `i' and `j' are both in 
|	the range [0..`ntax') and `matrix' is not NULL.
*/
void NxsDistancesBlock::SetDistance(
  unsigned i,	/* the row */
  unsigned j,	/* the column */
  double d)		/* the distance value */
	{
	NxsDistanceDatum & c =  GetCell(i, j);
	c.value = d;
	c.missing = false;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Sets the value of the (`i', `j')th `matrix' element to missing. Assumes `i' and `j' are both in the range 
|	[0..`ntax') and `matrix' is not NULL.
*/
void NxsDistancesBlock::SetMissing(
  unsigned i,	/* the row */
  unsigned j)	/* the column */
	{
	NxsDistanceDatum & c =  GetCell(i, j);
	c.missing = 1;
	c.value = 0.0;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Sets `nchar' to `n'.
*/
void NxsDistancesBlock::SetNchar(
  unsigned n)	/* the number of characters */
	{
	nchar = n;
	}

NxsDistancesBlock *NxsDistancesBlockFactory::GetBlockReaderForID(const std::wstring & idneeded, NxsReader *reader, NxsToken *)
	{
	if (reader == NULL || idneeded != L"DISTANCES")
		return NULL;
	NxsDistancesBlock * nb  = new NxsDistancesBlock(NULL);
	nb->SetCreateImpliedBlock(true);
	nb->SetImplementsLinkAPI(true);
	return nb;
	}
