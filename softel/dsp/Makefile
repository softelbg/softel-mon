#
# (C) Copyright Softel (softel.bg) 2012. All rights reserved.
#
#
# Description: This is the Makefile for the Softel DSP Library
#
.SILENT:

include ../makefile.pre

SUBDIRS = 	core

all: 
	for i in $(SUBDIRS); do \
        	cd "$$i" && $(MAKE) $@; \
    	done	

clean:
	for i in $(SUBDIRS); do \
                cd "$$i" && $(MAKE) $@; \
        done


recursive: true

