
CXX = g++

all : libMemImage.a

.PHONY : all clean

libMemImage.a : MemImage.o MemImageSimple.o
	$(AR) -r $@ $^

%.o : %.cc
	$(CXX) -O3 -std=c++11 -I. -c -o $@ $<

test.elf : test.cc libMemImage.a
	$(CXX) -std=c++11 -o $@ $^

clean :
	rm *.o *.a test.elf

