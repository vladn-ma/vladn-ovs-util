#######################################################################################
# vn2@novatok.com
# 1504051104
# Makefile for manual build of the project using directories DebugMake and ReleaseMake
######################################################################################


SRCDIR	  := src
BUILDDIR  := Debug
OBJDIR	  := $(BUILDDIR)/obj
TARGETDIR := $(BUILDDIR)
TGZDIR	  := $(BUILDDIR)
RUNDIR	  := .
MKDIR_P   := mkdir -p

TARGET    := ethsr
TGZ	      := $(TARGET).tgz
SRCS      := $(notdir $(wildcard $(SRCDIR)/*.c))
OBJS      := $(patsubst %.c,%.o, $(SRCS))
DEPS      := $(patsubst %.c,%.d, $(SRCS))

DSRCS     := $(addprefix $(SRCDIR)/, $(SRCS))
DOBJS     := $(addprefix $(OBJDIR)/, $(OBJS))
DDEPS     := $(addprefix $(OBJDIR)/, $(DEPS))
DTARGET   := $(addprefix $(TARGETDIR)/, $(TARGET))
DTGZ	  := $(addprefix $(TGZDIR)/, $(TGZ)) 

CXX       := gcc
RM        := rm -rf
CXXFLAGS  := -O0 -g3 -Wall -c -fmessage-length=0 
CXXLIBS   := 

#all:	info 
all:	directories $(DTARGET)

directories: ${OBJDIR}

${OBJDIR}:
	${MKDIR_P} ${OBJDIR}

-include $(DDEPS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CXX) $(CXXFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' -e '/^$$/d' -e '/^.*:$$/d' < $(OBJDIR)/$*.d > $(OBJDIR)/$*.d.tmp
	@mv -f $(OBJDIR)/$*.d.tmp $(OBJDIR)/$*.d

#%.o: %.c
#	gcc -c $(CFLAGS) $*.c -o $*.o
#	gcc -MM $(CFLAGS) $*.c > $*.d
#	@mv -f $*.d $*.d.tmp
#	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
#	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
#	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
#	@rm -f $*.d.tmp

$(DTARGET): $(DOBJS)
	@echo 'Building target: $@'
	$(CXX) -o "$(DTARGET)" $^ $(CXXLIBS)
	@echo 'Finished building target: $@'
	@echo ' '

.PHONY : clean
clean:
	-$(RM) $(DOBJS) $(DDEPS) $(DTARGET)
	-@echo clean finished

.PHONY : run
run :
	$(DTARGET)

.PHONY : tgz
tgz:
	tar cvzf $(DTGZ) $(SRCDIR) Makefile

info :
	@echo '#CXXFLAGS'
	@echo $(CXXFLAGS)
	@echo
	@echo '#LDDFLAGS'
	@echo $(CXXLIBS)
	@echo
	@echo '#srcS'
	@echo $(SRCS)
	@echo
	@echo '#objS'
	@echo $(OBJS)
	@echo
	@echo '#deps'
	@echo $(DEPS)
	@echo
	@echo '#dir/srcs'
	@echo $(DSRCS)
	@echo
	@echo '#dir/objs'
	@echo $(DOBJS)
	@echo
	@echo '#dir/deps'
	@echo $(DDEPS)
	@echo
	@echo '#dir/target'
	@echo $(DTARGET)

#end of the make file
