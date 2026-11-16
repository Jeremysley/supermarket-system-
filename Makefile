CXX     = g++
CXXFLAGS = -std=c++17 -Wall
LIBS    = -lsqlite3
TARGET  = supermarket
SRCS    = main.cpp database.cpp user.cpp inventory.cpp sales.cpp reports.cpp dashboard.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET) *.db *.txt
