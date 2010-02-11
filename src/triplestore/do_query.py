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
from sparql import query_store
from html import start_html, head, open_div, anchor, end_div, open_body, par, anchor ,end_body, end_html, heading_h1

cdaoURI= "http://www.evolutionaryontology.org/cdao.owl"
rdfNS="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
for row in query_store( sys.argv[1], default_graph_uri = cdaoURI, namespace_bindings=dict(cdao=Namespace(cdaoURI+"#"), rdf=Namespace(rdfNS)), queryString = sys.argv[2] ):
        print sys.argv[3] % row
