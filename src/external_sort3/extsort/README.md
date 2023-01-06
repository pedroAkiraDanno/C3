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
		cd C3/src/external_sort3/extsort/


   		 make
   		 ./mergesort








REFENRECE: https://github.com/umbrant/extsort
