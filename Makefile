CFLAGS = -O1 -Wall

all: log.c
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f log *.o
