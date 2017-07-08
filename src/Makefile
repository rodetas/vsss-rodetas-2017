COMPILER = g++ -std=c++11 -Wall -g
EXEC_PROG = Rodetas
BINARIES = $(EXEC_PROG)

SOURCES := $(shell find -name '*.cpp' -not -path "./test/*")

OBJECTS = $(SOURCES:.cpp=.o)

OPENCVLIB = `pkg-config opencv --cflags --libs`
GTKMMLIB = `pkg-config gtkmm-3.0 --cflags --libs`
LIBS = $(SFMLLIB) $(OPENCVLIB) $(GTKMMLIB) -lpthread

all: $(EXEC_PROG)
	@echo Rodetas Build Completed

%.o: %.cpp
	$(COMPILER) -c $(LIBS) -o $@ $< -w

$(EXEC_PROG): $(OBJECTS)
	$(COMPILER) -o $(EXEC_PROG) $(OBJECTS) $(LIBS)

# prevents make from getting confused
.PHONY : run
run:
	./$(EXEC_PROG)

.PHONY : clean 
clean:
	rm -rf $(EXEC_PROG) $(OBJECTS)

clean-instance:
	find files/instances/ ! -name 'fileCalibration.txt' -type f -exec rm -f {} +;