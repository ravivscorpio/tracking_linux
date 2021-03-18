IDIR =./include
CC=g++
CFLAGS=-I$(IDIR)
ODIR=./output
LDIR =./lib
LIBS=-lm

_DEPS =  defs_p.h\
         motorInfo.h\
         rc.h\
         Comm.h\
         defs.h\
         protocol.h\
         rc_p.h\
         Utils.h\
         matrix.h\
         init.h\
         serial.h\
		 MidjDrv.h\
	 	 Comm_p.h\
         Utils_p.h\
		 aim.h\
		 SGP4.h\
		 term.h\

_DEPS_P =./Comm/Comm_p.h\
         ./utils/Utils_p.h\
		 ./MIDG/MidjDrv_p.h

DEPS_G = $(addprefix $(IDIR)/,$(_DEPS))

DEPS =   $(_DEPS_P) $(DEPS_G)

SOURCES =	./motorInfo/motorInfo.c\
	        ./Comm/Comm.c\
		    ./utils/Utils.c\
	        ./matrix/matrix.c\
	        ./serial/serial.c\
	        ./init/init.c\
			./MIDG/MidjDrv.c\
			./aim/aim.c\
			./SGP4/SGP4.c

_OBJ=$(SOURCES:%.c=%.o)
OBJ = $(addprefix $(ODIR)/,$(notdir $(SOURCES:%.c=%.o)))

motor:  $(_OBJ)
	@echo Linking..........................................
	$(CC) -g  -o  $@	$(addprefix $(ODIR)/,$(^F)) $(CFLAGS) $(LIBS) -lpthread


$(_OBJ) : %.o : %.c
	@echo Compiling.......................................
	$(CC) -g $(CFLAGS)  -c -o $(ODIR)/$(@F)	$< -lpthread




.PHONY:clean
clean:
	rm motor
	#find -type f -name '*.o' -delete
	rm ./output/*.o


