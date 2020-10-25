#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/apple/Desktop/PRALG/zipimage
  make -f /Users/apple/Desktop/PRALG/zipimage/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/apple/Desktop/PRALG/zipimage
  make -f /Users/apple/Desktop/PRALG/zipimage/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/apple/Desktop/PRALG/zipimage
  make -f /Users/apple/Desktop/PRALG/zipimage/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/apple/Desktop/PRALG/zipimage
  make -f /Users/apple/Desktop/PRALG/zipimage/CMakeScripts/ReRunCMake.make
fi

