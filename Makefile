CC = gcc
CFLAGS = -Wall

EXAMPLE_OUTPUT = build/examples
EXAMPLE_OBJS = $(EXAMPLE_OUTPUT)/hello_world $(EXAMPLE_OUTPUT)/user_input $(EXAMPLE_OUTPUT)/nonblocking_user_input

$(EXAMPLE_OUTPUT):
	mkdir -p $(EXAMPLE_OUTPUT)

examples: $(EXAMPLE_OBJS)

$(EXAMPLE_OUTPUT)/%: examples/%.c | $(EXAMPLE_OUTPUT)
	$(CC) $(CFLAGS) -o $@ $<

