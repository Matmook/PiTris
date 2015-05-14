#
#################################################################################

DEBUG	= -O3
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe -pthread

LDFLAGS	= -L/usr/local/lib
LDLIBS  = -lm  -lpthread 

# Should not alter anything below this line
###############################################################################

SRC	=	utils.c i2c.c spi.c iotools.c uc1608.c wii.c pitris.c

OBJ	=	$(SRC:.c=.o)

BINS	=	$(SRC:.c=)

all:	pitris
		
pitris:	$(OBJ)
	@echo [link]
	@$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)
	
.c.o:
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo "[Clean]"
	@rm -f $(OBJ) *~ core tags $(BINS)

# DO NOT DELETE
