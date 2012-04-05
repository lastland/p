OBJS 		= minips.o
CC 		= gcc
LINK 		= gcc
TARGET 		= p
INSTDIR		= /usr/local/bin/

$(TARGET) : $(OBJS)
	$(LINK) -o $(TARGET) $(OBJS)

.PHONY : clean install

clean:
	-rm $(TARGET) $(OBJS)

install:
	cp $(TARGET) $(INSTDIR) 
