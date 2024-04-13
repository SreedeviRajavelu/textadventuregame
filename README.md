## COMPILING AND STARTING GAME FILE USING MAKEFILE

make CSVFILE="csv_name.csv" run

## SUMMARY ON COMPILING AND STARTING GAME FILE WITHOUT USING MAKEFILE

gcc -c gamefile.c -std=c99 -O3 -march=native

gcc -c pbPlots.c -std=c99 -O3 -march=native

gcc -c supportLib.c -std=c99 -O3 -march=native

gcc gamefile.o pbPlots.o supportLib.o -lm -lregex -o gamefile

./gamefile.exe startingfile.txt csvfilenameToGenerate.csv

### ARGUMENTS IN COMMAND LINE

./gamefile startingfile.txt csvfilenameToGenerate.csv

startingfile.txt is a starter textfile with initial state values for game which will get updated at each save state.

So the starter text file is mandatory to have in same directory as game file at start of game.

#### There will be errors if the specified flags above are not used

Argument after -o is for name of executable to be run

#### Information from make.sh file on how to compile and run scripts for plotting of graphs

gcc -c pbPlots.c -std=c99 -O3 -march=native

gcc -c supportLib.c -std=c99 -O3 -march=native

gcc -c example1.c -std=c99 -O3 -march=native

gcc -c example2.c -std=c99 -O3 -march=native

gcc -c example3.c -std=c99 -O3 -march=native

gcc example1.o pbPlots.o supportLib.o -lm -o example1

gcc example2.o pbPlots.o supportLib.o -lm -o example2

gcc example3.o pbPlots.o supportLib.o -lm -o example3

strip example1

strip example2

strip example3
