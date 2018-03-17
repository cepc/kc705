# kc705
Kintex-7 FPGA KC705

### Main library, executables and GUI

#### Windows
* CMake: download and execute the [CMake Win32 Installer](http://www.cmake.org/download/). Make sure to set the PATH variable during installation
* Visual Studio 2017 community: download and install [vs studio 2017](https://www.visualstudio.com/downloads/). 
* Swig

#### OS X

* Clang

  Clang (at least version 4.21)

* Qt

  Install Qt5, ```brew install qt ```  

* Pyqt

  Install pyqt5, ```brew install pyqt ```

* Swig

  Install swig3, ```brew install swig```

* CMake
  Install cmake3.6 or later, ```brew install cmake```


### Setup

#### Windows
See the document in doc

#### OS X

* Config and compile

```shell
mkdir build
cd build 
cmake -DCAMKE_INSTALL_PREFIX=where you want to install ..
make
```

* Install
```shell
make install # Install the dynamic libraries
make install-python # Install the python module to ${PYTHON_LIB}/site-packages/
```

* Clean
```shell
make clean
```