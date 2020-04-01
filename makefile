OBJECTS = main.o signal_handlers.o utils.o recursive.o
simpledu: $(OBJECTS)
	cc $(OBJECTS) -o simpledu
main.o: signal_handlers.h utils.h
signal_handlers.o: signal_handlers.h 
utils.o: utils.h
recursive.o: recursive.h

clean: $(OBJECTS)
	rm $(OBJECTS)