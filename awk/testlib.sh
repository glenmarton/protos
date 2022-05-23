AUTHOR='Glen Johnson <gjohnson at valcom dot com>'
VERSION='1.3'
progname=${0##*/}
tests=""

usage() {
	cat << EOF 1>&2

Verifies postinstall.sh does what it is expected to.

	usage: $progname [options] testA testB
		-h	help
		-t	Device type. Possible types are:
			 9 - Application Server
			10 - District Server
			15 - Cisco Server
			22 - Application Server Pro
			24 - eLaunch
		-v	version
EOF
}

run_tests() {
	DESC=$1

	echo ""
	echo "Testing $DESC:"

	set_type $TYPE

	for t in $tests ; do
		$t
	done
}

showversion() {
	echo "$progname version $VERSION, by $AUTHOR"
}

## Begin option parsing
while getopts t:hv opt; do
	case "$opt" in
		t) TYPE="$OPTARG" ;;
		v) showversion; exit 0 ;;
		h) usage; exit 0 ;;
		*) usage; exit 2 ;;
	esac
done

shift $(( $OPTIND - 1 ))
if [[ $# > 0 ]] ; then
	for t in $@ ; do
		tests+=" test_$t"
	done
else
	for t in $(grep "^test_" ${0##*/} | sed 's/().*//') ; do
		tests+=" $t"
	done
fi

# Variables
#
ALL_PASS=true
typeset -i TEST_PASS=0
typeset -i TEST_COUNT=0
typeset -i TOTAL_PASS=0
typeset -i TOTAL_COUNT=0

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# Test	Functions
#
tally() {
	typeset -i PASS_CNT=$1
	typeset -i TEST_CNT=$2

	(( TEST_COUNT+=$TEST_CNT ))
	(( TEST_PASS+=$PASS_CNT ))
# Take this out when you get ready to split them. gmj20111014
	endtest
}
total() {
	typeset -i PASS_CNT=$1
	typeset -i TEST_CNT=$2

	(( TEST_COUNT+=$TEST_CNT ))
	(( TEST_PASS+=$PASS_CNT ))
}
begintest() {
	echo -n "	$1: "
	TEST_PASS=0
	TEST_COUNT=0
}
endtest() {
	if [ ! "$TESTALL" ] ; then
		echo "  Passed: $TEST_PASS of $TEST_COUNT"
	fi
	(( TOTAL_COUNT+=$TEST_COUNT ))
	(( TOTAL_PASS+=$TEST_PASS ))
}

grtest() {
	echo -n "	$1: "
	shift
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
#		echo $RESULT
		if echo $RESULT | grep -qs "$1" ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: Result does not contain $1."
		fi
		shift
	done
	tally $N $COUNT
}

assertDirExists() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		if test -d $1 ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: Dir missing '$1'"
		fi
		shift
	done
	total $N $COUNT
}

assertDirExistsNot() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		if test -d $1 ; then
			echo "		FAILURE: Directory exists '$1'"
		else
			echo -n "."
			(( N++ ))
		fi
		shift
	done
	total $N $COUNT
}

function assertEqual {
	typeset EXPECTED=$1
	typeset ACTUAL=$2
	typeset -i COUNT=1
	typeset -i N=0

	if [[ $EXPECTED == $ACTUAL ]] ; then
		echo -n "."
		(( N++ ))
	else
		echo "FAILURE: $EXPECTED and $ACTUAL are not equal."
	fi
	total $N $COUNT
}

assertFileGrep() {
	TITLE=${1##*/}
	FN=$1
	shift
	echo -n "	$TITLE: "
	typeset -i COUNT=$#
	typeset -i N=0
	TEST_PASS=0
	TEST_COUNT=0
	while [ "$#" -gt 0 ] ; do
		if grep -qs "$1" $FN ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: Missing '$1'"
		fi
		shift
	done
	tally $N $COUNT

}

function assertFilesMatch {
	typeset EXPECTED=$1
	typeset ACTUAL=$2
	typeset -i COUNT=1
	typeset -i N=0

	if diff $EXPECTED $ACTUAL >/dev/null ; then
		echo -n "."
		(( N++ ))
	else
		echo "FAILURE: $EXPECTED and $ACTUAL do not match."
		if colordiff -v > /dev/null ; then
			colordiff $EXPECTED $ACTUAL
		else
			diff  $EXPECTED $ACTUAL
		fi
	fi
	total $N $COUNT
}

assertStringHas() {
	STRING=$1
	shift
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		if echo $STRING | grep -qs "$1" ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: '$STRING' missing '$1'"
		fi
		shift
	done
	total $N $COUNT
}

assertStringHasNot() {
	STRING=$1
	shift
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		if ! echo $STRING | grep -qs "$1" ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: '$STRING' has '$1'"
		fi
		shift
	done
	total $N $COUNT
}

function assertFileHas {
	typeset FN=$1
	typeset TITLE=${FN##*/}
	shift
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		grep "gmj: $1" $FN >&2
		if grep -qs "$1" $FN ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: Missing '$1'"
		fi
		shift
	done
	total $N $COUNT
}

assertFileHasNot() {
	TITLE=${1##*/}
	FN=$1
	shift
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		if ! test -f $FN ; then
			echo "		FAILURE: File missing '$FN'; cannot validate '$1'"
		elif grep -qs "$1" $FN ; then
			echo "		FAILURE: Has '$1'"
		else
			echo -n "."
			(( N++ ))
		fi
		shift
	done
	total $N $COUNT
}

assertFileExists() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		if test -f $1 ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: File missing '$1'"
		fi
		shift
	done
	total $N $COUNT
}
assertFileExistsNot() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		if test -f $1 ; then
			echo "		FAILURE: File exists '$1'"
		else
			echo -n "."
			(( N++ ))
		fi
		shift
	done
	total $N $COUNT
}
assertLinkExists() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		if ls -l $1 | grep -qs -- '->' ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: Link missing '$1'"
		fi
		shift
	done
	total $N $COUNT
}
assertLinkExistsNot() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		if test -L $1 ; then
			echo "		FAILURE: Link exists '$1'"
		else
			echo -n "."
			(( N++ ))
		fi
		shift
	done
	total $N $COUNT
}
assertFilePermissions() {
	typeset -i N=0
	FN=$1
	EXPECTED=$2
	RESULT=$(ls -ld $FN | cut -c1-10)

	if echo $RESULT | grep -q -- "$EXPECTED" ; then
		echo -n "."
		(( N++ ))
	else
		echo "	FAILURE: $FN expected $EXPECTED but got $RESULT"
	fi
	total $N 1
}

assertFileGroup() {
	typeset -i N=0
	FN=$1
	EXPECTED=$2
	RESULT=$(sudo ls -ld $FN | awk '{print $4}')
	if [ "$RESULT" = "$EXPECTED" ] ; then
		echo -n "."
		(( N++ ))
	else
		echo "	FAILURE: $FN expected $EXPECTED but got $RESULT"
	fi
	total $N 1
}
assertFileOwner() {
	typeset -i N=0
	FN=$1
	EXPECTED=$2
	RESULT=$(sudo ls -ld $FN | awk '{print $3}')
	if [ "$RESULT" = "$EXPECTED" ] ; then
		echo -n "."
		(( N++ ))
	else
		echo "	FAILURE: $FN expected owner $EXPECTED but got $RESULT"
	fi
	total $N 1
}

assertTrue() {
	typeset -i N=0
	if [ "$1" = "0" ] ; then
		echo -n "."
		(( N++ ))
	else
		echo "	FAILURE: $2, expected success but got $1"
	fi
	total $N 1
}

assertFalse() {
	typeset -i N=0
	if [ "$1" != "0" ] ; then
		echo -n "."
		(( N++ ))
	else
		echo "	FAILURE: $2, expected failure but got $1"
	fi
	total $N 1
}

assertDisableCache() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		DEV=$1
		if $SBIN/hdparm -I $1 | grep -qs "\*\s*Write cache" ; then
			N=$N	# Do nothing test failed.
		else
			(( N++ ))
		fi
		shift
	done
	total $N $COUNT
}

assertServices() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		DAEMON=${1%.*}
		CMD=${1#*.}
		if $SBIN/chkconfig --list | grep -qs "$DAEMON.*$CMD" ||
			([ "$CMD" = "off" ] && ! $SBIN/chkconfig --list | grep -qs "$DAEMON.*")
		then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: Expected $DAEMON to be $CMD."
		fi
		shift
	done
	total $N $COUNT
}

assertRpms() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		DAEMON=${1%-*}
		if $RPM -q $DAEMON >/dev/null 2>&1 ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: rpm $1 not installed."
		fi
		shift
	done
	total $N $COUNT
}

assertNotRpms() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		DAEMON=${1%-*}
		if $RPM -q $DAEMON >/dev/null 2>&1 ; then
			echo "		FAILURE: rpm $1 IS installed."
		else
			echo -n "."
			(( N++ ))
		fi
		shift
	done
	total $N $COUNT
}

assertGem() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		GEM=${1%-*}
		if $UBIN/gem list | grep -qs "$GEM" ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: gem $1 not installed."
		fi
		shift
	done
	total $N $COUNT
}

assertServiceOn() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		if $SBIN/service "$1" status >/dev/null ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: service $1 not running."
		fi
		shift
	done
	total $N $COUNT
}

assertServiceOff() {
	typeset -i COUNT=$#
	typeset -i N=0
	while [ "$#" -gt 0 ] ; do
		if [[ $($SBIN/service "$1" status) == "$1 is stopped" ]] ; then
			echo -n "."
			(( N++ ))
		else
			echo "		FAILURE: service $1 is running."
		fi
		shift
	done
	total $N $COUNT
}

display_report() {
	echo ""
	if [[ $TOTAL_PASS == $TOTAL_COUNT ]] ; then
		echo -n "SUCCESS"
	else
		echo -n "FAILURE"
	fi

	PERCENTAGE=$(awk -v n=$TOTAL_PASS -v d=$TOTAL_COUNT \
		'BEGIN{if(d==0){p-0.0}else{p=n/d*100} ; printf "%.2f",p}')
	echo "	-  Passed $TOTAL_PASS of $TOTAL_COUNT, ($PERCENTAGE%)."
	if [[ $TOTAL_PASS == $TOTAL_COUNT ]] ; then
		return 0
	else
		return 1
	fi
}

set_type() {
	TYPE=$1

	unset IS_VECAP IS_SCHEDULER IS_APP_PRO IS_TPS
	if [ "$TYPE" = '9' ] ; then
		export IS_SCHEDULER='true'
	elif [ "$TYPE" = '22' ] ; then
		export IS_APP_PRO='true'
		export IS_SCHEDULER='true'
	elif [ "$TYPE" = '24' ] ; then
		export IS_VECAP='true'
	elif [ "$TYPE" = '27' ] ; then
		export IS_TPS='true'
	elif [ "$TYPE" = '32' ] ; then
		export IS_EMAIL='true'
	fi
}

