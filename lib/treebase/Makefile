BISON=bison
FLEX=flex
CXX=g++
CXXFLAGS=-Wall 
LIBS=-ll

treebase: main.cpp codegen.o parser.o scanner.o treebase-node.o
	$(CXX) $(CXXFLAGS) -o treebase codegen.o parser.o scanner.o treebase-node.o main.cpp $(LIBS)

#cdao-importer.o: cdao-importer.cpp cdao-importer.hpp
#	$(CXX) $(CXXFLAGS) -c -o cdao-importer.o cdao-importer.cpp

codgen.o: codegen.cpp codegen.hpp treebase-node.o
	$(CXX) $(CXXFLAGS) -c -o codegen.o codegen.cpp

parser.o: parser.cpp
	$(CXX) $(CXXFLAGS) -c -o parser.o parser.cpp

scanner.o: scanner.cpp
	$(CXX) $(CXXFLAGS) -c -o scanner.o scanner.cpp

scanner.cpp: parser.cpp scanner.l
	$(FLEX) -o scanner.cpp scanner.l
parser.cpp: parser.yy treebase-node.o
	$(BISON) -d -oparser.cpp parser.yy
treebase-node.o: treebase-node.cpp treebase-node.hpp
	$(CXX) $(CXXFLAGS) -o treebase-node.o -c treebase-node.cpp

clean:
	rm -f scanner.cpp parser.cpp *.o treebase
