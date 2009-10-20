#! /bin/sh
#Brandon Chisham
#Script to automate adding autotools to a project
set -x
aclocal -I config
libtoolize --force --copy
autoheader
automake --add-missing --copy
autoconf
