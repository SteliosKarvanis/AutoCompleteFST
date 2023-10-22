# AutoCompleteFST

## Requirements
```
sudo apt-get install cmake build-essential qtbase5-dev qt5-qmake
```

## Build

```
mkdir build
cd build
cmake ..
make
```

## Run
```
cd bin
./autocomplete
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

# Generating package
After build, on `build` folder, run:
```
cpack
```
And will save the files to `packages` folder
> Note: if the UI not open, try to run `xhost +` on host machine