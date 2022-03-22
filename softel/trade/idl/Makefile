#
# (C) Copyright Softel 2011. All rights reserved.
#
#
# Description: This is the Makefile for the Softel Trade 
#
.SILENT:

include ../../makefile.pre

l = ../$(LIB)/lib$(LIBSFTRADE).a
d = ../$(BIN)/lib$(LIBSFTRADE).so

CFLAGS += $(INCPATH)

LDFLAGS += $(LIBPATH)

obj =   $(l)(messages.pb.o) \
		$(l)(gen-cpp/BaseMachine.o) \
		$(l)(gen-cpp/trade_constants.o) \
		$(l)(gen-cpp/trade_types.o) \

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

.cpp.a .cpp:
	@ echo --- c rule for $* --- $(CC) $(CFLAGS) -c $*.cpp -o $*.o
	@ echo --- $(AR) $(ADD) $(l) $*.o
	if ( $(CC) $(CFLAGS) -c $*.cpp -o $*.o) ; \
	then echo "compilation successful"; \
	$(AR) $(ADD) $(l) $*.o; \
	else echo "ERROR occurred in $*" ; \
	echo "delete older $*.o from archive" ; \
	$(AR) $(DELETE) $(l) $*.o ; \
	fi

all:	proto $(obj)

proto:
	@ protoc --cpp_out=./ messages.proto
	@ rm -rf gen-cpp; thrift -r --gen cpp trade.thrift
		
	
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
	-rm gen-cpp/*.o
	-rm $(d)
	-rm $(l)
