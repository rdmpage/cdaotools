#Brandon Chisham
#Automake rules for the cdao translator.
#April 15, 2008

INCLUDES=-I. -I$(top_builddir) -I$(top_builddir)/src/grammar/nexus -I$(top_builddir)/lib/ncl-2.0.02/ncl -I$(top_builddir)/lib/CppLoggerV2/src
AM_CXXFLAGS=-Wall -g

NEXDIR=grammar/nexus
CODEGEN=codegen
LIBDIR=$(top_builddir)/lib
NCLV=2.0.02

bin_PROGRAMS=cdao-import cdao-mass-import

cdao_import_SOURCES=main.cpp \
                    util.cpp util.hpp \
                    node.cpp node.hpp \
		    data_representation.cpp data_representation.hpp \
		    $(NEXDIR)/tree_description_scanner.cpp $(NEXDIR)/tree_description_scanner.hpp \
		    $(NEXDIR)/tree_description_parser.cpp $(NEXDIR)/tree_description_parser.hpp \
                    $(NEXDIR)/nexus_reader.cpp $(NEXDIR)/nexus_reader.hpp \
		    $(CODEGEN)/constants.hpp \
                    $(CODEGEN)/codegen.cpp $(CODEGEN)/codegen.hpp
cdao_import_LDADD=$(LIBDIR)/ncl-$(NCLV)/ncl/libncl.la $(LIBDIR)/CppLoggerV2/src/libCppLogger.la


cdao_mass_import_SOURCES=main2.cpp \
                    util.cpp util.hpp \
                    node.cpp node.hpp \
		    data_representation.cpp data_representation.hpp \
		    $(NEXDIR)/tree_description_scanner.cpp $(NEXDIR)/tree_description_scanner.hpp \
		    $(NEXDIR)/tree_description_parser.cpp $(NEXDIR)/tree_description_parser.hpp \
                    $(NEXDIR)/nexus_reader.cpp $(NEXDIR)/nexus_reader.hpp \
		    $(CODEGEN)/constants.hpp \
                    $(CODEGEN)/codegen.cpp $(CODEGEN)/codegen.hpp
cdao_mass_import_LDADD=$(LIBDIR)/ncl-$(NCLV)/ncl/libncl.la $(LIBDIR)/CppLoggerV2/src/libCppLogger.la
