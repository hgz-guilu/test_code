#!/bin/bash 

MAKE_COMPILE=

DIR_LIB     = 

DECODER_DFLAG = 

#set compile commands
CC  = $(MAKE_COMPILE)gcc
CXX = $(MAKE_COMPILE)g++
LD  = $(MAKE_COMPILE)ld
AR  = $(MAKE_COMPILE)ar
#RANLIB = $(MAKE_COMPILE)ranlib
#STRIP = $(MAKE_COMPILE)strip

#add predef sysmbols


#add predef sysmbols
DECODER_DFLAG += 

#add complie choice
DECODER_CFLAG = -g -Wall -fsigned-char -Wno-unused  -O0


LIB_SYSLIB    =  -lstdc++ -lrt  -ldl -lm -pthread -lpython2.7
DECODER_LIBS  = -L$(DIR_LIB) $(LIB_SYSLIB)  -L/lib64/


#add source files
SRC_SERVICE    = source source/url_handle
SRC_MAIN	   = main
DIR_PATH       = ./
DIR_SRCS       = $(SRC_SERVICE) $(SRC_MAIN)
#get source file
DECODER_CSRCS  = $(foreach dirs,$(DIR_SRCS),$(filter %.c, $(wildcard $(DIR_PATH)$(dirs)/*))) 
DECODER_CPPSRCS= $(foreach dirs,$(DIR_SRCS),$(filter %.cpp, $(wildcard $(DIR_PATH)$(dirs)/*))) 
#get object file
DECODER_COBJS  = $(DECODER_CSRCS:%.c=%.o)
DECODER_CPPOBJS= $(DECODER_CPPSRCS:%.cpp=%.o)
#get depend diles
DECODER_CDEPS  = $(DECODER_CSRCS:%.c=%.d)
DECODER_CPPDEPS= $(DECODER_CPPSRCS:%.cpp=%.d)




#add include files
DIR_INCLUDES    = ./include ./include/url_handle

DECODER_INCLUDES = $(foreach dirs,$(DIR_INCLUDES),-I$(wildcard $(dirs)))
DECODER_INCLUDES += -I/usr/include/python2.7/

DECODER_TARGET   = web_server
#rules to make web_server
server:$(DECODER_TARGET)
$(DECODER_TARGET):$(DECODER_COBJS) $(DECODER_CPPOBJS)
	@echo "start complie $(DECODER_TARGET)......"
	$(CXX) $^ $(DECODER_DFLAG) $(DECODER_CFLAG) $(DECODER_LIBS) -o $@
#	@$(CC) $^ $(DECODER_DFLAG) $(DECODER_CFLAG) $(DECODER_LIBS) -o $@ -Wl,-Map,$(DECODER_TARGET).map
#	@$(STRIP) $@
#	@$(STRIP) -x -R .note -R .comment $@
	@echo "------------------------------------"
	@echo "complie $(DECODER_TARGET) success"




#use depend file to refresh .o when .h or.c is changed
-include $(DECODER_CDEPS) $(DECODER_CPPDEPS)
#generate depend file	
%.d: %.c
	@echo create depend file $@....
	@set -e; rm -f $@
#make $*.o: $*.h... to $*.o $*.d: $*.h ...
	@$(CC) -MM -E $(DECODER_CFLAG) $(DECODER_DFLAG)  $(DECODER_INCLUDES) -I$(dir $@) $< > $@.$$$$; \
	sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $*)\1.o: ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
#add rules to make .o files
	@echo '	@echo "         CC    $$(notdir $$<)"' >> $@
	@echo '	@$$(CC) $$(DECODER_CFLAG) $$(DECODER_DFLAG) $$(DECODER_INCLUDES) -I$(dir $@) -c $$(filter %.c, $$^) -o $$(filter %.o, $$@)' >> $@ 
#add default rules in case of MM failed
	@sed '1 idefault-$(@):' -i $@
	
%.d: %.cpp
	@echo create depend file $@....
	@set -e; rm -f $@
#make $*.o: $*.h... to $*.o $*.d: $*.h ...
	@$(CC) -MM -E $(DECODER_CFLAG) $(DECODER_DFLAG)  $(DECODER_INCLUDES) -I$(dir $@) $< > $@.$$$$; \
	sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $*)\1.o: ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
#add rules to make .o files
	@echo '	@echo "         CXX   $$(notdir $$<)"' >> $@
	@echo '	@$$(CXX) $$(DECODER_CFLAG) $$(DECODER_DFLAG) $$(DECODER_INCLUDES) -I$(dir $@) $(DECODER_LIBS) -c $$(filter %.cpp, $$^) -o $$(filter %.o, $$@)' >> $@ 
#add default rules in case of MM failed
	@sed '1 idefault-$(@):' -i $@

clean:
	rm -rf  $(DECODER_COBJS) $(DECODER_CPPOBJS) $(DECODER_CDEPS) $(DECODER_CPPDEPS) $(DECODER_TARGET)


