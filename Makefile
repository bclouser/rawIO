
CFLAGS := -Wall

ALL: peek poke

peek: rawio.c
	$(CC) $(CFLAGS) -Dpoke $< -o $@

poke: rawio.c
	$(CC) $(CFLAGS) -Dpoke $< -o $@


clean:
	rm peek poke *.o
