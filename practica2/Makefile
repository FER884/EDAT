########################################################
CC = gcc
CFLAGS = -g -Wall
IFLAGS = -I.. -I.
LDFLAGS = -L.
LDLIBS = -lstack

EJS = p2_e1 p2_e2a p2_e2b p2_e3
STACK_EJS = p2_e1s p2_e2as p2_e2bs p2_e3s

RADIO_COMMON_OBJS = radio.o music.o queue.o
P2_E1_OBJS = p2_e1.o $(RADIO_COMMON_OBJS)
P2_E2A_OBJS = p2_e2a.o $(RADIO_COMMON_OBJS)
P2_E2B_OBJS = p2_e2b.o $(RADIO_COMMON_OBJS)
P2_E3_OBJS = p2_e3.o $(RADIO_COMMON_OBJS)
STACK_OBJS = stack.o
########################################################

all: $(EJS) $(STACK_EJS) clear

p2_e1: $(P2_E1_OBJS) libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P2_E1_OBJS) $(LDFLAGS) $(LDLIBS)

p2_e2a: $(P2_E2A_OBJS) libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P2_E2A_OBJS) $(LDFLAGS) $(LDLIBS)

p2_e2b: $(P2_E2B_OBJS) libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P2_E2B_OBJS) $(LDFLAGS) $(LDLIBS)

p2_e3: $(P2_E3_OBJS) libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P2_E3_OBJS) $(LDFLAGS) $(LDLIBS)

p2_e1s: $(P2_E1_OBJS) $(STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E1_OBJS) $(STACK_OBJS)

p2_e2as: $(P2_E2A_OBJS) $(STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E2A_OBJS) $(STACK_OBJS)

p2_e2bs: $(P2_E2B_OBJS) $(STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E2B_OBJS) $(STACK_OBJS)

p2_e3s: $(P2_E3_OBJS) $(STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E3_OBJS) $(STACK_OBJS)

p2_e1.o: p2_e1.c ../music.h ../radio.h stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p2_e2a.o: p2_e2a.c ../music.h ../radio.h stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p2_e2b.o: p2_e2b.c ../music.h ../radio.h stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p2_e3.o: p2_e3.c ../music.h ../radio.h stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

music.o: ../music.c ../music.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

radio.o: ../radio.c ../radio.h ../music.h stack.h queue.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

queue.o: queue.c queue.h ../types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

stack.o: stack.c stack.h ../types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clear:
	rm -rf *.o

clean:
	rm -rf *.o $(EJS) $(STACK_EJS)

run:
	@echo ">>>>>>Running p2_e1"
	./p2_e1 radio.txt
	@echo ">>>>>>Running p2_e2a"
	./p2_e2a playlist1.txt playlist2.txt
	@echo ">>>>>>Running p2_e2b"
	./p2_e2b playlistA.txt playlistB.txt
	@echo ">>>>>>Running p2_e3"
	./p2_e3 radio_para_dfs.txt 1 9

run_stack:
	@echo ">>>>>>Running p2_e1s"
	./p2_e1s radio.txt
	@echo ">>>>>>Running p2_e2as"
	./p2_e2as playlist1.txt playlist2.txt
	@echo ">>>>>>Running p2_e2bs"
	./p2_e2bs playlistA.txt playlistB.txt
	@echo ">>>>>>Running p2_e3s"
	./p2_e3s radio_para_dfs.txt 1 9
