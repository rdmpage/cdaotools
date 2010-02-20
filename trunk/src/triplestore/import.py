#!/usr/bin/env python


import html
import cgitb
import cgi
import re
import sys
#print "Content-Type: text/html\n\n";
cgitb.enable()
#form = cgi.FieldStorage()
from urllib import unquote
from store import connect_store store_ontology
from html import start_html, head, open_div, anchor, end_div, open_body, par, anchor ,end_body, end_html, heading_h1

import_list = sys.argv[2::1]

print start_html()
print head("Triples Storage System")
print open_body()
print open_div("", "main-content")
print heading_h1("Triples Storage System")
print par( "Connecting to the store..." )
graph = connect_store( sys.argv[1], sys.argv[2] )
for guri in import_list:
        print par( "Processing Your submission url: " + anchor( unquote( guri ), unquote( guri ) ) )
        store_ontology( graph, unquote( guri ) )
        print par("Imported Graph " + anchor( unquote( guri ), guri ) ) )
print par( anchor( "http://www.cs.nmsu.edu/~bchisham/cgi-bin/import", "Import another url" ) )
print end_div()
print end_body() 
print end_html()
