#
# (C) Copyright softel.bg 2012. All rights reserved.
#
#
# Description: This is the Makefile for the OSS Alert CM Monitoring
#
.SILENT:

include ../../makefile.pre

EXECUTABLE = CmtsCmAlert
l = ../$(LIB)/lib$(EXECUTABLE).a
d = ../$(BIN)/$(EXECUTABLE)

CFLAGS = -Wall
CFLAGS += $(INCPATH)

LDFLAGS += -L/usr/local/lib/mysql/
LDFLAGS += -L/usr/lib64/mysql/
LDFLAGS += $(LIBPATH)
LDFLAGS += -L../../../../lib/

LFLAGS += -l$(LIBSFOSS)
LFLAGS += -l$(LIBSFDB)
LFLAGS += -l$(LIBSFCORE)
LFLAGS += -lmysqlclient


obj =	$(l)(CCmtsCmAlertList.o) \
		$(l)(CSchedulerCmAlert.o) \
		$(l)(CSqlCmAlert.o) \
		$(l)(CmtsCmAlert.o)
		
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

