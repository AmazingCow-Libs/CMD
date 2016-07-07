#!/bin/bash
##----------------------------------------------------------------------------##
##               █      █                                                     ##
##               ████████                                                     ##
##             ██        ██                                                   ##
##            ███  █  █  ███        update_version.sh                         ##
##            █ █        █ █        CMD                                       ##
##             ████████████                                                   ##
##           █              █       Copyright (c) 2016                        ##
##          █     █    █     █      AmazingCow - www.AmazingCow.com           ##
##          █     █    █     █                                                ##
##           █              █       N2OMatt - n2omatt@amazingcow.com          ##
##             ████████████         www.amazingcow.com/n2omatt                ##
##                                                                            ##
##                  This software is licensed as GPLv3                        ##
##                 CHECK THE COPYING FILE TO MORE DETAILS                     ##
##                                                                            ##
##    Permission is granted to anyone to use this software for any purpose,   ##
##   including commercial applications, and to alter it and redistribute it   ##
##               freely, subject to the following restrictions:               ##
##                                                                            ##
##     0. You **CANNOT** change the type of the license.                      ##
##     1. The origin of this software must not be misrepresented;             ##
##        you must not claim that you wrote the original software.            ##
##     2. If you use this software in a product, an acknowledgment in the     ##
##        product IS HIGHLY APPRECIATED, both in source and binary forms.     ##
##        (See opensource.AmazingCow.com/acknowledgment.html for details).    ##
##        If you will not acknowledge, just send us a email. We'll be         ##
##        *VERY* happy to see our work being used by other people. :)         ##
##        The email is: acknowledgment_opensource@AmazingCow.com              ##
##     3. Altered source versions must be plainly marked as such,             ##
##        and must not be misrepresented as being the original software.      ##
##     4. This notice may not be removed or altered from any source           ##
##        distribution.                                                       ##
##     5. Most important, you must have fun. ;)                               ##
##                                                                            ##
##      Visit opensource.amazingcow.com for more open-source projects.        ##
##                                                                            ##
##                                  Enjoy :)                                  ##
##----------------------------------------------------------------------------##

################################################################################
## This will update the ProjectName (HEADER_FILE) and the                     ##
## Doxyfile (DOXY_FILE) version strings.                                      ##
## Very dirty, but does the job - We were forgeting to update the version     ##
## string before each release.                                                ##
## Today it is local to this project (but if it become too complex) it will   ##
## be placed into a separate project                                          ##
################################################################################

################################################################################
## CONFIG                                                                     ##
################################################################################
PROJECT_NAME="CMD"
HEADER_FILE=./include/CMD_Utils.h
DOXY_FILE=Doxyfile


################################################################################
## Vars                                                                       ##
################################################################################
MAJOR=$(echo $@ | cut -d. -f1);
MINOR=$(echo $@ | cut -d. -f2);
REVISION=$(echo $@ | cut -d. -f3);


################################################################################
## Sanity                                                                     ##
################################################################################
#Thanks to Charles Duffy in SO.
#http://stackoverflow.com/questions/806906/how-do-i-test-if-a-variable-is-a-number-in-bash
re='^[0-9]+$' #Check if is number.

if ! [[ $MAJOR =~ $re ]] ; then
   echo "MAJOR Not a number" >&2;
   exit 1
fi

if ! [[ $MINOR =~ $re ]] ; then
   echo "MINOR Not a number" >&2;
   exit 1
fi

if ! [[ $REVISION =~ $re ]] ; then
   echo "REVISION Not a number" >&2;
   exit 1
fi


################################################################################
## Update HEADER FILE                                                         ##
################################################################################
MAJOR_STR="#define COW_${PROJECT_NAME}_VERSION_MAJOR    ";
MINOR_STR="#define COW_${PROJECT_NAME}_VERSION_MINOR    ";
REVISION_STR="#define COW_${PROJECT_NAME}_VERSION_REVISION ";

cat $HEADER_FILE \
    | sed -e s/"$MAJOR_STR\".*\""/"$MAJOR_STR\"$MAJOR\""/g          \
    | sed -e s/"$MINOR_STR\".*\""/"$MINOR_STR\"$MINOR\""/g          \
    | sed -e s/"$REVISION_STR\".*\""/"$REVISION_STR\"$REVISION\""/g \
    > $HEADER_FILE.new;

################################################################################
## Update DOXY FILE                                                           ##
################################################################################
PROJECT_NUMBER_STR="PROJECT_NUMBER         ="
PROJECT_NUMBER_STR_REPLACE="PROJECT_NUMBER         = v$MAJOR.$MINOR.$REVISION";

cat $DOXY_FILE \
    | sed -e s/"$PROJECT_NUMBER_STR.*"/"$PROJECT_NUMBER_STR_REPLACE"/g \
    > $DOXY_FILE.new;



################################################################################
## CHECKING                                                                   ##
################################################################################
## CHECK IF OPERATION WAS OK ##
cat $HEADER_FILE.new;

echo "Is this correct?[y/n]";
read CORRECT;

if [ "$CORRECT" = "y" ]; then
    echo "Updating the files..."
    mv $DOXY_FILE.new  $DOXY_FILE;
    mv $HEADER_FILE.new  $HEADER_FILE;
else
  rm $DOXY_FILE.new
  rm $HEADER_FILE.new
fi;

