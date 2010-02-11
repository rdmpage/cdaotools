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
from store import store_ontology
from html import start_html, head, open_div, anchor, end_div, open_body, par, anchor ,end_body, end_html, heading_h1


print start_html()
print head("Triples Storage System")
print open_body()
print open_div("", "main-content")
print heading_h1("Triples Storage System") 
print par( "Processing Your submission url: " + anchor( unquote( sys.argv[2] ), unquote( sys.argv[2] ) ) ) 
store_ontology( sys.argv[1], unquote( sys.argv[2] ) )
print par("Imported Graph " + anchor( unquote( sys.argv[1] ), unquote( sys.argv[1] ) ) )
print par( anchor( "http://www.cs.nmsu.edu/~bchisham/cgi-bin/import", "Import another url" ) )
print end_div()
print end_body() 
print end_html()
