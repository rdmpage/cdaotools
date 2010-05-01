#Brandon Chisham
#Feb. 18, 2010.
#Deployment management makefile.

REMOTE_HOST=daedalus.cs.nmsu.edu
REMOTE_WEB_DIR=~/public_html/
REMOTE_BIN_DIR=~/bin/
REMOTE_DAT_DIR=~/bin/
REMOTE_XSL_DIR=~/public_html/




deploy:index.php.deployed footer.php.deployed header.php.deployed faq.php.deployed phylows.php.deployed about.php.deployed  style.css.deployed cdao-triplestore-logo.jpg.deployed cdao-triplestore-logo.ico.deployed nexml_to_csv.xsl.deployed study_result_to_html.xsl.deployed tu_to_filelist.xsl.deployed study_authors_to_html.xsl.deployed trees_to_file.xsl.deployed
	(cd bin && make deploy)
	(cd cgi-bin && make deploy)

%.php.deployed: %.php
	scp $< $(REMOTE_HOST):$(REMOTE_WEB_DIR)
	ssh $(REMOTE_HOST) "chmod 644 $(REMOTE_WEB_DIR)/$<"
	touch $@

%.css.deployed: %.css
	scp $< $(REMOTE_HOST):$(REMOTE_WEB_DIR)
	ssh $(REMOTE_HOST) "chmod 644 $(REMOTE_WEB_DIR)/$<"
	touch $@
%.jpg.deployed: %.jpg
	scp $< $(REMOTE_HOST):$(REMOTE_WEB_DIR)
	ssh $(REMOTE_HOST) "chmod 644 $(REMOTE_WEB_DIR)/$<"
	touch $@

%.ico.deployed: %.ico
	scp $< $(REMOTE_HOST):$(REMOTE_WEB_DIR)
	ssh $(REMOTE_HOST) "chmod 644 $(REMOTE_WEB_DIR)/$<"
	touch $@

%.xsl.deployed: %.xsl
	scp $< $(REMOTE_HOST):$(REMOTE_WEB_DIR)/$(REMOTE_STATIC_WEB_DIR)
	touch $@
