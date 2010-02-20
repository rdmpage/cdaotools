#!/usr/bin/env python
import rdflib
import xml
from mass_store import get_store, connect_store, store_ontology
from sys import argv
from os import environ
configString=environ['TRIPLESTORE_CONFIG_STRING']

graph_list=argv[1::1]
store = get_store( configString )
graph = connect_store( store, argv[1] )

for guri in graph_list:
	print "processing: " + guri + "\n"
	try:
		graph.parse( guri )
		graph.commit()
	except rdflib.exceptions.ParserError as exp:
		print "Error processing %s"
		print exp
	except xml.sax._exceptions.SAXParseException as exp:
		print "Error processing %s"
		print exp
#	print "The store now contains: %d triples", len( graph )
	print "processed: " + guri + "\n"

store.close()
