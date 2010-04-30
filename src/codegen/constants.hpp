#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_
/*
 * Brandon Chisham
 * Created: April 17, 2008
 * Define concept constants.
 */

#include <string>

namespace CDAO {
  namespace Codegen {
    /**
     * Declares URI's for imported namespaces
     */
    namespace Imports {
      //the evolutionaryontology.org url generates a 403 error which prevents Protege from opening documents referencing it.
      
      const std::wstring BASE_URI_PREFIX = L"http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows";      
      const std::wstring CDAO_DEF_URI =  L"http://www.evolutionaryontology.org/cdao.owl"; /*"http://www.cs.nmsu.edu/~epontell/CURRENT.owl";*/
      const std::wstring OWL_URI      =  L"http://www.w3.org/2002/07/owl";
      const std::wstring OWL11_URI    =  L"http://www.w3.org/2006/12/owl11";
      const std::wstring XSD_URI      =  L"http://www.w3.org/2001/XMLSchema";
      const std::wstring OWL11XML_URI =  L"http://www.w3.org/2006/12/owl11-xml";
      const std::wstring RDFS_URI     =  L"http://www.w3.org/2000/01/rdf-schema";
      const std::wstring RDF_URI      =  L"http://www.w3.org/1999/02/22-rdf-syntax-ns";
      //const std::wstring ONT_1206     = L"http://www.owl-ontologies.com/Ontology1206463179.owl";
      //const std::wstring CDAO2       = /*"http://www.evolutonaryontology.org/CDAO/cdao.owl";*/ L"http://www.cs.nmsu.edu/~epontell/CURRENT.owl";
      const std::wstring DC           =  L"http://purl.org/dc/elements/1.1/";
      const std::wstring AMINO_ACID   =  L"http://www.co-ode.org/ontologies/amino-acid/2005/10/11/amino-acid.owl"; 
      static std::wstring BASE_URI           =  L"http://www.cs.nmsu.edu/~epontell/stdout.owl";
      inline void setBaseURI(const std::wstring nURI){  BASE_URI = nURI; return; }
    }
    /**
     * Declares namespace prefixes used.
     */
    namespace NSDefs {
   
      const std::wstring CDAO       =  L"cdao";
      const std::wstring OWL        =  L"owl";
      const std::wstring OWL11      =  L"owl11";
      const std::wstring XSD        =  L"xsd";
      const std::wstring OWL11XML   =  L"owl11xml";
      const std::wstring RDFS       =  L"rdfs";
      const std::wstring RDF        =  L"rdf";
      const std::wstring BASE       =  L"base";
      const std::wstring ONT_1206   =  L"Ontology1206463179";
      //const std::wstring CDAO2      = L"cdao2";
      const std::wstring DC         =  L"dc";
      const std::wstring AMINO_ACID =  L"amino-acid";
      
    }
    /**
     * Declares builtin elements of RDF and OWL.
     */
     namespace Builtins {
       
       const std::wstring RDF         = NSDefs::RDF +  L":RDF";
       const std::wstring ONT         = NSDefs::OWL +  L":Ontology";
       const std::wstring RESOURCE    = NSDefs::RDF +  L":resource";
       const std::wstring ABOUT       = NSDefs::RDF +  L":about";
       const std::wstring ID          = NSDefs::RDF +  L":ID";
       const std::wstring IMPORTS     = NSDefs::OWL +  L":imports";
       const std::wstring DESCRIPTION = NSDefs::RDF +  L":Description";
       const std::wstring TYPE        = NSDefs::RDF +  L":type";
       const std::wstring THING       = NSDefs::OWL +  L":Thing";
    }
    
    /**
     * Declares the classes defined in CDAO. 
     */
    namespace Classes {
      
      const std::wstring BRANCH            =  L"Branch";
      const std::wstring CDAO_ANNOTATION   =  L"CDAOAnnotation";
      const std::wstring CSDM_ANNOTATION   =  L"CharacterStateDataMatrixAnnotation";
      const std::wstring ALIGN_PROC        =  L"AlignmentProcedure";
      const std::wstring EDGE_ANNOTATION   =  L"EdgeAnnotation";
      const std::wstring EDGE_LENGTH       =  L"EdgeLength";
      const std::wstring EDGE_TRANSFORM    =  L"Edge_Transformation";
      const std::wstring MODEL_DESCRIPTION =  L"ModelDescription";
      const std::wstring GAP_COST          =  L"GapCost";
      const std::wstring SUBST_MODEL       =  L"SubstitutionModel";
      const std::wstring GAMMA_DISTR       =  L"GammaDistribution";
      const std::wstring JUKES_AND_KANTOR  =  L"JukesKantor";
      const std::wstring KIMURA_2_PARAMS   =  L"Kimura2Parameters";
      const std::wstring TU_ANNOTATION     =  L"TUAnnotation";
      const std::wstring TAX_LINK          =  L"TaxonomicLink";
      const std::wstring TREE_ANNOTATION   =  L"TreeAnnotation";
      const std::wstring TREE_PROCEDURE    =  L"TreeProcedure";
      const std::wstring DIST_BASED_MTHD   =  L"Distance_based_method";
      const std::wstring MAX_LIKE          =  L"Maximum_likelihood_algorithm";
      const std::wstring PARSIMONY         =  L"Parsimony";
      const std::wstring CHARACTER         =  L"Character";
      const std::wstring AA_CHARACTER      =  L"AminoAcidResidueCharacter";
      const std::wstring CMPD_CHARACTER    =  L"CompoundCharacter";
      const std::wstring CONT_CHARACTER    =  L"ContinuousCharacter";
      const std::wstring NUC_CHARACTER     =  L"DesoxiRibonucleotideResidueCharacter";
      const std::wstring CSDM              =  L"CharacterStateDataMatrix";
      const std::wstring CSD               =  L"CharacterStateDatum";
      const std::wstring AA_DATUM          =  L"AminoAcidResidueStateDatum";
      const std::wstring CMPD_DATUM        =  L"CompoundStateDatum";
      const std::wstring CONT_DATUM        =  L"ContinuousStateDatum";
      const std::wstring NUC_DATUM         =  L"DesoxiRibonucleotideResidueStateDatum";
      const std::wstring CSDOM             =  L"CharacterStateDomain";
      const std::wstring AA                =  L"AminoAcid";
      const std::wstring AAR		  =  L"AminoAcidResidue";
      const std::wstring ALIPH_AA	  =  L"AliphaticAminoAcid";
      const std::wstring AROM_AA		  =  L"AromaticAminoAcid";
      const std::wstring LARGE_ALIPH_AA	  =  L"LargeAliphaticAminoAcid";
      const std::wstring NEG_CHRG_AA	  =  L"NegativeCharedAminoAcid";
      const std::wstring POS_CHRG_AA	  =  L"PositiveCharedAminoAcid";
      const std:: wstring A		  =  L"A";
      const std:: wstring C		  =  L"C";
      const std:: wstring D		  =  L"D";
      const std:: wstring E		  =  L"E";
      const std:: wstring F		  =  L"F";
      const std:: wstring G		  =  L"G";
      const std:: wstring H		  =  L"H";
      const std:: wstring I		  =  L"I";
      const std:: wstring K		  =  L"K";
      const std:: wstring L		  =  L"L";
      const std:: wstring M		  =  L"M";
      const std:: wstring N		  =  L"N";
      const std:: wstring P	 	  =  L"P";
      const std:: wstring Q		  =  L"Q";
      const std:: wstring R		  =  L"R";
      const std:: wstring S		  =  L"S";
      const std:: wstring T		  =  L"T";
      const std:: wstring V		  =  L"V";
      const std:: wstring W		  =  L"W";
      const std:: wstring Y 		  =  L"Y";
      const std::wstring CMPD              =  L"Compound";
      const std::wstring GENERIC_STATE     =  L"Generic_State";
      const std::wstring IMPORTED_AA       =  L"Imported_Amino_Acid";
      const std::wstring NUC               =  L"DesoxiRibonucleotideResidue";
      const std::wstring COORD_LIST        =  L"Coordinate_List";
      const std::wstring COORD_POINT       =  L"Coordinate_Point";
      const std::wstring COORD_RANGE       =  L"Coordinate_Range";
      const std::wstring EDGE              =  L"Edge";
      //const std::wstring BRANCH		  = L"Branch";
      const std::wstring DIRECTED_EDGE     =  L"DirectedEdge";
      const std::wstring NETWORK           =  L"Network";
      const std::wstring TREE              =  L"Tree";
      const std::wstring LINEAGE           =  L"Lineage";
      const std::wstring ROOTED_TREE       =  L"RootedTree";
      const std::wstring UNROOTED_TREE     =  L"UnrootedTree";
      const std::wstring NODE              =  L"Node";
      const std::wstring ANCES_NODE        =  L"AncestralNode";
      const std::wstring COM_ANCES_NODE    =  L"CommonAncestralNode";
      const std::wstring MRCA_NODE         =  L"MRCANode";
      const std::wstring SEQUENCE          =  L"Sequence";
      const std::wstring TU                =  L"TU";
      const std::wstring ABSENT            =  L"absent";
      const std::wstring GAP               =  L"gap";
      const std::wstring UKN               =  L"unknown";
      const std::wstring NODE_SET          =  L"SetOfNodes";
      const std::wstring CHAR_SET	  =  L"SetofCharacters";
    
    }
    /**
     * Declares the properties used in CDAO
     */
    namespace Properties {
    
      /*Object Properties */
      const std::wstring HAS_LEFT_NODE           =  L"has_Left_Node";
      const std::wstring HAS_CHILD               =  L"has_Child";
      const std::wstring HAS_STATE               =  L"has_State";
      const std::wstring HAS_DATUM               =  L"has_Datum";
      const std::wstring HAS_AA_STATE            =  L"has_Amino_Acid_State";
      const std::wstring HAS_NUC_STATE           =  L"has_Nucleotide_State";
      const std::wstring HAS_CMPD_STATE          =  L"has_Compound_State";
      const std::wstring HAS_COORD               =  L"has_Coordinate";
      const std::wstring HAS_FST_COORD_IT        =  L"has_First_Coordinate_Item";
      const std::wstring IS_ANNOTATION           =  L"is_annotation_of";
      const std::wstring BELONGS_TO_CHAR         =  L"belongs_to_Character";
      const std::wstring BELONGS_TO_CSDM         =  L"belongs_to_Character_State_Data_Matrix";
      const std::wstring BELONGS_TO_CMPD_CHAR    =  L"belongs_to_Compound_Character";
      const std::wstring BELONGS_TO_AA_CHAR      =  L"belongs_to_Amino_Acid_Character";
      const std::wstring BELONGS_TO_NUC_CHAR     =  L"belongs_to_Nucleotide_Character";
      const std::wstring BELONGS_TO_CONT_CHAR    =  L"belongs_to_Continuous_Character";
      const std::wstring BELONGS_TO_TU           =  L"belongs_to_TU";
      const std::wstring BELONGS_TO_EDGE_CH      =  L"belongs_to_Edge_as_Child";
      const std::wstring BELONGS_TO_EDGE_PR      =  L"belongs_to_Edge_as_Parent";
      const std::wstring HAS_COORD_SYS           =  L"has_Coordinate_System";
      const std::wstring TRANSFORM_OF            =  L"transformation_of";
      const std::wstring REPRESENTS_TU           =  L"represents_TU";
      const std::wstring HAS_NODE                =  L"has_Node";
      const std::wstring HAS_CHILD_NODE          =  L"has_Child_Node";
      const std::wstring HAS_PAR_NODE            =  L"has_Parent_Node";
      const std::wstring HOMOLOG_TO              =  L"homologous_to";
      const std::wstring IS_NODE_OF_EDGE         =  L"is_Node_of_Edge";
      const std::wstring IS_CHILD_OF_EDGE        =  L"is_Child_Node_of_Edge";
      const std::wstring IS_PARENT_OF_EDGE       =  L"is_Parent_Node_of_Edge";
      const std::wstring PRECEDES                =  L"precedes";
      const std::wstring SUBTREE_OF              =  L"subtree_of";
      const std::wstring HAS_ROOT                =  L"has_Root";
      const std::wstring HAS_RT_NODE             =  L"has_Right_Node";
      const std::wstring HAS_CHARACTER           =  L"has_Character";
      //const std::wstring REPRESENTS_TU           = L"represents_TU";
      const std::wstring REPRESENTED_BY_NODE     =  L"represented_by_Node";
      const std::wstring HAS                     =  L"has";
      const std::wstring HAS_RIGHT_STATE         =  L"has_Right_State";
      const std::wstring HAS_TU                  =  L"has_TU";
      const std::wstring HAS_LEFT_STATE          =  L"has_Left_State";
      const std::wstring PART_OF                 =  L"part_of";
      const std::wstring CONNECTS_TO             =  L"connects_to";
      const std::wstring HAS_PARENT              =  L"has_Parent";
      const std::wstring HAS_DESCENDANT          =  L"has_Descendant";
      const std::wstring HAS_REM_COORD_LIST      =  L"has_Remaining_Coordinate_List";
      const std::wstring HAS_LINEAGE_NODE        =  L"has_Lineage_node";
      const std::wstring NCA_OF                  =  L"nca_node_of";
      /* Data Properties */
      //const std::wstring HAS_STATE_VAL           = L"has_State_Value";
      const std::wstring HAS_PT_COORD_VAL        =  L"has_Point_Coordinate_Value";
      const std::wstring HAS_CONT_STATE          =  L"has_Continuous_State";
      const std::wstring HAS_RANGE_END_VAL       =  L"has_Range_End_Value";
      const std::wstring HAS_EXTERN_REF          =  L"has_External_Reference";
      const std::wstring HAS_RANGE_START_VAL     =  L"has_Range_Start_Value";
      const std::wstring HAS_SUPPORT_VAL         =  L"has_Support_Value";
      const std::wstring HAS_ELEMENT             =  L"has_Element";
    }
  }
}


#endif
