#Brandon Chisham
#Feb. 18, 2010.
#Deployment management makefile.

REMOTE_HOST=vm1.cs.nmsu.edu
REMOTE_WEB_DIR=~/public_html/
REMOTE_BIN_DIR=~/bin/
REMOTE_DAT_DIR=~/bin/

LOCAL_STATIC_WEB_DIR=html
REMOTE_STATIC_WEB_DIR=cdao-store

deploy: do_prolog_nca_query.sh.deployed do_prolog_tree_query.sh.deployed query_template_script.sh.deployed do_prolog_tu_query.sh.deployed do_tu_query_construct.sh.deployed tree_to_file.dat.deployed sparql.py.deployed import.py.deployed store.py.deployed index.py.deployed html.py.deployed
	(cd html && make deploy)


%.sh.deployed: %.sh
	scp $< $(REMOTE_HOST):$(REMOTE_BIN_DIR)
	touch $@

%.py.deployed: %.py
	scp $< $(REMOTE_HOST):$(REMOTE_BIN_DIR)
	touch $@ 
%.dat.deployed: %.dat
	scp $< $(REMOTE_HOST):$(REMOTE_DAT_DIR)
	touch $@
