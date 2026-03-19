########################################################
CC = gcc
CFLAGS = -g -Wall
IFLAGS = -I.. -I../practica2 -I.
LDFLAGS = -L../practica2
LDLIBS = -lstack

EJS = p3_e1 p3_e2 p3_e3
QUEUE_LIST_EJS = p3_e1s p3_e2s

RADIO_COMMON_OBJS = radio.o music.o queue.o
RADIO_COMMON_LIST_OBJS = radio.o music.o queueList.o list.o
P3_E1_OBJS = p3_e1.o $(RADIO_COMMON_OBJS)
P3_E2_OBJS = p3_e2.o $(RADIO_COMMON_OBJS)
P3_E3_OBJS = p3_e3.o $(RADIO_COMMON_OBJS) list.o
P3_QUEUE_LIST_E1_OBJS = p3_e1.o $(RADIO_COMMON_LIST_OBJS)
P3_QUEUE_LIST_E2_OBJS = p3_e2.o $(RADIO_COMMON_LIST_OBJS)
########################################################

all: $(EJS) $(QUEUE_LIST_EJS) clear

p3_e1: $(P3_E1_OBJS) ../practica2/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P3_E1_OBJS) $(LDFLAGS) $(LDLIBS)

p3_e2: $(P3_E2_OBJS) ../practica2/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P3_E2_OBJS) $(LDFLAGS) $(LDLIBS)

p3_e3: $(P3_E3_OBJS) ../practica2/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P3_E3_OBJS) $(LDFLAGS) $(LDLIBS)

p3_e1s: $(P3_QUEUE_LIST_E1_OBJS) ../practica2/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P3_QUEUE_LIST_E1_OBJS) $(LDFLAGS) $(LDLIBS)

p3_e2s: $(P3_QUEUE_LIST_E2_OBJS) ../practica2/libstack.a
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(P3_QUEUE_LIST_E2_OBJS) $(LDFLAGS) $(LDLIBS)

p3_e1.o: p3_e1.c ../music.h queue.h ../radio.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p3_e2.o: p3_e2.c ../music.h queue.h ../radio.h ../practica2/stack.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p3_e3.o: p3_e3.c list.h ../music.h ../radio.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

music.o: ../music.c ../music.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

radio.o: ../radio.c ../radio.h ../music.h ../practica2/stack.h queue.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

queue.o: queue.c queue.h ../types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

list.o: list.c list.h ../types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

queueList.o: queueList.c queue.h list.h ../types.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clear:
	rm -rf *.o

clean:
	rm -rf *.o $(EJS) $(QUEUE_LIST_EJS)

run:
	@echo ">>>>>>Running p3_e1"
	./p3_e1 ../practica2/radio.txt
	@echo ">>>>>>Running p3_e2"
	./p3_e2 radio_bfs.txt 1 2
	@echo ">>>>>>Running p3_e3"
	./p3_e3 radio_bfs.txt

run_queue_list:
	@echo ">>>>>>Running p3_e1s"
	./p3_e1s ../practica2/radio.txt
	@echo ">>>>>>Running p3_e2s"
	./p3_e2s radio_bfs.txt 1 2
