# AutoCompleteFST
Functionalities to create a FST index, and make autocompleteons and fuzzy search on it. Also have functionalities to build a Levestein DFA for a given word and distance 
## Requirements
```
sudo apt update
sudo apt-get install cmake build-essential qtbase5-dev qt5-qmake python3-pip python3.9 
pip install graphviz
```

## Build

```
mkdir build
cd build
cmake ..
make
```

## Run
To execute the functionalities:
```
cd bin
```
* Run functionalities with GUI:
```
./app
```
* Run autocomplete to a list of prefixes in file (The output will be saved on a file under `output_files` folder)
```
./autocomplete <path to file with prefixes>
```
* Run binary search for a preffix (The output will be saved on a file under `output_files` folder)
```
./binary_search <preffix>
```
* Analyse build index time and storage space for fst index and binary search index (list of strings)
```
./indexes_analiser
```
* Get words from dict based on levestein (the output will be storaged at output_files folder)
```
./levestein <prefix> <dist>
```
* Build a levestein DFA for a given word and distance
```
./levesteinDFA <prefix> <dist>
```
* Get autocomplete time for binary search and FST
```
./timer <file with prefixes>
```
* Write fst to file to plot it with graphviz (Checkout PLOT section)
```
./write_fst <input_dict_file> <output_file>
```



## Test
 To run tests, use the `test.ipynb` notebook
## Search Analysis
Check out `time_analysis.ipynb` that contains a report with the autocomplete times for FST and from binary search
## Docker
```
docker build -t autocomplete .
```

```
docker run --rm -it --name autocomplete -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix autocomplete
```
> Note: if the UI not open, try to run `xhost +` on host machine
# Plot FST Graph
To plot the FST graph, run:
```
cd analysis
python plot_fst.py --input_graph_file=<path to graph file>
```
# Generating package
After build, on `build` folder, run:
```
cpack
```
And will save the files to `packages` folder
