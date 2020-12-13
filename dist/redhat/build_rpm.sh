#!/usr/bin/sh

# IMPORTANT: This script must be run from the project root directory.

chmod +x dist/redhat/pack_for_rpm.sh
dist/redhat/pack_for_rpm.sh
mv v1.0.1.tar.gz ~/rpmbuild/SOURCES/
cp dist/redhat/pgs++.spec ~/rpmbuild/SPECS
rpmbuild -bb ~/rpmbuild/SPECS/pgs++.spec
