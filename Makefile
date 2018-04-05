CC := clang
CFLAGS := -g

define banner
	@printf "#\n# Starting $1...\n#\n"
endef

all:std_test

std_test:LDFLAGS += -fsanitize=address -lbsd
std_test:std_test.o

std_test.o:CFLAGS += -fsanitize-coverage=trace-pc-guard

clean:
	$(call banner,$@)
	rm std_test std_test.o

.PHONY:clean all