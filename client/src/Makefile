SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=client
CC:=gcc
CFLAGS:=-Wall -I ../include -I /usr/include/mysql/ -lmysqlclient -lcrypt  
$(ELF):$(OBJS)
	gcc $^ -o $@ $(CFLAGS) $(MYSQL)
.PYONY:clean
clean:
	rm -rf $(OBJS) $(ELF)
