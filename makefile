OBJECTS = main.o signal_handlers.o utils.o recursive.o log.o
simpledu: $(OBJECTS)
	cc $(OBJECTS) -Wall -o simpledu
main.o: signal_handlers.h utils.h
signal_handlers.o: signal_handlers.h 
utils.o: utils.h
recursive.o: recursive.h
log.o: log.h

clean: $(OBJECTS)
	rm $(OBJECTS)