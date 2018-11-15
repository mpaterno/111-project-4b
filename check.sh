LAB="lab4b"
README="README"
MAKEFILE="Makefile"

SOURCES=""
EXPECTED=""
EXPECTEDS=".c"
PGM="lab4b"
PGMS=$PGM
TEST_PGM=pipe_test

SUFFIXES=""

EXIT_OK=0
EXIT_ARG=1
EXIT_FAIL=1

TIMEOUT=1

let errors=0

# test input from pipe_test
echo
echo "... $PGM correctly processes input from $TEST_PGM"
../$TEST_PGM ./$PGM --period=$p --scale=$s --log="LOGFILE" > STDOUT 2> STDERR <<-EOF
	PAUSE 2
	SEND "SCALE=F\n"
	PAUSE 1
	SEND "PERIOD=1\n"
	PAUSE 1
	SEND "STOP\n"
	PAUSE 3
	SEND "START\n"
	PAUSE 2
	SEND "LOG test\n"
	PAUSE 2
	SEND "OFF\n"
	PAUSE 2
	CLOSE
EOF

if [ ! -s LOGFILE ]
then
	echo "did not create a log file"
	let errors+=1
else
	for c in SCALE=F PERIOD=1 START STOP OFF SHUTDOWN "LOG test"
	do
		grep "$c" LOGFILE > /dev/null
		if [ $? -ne 0 ]
		then
			echo "DID NOT LOG $c command"
			let errors+=1
		else
			echo "    $c ... RECOGNIZED AND LOGGED"
		fi
	done

	if [ $errors -gt 0 ]
	then
		echo "   LOG FILE DUMP FOLLOWS   "
		cat LOGFILE
	fi
fi

echo
echo "... correct reporting format"
egrep '[0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9]+\.[0-9]\>' LOGFILE > FOUND
if [ $? -eq 0 ] 
then
	echo "   " `cat FOUND` "... OK"
else
	echo NO VALID REPORTS IN LOGFILE:
	let errors+=1
	cat LOGFILE
fi