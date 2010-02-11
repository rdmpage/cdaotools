#Script to handle processing sparql queries.
#Brandon Chisham
#Feb. 9, 2010

import sys
#Add rdflib to the package search path
sys.path.append('/home/grad6/bchisham/public_html/cgi-bin/rdflib-2.4.2')

import rdflib
from rdflib.Graph import ConjunctiveGraph as Graph
from rdflib import plugin
from rdflib.store import Store, NO_STORE, VALID_STORE
from rdflib import Namespace
from rdflib import Literal
from rdflib import URIRef

def query_store( configString, default_graph_uri, namespace_bindings, queryString ):
        store = plugin.get( 'MySQL', Store)('rdflib')
        rt = store.open(configString, create=False)
        if rt == NO_STORE:
                store.open(configString, create=True)
        else:
                assert rt == VALID_STORE, "The underlying store is not valid"

        graph = Graph(store, identifier = URIRef( default_graph_uri ))
        return graph.query( queryString, initNs = namespace_bindings)
