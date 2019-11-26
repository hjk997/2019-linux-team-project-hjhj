#######################################################
# # 2019 3-2 system programming team project
# # hjsh
# # author@ 20173116 shj, 20173237 khj
# # date@ 2019.11.26
#######################################################

CC			= gcc
CFLAGS		= -g -O2
TARGET		= hjsh
OBJECTS		= execute.o parser.o redirect.o symbol.o util.o command.o
MAIN_SOURCE	= hjsh.c

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(MAIN_SOURCE)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
