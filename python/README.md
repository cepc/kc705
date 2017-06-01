

## Setup:

I am installing all python packages in a "virtualenv" which allows you to separate the packages needed for different projects, and doesn't pollute the system Python directory.  If you don't have virtualenv, you can install it by:

    pip install virtualenv virtualenvwrapper

For Windows, download latest `numpy` and `matplotlib` from http://www.lfd.uci.edu/~gohlke/pythonlibs.  Make sure it matches your Python version (for Python 3.6 64 bit: cp36-win_amd64).

    mkvirtualenv cmos

    pip install pyqt5
    pip install pypiwin32 
    pip install %USERPROFILE%\Downloads\numpy-1.13.0rc2+mkl-cp36-cp36m-win_amd64.whl 
    pip install %USERPROFILE%\Downloads\matplotlib-2.0.2-cp36-cp36m-win_amd64.whl

There is a bug in Virtualenv under Windows, and you have to copy one DLL to the virtual environment to get PyQt5 to work:

    copy %USERPROFILE%\AppData\Local\Programs\Python\Python36\python3.dll %VIRTUAL_ENV%\Scripts

## Installation of the C++ SWIG module:

Install swig, for Windows download it from http://www.swig.org/download.html.  Then run:

    cd swig
    "C:\Program Files\swigwin-3.0.12\swig.exe" -c++ -python daq.i

Setup a shell which has both python and MSVC, for example by

    "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\vsdevcmd" -arch=x64

and then build and install it.  Make sure you are in the `cmos` virtualenv.

	set DISTUTILS_USE_SDK=1
    python setup.py build
    python setup.py install

When you want to activate this environment later, just use

    workon cmos

## Scripts

- `runcontrol.py`: Run Control GUI written in PyQt.
- `pydaq.py`: A mockup of the DAQ module written in Python.  Generates dummy data.
- `emulate_device.py`: You can use this to test the real C++ DAQ module, without the FPGA.  Creates fake data and writes it to a pipe.
- `test_daq_module.py`: Reads fake data from the pipe, and gives it to the C++ DAQ module.
