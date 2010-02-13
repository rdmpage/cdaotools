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
      
      const std::string BASE_URI_PREFIX = "http://www.cs.nmsu.edu/~epontell/";      
      const std::string CDAO_DEF_URI = "http://www.evolutionaryontology.org/cdao.owl"; /*"http://www.cs.nmsu.edu/~epontell/CURRENT.owl";*/
      const std::string OWL_URI      = "http://www.w3.org/2002/07/owl";
      const std::string OWL11_URI    = "http://www.w3.org/2006/12/owl11";
      const std::string XSD_URI      = "http://www.w3.org/2001/XMLSchema";
      const std::string OWL11XML_URI = "http://www.w3.org/2006/12/owl11-xml";
      const std::string RDFS_URI     = "http://www.w3.org/2000/01/rdf-schema";
      const std::string RDF_URI      = "http://www.w3.org/1999/02/22-rdf-syntax-ns";
      //const std::string ONT_1206     = "http://www.owl-ontologies.com/Ontology1206463179.owl";
      //const std::string CDAO2       = /*"http://www.evolutonaryontology.org/CDAO/cdao.owl";*/ "http://www.cs.nmsu.edu/~epontell/CURRENT.owl";
      const std::string DC           = "http://purl.org/dc/elements/1.1/";
      const std::string AMINO_ACID   = "http://www.co-ode.org/ontologies/amino-acid/2005/10/11/amino-acid.owl"; 
      static std::string BASE_URI           = "http://www.cs.nmsu.edu/~epontell/stdout.owl";
      inline void setBaseURI(const std::string nURI){  BASE_URI = nURI; return; }
    }
    /**
     * Declares namespace prefixes used.
     */
    namespace NSDefs {
   
      const std::string CDAO       = "cdao";
      const std::string OWL        = "owl";
      const std::string OWL11      = "owl11";
      const std::string XSD        = "xsd";
      const std::string OWL11XML   = "owl11xml";
      const std::string RDFS       = "rdfs";
      const std::string RDF        = "rdf";
      const std::string BASE       = "base";
      const std::string ONT_1206   = "Ontology1206463179";
      //const std::string CDAO2      = "cdao2";
      const std::string DC         = "dc";
      const std::string AMINO_ACID = "amino-acid";
      
    }
    /**
     * Declares builtin elements of RDF and OWL.
     */
     namespace Builtins {
       
       const std::string RDF       = NSDefs::RDF + ":RDF";
       const std::string ONT       = NSDefs::OWL + ":Ontology";
       const std::string RESOURCE  = NSDefs::RDF + ":resource";
       const std::string ABOUT     = NSDefs::RDF + ":about";
       const std::string ID        = NSDefs::RDF + ":ID";
       const std::string IMPORTS   = NSDefs::OWL + ":imports";
    
    }
    
    /**
     * Declares the classes defined in CDAO. 
     */
    namespace Classes {
      
      const std::string BRANCH            = "Branch";
      const std::string CDAO_ANNOTATION   = "CDAOAnnotation";
      const std::string CSDM_ANNOTATION   = "CharacterStateDataMatrixAnnotation";
      const std::string ALIGN_PROC        = "AlignmentProcedure";
      const std::string EDGE_ANNOTATION   = "EdgeAnnotation";
      const std::string EDGE_LENGTH       = "EdgeLength";
      const std::string EDGE_TRANSFORM    = "Edge_Transformation";
      const std::string MODEL_DESCRIPTION = "ModelDescription";
      const std::string GAP_COST          = "GapCost";
      const std::string SUBST_MODEL       = "SubstitutionModel";
      const std::string GAMMA_DISTR       = "GammaDistribution";
      const std::string JUKES_AND_KANTOR  = "JukesKantor";
      const std::string KIMURA_2_PARAMS   = "Kimura2Parameters";
      const std::string TU_ANNOTATION     = "TUAnnotation";
      const std::string TAX_LINK          = "TaxonomicLink";
      const std::string TREE_ANNOTATION   = "TreeAnnotation";
      const std::string TREE_PROCEDURE    = "TreeProcedure";
      const std::string DIST_BASED_MTHD   = "Distance_based_method";
      const std::string MAX_LIKE          = "Maximum_likelihood_algorithm";
      const std::string PARSIMONY         = "Parsimony";
      const std::string CHARACTER         = "Character";
      const std::string AA_CHARACTER      = "AminoAcidResidueCharacter";
      const std::string CMPD_CHARACTER    = "CompoundCharacter";
      const std::string CONT_CHARACTER    = "ContinuousCharacter";
      const std::string NUC_CHARACTER     = "DesoxiRibonucleotideResidueCharacter";
      const std::string CSDM              = "CharacterStateDataMatrix";
      const std::string CSD               = "CharacterStateDatum";
      const std::string AA_DATUM          = "AminoAcidResidueStateDatum";
      const std::string CMPD_DATUM        = "CompoundStateDatum";
      const std::string CONT_DATUM        = "ContinuousStateDatum";
      const std::string NUC_DATUM         = "DesoxiRibonucleotideResidueStateDatum";
      const std::string CSDOM             = "CharacterStateDomain";
      const std::string AA                = "AminoAcid";
      const std::string AAR		  = "AminoAcidResidue";
      const std::string ALIPH_AA	  = "AliphaticAminoAcid";
      const std::string AROM_AA		  = "AromaticAminoAcid";
      const std::string LARGE_ALIPH_AA	  = "LargeAliphaticAminoAcid";
      const std::string NEG_CHRG_AA	  = "NegativeCharedAminoAcid";
      const std::string POS_CHRG_AA	  = "PositiveCharedAminoAcid";
      const std:: string A		  = "A";
      const std:: string C		  = "C";
      const std:: string D		  = "D";
      const std:: string E		  = "E";
      const std:: string F		  = "F";
      const std:: string G		  = "G";
      const std:: string H		  = "H";
      const std:: string I		  = "I";
      const std:: string K		  = "K";
      const std:: string L		  = "L";
      const std:: string M		  = "M";
      const std:: string N		  = "N";
      const std:: string P	 	  = "P";
      const std:: string Q		  = "Q";
      const std:: string R		  = "R";
      const std:: string S		  = "S";
      const std:: string T		  = "T";
      const std:: string V		  = "V";
      const std:: string W		  = "W";
      const std:: string Y 		  = "Y";
      const std::string CMPD              = "Compound";
      const std::string GENERIC_STATE     = "Generic_State";
      const std::string IMPORTED_AA       = "Imported_Amino_Acid";
      const std::string NUC               = "DesoxiRibonucleotideResidue";
      const std::string COORD_LIST        = "Coordinate_List";
      const std::string COORD_POINT       = "Coordinate_Point";
      const std::string COORD_RANGE       = "Coordinate_Range";
      const std::string EDGE              = "Edge";
      //const std::string BRANCH		  = "Branch";
      const std::string DIRECTED_EDGE     = "DirectedEdge";
      const std::string NETWORK           = "Network";
      const std::string TREE              = "Tree";
      const std::string LINEAGE           = "Lineage";
      const std::string ROOTED_TREE       = "RootedTree";
      const std::string UNROOTED_TREE     = "UnrootedTree";
      const std::string NODE              = "Node";
      const std::string ANCES_NODE        = "AncestralNode";
      const std::string COM_ANCES_NODE    = "CommonAncestralNode";
      const std::string MRCA_NODE         = "MRCANode";
      const std::string SEQUENCE          = "Sequence";
      const std::string TU                = "TU";
      const std::string ABSENT            = "absent";
      const std::string GAP               = "gap";
      const std::string UKN               = "unknown";
      const std::string NODE_SET          = "SetOfNodes";
      const std::string CHAR_SET	  = "SetofCharacters";
    
    }
    /**
     * Declares the properties used in CDAO
     */
    namespace Properties {
    
      /*Object Properties */
      const std::string HAS_LEFT_NODE           = "has_Left_Node";
      const std::string HAS_CHILD               = "has_Child";
      const std::string HAS_STATE               = "has_State";
      const std::string HAS_DATUM               = "has_Datum";
      const std::string HAS_AA_STATE            = "has_Amino_Acid_State";
      const std::string HAS_NUC_STATE           = "has_Nucleotide_State";
      const std::string HAS_CMPD_STATE          = "has_Compound_State";
      const std::string HAS_COORD               = "has_Coordinate";
      const std::string HAS_FST_COORD_IT        = "has_First_Coordinate_Item";
      const std::string IS_ANNOTATION           = "is_annotation_of";
      const std::string BELONGS_TO_CHAR         = "belongs_to_Character";
      const std::string BELONGS_TO_CSDM         = "belongs_to_Character_State_Data_Matrix";
      const std::string BELONGS_TO_CMPD_CHAR    = "belongs_to_Compound_Character";
      const std::string BELONGS_TO_AA_CHAR      = "belongs_to_Amino_Acid_Character";
      const std::string BELONGS_TO_NUC_CHAR     = "belongs_to_Nucleotide_Character";
      const std::string BELONGS_TO_CONT_CHAR    = "belongs_to_Continuous_Character";
      const std::string BELONGS_TO_TU           = "belongs_to_TU";
      const std::string BELONGS_TO_EDGE_CH      = "belongs_to_Edge_as_Child";
      const std::string BELONGS_TO_EDGE_PR      = "belongs_to_Edge_as_Parent";
      const std::string HAS_COORD_SYS           = "has_Coordinate_System";
      const std::string TRANSFORM_OF            = "transformation_of";
      const std::string REPRESENTS_TU           = "represents_TU";
      const std::string HAS_NODE                = "has_Node";
      const std::string HAS_CHILD_NODE          = "has_Child_Node";
      const std::string HAS_PAR_NODE            = "has_Parent_Node";
      const std::string HOMOLOG_TO              = "homologous_to";
      const std::string IS_NODE_OF_EDGE         = "is_Node_of_Edge";
      const std::string IS_CHILD_OF_EDGE        = "is_Child_Node_of_Edge";
      const std::string IS_PARENT_OF_EDGE       = "is_Parent_Node_of_Edge";
      const std::string PRECEDES                = "precedes";
      const std::string SUBTREE_OF              = "subtree_of";
      const std::string HAS_ROOT                = "has_Root";
      const std::string HAS_RT_NODE             = "has_Right_Node";
      const std::string HAS_CHARACTER           = "has_Character";
      //const std::string REPRESENTS_TU           = "represents_TU";
      const std::string REPRESENTED_BY_NODE     = "represented_by_Node";
      const std::string HAS                     = "has";
      const std::string HAS_RIGHT_STATE         = "has_Right_State";
      const std::string HAS_TU                  = "has_TU";
      const std::string HAS_LEFT_STATE          = "has_Left_State";
      const std::string PART_OF                 = "part_of";
      const std::string CONNECTS_TO             = "connects_to";
      const std::string HAS_PARENT              = "has_Parent";
      const std::string HAS_DESCENDANT          = "has_Descendant";
      const std::string HAS_REM_COORD_LIST      = "has_Remaining_Coordinate_List";
      const std::string HAS_LINEAGE_NODE        = "has_Lineage_node";
      const std::string NCA_OF                  = "nca_node_of";
      /* Data Properties */
      //const std::string HAS_STATE_VAL           = "has_State_Value";
      const std::string HAS_PT_COORD_VAL        = "has_Point_Coordinate_Value";
      const std::string HAS_CONT_STATE          = "has_Continuous_State";
      const std::string HAS_RANGE_END_VAL       = "has_Range_End_Value";
      const std::string HAS_EXTERN_REF          = "has_External_Reference";
      const std::string HAS_RANGE_START_VAL     = "has_Range_Start_Value";
      const std::string HAS_SUPPORT_VAL         = "has_Support_Value";
      const std::string HAS_ELEMENT             = "has_Element";
    }
  }
}


#endif
