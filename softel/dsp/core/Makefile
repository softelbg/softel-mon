#
# (C) Copyright Softel 2011. All rights reserved.
#
#
# Description: This is the Makefile for the Softel Core DSP Library 
#
.SILENT:

include ../../makefile.pre

l = ../$(LIB)/lib$(LIBSFDSP).a
d = ../$(BIN)/lib$(LIBSFDSP).so

CFLAGS += $(INCPATH)

LDFLAGS += $(LIBPATH)

obj =   $(l)(Signal.o) \
		$(l)(FFT.o)

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

