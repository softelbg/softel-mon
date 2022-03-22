#
# (C) Copyright Softel 2010. All rights reserved.
#
#
# Description: This is the Makefile for the Net Servers
#
.SILENT:

include ../../../makefile.pre

l = ../../$(LIB)/lib$(LIBSFCORE).a
d = ../../$(BIN)/lib$(LIBSFCORE).so

CFLAGS += $(INCPATH)
CFLAGS += -std=c++0x

LDFLAGS += $(LIBPATH)

LFLAGS += -lboost_system -lboost_thread-mt

obj =   $(l)(ClientBase.o) \
	$(l)(ClientHttpBase.o) \
	$(l)(ClientHttpAsync.o) \
	$(l)(ClientHttpSync.o) \
	$(l)(MimeTypes.o) \
	$(l)(HttpRequestParser.o) \
	$(l)(ProtoUtils.o) \
	$(l)(ReplyBase.o) \
	$(l)(ReplyHttpBase.o) \
	$(l)(RequestBase.o) \
	$(l)(RequestHttpBase.o) \
	$(l)(RequestHandlerBase.o) \
	$(l)(RequestHandlerHttp.o) \
	$(l)(RequestHandlerHttpCmdBase.o) \
	$(l)(ServerTcpBase.o) \
	$(l)(ServerHttpBase.o) \
	$(l)(ServerHttpCmdBase.o) \
	$(l)(ServerHttpStandard.o) \
	$(l)(ConnectionTcpBase.o) \
	$(l)(ConnectionHttpBase.o) \
	$(l)(IReplyInterface.o) \
	$(l)(IRequestHandlerInterface.o) \
	$(l)(WorkItemServerTcp.o) \
	$(l)(BFolderCache.o) \
	$(l)(Host.o)

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

