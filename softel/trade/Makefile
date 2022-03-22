#
# (C) Copyright Softel 2011. All rights reserved.
#
#
# Description: This is the Makefile for the Softel Trade 
#
.SILENT:

include ../makefile.pre

SUBDIRS = 	idl \
			../machine

l = $(LIB)/lib$(LIBSFTRADE).a
d = $(BIN)/lib$(LIBSFTRADE).so

CFLAGS += $(INCPATH)

LDFLAGS += $(LIBPATH)

obj =   $(l)(OrderBook.o) \
		$(l)(CommonHelpers.o)  \
		$(l)(TBSONProtocol.o)  \
		$(l)(TJSONProtocol.o)  \
		$(l)(TradeMachine.o)

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
		for i in $(SUBDIRS); do \
        	cd "$$i" && $(MAKE) $@; \
    	done


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
	for i in $(SUBDIRS); do \
       	cd "$$i" && $(MAKE) $@; \
    done

recursive: true
