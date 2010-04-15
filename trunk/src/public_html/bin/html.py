#Templates for common html elements.

def start_html():
        return """<?xml version=\"1.0\"?>
                  <!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML+RDFa 1.0//EN\" \"http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd\">
                  <html 
                        xmlns=\"http://www.w3.org/1999/xhtml\"
                        xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"
                        xmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\"
                        xmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\"
                        xmlns:cc=\"http://creativecommons.org/ns#\"
                        xmlns:dc=\"http://purl.org/dc/elements/1.1/\"
                        xmlns:foaf=\"http://xmlns.com/foaf/0.1/\"
                        xml:lang=\"en\">"""
def head(title):
        return """<head profile=\"http://www.w3.org/1999/xhtml/vocab\">
                      <link rel=\"stylesheet\" type=\"text/css\" href=\"../style.css\"/>
                      <title>""" + title + "</title>" + "</head>"

def open_div(id,cls):
	return "<div id= \"" + id + "\" class=\"" + cls + "\">"

def end_div():
	return "</div>"
def open_body():
        return "<body>"

def end_body():
        return "</body>"

def end_html():
        return "</html>"

def heading_h1( content ):
        return "<h1>" + content + "</h1>"

def anchor( url, text ):
	return "<a href=\"" + url + "\">" + text + "</a>"

def par( content ):
        return "<p>" + content + "</p>"

def begin_table():
        return "<table>"

def end_table():
        return "</table>"

def anchor( url, name):
        return "<a href=\"" + url + "\">" + name + "</a>"
