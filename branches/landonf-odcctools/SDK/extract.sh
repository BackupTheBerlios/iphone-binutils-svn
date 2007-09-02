#!/bin/sh

set -e

DISTDIR=arm-apple-darwin
SDKROOT=/Developer/SDKs/MacOSX10.4u.sdk
FRAMEWORK_ROOT=${SDKROOT}/System/Library/Frameworks

TOPSRCDIR=`pwd`

MAKEDISTFILE=0
UPDATEPATCH=0

while [ $# -gt 0 ]; do
    case $1 in
	--distfile)
	    shift
	    MAKEDISTFILE=1
	    ;;
	--updatepatch)
	    shift
	    UPDATEPATCH=1
	    ;;
	--heavenly)
	    shift
	    HEAVENLY=$1
	    shift
	    ;;
	--help)
	    echo "Usage: $0 [--help] [--distfile] [--updatepatch] [--heavenly dir]" 1>&2
	    exit 0
	    ;;
	*)
	    echo "Unknown option $1" 1>&2
	    exit 1
    esac
done

if [ "$HEAVENLY" == "" ]; then
    echo "Missing required --heavenly option"
    exit 1
fi

if [ ! -d "$HEAVENLY" ]; then
    echo "$HEAVENLY does not exist"
    exit 1
fi

if [ "`tar --help | grep -- --strip-components 2> /dev/null`" ]; then
    TARSTRIP=--strip-components
else
    TARSTRIP=--strip-path
fi

PATCHFILESDIR=${TOPSRCDIR}/patches

PATCHFILES=`cd "${PATCHFILESDIR}" && find * -type f \! -path \*/.svn\*`

ADDEDFILESDIR=${TOPSRCDIR}/files

if [ -d "${DISTDIR}" ]; then
    echo "${DISTDIR} already exists. Please move aside before running" 1>&2
    exit 1
fi

mkdir -p ${DISTDIR}

echo "Merging content from $SDKROOT"
if [ ! -d "$SDKROOT" ]; then
    echo "$SDKROOT must be present" 1>&2
    exit 1
fi

# Standard includes
tar cf - -C "$SDKROOT/usr/" include | tar xf - -C ${DISTDIR}

# CarbonCore
mkdir -p ${DISTDIR}/include/CarbonCore
tar cf - -C "$FRAMEWORK_ROOT/CoreServices.framework/Frameworks/CarbonCore.framework/Headers" . | tar xf - -C ${DISTDIR}/include/CarbonCore

# CoreGraphics
mkdir -p ${DISTDIR}/include/CoreGraphics
tar cf - -C "$FRAMEWORK_ROOT/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers" . | tar xf - -C ${DISTDIR}/include/CoreGraphics

# Standard libraries
tar cf - -C "$HEAVENLY/usr" lib | tar xf - -C ${DISTDIR}

set +e

INTERACTIVE=0
echo "Applying patches"
for p in ${PATCHFILES}; do			
    dir=`dirname $p`
    if [ $INTERACTIVE -eq 1 ]; then
	read -p "Apply patch $p? " REPLY
    else
	echo "Applying patch $p"
    fi
    pushd ${DISTDIR}/$dir > /dev/null
    patch --backup --posix -p0 < ${PATCHFILESDIR}/$p
    if [ $? -ne 0 ]; then
	echo "There was a patch failure. Please manually merge and exit the sub-shell when done"
	$SHELL
	if [ $UPDATEPATCH -eq 1 ]; then
	    find . -type f | while read f; do
		if [ -f "$f.orig" ]; then
		    diff -u -N "$f.orig" "$f"
		fi
	    done > ${PATCHFILESDIR}/$p
	fi
    fi
    find . -type f -name \*.orig -exec rm -f "{}" \;
    popd > /dev/null
done

set -e

echo "Adding new files"
tar cf - --exclude=CVS --exclude=.svn -C ${ADDEDFILESDIR} . | tar xvf - -C ${DISTDIR}

echo "Deleting cruft"
find ${DISTDIR} -name Makefile -exec rm -f "{}" \;
find ${DISTDIR} -name \*~ -exec rm -f "{}" \;
find ${DISTDIR} -name .\#\* -exec rm -f "{}" \;

if [ $MAKEDISTFILE -eq 1 ]; then
    DATE=$(date +%Y%m%d)
    mv ${DISTDIR} ${DISTDIR}-$DATE
    tar jcf ${DISTDIR}-$DATE.tar.bz2 ${DISTDIR}-$DATE
fi

echo "WARNING: ${DISTDIR} contains NON-REDISTRIBUTABLE Mac OS X components, merged from ${SDKROOT}. Do not re-distribute this directory!"

exit 0
