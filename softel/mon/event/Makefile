#
# (C) Copyright Softel 2011. All rights reserved.
#
#
# Description: This is the Makefile for the Softel Monitoring Event Library 
#
.SILENT:

include ../../makefile.pre

l = ../$(LIB)/lib$(LIBSFMON).a
d = ../$(BIN)/lib$(LIBSFMON).so

CFLAGS += $(INCPATH)
CFLAGS += -std=c++0x

LDFLAGS += $(LIBPATH)

obj =   $(l)(EventInterface.o) \
		$(l)(RQueueEvent.o) \
		$(l)(RQueueEventSingleton.o) \
		$(l)(EventHandlerBase.o) \
		$(l)(EventHandlerTest.o) \
		$(l)(EventHandlerManager.o) \
		$(l)(EventManager.o) \
		$(l)(EventSimHealthChanged.o) \
		$(l)(EventBase.o) \
		$(l)(EventTest.o) \
		$(l)(EventQueueProvider.o) \
		$(l)(WorkItemEventHandler.o) \
		$(l)(WorkItemEventReceiver.o) \
		$(l)(WorkItemEventProvider.o)

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

