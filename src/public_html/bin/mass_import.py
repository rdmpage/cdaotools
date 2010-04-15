#!/usr/bin/env python
import rdflib
import xml
from mass_store import get_store, connect_store, store_ontology
from sys import argv
from os import environ, system
from re import compile, match, sub

configString=environ['TRIPLESTORE_CONFIG_STRING']

regex = re.compile("gz$")

graph_list=argv[1::1]
store = get_store( configString )
graph = connect_store( store, argv[1] )


for guri in graph_list:
	print "processing: " + guri + "\n"
        if regex.match( guri ):
                uncompguri = system( "gunzip " + guri )
                store_ontology( graph, uncompguri )
                system( "gzip --best " + uncompguri )
        else:
                store_ontology( graph, guri )

#	print "The store now contains: %d triples", len( graph )
	print "processed: " + guri + "\n"

store.close()
