#Deployment management for the cgi-bin binaries.
#Brandon Chisham
#April 14, 2010

REMOTE_HOST=daedalus.cs.nmsu.edu
REMOTE_CGI_DIR=~/public_html/cgi-bin



deploy: cdao-import.deployed
	(cd phylows && make deploy)

cdao-import.deployed: cdao-import.sh
	scp cdao-import.sh $(REMOTE_HOST):$(REMOTE_CGI_DIR)/cdao-import
	ssh $(REMOTE_HOST) "cd $(REMOTE_CGI_DIR) && chmod 711 cdao-import"
	touch cdao-import.deployed
