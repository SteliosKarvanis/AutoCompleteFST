# AutoCompleteFST

## Requirements
```
sudo apt-get install cmake build-essential qtbase5-dev qt5-qmake python3.9 python3-pip 
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
To execute the functionalities with the UI, run:
```
cd bin
./app
```

## Test
 To run tests, use the `test.ipynb` notebook (move it to the `bin` folder first)

## Docker
```
docker build -t autocomplete .
```

```
docker run --rm -it --name autocomplete -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix autocomplete
```
# Plot FST Graph
To plot the FST graph, run:
```
python plot_fst.py ----input_dict_file=<path to your dict>
```
# Generating package
After build, on `build` folder, run:
```
cpack
```
And will save the files to `packages` folder
> Note: if the UI not open, try to run `xhost +` on host machine