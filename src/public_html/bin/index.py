#!/usr/bin/env python


import html
import cgitb
import cgi
import re

print "Content-Type: text/html\n\n";
cgitb.enable()
form = cgi.FieldStorage()

from store import store_ontology
from html import start_html, head, open_body, par, anchor ,end_body, end_html, heading_h1


print start_html()
print head("Triples Storage System")
print open_body()
print heading_h1("Triples Storage System") 
print par( "Processing Your submission url: " + form["ontology"].value  ) 
store_ontology( form["ontology"].value )
print par("Imported Graph " + form["ontology"].value )
print  end_body() 
print end_html()
