all: sr_main1 sr_main2 sr_main3

sr_main1:
	@echo " Compile sr_main1 ...";
	gcc -g3 -Wall -I ./include/ -L ./lib/ -Wl,-rpath,./lib/ ./examples/sr_main1.c ./src/sort_file.c ./src/merge.c -lbf -o ./build/sr_main1 -O2

sr_main2:
	@echo " Compile sr_main2 ...";
	gcc  -g3 -Wall -I ./include/ -L ./lib/ -Wl,-rpath,./lib/ ./examples/sr_main2.c ./src/sort_file.c ./src/merge.c -lbf -o ./build/sr_main2 -O2

sr_main3:
	@echo " Compile sr_main3 ...";
	gcc  -g3  -Wall -I ./include/ -L ./lib/ -Wl,-rpath,./lib/ ./examples/sr_main3.c ./src/sort_file.c ./src/merge.c -lbf -o ./build/sr_main3 -O2



bf:
	@echo " Compile bf_main ...";
	gcc -I ./include/ -L ./lib/ -Wl,-rpath,./lib/ ./examples/bf_main.c -lbf -o ./build/runner -O2
