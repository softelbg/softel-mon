#
# (C) Copyright softel.bg 2012. All rights reserved.
#
#
# Description: This is the Makefile for the Softel Core Library
#
.SILENT:

include ../makefile.pre

SUBDIRS = 	error \
		../intf \
		../net \
		../system \
		../http \
		../lock \
		../pool \
		../util
		
l = $(LIB)/lib$(LIBSFCORE).a
d = $(BIN)/lib$(LIBSFCORE).so

CFLAGS += $(INCPATH)

LDFLAGS += $(LIBPATH)

obj =   $(l)(CObject.o) \
		$(l)(BPtr.o) \
		$(l)(BaseSingleton.o) \
		$(l)(ManagerBase.o) \
		$(l)(Helper.o) \
		$(l)(Ptr.o)
		
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

