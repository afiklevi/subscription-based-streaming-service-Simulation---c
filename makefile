CC = gcc
OBJS = mtmflix.o aux_f.o series.o user.o   tests/mtmFlixTestEx.o
EXEC = mtmflix
COMP_FLAG = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG


$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) mtm_ex3.o libmtm.a -o $@ -L.	

mtmflix.o: mtmflix.c mtmflix.h mtm_ex3.h list.h set.h user.h aux_f.h series.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

aux_f.o: aux_f.c mtm_ex3.h list.h series.h set.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

series.o: series.c series.h mtm_ex3.h list.h set.h user.h mtmflix.h aux_f.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

user.o: user.c user.h set.h list.h mtm_ex3.h mtmflix.h series.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

mtmFlixTestEx.o: tests/mtmFlixTestEx.c tests/test_utilities.h mtmflix.h mtm_ex3.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/$*.c

clean:
	rm -f $(OBJS) $(EXEC)

