#
# (C) Copyright Softel 2011. All rights reserved.
#
#
# Description: This is the Makefile for the OSS HTTP Tools - http header, response, request... 
#
.SILENT:

include ../../makefile.pre

l = ../$(LIB)/lib$(LIBSFCORE).a
d = ../$(BIN)/lib$(LIBSFCORE).so

CFLAGS += $(INCPATH)

LDFLAGS += $(LIBPATH)

obj =   $(l)(CHttpHeader.o) \
		$(l)(CHttpResponse.o) \
		$(l)(CHttpRequest.o)

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

