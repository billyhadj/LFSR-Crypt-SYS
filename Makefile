CC = gcc
CFLAGS = -Wall -O3

.SUFFIXES: .c .o

PRGS = lfsr_test

all:\
	$(PRGS)

lfsr_test:\
	lfsr_test.o lfsr.o
	$(CC) lfsr_test.o lfsr.o -o $@

.c.o: \
	$*.h $*.c
	@echo $*
	$(CC) $(CFLAGS) -c $< 

clean:
	find . -name '*.o' -exec rm -f {} ';'
	find . -name '*~' -exec rm -f {} ';'
	find . -name '#*#' -exec rm -f {} ';'
	find . -name '.#*' -exec rm -f {} ';'
	find . -name 'core' -exec rm -f {} ';'
	find . -name '*.core' -exec rm -f {} ';'
	-rm -rf $(PRGS)
