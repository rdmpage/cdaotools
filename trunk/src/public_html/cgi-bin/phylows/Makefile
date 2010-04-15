#Handle deployment  of the phylows interface
REMOTE_WS_DIR=~/public_html/cgi-bin/phylows
REMOTE_WS_HOST=daedalus.cs.nmsu.edu

deploy: tree.deployed listing.deployed matrix.deployed msc.deployed nca.deployed phylows.env.deployed size.deployed tree-count.pl.deployed tree-query-main.pl.deployed

listing.deployed: listing
	scp listing $(REMOTE_WS_HOST):$(REMOTE_WS_DIR)
	ssh $(REMOTE_WS_HOST) "cd $(REMOTE_WS_DIR) && chmod 755 listing"
	touch listing.deployed

tree.deployed: tree phylows.env.deployed
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
nca.deployed: nca phylows.env.deployed
	scp nca $(REMOTE_WS_HOST):$(REMOTE_WS_DIR)
	ssh $(REMOTE_WS_HOST) "cd $(REMOTE_WS_DIR) && chmod 755 nca"
	touch nca.deployed
phylows.env.deployed: phylows.env
	scp phylows.env $(REMOTE_WS_HOST):$(REMOTE_WS_DIR)
	touch phylows.env.deployed
size.deployed: size tree-query-main.pl
	scp size $(REMOTE_WS_HOST):$(REMOTE_WS_DIR)
	ssh $(REMOTE_WS_HOST) "cd $(REMOTE_WS_DIR) && chmod 755 size"
	touch size.deployed
tree-count.pl.deployed: tree-count.pl
	scp tree-count.pl $(REMOTE_WS_HOST):$(REMOTE_WS_DIR)
	ssh $(REMOTE_WS_HOST) "cd $(REMOTE_WS_DIR) && gzip -f tree-count.pl && zcat prolog-trees.pl.gz tree-count.pl.gz | gzip  > prolog-trees-integrated-functions.pl.gz"
	touch tree-count.pl.deployed
tree-query-main.pl.deployed: tree-query-main.pl
	scp tree-query-main.pl $(REMOTE_WS_HOST):$(REMOTE_WS_DIR)
#ssh $(REMOTE_WS_HOST) "cd $(REMOTE_WS_DIR) && pl -o tree-query-main -c tree-query-main.pl"
	touch tree-query-main.pl.deployed
