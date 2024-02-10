targets := libuthread
objs    := context.o preempt.o queue.o sem.o uthread.o

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -MMD
CFLAGS  += -g
PANDOC  := pandoc

AR      := ar
ARFLAGS := rcs

ifneq ($(V),1)
Q = @
endif

all: $(targets)

# Dep tracking *must* be below the 'all' rule
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

libuthread.a: $(objs)
	@echo "AR $@"
	$(Q)$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "clean"
	$(Q)rm -f $(targets) $(objs) $(deps)
