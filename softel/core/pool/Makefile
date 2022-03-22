#
# (C) Copyright softel.bg 2012. All rights reserved.
#
#
# Description: This is the Makefile for the Softel Pools - thread pools, connection pools...
#
.SILENT:

include ../../makefile.pre

l = ../$(LIB)/lib$(LIBSFCORE).a
d = ../$(BIN)/lib$(LIBSFCORE).so

CFLAGS += $(INCPATH)
CFLAGS += -std=c++0x

LDFLAGS += $(LIBPATH)

LFLAGS += -lboost_system -lboost_thread-mt


obj =   $(l)(WorkItemBase.o) \
		$(l)(WorkItemMokup.o) \
		$(l)(RQueue.o) \
		$(l)(RContainer.o) \
		$(l)(RFileIni.o) \
		$(l)(DbPool.o) \
		$(l)(BConnectionBase.o) \
		$(l)(WorkItemSqlPost.o) \
		$(l)(WorkItemSchedule.o) \
		$(l)(ScheduledItem.o) \
		$(l)(ThreadPoolAsio.o) \
		$(l)(ThreadPoolAsioSingleton.o)

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


all:   $(obj)


link:
	@ echo "--- create $(d) ---"
	@ echo --- $(LD) *.o $(l)  \
	-L$(LIB) \
	$(LDFLAGS) \
	$(LFLAGS) \
	-o $(d)
	$(LD) *.o $(l)  \
	-L$(LIB) \
	$(LDFLAGS) \
	$(LFLAGS) \
	-o $(d)


clean:
	@ echo --- cleanup
	-rm *.o
	-rm $(d)
	-rm $(l)

