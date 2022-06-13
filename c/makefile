HELPER=.
HEADER_DIR=include
CFLAGS=-I $(HEADER_DIR)
LDFLAGS=-L $(HEADER_DIR)
PROJ=protos

SRCS = $(shell ls src/*.c)
OBJS = 	$(addprefix objs/,$(notdir $(addsuffix .o,$(basename $(SRCS)))))

all: .depend objs $(OBJS) makefile $(PROJ)


$(PROJ): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

objs:
	mkdir objs

objs/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.depend: $(SRCS)
	$(CC) -Iinclude -MM $(SRCS) > .depend

#
#   P H O N Y
#   T A R G E T S
#
.PHONY: clean
clean:
	rm -f .depend $(PROJ) $(PROJ)_test
	rm -rf objs

.PHONY: flags
flags:
	# SRCS=$(SRCS)
	# OBJS=$(OBJS)
	# PROJ=$(PROJ)
	# CFLAGS=$(CFLAGS)

.PHONY: test
test:
	$(MAKE) -f test.mak

include .depend
