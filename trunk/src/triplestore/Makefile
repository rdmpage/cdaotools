#Brandon Chisham
#Feb. 18, 2010.
#Deployment management makefile.

REMOTE_HOST=vm1.cs.nmsu.edu
REMOTE_WEB_DIR=~/public_html/
REMOTE_BIN_DIR=~/bin/
REMOTE_DAT_DIR=~/bin/
REMOTE_XSL_DIR=~/public_html/

LOCAL_STATIC_WEB_DIR=html
REMOTE_STATIC_WEB_DIR=cdao-store

deploy: do_html_author_query.sh.deployed do_html_tree_query.sh.deployed do_html_study_query.sh.deployed do_study_list_authors.sh.deployed do_study_detail_query_construct.sh.deployed do_study_list_trees.sh.deployed query_template_script.sh.deployed do_prolog_tu_query.sh.deployed do_author_work_list_query_construct.sh.deployed do_query.py.deployed sparql.py.deployed import.py.deployed store.py.deployed index.py.deployed html.py.deployed study_authors_to_html.xsl.deployed study_result_to_html.xsl.deployed
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
%.xsl.deployed: %.xsl
	scp $< $(REMOTE_HOST):$(REMOTE_WEB_DIR)/$(REMOTE_STATIC_WEB_DIR)
	touch $@