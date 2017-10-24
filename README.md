# kc705
Kintex-7 FPGA KC705

### Main library, executables and GUI

#### Windows
* CMake: download and execute the [CMake Win32 Installer](http://www.cmake.org/download/). Make sure to set the PATH variable during installation
* Mingw-64: download and install [mingw-w64](http://mingw-w64.yaxm.org/doku.php/download/mingw-builds). The default options work
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
Modify the build_env.bat according to your path 
Open cmd and execute the following commands:
```powershell
mkdir build
cd build
../build_env.bat
cmake -D_hypot=hypot .. -G "MinGW Makefiles"
mingw32-make
```

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

### Test

```python
python 
import daq
```
