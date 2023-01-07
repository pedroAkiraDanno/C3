# Store-random-numbers-in-a-text-file

Receive an unsigned integer N as input. Generate N random numbers between [0-100] and store them in a text file. Put newline character between each generated number.










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
		cd C3/src/random-number-file/r1/


   		 gcc random_file.c -o random_file
   		 ./random_file












# reference: https://github.com/ariseyda/Store-random-numbers-in-a-text-file/blob/master/README.md
