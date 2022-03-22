#
# (C) Copyright Softel 2010. All rights reserved.
#
#
# Description: This is the Makefile for the Net Web Servers
#
.SILENT:

include ../../../makefile.pre

l = ../../$(LIB)/lib$(LIBSFWEB).a
d = ../../$(BIN)/lib$(LIBSFWEB).so

CFLAGS += $(INCPATH)
CFLAGS += -std=c++0x

LDFLAGS += $(LIBPATH)

LFLAGS += -lboost_system -lboost_thread-mt

obj =		$(l)(FolderCacheHtml.o) \
		$(l)(FileHtml.o) \
		$(l)(BPluginBase.o) \
		$(l)(PluginInfo.o) \
		$(l)(PluginContainer.o) \
		$(l)(PluginContainerWebSrvSingleton.o) \
		$(l)(RequestHandlerHttp.o) \
		$(l)(ServerHttpWeb.o)

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

