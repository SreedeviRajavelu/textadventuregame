## To compile the gamefile.c type in:

gcc gamefile.c -o gamefile -lregex

### To compile gamefile.c with graph function

gcc -c gamefile.c -o gamefile -std=c99 -O3 -march=native -lregex

gcc -c bar_graph.c -std=c99 -O3 -march=native

## To start the game, compile it and type in:

./gamefile.exe startingfile.txt csvfilenameToGenerate.csv

startingfile.txt is a starter textfile with initial state values for game which will get updated at each save state.

So the starter text file is mandatory to have in same directory as game file at start of game.

## To use the compiled graph file to output a PNG for bar graph and make use of graph dependecies

#### There will be errors if the following flags are not used

gcc bar_graph.o pbPlots.o supportLib.o -lm -o bar_graph -lregex
gcc gamefile.o pbPlots.o supportLib.o -lm -o inventory_graph -lregex

## Information from make.sh file on how to compile and run scripts for plotting of graphs

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
