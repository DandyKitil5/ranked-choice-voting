# CMSC216 Project 1 Makefile
# Updated: Thu Sep 12 05:52:36 PM EDT 2024 
AN = p1
CLASS = 216

# -Wno-comment: disable warnings for multi-line comments, present in some tests
# -Werror=format-security: warn/error for using printf() with raw strings
CFLAGS = -Wall -Werror -g -Wno-unused-variable
CC     = gcc $(CFLAGS)
SHELL  = /bin/bash
CWD    = $(shell pwd | sed 's/.*\///g')

PROGRAMS = \
	test_rcv_funcs  \
	rcv_main  \

export PARALLEL = True		#enable parallel testing

all : $(PROGRAMS) 

# cleaning target to remove compiled programs/objects
clean :
	rm -f $(PROGRAMS) *.o

help :
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'
	@echo '  > make zip                      # create a zip file for submission'
	@echo '  > make prob1                    # built targets associated with problem 1'
	@echo '  > make test                     # run all tests'
	@echo '  > make test-prob2               # run test for problem 2'
	@echo '  > make test-prob2 testnum=5     # run problem 2 test #5 only'
	@echo '  > make update                   # download and install any updates to project files'


############################################################
# 'make zip' to create complete.zip for submission
ZIPNAME = $(AN)-complete.zip
zip : clean clean-tests
	rm -f $(ZIPNAME)
	cd .. && zip "$(CWD)/$(ZIPNAME)" -r "$(CWD)"
	@echo Zip created in $(ZIPNAME)
	@if (( $$(stat -c '%s' $(ZIPNAME)) > 10*(2**20) )); then echo "WARNING: $(ZIPNAME) seems REALLY big, check there are no abnormally large test files"; du -h $(ZIPNAME); fi
	@if (( $$(unzip -t $(ZIPNAME) | wc -l) > 256 )); then echo "WARNING: $(ZIPNAME) has 256 or more files in it which may cause submission problems"; fi

############################################################
# `make update` to get project updates
update :
	curl -s https://www.cs.umd.edu/~profk/216/p1-update.sh | /bin/bash 

############################################################
# ranked-choice voting problem
rcv_main : rcv_main.o rcv_funcs.o 
	$(CC) -o $@ $^

rcv_main.o : rcv_main.c rcv.h
	$(CC) -c $<

rcv_funcs.o : rcv_funcs.c rcv.h
	$(CC) -c $<

test_rcv_funcs : test_rcv_funcs.c rcv_funcs.o 
	$(CC) -o $@ $^



# problem targets
prob1 : rcv_funcs.o test_rcv_funcs

prob2 : rcv_funcs.o test_rcv_funcs

prob3 : rcv_main test_rcv_funcs

# Testing Targets
test : test-prob1 test-prob2 test-prob3

test-setup:
	@chmod u+x testy

test-prob1 : test_rcv_funcs test-setup
	./testy -o md test_rcv1.org $(testnum)

test-prob2 : test_rcv_funcs test-setup
	./testy -o md test_rcv2.org $(testnum)

test-prob3 : test_rcv_funcs rcv_main test-setup
	./testy -o md test_rcv3.org $(testnum)

test-makeup : rcv_main
	./testy -o md test_rcv_makeup.org $(testnum)

clean-tests :
	rm -rf test-results

