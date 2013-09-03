CXXFLAGS=-Wall -I./include

all: receiver

receiver: receiver.cpp src/pru.o include/pru.h rc_switch_in.bin
	g++ $(CXXFLAGS) receiver.cpp src/pru.o -lprussdrv -lpthread -o receiver

rc_switch_in.bin: src/rc_switch_in.p include/pru.hp
	cd src; pasm -b rc_switch_in.p; mv rc_switch_in.bin ../; cd ..
	
clean:
	rm rc_switch_in.bin receiver src/*.o
