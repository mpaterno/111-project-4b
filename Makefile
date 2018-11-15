
# default:
# 	gcc -Wall -Wextra -DDUMMY -g -o lab4b lab4b.c
default:
	gcc -lmraa -lm  -Wall -Wextra -g -o lab4b lab4b.c

check: default
	echo "OFF" | ./lab4b --log=test.txt
	grep -Eo '[6-9][0-9]' test.txt
	rm -rf test.txt

clean:
	rm -f lab4b *.txt *.o *.tar.gz

dist:
	tar -czvf lab4b-904756085.tar.gz Makefile README lab4b.c

argTest:
	./lab4b --random \
	if [[ $$? -ne 1 ]]; \
	then \
		echo "failure to recognize bogus argument: exit code was not 1\n" > error_output.txt \
	fi;