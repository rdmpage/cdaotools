#Brandon Chisham
#Feb. 18, 2010.
#Deployment management makefile.

REMOTE_HOST=daedalus.cs.nmsu.edu
REMOTE_WEB_DIR=~/public_html/
REMOTE_BIN_DIR=~/bin/

LOCAL_STATIC_WEB_DIR=.
REMOTE_STATIC_WEB_DIR=cdao-store

deploy: index.html.deployed


%.html.deployed: %.html
	scp $< $(REMOTE_HOST):$(REMOTE_WEB_DIR)/$(REMOTE_STATIC_WEB_DIR)
	touch $@
