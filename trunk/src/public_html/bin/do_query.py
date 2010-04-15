#!/usr/bin/env python

#Script to run sparql queries on the triple store.
#Brandon Chisham
#Feb, 9, 2010
import sys
#Add rdflib to the package search path
sys.path.append('/home/grad6/bchisham/public_html/cgi-bin/rdflib-2.4.2')
from rdflib import Namespace
from rdflib import Literal
from rdflib import URIRef


import html
import cgitb
import cgi
import re
import sys
#print "Content-Type: text/html\n\n";
#cgitb.enable()
#form = cgi.FieldStorage()
from urllib import unquote
from sparql import get_graph
from html import start_html, head, open_div, anchor, end_div, open_body, par, anchor ,end_body, end_html, heading_h1

configString = sys.argv[1]
fileURI = sys.argv[2]

sparqlQuery = sys.argv[3::2]
formatString = sys.argv[4::2]


#sparqlQuery = sys.argv[3]
#formatString= sys.argv[4]
#fileNameSpace=fileURI + "#"
cdaoURI= "http://www.evolutionaryontology.org/cdao.owl"
cdaoNS= "http://www.evolutionaryontology.org/cdao.owl#"
rdfNS="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
owlNS="http://www.w3.org/2002/07/owl#"
studyNS="http://www.cs.nmsu.edu/~bchisham/study.owl#"
contactNS="http://www.w3.org/2000/10/swap/pim/contact#"
foafNS="http://www.mindswap.org/2003/owl/foaf#"
nsdict=dict(owl=Namespace(owlNS), cdao=Namespace(cdaoNS), rdf=Namespace(rdfNS), data=Namespace(fileURI), study=Namespace(studyNS), contact=Namespace(contactNS), foaf=Namespace(foafNS))

graph = get_graph( configString, fileURI )

for i in range(len(sparqlQuery)):
        for row in graph.query( sparqlQuery[i], initNs = nsdict ):
                print formatString[i] % row

#for row in query_store( configString, default_graph_uri = fileURI, namespace_bindings=nsdict, queryString = sparqlQuery ):
 #       print formatString % row
