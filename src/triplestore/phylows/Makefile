#Handle deployment  of the phylows interface
REMOTE_WS_DIR=~/public_html/cgi-bin/phylows
REMOTE_WS_HOST=daedalus.cs.nmsu.edu

deploy: tree.deployed matrix.deployed msc.deployed nca.deployed

tree.deployed: tree
	scp tree $(REMOTE_WS_HOST):$(REMOTE_WS_DIR)
	ssh $(REMOTE_WS_HOST) "cd $(REMOTE_WS_DIR) && chmod 755 tree"
	touch tree.deployed

matrix.deployed: matrix
	scp matrix $(REMOTE_WS_HOST):$(REMOTE_WS_DIR)
	ssh $(REMOTE_WS_HOST) "cd $(REMOTE_WS_DIR) && chmod 755 matrix"
	touch matrix.deployed
msc.deployed: msc nca.deployed
	scp msc $(REMOTE_WS_HOST):$(REMOTE_WS_DIR)
	ssh $(REMOTE_WS_HOST) "cd $(REMOTE_WS_DIR) && chmod 755 msc"
	touch msc.deployed
nca.deployed: nca
	scp nca $(REMOTE_WS_HOST):$(REMOTE_WS_DIR)
	ssh $(REMOTE_WS_HOST) "cd $(REMOTE_WS_DIR) && chmod 755 nca"
	touch nca.deployed
