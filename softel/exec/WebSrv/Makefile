#
# (C) Copyright Softel 2010. All rights reserved.
#
#
# Description: This is the Makefile for the Softel Web Server
#
.SILENT:

include ../../makefile.pre

EXECUTABLE = WebSrv
l = ../$(LIB)/lib$(EXECUTABLE).a
d = ../$(BIN)/$(EXECUTABLE)

CFLAGS = -Wall
CFLAGS += $(INCPATH)
CFLAGS += -std=c++0x

LDFLAGS += -L/usr/local/lib/mysql/
LDFLAGS += -L/usr/lib64/mysql/
LDFLAGS += $(LIBPATH)
LDFLAGS += -L../../../../lib/

LFLAGS += -l$(LIBSFWEB)
LFLAGS += -l$(LIBSFCORE)
LFLAGS += -lmysqlclient_r


obj =		$(l)($(EXECUTABLE).o) \
		$(l)(DemonizeWebSrv.o)
		
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
	
install: $(EXECUTABLE) 
	@ echo --- install
	cp $(d) /usr/local/sbin
	cp $(EXECUTABLE).ini /etc/

