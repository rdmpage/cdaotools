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

#include "ncl/nxsassumptionsblock.h"

#include "ncl/nxssetreader.h"
#include "ncl/nxsreader.h"
using namespace std;

class BogusToIndMapper: public NxsLabelToIndicesMapper
	{
	public:
		BogusToIndMapper()
			:queried(false)
			{}
		virtual ~BogusToIndMapper(){}
		virtual unsigned GetMaxIndex() const
			{
			return UINT_MAX;
			}
		virtual unsigned GetIndicesForLabel(const std::wstring &label, NxsUnsignedSet *) const
			{
			queried=true;
			NxsString e;
			e << L"The symbol " << label << L" was not recognized";
			throw NxsException(e);
			}
			
		virtual unsigned GetIndexSet(const std::wstring &, NxsUnsignedSet * ) const
			{
			return 0;
			}
		virtual bool AddNewIndexSet(const std::wstring &, const NxsUnsignedSet & )
			{
			return false;
			}
		virtual bool AddNewPartition(const std::wstring &, const NxsPartition & )
			{
			return false;
			}

		/// AppendNewLabel is only overloaded in Taxa and State LabelToIndexMappers
		virtual unsigned AppendNewLabel(std::wstring &label)
			{
			queried=true;
			NxsString e;
			e << L"The symbol " << label << L" was not recognized";
			throw NxsException(e);
			}
			
		std::vector<std::wstring> GetLabels() const
			{
			queried=true;
			NxsString e;
			throw NxsException(e);
			}
		mutable bool queried;
	};
	

void NxsTransformationManager::Reset()
	{
	standardTypeNames.clear();
	standardTypeNames.insert(L"UNORD");
	standardTypeNames.insert(L"ORD");
	standardTypeNames.insert(L"IRREV");
	standardTypeNames.insert(L"IRREV.UP");
	standardTypeNames.insert(L"IRREV.DOWN");
	standardTypeNames.insert(L"DOLLO");
	standardTypeNames.insert(L"DOLLO.UP");
	standardTypeNames.insert(L"DOLLO.DOWN");
	standardTypeNames.insert(L"STRAT");
	standardTypeNames.insert(L"SQUARED"); /* new in Mesquite */
	standardTypeNames.insert(L"LINEAR"); /* new in Mesquite */
	allTypeNames.clear();
	allTypeNames.insert(standardTypeNames.begin(), standardTypeNames.end());

	userTypeNames.clear();
	dblUserTypes.clear();
	intUserTypes.clear();
	dblWtSets.clear();
	intWtSets.clear();
	typeSets.clear();
	def_wtset.clear();
	def_typeset.clear();
	def_type.clear();
	}

void NxsTransformationManager::WriteUserType(std::wostream &out) const
	{
	if (dblUserTypes.empty() && intUserTypes.empty())
		return;
	for (std::map<std::wstring, NxsRealStepMatrix>::const_iterator csIt = dblUserTypes.begin(); csIt != dblUserTypes.end(); ++csIt)
		{
		out << L"    UserType ";
		out << NexusToken::EscapeString(csIt->first) << L" (Stepmatrix) = ";
		const NxsRealStepMatrix & p = csIt->second;
		const std::vector<std::wstring> & states = p.GetSymbols();
		const NxsRealStepMatrix::DblMatrix & mat = p.GetMatrix();
		const unsigned nStates = (const unsigned)states.size();
		out << nStates;
		out << L"\n    ";
		for (std::vector<std::wstring>::const_iterator sIt = states.begin(); sIt != states.end(); ++sIt)
			out << L"   "<< NxsString::GetEscaped(*sIt) ;
		NxsString n;
		std::ios::fmtflags prevflags = out.setf(std::ios::showpoint);
		for (unsigned i = 0; i < nStates; ++i)
			{
			out << L"\n    ";
			for (unsigned j = 0; j < nStates; ++j)
				{
				const double el = mat.at(i).at(j);
				if (i == j && el == 0.0)
					out << L"   .";
				else
					{
					n.clear();
					if (el == DBL_MAX)
						n += L"i";
					else
						n += el;
					out << L"   " << NxsString::GetEscaped(n); 
					}
				}
			}
		out.flags(prevflags);
		out << L";\n";
		}

	for (std::map<std::wstring, NxsIntStepMatrix>::const_iterator csIt = intUserTypes.begin(); csIt != intUserTypes.end(); ++csIt)
		{
		out << L"    UserType ";
		out << NexusToken::EscapeString(csIt->first) << L" (Stepmatrix) = ";
		const NxsIntStepMatrix & p = csIt->second;
		const std::vector<std::wstring> & states = p.GetSymbols();
		const NxsIntStepMatrix::IntMatrix & mat = p.GetMatrix();
		const unsigned nStates = (const unsigned)states.size();
		out << nStates;
		out << L"\n    ";
		for (std::vector<std::wstring>::const_iterator sIt = states.begin(); sIt != states.end(); ++sIt)
			out << L"   "<< NxsString::GetEscaped(*sIt) ;
		NxsString n;
		for (unsigned i = 0; i < nStates; ++i)
			{
			out << L"\n    ";
			for (unsigned j = 0; j < nStates; ++j)
				{
				const int el = mat.at(i).at(j);
				if (i == j && el == 0)
					out << L"   .";
				else
					{
					if (el == INT_MAX)
						n = L"i";
					else
						{
						n.clear();
						n += el;
						}
					out << L"   " << NxsString::GetEscaped(n); 
					}
				}
			}
		out << L";\n";
		}
	}

void NxsTransformationManager::WriteWtSet(std::wostream &out) const
	{
	if (dblWtSets.empty() && intWtSets.empty())
		return;
	const wchar_t* dtp = (def_wtset.empty() ? NULL : def_wtset.c_str());
	for (std::map<std::wstring, ListOfDblWeights>::const_iterator csIt = dblWtSets.begin(); csIt != dblWtSets.end(); ++csIt)
		{
		out << L"    WtSet ";
		if (NxsString::case_insensitive_equals(csIt->first.c_str(), dtp))
			out << L"* ";
		out << NexusToken::EscapeString(csIt->first) << L" =";
		const ListOfDblWeights & p = csIt->second;
		bool first = true;
		for (ListOfDblWeights::const_iterator pIt = p.begin(); pIt != p.end(); ++pIt)
			{
			const DblWeightToIndexSet & g = *pIt;
			if (!first)
				out << ',';
			out << L" \'" << g.first << L"' :";
			NxsSetReader::WriteSetAsNexusValue(g.second, out);
			first = false;
			}
		out << L";\n";
		}
	for (std::map<std::wstring, ListOfIntWeights>::const_iterator csIt = intWtSets.begin(); csIt != intWtSets.end(); ++csIt)
		{
		out << L"    WtSet ";
		if (NxsString::case_insensitive_equals(csIt->first.c_str(), dtp))
			out << L"* ";
		out << NexusToken::EscapeString(csIt->first) << L" =";
		const ListOfIntWeights & p = csIt->second;
		bool first = true;
		for (ListOfIntWeights::const_iterator pIt = p.begin(); pIt != p.end(); ++pIt)
			{
			const IntWeightToIndexSet & g = *pIt;
			if (!first)
				out << ',';
			out << L" \'" << g.first << L"' :";
			NxsSetReader::WriteSetAsNexusValue(g.second, out);
			first = false;
			}
		out << L";\n";
		}
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Returns -1 if `index` is not in `wtset`
*/
void NxsTransformationManager::SetDefaultTypeName(const std::wstring &n)
	{
	if (!(n.empty() || IsValidTypeName(n)))
		{
		NxsString e(n.c_str());
		e << L" is not the name of a known type (and therefore is not a valid default type)";
		throw NxsException(e);
		}
	def_type = n;
	}


bool NxsTransformationManager::IsEmpty() const
	{
	return (userTypeNames.empty() 
			&& dblWtSets.empty() 
			&& intWtSets.empty() 
			&& typeSets.empty() 
			&& (def_type.empty() || !NxsString::case_insensitive_equals(def_type.c_str(), L"ORD")));
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns -1 if `index` is not in `wtset`
*/
double NxsTransformationManager::GetWeightForIndex(const ListOfDblWeights & wtset, unsigned index)
	{
	for (ListOfDblWeights::const_iterator wIt = wtset.begin(); wIt != wtset.end(); ++wIt)
		{
		if (wIt->second.count(index) > 0)
			return wIt->first;
		}
	return -1.0;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns -1 if `index` is not in `wtset`
*/
int NxsTransformationManager::GetWeightForIndex(const ListOfIntWeights & wtset, unsigned index)
	{
	for (ListOfIntWeights::const_iterator wIt = wtset.begin(); wIt != wtset.end(); ++wIt)
		{
		if (wIt->second.count(index) > 0)
			return wIt->first;
		}
	return 1;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Raises an NxsNCLAPIException if the `n` is not a type name.
*/
bool NxsTransformationManager::IsIntType(const std::wstring & n) const
	{
	std::wstring capName(n.c_str());
	NxsString::to_upper(capName);
	if (standardTypeNames.count(capName) > 0)
		return true;
	if (intUserTypes.find(capName) != intUserTypes.end())
		return true;
	if (dblUserTypes.find(capName) != dblUserTypes.end())
		return false;
	NxsString errormsg = L"Type name ";
	errormsg << n << L" not found.";
	throw NxsNCLAPIException(errormsg);
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if `n` is the name of a known type (standard or user type) -- not case-sensitive.
*/
bool NxsTransformationManager::IsValidTypeName(const std::wstring & n) const
	{
	std::wstring capName(n.c_str());
	NxsString::to_upper(capName);
	return (allTypeNames.count(capName) > 0);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Raises an NxsNCLAPIException if the `n` is not a type name.
*/
bool NxsTransformationManager::IsStandardType(const std::wstring & n) const
	{
	std::wstring capName(n.c_str());
	NxsString::to_upper(capName);
	return (standardTypeNames.count(capName) > 0);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Raises an NxsNCLAPIException if the `n` is not a type name.
*/
const NxsIntStepMatrix & NxsTransformationManager::GetIntType(const std::wstring & n) const
	{
	std::wstring capName(n.c_str());
	NxsString::to_upper(capName);
	if (IsStandardType(n))
		throw NxsNCLAPIException(L"Standard (predefined) types cannot be fetched using GetIntType.");

	std::map<std::wstring, NxsIntStepMatrix>::const_iterator i = intUserTypes.find(capName);
	if (i != intUserTypes.end())
		return i->second;
	NxsString errormsg = L"Type name ";
	errormsg << n << L" not found.";
	throw NxsNCLAPIException(errormsg);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Raises an NxsNCLAPIException if the `n` is not a type name.
*/
const NxsRealStepMatrix & NxsTransformationManager::GetRealType(const std::wstring & n) const
	{
	std::wstring capName(n.c_str());
	NxsString::to_upper(capName);
	std::map<std::wstring, NxsRealStepMatrix>::const_iterator i = dblUserTypes.find(capName);
	if (i != dblUserTypes.end())
		return i->second;
	NxsString errormsg = L"Type name ";
	errormsg << n << L" not found.";
	throw NxsNCLAPIException(errormsg);
	}

const NxsIntStepMatrix::IntMatrix NxsTransformationManager::GetOrderedType(unsigned nStates)
	{
	NxsIntStepMatrix::IntVec v(nStates, 1);
	NxsIntStepMatrix::IntMatrix m(nStates, v);
	for (unsigned i = 0; i < nStates; ++i)
		m[i][i] = 0;
	return m;
	}

const NxsIntStepMatrix::IntMatrix NxsTransformationManager::GetUnorderedType(unsigned nStates)
	{
	NxsIntStepMatrix::IntVec v(nStates, 0);
	NxsIntStepMatrix::IntMatrix m(nStates, v);
	for (unsigned i = 0; i < nStates; ++i)
		{
		for (unsigned j = 0; j < nStates; ++j)
			{
			if (i > j)
				m[i][j] = j - i;
			else
				m[i][j] = i - j;
			}
		}
	return m;
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Raises an NxsNCLAPIException if the `name` is the name of a "standard" type.
|	Returns true if another user type was replaced.
*/
bool NxsTransformationManager::AddIntType(const std::wstring &name, const NxsIntStepMatrix &s)
	{
	std::wstring capName(name.c_str());
	NxsString::to_upper(capName);
	if (standardTypeNames.count(capName) > 0)
		{
		NxsString errormsg(name.c_str());
		errormsg << L" is a predefined type which cannot be redefined";
		throw NxsNCLAPIException(errormsg);
		}
	bool replacing = intUserTypes.find(capName) != intUserTypes.end();
	if (!replacing &&  dblUserTypes.find(capName) != dblUserTypes.end())
		{
		replacing = true;
		dblUserTypes.erase(capName);
		}
	intUserTypes.insert(pair<std::wstring, NxsIntStepMatrix>(capName, s));
	userTypeNames.insert(name);
	allTypeNames.insert(capName);
	return replacing;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Raises an NxsNCLAPIException if the `name` is the name of a "standard" type.
|	Returns true if another user type was replaced.
*/
bool NxsTransformationManager::AddRealType(const std::wstring &name, const NxsRealStepMatrix &s)
	{
	std::wstring capName(name.c_str());
	NxsString::to_upper(capName);
	if (standardTypeNames.count(capName) > 0)
		{
		NxsString errormsg(name.c_str());
		errormsg << L" is a predefined type which cannot be redefined";
		throw NxsNCLAPIException(errormsg);
		}
	bool replacing = dblUserTypes.find(capName) != dblUserTypes.end();
	if (!replacing &&  intUserTypes.find(capName) != intUserTypes.end())
		{
		replacing = true;
		intUserTypes.erase(capName);
		}
	dblUserTypes.insert(pair<std::wstring, NxsRealStepMatrix>(capName, s));
	userTypeNames.insert(capName);
	allTypeNames.insert(capName);
	return replacing;
	}

bool NxsTransformationManager::AddIntWeightSet(const std::wstring &name, const NxsTransformationManager::ListOfIntWeights &ws, bool isDefault)
	{
	std::wstring capName(name.c_str());
	NxsString::to_upper(capName);
	bool replacing = intWtSets.find(capName) != intWtSets.end();
	if (!replacing &&  dblWtSets.find(capName) != dblWtSets.end())
		{
		replacing = true;
		dblWtSets.erase(capName);
		}
	intWtSets[capName] = ws;
	if (isDefault)
		def_wtset = name;
	return replacing;
	}

bool NxsTransformationManager::AddRealWeightSet(const std::wstring &name, const NxsTransformationManager::ListOfDblWeights &ws, bool isDefault)
	{
	std::wstring capName(name.c_str());
	NxsString::to_upper(capName);
	bool replacing = dblWtSets.find(capName) != dblWtSets.end();
	if (!replacing &&  intWtSets.find(capName) != intWtSets.end())
		{
		replacing = true;
		intWtSets.erase(capName);
		}
	dblWtSets[capName] = ws;
	if (isDefault)
		{
		def_wtset = name;
		}
	return replacing;
	}
	
bool NxsTransformationManager::AddTypeSet(const std::wstring &name, const NxsPartition &ts, bool isDefault)
	{
	std::wstring capName(name.c_str());
	NxsString::to_upper(capName);
	bool replacing = typeSets.find(capName) != typeSets.end();
	typeSets[capName] = ts;
	if (isDefault)
		{
		def_typeset = name;
		}
	return replacing;
	}

std::set<std::wstring> NxsTransformationManager::GetWeightSetNames() const
	{
	std::set<std::wstring> s;
	std::map<std::wstring, ListOfDblWeights>::const_iterator dws = dblWtSets.begin();
	for (; dws != dblWtSets.end(); ++dws)
		s.insert(dws->first);
	std::map<std::wstring, ListOfIntWeights>::const_iterator iws = intWtSets.begin();
	for (; iws != intWtSets.end(); ++iws)
		s.insert(iws->first);
	return s;
	}

std::set<std::wstring> NxsTransformationManager::GetTypeSetNames() const
	{
	std::set<std::wstring> s;
	std::map<std::wstring, ListOfTypeNamesToSets>::const_iterator dws = typeSets.begin();
	for (; dws != typeSets.end(); ++dws)
		s.insert(dws->first);
	return s;
	}


void NxsAssumptionsBlock::AddCharPartition(const std::wstring & name, const NxsPartition &p)
	{
	charPartitions[name] = p;
	}
void NxsAssumptionsBlock::AddTaxPartition(const std::wstring & name, const NxsPartition &p)
	{
	taxPartitions[name] = p;
	}

void NxsAssumptionsBlock::AddTreePartition(const std::wstring & name, const NxsPartition &p)
	{
	treePartitions[name] = p;
	}

void NxsAssumptionsBlock::AddCodeSet(const std::wstring & name, const NxsPartition &p, bool asterisked)
	{
	codeSets[name] = p;
	if (asterisked)
		def_codeSet.assign(name.c_str());
	}

void NxsAssumptionsBlock::AddCodonPosSet(const std::wstring & name, const NxsPartition &p, bool asterisked)
	{
	codonPosSets[name] = p;
	if (asterisked)
		def_codonPosSet.assign(name.c_str());
	}

	
void NxsAssumptionsBlock::WriteOptions(std::wostream & out) const
	{
	const std::wstring d = transfMgr.GetDefaultTypeName();
	if ((!d.empty() && !NxsString::case_insensitive_equals(d.c_str(), L"ORD")) 
		|| gapsAsNewstate || polyTCountValue != POLY_T_COUNT_UNKNOWN)
		{
		out << L"    OPTIONS";
		if (!d.empty())
			out << L" DefType = " << NxsString::GetEscaped(d);
		if (gapsAsNewstate)
			out << L" GapMode = NewState";
		if (polyTCountValue == POLY_T_COUNT_MIN)
			out << L" PolyTCount = MinSteps";
		else if (polyTCountValue == POLY_T_COUNT_MAX)
			out << L" PolyTCount = MaxSteps";
		out << L";\n";
		}
	}
	
bool NxsAssumptionsBlock::HasAssumptionsBlockCommands() const
	{
	return (gapsAsNewstate 
			|| !transfMgr.IsEmpty()
			|| !exsets.empty() 
			||  polyTCountValue != POLY_T_COUNT_UNKNOWN);
	}
bool NxsAssumptionsBlock::HasSetsBlockCommands() const
	{
	return (!charsets.empty() || !taxsets.empty() || !treesets.empty() 
			||!charPartitions.empty() || !taxPartitions.empty() || !treePartitions.empty() );
	}
bool NxsAssumptionsBlock::HasCodonsBlockCommands() const
	{
	return (!codeSets.empty() || !codonPosSets.empty() || !codesMgr.IsEmpty());
	}

bool NxsAssumptionsBlock::CanReadBlockType(const NxsToken & token)
	{
	if (token.Equals(L"ASSUMPTIONS"))
		{
		id = L"ASSUMPTIONS";
		readAs = ASSUMPTIONS_BLOCK_READ;
		return true;
		}
	if (token.Equals(L"SETS"))
		{
		id = L"SETS";
		readAs = SETS_BLOCK_READ;
		return true;
		}
	if (token.Equals(L"CODONS"))
		{
		id = L"CODONS";
		readAs = CODONS_BLOCK_READ;
		return true;
		}
	return token.Equals(GetID());
	}



void NxsAssumptionsBlock::WriteAsNexus(std::wostream &out) const
	{
	NxsAssumptionsBlock *t = const_cast<NxsAssumptionsBlock *>(this); /*title switching*/
	const std::wstring ft = title;
	t->title.clear();
	NameOfAssumpBlockAsRead treatAs = this->readAs;
	if (treatAs == UNREAD_OR_GENERATED_BLOCK)
		{
		/*	Precedence order here is reverse the order of writing. This delays
			skipped blocks to the last among SETS, CODONS, and ASSUMPTIONS blocks.
		*/
		if (HasAssumptionsBlockCommands())
			treatAs = ASSUMPTIONS_BLOCK_READ;
		else if (HasCodonsBlockCommands())
			treatAs = CODONS_BLOCK_READ;
		else if (HasSetsBlockCommands())
			treatAs = SETS_BLOCK_READ;
		}
	try 
		{
		if (HasSetsBlockCommands())
			{
			if (treatAs == SETS_BLOCK_READ && !IsAutoGeneratedTitle())
				t->title = ft;
			out << L"BEGIN SETS;\n";
			WriteBasicBlockCommands(out);
			this->WriteTaxSet(out);
			this->WriteCharSet(out);
			this->WriteTreeSet(out);
			this->WriteTaxPartition(out);
			this->WriteCharPartition(out);
			this->WriteTreePartition(out);
			if (treatAs == SETS_BLOCK_READ)
				{
				WriteSkippedCommands(out);
				t->title.clear();
				}
			out << L"END;\n";
			}
		if (HasCodonsBlockCommands())
			{
			if (treatAs == CODONS_BLOCK_READ && !IsAutoGeneratedTitle())
				t->title = ft;
			out << L"BEGIN CODONS;\n";
			WriteBasicBlockCommands(out);
			this->codesMgr.WriteGeneticCode(out);
			this->WriteCodonPosSet(out);
			this->WriteCodeSet(out);
			if (treatAs == CODONS_BLOCK_READ)
				{
				WriteSkippedCommands(out);
				t->title.clear();
				}
			out << L"END;\n";
			}
		if (HasAssumptionsBlockCommands())
			{
			if (treatAs == ASSUMPTIONS_BLOCK_READ && !IsAutoGeneratedTitle())
				t->title = ft;
			out << L"BEGIN ASSUMPTIONS;\n";
			WriteBasicBlockCommands(out);
			this->WriteExSet(out);
			this->transfMgr.WriteUserType(out);
			this->transfMgr.WriteWtSet(out);
			this->transfMgr.WriteTypeSet(out);
			this->WriteOptions(out);
			if (treatAs == ASSUMPTIONS_BLOCK_READ)
				WriteSkippedCommands(out);
			out << L"END;\n";
			}
		}
	catch (...)
		{
		t->title =  ft;
		throw;
		}
	t->title =  ft;
	}


NxsCharactersBlockAPI * NxsAssumptionsBlock::GetCharBlockPtr(int *status)
	{
	if (status)
		*status = GetCharLinkStatus();
	return charBlockPtr;
	}
NxsTaxaBlockAPI * NxsAssumptionsBlock::GetTaxaBlockPtr(int *status)
	{
	if (status)
		*status = GetTaxaLinkStatus();
	return taxa;
	}
NxsTreesBlockAPI * NxsAssumptionsBlock::GetTreesBlockPtr(int *status)
	{
	if (status)
		*status = GetTreesLinkStatus();
	return treesBlockPtr;
	}

NxsAssumptionsBlockAPI	*NxsAssumptionsBlock::CreateNewAssumptionsBlock(NxsToken &token)
	{
	NxsAssumptionsBlockAPI * aba = NULL;
	if (nexusReader)
		{
		NxsString n(L"ASSUMPTIONS");
		NxsBlock * block = nexusReader->CreateBlockFromFactories(n, token, NULL);
		aba = static_cast<NxsAssumptionsBlockAPI *>(block);
		}
	if (aba == NULL)
		{
		NxsAssumptionsBlock *ab =  new NxsAssumptionsBlock(NULL);
		ab->SetImplementsLinkAPI(this->ImplementsLinkAPI());
		aba = ab;
		}
	aba->SetNexus(nexusReader);
	passedRefOfOwnedBlock = false;
	createdSubBlocks.push_back(aba);
	return aba;
	}

NxsAssumptionsBlockAPI	*NxsAssumptionsBlock::GetAssumptionsBlockForCharBlock(NxsCharactersBlockAPI *cb, NxsBlockLinkStatus status, NxsToken &token)
	{
	int cbstatus;
	NxsCharactersBlockAPI * thisCB = GetCharBlockPtr(&cbstatus);
	if (thisCB == NULL)
		{
		SetCharBlockPtr(cb, status);
		return this;
		}
	else if (cb == thisCB)
		{
		int f = cbstatus & NxsBlock::BLOCK_LINK_UNUSED_MASK;
		if (f == status)
			return this;	
		if (!(cbstatus & NxsBlock::BLOCK_LINK_USED))
			{
			if (cbstatus == BLOCK_LINK_UNINITIALIZED || cbstatus == BLOCK_LINK_UNKNOWN_STATUS)
				{
				SetCharLinkStatus(status);
				return this;
				}
			}
		else
			{
			/* return the same block for these two link statuses since they are both "safe" */
			if ((f | status) == (BLOCK_LINK_FROM_LINK_CMD | BLOCK_LINK_TO_ONLY_CHOICE))
				{
				charLinkStatus |= (BLOCK_LINK_FROM_LINK_CMD | BLOCK_LINK_TO_ONLY_CHOICE);
				return this;
				}
			}
		}
	for (VecAssumpBlockPtr::iterator bIt = createdSubBlocks.begin(); bIt != createdSubBlocks.end(); ++bIt)
		{
		NxsAssumptionsBlockAPI	* c = *bIt;
		if (c && cb == c->GetCharBlockPtr(&cbstatus) && (cbstatus & BLOCK_LINK_UNUSED_MASK) == status)
			return c;
		}
	NxsAssumptionsBlockAPI	*effBlock = CreateNewAssumptionsBlock(token);
	effBlock->SetCharBlockPtr(cb, status);
	return effBlock;
	}

NxsAssumptionsBlockAPI	*NxsAssumptionsBlock::GetAssumptionsBlockForTaxaBlock(NxsTaxaBlockAPI *cb, NxsBlockLinkStatus status, NxsToken &token)
	{
	int cbstatus;
	NxsTaxaBlockAPI * thisCB = GetTaxaBlockPtr(&cbstatus);
	if (thisCB == NULL)
		{
		SetTaxaBlockPtr(cb, status);
		return this;
		}
	else if (cb == GetTaxaBlockPtr(&cbstatus))
		{
		int f = cbstatus & NxsBlock::BLOCK_LINK_UNUSED_MASK;
		if (f == status)
			return this;	
		if (!(cbstatus & NxsBlock::BLOCK_LINK_USED))
			{
			if (cbstatus == BLOCK_LINK_UNINITIALIZED || cbstatus == BLOCK_LINK_UNKNOWN_STATUS)
				{
				SetTaxaLinkStatus(status);
				return this;
				}
			}
		else
			{
			/* return the same block for these two link statuses since they are both "safe" */
			if ((f | status) == (BLOCK_LINK_FROM_LINK_CMD | BLOCK_LINK_TO_ONLY_CHOICE))
				{
				taxaLinkStatus |= (BLOCK_LINK_FROM_LINK_CMD | BLOCK_LINK_TO_ONLY_CHOICE);
				return this;
				}
			}
		}
	for (VecAssumpBlockPtr::iterator bIt = createdSubBlocks.begin(); bIt != createdSubBlocks.end(); ++bIt)
		{
		NxsAssumptionsBlockAPI	* c = *bIt;
		if (c && cb == c->GetTaxaBlockPtr(&cbstatus) && (cbstatus & BLOCK_LINK_UNUSED_MASK) == status)
			return c;
		}
	NxsAssumptionsBlockAPI	*effBlock = CreateNewAssumptionsBlock(token);
	effBlock->SetTaxaBlockPtr(cb, status);
	return effBlock;
	}
NxsAssumptionsBlockAPI	*NxsAssumptionsBlock::GetAssumptionsBlockForTreesBlock(NxsTreesBlockAPI *cb, NxsBlockLinkStatus status, NxsToken &token)
	{
	int cbstatus;
	NxsTreesBlockAPI * thisCB = GetTreesBlockPtr(&cbstatus);
	if (thisCB == NULL)
		{
		SetTreesBlockPtr(cb, status);
		return this;
		}
	else if (cb == GetTreesBlockPtr(&cbstatus))
		{
		int f = cbstatus & NxsBlock::BLOCK_LINK_UNUSED_MASK;
		if (f == status)
			return this;	
		if (!(cbstatus & NxsBlock::BLOCK_LINK_USED))
			{
			if (cbstatus == BLOCK_LINK_UNINITIALIZED || cbstatus == BLOCK_LINK_UNKNOWN_STATUS)
				{
				SetTreesLinkStatus(status);
				return this;
				}
			}
		else
			{
			/* return the same block for these two link statuses since they are both "safe" */
			if ((f | status) == (BLOCK_LINK_FROM_LINK_CMD | BLOCK_LINK_TO_ONLY_CHOICE))
				{
				treesLinkStatus |= (BLOCK_LINK_FROM_LINK_CMD | BLOCK_LINK_TO_ONLY_CHOICE);
				return this;
				}
			}
		}
	for (VecAssumpBlockPtr::iterator bIt = createdSubBlocks.begin(); bIt != createdSubBlocks.end(); ++bIt)
		{
		NxsAssumptionsBlockAPI	* c = *bIt;
		if (c && cb == c->GetTreesBlockPtr(&cbstatus) && (cbstatus & BLOCK_LINK_UNUSED_MASK) == status)
			return c;
		}
	NxsAssumptionsBlockAPI	*effBlock = CreateNewAssumptionsBlock(token);
	effBlock->SetTreesBlockPtr(cb, status);
	return effBlock;
	}

NxsAssumptionsBlockAPI	*NxsAssumptionsBlock::GetAssumptionsBlockForCharTitle(const wchar_t*charTitle, NxsToken &token, const wchar_t*cmd)
	{
	if (!nexusReader)
		NxsNCLAPIException(L"No NxsReader when reading Assumptions block.");
	if (charTitle == NULL)
		{
		int cbstatus;
		NxsCharactersBlockAPI * thisCB = GetCharBlockPtr(&cbstatus);
		int ust = cbstatus & NxsBlock::BLOCK_LINK_UNUSED_MASK;
		if (thisCB != NULL && ust != BLOCK_LINK_UNINITIALIZED && ust != BLOCK_LINK_UNKNOWN_STATUS)
			return this;
		}
	unsigned ncb = 0;
	NxsCharactersBlockAPI * cb = nexusReader->GetCharBlockByTitle(charTitle, &ncb);
	NxsAssumptionsBlockAPI *effectiveB = NULL;
	if (cb == NULL)
		{
		if (charBlockPtr)
			{
			const NxsString	 t=charBlockPtr->GetID();
			if (t.empty())
				{
				if (charTitle == NULL)
					effectiveB = this;
				}
			else 
				{
				if (NxsString::case_insensitive_equals(charTitle, t.c_str()))
					effectiveB = this;
				}
			}
		if (effectiveB == NULL)
			{
			errormsg.clear();
			errormsg << L"A CHARACTERS (or DATA) block ";
			if (charTitle)
				errormsg << L"with the title " << NxsString::GetEscaped(charTitle);
			errormsg << L" must precede an " << id << L" block with a " <<  cmd << L" command.";
			errormsg << L"\n(If such a block exists, then this program may not be using an API for the NCL library that supports block linking).";
			throw NxsException(errormsg, token);
			}
		}
	else if (ncb > 1)
		{
		errormsg = L"Multiple CHARACTERS blocks have been encountered, but a ";
		errormsg += cmd;
		errormsg += L" command was found which does not specify which CHARACTERS block it uses.   The most recent CHARACTERS block will be used.";
		if (nexusReader)
			nexusReader->NexusWarnToken(errormsg, NxsReader::AMBIGUOUS_CONTENT_WARNING, token); 
		errormsg.clear();
		effectiveB = GetAssumptionsBlockForCharBlock(cb, NxsBlock::BLOCK_LINK_TO_MOST_RECENT, token);
		}
	else 
		{
		NxsBlockLinkStatus statusRequested = (charTitle == NULL ? NxsBlock::BLOCK_LINK_TO_ONLY_CHOICE : NxsBlock::BLOCK_LINK_FROM_LINK_CMD);
		effectiveB = GetAssumptionsBlockForCharBlock(cb, statusRequested, token);
		}
	effectiveB->FlagCharBlockAsUsed();
	return effectiveB;
	}

NxsAssumptionsBlockAPI	*NxsAssumptionsBlock::GetAssumptionsBlockForTaxaTitle(const wchar_t*taxTitle, NxsToken &token, const wchar_t*cmd)
	{
	if (!nexusReader)
		NxsNCLAPIException(L"No NxsReader when reading Assumptions block.");
	if (taxTitle == NULL)
		{
		int cbstatus;
		NxsTaxaBlockAPI * thisCB = GetTaxaBlockPtr(&cbstatus);
		int ust = cbstatus & NxsBlock::BLOCK_LINK_UNUSED_MASK;
		if (thisCB != NULL && ust != BLOCK_LINK_UNINITIALIZED && ust != BLOCK_LINK_UNKNOWN_STATUS)
			return this;
		}
	unsigned ncb = 0;
	NxsTaxaBlockAPI * cb = nexusReader->GetTaxaBlockByTitle(taxTitle, &ncb);
	NxsAssumptionsBlockAPI *effectiveB = NULL;
	if (cb == NULL)
		{
		if (taxa)
			{
			const NxsString	 t=taxa->GetID();
			if (t.empty())
				{
				if (taxTitle == NULL)
					effectiveB = this;
				}
			else 
				{
				if (NxsString::case_insensitive_equals(taxTitle, t.c_str()))
					effectiveB = this;
				}
			}
		if (effectiveB == NULL)
			{
			errormsg.clear();
			errormsg << L"A TAXA block ";
			if (taxTitle)
				errormsg << L"with the title " << NxsString::GetEscaped(taxTitle);
			errormsg << L" must precede an " << id << L" block with a " <<  cmd << L" command.";
			errormsg << L"\n(If such a block exists, then this program may not be using an API for the NCL library that supports block linking).";
			throw NxsException(errormsg, token);
			}
		}
	else if (ncb > 1)
		{
		errormsg = L"Multiple TAXA blocks have been encountered, but a ";
		errormsg += cmd;
		errormsg += L" command was found which does not specify which TAXA block it uses.  The most recent TAXA block will be used.";
		nexusReader->NexusWarnToken(errormsg, NxsReader::AMBIGUOUS_CONTENT_WARNING, token);
		errormsg.clear();
		effectiveB = GetAssumptionsBlockForTaxaBlock(cb, NxsBlock::BLOCK_LINK_TO_MOST_RECENT, token);
		}
	else 
		{
		NxsBlockLinkStatus statusRequested = (taxTitle == NULL ? NxsBlock::BLOCK_LINK_TO_ONLY_CHOICE : NxsBlock::BLOCK_LINK_FROM_LINK_CMD);
		effectiveB = GetAssumptionsBlockForTaxaBlock(cb, statusRequested, token);
		}
	effectiveB->FlagTaxaBlockAsUsed();
	return effectiveB;
	}

NxsAssumptionsBlockAPI	*NxsAssumptionsBlock::GetAssumptionsBlockForTreesTitle(const wchar_t*treesTitle, NxsToken &token, const wchar_t*cmd)
	{
	if (!nexusReader)
		NxsNCLAPIException(L"No NxsReader when reading Assumptions block.");
	if (treesTitle == NULL)
		{
		int cbstatus;
		NxsTreesBlockAPI * thisCB = GetTreesBlockPtr(&cbstatus);
		int ust = cbstatus & NxsBlock::BLOCK_LINK_UNUSED_MASK;
		if (thisCB != NULL && ust != BLOCK_LINK_UNINITIALIZED && ust != BLOCK_LINK_UNKNOWN_STATUS)
			return this;
		}
	unsigned ncb = 0;
	NxsTreesBlockAPI * cb = nexusReader->GetTreesBlockByTitle(treesTitle, &ncb);
	NxsAssumptionsBlockAPI *effectiveB = NULL;
	if (cb == NULL)
		{
		if (treesBlockPtr)
			{
			const NxsString	 t=treesBlockPtr->GetID();
			if (t.empty())
				{
				if (treesTitle == NULL)
					effectiveB = this;
				}
			else 
				{
				if (NxsString::case_insensitive_equals(treesTitle, t.c_str()))
					effectiveB = this;
				}
			}
		if (effectiveB == NULL)
			{
			errormsg.clear();
			errormsg << L"A TREES block";
			if (treesTitle)
				errormsg << L"with the title " << NxsString::GetEscaped(treesTitle);
			errormsg << L" must precede an " << id << L" block with a " <<  cmd << L" command.";
			errormsg << L"\n(If such a block exists, then this program may not be using an API for the NCL library that supports block linking).";
			throw NxsException(errormsg, token);
			}
		}
	else if (ncb > 1)
		{
		errormsg = L"Multiple TREES blocks have been encountered, but a ";
		errormsg += cmd;
		errormsg += L" command was found which does not specify which TREES block it uses.  The most recent TREES block will be used.";
		nexusReader->NexusWarnToken(errormsg, NxsReader::AMBIGUOUS_CONTENT_WARNING, token);
		errormsg.clear();
		effectiveB = GetAssumptionsBlockForTreesBlock(cb, NxsBlock::BLOCK_LINK_TO_MOST_RECENT, token);
		}
	else 
		{
		NxsBlockLinkStatus statusRequested = (treesTitle == NULL ? NxsBlock::BLOCK_LINK_TO_ONLY_CHOICE : NxsBlock::BLOCK_LINK_FROM_LINK_CMD);
		effectiveB = GetAssumptionsBlockForTreesBlock(cb, statusRequested, token);
		}
	effectiveB->FlagTreesBlockAsUsed();
	return effectiveB;
	}

void NxsAssumptionsBlock::SetCharLinkStatus(NxsBlockLinkStatus s)
	{
	if (charLinkStatus & NxsBlock::BLOCK_LINK_USED)
		{
		throw NxsNCLAPIException(L"Resetting a used charLinkStatus");
		}
	charLinkStatus = s;
	}

void NxsAssumptionsBlock::SetTaxaLinkStatus(NxsBlockLinkStatus s)
	{
	if (taxaLinkStatus & NxsBlock::BLOCK_LINK_USED)
		{
		throw NxsNCLAPIException(L"Resetting a used taxaLinkStatus");
		}
	taxaLinkStatus = s;
	}

void NxsAssumptionsBlock::SetTreesLinkStatus(NxsBlockLinkStatus s)
	{
	if (treesLinkStatus & NxsBlock::BLOCK_LINK_USED)
		{
		throw NxsNCLAPIException(L"Resetting a used charLinkStatus");
		}
	treesLinkStatus = s;
	}

void NxsAssumptionsBlock::SetCharBlockPtr(NxsCharactersBlockAPI * c, NxsBlockLinkStatus s)
	{
	SetCharLinkStatus(s);
	charBlockPtr = c;
	}

void NxsAssumptionsBlock::SetTaxaBlockPtr(NxsTaxaBlockAPI *c, NxsBlockLinkStatus s) 
	{
	SetTaxaLinkStatus(s);
	taxa = c;
	}

void NxsAssumptionsBlock::SetTreesBlockPtr(NxsTreesBlockAPI * c, NxsBlockLinkStatus s) 
	{
	SetTreesLinkStatus(s);
	treesBlockPtr = c;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Sets id = "ASSUMPTIONS", charBlockPtr = NULL, and taxa = t. Assumes taxa is non-NULL.
*/
NxsAssumptionsBlock::NxsAssumptionsBlock(
  NxsTaxaBlockAPI *t)	/* pointer to the taxa block */
	:taxa(t),
	charBlockPtr(NULL),
	treesBlockPtr(NULL),
	charLinkStatus(NxsBlock::BLOCK_LINK_UNINITIALIZED),
	taxaLinkStatus(NxsBlock::BLOCK_LINK_UNINITIALIZED),
	treesLinkStatus(NxsBlock::BLOCK_LINK_UNINITIALIZED),
	passedRefOfOwnedBlock(false)
	{
	taxaLinkStatus = (t == NULL ? NxsBlock::BLOCK_LINK_UNINITIALIZED : NxsBlock::BLOCK_LINK_UNKNOWN_STATUS);
	id				= L"ASSUMPTIONS";
	Reset();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Nothing needs to be done in the destructor.
*/
NxsAssumptionsBlock::~NxsAssumptionsBlock()
	{
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Makes data member taxa point to 'tb'. Assumes tb is non-NULL.
|	NOTEAPICHANGE: In v2.1 this can throw a NxsNCLAPIException if the Assumptions block has been used, but not reset 
|	before the pointer is reassigned.
*/
void NxsAssumptionsBlock::ReplaceTaxaBlockPtr(
  NxsTaxaBlockAPI *tb)	/* pointer to new NxsTaxaBlockAPI object */
	{
	assert(tb != NULL);
	if (tb != taxa)
		SetTaxaBlockPtr(tb, NxsBlock::BLOCK_LINK_UNKNOWN_STATUS);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the number of character sets stored.
*/
int NxsAssumptionsBlock::GetNumCharSets() const
	{
	return (int)charsets.size();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Erases 'names' vector, then fills 'names' with the names of all stored character sets.
*/
void NxsAssumptionsBlock::GetCharSetNames(
  NxsStringVector &names) const	/* the vector in which to store the names */
	{
	names.erase(names.begin(), names.end());
	NxsUnsignedSetMap::const_iterator i;
	for (i = charsets.begin(); i != charsets.end(); i++)
		names.push_back((*i).first);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns pointer to character set having name 'nm'.
*/
const NxsUnsignedSet *NxsAssumptionsBlock::GetCharSet(
  NxsString nm) const /* the name of the character set to return */
	{
	NxsUnsignedSetMap::const_iterator it = charsets.find(nm);
	if (it == charsets.end())
		return NULL;
	return &(it->second);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the number of character partitions stored.
*/
int NxsAssumptionsBlock::GetNumCharPartitions()
	{
	return (int)charPartitions.size();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Erases 'names' vector, then fills 'names' with the names of all stored character partitions.
*/
void NxsAssumptionsBlock::GetCharPartitionNames(
	vector<std::wstring> &names)	/* the vector in which to store the names */
	{
	names.erase(names.begin(), names.end());
	NxsPartitionsByName::const_iterator i;
	for (i = charPartitions.begin(); i != charPartitions.end(); i++)
	names.push_back((*i).first);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns pointer to character partition having name 'nm'.
*/
const NxsPartition *NxsAssumptionsBlock::GetCharPartition(
	std::wstring nm) const /* the name of the character set to return */
	{
	NxsPartitionsByName::const_iterator it = charPartitions.find(nm);
	if (it == charPartitions.end())
		return NULL;
	return &(it->second);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the number of taxon sets stored.
*/
int NxsAssumptionsBlock::GetNumTaxSets()
	{
	return (int)taxsets.size();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Erases 'names' vector, then fills 'names' with the names of all stored taxon sets.
*/
void NxsAssumptionsBlock::GetTaxSetNames(
  NxsStringVector &names)	/* the vector in which to store the names */
	{
	names.erase(names.begin(), names.end());
	NxsUnsignedSetMap::const_iterator i;
	for (i = taxsets.begin(); i != taxsets.end(); i++)
		names.push_back((*i).first);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns reference to taxon set having name 'nm'.
*/
NxsUnsignedSet &NxsAssumptionsBlock::GetTaxSet(
  NxsString nm) /* the name of the taxon set to return */
	{
	return taxsets[nm];
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Returns the number of exclusion sets stored.
*/
int NxsAssumptionsBlock::GetNumExSets()
	{
	return (int)exsets.size();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Erases names, then fills names with the names of all stored exclusion sets.
*/
void NxsAssumptionsBlock::GetExSetNames(
  NxsStringVector &names)	/* the vector in which to store the names */
	{
	names.erase(names.begin(), names.end());
	NxsUnsignedSetMap::const_iterator i;
	for (i = exsets.begin(); i != exsets.end(); i++)
		names.push_back((*i).first);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns reference to exclusion set having name 'nm'.
*/
NxsUnsignedSet &NxsAssumptionsBlock::GetExSet(
  NxsString nm) /* the name of the exclusion set to return */
	{
	return exsets[nm];
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns name of default exclusion set. If returned wstring has zero length, then no default exclusion set was defined
|	in the data set.
*/
NxsString NxsAssumptionsBlock::GetDefExSetName()
	{
	return def_exset;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Applies exclusion set having name 'nm' by calling the ApplyExset method of the NxsCharactersBlockAPI or 
|	NxsCharactersBlockAPI-derived object stored in the charBlockPtr pointer (which will be whichever block last called the 
|	NxsAssumptionsBlock::SetCallback method).
*/
void NxsAssumptionsBlock::ApplyExset(
  NxsString nm) /* the name of the exclusion set to apply */
	{
	assert(charBlockPtr != NULL);
	charBlockPtr->ApplyExset(exsets[nm]);
	}

NxsAssumptionsBlockAPI *NxsAssumptionsBlock::DealWithPossibleParensInCharDependentCmd(NxsToken &token, const wchar_t*cmd, const std::vector<std::wstring> *unsupported, bool * isVect)
	{
	token.GetNextToken();
	NxsString charblock_name;
	errormsg.clear();
	if (isVect)
		*isVect = false;
	if (token.Equals(L"("))
		{
		token.GetNextToken();
		while (!token.Equals(L")"))
			{		 
			if (token.Equals(L"CHARACTERS"))
				{
				NxsString t;
				t << L"after \"(Characters\" in a " << cmd << L" command";
				DemandEquals(token, t.c_str());
				token.GetNextToken();
				charblock_name = token.GetToken();
				}
			else if (token.Equals(L"VECTOR"))
				{
				if (!isVect)
					GenerateNxsException(token, L"VECTOR-style set definitions are not currently supported");
				else
					*isVect = true;
				}
			else if (token.Equals(L"NOTOKENS"))
				GenerateNxsException(token, L"NOTOKENS-style set definitions are not currently supported");
			else if (token.Equals(L";"))
				{
				NxsString s;
				s << L"; encountered in " << cmd << L" command before parentheses were closed";
				GenerateNxsException(token, s.c_str());
				}
			else if (!(token.Equals(L"STANDARD") || token.Equals(L"TOKENS")) && nexusReader)
				{
				bool found = false;
				if (unsupported)
					{
					for (std::vector<std::wstring>::const_iterator u = unsupported->begin(); u != unsupported->end(); ++u)
						{
						if (token.Equals(u->c_str()))
							{
							found = true;
							break;
							}
						}
					}
				if (found)
					{
					NxsString s;
					s << L"The " << token.GetTokenReference()<< L" as a " << cmd << L" qualifier is not supported.";
					GenerateNxsException(token, s.c_str());
					}
				else
					{
					errormsg  << L"Skipping unknown " << cmd << L" qualifier: "  << token.GetTokenReference();
					nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
					errormsg.clear();
					}
				}
			token.GetNextToken();
			}
		token.GetNextToken();
		}
	const wchar_t*cbn = (charblock_name.empty() ? NULL : charblock_name.c_str());
	NxsString u;
	u << L"in " << cmd << L" definition";
	DemandIsAtEquals(token, u.c_str());
	return this->GetAssumptionsBlockForCharTitle(cbn, token, cmd);
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Reads and stores information contained in the command TypeSet within an ASSUMPTIONS block.
*/
void NxsAssumptionsBlock::HandleTypeSet(
  NxsToken &token)	/* the token used to read from in */
	{
	errormsg.clear();
	bool asterisked = false;
	token.GetNextToken();
	if (token.Equals(L"*"))
		{
		asterisked = true;
		token.GetNextToken();
		}
	NxsString typeset_name = token.GetToken();
	//typeset_name.ToUpper();
	NxsAssumptionsBlockAPI * effectiveAssumpBlock = DealWithPossibleParensInCharDependentCmd(token, L"TypeSet");
	token.GetNextToken();
	assert(effectiveAssumpBlock);
	NxsPartition newPartition;
	NxsCharactersBlockAPI *cbp = effectiveAssumpBlock->GetCharBlockPtr();
	assert(cbp);
	effectiveAssumpBlock->ReadPartitionDef(newPartition, *cbp, typeset_name, L"Character", L"TypeSet", token, false, false, false);
	NxsTransformationManager &ctm = cbp->GetNxsTransformationManagerRef();
	for (NxsPartition::const_iterator groupIt = newPartition.begin(); groupIt != newPartition.end(); ++groupIt)
		{
		if (!ctm.IsValidTypeName(groupIt->first))
			{
			errormsg << L"The group name " << groupIt->first << L" found in a TypeSet command does not correspond to a known type";
			throw NxsException(errormsg, token);
			}
		}
	NxsTransformationManager &tm = effectiveAssumpBlock->GetNxsTransformationManagerRef();
	ctm.AddTypeSet(typeset_name, newPartition, asterisked);
	tm.AddTypeSet(typeset_name, newPartition, asterisked);
	}


void NxsAssumptionsBlock::HandleUserType(NxsToken& token)
	{
	token.GetNextToken();
	errormsg.clear();
	if (token.Equals(L"*"))
		{
		errormsg << L"An * is ignored in a UserType command";
		if (nexusReader)
			nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
		token.GetNextToken();
		errormsg.clear();
		}
	NxsString user_type_name = token.GetToken();
	//codonPosSetName.ToUpper();
	token.GetNextToken();
	NxsString charblock_name;
	bool floatMat = false;
	bool cstreeform = false;
	if (token.Equals(L"("))
		{
		token.GetNextToken();
		while (!token.Equals(L")"))
			{		 
			if (token.Equals(L"CHARACTERS"))
				{
				NxsString t;
				t << L"after \"(Characters\" in a UserType command";
				DemandEquals(token, t.c_str());
				token.GetNextToken();
				charblock_name = token.GetToken();
				}
			else if (token.Equals(L"CSTREE"))
				cstreeform = true;
			else if (token.Equals(L"NOTOKENS"))
				GenerateNxsException(token, L"NOTOKENS-style UserType are not supported");
			else if (token.Equals(L"REALMATRIX"))
				floatMat = true;
			else if (token.Equals(L";"))
				{
				NxsString s;
				s << L"; encountered in UserType command before parentheses were closed";
				GenerateNxsException(token, s.c_str());
				}
			else if (!(token.Equals(L"STEPMATRIX") || token.Equals(L"TOKENS")) && nexusReader)
				{
				errormsg  << L"Skipping unknown UserType qualifier: "  << token.GetTokenReference();
				nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
				errormsg.clear();
				}
			token.GetNextToken();
			}
		token.GetNextToken();
		}
	if (token.Equals(L"STEPMATRIX") || token.Equals(L"REALMATRIX"))
		{
		errormsg  << L"UserType qualifier "<< token.GetTokenReference() << L" should occur in parentheses ("<< token.GetTokenReference() <<L") ";
		nexusReader->NexusWarnToken(errormsg, NxsReader::DEPRECATED_WARNING, token);
		errormsg.clear();
		token.GetNextToken();
		}
	DemandIsAtEquals(token, L"in UserType definition");
	
	const wchar_t*cbn = (charblock_name.empty() ? NULL : charblock_name.c_str());
	NxsAssumptionsBlockAPI * effectiveAssumpBlock =  this->GetAssumptionsBlockForCharTitle(cbn, token, L"UserType");
	assert(effectiveAssumpBlock);
	NxsCharactersBlockAPI *cbp = effectiveAssumpBlock->GetCharBlockPtr();
	assert(cbp);
	
	NxsRealStepMatrix::DblMatrix dm; 
	NxsIntStepMatrix::IntMatrix im;
	std::vector<std::wstring> symbolsOrder;
	const std::vector<const NxsDiscreteDatatypeMapper *> mappers = cbp->GetAllDatatypeMappers();
		
	if (cstreeform)
		{
		bool success = false;
		ProcessedNxsCommand tokenVec;
		token.GetNextToken();
		token.ProcessAsCommand(&tokenVec);

		for (std::vector<const NxsDiscreteDatatypeMapper *>::const_iterator mIt = mappers.begin(); !success && mIt != mappers.end(); ++mIt)
			{
			const NxsDiscreteDatatypeMapper * mapper = *mIt;
			std::wstring s = mapper->GetSymbolsWithGapChar();
			symbolsOrder.clear();
			std::map<std::wstring, unsigned> symMap;
			for (unsigned i = 0; i < s.length(); ++i)
				{
				std::wstring sym(1, s[i]);
				symbolsOrder.push_back(sym);
				symMap[sym] = i;
				}
			std::wstring newick;
			NxsFullTreeDescription treeDesc(newick, user_type_name, 0);
			BogusToIndMapper labelToIndMapper;
			try
				{
				NxsTreesBlock::ProcessTokenVecIntoTree(tokenVec, treeDesc, &labelToIndMapper, symMap, false, nexusReader, mapper->IsRespectCase());
				success = true;
				}
			catch (NxsException &x)
				{
				if (!labelToIndMapper.queried)
					throw x;
				}
			NxsSimpleTree cstree(treeDesc, 1, 1.0);
			if (treeDesc.SomeEdgesHaveLengths() && !treeDesc.EdgeLengthsAreAllIntegers())
				{
				floatMat = true;
				dm = cstree.GetDblPathDistances(false);
				if (dm.size() < symbolsOrder.size())
					symbolsOrder.resize(dm.size());
				else if (dm.size() > symbolsOrder.size())
					success = false;
				}
			else
				{
				im = cstree.GetIntPathDistances();
				if (im.size() < symbolsOrder.size())
					symbolsOrder.resize(im.size());
				else if (im.size() > symbolsOrder.size())
					success = false;
				}

			}
		if (!success)
			{
			errormsg << L"No datatype was found with all of the symbols the UserType CSTree";
			throw NxsException(errormsg, token);
			}
		}
	else
		{
		/* BEGIN Read as Stepmatrix section  */
		token.GetNextToken();
		NxsString t = token.GetToken();
		long longNstates;
		if (!NxsString::to_long(t.c_str(), &longNstates) || longNstates < 2)
			{
			errormsg << L"Expecting a number of states after the = in the UserType command (the number of states must be greater than one).  Found " << t;
			throw NxsException(t, token);
			}
		const bool respectCase = cbp->IsRespectCase();
		unsigned nStates = (unsigned) longNstates;
		NxsRealStepMatrix::DblVec dv(nStates, DBL_MAX); 
		NxsIntStepMatrix::IntVec iv(nStates, INT_MAX); 
		dm.assign(nStates, dv); 
		im.assign(nStates, iv);
		std::set<char> symbolsSet;
		for (unsigned i = 0; i < nStates;)
			{
			token.GetNextToken();
			if (token.Equals(L";"))
				{
				errormsg << L"; prematurely terminated the state declaration portion of a UserType stepmatrix.";
				throw NxsException(t, token);
				}
			NxsString tokStr = token.GetToken();
			if (!respectCase)
				tokStr.ToUpper();
			for (NxsString::const_iterator cIt = tokStr.begin(); cIt != tokStr.end(); ++cIt, ++i)
				{
				char s = *cIt;
				if (symbolsSet.count(s) > 0)
					{
					errormsg << L"State names cannot be repeated in a UserType stepmatrix.  " << s << L" was encountered more than once.";
					throw NxsException(t, token);
					}
				bool found = false;
				for (std::vector<const NxsDiscreteDatatypeMapper *>::const_iterator mIt = mappers.begin(); mIt != mappers.end(); ++mIt)
					{
					const NxsDiscreteDatatypeMapper * mapper = *mIt;
					if (mapper->PositionInSymbolsOrGaps(s) != NXS_INVALID_STATE_CODE)
						{
						found = true;
						break;
						}
					}
				if (!found)
					{
					errormsg << L"Unrecognized state " << s << L"in  UserType stepmatrix.";
					throw NxsException(t, token);
					}
				symbolsSet.insert(s);
				symbolsOrder.push_back(std::wstring(1,s));
				}
			}
		
		double currDblWt;
		long currLongWt;
		
		for (unsigned i = 0; i < nStates; ++i)
			{
			for (unsigned j = 0; j < nStates; ++j)
				{
				token.SetLabileFlagBit(NxsToken::hyphenNotPunctuation); // this allows us to deal with sci. not. in weights.
				token.GetNextToken();
				NxsString s = token.GetToken();
				if (i == j && (token.Equals(L".") || token.Equals(L"-")))
					{
					im[i][i] = 0;
					dm[i][i] = 0.0;
					}
				else
					{
					bool v = NxsString::to_double(s.c_str(), &currDblWt);
					if (!v)
						{
						if (!token.Equals(L"I") && !token.Equals(L"INF"))
							{
							errormsg << L"Expecting a number or i (for infinity) as an element of the UserType stepmatrix. Found " << s;
							throw NxsException(errormsg, token);
							}
						}
					else
						{
						dm[i][j] = currDblWt;
						if (!floatMat)
							{
							floatMat = !NxsString::to_long(s.c_str(), &currLongWt);
							if (!floatMat)
								im[i][j] = currLongWt;
							}
						}
					}
				}
			}
		/* END Read as Stepmatrix section  */
		token.GetNextToken();
		if (!token.Equals(L";"))
			{
			errormsg << L"Expecting ; at the end of the UserType command. Found "  << token.GetTokenReference();
			throw NxsException(t, token);
			}
		}
	
	NxsTransformationManager &ctm = cbp->GetNxsTransformationManagerRef();
	NxsTransformationManager &tm = effectiveAssumpBlock->GetNxsTransformationManagerRef();
	if (floatMat)
		{
		const NxsRealStepMatrix nrsm(symbolsOrder, dm);
		ctm.AddRealType(user_type_name, nrsm);
		tm.AddRealType(user_type_name, nrsm);
		}
	else
		{
		const NxsIntStepMatrix nism(symbolsOrder, im);
		ctm.AddIntType(user_type_name, nism);
		tm.AddIntType(user_type_name, nism);
		}
	}


NxsGeneticCodesManager::NxsGeneticCodesManager()
	{
	standardCodeNames.insert(std::wstring(L"UNIVERSAL"));
	standardCodeNames.insert(std::wstring(L"UNIVERSAL.EXT"));
	standardCodeNames.insert(std::wstring(L"MTDNA.DROS"));
	standardCodeNames.insert(std::wstring(L"MTDNA.DROS.EXT"));
	standardCodeNames.insert(std::wstring(L"MTDNA.MAM"));
	standardCodeNames.insert(std::wstring(L"MTDNA.MAM.EXT"));
	standardCodeNames.insert(std::wstring(L"MTDNA.YEAST"));
	}

bool NxsGeneticCodesManager::IsValidCodeName(const std::wstring &n) const
	{
	std::wstring capName(n.c_str());
	NxsString::to_upper(capName);
	return (standardCodeNames.count(capName) > 0) || (userDefinedCodeNames.count(capName) > 0);
	}
/*----------------------------------------------------------------------------------------------------------------------
|	Reads and stores information contained in the command TypeSet within an ASSUMPTIONS block.
*/
void NxsAssumptionsBlock::HandleCodeSet(
  NxsToken &token)	/* the token used to read from in */
	{
	bool asterisked = false;
	token.GetNextToken();
	if (token.Equals(L"*"))
		{
		asterisked = true;
		token.GetNextToken();
		}
	std::vector<std::wstring> unsupported;
	unsupported.push_back(std::wstring(L"TAXA"));
	unsupported.push_back(std::wstring(L"UNALIGNED"));
	NxsString codeset_name = token.GetToken();
	//codeset_name.ToUpper();
	NxsAssumptionsBlockAPI * effectiveAssumpBlock = DealWithPossibleParensInCharDependentCmd(token, L"CodeSet", &unsupported);
	token.GetNextToken();
	NxsPartition newPartition;
	NxsCharactersBlockAPI *cbp = effectiveAssumpBlock->GetCharBlockPtr();
	assert(cbp);
	effectiveAssumpBlock->ReadPartitionDef(newPartition, *cbp, codeset_name, L"Character", L"CodeSet", token, false, false, false);
	NxsGeneticCodesManager &gcm = effectiveAssumpBlock->GetNxsGeneticCodesManagerRef();
	for (NxsPartition::const_iterator groupIt = newPartition.begin(); groupIt != newPartition.end(); ++groupIt)
		{
		const std::wstring & s = groupIt->first;
		if (!gcm.IsValidCodeName(s))
			{
			errormsg << L"The Genetic code name " << groupIt->first << L" found in a CodeSet command does not correspond to a known code";
			throw NxsException(errormsg, token);
			}
		}
	effectiveAssumpBlock->AddCodeSet(codeset_name, newPartition, asterisked);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Reads and stores information contained in the command TypeSet within an ASSUMPTIONS block.
*/
void NxsAssumptionsBlock::HandleCodonPosSet(
  NxsToken &token)	/* the token used to read from in */
	{
	bool asterisked = false;
	token.GetNextToken();
	if (token.Equals(L"*"))
		{
		asterisked = true;
		token.GetNextToken();
		}
	NxsString codonPosSetName = token.GetToken();
	//codonPosSetName.ToUpper();
	NxsAssumptionsBlockAPI * effectiveAssumpBlock = DealWithPossibleParensInCharDependentCmd(token, L"CodonPosSet", NULL);
	token.GetNextToken();
	NxsPartition newPartition;
	NxsCharactersBlockAPI *cbp = effectiveAssumpBlock->GetCharBlockPtr();
	assert(cbp);
	effectiveAssumpBlock->ReadPartitionDef(newPartition, *cbp, codonPosSetName, L"Character", L"CodonPosSet", token, false, false, false);
	for (NxsPartition::const_iterator groupIt = newPartition.begin(); groupIt != newPartition.end(); ++groupIt)
		{
		const std::wstring & s = groupIt->first;
		bool legal = false;
		if (s.length() == 1)
			{
			const wchar_t c = s[0];
			if (c == 'n' || c == 'N' || c == '1' || c == '2' || c == '3' || c == '?')
				legal = true;
			}
		if (!legal)
			{
			errormsg << L"The Codon Position category name " << groupIt->first << L" found in a CodonPosSet command is not legal.  \"N\", \"1\", \"2\", or \"3\" were expected.";
			throw NxsException(errormsg, token);
			}
		}
	effectiveAssumpBlock->AddCodonPosSet(codonPosSetName, newPartition, asterisked);
	cbp->AddNewCodonPosPartition(codonPosSetName, newPartition, asterisked);
	}

class NxsSetVectorItemValidator
	{
	public:
		virtual ~NxsSetVectorItemValidator(){};
		virtual std::wstring convert(NxsToken &) = 0;
	};
	
class WtSetVectorItemValidator: public NxsSetVectorItemValidator
	{
	public:
		virtual ~WtSetVectorItemValidator(){};
		virtual std::wstring convert(NxsToken & token)
			{
			NxsString s = token.GetToken();
			const wchar_t* c = s.c_str();
			long currLongWt;
			double currDblWt;
			if (NxsString::to_long(c, &currLongWt) || NxsString::to_double(s.c_str(), &currDblWt))
				return std::wstring(c);
			NxsString errormsg;
			errormsg << L"Expecting a number as a character weight.  Found \"" << c << L"\" instead.";
			throw NxsException(errormsg, token);
			}
		
	};

/*----------------------------------------------------------------------------------------------------------------------
|	Reads and stores information contained in the command TypeSet within an ASSUMPTIONS block.
*/
void NxsAssumptionsBlock::HandleWeightSet(
  NxsToken &token)	/* the token used to read from in */
	{
	bool asterisked = false;
	token.GetNextToken();
	if (token.Equals(L"*"))
		{
		asterisked = true;
		token.GetNextToken();
		}
	NxsString wtset_name = token.GetToken();
	//wtset_name.ToUpper();
	bool isVect;
	NxsAssumptionsBlockAPI * effectiveAssumpBlock = DealWithPossibleParensInCharDependentCmd(token, L"WtSet", NULL, &isVect);
	token.SetLabileFlagBit(NxsToken::hyphenNotPunctuation); // this allows us to deal with sci. not. in weights.
	token.GetNextToken();
	NxsPartition newPartition;
	NxsCharactersBlockAPI *cbp = effectiveAssumpBlock->GetCharBlockPtr();
	assert(cbp);
	if (isVect)
		{
		WtSetVectorItemValidator validator; 
		effectiveAssumpBlock->ReadVectorPartitionDef(newPartition, *cbp, wtset_name, L"Character", L"WtSet", token, false, true, validator);
		}
	else
		effectiveAssumpBlock->ReadPartitionDef(newPartition, *cbp, wtset_name, L"Character", L"WtSet", token, false, false, false);
	bool floatWts = false;
	long currLongWt;
	double currDblWt;
	NxsTransformationManager &ctm = cbp->GetNxsTransformationManagerRef();
	NxsTransformationManager::ListOfIntWeights liw;
	NxsTransformationManager::ListOfDblWeights diw;
	for (NxsPartition::const_iterator groupIt = newPartition.begin(); groupIt != newPartition.end(); ++groupIt)
		{
		const std::wstring & s = groupIt->first;
		if (!floatWts)
			{
			floatWts = !NxsString::to_long(s.c_str(), &currLongWt);
			if (!floatWts)
				liw.push_back(NxsTransformationManager::IntWeightToIndexSet((int)currLongWt, groupIt->second));
			}
		bool v = NxsString::to_double(s.c_str(), &currDblWt);
		if (!v)
			{
			errormsg << L"Invalid weight " << groupIt->first << L" found in a WtSet command.";
			throw NxsException(errormsg, token);
			}
		diw.push_back(NxsTransformationManager::DblWeightToIndexSet(currDblWt, groupIt->second));
		}
	NxsTransformationManager &tm = effectiveAssumpBlock->GetNxsTransformationManagerRef();
	if (floatWts)
		{
		ctm.AddRealWeightSet(wtset_name, diw, asterisked);
		tm.AddRealWeightSet(wtset_name, diw, asterisked);
		}
	else
		{
		ctm.AddIntWeightSet(wtset_name, liw, asterisked);
		tm.AddIntWeightSet(wtset_name, liw, asterisked);
		}
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Reads and stores information contained in the command CharPartition within an ASSUMPTIONS block.
*/
void NxsAssumptionsBlock::HandleCharPartition(
  NxsToken &token)	/* the token used to read from in */
	{
	bool asterisked = false;
	token.GetNextToken();
	if (token.Equals(L"*"))
		{
		asterisked = true;
		token.GetNextToken();
		}
	NxsString charpart_name = token.GetToken();
	//charpart_name.ToUpper();
	NxsAssumptionsBlockAPI * effectiveAssumpBlock = DealWithPossibleParensInCharDependentCmd(token, L"CharPartition");
	token.GetNextToken();

	NxsPartition newPartition;
	NxsCharactersBlockAPI *cbp = effectiveAssumpBlock->GetCharBlockPtr();
	assert(cbp);
	effectiveAssumpBlock->ReadPartitionDef(newPartition, *cbp, charpart_name, L"Character", L"CharPartition", token, asterisked, false, true);
	effectiveAssumpBlock->AddCharPartition(charpart_name, newPartition);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Reads and stores information contained in the command CHARSET within an ASSUMPTIONS block.
*/
void NxsAssumptionsBlock::HandleCharSet(
  NxsToken &token)	/* the token used to read from in */
	{
	bool asterisked = false;
	token.GetNextToken();
	if (token.Equals(L"*"))
		{
		asterisked = true;
		token.GetNextToken();
		}
	NxsString charset_name = token.GetToken();
	//charset_name.ToUpper();
	NxsAssumptionsBlockAPI * effectiveAssumpBlock = DealWithPossibleParensInCharDependentCmd(token, L"CharSet");
	token.GetNextToken();
	effectiveAssumpBlock->ReadCharsetDef(charset_name, token, asterisked);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called after verifying that the correct Char block pointer is set.
*/
void NxsAssumptionsBlock::ReadCharsetDef(NxsString charset_name, NxsToken &token, bool asterisked)
	{
	assert(charBlockPtr != NULL);
	NxsCharactersBlockAPI &charBlock = *charBlockPtr;
	NxsUnsignedSet s;
	NxsSetReader::ReadSetDefinition(token, charBlock, L"Character", L"CharSet", &s);
	charsets[charset_name] = s;
	if (asterisked && nexusReader != NULL)
		{
		nexusReader->NexusWarnToken(L"An * is ignored in a CHARSET command", NxsReader::SKIPPING_CONTENT_WARNING, token);
		errormsg.clear();
		}
	if (charBlock.AddNewIndexSet(charset_name, s) && nexusReader)
		{
		errormsg = L"A CHARSET with the name ";
		errormsg += charset_name;
		errormsg += L" has already been encountered.    The later definition will preempt the earlier definition(s).";
		nexusReader->NexusWarnToken(errormsg, NxsReader::OVERWRITING_CONTENT_WARNING, token);
		errormsg.clear();
		}
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Reads and stores information contained in the command CHARSET within an ASSUMPTIONS block.
*/
void NxsAssumptionsBlock::HandleExSet(
  NxsToken &token)	/* the token used to read from in */
	{
	
	bool asterisked = false;
	token.GetNextToken();
	if (token.Equals(L"*"))
		{
		asterisked = true;
		token.GetNextToken();
		}
	NxsString exset_name = token.GetToken();
	//exset_name.ToUpper();
	NxsAssumptionsBlockAPI * effectiveAssumpBlock = DealWithPossibleParensInCharDependentCmd(token, L"ExSet");
	token.GetNextToken();
	effectiveAssumpBlock->ReadExsetDef(exset_name, token, asterisked);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called after verifying that the correct Char block pointer is set.
*/
void NxsAssumptionsBlock::ReadExsetDef(NxsString charset_name, NxsToken &token, bool asterisked)
	{
	assert(charBlockPtr != NULL);
	NxsCharactersBlockAPI &charBlock = *charBlockPtr;
	NxsUnsignedSet s;
	NxsSetReader::ReadSetDefinition(token, charBlock, L"Character", L"ExSet", &s);
	exsets[charset_name] = s;
	if (charBlock.AddNewExSet(charset_name, s) && nexusReader)
		{
		errormsg = L"A ExSet with the name ";
		errormsg += charset_name;
		errormsg += L" has already been encountered.    The later definition will preempt the earlier definition(s).";
		nexusReader->NexusWarnToken(errormsg, NxsReader::OVERWRITING_CONTENT_WARNING, token);
		errormsg.clear();
		}
	if (asterisked)
		{
		def_exset = charset_name;
		ApplyExset(charset_name);
		}
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Reads and stores information contained in the command TaxPartition within an ASSUMPTIONS block.
*/
void NxsAssumptionsBlock::HandleTaxPartition(
  NxsToken &token)	/* the token used to read from in */
	{
	bool asterisked = false;
	token.GetNextToken();
	if (token.Equals(L"*"))
		{
		asterisked = true;
		token.GetNextToken();
		}
	NxsString taxpart_name = token.GetToken();
	//taxpart_name.ToUpper();

	NxsAssumptionsBlockAPI *effectiveAssumpBlock = NULL;
	token.GetNextToken();
	NxsString taxblock_name;

	if (token.Equals(L"("))
		{
		token.GetNextToken();
		while (!token.Equals(L")"))
			{		 
			if (token.Equals(L"TAXA"))
				{
				DemandEquals(token, L"after \"(Taxa\" in a TaxPartition command");
				token.GetNextToken();
				taxblock_name = token.GetToken();
				}
			else if (token.Equals(L"VECTOR"))
				GenerateNxsException(token, L"VECTOR-style set definitions are not currently supported");
			else if (token.Equals(L"NOTOKENS"))
				GenerateNxsException(token, L"NOTOKENS-style set definitions are not currently supported");
			else if (token.Equals(L";"))
				GenerateNxsException(token, L"; encountered in TaxPartition command before parentheses were closed");
			else if (!(token.Equals(L"STANDARD") || token.Equals(L"TOKENS")) && nexusReader)
				{
				errormsg = L"Skipping unknown TaxPartition qualifier: ";
				errormsg << token.GetTokenReference();
				nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
				errormsg.clear();
				}
			token.GetNextToken();
			}
		token.GetNextToken();
		}
	const wchar_t*cbn = (taxblock_name.empty() ? NULL : taxblock_name.c_str());
	effectiveAssumpBlock = this->GetAssumptionsBlockForTaxaTitle(cbn, token, L"TaxPartition");
	DemandIsAtEquals(token, L"in TaxPartition definition");
	token.GetNextToken();
	NxsPartition newPartition;
	assert(taxa);
	effectiveAssumpBlock->ReadPartitionDef(newPartition, *taxa, taxpart_name, L"Taxa", L"TaxPartition", token, asterisked, false, true);
	effectiveAssumpBlock->AddTaxPartition(taxpart_name, newPartition);
	}
/*----------------------------------------------------------------------------------------------------------------------
|	Reads and stores information contained in the command TreePartition within an ASSUMPTIONS block.
*/
void NxsAssumptionsBlock::HandleTreePartition(
  NxsToken &token)	/* the token used to read from in */
	{
	bool asterisked = false;
	token.GetNextToken();
	if (token.Equals(L"*"))
		{
		asterisked = true;
		token.GetNextToken();
		}
	NxsString treepart_name = token.GetToken();
	//treepart_name.ToUpper();
	NxsAssumptionsBlockAPI *effectiveAssumpBlock = NULL;
	token.GetNextToken();
	NxsString treeblock_name;

	if (token.Equals(L"("))
		{
		token.GetNextToken();
		while (!token.Equals(L")"))
			{		 
			if (token.Equals(L"TREES"))
				{
				DemandEquals(token, L"after \"(Trees\" in a TreePartition command");
				token.GetNextToken();
				treeblock_name = token.GetToken();
				}
			else if (token.Equals(L"VECTOR"))
				GenerateNxsException(token, L"VECTOR-style set definitions are not currently supported");
			else if (token.Equals(L"NOTOKENS"))
				GenerateNxsException(token, L"NOTOKENS-style set definitions are not currently supported");
			else if (token.Equals(L";"))
				GenerateNxsException(token, L"; encountered in TreePartition command before parentheses were closed");
			else if (!(token.Equals(L"STANDARD") || token.Equals(L"TOKENS")) && nexusReader)
				{
				errormsg = L"Skipping unknown TreePartition qualifier: ";
				errormsg << token.GetTokenReference();
				nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
				errormsg.clear();
				}
			token.GetNextToken();
			}
		token.GetNextToken();
		}
	const wchar_t*cbn = (treeblock_name.empty() ? NULL : treeblock_name.c_str());
	effectiveAssumpBlock = this->GetAssumptionsBlockForTreesTitle(cbn, token, L"TreePartition");
	DemandIsAtEquals(token, L"in TreePartition definition");
	token.GetNextToken();
	NxsPartition newPartition;
	assert(treesBlockPtr);
	effectiveAssumpBlock->ReadPartitionDef(newPartition, *treesBlockPtr, treepart_name, L"Tree", L"TreePartition", token, asterisked, false, true);
	effectiveAssumpBlock->AddTreePartition(treepart_name, newPartition);
	}

void NxsBlock::ReadPartitionDef(
  NxsPartition &np, 
  NxsLabelToIndicesMapper &ltm, 
  const std::wstring & partName, 
  const wchar_t* ptype, 
  const wchar_t* cmd, 
  NxsToken & token, 
  bool warnAsterisked, 
  bool demandAllInds, 
  bool storeAsPartition)
	{
	NxsUnsignedSet allInds;
	const unsigned total = ltm.GetMaxIndex() + 1;
	std::set<NxsString> prevGroupNames;
	errormsg.clear();
	for (;;)
		{
		if (token.Equals(L";"))
			break;
		NxsString groupN = token.GetToken();
		NxsString capGroupN = groupN;
		capGroupN.ToUpper();
		if (prevGroupNames.count(capGroupN) > 0)
			{
			errormsg << L"Illegal repitition of a subset name (" << groupN << L") in the " << cmd << L" definition of " << partName;
			throw NxsException(errormsg, token);
			}
		token.GetNextToken();
		if (!token.Equals(L":"))
			{
			errormsg << L"Expecting a : after the subset name " << groupN << L" in the " << cmd  << L" definition of " << partName << L". Found " << token.GetToken(); 
			throw NxsException(errormsg, token);
			}
		token.GetNextToken();
		NxsUnsignedSet s;
		NxsSetReader::ReadSetDefinition(token, ltm, ptype, cmd, &s, &allInds);
		allInds.insert(s.begin(), s.end());
		np.push_back(NxsPartitionGroup(groupN, s));
		if (token.Equals(L";"))
			break;
		assert(token.Equals(L","));
		 // this flag allows us to deal with sci. not. in WtSet commands. 
		 //	It shouldn't hurt in other contexts, though the parser will be
		 //		more lax than it should (and will accept unquoted tokens-like-this as names).
		token.SetLabileFlagBit(NxsToken::hyphenNotPunctuation);
		token.GetNextToken();	
		}
	if (allInds.size() < total)
		{
		unsigned n = 0;
		for (;n < total; ++n)
			{
			if (allInds.count(n) == 0)
				break;
			}
		errormsg << partName << L" is a not a valid "<< cmd <<L". At least one " << ptype << L" ("<< n+1 << L") is not included";
		if (demandAllInds)
			throw NxsException(errormsg, token);
		else if (nexusReader)
			{
			nexusReader->NexusWarnToken(errormsg, NxsReader::ILLEGAL_CONTENT_WARNING, token);
			errormsg.clear();
			}
		}
	if (warnAsterisked && nexusReader != NULL)
		{
		errormsg << L"An * is ignored in a " << cmd << L" command";
		nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
		errormsg.clear();
		}
	if (storeAsPartition && ltm.AddNewPartition(partName, np) && nexusReader)
		{
		errormsg << L"A " << cmd << L" with the name ";
		errormsg += partName;
		errormsg += L" has already been encountered.    The later definition will preempt the earlier definition(s).";
		nexusReader->NexusWarnToken(errormsg, NxsReader::OVERWRITING_CONTENT_WARNING, token);
		errormsg.clear();
		}
	}

unsigned NxsBlock::ReadVectorPartitionDef(NxsPartition &np, NxsLabelToIndicesMapper &ltm, const std::wstring & partName, const wchar_t* ptype, const wchar_t* cmd, NxsToken & token, bool warnAsterisked, bool demandAllInds, NxsSetVectorItemValidator & v)
	{
	NxsUnsignedSet allInds;
	const unsigned total = ltm.GetMaxIndex() + 1;
	std::map<std::wstring, NxsUnsignedSet> subsetMap;
	errormsg.clear();
	unsigned ind = 0;
	for (; ind < total; ++ind)
		{
		if (token.Equals(L";"))
			break;
		const std::wstring key = v.convert(token);
		const std::wstring capKey = NxsString::get_upper(key);
		NxsUnsignedSet & s = subsetMap[key];
		s.insert(ind);
		token.GetNextToken();
		}
	if (ind < total)
		{
		errormsg << partName << L" is a not a valid "<< cmd <<L". Only " << ind + 1 << L" entries for " << ptype << L"(s) were included in the definition";
		if (demandAllInds)
			throw NxsException(errormsg, token);
		else if (nexusReader)
			{
			nexusReader->NexusWarnToken(errormsg, NxsReader::ILLEGAL_CONTENT_WARNING, token);
			errormsg.clear();
			}
		}
	if (warnAsterisked && nexusReader != NULL)
		{
		errormsg << L"An * is ignored in a " << cmd << L" command";
		nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
		errormsg.clear();
		}
	np.clear();
	for (std::map<std::wstring, NxsUnsignedSet>::const_iterator sIt = subsetMap.begin(); sIt != subsetMap.end(); ++sIt)
		{
		const std::wstring & k = sIt->first;
		const NxsUnsignedSet & valset = sIt->second;
		np.push_back(NxsPartitionGroup(k, valset));
		}
	if (ltm.AddNewPartition(partName, np) && nexusReader)
		{
		errormsg << L"A " << cmd << L" with the name ";
		errormsg += partName;
		errormsg += L" has already been encountered.    The later definition will preempt the earlier definition(s).";
		nexusReader->NexusWarnToken(errormsg, NxsReader::OVERWRITING_CONTENT_WARNING, token);
		errormsg.clear();
		}
	return ind;
	}

void NxsWriteSetCommand(const wchar_t*cmd, const NxsUnsignedSetMap & usetmap, std::wostream &out, const wchar_t* nameOfDef)
	{
	if (usetmap.empty())
		return;
	for (NxsUnsignedSetMap::const_iterator csIt = usetmap.begin(); csIt != usetmap.end(); ++csIt)
		{
		out << L"    " << cmd << L" ";
		if (NxsString::case_insensitive_equals(csIt->first.c_str(), nameOfDef))
			out << L"* ";
		out << NexusToken::EscapeString(csIt->first) << L" =";
		NxsSetReader::WriteSetAsNexusValue(csIt->second, out);
		out << L";\n";
		}
	}

void NxsWritePartitionCommand(const wchar_t*cmd, const NxsPartitionsByName & usetmap, std::wostream &out, const wchar_t* nameOfDef)
	{
	if (usetmap.empty())
		return;
	for (NxsPartitionsByName::const_iterator csIt = usetmap.begin(); csIt != usetmap.end(); ++csIt)
		{
		out << L"    " << cmd << L" ";
		if (NxsString::case_insensitive_equals(csIt->first.c_str(), nameOfDef))
			out << L"* ";
		out << NexusToken::EscapeString(csIt->first) << L" =";
		const NxsPartition & p = csIt->second;
		bool first = true;
		for (NxsPartition::const_iterator pIt = p.begin(); pIt != p.end(); ++pIt)
			{
			const NxsPartitionGroup & g = *pIt;
			if (!first)
				out << (wchar_t)',';
			out << (wchar_t)' ' << NxsString::GetEscaped(g.first) << L" :";
			NxsSetReader::WriteSetAsNexusValue(g.second, out);
			first = false;
			}
		out << L";\n";
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Reads and stores information contained in the command TAXSET within an ASSUMPTIONS block.
*/
void NxsAssumptionsBlock::HandleTaxSet(
  NxsToken &token)	/* the token used to read from in */
	{
	bool asterisked = false;
	token.GetNextToken();
	if (token.Equals(L"*"))
		{
		asterisked = true;
		token.GetNextToken();
		}
	NxsString taxset_name = token.GetToken();
	//taxset_name.ToUpper();
	token.GetNextToken();
	NxsAssumptionsBlockAPI *effectiveAssumpBlock = NULL;
	NxsString taxblock_name;
	if (token.Equals(L"("))
		{
		token.GetNextToken();
		while (!token.Equals(L")"))
			{		 
			if (token.Equals(L"TAXA"))
				{
				DemandEquals(token, L"after \"(Taxa\" in a TaxSet command");
				token.GetNextToken();
				taxblock_name = token.GetToken();
				}
			else if (token.Equals(L"VECTOR"))
				GenerateNxsException(token, L"VECTOR-style set definitions are not currently supported");
			else if (token.Equals(L";"))
				GenerateNxsException(token, L"; encountered in TaxSet command before parentheses were closed");
			else if (!token.Equals(L"STANDARD") && nexusReader)
				{
				errormsg = L"Skipping unknown TaxSet qualifier: ";
				errormsg << token.GetTokenReference();
				nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
				errormsg.clear();
				}
			token.GetNextToken();
			}
		token.GetNextToken();
		}
	const wchar_t*tbn = (taxblock_name.empty() ? NULL : taxblock_name.c_str());
	effectiveAssumpBlock = this->GetAssumptionsBlockForTaxaTitle(tbn, token, L"TAXSET");
	DemandIsAtEquals(token, L"in TAXSET definition");
	token.GetNextToken();
	effectiveAssumpBlock->ReadTaxsetDef(taxset_name, token, asterisked);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called after verifying that the correct taxa block pointer is set.
*/
void NxsAssumptionsBlock::ReadTaxsetDef(NxsString taxset_name, NxsToken &token, bool asterisked)
	{
	assert(taxa != NULL);
	NxsTaxaBlockAPI &taxaBlock = *taxa;
	NxsUnsignedSet s;
	NxsSetReader::ReadSetDefinition(token, taxaBlock, L"Taxon", L"TaxSet", &s);
	taxsets[taxset_name] = s;
	if (asterisked && nexusReader != NULL)
		{
		nexusReader->NexusWarnToken(L"An * is ignored in a TaxSet command", NxsReader::SKIPPING_CONTENT_WARNING, token);
		errormsg.clear();
		}
	if (taxaBlock.AddNewIndexSet(taxset_name, s) && nexusReader)
		{
		errormsg = L"A TaxSet with the name ";
		errormsg += taxset_name;
		errormsg += L" has already been encountered.    The later definition will preempt the earlier definition(s).";
		nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
		errormsg.clear();
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Reads and stores information contained in the command TAXSET within an ASSUMPTIONS block.
*/
void NxsAssumptionsBlock::HandleTreeSet(
  NxsToken &token)	/* the token used to read from in */
	{
	bool asterisked = false;
	token.GetNextToken();
	if (token.Equals(L"*"))
		{
		asterisked = true;
		token.GetNextToken();
		}
	NxsString treeset_name = token.GetToken();
	//treeset_name.ToUpper();
	token.GetNextToken();
	NxsAssumptionsBlockAPI *effectiveAssumpBlock = NULL;
	NxsString treeblock_name;
	if (token.Equals(L"("))
		{
		token.GetNextToken();
		while (!token.Equals(L")"))
			{		 
			if (token.Equals(L"TREES"))
				{
				DemandEquals(token, L"after \"(Trees\" in a TreeSet command");
				token.GetNextToken();
				treeblock_name = token.GetToken();
				}
			else if (token.Equals(L"VECTOR"))
				GenerateNxsException(token, L"VECTOR-style set definitions are not currently supported");
			else if (token.Equals(L";"))
				GenerateNxsException(token, L"; encountered in TreeSet command before parentheses were closed");
			else if (!token.Equals(L"STANDARD") && nexusReader)
				{
				errormsg = L"Skipping unknown TreeSet qualifier: ";
				errormsg << token.GetTokenReference();
				nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
				errormsg.clear();
				}
			token.GetNextToken();
			}
		token.GetNextToken();
		}
	const wchar_t*tbn = (treeblock_name.empty() ? NULL : treeblock_name.c_str());
	effectiveAssumpBlock = this->GetAssumptionsBlockForTreesTitle(tbn, token, L"TreeSet");
	DemandIsAtEquals(token, L"in TreeSet definition");
	token.GetNextToken();
	effectiveAssumpBlock->ReadTreesetDef(treeset_name, token, asterisked);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Called after verifying that the correct trees block pointer is set.
*/
void NxsAssumptionsBlock::ReadTreesetDef(NxsString treeset_name, NxsToken &token, bool asterisked)
	{
	assert(treesBlockPtr != NULL);
	NxsTreesBlockAPI &treesBlock = *treesBlockPtr;
	NxsUnsignedSet s;
	NxsSetReader::ReadSetDefinition(token, treesBlock, L"Trees", L"TreeSet", &s);
	treesets[treeset_name] = s;
	if (asterisked && nexusReader != NULL)
		{
		nexusReader->NexusWarnToken(L"An * is ignored in a TreeSet command", NxsReader::SKIPPING_CONTENT_WARNING, token);
		errormsg.clear();
		}
	if (treesBlock.AddNewIndexSet(treeset_name, s) && nexusReader)
		{
		errormsg = L"A TreeSet with the name ";
		errormsg += treeset_name;
		errormsg += L" has already been encountered.    The later definition will preempt the earlier definition(s).";
		nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
		errormsg.clear();
		}
	}

/*----------------------------------------------------------------------------------------------------------------------
|	This function provides the ability to read everything following the block name (which is read by the NxsReader 
|	object) to the end or ENDBLOCK statement. Characters are read from the input stream in. Overrides the pure virtual
|	function in the base class.
*/
void NxsAssumptionsBlock::Read(
  NxsToken &token)	/* the token used to read from in */
	{
	isEmpty = false;
	isUserSupplied = true;
	NxsString n = L"BEGIN ";
	n << id;
	DemandEndSemicolon(token, n.c_str());

	for(;;)
		{
		token.GetNextToken();
		NxsBlock::NxsCommandResult res = HandleBasicBlockCommands(token);
		if (res == NxsBlock::NxsCommandResult(STOP_PARSING_BLOCK))
			return;
		if (res != NxsBlock::NxsCommandResult(HANDLED_COMMAND))
			{
			if (token.Equals(L"CHARPARTITION"))
				HandleCharPartition(token);
			else if (token.Equals(L"CHARSET"))
				HandleCharSet(token);
			else if (token.Equals(L"CODESET"))
				HandleCodeSet(token);
			else if (token.Equals(L"CODONPOSSET"))
				HandleCodonPosSet(token);
			else if (token.Equals(L"EXSET"))
				HandleExSet(token);
			else if (token.Equals(L"OPTIONS"))
				HandleOptions(token);
			else if (token.Equals(L"TAXSET"))
				HandleTaxSet(token);
			else if (token.Equals(L"TAXPARTITION"))
				HandleTaxPartition(token);
			else if (token.Equals(L"TREESET"))
				HandleTreeSet(token);
			else if (token.Equals(L"TREEPARTITION"))
				HandleTreePartition(token);
			else if (token.Equals(L"TYPESET"))
				HandleTypeSet(token);
			else if (token.Equals(L"USERTYPE"))
				HandleUserType(token);
			else if (token.Equals(L"WTSET"))
				HandleWeightSet(token);
			else
				SkipCommand(token);

			}
		}	// for(;;)
	}
void NxsAssumptionsBlock::HandleOptions(NxsToken &token)
	{
	errormsg.clear();
	token.GetNextToken();
	std::map<std::wstring, std::wstring> kv = token.ProcessAsSimpleKeyValuePairs(L"OPTIONS");
	std::map<std::wstring, std::wstring>::const_iterator kvIt = kv.begin();
	for (; kvIt != kv.end(); ++kvIt)
		{
		if (NxsString::case_insensitive_equals(kvIt->first.c_str(), L"DEFTYPE"))
			{
			NxsAssumptionsBlockAPI	* effAssumpB = GetAssumptionsBlockForCharTitle(NULL, token, L"OPTIONS");
			assert(effAssumpB);
			NxsCharactersBlockAPI * cb = effAssumpB->GetCharBlockPtr();
			assert(cb);
			NxsTransformationManager & tmRef = cb->GetNxsTransformationManagerRef();
			if (!tmRef.IsValidTypeName(kvIt->second.c_str()))
				{
				errormsg << kvIt->second << L" is not a valid type name for OPTIONS DefType. Expceting one of:\n";
				const std::set<std::wstring> & tn = tmRef.GetTypeNames();
				for (std::set<std::wstring>::const_iterator tnIt = tn.begin(); tnIt != tn.end(); ++tnIt)
					errormsg << ' ' << NxsString::GetEscaped(*tnIt);
				throw NxsException(errormsg, token);
				}
			try 
				{
				tmRef.SetDefaultTypeName(kvIt->second);
				NxsTransformationManager & etmRef  = effAssumpB->GetNxsTransformationManagerRef();
				etmRef.SetDefaultTypeName(kvIt->second);
				}
			catch (const NxsException & x)
				{
				throw NxsException(x.msg, token);
				}
			}
		else if (NxsString::case_insensitive_equals(kvIt->first.c_str(), L"POLYTCOUNT"))
			{
			if (NxsString::case_insensitive_equals(kvIt->second.c_str(), L"MINSTEPS"))
				polyTCountValue = POLY_T_COUNT_MIN;
			else if (NxsString::case_insensitive_equals(kvIt->second.c_str(), L"MAXSTEPS"))
				polyTCountValue = POLY_T_COUNT_MAX;
			else
				{
				errormsg << L"Unknown value (" << kvIt->second << L") found for OPTIONS PolyTCount (expecting MINSTEPS or MAXSTEPS).";
				throw NxsException(errormsg, token);
				}
			}
		else if (NxsString::case_insensitive_equals(kvIt->first.c_str(), L"GAPMODE"))
			{
			if (NxsString::case_insensitive_equals(kvIt->second.c_str(), L"MISSING"))
				gapsAsNewstate = false;
			else if (NxsString::case_insensitive_equals(kvIt->second.c_str(), L"NEWSTATE"))
				gapsAsNewstate = true;
			else
				{
				errormsg << L"Unknown value (" << kvIt->second << L") found for OPTIONS GapMode (expecting MISSING or NEWSTATE).";
				throw NxsException(errormsg, token);
				}
			}
		else if (nexusReader)
			{
			errormsg << L"Skipping unknown subcommand (" << kvIt->first << L") in OPTIONS command of " << id << L" Block";
			nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
			errormsg.clear();
			}
		}
	}
	
/*----------------------------------------------------------------------------------------------------------------------
|	Prepares for reading a new ASSUMPTIONS block. Overrides the pure virtual function in the base class.
*/
void NxsAssumptionsBlock::Reset()
	{
	if (!passedRefOfOwnedBlock)
		{
		VecAssumpBlockPtr::iterator bIt = createdSubBlocks.begin();
		for(; bIt != createdSubBlocks.end(); ++bIt)
			{
			if (*bIt)
				delete *bIt;
			}
		createdSubBlocks.clear();
		}
	passedRefOfOwnedBlock = false;
	NxsBlock::Reset();
	exsets.clear();
	taxsets.clear();
	charsets.clear();
	def_exset.clear();
	charPartitions.clear();
	taxPartitions.clear();
	treePartitions.clear();
	readAs = UNREAD_OR_GENERATED_BLOCK;
	charLinkStatus &= BLOCK_LINK_UNUSED_MASK;
	taxaLinkStatus &= BLOCK_LINK_UNUSED_MASK;
	treesLinkStatus &= BLOCK_LINK_UNUSED_MASK;
	if (charLinkStatus & NxsBlock::BLOCK_LINK_FROM_LINK_CMD)
		SetCharBlockPtr(NULL, NxsBlock::BLOCK_LINK_UNINITIALIZED);
	if (taxaLinkStatus & NxsBlock::BLOCK_LINK_FROM_LINK_CMD)
		SetTaxaBlockPtr(NULL, NxsBlock::BLOCK_LINK_UNINITIALIZED);
	if (treesLinkStatus & NxsBlock::BLOCK_LINK_FROM_LINK_CMD)
		SetTreesBlockPtr(NULL, NxsBlock::BLOCK_LINK_UNINITIALIZED);
	transfMgr.Reset();
	codesMgr.Reset();
	polyTCountValue = POLY_T_COUNT_UNKNOWN;
	gapsAsNewstate = false;
	codonPosSets.clear();
	def_codonPosSet.clear();
	codeSets.clear();
	def_codeSet.clear();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	This function outputs a brief report of the contents of this ASSUMPTIONS block. Overrides the pure virtual function
|	in the base class.
*/
void NxsAssumptionsBlock::Report(
  std::wostream &out)  NCL_COULD_BE_CONST /* the output stream to which to write the report */
	{
	out << endl;
	out << id << L" block contains the following:" << endl;

	if (charsets.empty())
		out << L"  No character sets were defined" << endl;
	else
		{
		NxsUnsignedSetMap::const_iterator charsets_iter = charsets.begin();
		if (charsets.size() == 1)
			{
			out << L"  1 character set defined:" << endl;
			out << L"   " << (*charsets_iter).first << endl;
			}
		else
			{
			out << L"  " << (unsigned)charsets.size() << L" character sets defined:" << endl;
			for (; charsets_iter != charsets.end(); charsets_iter++)
				{
				NxsString nm = (*charsets_iter).first;
				out << L"   " << nm;
				out << endl;
				}
			}
		}	// if (charsets.empty()) ... else

	if (taxsets.empty())
		out << L"  No taxon sets were defined" << endl;
	else
		{
		NxsUnsignedSetMap::const_iterator taxsets_iter = taxsets.begin();
		if (taxsets.size() == 1)
			{
			out << L"  1 taxon set defined:" << endl;
			out << L"   " << (*taxsets_iter).first << endl;
			}
		else
			{
			out << L"  " << (unsigned)taxsets.size() << L" taxon sets defined:" << endl;
			for (; taxsets_iter != taxsets.end(); taxsets_iter++)
				{
				NxsString nm = (*taxsets_iter).first;
				out << L"   " << nm;
				out << endl;
				}
			}
		}	// if (taxsets.empty()) ... else

	if (exsets.empty())
		out << L"  No exclusion sets were defined" << endl;
	else
		{
		NxsUnsignedSetMap::const_iterator exsets_iter = exsets.begin();
		if (exsets.size() == 1)
			{
			out << L"  1 exclusion set defined:" << endl;
			out << L"   " << (*exsets_iter).first << endl;
			}
		else
			{
			out << L"  " << (unsigned)exsets.size() << L" exclusion sets defined:" << endl;
			for (; exsets_iter != exsets.end(); exsets_iter++)
				{
				NxsString nm = (*exsets_iter).first;
				out << L"   " << nm;
				if (NxsString::case_insensitive_equals(nm.c_str(), def_exset.c_str()))
					out << L" (default)";
				out << endl;
				}
			}
		}

	out << endl;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	A CHARACTERS, DATA, or ALLELES block can call this function to specify that it is to receive notification when the 
|	current taxon or character set changes (e.g., an "EXSET *" command is read or a program requests that one of the 
|	predefined taxon sets, character sets, or exsets be applied). Normally, a NxsCharactersBlockAPI-derived object calls 
|	this function upon entering its MATRIX command, since when that happens it becomes the primary data-containing block.
*/
void NxsAssumptionsBlock::SetCallback(
  NxsCharactersBlockAPI* p) /* the object to be called in the event of a change in character status */
	{
	charBlockPtr = p;
	SetCharLinkStatus(NxsBlock::BLOCK_LINK_TO_MOST_RECENT);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Converts a taxon label to a number corresponding to the taxon's position within the list maintained by the 
|	NxsTaxaBlockAPI object. This method overrides the virtual function of the same name in the NxsBlock base class. If s 
|	is not a valid taxon label, returns the value 0.
*/
unsigned NxsAssumptionsBlock::TaxonLabelToNumber(
  NxsString s)	const /* the taxon label to convert */
	{
	assert(taxa != NULL);
	int i;
	try
		{
		i = 1 + taxa->FindTaxon(s);
		}
	catch(NxsTaxaBlock::NxsX_NoSuchTaxon)
		{
		i = 0;
		}

	return i;
	}

/*only used it the linkAPI is enabled*/
void NxsAssumptionsBlock::HandleLinkCommand(NxsToken & token)
	{
	if (!nexusReader)
		NxsNCLAPIException(L"No NxsReader when reading Assumptions block.");

	token.GetNextToken();
	const std::map<std::wstring, std::wstring> kv = token.ProcessAsSimpleKeyValuePairs(L"LINK");
	std::map<std::wstring, std::wstring>::const_iterator pairIt = kv.begin();
	for (;pairIt != kv.end(); ++pairIt)
		{
		NxsString key(pairIt->first.c_str());
		key.ToUpper();
		NxsString value(pairIt->second.c_str());
		if (key == L"TAXA")
			{
			if (taxa && !taxa->GetID().EqualsCaseInsensitive(value))
				{
				if (GetTaxaLinkStatus() & NxsBlock::BLOCK_LINK_USED)
					{
					errormsg = L"LINK to a Taxa block must occur before commands that use a taxa block";
					throw NxsException(errormsg, token);
					}
				SetTaxaBlockPtr(NULL, NxsBlock::BLOCK_LINK_UNINITIALIZED);
				}
			if (!taxa)
				{
				NxsTaxaBlockAPI * cb = nexusReader->GetTaxaBlockByTitle(value.c_str(), NULL);
				if (cb == NULL)
					{
					errormsg = L"Unknown TAXA block (";
					errormsg += value;
					errormsg +=L") referred to in the LINK command";
					throw NxsException(errormsg, token);
					}
				SetTaxaBlockPtr(cb, NxsBlock::BLOCK_LINK_FROM_LINK_CMD);
				}				
			}
		else if (key == L"CHARACTERS")
			{
			if (charBlockPtr && !charBlockPtr->GetID().EqualsCaseInsensitive(value))
				{
				if (GetCharLinkStatus() & NxsBlock::BLOCK_LINK_USED)
					{
					errormsg = L"LINK to a CHARACTERS block must occur before commands that use a CHARACTERS block";
					throw NxsException(errormsg, token);
					}
				SetCharBlockPtr(NULL, NxsBlock::BLOCK_LINK_UNINITIALIZED);
				}
			if (!charBlockPtr)
				{
				NxsCharactersBlockAPI * cb = nexusReader->GetCharBlockByTitle(value.c_str(), NULL);
				if (cb == NULL)
					{
					errormsg = L"Unknown CHARACTERS block (";
					errormsg += value;
					errormsg +=L") referred to in the LINK command";
					throw NxsException(errormsg, token);
					}
				SetCharBlockPtr(cb, NxsBlock::BLOCK_LINK_FROM_LINK_CMD);
				}				
			}
		else if (key == L"TREES")
			{
			if (treesBlockPtr && !treesBlockPtr->GetID().EqualsCaseInsensitive(value))
				{
				if (GetTreesLinkStatus() & NxsBlock::BLOCK_LINK_USED)
					{
					errormsg = L"LINK to a TREES block must occur before commands that use a TREES block";
					throw NxsException(errormsg, token);
					}
				SetTreesBlockPtr(NULL, NxsBlock::BLOCK_LINK_UNINITIALIZED);
				}
			if (!treesBlockPtr)
				{
				NxsTreesBlockAPI * cb = nexusReader->GetTreesBlockByTitle(value.c_str(), NULL);
				if (cb == NULL)
					{
					errormsg = L"Unknown TREES block (";
					errormsg += value;
					errormsg +=L") referred to in the LINK command";
					throw NxsException(errormsg, token);
					}
				SetTreesBlockPtr(cb, NxsBlock::BLOCK_LINK_FROM_LINK_CMD);
				}				
			}
		else
			{
			errormsg = L"Skipping unknown LINK subcommand: ";
			errormsg += pairIt->first.c_str();
			nexusReader->NexusWarnToken(errormsg, NxsReader::SKIPPING_CONTENT_WARNING, token);
			errormsg.clear(); //this token pos will be off a bit.
			}
		}
	}
void NxsAssumptionsBlock::WriteLinkCommand(std::wostream &out) const
	{
	if ( (taxa && !(taxa->GetTitle().empty()))
		|| (treesBlockPtr  && !(treesBlockPtr->GetTitle().empty()))
		|| (charBlockPtr && !(charBlockPtr->GetTitle().empty())))
		{
		out << L"    LINK";
		if (taxa)
			out << L" TAXA = " << NxsString::GetEscaped(taxa->GetTitle());
		if (charBlockPtr)
			out << L" CHARACTERS = " << NxsString::GetEscaped(charBlockPtr->GetTitle());
		if (treesBlockPtr)
			out << L" TREES = " << NxsString::GetEscaped(treesBlockPtr->GetTitle());
		out << L";\n";
		}
	}
	
VecBlockPtr NxsAssumptionsBlock::GetCreatedTaxaBlocks()
	{
	passedRefOfOwnedBlock = true;
	VecBlockPtr r;
	VecAssumpBlockPtr::iterator bIt = createdSubBlocks.begin();
	for(; bIt != createdSubBlocks.end(); ++bIt)
		r.push_back(*bIt);
	return r;
	}


NxsAssumptionsBlock *NxsAssumptionsBlockFactory::GetBlockReaderForID(const std::wstring & idneeded, NxsReader *reader, NxsToken *)
	{
	if (reader == NULL || (idneeded != L"ASSUMPTIONS" && idneeded != L"CODONS" && idneeded != L"SETS"))
		return NULL;
	NxsAssumptionsBlock * nb =	new NxsAssumptionsBlock(NULL);
	nb->SetImplementsLinkAPI(true);
	return nb;
	}
