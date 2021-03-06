#! /bin/bash

TAG=${1-HEAD}
cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
MODULES=`$cvswork/configuration/scripts/modules-toplevel.sh`;
CVSREPO=`cat $cvswork/configuration/CVS/Root`;

GRALROOT=GrAL
cd ${HOME}/tmp;
rm -rf ${GRALROOT};
mkdir ${GRALROOT};
cd ${GRALROOT};
for i in ${MODULES}
do
 cvs -d ${CVSREPO} export -r ${TAG} $i;
done;
mv gral/README .
mv gral/LICENSE .
mv gral/MODULES .
mv gral/INSTALL .
mv gral/Makefile.global ./Makefile

rm -rf configuration/scripts
rm -f  gral/TODO.html

# assume docs will be installed under ./doc
mv gral/index-global.html ./index.html 
perl -p -i -e 's|\$GRALDOC|doc|g' ./index.html


# insert license text in all sources
find . -name "*.h" -exec ${cvswork}/configuration/scripts/replace-license.pl {} \;
find . -name "*.C" -exec ${cvswork}/configuration/scripts/replace-license.pl {} \;


cd ..;
tar cf gral-src.tar ${GRALROOT};
gzip -f gral-src.tar;
rm -rf ${GRALROOT};

