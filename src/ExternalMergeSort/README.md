STEPS by STEPS on Linux Environment (completed)
		sudo su -
		sudo apt-get install gcc -y
		sudo apt install make

		cd ~
		rm -fr C2/
		git clone https://github.com/pedroAkiraDanno/C2
		cd C2/ ; git checkout develop ; cd ..
		cd C2/src/ExternalMergeSort/


   		 make
   		 ./mycreatelargefile








REFERENCE: https://github.com/Zinurist/ExternalMergeSort


