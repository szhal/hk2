TARGET	= hk2.dll
OBJS	= atsplugin.h Ats.h Ats.cpp hk.hpp

all:	$(TARGET)

test:	$(TARGET)
	./$(TARGET) -test

$(TARGET): $(OBJS)
	gcc -Wall -o Ats.o -c Ats.cpp -DATS_EXPORTS -lm
	gcc -shared -o $(TARGET) Ats.def Ats.o

gnm:
	nm $(TARGET) -g

cl:
	del Ats.o

call:
	del $(TARGET) Ats.o