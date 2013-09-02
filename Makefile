CXXFLAGS=-Wall -I./include

all: receiver sender

receiver: receiver.cpp src/pru.o include/pru.h rc_switch_in.bin
	g++ $(CXXFLAGS) receiver.cpp src/pru.o -lprussdrv -lpthread -o receiver

sender: sender.cpp src/pru.o include/pru.h rc_switch_out.bin
	g++ $(CXXFLAGS) sender.cpp src/pru.o -lprussdrv -lpthread -o sender

rc_switch_out.bin: src/rc_switch_out.p include/pru.hp
	cd src; pasm -b rc_switch_out.p; mv rc_switch_out.bin ../; cd ..
	
rc_switch_in.bin: src/rc_switch_in.p include/pru.hp
	cd src; pasm -b rc_switch_in.p; mv rc_switch_in.bin ../; cd ..
	
clean:
	rm rc_switch_*.bin receiver sender src/*.o
