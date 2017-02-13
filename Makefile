CC = gcc
OBJS = main.o
TARGET = LogColletor
LOGFILE = logFile_*
MAIN = main.c

all : $(TARGET)

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS)

clean :
	rm -f $(OBJS) $(TARGET)
	rm -f $(LOGFILE)
