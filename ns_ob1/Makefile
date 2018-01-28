# This Makefile is used to compile BB1 v4 and archive the object
# files into library libBB1.a

#########################################################
#                                                       #
#       Nom du programme et des fichiers sources        #
#                                                       #
#########################################################

NAME	= libBB1.a				# nom de la lib

IPATH	= ../include				# liste des include
LPATH	= 					# liste des libs

SRC	= AttValPair.cpp TypedVal.cpp\
	BB1Link.cpp BB1Object.cpp BB1AppInst.cpp BB1Class.cpp BB1Event.cpp BB1Agenda.cpp\
	BB1Decision.cpp BB1Focus.cpp BB1Strategy.cpp BB1Message.cpp\
	BB1KB.cpp BB1KS.cpp BB1KSAR.cpp\
	BB1BBam.cpp BB1BBcreate.cpp BB1BBcycle.cpp BB1BBeval.cpp BB1BBevent.cpp BB1BBfns.cpp\
	BB1BBmmr.cpp BB1BBmsg.cpp BB1BBname.cpp BB1BBphase.cpp BB1BBrate.cpp\
	BB1API.cpp BB1APIconst.cpp BB1APIdef.cpp BB1APIfns.cpp BB1APIget.cpp BB1APIui.cpp\
	BB1KSKB.cpp ClassKB.cpp ControlPlanKB.cpp KBLangParser.cpp KBLangLexer.cpp KBLangFns.cpp\
	BB1List.cpp BB1Set.cpp BB1Exception.cpp\
	BB1types.cpp

LIB	=

#########################################################
#                                                       #
#               Definitions des macros                  #
#                                                       #
#########################################################

# Use -g flag for development, use -O<n> flag for production <n> = null,1,2,3
# Use -D__IPC__ to compile library with IPC functions

# The *.a library itself.  Remove the old one and build an entirely new one.
# The *.o files, not being automatically generated, always have the short names.

AR	= ar
ARFLAGS	= rcv			# rcvs

CPP	= g++
MAKE	= make
MAKELIB	= $(AR) $(ARFLAGS)
RANLIB	= ranlib		# /bin/true
LEX	= flex			# lex
YACC	= bison			# yacc
YFLAGS	= 
SHELL	= /bin/sh
OBJS	= $(SRC:.cpp=.o)
RM	= /bin/rm -f
COMP	= gzip -9v
UNCOMP	= gzip -df
STRIP	= strip

OBJOPT	= -g2 -exceptions	# -fhandle-exceptions does NOT mix with -O
EXEOPT	= -o

COMPIL	= $(CPP) $(OBJOPT) $(EXEOPT)

CFLAGS	= $(OBJOPT) -I$(IPATH)
LFLAGS	= $(EXEOPT)

LIBS	= -L$(LPATH) $(LIB)
INCLS	= -I$(IPATH)

.SUFFIXES: .h.Z .c.Z .h.gz .c.gz .c.z .h.z
#.SUFFIXES : .C .c .cpp .h .l .o .y $(SUFFIXES)

#########################################################
#                                                       #
#       Instructions de compilation primaires           #
#                                                       #
#########################################################

all	: $(NAME)

$(NAME)	: $(OBJS)
	$(MAKELIB) $(NAME) $(OBJS)
	$(RANLIB) $(NAME)
	mv $(NAME) ../lib

clean	:
	-$(RM) $(OBJS) *~ \#* *.core

fclean	: clean
	-$(RM) ../lib/$(NAME)

comp	: fclean
	$(COMP) $(INCL) $(SRC)

uncomp	:
	$(UNCOMP) $(SRC) $(INCL)

.c.Z.c .h.Z.h .c.gz.c .h.gz.h .c.z.c .h.z.h :
	-$(UNCOMP) $<

.c.o :
	${CPP} ${CFLAGS} -c $<

.cc.o :
	${CPP} ${CFLAGS} -c $<

.C.o :
	${CPP} ${CFLAGS} -c $<

.cpp.o :
	${CPP} ${CFLAGS} -c $<

.l.c :
	${LEX} $<
	mv lex.yy.c $@

.y.c :
	${YACC} -d $<
	mv $*.tab.h ../include/$*.h
	mv $*.tab.c $@


#########################################################
#                                                       #
#                       Dependences                     #
#                                                       #
#########################################################

# ##############################################################################
#
# The rest of BB1
# The rest of this make file builds the BB1 library functions
#
# ##############################################################################

AttValPair.o	: ../include/AttValPair.h

BB1Agenda.o	: ../include/BB1Agenda.h ../include/BB1KSAR.h

BB1API.o	: ../include/AttValPair.h ../include/BB1BB.h ../include/BB1KB.h ../include/BB1KSKB.h ../include/ClassKB.h ../include/ControlPlanKB.h

BB1APIconst.o	: ../include/AttValPair.h ../include/BB1BB.h 

BB1APIdef.o	: ../include/BB1BB.h ../include/BB1Class.h ../include/BB1Focus.h ../include/BB1KB.h ../include/BB1KS.h ../include/BB1Link.h ../include/BB1Strategy.h

BB1APIfns.o	: ../include/AttValPair.h ../include/BB1BB.h

BB1APIget.o	: ../include/BB1BB.h ../include/BB1KB.h ../include/BB1Event.h ../include/BB1KSAR.h

BB1APIui.o	: ../include/BB1BB.h ../include/BB1Event.h ../include/BB1KB.h ../include/BB1KS.h ../include/BB1KSAR.h

BB1AppInst.o	: ../include/BB1AppInst.h

BB1BBam.o	: ../include/BB1BB.h ../include/BB1KSAR.h

BB1BBcreate.o	: ../include/BB1BB.h ../include/BB1Event.h ../include/BB1KB.h ../include/BB1KS.h ../include/BB1KSAR.h ../include/BB1Link.h

BB1BBcycle.o	: ../include/BB1BB.h ../include/BB1Event.h ../include/BB1KB.h ../include/BB1KS.h ../include/BB1KSAR.h

BB1BBeval.o	: ../include/BB1BB.h ../include/BB1Event.h ../include/BB1KS.h ../include/BB1KSAR.h

BB1BBevent.o	: ../include/AttValPair.h ../include/BB1Link.h ../include/BB1BB.h ../include/BB1Event.h ../include/BB1KB.h

BB1BBfns.o	: ../include/BB1BB.h ../include/BB1Class.h ../include/BB1KB.h ../include/BB1Link.h

BB1BBmmr.o	: ../include/BB1BB.h ../include/BB1AppInst.h ../include/BB1Class.h ../include/BB1KB.h

BB1BBmsg.o	: ../include/BB1BB.h ../include/AttValPair.h ../include/BB1KB.h ../include/BB1Message.h

BB1BBname.o	: ../include/BB1BB.h

BB1BBphase.o	: ../include/BB1BB.h

BB1BBrate.o	: ../include/BB1BB.h ../include/BB1Focus.h ../include/BB1KSAR.h

BB1Class.o	: ../include/BB1Class.h

BB1Decision.o	: ../include/BB1Decision.h ../include/BB1BB.h

BB1Event.o	: ../include/BB1Event.h ../include/AttValPair.h ../include/BB1KB.h ../include/BB1Link.h

BB1Exception.o	: ../include/BB1Exception.h ../include/BB1BB.h

BB1Focus.o	: ../include/BB1Focus.h

BB1KB.o		: ../include/BB1KB.h ../include/AttValPair.h ../include/BB1BB.h ../include/BB1Class.h ../include/BB1Link.h

BB1KS.o		: ../include/BB1KS.h

BB1KSAR.o	: ../include/BB1KSAR.h ../include/BB1Event.h ../include/BB1KB.h ../include/BB1KS.h

BB1KSKB.o	: ../include/BB1KSKB.h ../include/BB1BB.h ../include/BB1KB.h ../include/BB1Event.h ../include/BB1Strategy.h

BB1List.o	: ../include/BB1List.h ../include/BB1Exception.h

BB1Link.o	: ../include/BB1Link.h ../include/BB1Object.h ../include/BB1KB.h ../include/BB1BB.h

BB1Message.o	: ../include/BB1Message.h

BB1Object.o	: ../include/BB1Object.h ../include/BB1Class.h ../include/BB1KB.h ../include/BB1Link.h

BB1Set.o	: ../include/BB1Set.h

BB1Strategy.o	: ../include/BB1Strategy.h ../include/BB1BB.h ../include/BB1Focus.h

ClassKB.o	: ../include/ClassKB.h ../include/BB1BB.h

ControlPlanKB.o	: ../include/ControlPlanKB.h ../include/BB1BB.h ../include/BB1KSAR.h

KBLangFns.o	: ../include/AttValPair.h ../include/BB1BB.h ../include/BB1Class.h ../include/BB1KB.h ../include/BB1Link.h ../include/KBLangFns.h ../include/KBLang.h

KBLangLexer.o	: ../include/KBLangFns.h ../include/KBLang.h 

KBLangParser.o	: ../include/KBLangFns.h

TypedVal.o	: ../include/TypedVal.h

#
# Header (*.h) files
#

../include/AttValPair.h	:
	../include/TypedVal.h
	touch ../include/AttValPair.h

../include/BB1Agenda.h :
	../include/BB1types.h
	touch ../include/BB1Agenda.h

../include/BB1BB.h :
	../include/BB1Agenda.h
	touch ../include/BB1BB.h

../include/BB1AppInst.h :
	../include/BB1Object.h
	touch ../include/BB1AppInst.h

../include/BB1Class.h :
	../include/BB1AppInst.h
	touch ../include/BB1Class.h

../include/BB1Decision.h :
	../include/BB1Object.h
	touch ../include/BB1Decision.h

../include/BB1Event.h :
	../include/BB1Object.h
	touch ../include/BB1Event.h

../include/BB1Exception.h :

../include/BB1Focus.h :
	../include/BB1Decision.h
	touch ../include/BB1Focus.h

../include/BB1KB.h :
	../include/BB1types.h
	touch ../include/BB1KB.h

../include/BB1KS.h :
	../include/BB1Object.h
	touch ../include/BB1KS.h

../include/BB1KSAR.h :
	../include/AttValPair.h
	../include/BB1Object.h
	touch ../include/BB1KSAR.h

../include/BB1KSKB.h :
	../include/BB1types.h
	touch ../include/BB1KSKB.h

../include/BB1Link.h :
	../include/BB1types.h
	touch ../include/BB1Link.h

../include/BB1List.h :

../include/BB1Message.h :
	../include/BB1Object.h
	touch ../include/BB1Message.h

../include/BB1Object.h :
	../include/TypedVal.h
	../include/AttValPair.h
	../include/BB1types.h
	touch ../include/BB1Object.h

../include/BB1Set.h :
	../include/BB1List.h
	touch ../include/BB1Set.h

../include/BB1Strategy.h :
	../include/BB1Decision.h
	touch ../include/BB1Strategy.h

../include/BB1types.h :
	../include/BB1List.h
	../include/BB1Set.h
	../include/BB1Exception.h
	touch ../include/BB1types.h

../include/ClassKB.h :

../include/ControlPlanKB.h :

../include/TypedVal.h :
	../include/BB1types.h
	touch ../include/TypedVal.h

#
# KB definition language, made with lex and yacc
#

KBLangLexer.c: KBLangLexer.l
	${LEX} KBLangLexer.l
	mv lex.yy.c KBLangLexer.c

KBLangParser.c ../include/KBLang.h: KBLangParser.y ../include/KBLangFns.h
	${YACC} -d KBLangParser.y
	mv KBLangParser.tab.h ../include/KBLangParser.h
	mv KBLangParser.tab.c KBLangParser.c
