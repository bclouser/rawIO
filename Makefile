
CFLAGS := -Wall

ALL: peek poke

peek: rawio.c
	$(CC) $(CFLAGS) -DPEEK $< -o $@

poke: rawio.c
	$(CC) $(CFLAGS) -DPOKE $< -o $@


clean:
	rm peek poke *.o
