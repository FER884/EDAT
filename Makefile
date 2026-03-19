########################################################
CC = gcc
CFLAGS = -g -Wall
IFLAGS = -I./ -I./practica2 -I./practica3
LDFLAGS = -L./practica2
LDLIBS = -lstack

P1_DIR = practica1
P2_DIR = practica2
P3_DIR = practica3

P1_EJS = p1_e1 p1_e2 p1_e3
P2_EJS = p2_e1 p2_e2a p2_e2b p2_e3
P2_STACK_EJS = p2_e1s p2_e2as p2_e2bs p2_e3s
P3_EJS = p3_e1 p3_e2 p3_e3
P3_QUEUE_LIST_EJS = p3_e1s p3_e2s
EJS = $(P1_EJS) $(P2_EJS) $(P2_STACK_EJS) $(P3_EJS) $(P3_QUEUE_LIST_EJS)

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
########################################################

all: $(EJS) clear

p1_e1: $(P1_E1_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P1_E1_OBJS)

p1_e2: $(P1_E2_OBJS) $(P2_DIR)/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P1_E2_OBJS) $(LDFLAGS) $(LDLIBS)

p1_e3: $(P1_E3_OBJS) $(P2_DIR)/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P1_E3_OBJS) $(LDFLAGS) $(LDLIBS)

p2_e1: $(P2_E1_OBJS) $(P2_DIR)/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P2_E1_OBJS) $(LDFLAGS) $(LDLIBS)

p2_e2a: $(P2_E2A_OBJS) $(P2_DIR)/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P2_E2A_OBJS) $(LDFLAGS) $(LDLIBS)

p2_e2b: $(P2_E2B_OBJS) $(P2_DIR)/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P2_E2B_OBJS) $(LDFLAGS) $(LDLIBS)

p2_e3: $(P2_E3_OBJS) $(P2_DIR)/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P2_E3_OBJS) $(LDFLAGS) $(LDLIBS)

p2_e1s: $(P2_E1_OBJS) $(P2_STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E1_OBJS) $(P2_STACK_OBJS)

p2_e2as: $(P2_E2A_OBJS) $(P2_STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E2A_OBJS) $(P2_STACK_OBJS)

p2_e2bs: $(P2_E2B_OBJS) $(P2_STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E2B_OBJS) $(P2_STACK_OBJS)

p2_e3s: $(P2_E3_OBJS) $(P2_STACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P2_E3_OBJS) $(P2_STACK_OBJS)

p3_e1: $(P3_E1_OBJS) $(P2_DIR)/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P3_E1_OBJS) $(LDFLAGS) $(LDLIBS)

p3_e2: $(P3_E2_OBJS) $(P2_DIR)/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P3_E2_OBJS) $(LDFLAGS) $(LDLIBS)

p3_e3: $(P3_E3_OBJS) $(P2_DIR)/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P3_E3_OBJS) $(LDFLAGS) $(LDLIBS)

p3_e1s: $(P3_QUEUE_LIST_E1_OBJS) $(P2_DIR)/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P3_QUEUE_LIST_E1_OBJS) $(LDFLAGS) $(LDLIBS)

p3_e2s: $(P3_QUEUE_LIST_E2_OBJS) $(P2_DIR)/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P3_QUEUE_LIST_E2_OBJS) $(LDFLAGS) $(LDLIBS)

p1_e1.o: $(P1_DIR)/p1_e1.c music.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p1_e2.o: $(P1_DIR)/p1_e2.c music.h radio.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p1_e3.o: $(P1_DIR)/p1_e3.c music.h radio.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p2_e1.o: $(P2_DIR)/p2_e1.c music.h radio.h $(P2_DIR)/stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p2_e2a.o: $(P2_DIR)/p2_e2a.c music.h radio.h $(P2_DIR)/stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p2_e2b.o: $(P2_DIR)/p2_e2b.c music.h radio.h $(P2_DIR)/stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p2_e3.o: $(P2_DIR)/p2_e3.c music.h radio.h $(P2_DIR)/stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p3_e1.o: $(P3_DIR)/p3_e1.c music.h $(P3_DIR)/queue.h radio.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p3_e2.o: $(P3_DIR)/p3_e2.c music.h $(P3_DIR)/queue.h radio.h $(P2_DIR)/stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p3_e3.o: $(P3_DIR)/p3_e3.c $(P3_DIR)/list.h music.h radio.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

music.o: music.c music.h
	$(CC) $(CFLAGS) -c $< -o $@

radio.o: radio.c $(P3_DIR)/queue.h radio.h music.h $(P2_DIR)/stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

stack.o: $(P2_DIR)/stack.c $(P2_DIR)/stack.h types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

queue.o: $(P3_DIR)/queue.c $(P3_DIR)/queue.h types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

list.o: $(P3_DIR)/list.c $(P3_DIR)/list.h types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

queueList.o: $(P3_DIR)/queueList.c $(P3_DIR)/queue.h $(P3_DIR)/list.h types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clear:
	rm -rf *.o

clean:
	rm -rf *.o $(EJS)

run:
	@echo ">>>>>>Running p1_e1"
	./p1_e1
	@echo ">>>>>>Running p1_e2"
	./p1_e2
	@echo ">>>>>>Running p1_e3"
	./p1_e3 $(P1_DIR)/radio1.txt
	@echo ">>>>>>Running p2_e1"
	./p2_e1 $(P2_DIR)/radio.txt
	@echo ">>>>>>Running p2_e2a"
	./p2_e2a $(P2_DIR)/playlist1.txt $(P2_DIR)/playlist2.txt
	@echo ">>>>>>Running p2_e2b"
	./p2_e2b $(P2_DIR)/playlistA.txt $(P2_DIR)/playlistB.txt
	@echo ">>>>>>Running p2_e3"
	./p2_e3 $(P2_DIR)/radio_para_dfs.txt 1 9
	@echo ">>>>>>Running p3_e1"
	./p3_e1 $(P2_DIR)/radio.txt
	@echo ">>>>>>Running p3_e2"
	./p3_e2 $(P3_DIR)/radio_bfs.txt 1 2
	@echo ">>>>>>Running p3_e3"
	./p3_e3 $(P3_DIR)/radio_bfs.txt

runv:
	@echo ">>>>>>Running p2_e1 with valgrind"
	valgrind --leak-check=full ./p2_e1 $(P2_DIR)/radio.txt

run_stack:
	@echo ">>>>>>Running p2_e1s"
	./p2_e1s $(P2_DIR)/radio.txt
	@echo ">>>>>>Running p2_e2as"
	./p2_e2as $(P2_DIR)/playlist1.txt $(P2_DIR)/playlist2.txt
	@echo ">>>>>>Running p2_e2bs"
	./p2_e2bs $(P2_DIR)/playlistA.txt $(P2_DIR)/playlistB.txt
	@echo ">>>>>>Running p2_e3s"
	./p2_e3s $(P2_DIR)/radio_para_dfs.txt 1 9

run_queue_list:
	@echo ">>>>>>Running p3_e1s"
	./p3_e1s $(P2_DIR)/radio.txt
	@echo ">>>>>>Running p3_e2s"
	./p3_e2s $(P3_DIR)/radio_bfs.txt 1 2
