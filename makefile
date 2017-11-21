CC = gcc
CFLAGS = -Wall -Werror -Wmissing-prototypes
OBJS = krashell.o utils.o commands.o
PROG = krashell
LOCALLIBDIR = /usr/local/lib
LDFLAGS = -L$(LOCALLIBDIR)


all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

krashell.o: utils.h commands.h 
utils.o: utils.h commands.h
commands.o: commands.h

clean:
	rm -f *~ *.o $(PROG) core a.out
