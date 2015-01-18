TARGET	= ATS_HK.dll
OBJS	= atsplugin.h Ats.h Ats.cpp hk.hpp

all:	$(TARGET)

test:	$(TARGET)
	./$(TARGET) -test

$(TARGET): $(OBJS)
	gcc -Wall -c -DATS_EXPORTS Ats.cpp -lm -o Ats.o
	gcc -shared -o $(TARGET) Ats.def Ats.o

gnm:
	nm $(TARGET) -g

cl:
	del Ats.o

call:
	del $(TARGET) Ats.o