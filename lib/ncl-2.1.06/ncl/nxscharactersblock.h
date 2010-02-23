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

#ifndef NCL_NXSCHARACTERSBLOCK_H
#define NCL_NXSCHARACTERSBLOCK_H

#include <sstream>
#include <cfloat>
#include <climits>

#include "ncl/nxsdiscretedatum.h"
#include "ncl/nxsdefs.h"
#include "ncl/nxstaxablock.h"


class NxsTaxaBlockAPI;
class NxsAssumptionsBlockAPI;
class NxsDiscreteDatatypeMapper;

void NxsWriteSetCommand(const wchar_t*cmd, const NxsUnsignedSetMap & usetmap, std::wostream &out, const wchar_t* nameOfDef = NULL);
void NxsWritePartitionCommand(const wchar_t*cmd, const NxsPartitionsByName &partitions, std::wostream & out, const wchar_t* nameOfDef = NULL);


class NxsRealStepMatrix
	{
	public:
		typedef std::vector<double> DblVec;
		typedef std::vector<DblVec> DblMatrix;

		NxsRealStepMatrix(const std::vector<std::wstring> &symbolsOrder, const DblMatrix & mat)
			:symbols(symbolsOrder),
			matrix(mat)
			{
			}

		const std::vector<std::wstring> & GetSymbols() const
			{
			return symbols;
			}
		const DblMatrix & GetMatrix() const
			{
			return matrix;
			}
	private:
		std::vector<std::wstring> symbols;
		DblMatrix matrix;
	};

class NxsIntStepMatrix
	{
	public:
		typedef std::vector<int> IntVec;
		typedef std::vector<IntVec> IntMatrix;

		NxsIntStepMatrix(const std::vector<std::wstring> &symbolsOrder, const IntMatrix & mat)
			:symbols(symbolsOrder),
			matrix(mat)
			{
			}
		const std::vector<std::wstring> & GetSymbols() const
			{
			return symbols;
			}
		const IntMatrix & GetMatrix() const
			{
			return matrix;
			}
	private:
		std::vector<std::wstring> symbols;
		IntMatrix matrix;
	};

class NxsGeneticCodesManager
	{
	public:
		NxsGeneticCodesManager();
		void Reset() {}
		bool IsEmpty() const
			{
			return true;
			}
		void WriteGeneticCode(std::wostream &	) const
			{}
		bool IsValidCodeName(const std::wstring &cn) const;
	protected:
		std::set<std::wstring> standardCodeNames;
		std::set<std::wstring> userDefinedCodeNames;

	};


/*----------------------------------------------------------------------------------------------------------------------
 | NEXUS "types" (as in UserType and TypeSet commands) are assumptions about the costs of transformations of state (in
 |	parsimony.
 | The NxsTransformationManager class stores these types.
 |
 */
class NxsTransformationManager
	{
	public:
		typedef std::pair<int, std::set<unsigned> > IntWeightToIndexSet;
		typedef std::list<IntWeightToIndexSet> ListOfIntWeights;
		typedef std::pair<double, std::set<unsigned> > DblWeightToIndexSet;
		typedef std::list<DblWeightToIndexSet> ListOfDblWeights;

		typedef std::pair<std::wstring, std::set<unsigned> > TypeNameToIndexSet;
		typedef std::list<TypeNameToIndexSet> ListOfTypeNamesToSets;
	
	
		NxsTransformationManager()
			{
			Reset();
			}
	
		
		static int GetWeightForIndex(const ListOfIntWeights & wtset, unsigned index);
		static double GetWeightForIndex(const ListOfDblWeights & wtset, unsigned index);
		static const NxsIntStepMatrix::IntMatrix GetOrderedType(unsigned nStates);
		static const NxsIntStepMatrix::IntMatrix GetUnorderedType(unsigned nStates);
	
		const std::set<std::wstring> & GetTypeNames() const;
		const std::set<std::wstring> & GetUserTypeNames() const;
		const std::set<std::wstring> & GetStandardTypeNames() const;
		const std::wstring GetDefaultTypeName() const;
		std::set<std::wstring> GetWeightSetNames() const;
		bool  IsDoubleWeightSet(const std::wstring &) const;
		
		std::vector<double> GetDefaultDoubleWeights() const
			{
			return GetDoubleWeights(def_wtset);
			}
		std::vector<int> GetDefaultIntWeights() const
			{
			return GetIntWeights(def_wtset);
			}

		std::vector<double> GetDoubleWeights(const std::wstring &) const;
		std::vector<int> GetIntWeights(const std::wstring &) const;

		std::set<std::wstring> GetTypeSetNames() const;

		const std::wstring & GetDefaultWeightSetName() const;
		const std::wstring & GetDefaultTypeSetName() const;

		bool IsEmpty() const;
		
		bool IsValidTypeName(const std::wstring & ) const;
		bool IsStandardType(const std::wstring & ) const;
		bool IsIntType(const std::wstring & ) const;
		
		const NxsIntStepMatrix & GetIntType(const std::wstring & name) const;

		const NxsRealStepMatrix & GetRealType(const std::wstring & name) const;


		void SetDefaultTypeName(const std::wstring &);
		bool AddIntType(const std::wstring &, const NxsIntStepMatrix &);
		bool AddRealType(const std::wstring &, const NxsRealStepMatrix &);

		bool AddIntWeightSet(const std::wstring &, const ListOfIntWeights &, bool isDefault);
		bool AddRealWeightSet(const std::wstring &, const ListOfDblWeights &, bool isDefault);
	
		bool AddTypeSet(const std::wstring &, const NxsPartition &, bool isDefault);
		
		void Reset();
		
		void WriteUserType(std::wostream &out) const;
		void WriteWtSet(std::wostream &out) const;
		void WriteTypeSet(std::wostream &out) const
			{
			NxsWritePartitionCommand(L"TypeSet", typeSets, out, def_typeset.c_str());
			}

	private:
		std::set<std::wstring> standardTypeNames;
		std::set<std::wstring> userTypeNames;
		std::set<std::wstring> allTypeNames;
		std::map<std::wstring, NxsRealStepMatrix> dblUserTypes;
		std::map<std::wstring, NxsIntStepMatrix> intUserTypes;
		std::set<std::wstring> allWtSetNames;
		std::map<std::wstring, ListOfDblWeights> dblWtSets;
		std::map<std::wstring, ListOfIntWeights> intWtSets;
		NxsPartitionsByName typeSets;
		std::wstring def_wtset;
		std::wstring def_typeset;
		std::wstring def_type;
	};

inline const std::wstring NxsTransformationManager::GetDefaultTypeName() const
	{
	return def_type;
	}
inline const std::wstring & NxsTransformationManager::GetDefaultWeightSetName() const
	{
	return def_wtset;
	}
inline const std::wstring & NxsTransformationManager::GetDefaultTypeSetName() const
	{
	return def_typeset;
	}
inline const std::set<std::wstring> & NxsTransformationManager::GetTypeNames() const
	{
	return allTypeNames;
	}
inline const std::set<std::wstring> & NxsTransformationManager::GetUserTypeNames() const
	{
	return userTypeNames;
	}
inline const std::set<std::wstring> & NxsTransformationManager::GetStandardTypeNames() const
	{
	return standardTypeNames;
	}
inline bool  NxsTransformationManager::IsDoubleWeightSet(const std::wstring &s) const
	{
	const std::vector<double> d = GetDoubleWeights(s);
	return !(d.empty());
	}


class NxsCharactersBlockAPI
  : public NxsBlock, public NxsLabelToIndicesMapper
	{
	public:
		virtual unsigned	ApplyExset(NxsUnsignedSet &exset) = 0;
		virtual bool		AddNewExSet(const std::wstring &label, const NxsUnsignedSet & inds) = 0;
		virtual bool		IsRespectCase() const = 0;
		virtual unsigned	GetNCharTotal() const = 0;
		virtual NxsTransformationManager & GetNxsTransformationManagerRef() = 0;
		virtual const NxsTransformationManager & GetNxsTransformationManagerRef() const = 0;
		virtual std::vector<const NxsDiscreteDatatypeMapper *> GetAllDatatypeMappers() const = 0;
		virtual bool AddNewCodonPosPartition(const std::wstring &label, const NxsPartition & inds, bool isDefault) = 0;
		virtual std::wstring GetDefaultCodonPosPartitionName() const = 0;
		virtual NxsPartition GetCodonPosPartition(const std::wstring &label) const = 0;
	};

typedef std::vector<int> NxsDiscreteStateRow;
typedef std::vector<NxsDiscreteStateRow> NxsDiscreteStateMatrix;


/*----------------------------------------------------------------------------------------------------------------------
|	NXS_INVALID_STATE_CODE is used as a flag for uninitialized or unrecognized values
|	NXS_GAP_STATE_CODE may not be found in all datatypes, but is always -2 when 
|		it present.
|	NXS_MISSING_CODE is always -1.  It must be distinguished from the ambiguous set of all states because ? does not
|		mean that a new state could necessarily be present.  This arises is PAUP-style symbols extensions to the 
|		built-in datatypes.  If you say FORMAT DATATYPE=DNA SYMBOLS="01" ; then the valid symbols become "ACGT01"
|		See AugmentedSymbolsToMixed.
*/
enum {
	NXS_INVALID_STATE_CODE = -3, /* this must be kept negative */
	NXS_GAP_STATE_CODE = -2, /* this must be kept negative */
	NXS_MISSING_CODE = -1 /* this must be kept negative */
	};
	
class NxsCodonTriplet {
	public:
		unsigned wchar_t firstPos;
		unsigned wchar_t secondPos;
		unsigned wchar_t thirdPos;
		
		NxsCodonTriplet(const wchar_t*triplet);
		//////////////////////////////////////////////////////////////////////// 
		// returns for a this => other substitution a (from-base, to-base) pair or 
		// (-1,-1) for codons that differ by more than one position.
		// If codons are identical, then (0,0) will be returned.
		// 
		std::pair<int, int> getSingleMut(const NxsCodonTriplet & other) const;
		
	};
	
enum NxsGeneticCodesEnum {
	NXS_GCODE_NO_CODE = -1,
	NXS_GCODE_STANDARD = 0,
	NXS_GCODE_VERT_MITO = 1,
	NXS_GCODE_YEAST_MITO = 2,
	NXS_GCODE_MOLD_MITO = 3,
	NXS_GCODE_INVERT_MITO = 4,
	NXS_GCODE_CILIATE = 5,
	NXS_GCODE_ECHINO_MITO = 8,
	NXS_GCODE_EUPLOTID = 9,
	NXS_GCODE_PLANT_PLASTID = 10,
	NXS_GCODE_ALT_YEAST = 11,
	NXS_GCODE_ASCIDIAN_MITO = 12,
	NXS_GCODE_ALT_FLATWORM_MITO = 13,
	NXS_GCODE_BLEPHARISMA_MACRO = 14,
	NXS_GCODE_CHLOROPHYCEAN_MITO = 15,
	NXS_GCODE_TREMATODE_MITO = 20,
	NXS_GCODE_SCENEDESMUS_MITO = 21,
	NXS_GCODE_THRAUSTOCHYTRIUM_MITO = 22,
	NXS_GCODE_CODE_ENUM_SIZE = 23
	};
NxsGeneticCodesEnum geneticCodeNameToEnum(std::wstring);
std::wstring geneticCodeEnumToName(NxsGeneticCodesEnum);
std::wstring getGeneticCodeAAOrder(NxsGeneticCodesEnum codeIndex);
std::vector<std::wstring> getGeneticCodeNames();

/* structure used to store information about how the codon indices of a compressed 
(no stop codons permitted) character matrix correspond to the:
	* 64 codons in alphabetical order,
	* the amino acids
	* the codon wstrings ("AAA", L"AAC"...)
*/

class CodonRecodingStruct 
{
	public:
		std::vector<int> compressedCodonIndToAllCodonsInd;
		std::vector<int> aaInd; /* The index 0 to 20 of the amino acid for each codon -  the order of the aas is "ACDEFGHIKLMNPQRSTVWY*" */
		std::vector<std::wstring> codonStrings; /* The nucleotide abbreviations for each codon "AAA", L"AAC"... */
};

class NxsDiscreteDatatypeMapper;
/*----------------------------------------------------------------------------------------------------------------------
|	This class handles reading and storage for the NEXUS block CHARACTERS. It overrides the member functions Read and 
|	Reset, which are abstract virtual functions in the base class NxsBlock. 
|
|	Important change in starting in version 2.1:
|		The ELIMINATE command is now dealt with as if it were an automatic exclude. Previous versions of NCL were more
|		in keeping with the NEXUS specification, in that NCL did not store eliminate characters.  This resulted
|		in a confusing situation in which the Characters block maintained an original index for a character and a 
|		current index.	Some public functions of NxsCharactersBlock took arguments that needed the original character
|		index, while the vast majority of methods interpretted a character index as the current index of a character.
|		ELIMINATE commands are *very* rare in modern NEXUS files (Mesquite does not even recognize the command), thus
|		the increased complexity of the API that was caused by not storing ELIMINATED character was deemed a
|		counterproductive.
|		In NCL 2.1 (and later), the characters block stores every character, and the user of NCL can query the 
|		NxsCharactersBlock about whether a character has been excluded or not (you can also ask for the set of 
|		eliminated chararcters).  Optimizations for avoiding excluded characters are no longer the responsibility
|		of NxsCharactersBlock.	
|
|	Thus, a normal loop through all characters in the data matrix should look something 
|	like this:
|>
|	for(unsigned j = 0; j < nchar; j++)
|		{
|		if (IsExcluded(j))
|			continue;
|		.
|		.
|		.
|		}
|>
|	While there is no ELIMINATE command for taxa, it is possible that some taxa
|	in the TAXA block will not have any character data recorded.  Starting in
|	version 2.1 NCL will handle these taxa as if they were permanently 
|	delete but will not alter the taxon numbering.
|	
|	To query whether a taxon is currently deleted, use either IsActiveTaxon or 
|	IsDeleted. 
|	A normal loop across all active taxa can be constructed as follows:
|>
|	for (unsigned i = 0; i < ntax; i++)
|		{
|		if (IsDeleted(i))
|			continue;
|		.
|		.
|		.
|		}
|>
|	Below is a table showing the correspondence between the elements of a CHARACTERS block in a NEXUS file and the
|	variables and member functions of the NxsCharactersBlock class that can be used to access each piece of information
|	stored. Items in parenthesis should be viewed as "see also" items.
|>
|	NEXUS		  Command		 Data			Member
|	Command		  Atribute		 Member			Functions
|	---------------------------------------------------------------------
|	DIMENSIONS	  NEWTAXA		 newtaxa
|	
|	
|				  NCHAR			 nChar			GetNChar
|	
|	FORMAT		  DATATYPE		 datatype		GetDataType
|	
|				  RESPECTCASE	 respectingCase IsRespectCase
|	
|				  MISSING		 missing		GetMissingSymbol
|	
|				  GAP			 gap			GetGapSymbol
|	
|				  SYMBOLS		 symbols		GetSymbols
|	
|				  EQUATE		 userEquates	GetEquateKey
|												GetEquateValue
|												GetNumEquates
|	
|				  MATCHCHAR		 matchchar		GetMatchcharSymbol
|	
|				  (NO)LABELS	 labels			IsLabels
|	
|				  TRANSPOSE		 transposing	IsTranspose
|	
|				  INTERLEAVE	 interleaving	IsInterleave
|	
|				  ITEMS							GetItems
|	
|				  STATESFORMAT					GetStatesPresent
|	
|				  (NO)TOKENS	 tokens			IsTokens
|	
|	ELIMINATE					 eliminated		IsEliminated
|												GetNumEliminated
|												GetEliminatedSet
|												IsExcluded
|	MATRIX						 matrix			GetState
|												GetInternalRepresentation
|												GetNumStates
|												GetNumMatrixRows
|												GetNumMatrixCols
|												IsPolymorphic
|>
*/
class NxsCharactersBlock
  : public NxsCharactersBlockAPI, public NxsTaxaBlockSurrogate
	{
	friend class NxsAssumptionsBlock;
		
		
	public:
		typedef std::map<std::wstring, std::vector<double> > ContinuousCharCell;
		typedef std::vector<ContinuousCharCell> ContinuousCharRow;
		typedef std::vector<ContinuousCharRow> ContinuousCharMatrix;
		typedef std::vector<std::wstring> VecString;
		typedef std::map<unsigned, std::wstring> IndexToLabelMap;
		typedef std::map<std::wstring, unsigned> LabelToIndexMap;
		typedef std::pair<NxsDiscreteDatatypeMapper, NxsUnsignedSet> DatatypeMapperAndIndexSet;
		typedef std::vector<DatatypeMapperAndIndexSet> VecDatatypeMapperAndIndexSet;
		

		enum DataTypesEnum		/* values used to represent different basic types of data stored in a CHARACTERS block, and used with the data member `datatype' */
			{
			standard = 1,		/* indicates `matrix' holds characters with arbitrarily-assigned, discrete states, such as discrete morphological data */
			dna,				/* indicates `matrix' holds DNA sequences (states A, C, G, T) */
			rna,				/* indicates `matrix' holds RNA sequences (states A, C, G, U) */
			nucleotide,			/* indicates `matrix' holds nucleotide sequences */
			protein,			/* indicates `matrix' holds amino acid sequences */
			codon,				/* AAA=>0, AAC=1, AAAG=>2, AAU=>3, ACA=>4... UUU=>63 */
			continuous,			/* indicates `matrix' holds continuous data */
			mixed				/* indicates that there are multiple datatype mappers that must be used to decode the columns of the matrix (one mapper per column, but not one mapper per matrix). A MrBayes NEXUS feature*/
			};
		enum StatesFormatEnum
			{
			STATES_PRESENT = 1,
			STATE_COUNT,
			STATE_FREQUENCY,
			INDIVIDUALS
			};

		static const wchar_t* GetNameOfDatatype(DataTypesEnum);
		static std::map<wchar_t, NxsString>	GetDefaultEquates(DataTypesEnum);
		static std::wstring GetDefaultSymbolsForType(DataTypesEnum dt);

								NxsCharactersBlock(NxsTaxaBlockAPI *tb, NxsAssumptionsBlockAPI *ab);
		virtual					~NxsCharactersBlock() {}


		virtual unsigned		CharLabelToNumber(NxsString s) NCL_COULD_BE_CONST ;
		virtual unsigned		CharLabelToNumber(const std::wstring & s) const;
		virtual const std::wstring & GetBlockName() const;
		NxsString				GetCharLabel(unsigned i) const;
		bool 					HasCharLabels() const;
		std::vector<double>		GetContinuousValues(unsigned i, unsigned j, const std::wstring key) NCL_COULD_BE_CONST;
		const ContinuousCharRow & GetContinuousMatrixRow(unsigned taxNum) const;
		const NxsDiscreteDatatypeMapper * GetDatatypeMapperForChar(unsigned wchar_tIndex) const;
		std::vector<const NxsDiscreteDatatypeMapper *> GetAllDatatypeMappers() const;
		const wchar_t*			GetDatatypeName() const
			{
			return NxsCharactersBlock::GetNameOfDatatype(datatype);
			}
		unsigned GetIndexSet(const std::wstring &label, NxsUnsignedSet * toFill) const
			{
			return NxsLabelToIndicesMapper::GetIndicesFromSets(label, toFill, charSets);
			}
			
		DataTypesEnum				GetDataType() const;
		DataTypesEnum				GetOriginalDataType() const;
		const NxsDiscreteStateRow & GetDiscreteMatrixRow(unsigned taxNum) const;
		wchar_t					GetGapSymbol() const;
		void					SetGapSymbol(wchar_t);
		virtual VecBlockPtr		GetImpliedBlocks();
		unsigned				GetIndicesForLabel(const std::wstring &label, NxsUnsignedSet *inds) const;
		int						GetInternalRepresentation(unsigned i, unsigned j, unsigned k = 0) NCL_COULD_BE_CONST;
		std::vector<std::wstring> GetItems() const;
		wchar_t					GetMatchcharSymbol() NCL_COULD_BE_CONST ;
		unsigned				GetMaxIndex() const;
		virtual unsigned		GetMaxObsNumStates(bool countMissingStates=true) NCL_COULD_BE_CONST ;
		wchar_t					GetMissingSymbol() const;
		unsigned				GetNCharTotal()  ;
		unsigned				GetNCharTotal() const ;
		unsigned				GetNTaxWithData() const ;
		unsigned				GetNumActiveChar() NCL_COULD_BE_CONST ;
		unsigned				GetNumIncludedChars() const ;
		unsigned				GetNumEliminated() NCL_COULD_BE_CONST ;
		unsigned				GetNumEquates() NCL_COULD_BE_CONST ;
		unsigned				GetNumUserEquates() const;
		unsigned				GetNumMatrixCols() NCL_COULD_BE_CONST ;
		unsigned				GetNumMatrixRows() NCL_COULD_BE_CONST ;
		unsigned				GetNChar() const;
		unsigned				GetNumChar() const;
		unsigned				GetNumStates(unsigned i, unsigned j) NCL_COULD_BE_CONST ;

		// if countMissingStates is true then missing data is treated as an observation of each state
		// treats a gap as a state
		virtual unsigned		GetObsNumStates(unsigned columnIndex, bool countMissingStates=true) NCL_COULD_BE_CONST {
			return (unsigned)GetObsStates(columnIndex, countMissingStates).size();
		}

		// if countMissingStates is true then missing data is treated as an observation of each state
		// will return non-negative integers, except for NXS_GAP_STATE_CODE (which is negative).
		std::set<int>			GetObsStates(unsigned columnIndex, bool countMissingStates=true) const {
			if (countMissingStates)
				return GetMaximalStateSetOfColumn(columnIndex);
			return GetNamedStateSetOfColumn(columnIndex);
		}
		double					GetSimpleContinuousValue(unsigned i, unsigned j) NCL_COULD_BE_CONST ;
		wchar_t					GetState(unsigned i, unsigned j, unsigned k = 0) const;
		StatesFormatEnum		GetStatesFormat() const;
		NxsString				GetStateLabel(unsigned i, unsigned j) 
			{
			return GetStateLabelImpl(i, j);
			}
		NxsString				GetStateLabel(unsigned i, unsigned j) const
			{
			return GetStateLabelImpl(i, j);
			}
		unsigned				GetStateSymbolIndex(unsigned i, unsigned j, unsigned k = 0) NCL_COULD_BE_CONST;	// added by mth for standard data types
		const wchar_t				*GetSymbols() NCL_COULD_BE_CONST ;
		NxsString				GetTaxonLabel(unsigned i) const;
		NxsTransformationManager & GetNxsTransformationManagerRef()
			{
			return transfMgr;
			}
		const NxsTransformationManager & GetNxsTransformationManagerRef() const
			{
			return transfMgr;
			}
		const NxsUnsignedSet & 	GetExcludedIndexSet() const;
		bool					IsActiveChar(unsigned j) ;
		bool					IsActiveChar(unsigned j) const;
		bool					IsGapState(unsigned i, unsigned j) NCL_COULD_BE_CONST ;
		bool					IsInterleave() NCL_COULD_BE_CONST ;
		bool					IsEliminated(unsigned wchar_tIndex) NCL_COULD_BE_CONST ;
		bool					IsExcluded(unsigned j) ;
		bool					IsExcluded(unsigned j) const;
		bool					IsLabels() NCL_COULD_BE_CONST ;
		bool					IsMissingState(unsigned i, unsigned j) NCL_COULD_BE_CONST ;
		bool					IsMixedType() const;
		bool					IsPolymorphic(unsigned i, unsigned j) NCL_COULD_BE_CONST ;
		bool					IsRespectCase() const;
		bool					IsTokens() NCL_COULD_BE_CONST ;
		bool					IsTranspose() NCL_COULD_BE_CONST ;
		bool					TaxonIndHasData(const unsigned ind) const;
		virtual void			Report(std::wostream &out) NCL_COULD_BE_CONST ;
		void					ShowStateLabels(std::wostream &out, unsigned i, unsigned c, unsigned first_taxon) const;
		virtual unsigned		TaxonLabelToNumber(NxsString s) const;
		void					WriteAsNexus(std::wostream &out) const;
		
		bool					WasRestrictionDataype() const;

		virtual bool 			AddNewCodonPosPartition(const std::wstring &label, const NxsPartition & inds, bool isDefault);
		bool					AddNewIndexSet(const std::wstring &label, const NxsUnsignedSet & inds);
		bool					AddNewExSet(const std::wstring &label, const NxsUnsignedSet & inds);
		bool					AddNewPartition(const std::wstring &label, const NxsPartition & inds);
		unsigned				ApplyExset(NxsUnsignedSet &exset);
		unsigned				ApplyIncludeset(NxsUnsignedSet &inset);
		void					Consume(NxsCharactersBlock &other);
		void					ExcludeCharacter(unsigned i);
		void					IncludeCharacter(unsigned i);
		virtual void			Reset();
		void					SetNexus(NxsReader *nxsptr);
		
		void					SetSupportMixedDatatype(bool v)
			{
			supportMixedDatatype = v;
			}
		///If datatype is mixed, GetDatatypeMapForMixedType returns a map of types that occur in the matrix to sets of indices (starting at 0) of the characters that are assigned to that type.
		std::map<DataTypesEnum, NxsUnsignedSet> GetDatatypeMapForMixedType() const
			{
			return mixedTypeMapping;
			}

		virtual void			DebugShowMatrix(std::wostream &out, bool use_matchchar, const wchar_t*marginText = NULL) const;

		/*only used it the linkAPI is enabled*/
		virtual void			HandleLinkCommand(NxsToken & token);
		virtual void			WriteLinkCommand(std::wostream &out) const;
			
		
		/* Behaves like GetMaximalStateSetOfColumn except that missing data columns do not increase
			size of the returned state set.
		*/
		std::set<int>		GetNamedStateSetOfColumn(const unsigned colIndex) const;
		/* Returns the union of all states that are consistent with a column */
		std::set<int>		GetMaximalStateSetOfColumn(const unsigned colIndex) const;
		bool				AugmentedSymbolsToMixed();

		/*---------------------------------------------------------------------------------------
		| Results in aliasing of the taxa, assumptionsBlock blocks!
		*/
		NxsCharactersBlock & operator=(const NxsCharactersBlock &other)
			{
			Reset();
			CopyBaseBlockContents(static_cast<const NxsBlock &>(other));
			CopyTaxaBlockSurrogateContents(other);
			CopyCharactersContents(other);
			return *this;
			}
			
		virtual void CopyCharactersContents(const NxsCharactersBlock &other);
		virtual NxsCharactersBlock * Clone() const
			{
			NxsCharactersBlock * a = new NxsCharactersBlock(taxa, assumptionsBlock);
			*a = *this;
			return a;
			}
			
		void SetConvertAugmentedToMixed(bool v)
			{
			convertAugmentedToMixed = v;
			}
		void SetAllowAugmentingOfSequenceSymbols(bool v)
			{
			allowAugmentingOfSequenceSymbols = v;
			}
		bool GetAllowAugmentingOfSequenceSymbols() const 
			{
			return allowAugmentingOfSequenceSymbols;
			}
		void FindConstantCharacters(NxsUnsignedSet &c) const;
		void FindGappedCharacters(NxsUnsignedSet &c) const;
		const NxsDiscreteStateMatrix & GetRawDiscreteMatrixRef() const
			{
			return discreteMatrix;
			}
		void SetWriteInterleaveLen(int interleaveLen)
			{
			writeInterleaveLen = interleaveLen;
			}

		void WriteStatesForTaxonAsNexus(std::wostream &out, unsigned taxNum, unsigned begChar, unsigned endChar) const;

		void					WriteCharLabelsCommand(std::wostream &out) const;
		void					WriteCharStateLabelsCommand(std::wostream &out) const;
		void					WriteEliminateCommand(std::wostream &out) const;
		void					WriteFormatCommand(std::wostream &out) const;
		void					WriteMatrixCommand(std::wostream &out) const;

		int						GetStateIndex(unsigned i, unsigned j, unsigned k) const;


		/** converts a CodonPosPartition into the list<int> argument needed for the NewCodonsCharactersBlock call */
		static void CodonPosPartitionToPosList(const NxsPartition &codonPos, std::list<int> * charIndices);

		/* allocates a new charaters block with all of the active characters in `charBlock` 
			but with a 64-state codon datatype. The order of codons is:
			 0   1   2   3   4   5  ... 63
			AAA AAC AAG AAT ACA ACC ... TTT
			The caller is responsible for deleting the new NxsCharactersBlock object
			If charIndices is provided, it lists the bases in the RF by position the int can be < 0 to indicate that that position was not sampled 
		*/
		static NxsCharactersBlock * NewCodonsCharactersBlock(
			const NxsCharactersBlock * charBlock, 
			bool mapPartialAmbigToUnknown, 
			bool gapsToUnknown, 
			bool honorCharActive,  /* if true then inactive characters are treated as missing */
			const std::list<int> * charIndices = NULL, /* specifies the indices of the positions in the gene. -1 can be used to indicate tha codon position was not included in the original matrix */
			NxsCharactersBlock ** spareNucs = NULL /* If non-null, then, on exit the NxsCharactersBlock * pointer will refer to a new character block with all of the positions that were not translated (all of the non-coding nucleotide positions) */
			);
		static NxsCharactersBlock * NewProteinCharactersBlock(
			const NxsCharactersBlock * codonBlock, 
			bool mapPartialAmbigToUnknown,
			bool gapToUnknown,
			NxsGeneticCodesEnum codeIndex);
		static NxsCharactersBlock * NewProteinCharactersBlock(
			const NxsCharactersBlock * codonBlock, 
			bool mapPartialAmbigToUnknown,
			bool gapToUnknown,
			const std::vector<int> & aaIndices); /** the index of the amino acid symbols for the codon (where the order of codons is alphabetical: AAA, AAC, AAG, AAT, ACA, ...TTT **/

		virtual std::wstring GetDefaultCodonPosPartitionName() const {
			return defCodonPosPartitionName;
		}
		virtual NxsPartition GetCodonPosPartition(const std::wstring &label) const {
			NxsPartitionsByName::const_iterator pIt = codonPosPartitions.find(label);
			if (pIt == codonPosPartitions.end())
				return NxsPartition();
			return pIt->second;
		}
		
		unsigned NumAmbigInTaxon(const unsigned taxInd, const NxsUnsignedSet * charIndices, const bool countOnlyCompletelyMissing, const bool treatGapsAsMissing) const;
		bool FirstTaxonStatesAreSubsetOfSecond(const unsigned firstTaxonInd, const unsigned secondTaxonInd, const NxsUnsignedSet * charIndices, const bool treatAmbigAsMissing, const bool treatGapAsMissing) const;
		//Returns the number of characters that differ, and the number of positions for which both taxa were non-missing
		
		std::pair<unsigned, unsigned> GetPairwiseDist(const unsigned firstTaxonInd, const unsigned secondTaxonInd, const NxsUnsignedSet * charIndices, const bool treatAmbigAsMissing, const bool treatGapAsMissing) const;
		CodonRecodingStruct RemoveStopCodons(NxsGeneticCodesEnum);
		bool 		SwapEquivalentTaxaBlock(NxsTaxaBlockAPI * tb)
		{
			return SurrogateSwapEquivalentTaxaBlock(tb);
		}
	protected:
		// This function should not be called to remove characters, it is only used in the creation of new char blocks from existing blocks
		void					SetNChar(unsigned nc)
			{
			this->nChar = nc;
			}
		// This function should not be called to remove characters, it is only used in the creation of new char blocks from existing blocks
		void					SetNTax(unsigned nt)
			{
			this->nTaxWithData = nt;
			}
		
		NxsString				GetStateLabelImpl(unsigned i, unsigned j) const;
		void					WriteStatesForMatrixRow(std::wostream &out, unsigned taxon, unsigned first_taxon, unsigned begChar, unsigned endChar) const;

		NxsDiscreteDatatypeMapper * GetMutableDatatypeMapperForChar(unsigned wchar_tIndex);
		bool					IsInSymbols(wchar_t ch) NCL_COULD_BE_CONST ;
		void					ShowStates(std::wostream &out, unsigned i, unsigned j)  NCL_COULD_BE_CONST ;

		void					HandleCharlabels(NxsToken &token);
		void					HandleCharstatelabels(NxsToken &token);
		void					HandleDimensions(NxsToken &token, NxsString newtaxaLabel, NxsString ntaxLabel, NxsString ncharLabel);
		void					HandleEliminate(NxsToken &token);
		virtual void			HandleFormat(NxsToken &token);
		virtual void			HandleMatrix(NxsToken &token);
		bool					HandleNextContinuousState(NxsToken &token, unsigned taxNum, unsigned wchar_tNum, ContinuousCharRow & row, const NxsString & nameStr);
		bool					HandleNextDiscreteState(NxsToken &token, unsigned taxNum, unsigned wchar_tNum, NxsDiscreteStateRow & row, NxsDiscreteDatatypeMapper &, const NxsDiscreteStateRow * firstTaxonRow, const NxsString & nameStr);
		bool					HandleNextTokenState(NxsToken &token, unsigned taxNum, unsigned wchar_tNum, NxsDiscreteStateRow & row, NxsDiscreteDatatypeMapper &, const NxsDiscreteStateRow * firstTaxonRow, const NxsString & nameStr);
		void					HandleStatelabels(NxsToken &token);
		virtual void			HandleStdMatrix(NxsToken &token);
		virtual int				HandleTokenState(NxsToken &token, unsigned taxNum, unsigned wchar_tNum, NxsDiscreteDatatypeMapper &mapper, const NxsDiscreteStateRow * firstTaxonRow, const NxsString & nameStr);
		virtual void			HandleTransposedMatrix(NxsToken &token);
		virtual void			Read(NxsToken &token);
		void					ResetSymbols();

		void					WriteStates(NxsDiscreteDatum &d, wchar_t *s, unsigned slen) NCL_COULD_BE_CONST ;


		NxsAssumptionsBlockAPI	*assumptionsBlock;	/* pointer to the ASSUMPTIONS block in which exsets, taxsets and charsets are stored */

		unsigned				nChar;				/* number of columns in matrix	*/
		unsigned				nTaxWithData;		/* number of non empty rows in the matrix*/
		
		wchar_t					matchchar;			/* match symbol to use in matrix */
		bool					respectingCase;		/* if true, RESPECTCASE keyword specified in FORMAT command */
		bool					transposing;		/* indicates matrix will be in transposed format */
		bool					interleaving;		/* indicates matrix will be in interleaved format */
		mutable bool			tokens;				/* if false, data matrix entries must be single symbols; if true, multicharacter entries are allows */
		bool					labels;				/* indicates whether or not labels will appear on left side of matrix */

		wchar_t					missing;			/* missing data symbol */
		wchar_t					gap;				/* gap symbol for use with molecular data */

		std::wstring				symbols;			/* list of valid character state symbols */
		std::map<wchar_t, NxsString> userEquates;			/* list of associations defined by EQUATE attribute of FORMAT command */
		std::map<wchar_t, NxsString> defaultEquates;
		VecDatatypeMapperAndIndexSet datatypeMapperVec;
		NxsDiscreteStateMatrix	discreteMatrix;		/* storage for discrete data */
		ContinuousCharMatrix	continuousMatrix;	/* */

		NxsUnsignedSet			eliminated;			/* array of (0-offset) character numbers that have been eliminated (will remain empty if no ELIMINATE command encountered) */
		NxsUnsignedSet			excluded;			/* set of (0-offset) indices of characters that have been excluded.*/

		LabelToIndexMap			ucCharLabelToIndex;
		IndexToLabelMap			indToCharLabel;
		NxsStringVectorMap		charStates;			/* storage for character state labels (if provided) */
		NxsStringVector			globalStateLabels; /* state labels that apply to all characters (if not pre-empted by thy charStates field) */
		VecString				items;

		NxsUnsignedSetMap		charSets;
		NxsUnsignedSetMap		exSets;
		NxsPartitionsByName		charPartitions;
		NxsTransformationManager transfMgr;
		bool					datatypeReadFromFormat;
		NxsPartitionsByName		codonPosPartitions;
		std::wstring				defCodonPosPartitionName;
		std::map<DataTypesEnum, NxsUnsignedSet> mixedTypeMapping;
	private:		
		DataTypesEnum			datatype;			/* flag variable (see datatypes enum) */
		DataTypesEnum			originalDatatype;			/* flag variable (see datatypes enum) */
		StatesFormatEnum		statesFormat;
		bool					restrictionDataype;
		bool 					supportMixedDatatype;	/* (false by default) flag for whether or not MrBayes-style Mixed blocks should be supported */
		bool 					convertAugmentedToMixed; /* false by default (see AugmentedSymbolsToMixed) */
		bool					allowAugmentingOfSequenceSymbols;
		int						writeInterleaveLen;
		
		void					CreateDatatypeMapperObjects(const NxsPartition & , const std::vector<DataTypesEnum> &);
		friend class PublicNexusReader;
		friend class MultiFormatReader;
	};

typedef NxsCharactersBlock CharactersBlock;


class NxsCharactersBlockFactory
	:public NxsBlockFactory
	{
	public:
		virtual NxsCharactersBlock	*	GetBlockReaderForID(const std::wstring & id, NxsReader *reader, NxsToken *token);
	};

class NxsDiscreteStateSetInfo
	{
	public:
		NxsDiscreteStateSetInfo(const std::set<int> & stateSet, bool polymorphic=false, wchar_t symbol='\0')
			:states(stateSet),
			nexusSymbol(symbol),
			isPolymorphic(polymorphic)
			{}
			

		std::set<int>  states;
		wchar_t	  nexusSymbol;
		bool	  isPolymorphic;
	};
	
	
class NxsDiscreteDatatypeMapper
	{
	public:
		static void GenerateNxsExceptionMatrixReading(const wchar_t*, unsigned taxInd, unsigned wchar_tInd, NxsToken &, const NxsString &nameStr);
		static void GenerateNxsExceptionMatrixReading(const std::wstring &s, unsigned taxInd, unsigned charInd, NxsToken & token, const NxsString &nameStr)
			{
			GenerateNxsExceptionMatrixReading(s.c_str(), taxInd, charInd, token, nameStr);
			}


		NxsDiscreteDatatypeMapper();

		NxsDiscreteDatatypeMapper(NxsCharactersBlock::DataTypesEnum datatype, const std::wstring & symbols,
								wchar_t missingChar, wchar_t gapChar, wchar_t matchChar,
								bool respectCase, const std::map<wchar_t, NxsString> & extraEquates);
		
		NxsDiscreteDatatypeMapper(const NxsDiscreteDatatypeMapper& other)
			:datatype(other.datatype)
			{
			*this = other;
			}
		NxsDiscreteDatatypeMapper & operator=(const NxsDiscreteDatatypeMapper&);
		
		NxsCharactersBlock::DataTypesEnum GetDatatype() const 
			{
			return datatype;
			}
		unsigned		 GetNumStates() const;
		unsigned		 GetNumStatesIncludingGap() const;
		unsigned         GetNumStateCodes() const
		    {
		    return (unsigned)stateSetsVec.size();
		    }
		std::wstring      GetSymbols() const
		    {
		    return symbols;
		    }
		std::wstring      GetSymbolsWithGapChar() const
		    {
		    if (gapChar == '\0')
		    	return GetSymbols();
		    std::wstring s;
		    s = symbols;
		    s.append(1, gapChar);
		    return s;
		    }
		wchar_t             GetGapSymbol() const
		    {
		    return gapChar;
		    }
		// warning: unsafe to call after reading -- does not recode data!
		void             SetGapSymbol(wchar_t c)
		    {
		    gapChar = c;
		    }
		wchar_t             GetMissingSymbol() const
		    {
		    return missing;
		    }
		std::map<wchar_t, NxsString> GetExtraEquates() const
			{
			return extraEquates;
			}
		unsigned		 GetNumStatesInStateCode(int stateCode) const;
		int				 GetOneStateForCode(int stateCode, unsigned stateIndex) const;
		std::vector<int> GetStateVectorForCode(int stateCode) const;
		const std::set<int>	 & GetStateSetForCode(int stateCode) const;
		/* will return NXS_INVALID_STATE_CODE if the char is unknown */
		int				 GetStateCodeStored(wchar_t currChar) const 
			{
			return cLookup[currChar];
			}
		bool			 IsPolymorphic(int stateCode) const;
		int				 PositionInSymbols(const wchar_t currChar) const;
		int				 PositionInSymbolsOrGaps(const wchar_t currChar) const
			{
			if (currChar == gapChar)
				return NXS_GAP_STATE_CODE;
			return PositionInSymbols(currChar);
			}
		std::wstring		 StateCodeToNexusString(int,  bool demandSymbols = true) const;
		int				 StateCodeForNexusChar(const wchar_t currChar, NxsToken & token,
								  unsigned taxInd, unsigned wchar_tInd,
								  const NxsDiscreteStateRow * firstTaxonRow, const NxsString &nameStr) const;
		void			 WriteStartOfFormatCommand(std::wostream & out) const;
		void			 WriteStateCodeRowAsNexus(std::wostream & out, const std::vector<int> &row) const;
		void			 WriteStateCodeRowAsNexus(std::wostream & out, std::vector<int>::const_iterator & begIt, const std::vector<int>::const_iterator & endIt) const;
		void			 WriteStateCodeAsNexusString(std::wostream & out, int scode, bool demandSymbols = true) const;
		bool			 WasRestrictionDataype() const;
		void			 SetWasRestrictionDataype(bool v) {restrictionDataype = v;}
		int EncodeNexusStateString(const std::wstring &stateAsNexus, NxsToken & token,
								   const unsigned taxInd, const unsigned wchar_tInd, 
								   const NxsDiscreteStateRow * firstTaxonRow, const NxsString &nameStr);
		int StateCodeForStateSet(const std::set<int> &, const bool isPolymorphic, 
								 const bool addToLookup, const wchar_t symbol);
		
		void 			DebugPrint(std::wostream &) const;

 		bool GetUserDefinedEquatesBeforeConversion() const
 			{
 			return userDefinedEquatesBeforeConversion;
			}

		bool IsRespectCase() const
			{
			return respectCase;
			}
		
		const std::set<int> & GetStateIntersection(int stateCode, int otherStateCode) const 
			{
			if (stateIntersectionMatrix.empty())
				BuildStateIntersectionMatrix();
			const int sc = stateCode - NXS_GAP_STATE_CODE;
			const int osc = otherStateCode - NXS_GAP_STATE_CODE;
			return stateIntersectionMatrix.at(sc).at(osc);
			}

		bool FirstIsSubset(int stateCode, int otherStateCode, bool treatGapAsMissing) const 
			{
			if (isStateSubsetMatrix.empty())
				BuildStateSubsetMatrix();
			const int sc = stateCode - NXS_GAP_STATE_CODE;
			const int osc = otherStateCode - NXS_GAP_STATE_CODE;
			if (treatGapAsMissing)
				return isStateSubsetMatrixGapsMissing.at(sc).at(osc);
			return isStateSubsetMatrix.at(sc).at(osc);
			}
		
		NxsGeneticCodesEnum		geneticCode; /* only used for compressed codon codings */

	private:
		int AddStateSet(const std::set<int> & states, wchar_t nexusSymbol, bool symRespectCase, bool isPolymorphic);
		int StateCodeForNexusMultiStateSet(const wchar_t nexusSymbol, const std::wstring & stateAsNexus, NxsToken & token,
								  unsigned taxInd, unsigned wchar_tInd,
								  const NxsDiscreteStateRow * firstTaxonRow, const NxsString &nameStr);
		int StateCodeForNexusPossibleMultiStateSet(const wchar_t nexusSymbol, const std::wstring & stateAsNexus, NxsToken & token,
								  unsigned taxInd, unsigned wchar_tInd,
								  const NxsDiscreteStateRow * firstTaxonRow, const NxsString &nameStr);
								  
		void RefreshMappings(NxsToken *token);
		void ValidateStateIndex(int state) const;
		void ValidateStateCode(int state) const;
		void BuildStateSubsetMatrix() const;
		void BuildStateIntersectionMatrix() const;
		void DeleteStateIndices(const std::set<int> & deletedInds);
		
		int * cLookup; /* Nexus char to state code lookup -- alias to member of charToStateCodeLookup*/
		NxsDiscreteStateSetInfo * stateCodeLookupPtr; /* state code to NxsDiscreteStateSetInfo object table -- alias to stateSets */
		std::wstring symbols;
		std::wstring lcsymbols; /* lowercase symbols (in the same order as symbols) */
		unsigned nStates;
		wchar_t matchChar;
		wchar_t gapChar;
		wchar_t missing;
		bool respectCase;
		std::map<wchar_t, NxsString> extraEquates;
		NxsCharactersBlock::DataTypesEnum datatype;			/* flag variable (see datatypes enum) */
		std::vector<NxsDiscreteStateSetInfo> stateSetsVec; /* memory management for cLookup*/
		std::vector<int> charToStateCodeLookup; /* stateCodeLookup */
		int sclOffset; /* offset of stateCodeLookup in stateSets */
		bool restrictionDataype;
		bool userDefinedEquatesBeforeConversion;
		
		typedef std::vector< std::set<int> > StateIntersectionRow;
		typedef std::vector< StateIntersectionRow > StateIntersectionMatrix;
		typedef std::vector< bool > IsStateSubsetRow;
		typedef std::vector< IsStateSubsetRow > IsStateSubsetMatrix;
		mutable StateIntersectionMatrix stateIntersectionMatrix;
		mutable IsStateSubsetMatrix isStateSubsetMatrix;
		mutable IsStateSubsetMatrix isStateSubsetMatrixGapsMissing;

		friend class NxsCharactersBlock;
	};

inline unsigned NxsDiscreteDatatypeMapper::GetNumStatesIncludingGap() const
	{
	return nStates + (gapChar == '\0' ? 0 : 1);
	}

inline unsigned NxsDiscreteDatatypeMapper::GetNumStates() const
	{
	return nStates;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the set of state indices that correspond to the states of state code `c`
|	Generates a NxsNCLAPIException if `c` is not a valid state code.
|	Not as efficient as GetStateSetForCode
*/
inline std::vector<int>	 NxsDiscreteDatatypeMapper::GetStateVectorForCode(int c) const
	{
	const std::set<int>	 & ss = GetStateSetForCode(c);
	return std::vector<int>(ss.begin(), ss.end());
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the set of state indices that correspond to the states of state code `c`
|	Generates a NxsNCLAPIException if `c` is not a valid state code.
*/
inline const std::set<int>	& NxsDiscreteDatatypeMapper::GetStateSetForCode(int c) const
	{
	assert(stateCodeLookupPtr);
	ValidateStateCode(c);
	return stateCodeLookupPtr[c].states;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the `stateIndex`-th state for `stateCode`  Thus if stateCode = 6 and this corresponds to {AG}
|	then:
|		GetOneStateForCode(6, 0) would return 0 (assuming that A is state 0), and
|		GetOneStateForCode(6, 1) would return 2 (assuming that G is state 2 in the symbols list)
*/
inline int NxsDiscreteDatatypeMapper::GetOneStateForCode(int stateCode, unsigned stateIndex) const
	{
	const std::set<int>	 & s = GetStateSetForCode(stateCode);
	unsigned i = 0;
	for (std::set<int>::const_iterator sIt = s.begin(); sIt != s.end(); ++sIt, ++i)
		{
		if (i == stateIndex)
			return *sIt;
		}
	assert(false);
	throw NxsException(L"State index out of range in NxsDiscreteDatatypeMapper::GetOneStateForCode");
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the NEXUS reperesenation of the state code `scode` which may be a multiple character wstring such as {DNY}
|	Generates a NxsNCLAPIException if `c` is not a valid state code.
|	If insufficient symbols exist, then `demandSymbols` controls the behavior (if true then an NxsNCLAPIException
|	is raised, otherwise an empty wstring is returned.
|
|	Note that 
|		WriteStateCodeAsNexusString(out, c);
|	Is more efficient than
|		out << StateCodeToNexusString(c);
*/
inline std::wstring NxsDiscreteDatatypeMapper::StateCodeToNexusString(int scode,  bool demandSymbols) const
	{
	std::wostringstream o;
	WriteStateCodeAsNexusString(o, scode, demandSymbols);
	return o.str();
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Called from HandleStdMatrix or HandleTransposedMatrix function to read in the next state. Always returns true 
|	except in the special case of an interleaved matrix, in which case it returns false if a newline character is 
|	encountered before the next token.
*/
inline int NxsDiscreteDatatypeMapper::EncodeNexusStateString(
  const std::wstring &stateAsNexus,
  NxsToken & token, /* the token used to read from `in' */
  const unsigned taxInd,		/* the taxon index, in range [0..`ntax') */
  const unsigned charInd,		/* the character index, in range [0..`nChar') */
  const NxsDiscreteStateRow * firstTaxonRow, const NxsString &nameStr)
	{
	const unsigned tlen = (unsigned) stateAsNexus.length();
	if (tlen == 0)
		GenerateNxsExceptionMatrixReading(L"Unexpected empty token encountered", taxInd, charInd, token, nameStr);
	if (tlen == 1)
		return StateCodeForNexusChar(stateAsNexus[0], token, taxInd, charInd, firstTaxonRow, nameStr);
	return StateCodeForNexusMultiStateSet('\0', stateAsNexus, token, taxInd, charInd, firstTaxonRow, nameStr);
	}
	
/*----------------------------------------------------------------------------------------------------------------------
|	MrBayes introduced the datatype=restriction syntax for 01 symbols.
|	NCL reads this type as standard, but sets a flag.  If the datatype is reported as Standard, then you can call
|	WasRestrictionDataype to see if the datatype was declared "RESTRICTION"
*/
inline bool NxsDiscreteDatatypeMapper::WasRestrictionDataype() const 
	{
	return restrictionDataype;
	}
/*----------------------------------------------------------------------------------------------------------------------
|	MrBayes introduced the datatype=restriction syntax for 01 symbols.
|	NCL reads this type as standard, but sets a flag.  If the datatype is reported as Standard, then you can call
|	WasRestrictionDataype to see if the datatype was declared "RESTRICTION"
*/
inline bool NxsCharactersBlock::WasRestrictionDataype() const 
	{
	return restrictionDataype;
	}

inline void NxsCharactersBlock::SetNexus(NxsReader *nxsptr)
	{
	NxsBlock::SetNexus(nxsptr);
	NxsTaxaBlockSurrogate::SetNexusReader(nxsptr);
	}

inline bool NxsCharactersBlock::IsMixedType() const
	{
	return (datatypeMapperVec.size() > 1);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the list of items that will be in each cell.  This is always "STATES" for discrete datatypes, but can be
|	a vector of any wstring for continuous types
*/
inline std::vector<std::wstring> NxsCharactersBlock::GetItems() const
	{
	return items;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Accessor for getting the list of continuous values associated with an "ITEM." Usually, these vectors will have
|	length of 1, but the "STATES" item may have a list of all observed values.
|	
|	Values of DBL_MAX indicate missing data.
|	An empty vector indicates that the key was not used in this cell.
*/
inline std::vector<double> NxsCharactersBlock::GetContinuousValues(
	unsigned i,		/* the taxon in range [0..`ntax') */
	unsigned j,		/* the character in range [0..`nChar') */
	const std::wstring key)  NCL_COULD_BE_CONST /* The name of the ITEM in the FORMAT command.  Must be ALL CAPS.*/
	{
	const ContinuousCharCell & cell = continuousMatrix.at(i).at(j);
	ContinuousCharCell::const_iterator cIt = cell.find(key);
	if (cIt == cell.end())
		return std::vector<double>();
	return cIt->second;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Short cut for returning the AVERAGE item, which is the default of a continuous cell type.  Note this does not 
|	compute the average, this is just a shortcut for dealing with simple continuous matrices that do not 
|	use the ITEMS subcommand of FORMAT.
|
|	Values of DBL_MAX indicate missing data.
*/
inline double NxsCharactersBlock::GetSimpleContinuousValue(
	unsigned i,		/* the taxon in range [0..`ntax') */
	unsigned j)  NCL_COULD_BE_CONST /* the character in range [0..`nChar') */
	{
	const std::vector<double> av = GetContinuousValues(i, j, std::wstring(L"AVERAGE"));
	if (av.empty())
		return DBL_MAX;
	return av.at(0);
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Returns label for character `i' (starting at zero), if a label has been specified. If no label was specified, returns wstring 
|	containing a single blank (i.e., L" ").
*/
inline NxsString NxsCharactersBlock::GetCharLabel(
  unsigned i) const	/* the character in range [0..`nChar') */
	{
	std::map<unsigned, std::wstring>::const_iterator tlIt = indToCharLabel.find(i);
	if (tlIt == indToCharLabel.end())
		return NxsString(L" ");
	return NxsString(tlIt->second.c_str());
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if at least one character has charlabels
*/
inline bool NxsCharactersBlock::HasCharLabels() const
	{
	return !indToCharLabel.empty();
	}
/*----------------------------------------------------------------------------------------------------------------------
|	Returns the gap symbol currently in effect. If no gap symbol specified, returns '\0'.
*/
inline wchar_t NxsCharactersBlock::GetGapSymbol() const
	{
	return gap;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	//Warning: this function is unsafe -- it  only effects the writing of the matrix as NEXUS and it does not correctly
| recode the matrix.
*/
inline void NxsCharactersBlock::SetGapSymbol(wchar_t g)
	{
	gap = g;
	if (datatypeMapperVec.size() == 1)
		datatypeMapperVec[0].first.SetGapSymbol(g);
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Returns value of `datatype'.
*/
inline NxsCharactersBlock::DataTypesEnum NxsCharactersBlock::GetDataType() const
	{
	if (datatypeMapperVec.empty())
		return datatype;
	if (datatypeMapperVec.size() > 1)
		return mixed;
	return datatypeMapperVec[0].first.GetDatatype();
	}
/*----------------------------------------------------------------------------------------------------------------------
|	Returns datatype listed in the CHARACTERS block.
|	The original datatype can differ from the current datatype if the symbols list of a built in type was augmented
|	(thus converting it to standard).
*/
inline NxsCharactersBlock::DataTypesEnum NxsCharactersBlock::GetOriginalDataType() const
	{
	return originalDatatype;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the `matchchar' symbol currently in effect. If no `matchchar' symbol specified, returns '\0'.
*/
inline wchar_t NxsCharactersBlock::GetMatchcharSymbol() NCL_COULD_BE_CONST 
	{
	return matchchar;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	This function is no longer the most efficient way to access parsed data (see notes on NxsCharacterBlock and 
|	GetMatrix() and GetMatrixDecoder() methods.
|
|	Returns internal representation of the state for taxon `i', character `j'. In the normal situation, `k' is 0 meaning
|	there is only one state with no uncertainty or polymorphism. If there are multiple states, specify a number in the 
|	range [0..n) where n is the number of states returned by the GetNumStates function. Use the IsPolymorphic 
|	function to determine whether the multiple states correspond to uncertainty in state assignment or polymorphism in 
|	the taxon. The value returned from this function is one of the following:
|~
|	o -3 means gap state (see note below)
|	o -2 means missing state (see note below)
|	o an integer 0 or greater is internal representation of a state
|~
|	Note: gap and missing states are actually represented internally in a different way; for a description of the actual
|	internal representation of states, see the documentation for NxsDiscreteDatum.
|	
*/
inline int NxsCharactersBlock::GetInternalRepresentation(
  unsigned i,	/* the taxon in range [0..`ntax') */
  unsigned j,	/* the character in range [0..`nchar') */
  unsigned k)  NCL_COULD_BE_CONST /* the 0-offset index of state to return */
	{
	if (IsGapState(i, j))
		return -3;
	else if (IsMissingState(i, j))
		return -2;
	else
		return GetStateIndex(i, j, k);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the missing data symbol currently in effect. If no missing data symbol specified, returns '\0'.
*/
inline wchar_t NxsCharactersBlock::GetMissingSymbol() const 
	{
	return missing;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Name change	 to reinforce the change in meaning -- in NCL after 2.1 this will behaves just like the
|	GetNCharTotal().  It returns the number of characters in the matrix (regardless of whether they have been excluded).
|	The old GetNChar() function is now called GetNumIncludedChars();
*/
inline unsigned NxsCharactersBlock::GetNumChar() const
	{
	return nChar;
	}
	
/*----------------------------------------------------------------------------------------------------------------------
|	Note the change in meaning -- in NCL after 2.1 this will behaves just like the
|	GetNCharTotal().  It returns the number of characters in the matrix (regardless of whether they have been excluded).
|	The old GetNChar() function is now called GetNumIncludedChars();
*/
inline unsigned NxsCharactersBlock::GetNChar() const
	{
	return nChar;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the number of characters which are not excluded (or eliminated) this number is <= GetNumChar()
*/
inline unsigned NxsCharactersBlock::GetNumIncludedChars() const
	{
	return nChar - excluded.size();
	}


inline unsigned NxsCharactersBlock::GetNCharTotal()
	{
	return nChar;
	}

inline unsigned NxsCharactersBlock::GetNCharTotal() const
	{
	return nChar;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the value of `ntax'.
*/
inline unsigned NxsCharactersBlock::GetNTaxWithData() const
	{
	return nTaxWithData;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the number of characters eliminated with the ELIMINATE command.
*/
inline unsigned NxsCharactersBlock::GetNumEliminated()  NCL_COULD_BE_CONST 
	{
	return (unsigned)eliminated.size();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the number of stored equates associations.
*/
inline unsigned NxsCharactersBlock::GetNumUserEquates() const
	{
	return (unsigned)(userEquates.size());
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the number of stored equates associations.
*/
inline unsigned NxsCharactersBlock::GetNumEquates() NCL_COULD_BE_CONST 
	{
	return (unsigned)(userEquates.size() + defaultEquates.size());
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the number of actual columns in `matrix'. This number is equal to `nchar', but can be smaller than 
|	`ncharTotal' since the user could have eliminated some of the characters.
*/
inline unsigned NxsCharactersBlock::GetNumMatrixCols() NCL_COULD_BE_CONST 
	{
	return nChar;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns the number of actual rows in `matrix'. This number is equal to `ntax', but can be smaller than `ntaxTotal'
|	since the user did not have to provide data for all taxa specified in the TAXA block.
*/
inline unsigned NxsCharactersBlock::GetNumMatrixRows() NCL_COULD_BE_CONST 
	{
	return GetNTaxTotal();
	}

inline int NxsCharactersBlock::GetStateIndex(
  unsigned taxInd,	/* the taxon in range [0..`ntax') */
  unsigned charInd, /* the character in range [0..`nchar') */
  unsigned k) const
	{
	const NxsDiscreteDatatypeMapper * currMapper =	GetDatatypeMapperForChar(charInd);
	assert(currMapper);
	const NxsDiscreteStateRow & row = GetDiscreteMatrixRow(taxInd);
	assert(row.size() > charInd);
	return currMapper->GetOneStateForCode(row[charInd], k);
	}
/*----------------------------------------------------------------------------------------------------------------------
|	Returns symbol from symbols list representing the state for taxon `i' and character `j'. The normal situation in 
|	which there is only one state with no uncertainty or polymorphism is represented by `k' = 0. If there are multiple 
|	states, specify a number in the range [0..n) where n is the number of states returned by the GetNumStates function.
|	Use the IsPolymorphic function to determine whether the multiple states correspond to uncertainty in state 
|	assignment or polymorphism in the taxon. Assumes `symbols' is non-NULL.
*/
inline wchar_t NxsCharactersBlock::GetState(
  unsigned i,	/* the taxon in range [0..`ntax') */
  unsigned j,	/* the character in range [0..`nchar') */
  unsigned k) const	/* the 0-offset index of the state to return */
	{
	assert(!symbols.empty());
	const int p = GetStateIndex(i, j, k);
	if (p < 0)
		{
		assert(p == NXS_GAP_STATE_CODE);
		return gap;
		}
	assert(p < (int)symbols.length());
	return symbols[p];
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns data member `symbols'.	Warning: returned value may be NULL.
*/
inline const wchar_t*NxsCharactersBlock::GetSymbols() NCL_COULD_BE_CONST 
	{
	return symbols.c_str();
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns label for taxon number `i' (`i' ranges from 0 to `ntax' - 1).
*/
inline NxsString NxsCharactersBlock::GetTaxonLabel(
  unsigned i) const	/* the taxon's position in the taxa block */
	{
	NxsString s = taxa->GetTaxonLabel(i);
	return s;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true If character `j' has some stored character state. Assumes `taxInd' is in the range [0..`nchar').
*/
inline bool NxsCharactersBlock::TaxonIndHasData(
  unsigned taxInd) const /* the character in question, in the range [0..`nchar') */
	{
	if (datatype == continuous)
		return (taxInd < continuousMatrix.size() && !continuousMatrix[taxInd].empty());
	return (taxInd < discreteMatrix.size() && !discreteMatrix[taxInd].empty());
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns a reference to the set of indices that are currently excluded.
*/
inline const NxsUnsignedSet & NxsCharactersBlock::GetExcludedIndexSet() const
	{
	return excluded;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if character `j' is active. If character `j' has been excluded, returns false. Assumes `j' is in the 
|	range [0..`nchar').
*/
inline bool NxsCharactersBlock::IsActiveChar(
  unsigned j) const	/* the character in question, in the range [0..`nchar') */
	{
	return (j < nChar && excluded.count(j) == 0);
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if character `j' has been excluded. If character `j' is active, returns false. Assumes `j' is in the 
|	range [0..`nchar').
*/
inline bool NxsCharactersBlock::IsExcluded(
  unsigned j) const	/* the character in question, in the range [0..`nchar') */
	{
	return !IsActiveChar(j);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if character `j' is active. If character `j' has been excluded, returns false. Assumes `j' is in the 
|	range [0..`nchar').
*/
inline bool NxsCharactersBlock::IsActiveChar(
  unsigned j) /* the character in question, in the range [0..`nchar') */
	{
	return (j < nChar && excluded.count(j) == 0);
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if character `j' has been excluded. If character `j' is active, returns false. Assumes `j' is in the 
|	range [0..`nchar').
*/
inline bool NxsCharactersBlock::IsExcluded(
  unsigned j) /* the character in question, in the range [0..`nchar') */
	{
	return !IsActiveChar(j);
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if INTERLEAVE was specified in the FORMAT command, false otherwise.
*/
inline bool NxsCharactersBlock::IsInterleave() NCL_COULD_BE_CONST 
	{
	return interleaving;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if LABELS was specified in the FORMAT command, false otherwise.
*/
inline bool NxsCharactersBlock::IsLabels() NCL_COULD_BE_CONST 
	{
	return labels;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if RESPECTCASE was specified in the FORMAT command, false otherwise.
*/
inline bool NxsCharactersBlock::IsRespectCase() const
	{
	return respectingCase;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if TOKENS was specified in the FORMAT command, false otherwise.
*/
inline bool NxsCharactersBlock::IsTokens() NCL_COULD_BE_CONST 
	{
	return tokens;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if TRANSPOSE was specified in the FORMAT command, false otherwise.
*/
inline bool NxsCharactersBlock::IsTranspose() NCL_COULD_BE_CONST 
	{
	return transposing;
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Converts a taxon label to a number corresponding to the taxon's position within the list maintained by the 
|	NxsTaxaBlockAPI object. This method overrides the virtual function of the same name in the NxsBlock base class. If 
|	`s' is not a valid taxon label, returns the value 0.
*/
inline unsigned NxsCharactersBlock::TaxonLabelToNumber(
  NxsString s) const	/* the taxon label to convert */
	{
	try
		{
		return 1 + taxa->FindTaxon(s);
		}
	catch(NxsTaxaBlock::NxsX_NoSuchTaxon)
		{
		}

	return 0;
	}


inline VecBlockPtr NxsCharactersBlock::GetImpliedBlocks()
	{
	return GetCreatedTaxaBlocks();
	}
inline const std::wstring & NxsCharactersBlock::GetBlockName() const
	{
	return id;
	}
inline void NxsCharactersBlock::HandleLinkCommand(NxsToken & token)
	{
	HandleLinkTaxaCommand(token);
	}
inline void NxsCharactersBlock::WriteLinkCommand(std::wostream &out) const
	{
	WriteLinkTaxaCommand(out);
	}
	

inline NxsCharactersBlock::StatesFormatEnum NxsCharactersBlock::GetStatesFormat() const
	{
	return statesFormat;
	}

inline	unsigned NxsCharactersBlock::CharLabelToNumber(NxsString s) NCL_COULD_BE_CONST
	{
	const NxsCharactersBlock *b = (const NxsCharactersBlock *)(this);
	return b->CharLabelToNumber(s);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if character with `charIndex' (0-based index) was eliminated, false otherwise. 
*/
inline bool NxsCharactersBlock::IsEliminated(
  unsigned charIndex) NCL_COULD_BE_CONST /* the character in question */
	{
	return eliminated.count(charIndex) > 0 ;
	}

/*
|	Returns an alias to the NxsDiscreteDatatypeMapper for character index.
|	NULL will be returned if the NxsCharactersBlock is not fully initialized or 
|		if the block stores continuous characters.
|	The pointer is only guaranteed to be valid until the NxsCharactersBlock is modified.
|		(so do not store for long term usage).
*/
inline const NxsDiscreteDatatypeMapper * NxsCharactersBlock::GetDatatypeMapperForChar(unsigned charIndex) const
	{
	NxsCharactersBlock *mt = const_cast<NxsCharactersBlock *>(this);
	return mt->GetMutableDatatypeMapperForChar(charIndex);
	}

inline const NxsDiscreteStateRow & NxsCharactersBlock::GetDiscreteMatrixRow(unsigned int taxIndex) const
	{
	return discreteMatrix.at(taxIndex);
	}

inline const NxsCharactersBlock::ContinuousCharRow & NxsCharactersBlock::GetContinuousMatrixRow(unsigned taxIndex) const
	{
	return continuousMatrix.at(taxIndex);
	}

/*----------------------------------------------------------------------------------------------------------------------
|	Returns an alias to the NxsDiscreteDatatypeMapper for character index.
|	NULL will be returned if the NxsCharactersBlock is not fully initialized or 
|		if the block stores continuous characters.
|	The pointer is only guaranteed to be valid until the NxsCharactersBlock is modified.
|		(so do not store for long term usage).
*/
inline NxsDiscreteDatatypeMapper * NxsCharactersBlock::GetMutableDatatypeMapperForChar(unsigned int charIndex)
	{
	if (datatypeMapperVec.size() == 1)
		return &(datatypeMapperVec[0].first);
	for (VecDatatypeMapperAndIndexSet::iterator dmvIt = datatypeMapperVec.begin(); dmvIt != datatypeMapperVec.end(); ++dmvIt)
		{
		const NxsUnsignedSet & currCS = dmvIt->second;
		if (currCS.count(charIndex) > 0)
			return &(dmvIt->first);
		}
	return NULL;
	}
	
inline std::vector<const NxsDiscreteDatatypeMapper *> NxsCharactersBlock::GetAllDatatypeMappers() const
	{
	std::vector<const NxsDiscreteDatatypeMapper *> v;
	for (VecDatatypeMapperAndIndexSet::const_iterator dmvIt = datatypeMapperVec.begin(); dmvIt != datatypeMapperVec.end(); ++dmvIt)
		v.push_back(&(dmvIt->first));
	return v;
	}
	
inline void NxsDiscreteDatatypeMapper::WriteStateCodeRowAsNexus(std::wostream & out, const std::vector<int> &row) const
	{//@mth optimize
	std::vector<int>::const_iterator b = row.begin();
	const std::vector<int>::const_iterator e = row.end();
	WriteStateCodeRowAsNexus(out, b, e);
	}

inline void NxsDiscreteDatatypeMapper::WriteStateCodeRowAsNexus(std::wostream & out, std::vector<int>::const_iterator & begIt, const std::vector<int>::const_iterator & endIt) const
	{//@mth optimize
	for (; begIt != endIt; ++begIt)
		WriteStateCodeAsNexusString(out, *begIt, true);
	}


#endif
