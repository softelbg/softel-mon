.SILENT:

include ../../makefile.pre

EXECUTABLE = zmqsub
l = ../$(LIB)/lib$(EXECUTABLE).a
d = ../$(BIN)/$(EXECUTABLE)

CFLAGS += $(INCPATH)

LDFLAGS += $(LIBPATH) -L../../../../lib/
LFLAGS += -l$(LIBSFTRADE) -l$(LIBSFCORE)


obj =		$(l)(ModulePublish.o) \
			$(l)(ModuleMsgMD.o) \
			$(l)(ModuleMsgRate.o) \
			$(l)(ModuleMsgRaw.o) \
			$(l)(ModuleTrade.o) \
			$(l)(ZMQSub.o)
		
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

all:   $(submodules) $(obj) $(EXECUTABLE) 


$(EXECUTABLE): $(obj) 
	@ echo --- Build $(EXECUTABLE) 
	@ echo --- "$(CC) $(CFLAGS) $(INCPATH) *.o $(LDFLAGS) $(LFLAGS) -o $(d)"
	$(CC) $(CFLAGS) $(INCPATH) *.o $(LDFLAGS) $(LFLAGS) -o $(d)

submodules:
	@ echo build modules
	
link:


clean:
	@ echo --- cleanup
	-rm *.o
	-rm $(d)
	-rm $(l)
	for i in $(SUBDIRS); do \
                cd "$$i" && $(MAKE) $@; \
        done	

install: $(EXECUTABLE) 
	@ echo --- install
	cp $(d) $(BIN) 
	cp $(d) /usr/local/sbin

recursive: true

