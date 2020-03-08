#!/bin/sh
# $Id: display2pdf.sh,v 1.1 2019-05-15 17:08:40-07 - - $

# Run an X11 program and convert the screen into pdf and ps files.
# See import manual for other options.
# Examples:
# -resize 80% to make the image fit the page
# ???  -units pixelsperinch
# ???  -density 72

if [ "$1" = "" ]; then
   echo Usage: $0 filename.pdf "[options...]" 1>&2
   exit 1
fi

basename=$1
shift
options="$*"

import \
   -border \
   -frame \
   -page letter \
   -gravity center\
   $options \
   $basename.img.pdf
xpdf $basename.img.pdf
pdf2ps $basename.img.pdf
gv $basename.img.ps

