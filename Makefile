########################################################
CC = gcc
AR = ar
CFLAGS = -g -Wall
IFLAGS = -I.

P1_EJS = p1_e1 p1_e2 p1_e3
P2_EJS = p2_e1 p2_e2a p2_e2b p2_e3
P2_STACK_EJS = p2_e1s p2_e2as p2_e2bs p2_e3s
P3_EJS = p3_e1 p3_e2 p3_e3
P3_QUEUE_LIST_EJS = p3_e1s p3_e2s
P4_EJS = p4_e1
EJS = $(P1_EJS) $(P2_EJS) $(P2_STACK_EJS) $(P3_EJS) $(P3_QUEUE_LIST_EJS) $(P4_EJS)

STACK_LIB = libstack.a

P1_E1_OBJS = p1_e1.o music.o
RADIO_COMMON_OBJS = radio.o music.o queue.o
RADIO_COMMON_LIST_OBJS = radio.o music.o queueList.o list.o
P1_E2_OBJS = p1_e2.o $(RADIO_COMMON_OBJS)
P1_E3_OBJS = p1_e3.o $(RADIO_COMMON_OBJS)
P2_E1_OBJS = p2_e1.o $(RADIO_COMMON_OBJS)
P2_E2A_OBJS = p2_e2a.o $(RADIO_COMMON_OBJS)
P2_E2B_OBJS = p2_e2b.o $(RADIO_COMMON_OBJS)
P2_E3_OBJS = p2_e3.o $(RADIO_COMMON_OBJS)
P2_STACK_OBJS = stack.o
P3_E1_OBJS = p3_e1.o $(RADIO_COMMON_OBJS)
P3_E2_OBJS = p3_e2.o $(RADIO_COMMON_OBJS)
P3_E3_OBJS = p3_e3.o $(RADIO_COMMON_OBJS) list.o
P3_QUEUE_LIST_E1_OBJS = p3_e1.o $(RADIO_COMMON_LIST_OBJS)
P3_QUEUE_LIST_E2_OBJS = p3_e2.o $(RADIO_COMMON_LIST_OBJS)
P4_E1_OBJS = p4_e1.o bstree.o $(RADIO_COMMON_OBJS)
########################################################

all: $(EJS) clear

p1_e1: $(P1_E1_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P1_E1_OBJS)

p1_e2: $(P1_E2_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P1_E2_OBJS) $(STACK_LIB)

p1_e3: $(P1_E3_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P1_E3_OBJS) $(STACK_LIB)

p2_e1: $(P2_E1_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P2_E1_OBJS) $(STACK_LIB)

p2_e2a: $(P2_E2A_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P2_E2A_OBJS) $(STACK_LIB)

p2_e2b: $(P2_E2B_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P2_E2B_OBJS) $(STACK_LIB)

p2_e3: $(P2_E3_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P2_E3_OBJS) $(STACK_LIB)

p2_e1s: $(P2_E1_OBJS) $(P2_STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E1_OBJS) $(P2_STACK_OBJS)

p2_e2as: $(P2_E2A_OBJS) $(P2_STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E2A_OBJS) $(P2_STACK_OBJS)

p2_e2bs: $(P2_E2B_OBJS) $(P2_STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E2B_OBJS) $(P2_STACK_OBJS)

p2_e3s: $(P2_E3_OBJS) $(P2_STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E3_OBJS) $(P2_STACK_OBJS)

p3_e1: $(P3_E1_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P3_E1_OBJS) $(STACK_LIB)

p3_e2: $(P3_E2_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P3_E2_OBJS) $(STACK_LIB)

p3_e3: $(P3_E3_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P3_E3_OBJS) $(STACK_LIB)

p3_e1s: $(P3_QUEUE_LIST_E1_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P3_QUEUE_LIST_E1_OBJS) $(STACK_LIB)

p3_e2s: $(P3_QUEUE_LIST_E2_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P3_QUEUE_LIST_E2_OBJS) $(STACK_LIB)

p4_e1: $(P4_E1_OBJS) $(STACK_LIB)
	$(CC) $(CFLAGS) -o $@ $(P4_E1_OBJS) $(STACK_LIB)

p1_e1.o: p1_e1.c music.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p1_e2.o: p1_e2.c music.h radio.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p1_e3.o: p1_e3.c music.h radio.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p2_e1.o: p2_e1.c music.h radio.h stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p2_e2a.o: p2_e2a.c music.h radio.h stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p2_e2b.o: p2_e2b.c music.h radio.h stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p2_e3.o: p2_e3.c music.h radio.h stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p3_e1.o: p3_e1.c music.h queue.h radio.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p3_e2.o: p3_e2.c music.h queue.h radio.h stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p3_e3.o: p3_e3.c list.h music.h radio.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p4_e1.o: p4_e1.c bstree.h radio.h types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

music.o: music.c music.h
	$(CC) $(CFLAGS) -c $< -o $@

radio.o: radio.c queue.h radio.h music.h stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

stack.o: stack.c stack.h types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

queue.o: queue.c queue.h types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

list.o: list.c list.h types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

bstree.o: bstree.c bstree.h list.h types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

queueList.o: queueList.c queue.h list.h types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(STACK_LIB): stack.o
	$(AR) rcs $@ $<

clear:
	rm -rf *.o

clean:
	rm -rf *.o $(EJS) $(STACK_LIB)

run:
	@echo ">>>>>>Running p1_e1"
	./p1_e1
	@echo ">>>>>>Running p1_e2"
	./p1_e2
	@echo ">>>>>>Running p1_e3"
	./p1_e3 radio1.txt
	@echo ">>>>>>Running p2_e1"
	./p2_e1 radio.txt
	@echo ">>>>>>Running p2_e2a"
	./p2_e2a playlist1.txt playlist2.txt
	@echo ">>>>>>Running p2_e2b"
	./p2_e2b playlistA.txt playlistB.txt
	@echo ">>>>>>Running p2_e3"
	./p2_e3 radio_para_dfs.txt 1 9
	@echo ">>>>>>Running p3_e1"
	./p3_e1 radio.txt
	@echo ">>>>>>Running p3_e2"
	./p3_e2 radio_bfs.txt 1 2
	@echo ">>>>>>Running p3_e3"
	./p3_e3 radio_bfs.txt

runv:
	@echo ">>>>>>Running p2_e1 with valgrind"
	valgrind --leak-check=full ./p2_e1 radio.txt

run_stack:
	@echo ">>>>>>Running p2_e1s"
	./p2_e1s radio.txt
	@echo ">>>>>>Running p2_e2as"
	./p2_e2as playlist1.txt playlist2.txt
	@echo ">>>>>>Running p2_e2bs"
	./p2_e2bs playlistA.txt playlistB.txt
	@echo ">>>>>>Running p2_e3s"
	./p2_e3s radio_para_dfs.txt 1 9

run_queue_list:
	@echo ">>>>>>Running p3_e1s"
	./p3_e1s radio.txt
	@echo ">>>>>>Running p3_e2s"
	./p3_e2s radio_bfs.txt 1 2
