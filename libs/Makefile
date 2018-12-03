
#Name of this library:
LIBNAME=projectlib
LIBRARY=$(LIBNAME).lib


#Which files need to be used for this library:

SOURCES= HT1621_NoLib.c
SOURCES+= tim4_tbase.c
SOURCES+= tim1.c
SOURCES+= tim2.c
SOURCES+= gpio.c
SOURCES+= clock.c
SOURCES+= adc1_2.c
SOURCES+= uart1.c

RELS=$(SOURCES:.c=.rel)

SDCC=sdcc
SDLD=sdld
SDAR=sdar

PROCESSOR=stm8s103f3
DEBUGPROBE=stlinkv2

# Define tool path. Adjust to your own setup:
LIBROOT=/home/onat/elektronik/ARM/stm8/STM8S_StdPeriph_Lib/Libraries/STM8S_StdPeriph_Driver

INCLUDES=$(LIBROOT)/inc/
SOURCEDIR=$(LIBROOT)/src/

CFLAGSLIB= -I$(INCLUDES)
CFLAGSLIB+= -I.
CFLAGSLIB+= -I..
CFLAGSLIB+= -c

SDARFLAGS= -rc

#These macros are defined here to keep the header files
# (which want to see a specific compiler and processor) happy.
MACROS= -D__CSMC__ -DSTM8S103
PROCTYPE= -lstm8 -mstm8

vpath %.c
vpath %.c $(SOURCEDIR)


#.phony all # all is not a file name.

all: $(LIBRARY)

$(LIBRARY) : $(SOURCES) # $(HEADERS)


%.lib : $(RELS)  
	$(SDAR) $(SDARFLAGS) $@ $(RELS)

%.rel: %.c
	$(SDCC) $(MACROS) $(PROCTYPE) $(CFLAGSLIB) $(LDFLAGS) $<

clean:
	rm -f $(RELS) $(RELS:.rel=.cdb) $(RELS:.rel=.map) $(RELS:.rel=.lst) $(RELS:.rel=.asm) $(RELS:.rel=.lk) $(RELS:.rel=.rst) $(RELS:.rel=.sym)


