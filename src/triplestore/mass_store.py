#Script to handle storing rdf triples from some url into the local store.
#Brandon Chisham
#September 9, 2009

import sys

#sys.path.append('/home/grad6/bchisham/public_html/cgi-bin/MySQL_python-1.2.3c1-py2.5-linux-i686.egg')
#sys.path.append('/home/grad6/bchisham/public_html/cgi-bin/rdflib-2.4.2')



import rdflib
from rdflib.Graph import ConjunctiveGraph as Graph
from rdflib import plugin
from rdflib.store import Store, NO_STORE, VALID_STORE
from rdflib import Namespace
from rdflib import Literal
from rdflib import URIRef

def get_store( configString ):
	store = plugin.get( 'MySQL', Store)('rdflib')
        rt = store.open(configString, create=False)
        if rt == NO_STORE:
                store.open(configString, create=True)
        else:
                assert rt == VALID_STORE, "The underlying store is not valid"
        print "Opened Store"
	return store

def cleanup_store( store ):
	store.close()

def connect_store( store, default_graph_uri ):
        graph = Graph(store, identifier = URIRef( default_graph_uri ))
	return graph

def store_ontology( graph, default_graph_uri ):
        graph.parse( default_graph_uri )
        print "Committing Graph to database\n"
        graph.commit()
        print "Saved Graph to database\n"
 
def store_ontology( default_graph_uri ):
      #  graph_local = Graph( identifier = URIRef( default_graph_uri ) )
        print "Preparing to read graph: " + default_graph_uri + "\n"
     #   graph_local.parse( default_graph_uri )
      #  print "Read Graph saving to database"
        configString = "host=dbclass,user=brandon,password=b2341x,db=portal"
        store = plugin.get( 'MySQL', Store)('rdflib')
        rt = store.open(configString, create=False)
        if rt == NO_STORE:
                store.open(configString, create=True)
        else:
                assert rt == VALID_STORE, "The underlying store is not valid"
        print "Opened Store"
        graph = Graph(store, identifier = URIRef( default_graph_uri ))
#        graph = Graph( identifier = URIRef( default_graph_uri ))       
        # for stmt in graph_local:
        #        graph.add( stmt ) 
        graph.parse( default_graph_uri )
        print "Committing Graph to database\n"
        graph.commit()
        print "Saved Graph to database\n"
        store.close()
        return len( graph )
