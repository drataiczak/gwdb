CC := gcc

TOP := .

LIBDIR := $(TOP)/lib
INCDIR := $(TOP)/include
OUTDIR := $(TOP)/obj

SRC := $(wildcard ./*.c)
OBJ := $(patsubst ./%.c, $(OUTDIR)/%.o, $(SRC))
BIN := $(TOP)/gwdb.bin

CFLAGS := -Wall -std=c11 -O0 -g -I$(INCDIR)
LDFLAGS := -L$(LIBDIR) -ldiscord -lcurl -lcrypto -lpthread -lm


all: directory $(BIN)

$(OUTDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	
clean:
	rm -rf $(OUTDIR)

directory:
	@if [ ! -d $(OUTDIR) ]; then mkdir $(OUTDIR); fi;

.PHONY:
	all clean directory
