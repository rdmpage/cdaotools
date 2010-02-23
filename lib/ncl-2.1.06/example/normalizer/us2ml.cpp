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

#include <cstdio>
#include "ncl/ncl.h"
#include "ncl/nxsblock.h"
#include "ncl/nxspublicblocks.h"

#if defined HAVE_CONFIG
#	include "config.h"
#endif
#if ! defined(NCL_NAME_AND_VERSION)
#	define NCL_NAME_AND_VERSION "NCL unknown version"
#endif

void writeAsNexml(PublicNexusReader & nexusReader, wostream & os);


class NexmlIDStrorer;

typedef std::pair<const NxsDiscreteDatatypeMapper *, std::vector<std::wstring> > MapperStateLabelVec;
template <typename T>
std::wstring getOrGenId(std::pair<T, unsigned> & p, std::map<T, std::wstring> & toId, std::map<std::wstring, T> & toObj, const std::wstring & pref);

typedef std::pair<const NxsTaxaBlock *, unsigned> TaxaBlockPtrIndPair;
typedef std::pair<const NxsCharactersBlock *, unsigned> CharBlockPtrIndPair;
typedef std::pair<const NxsTreesBlock *, unsigned> TreesBlockPtrIndPair;
typedef std::pair<MapperStateLabelVec, unsigned> MapperStateLabelVecIndPair;

void writeOTUS(wostream & os, const NxsTaxaBlock *, const std::vector<const NxsAssumptionsBlock *> & assumps, NexmlIDStrorer &, unsigned );
void writeCharacters(wostream & os, const NxsCharactersBlock *, const std::vector<const NxsAssumptionsBlock *> & assumps, NexmlIDStrorer &, unsigned);
void writeTrees(wostream & os, const NxsTreesBlock *, const std::vector<const NxsAssumptionsBlock *> & assumps, NexmlIDStrorer &, unsigned);

typedef std::pair<std::wstring, std::wstring> AttributeData;
typedef std::vector<AttributeData> AttributeDataVec;

void writeAttribute(wostream & out, const AttributeData & aIt);
void writeAttributeValue(wostream & out, const std::wstring & v);




std::wstring generateID(std::wstring prefix, unsigned n)
	{
	wchar_t tmp[81];
	wprintf(tmp, L"%u", n);
	prefix.append(tmp);
	return prefix;
	}


template <typename T>
std::wstring getOrGenId(std::pair<T, unsigned> & p, std::map<T, std::wstring> & toId, std::map<std::wstring, T> & toObj, const std::wstring & pref)
{
	typedef typename std::map<T, std::wstring>::const_iterator ToIDIterator;
	T & t = p.first;
	ToIDIterator f = toId.find(t);
	if (f == toId.end())
		{
		unsigned index = p.second;
		if (index == UINT_MAX)
			{
			wcerr << L"could not find ID for NEXUS object";
			exit(3);
			}
		std::wstring id;
		do 
			{
			id = generateID(pref, index++);
			}
		while (toObj.find(id) != toObj.end());
		toId[t] = id;
		toObj[id] = t;
		return id;
		}
	return f->second;
}


class NexmlIDStrorer
	{
	public:
		
		std::wstring getID(TaxaBlockPtrIndPair taxa)
			{
			const std::wstring pref(L"t");
			return getOrGenId<const NxsTaxaBlock *>(taxa, taxaBToID, idToTaxaB, pref);
			}
		std::wstring getID(CharBlockPtrIndPair chars)
			{
			const std::wstring pref(L"c");
			return getOrGenId<const NxsCharactersBlock *>(chars, charsBToID, idToCharsB, pref);
			}
		std::wstring getID(TreesBlockPtrIndPair trees)
			{
			const std::wstring pref(L"g");
			return getOrGenId<const NxsTreesBlock *>(trees, treesBToID, idToTreesB, pref);
			}
		std::wstring getID(TaxaBlockPtrIndPair taxa, unsigned taxonInd)
			{
			const std::wstring pref(L"t");
			std::wstring p =  getOrGenId<const NxsTaxaBlock *>(taxa, taxaBToID, idToTaxaB, pref);
			p.append(1, 'n');
			return generateID(p, taxonInd);
			}
		std::wstring getCharID(CharBlockPtrIndPair chars, unsigned charInd)
			{
			const std::wstring pref(L"c");
			std::wstring p =  getOrGenId<const NxsCharactersBlock *>(chars, charsBToID, idToCharsB, pref);
			p.append(1, 'n');
			return generateID(p, charInd);
			}
		std::wstring getID(CharBlockPtrIndPair chars, unsigned charInd)
			{
			const std::wstring pref(L"r");
			std::wstring p =  getOrGenId<const NxsCharactersBlock *>(chars, charsBToID, idToCharsB, pref);
			p.append(1, 'n');
			return generateID(p, charInd);
			}
		std::wstring getID(TreesBlockPtrIndPair trees, unsigned treeInd)
			{
			const std::wstring pref(L"g");
			std::wstring p =  getOrGenId<const NxsTreesBlock *>(trees, treesBToID, idToTreesB, pref);
			p.append(1, 'n');
			return generateID(p, treeInd);
			}
		std::wstring getID(MapperStateLabelVecIndPair m, unsigned sIndex)
			{
			const std::wstring pref(L"s");
			std::wstring p =  getOrGenId<MapperStateLabelVec>(m, mapperToID, idToMapper, pref);
			p.append(1, 'n');
			return generateID(p, sIndex);
			}
			
		
	protected:
		std::map<const NxsTaxaBlock *, std::wstring> taxaBToID;
		std::map<std::wstring, const NxsTaxaBlock *> idToTaxaB;
		std::map<const NxsCharactersBlock *, std::wstring> charsBToID;
		std::map<std::wstring, const NxsCharactersBlock *> idToCharsB;
		std::map<const NxsTreesBlock *, std::wstring> treesBToID;
		std::map<std::wstring, const NxsTreesBlock *> idToTreesB;
		std::map<MapperStateLabelVec, std::wstring> mapperToID;
		std::map<std::wstring, MapperStateLabelVec> idToMapper;
	};



void writeAttributeValue(wostream & out, const std::wstring & v)
	{
	if (v.c_str() == NULL)
		out << "\'\'";
	else 
		{
		
		if (v.find_first_of(L"\'\"&") != wstring::npos)
			{
			if (wcschr(v.c_str(), '\'') != NULL)
				{
				out << '\"';
				for (std::wstring::const_iterator cIt = v.begin(); cIt != v.end(); ++cIt)
					{
					const wchar_t & c = *cIt;
					if (c == '\"')
						out << L"&quot;";
					else if (c == '&')
						out << L"&amp;";
					else 
						out << c;
					}
				out << '\"';
				
				}
			else
				{
				out << '\'';
				for (std::wstring::const_iterator cIt = v.begin(); cIt != v.end(); ++cIt)
					{
					const wchar_t & c = *cIt;
					if (c == '&')
						out << L"&amp;";
					else 
						out << c;
					}
				out << '\'';
				}
			}
		else
			out << '\'' << v << '\'';
		}
	}






inline void writeAttribute(wostream & out, const AttributeData & aIt)
	{
	out << ' ' << aIt.first << '=';
	writeAttributeValue(out, aIt.second);
	}


class XMLElement
{
	public:
	
	XMLElement(const wchar_t *name, wostream &outstream, bool hasSubElements, const wchar_t *indentStr, const AttributeDataVec *ovec=NULL)
		:out(outstream),
		elementName(name)
		,containsElements(hasSubElements)
		,indentation(indentStr)
		{
		if (ovec)
			this->open(*ovec);
		}
		
	void open()
		{
		const std::vector<AttributeData> atts;
		this->open(atts);
		}
	void open(const std::vector<AttributeData> &atts)
		{
		out << indentation << L"<" << this->elementName;
		std::vector<AttributeData>::const_iterator aIt = atts.begin();
		for (; aIt != atts.end(); ++aIt)
			{
			writeAttribute(out, *aIt);
			}
		if (containsElements)
			out << L">\n";
		else
			out << L"/>\n";
		
		}
	virtual ~XMLElement()
		{
		if (containsElements)
			out << indentation << L"</" << this->elementName << L">\n";
		}
	protected:
		wostream & out;
		const std::wstring elementName;
		bool containsElements;
		const wchar_t *indentation;
};

const wchar_t * getNexmlCharPref(NxsCharactersBlock::DataTypesEnum dt)
{
	if (dt == NxsCharactersBlock::standard)
		return L"nex:Standard";
	if (dt == NxsCharactersBlock::dna)
		return L"nex:Dna";
	if (dt == NxsCharactersBlock::rna)
		return L"nex:Rna";
	if (dt == NxsCharactersBlock::protein)
		return L"nex:Protein";
	if (dt == NxsCharactersBlock::continuous)
		return L"nex:Continuous";
	wcerr << L"Mixed and Nucleotide data (int " << int(dt) <<L") type not supported for nexml output\n";
	exit(2);
}

std::wstring getNexmlCharSeqType(NxsCharactersBlock::DataTypesEnum dt)
{
	std::wstring p(getNexmlCharPref(dt));
	p.append(L"Seqs");
	return p;
}

std::wstring getNexmlCharCellsType(NxsCharactersBlock::DataTypesEnum dt)
{
	std::wstring p(getNexmlCharPref(dt));
	p.append(L"Cells");
	return p;
}

class IDLabelledElement: public XMLElement
{
	public:
		IDLabelledElement(const wchar_t *elN, std::wstring id, std::wstring titleStr, wostream &outstream, bool contains, const wchar_t *indent, const AttributeDataVec *ovec=NULL)
			:XMLElement(elN, outstream, contains, indent)
			{
			AttributeDataVec v;
			v.push_back(AttributeData(L"id", id));
			if (!titleStr.empty())
				v.push_back(AttributeData(L"label", titleStr));
			if (ovec)
				v.insert(v.end(), ovec->begin(), ovec->end());
			XMLElement::open(v);
			}
};

class OtusElement: public IDLabelledElement
{
	public:
		OtusElement(std::wstring id, std::wstring titleStr, wostream &outstream, const AttributeDataVec *ovec=NULL)
			:IDLabelledElement(L"otus", id, titleStr, outstream, true, L"  ", ovec)
			{}
};

class OtuElement: public IDLabelledElement
{
	public:
		OtuElement(std::wstring id, std::wstring titleStr, wostream &outstream, const AttributeDataVec *ovec=NULL)
			:IDLabelledElement(L"otu", id, titleStr, outstream, false, L"    ", ovec)
			{}			
};

class OtherObjLinkedElement : public XMLElement
{
	public:
		OtherObjLinkedElement(const wchar_t  * elN, std::wstring id, std::wstring titleStr, const wchar_t *otherAttN, std::wstring otherID, wostream &outstream, bool contains, const wchar_t * indent, const AttributeDataVec * att)
			:XMLElement(elN, outstream, contains, indent)
			{
			AttributeDataVec v;
			v.push_back(AttributeData(L"id", id));
			v.push_back(AttributeData(otherAttN, otherID));
			if (!titleStr.empty())
				v.push_back(AttributeData(L"label", titleStr));
			if (att)
				v.insert(v.end(), att->begin(), att->end());
			XMLElement::open(v);
			}
};

class OTULinkedElement: public OtherObjLinkedElement
{
	public:
		OTULinkedElement(const wchar_t  * elN, std::wstring id, std::wstring titleStr, std::wstring taxaBlockID, wostream &outstream, bool contains, const wchar_t * indent, const AttributeDataVec * att)
			:OtherObjLinkedElement(elN, id, titleStr, L"otu", taxaBlockID, outstream, contains, indent, att)
			{}
};

class OTUSLinkedElement: public OtherObjLinkedElement
{
	public:
		OTUSLinkedElement(const wchar_t  * elN, std::wstring id, std::wstring titleStr, std::wstring taxaBlockID, wostream &outstream, bool contains, const wchar_t * indent, const AttributeDataVec * att)
			:OtherObjLinkedElement(elN, id, titleStr, L"otus", taxaBlockID, outstream, contains, indent, att)
			{}
};
class CharactersElement: public OTUSLinkedElement
{
	public:
		CharactersElement(std::wstring id, std::wstring titleStr, std::wstring taxaBlockID, wostream &outstream, const AttributeDataVec * att)
			:OTUSLinkedElement(L"characters", id, titleStr, taxaBlockID, outstream, true, L"  ", att)
			{}			
};


void writeAsNexml(PublicNexusReader & nexusReader, wostream & os)
{
	os << L"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n<nex:nexml xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.nexml.org/1.0 http://www.nexml.org/1.0/nexml.xsd\" xmlns:nex=\"http://www.nexml.org/1.0\" ";
	os << L"xmlns:xml=\"http://www.w3.org/XML/1998/namespace\" version=\"1.0\" generator=\"NEX_us2ml " << NCL_NAME_AND_VERSION << L"\">";
	
	const unsigned nTaxaBlocks = nexusReader.GetNumTaxaBlocks();
	NexmlIDStrorer memo;
	unsigned nCharBlocksRead = 0;
	unsigned nTreeBlocksRead = 0;
	
	for (unsigned t = 0; t < nTaxaBlocks; ++t)
		{
		const NxsTaxaBlock * tb = nexusReader.GetTaxaBlock(t);

		std::vector<const NxsAssumptionsBlock *> assumps;
		for (unsigned j= 0; j < nexusReader.GetNumAssumptionsBlocks(tb); ++j)
			assumps.push_back(nexusReader.GetAssumptionsBlock(tb, j));

		writeOTUS(os, tb, assumps, memo, t);

		const unsigned nCharBlocks = nexusReader.GetNumCharactersBlocks(tb);
		for (unsigned i = 0; i < nCharBlocks; ++i)
			{
			NxsCharactersBlock * cb = nexusReader.GetCharactersBlock(tb, i);

			assumps.clear();
			for (unsigned j= 0; j < nexusReader.GetNumAssumptionsBlocks(cb); ++j)
				assumps.push_back(nexusReader.GetAssumptionsBlock(cb, j));
			
			writeCharacters(os, cb, assumps, memo, nCharBlocksRead++);
			}

		const unsigned nTreesBlocks = nexusReader.GetNumTreesBlocks(tb);
		for (unsigned i = 0; i < nTreesBlocks; ++i)
			{
			NxsTreesBlock * cb = nexusReader.GetTreesBlock(tb, i);

			assumps.clear();
			for (unsigned j= 0; j < nexusReader.GetNumAssumptionsBlocks(cb); ++j)
				assumps.push_back(nexusReader.GetAssumptionsBlock(cb, j));

			writeTrees(os, cb, assumps, memo, nTreeBlocksRead++);
			}
		}
	
	os << L"</nex:nexml>\n";
}


void writeOTUS(wostream & os, const NxsTaxaBlock *taxa, const std::vector<const NxsAssumptionsBlock *> & , NexmlIDStrorer &memo, unsigned index)
{
	if (taxa == NULL)
		return;
	TaxaBlockPtrIndPair tbp(taxa, index);
	std::wstring taxaBlockID = memo.getID(tbp);
	std::wstring title = taxa->GetTitle();
	OtusElement otus(taxaBlockID, title, os);

	const std::vector<std::wstring> labels = taxa->GetAllLabels();
	std::vector<std::wstring>::const_iterator labelIt = labels.begin();
	unsigned n = 0;
	for (; labelIt != labels.end(); ++labelIt, ++n)
		{
		std::wstring taxonId = memo.getID(tbp, n);
		OtuElement o(taxonId, *labelIt, os);
		}
}


void writeCharLabels(wostream & os, const NxsCharactersBlock *cb, NexmlIDStrorer &memo, unsigned index, std::vector<std::wstring> * indToStatesID)
{
	CharBlockPtrIndPair cbp(cb, index);
	const unsigned nchars = cb->GetNumChar();
	for (unsigned i = 0; i < nchars; ++i)
		{
		NxsString label = cb->GetCharLabel(i);
		if (label != L" " || indToStatesID)
			{
			std::wstring charId = memo.getCharID(cbp, i);
			if (indToStatesID)
				{
				std::wstring labelToShow;
				if (label != L" ")
					labelToShow = label;
				std::wstring statesID = (*indToStatesID)[i];
				AttributeDataVec v(1, AttributeData(L"states", statesID));
				IDLabelledElement c(L"char", charId, labelToShow, os, false, L"      ", &v);
				}
			else
				IDLabelledElement c(L"char", charId, label, os, false, L"      ");
			}
		}
}

std::vector<std::wstring> getStateLabesVec(const NxsCharactersBlock *cb, unsigned charIndex, const NxsDiscreteDatatypeMapper *mapper)
{
	const unsigned nst = mapper->GetNumStates();
	unsigned i = 0;
	std::vector<std::wstring> v;
	const std::wstring emptyString;
	for (unsigned n = 0; n < nst; ++n)
		{
		std::wstring l = cb->GetStateLabel(charIndex, n);
		if (l != L" ")
			{
			if (i > 0)
				{
				for (unsigned j = 0; j < i; ++j)
					v.push_back(emptyString);
				i = 0;
				}
			v.push_back(l);
			}
		else
			i++;
		}
	return v;
}


const wchar_t * gDigits = L"0123456789";
const wchar_t * gLetters = L"abcdefghijklmnopqrstuvwxyz";

////////////////////////////////////////////////////////////////////////////////
// Creates a new states element and returns its ID.  
// 	the id's of the each state within the states element will be the states
//	element's ID + 's' + the state index.
//	except that that:
//		the missing state's id will have the last state code's index 
//			(mapper->GetNumStateCodes() - 1).
//		the gap state's id will have the next-to-last state code's index 
//			(mapper->GetNumStateCodes() - 2).
////////////////////////////////////////////////////////////////////////////////
std::wstring writeStatesElement(const MapperStateLabelVec & mslp, wostream &os, NexmlIDStrorer &memo, unsigned charIndex, unsigned statesIndex)
{
	MapperStateLabelVecIndPair m(mslp, charIndex);
	const std::wstring emptyString;
	const std::wstring id = memo.getID(m, statesIndex);
	IDLabelledElement statesElement(L"states", id, emptyString, os, true, L"        ", NULL);
	const NxsDiscreteDatatypeMapper * mapper = mslp.first;
	std::vector<std::wstring> symbols;
	
	// figure out what symbols to use (in almost all cases this will be the NEXUS symbols
	//	but it is possible that (if the matrix was entered in TOKENS mode) there 
	//	will not be NEXUS symbols for all states -- in this case we'll just numbre states.
	
	const unsigned nsc = mapper->GetNumStateCodes();
	const bool hasGap = (mapper->GetNumStatesIncludingGap() > mapper->GetNumStates());
	const int endNum = (hasGap ? nsc - 2: nsc - 1); // minus one for the missing state and one more for the gap state (if used)

	/*
	Commented out code for using NEXUS symbols -- apparently symbol has to be a number for standard types
	(and we only use this for standard types 
	bool useNexusSymbols = true;
	for (int i = 0; useNexusSymbols && i < endNum; ++i)
		{
		//cerr << i << '\n';
		std::wstring s = mapper->StateCodeToNexusString(i, false);
		if (s.length() != 1)
			useNexusSymbols = false;
		else
			symbols.push_back(string(1, s[0]));
		}
	if (useNexusSymbols)
		symbols.push_back(string(1, mapper->GetGapSymbol()));
	if (useNexusSymbols)
		symbols.push_back(string(1, mapper->GetMissingSymbol()));
	const int unnumberedCutoff = 36; // ndigits + nletters


	*/
	
	const bool useNexusSymbols = false;
	const unsigned unnumberedCutoff = 10;
	if (!useNexusSymbols)
		{
		symbols.clear();
		if (nsc <= unnumberedCutoff)
			{
			for (int i = 0; i < 10 && (int) symbols.size() < endNum; ++i)
				symbols.push_back(std::wstring(1, gDigits[i]));
			for (int i = 0; i < 26 && (int) symbols.size() < endNum; ++i)
				symbols.push_back(std::wstring(1, gLetters[i]));
			}
		else
			{
			for (int i = 0; i < endNum; ++i)
				symbols.push_back(generateID(emptyString, i));
			}
		/*
		if (hasGap)
			symbols.push_back("-");
		symbols.push_back("?");
		*/
		if (hasGap)
			symbols.push_back(generateID(emptyString, symbols.size()));
		symbols.push_back(generateID(emptyString, symbols.size()));
		}
	/// now we write the "fundamental" states
	const unsigned nst = mapper->GetNumStates();
	const std::vector<std::wstring> & sl = mslp.second;
	std::wstring stateIDpref = id;
	stateIDpref.append(1, 's');
	for (unsigned i = 0; i < nst; ++i)
		{
		const std::wstring stateID = generateID(stateIDpref, i);
		const std::wstring label = (i < sl.size() ? sl[i] : emptyString);
		AttributeDataVec v(1, AttributeData(L"symbol", symbols[i]));
		IDLabelledElement c(L"state", stateID, label, os, false, L"          ", &v);
		}
	/// now we write the state sets:
	std::wstring gapStateID;
	if (hasGap)
		gapStateID = generateID(stateIDpref, nsc - 2);
	std::wstring missingStateID = generateID(stateIDpref, nsc - 1);
	/// now we deal with the gap "state"
	if (hasGap)
		{
		const std::wstring label(L"Gap");
		AttributeDataVec v(1, AttributeData(L"symbol", symbols[nsc - 2]));
		IDLabelledElement c(L"state", gapStateID, label, os, false, L"          ", &v);
		}
	/// now we deal with the gap "state"
	if (hasGap)
		{
		const std::wstring label(L"Missing");
		AttributeDataVec v(1, AttributeData(L"symbol", symbols[nsc - 1]));
		IDLabelledElement c(L"state", missingStateID, label, os, false, L"          ", &v);
		}
	for (int polyuncertain = 0; polyuncertain < 2; ++polyuncertain)
		{
		for (int i = nst; i < endNum; ++i)
			{
			const bool isPoly = mapper->IsPolymorphic(i);
			if ((isPoly && (polyuncertain == 0)) || ((!isPoly) && (polyuncertain == 1)))
				{
				const wchar_t * elName = (isPoly ? L"polymorphic_state_set" : L"uncertain_state_set");
				const std::wstring stateID = generateID(stateIDpref, i);
				AttributeDataVec v(1, AttributeData(L"symbol", symbols[i]));
				IDLabelledElement stateSetElement(elName, stateID, emptyString, os, true, L"          ", &v);
				const std::set<int>	 & ss = mapper->GetStateSetForCode(i);
				for (std::set<int>::const_iterator subStateIt = ss.begin(); subStateIt != ss.end(); ++subStateIt)
					{
					const int subStateCode = *subStateIt;
					wstring subStateID;
					if (subStateCode < 0)
						{
						if (subStateCode == NXS_GAP_STATE_CODE)
							subStateID = gapStateID;
						else
							{
							wcerr << L"unexpected negative state code\n";
							exit(4);
							}
						}
					else
						subStateID = generateID(stateIDpref, (unsigned) subStateCode);
					AttributeDataVec v2(1, AttributeData(L"state", subStateID));
					XMLElement ss2(L"member", os, false, L"            ", &v2);
					}
				}
			}
		}
	return id;
}

void writeCharacters(wostream & os, const NxsCharactersBlock *cb , const std::vector<const NxsAssumptionsBlock *> & , NexmlIDStrorer &memo, unsigned index)
{
	if (!cb)
		return;
	NxsTaxaBlock * taxa  = dynamic_cast<NxsTaxaBlock *>(cb->GetTaxaBlockPtr(NULL));
	if (!taxa)
		return;
	TaxaBlockPtrIndPair tbp(taxa, UINT_MAX);
	std::wstring taxaBlockID = memo.getID(tbp);
	CharBlockPtrIndPair cbp(cb, index);
	std::wstring charBlockID = memo.getID(cbp);
	std::wstring title = cb->GetTitle();
	
	NxsCharactersBlock::DataTypesEnum dt = cb->GetDataType();
	const unsigned nchars = cb->GetNumChar();
		
	if (dt == NxsCharactersBlock::standard || cb->GetNumUserEquates() > 0)
		{
		if (dt != NxsCharactersBlock::standard)
			{
			wcerr << L"Warning: user defined equates causing the coercion of " << getNexmlCharCellsType(dt) << L" type to nex:StandardCells.\n";
			dt = NxsCharactersBlock::standard;
			}

		std::wstring dtStr = getNexmlCharCellsType(dt);
		AttributeDataVec atts(1, AttributeData(L"xsi:type", dtStr));
		CharactersElement charEl(charBlockID, title, taxaBlockID, os, &atts);
		std::vector<std::wstring> statesIDVec;
		if (true)
			{
			XMLElement format(L"format", os, true, L"    ");
			format.open();
			typedef std::map<MapperStateLabelVec, std::wstring> MSLToId;
			MSLToId mappersUsed;
			for (unsigned i = 0; i < nchars; ++i)
				{
				const NxsDiscreteDatatypeMapper * mapper = cb->GetDatatypeMapperForChar(i);
				std::vector<std::wstring> lv = getStateLabesVec(cb, i, mapper);
				MapperStateLabelVec mslPair(mapper, lv);
				MSLToId::const_iterator prev = mappersUsed.find(mslPair);
				const bool wse = (prev == mappersUsed.end());
				std::wstring sid;
				if (wse)
					{
					sid = writeStatesElement(mslPair, os, memo, index, mappersUsed.size());
					mappersUsed[mslPair] = sid;
					}
				else
					sid = prev->second;
				statesIDVec.push_back(sid);
				}
			writeCharLabels(os, cb, memo, index, &statesIDVec);
			}
		if (true)
			{
			XMLElement mat(L"matrix", os, true, L"    ");
			mat.open();
			
			const std::vector<std::wstring> labels = taxa->GetAllLabels();
			std::vector<std::wstring>::const_iterator labelIt = labels.begin();
			unsigned n = 0;
			const std::wstring emptyString;
			for (; labelIt != labels.end(); ++labelIt, ++n)
				{
				if (cb->TaxonIndHasData(n))
					{
					std::wstring rowId = memo.getID(cbp, n);
					std::wstring otuId = memo.getID(tbp, n);
					OTULinkedElement row(L"row", rowId,  emptyString, otuId, os, true, L"      ", NULL);
					AttributeDataVec csAtts;
					csAtts.push_back(AttributeData(L"char",emptyString));
					csAtts.push_back(AttributeData(L"state",emptyString));
					AttributeData & charAttribute = csAtts[0];
					AttributeData & stateAttribute = csAtts[1];
					const NxsDiscreteStateRow & dataRow =  cb->GetDiscreteMatrixRow(n);
					for (unsigned k = 0; k < nchars; ++k)
						{
						charAttribute.second = memo.getCharID(cbp, k);
						const int sc = dataRow[k];
						unsigned nexmlStatesIndex = 0;
						if (sc >= 0)
							nexmlStatesIndex = (unsigned) sc; 
						else
							{
							const NxsDiscreteDatatypeMapper * mapper = cb->GetDatatypeMapperForChar(k);
							const unsigned nsc = mapper->GetNumStateCodes();
							if (sc == NXS_GAP_STATE_CODE)
								nexmlStatesIndex = nsc - 2; 
							else if (sc == NXS_MISSING_CODE)
								nexmlStatesIndex = nsc - 1; 
							else
								{
								wcerr << L"Unknown state code " << sc << '\n';
								exit(5);
								}
							}
						std::wstring stateIDpref = statesIDVec[k];
						stateIDpref.append(1, 's');
						stateAttribute.second = generateID(stateIDpref, nexmlStatesIndex);
						XMLElement(L"cell", os, false, L"        ", &csAtts);
						}
					}
				}
			}
		}
	else
		{
		std::wstring dtStr = getNexmlCharSeqType(dt);
		AttributeDataVec atts(1, AttributeData(L"xsi:type", dtStr));
		CharactersElement charEl(charBlockID, title, taxaBlockID, os, &atts);
		if (cb->HasCharLabels())
			{
			XMLElement format(L"format", os, true, L"    ");
			format.open();
			writeCharLabels(os, cb, memo, index, NULL);
			}
		if (true)
			{
			XMLElement mat(L"matrix", os, true, L"    ");
			mat.open();
			
			const std::vector<std::wstring> labels = taxa->GetAllLabels();
			std::vector<std::wstring>::const_iterator labelIt = labels.begin();
			unsigned n = 0;
			const std::wstring emptyString;
			for (; labelIt != labels.end(); ++labelIt, ++n)
				{
				if (cb->TaxonIndHasData(n))
					{
					std::wstring rowId = memo.getID(cbp, n);
					std::wstring otuId = memo.getID(tbp, n);
					OTULinkedElement row(L"row", rowId,  emptyString, otuId, os, true, L"      ", NULL);
					if (true)
						{
						os << L"        <seq>";
						cb->WriteStatesForTaxonAsNexus(os, n, 0, nchars);
						os << "L</seq>\n";
						}
					}
				}
			}
		}
}

std::wstring writeSimpleNode(wostream & os, const NxsSimpleNode &nd, const TaxaBlockPtrIndPair & taxa, unsigned nodeIndex, NexmlIDStrorer &memo, AttributeDataVec*oatts)
{
	AttributeDataVec v;
	std::wstring prefix(L"n");
	unsigned otuInd = nd.GetTaxonIndex();
	std::wstring otuID;
	std::wstring label;
	std::wstring id = generateID(prefix, nodeIndex);
	if (otuInd != UINT_MAX)
		v.push_back(AttributeData(L"otu", memo.getID(taxa, otuInd)));
	else
		label = nd.GetName();
	if (oatts)
		v.insert(v.end(), oatts->begin(), oatts->end());
	IDLabelledElement nodeEl (L"node", id, label, os, false, L"      ", &v);
	return id;
}

std::wstring writeSimpleEdge(wostream & os, const NxsSimpleNode *nd, std::map<const NxsSimpleNode *, std::wstring>  & ndToIdMap, bool edgesAsIntegers)
{
	const NxsSimpleEdge edge = nd->GetEdgeToParent();
	bool defEdgeLen = edge.EdgeLenIsDefaultValue();
	assert(edge.GetChild() == nd);
	std::wstring eid(1, 'e');
	const std::wstring & nid = ndToIdMap[nd];
	eid.append(nid);
	NxsString lenstring;
	AttributeDataVec v;
	if (edgesAsIntegers)
		lenstring << edge.GetIntEdgeLen();
	else
		lenstring << edge.GetDblEdgeLen();
	if (!defEdgeLen)
		v.push_back(AttributeData(L"length", lenstring));
	v.push_back(AttributeData(L"target", nid));
	const NxsSimpleNode * par = edge.GetParent();
	assert(par);
	assert(ndToIdMap.find(par) != ndToIdMap.end());
	v.push_back(AttributeData(L"source", ndToIdMap[par]));
	IDLabelledElement edgeEl(L"edge", eid, std::wstring(), os, false, L"      ", &v);
	return eid;
}
void writeTrees(wostream & os, const NxsTreesBlock *tb, const std::vector<const NxsAssumptionsBlock *> & , NexmlIDStrorer &memo, unsigned index)
{
	if (!tb)
		return;
	const NxsTaxaBlock * taxa  = dynamic_cast<const NxsTaxaBlock *>(tb->GetTaxaBlockPtr(NULL));
	if (!taxa)
		return;
	TaxaBlockPtrIndPair tbp(taxa, UINT_MAX);
	std::wstring taxaBlockID = memo.getID(tbp);
	TreesBlockPtrIndPair treesbp(tb, index);
	std::wstring treesBlockID = memo.getID(treesbp);
	std::wstring title = tb->GetTitle();
	const unsigned ntrees = tb->GetNumTrees();
	if (ntrees == 0)
		return;
	OTUSLinkedElement treesEl(L"trees", treesBlockID, title, taxaBlockID, os, true, L"  ", NULL);
	tb->ProcessAllTrees();
	for (unsigned treen = 0; treen < ntrees; ++treen)
		{
		const NxsFullTreeDescription &ftd = tb->GetFullTreeDescription(treen);
		const bool edgesAsIntegers = ftd.EdgeLengthsAreAllIntegers();
		const wchar_t * treeType = (edgesAsIntegers ?  L"nex:IntTree": L"nex:FloatTree" );
		std::wstring id = memo.getID(treesbp, treen);
		AttributeDataVec treeAtts(1, AttributeData(L"xsi:type", std::wstring(treeType)));
		IDLabelledElement treeEl(L"tree", id, ftd.GetName(), os, true, L"    ", &treeAtts);
		NxsSimpleTree tree(ftd, INT_MAX, DBL_MAX);
		std::vector<const NxsSimpleNode *> preorder = tree.GetPreorderTraversal();
		std::vector<const NxsSimpleNode *>::const_iterator ndIt = preorder.begin();
		std::map<const NxsSimpleNode *, std::wstring> nodeToIDMap;
		unsigned nodeIndex = 0;
		if (ndIt != preorder.end())
			{
			AttributeDataVec rootAtts;
			wstring rv(ftd.IsRooted() ? L"true" : L"false");
			rootAtts.push_back(AttributeData(L"root", rv));
			const NxsSimpleNode * nd = *ndIt;
			nodeToIDMap[nd] = writeSimpleNode(os, *nd, tbp, nodeIndex++, memo, &rootAtts);
			++ndIt;
			for (; ndIt != preorder.end(); ++ndIt)
				{
				nd = *ndIt;
				nodeToIDMap[nd] = writeSimpleNode(os, *nd, tbp, nodeIndex++, memo, NULL);
				}
			}
		ndIt = preorder.begin();
		nodeIndex = 0;
		if (ndIt != preorder.end())
			{
			const NxsSimpleNode * nd = *ndIt;
			const NxsSimpleEdge edge = nd->GetEdgeToParent();
			bool defEdgeLen = edge.EdgeLenIsDefaultValue();
			if (!defEdgeLen)
				{
				std::wstring eid(1, 'e');
				const std::wstring & nid = nodeToIDMap[nd];
				eid.append(nid);
				NxsString lenstring;
				AttributeDataVec v;
				if (edgesAsIntegers)
					lenstring << edge.GetIntEdgeLen();
				else
					lenstring << edge.GetDblEdgeLen();
				v.push_back(AttributeData(L"length", lenstring));
				v.push_back(AttributeData(L"target", nid));
				IDLabelledElement edgeEl(L"rootedge", eid, std::wstring(), os, false, L"      ", &v);
				}
			++ndIt;
			for (; ndIt != preorder.end(); ++ndIt)
				{
				nd = *ndIt;
				writeSimpleEdge(os, nd, nodeToIDMap, edgesAsIntegers);
				}
			}
		}

}
