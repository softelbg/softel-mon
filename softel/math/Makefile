#
# (C) Copyright softel.bg 2012. All rights reserved.
#
#
# Description: This is the Makefile for the Softel Math Tools
#
.SILENT:

include ../makefile.pre

EXECUTABLE = math
l = $(LIB)/lib$(LIBSFCORE).a
d = $(BIN)/$(EXECUTABLE)

CFLAGS = -Wall
CFLAGS += $(INCPATH)
CFLAGS += -std=c++0x

LDFLAGS += $(LIBPATH)
LDFLAGS += -L../../../lib/

LFLAGS += -l$(LIBSFCORE)


obj =	$(l)(Matrix.o) \
		$(l)(BigInt.o) \
		$(l)(math.o)
		
.cc.a .cc:
	@ echo --- c rule for $* --- $(CC) $(CFLAGS) -c $*.$(CCTYPE) 
	@ echo --- $(AR) $(ADD) $(l) $*.o
	if ( $(CC) $(CFLAGS) -c $*.$(CCTYPE) ) ; \
	then echo "compilation successful"; \
	$(AR) $(ADD) $(l) $*.o; \
	else echo "ERROR occurred in $*" ; \
	echo "delete older $*.o from archive" ; \
	$(AR) $(DELETE) $(l) $*.o ; \
	fi


all:   $(obj) $(EXECUTABLE) 

$(EXECUTABLE): $(obj) 
	@ echo --- Build $(EXECUTABLE) 
	@ echo --- "$(CC) $(CFLAGS) $(INCPATH) *.o $(LDFLAGS) $(LFLAGS) -o $(d)"
	$(CC) $(CFLAGS) $(INCPATH) *.o $(LDFLAGS) $(LFLAGS) -o $(d)

link:
	

clean:
	@ echo --- cleanup
	-rm *.o
	-rm $(d)
	-rm $(l)
	

