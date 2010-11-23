#Brandon Chisham
#Deployment management for web binaries.
#April 14, 2010

REMOTE_HOST=daedalus.cs.nmsu.edu
REMOTE_BIN_DIR=~/bin
REMOTE_RC_DIR=~/

deploy: Makefile.remote.deployed cdaostorecgi.rc.deployed do_phylows_author_query.sh.deployed do_phylows_algorithm_query.sh.deployed do_phylows_program_query.sh.deployed do_phylows_study_query.sh.deployed do_phylows_tree.sh.deployed do_phylows_tu_query.sh.deployed do_phylows_unordered_tree.sh.deployed order_graphml.sh.deployed prolog_exe_driver.c.deployed do_html_author_query.sh.deployed do_html_algorithm_query.sh.deployed do_html_nca_query.sh.deployed do_html_msc_query.sh.deployed  do_html_tree_query.sh.deployed do_node_query.sh.deployed do_html_program_query.sh.deployed do_tree_query.sh.deployed do_html_tu_query.sh.deployed do_html_size_query.sh.deployed do_html_study_query.sh.deployed do_study_list_authors.sh.deployed do_study_detail_query_construct.sh.deployed do_study_list_trees.sh.deployed query_template_script.sh.deployed do_prolog_msc_query.sh.deployed do_prolog_tu_query.sh.deployed do_author_work_list_query_construct.sh.deployed do_query.py.deployed sparql.py.deployed import.py.deployed store.py.deployed index.py.deployed html.py.deployed

production-deploy: deploy
	(make deploy)
	ssh $(REMOTE_HOST) "cd $(REMOTE_BIN_DIR) && make"

%.rc.deployed: %.rc
	scp $< $(REMOTE_HOST):$(REMOTE_RC_DIR)
	ssh $(REMOTE_HOST) "ln -f $(REMOTE_RC_DIR)/$< $(REMOTE_BIN_DIR)/$<"
	touch $@

%.remote.deployed: %.remote
	scp $< $(REMOTE_HOST):$(REMOTE_BIN_DIR)/Makefile
	touch $@

%.c.deployed: %.c
	scp $< $(REMOTE_HOST):$(REMOTE_BIN_DIR)
	ssh $(REMOTE_HOST) "cd $(REMOTE_BIN_DIR) && gcc -o `echo $< | sed 's/.c$$//'` $< && rm -f $<"
	touch $@

%.sh.deployed: %.sh
	scp $< $(REMOTE_HOST):$(REMOTE_BIN_DIR)
	ssh $(REMOTE_HOST) "chmod 755 $(REMOTE_BIN_DIR)/$<"
	touch $@

%.py.deployed: %.py
	scp $< $(REMOTE_HOST):$(REMOTE_BIN_DIR)
	touch $@ 
%.dat.deployed: %.dat
	scp $< $(REMOTE_HOST):$(REMOTE_DAT_DIR)
	touch $@

