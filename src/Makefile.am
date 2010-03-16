#Brandon Chisham
#Automake rules for the cdao translator.
#April 15, 2008

INCLUDES=-I. -I$(top_builddir) -I$(top_builddir)/src/grammar/nexus -I$(top_builddir)/src/grammar/phylip -I$(top_builddir)/lib/ncl-2.1.06/ -I$(top_builddir)/lib/CppLoggerV2/src
AM_CXXFLAGS=-Wall -g

NEXDIR=grammar/nexus
PHYDIR=grammar/phylip
CODEGEN=codegen
LIBDIR=$(top_builddir)/lib
NCLV=2.1.06

bin_PROGRAMS=cdao-import cdao-mass-import cdao-tree-import cdao-read-tree extract_trees.pl cdao-phylip-import

bin_SCRIPTS=extract_trees.pl

cdao_import_SOURCES=main.cpp \
		    shm_heap.cpp shm_heap.hpp \
		    util.cpp util.hpp \
		    node.cpp node.hpp \
		    data_representation.cpp data_representation.hpp \
		    $(NEXDIR)/tree_description_scanner.cpp $(NEXDIR)/tree_description_scanner.hpp \
		    $(NEXDIR)/tree_description_parser.cpp $(NEXDIR)/tree_description_parser.hpp \
		    $(NEXDIR)/nexus_reader.cpp $(NEXDIR)/nexus_reader.hpp \
		    $(PHYDIR)/phylip_reader.cpp $(PHYDIR)/phylip_reader.hpp \
		    $(CODEGEN)/constants.hpp \
		    $(CODEGEN)/codegen.cpp $(CODEGEN)/codegen.hpp
cdao_import_LDADD=$(LIBDIR)/ncl-$(NCLV)/ncl/libncl.la $(LIBDIR)/CppLoggerV2/src/libCppLogger.la


cdao_mass_import_SOURCES=main2.cpp \
		    shm_heap.cpp shm_heap.hpp \
		    util.cpp util.hpp \
		    node.cpp node.hpp \
		    data_representation.cpp data_representation.hpp \
		    $(NEXDIR)/tree_description_scanner.cpp $(NEXDIR)/tree_description_scanner.hpp \
		    $(NEXDIR)/tree_description_parser.cpp $(NEXDIR)/tree_description_parser.hpp \
		    $(NEXDIR)/nexus_reader.cpp $(NEXDIR)/nexus_reader.hpp \
		    $(PHYDIR)/phylip_reader.cpp $(PHYDIR)/phylip_reader.hpp \
	 	    $(CODEGEN)/constants.hpp \
		    $(CODEGEN)/codegen.cpp $(CODEGEN)/codegen.hpp
cdao_mass_import_LDADD=$(LIBDIR)/ncl-$(NCLV)/ncl/libncl.la $(LIBDIR)/CppLoggerV2/src/libCppLogger.la

cdao_tree_import_SOURCES=main3.cpp \
		    shm_heap.cpp shm_heap.hpp \
		    util.cpp util.hpp \
		    node.cpp node.hpp \
		    data_representation.cpp data_representation.hpp \
		    $(NEXDIR)/tree_description_scanner.cpp $(NEXDIR)/tree_description_scanner.hpp \
		    $(NEXDIR)/tree_description_parser.cpp $(NEXDIR)/tree_description_parser.hpp \
	 	    $(CODEGEN)/constants.hpp \
		    $(CODEGEN)/codegen.cpp $(CODEGEN)/codegen.hpp
cdao_tree_import_LDADD=$(LIBDIR)/ncl-$(NCLV)/ncl/libncl.la $(LIBDIR)/CppLoggerV2/src/libCppLogger.la

cdao_read_tree_SOURCES=read-tree.cpp \
		    shm_heap.cpp shm_heap.hpp \
		    util.cpp util.hpp \
		    node.cpp node.hpp \
		    data_representation.cpp data_representation.hpp \
		    $(NEXDIR)/tree_description_scanner2.cpp  \
		    $(NEXDIR)/tree_description_parser2.cpp  \
	 	    $(CODEGEN)/constants.hpp \
		    $(CODEGEN)/codegen.cpp $(CODEGEN)/codegen.hpp
cdao_read_tree_LDADD=$(LIBDIR)/ncl-$(NCLV)/ncl/libncl.la $(LIBDIR)/CppLoggerV2/src/libCppLogger.la -ll

cdao_phylip_import_SOURCES=main4.cpp \
		    util.cpp util.hpp \
		    node.cpp node.hpp \
		    shm_heap.cpp shm_heap.hpp \
		    data_representation.cpp data_representation.hpp \
		    $(NEXDIR)/tree_description_scanner.cpp $(NEXDIR)/tree_description_scanner.hpp \
		    $(NEXDIR)/tree_description_parser.cpp $(NEXDIR)/tree_description_parser.hpp \
		    $(NEXDIR)/nexus_reader.cpp $(NEXDIR)/nexus_reader.hpp \
		    $(PHYDIR)/phylip_reader.cpp $(PHYDIR)/phylip_reader.hpp \
	 	    $(CODEGEN)/constants.hpp \
		    $(CODEGEN)/codegen.cpp $(CODEGEN)/codegen.hpp
cdao_phylip_import_LDADD=$(LIBDIR)/ncl-$(NCLV)/ncl/libncl.la $(LIBDIR)/CppLoggerV2/src/libCppLogger.la


$(NEXDIR)/tree_description_parser2.cpp: $(NEXDIR)/tree_description_parser2.yy
	(cd $(NEXDIR) && bison -d -o $(NEXDIR)/tree_description_parser2.cpp $(NEXDIR)/tree_description_parser2.yy && echo -e "#include <string>\n#include <node.hpp>\n#include <util.hpp>" > tree_description_parser2.hpp.tmp && cat tree_description_parser2.hpp >> tree_description_parser2.hpp.tmp && mv tree_description_parser.2.hpp.tmp tree_description_parser2.hpp)
$(NEXDIR)/tree_description_scanner2.cpp: $(NEXDIR)/tree_description_scanner2.l $(NEXDIR)/tree_description_parser2.cpp
	flex -o $(NEXDIR)/tree_description_scanner2.cpp $(NEXDIR)/tree_description_scanner2.l


extract_trees.pl: $(NEXDIR)/extract_trees.pl
	cp $(NEXDIR)/extract_trees.pl .
