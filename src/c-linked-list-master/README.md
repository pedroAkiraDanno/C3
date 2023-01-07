To compile:
  make
To run:
  ./list
To check for memory leaks (this will attempt to run with valgrind):
  make leaks


TODO:
- refactor to get rid of the List struct, so only left with node struct, should make things simpler (arguably)
- add CuTest to project
- create some simple test in separate directory 
- main function should do nothing except run the tests, but should allow option not to run the tests (which will do nothing for the moment)







## 1- step install gcc
		### login server:
		### On Ubuntu ###
		sudo su -
		sudo apt update  --yes
		sudo apt-get update && sudo apt-get upgrade -y
		apt list --upgradable
		sudo apt-get install -y gcc
		sudo apt-get install build-essential
		## Installing Clang on Linux
		sudo apt install -y clang


## STEPS by STEPS on Linux Environment (completed)

		sudo su -
		sudo apt-get install gcc -y
		sudo apt install make

		cd ~
		rm -fr C3/
		git clone https://github.com/pedroAkiraDanno/C3
		cd C3/ ; git checkout develop ; cd ..
		cd C3/src/c-linked-list-master/


   		 make
   		 ./list








#reference: https://github.com/skorks/c-linked-list
