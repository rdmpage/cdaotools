SUBDIRS = ncl example $(MAYBE_TEST_DIR)

EXTRA_DIST = data html test Jamroot

dist-hook:
	find "$(distdir)/data" -depth -name .svn -and -type d -and -exec rm -rf {} \; 
	find "$(distdir)/html" -depth -name .svn -and -type d -and -exec rm -rf {} \; 
	find "$(distdir)/test" -depth -name .svn -and -type d -and -exec rm -rf {} \; 

