#!/bin/bash
source ~/.bashrc
echo -n -e "Content-type: text/xml\n\n"
java -cp "$HOME/bin/SimpleGraphmlParser.jar" "simplegraphmlparser.Main"
