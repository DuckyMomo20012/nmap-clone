include ./Make.defines

OBJS = main.o proc_v4.o readloop.o \
		send_v4.o sig_alrm.o tv_sub.o
PROGS =	nmap

all:	${PROGS}

ping:	${OBJS}
		${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
