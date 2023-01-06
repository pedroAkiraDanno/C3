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
		rm -fr C2/
		git clone https://github.com/pedroAkiraDanno/C2
		cd C2/ ; git checkout develop ; cd ..
		cd C2/src/external_sort3/extsort/


   		 make
   		 ./mergesort








REFENRECE: https://github.com/umbrant/extsort
