#!/bin/ksh
# PURPOSE: Test to verify that protos works.
#
# Include Routines
#
TEST_ENV=${0%/*}
cd $TEST_ENV

. ./testlib.sh

# Variables
#
if [ -f makefile ] ; then
	ROOT=root
	USR=$ROOT/usr
else
	ROOT=''
	USR=/usr/local/bin
fi

export ROOT
SBIN=$ROOT/sbin
UBIN=$ROOT/usr/bin

#
#  F U N C T I O N S
#
setup_test_framework() {
	typeset DIR=$1
	if [ ! -f makefile ] ; then
		rm -rf $DIR && mkdir -p $DIR
	fi
}

# # # # # # # # # # #
#  B E G I N
#  T E S T
#  F U N C T I O N S
test_header() {
	begintest 'header'
	FN=regressions.h
	PURPOSE="For regresion testing of protos."

	rm -f $FN
	awk -f ./protos.awk -v purpose="$PURPOSE" regressions.c >/dev/null

	assertFileHas "$FN" \
		"\\\* $FN	\+DATE: [-0-9]\+" \
		"PURPOSE: $PURPOSE" \
		"#ifndef __REGRESSIONS_H__" \
		"#define __REGRESSIONS_H__" \
		"\/\* Global count = 7 \*\/" \
		"int nospace (void);" \
		"int withbrace (void);" \
		"int withspacesaroundvoid (void);" \
		"int withspaceafterfunction (void);" \
		"bool LicCrypt (char \*str, // encrypt or decrypt str" \
                "bool decrypt,          // decrypt if true, encrypt if false" \
                "bool registration);     // registration key if true, license key if false" \
		"int withspacesbeforefunction (void);" \
		"char\* alongfunctionwith_many_paramaters_across_multiple_lines (int a, int b," \
		"int c, int d, char\* str1, char\* str2, char\* str3, char\* str4," \
		"double d1, double d2," \
		"float f1, float f2);" \
		"#endif \/\* __REGRESSIONS_H__ \*\/" \

	endtest
}

test_prototypes() {
	begintest 'prototypes'

	#sed -i '/^static .*);/d' regressions.c

	awk -f ./protos.awk -v purpose="$PURPOSE" regressions.c >/dev/null

	assertFileHas regressions.c \
		"static int withspacesbeforelocalfunction (void);" \
		"static void LicKey (char \*str, // build an unencrypted key" \
		"time_t expire,   // expriation time" \
                "int  device_type," \
                "int  platform_version," \
                "unsigned char  license_type, // 0 is general product license, 1-15 for feature" \
                "unsigned int   data);         // 24 bytes of device_type dependent data" \
		"static int localfunction (void);"
	endtest
}

#  E N D
#  T E S T
#  F U N C T I O N S
# # # # # # # # # # #


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# MAIN	Script	Body
#
setup_test_framework $ROOT

run_tests "Valcom Configuration of vip$TYPE"

display_report
exit $?
