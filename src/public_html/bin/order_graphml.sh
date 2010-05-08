#!/bin/bash
source ~/.bashrc
echo -e -n "Content-type: text/xml\n\n"
HOME="/home/grad6/bchisham"
java -cp "$HOME/bin/SimpleGraphmlParser.jar" "simplegraphmlparser.Main"
