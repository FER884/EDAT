########################################################
CC = gcc
CFLAGS = -g -Wall
IFLAGS = -I.. -I.

EJS = p1_e1 p1_e2 p1_e3

P1_E1_OBJS = p1_e1.o music.o
RADIO_COMMON_OBJS = radio.o music.o queue.o
STACK_OBJS = stack.o
P1_E2_OBJS = p1_e2.o $(RADIO_COMMON_OBJS) $(STACK_OBJS)
P1_E3_OBJS = p1_e3.o $(RADIO_COMMON_OBJS) $(STACK_OBJS)
########################################################

all: $(EJS) clear

p1_e1: $(P1_E1_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P1_E1_OBJS)

p1_e2: $(P1_E2_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P1_E2_OBJS)

p1_e3: $(P1_E3_OBJS)
	$(CC) $(CFLAGS) -o $@ $(P1_E3_OBJS)

p1_e1.o: p1_e1.c ../music.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p1_e2.o: p1_e2.c ../music.h ../radio.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

p1_e3.o: p1_e3.c ../music.h ../radio.h
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
	rm -rf *.o $(EJS)

run:
	@echo ">>>>>>Running p1_e1"
	./p1_e1
	@echo ">>>>>>Running p1_e2"
	./p1_e2
	@echo ">>>>>>Running p1_e3"
	./p1_e3 radio1.txt
