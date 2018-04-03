CC = clang
CFLAGS = -g -fsanitize-coverage=trace-pc-guard
LDFLAGS = -fsanitize=address 

all: std_test

std_test: std_test.o

banner:
	@printf "#\n# Starting build...\n#\n"

clean:
	rm std_test

.PHONY:clean all banner