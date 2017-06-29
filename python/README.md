

## Setup:

I am installing all python packages in a "virtualenv" which allows you to separate the packages needed for different projects, and doesn't pollute the system Python directory.  If you don't have virtualenv, you can install it by:

    pip install virtualenvwrapper-win   # for windows
    # or
    pip install virtualenvwrapper       # for linux (not yet supported)

If Python is installed in `C:\Program Files`, you must run that in an elevated shell (`cmd`, "Run as Administrator").

    mkvirtualenv cmos

    pip install pyqt5
    pip install pypiwin32
    pip install numpy matplotlib

If numpy and matplotlib fail, you can get prebuild packages from http://www.lfd.uci.edu/~gohlke/pythonlibs, and install them via:

    pip install %USERPROFILE%\Downloads\numpy-1.13.0rc2+mkl-cp36-cp36m-win_amd64.whl 
    pip install %USERPROFILE%\Downloads\matplotlib-2.0.2-cp36-cp36m-win_amd64.whl

There is a bug in Virtualenv under Windows, and you have to copy one DLL to the virtual environment to get PyQt5 to work:

    copy %USERPROFILE%\AppData\Local\Programs\Python\Python36\python3.dll %VIRTUAL_ENV%\Scripts

## Installation of the C++ SWIG module:

Install swig, for Windows download it from http://www.swig.org/download.html.  Then run:

    cd swig
    "C:\Program Files\swigwin-3.0.12\swig.exe" -c++ -python -threads daq2.i

Setup a shell which has both python and MSVC, for example by

    "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\vsdevcmd" -arch=x64

and then build and install it.  Make sure you are in the `cmos` virtualenv.

    python setup.py build
    python setup.py install

When you want to activate this environment later, just use

    workon cmos

## Scripts

- `runcontrol.py`: Run Control GUI written in PyQt.
- `pydaq.py`: A mockup of the DAQ module written in Python.  Generates dummy data.
- `emulate_device.py`: You can use this to test the real C++ DAQ module, without the FPGA.  Creates fake data and writes it to a pipe.
- `test_daq_module.py`: Reads fake data from the pipe, and gives it to the C++ DAQ module.

## Testing

In one command window, do:

    cd src\kc705\python
    workon cmos
    python emulate_device.py

In another, do:

    cd src\kc705\python
    workon cmos
    python test_daq2_module.py

or

    python runcontrol.py

## Building and debugging the module with Visual Studio

There is a Visual Studio 2017 project in `swig\msvc`.  To use it, you must install the swig package manually once.  Then, edit `swig\msvc\DAQModule\path.props`.  Specify the path to SWIG, to your python.exe, and your python virtualenv (or python installation directory, if not using a venv):

    <SWIG_PATH>C:\Program Files\swigwin-3.0.12</SWIG_PATH>
    <PYTHON_PATH>$(USERPROFILE)\Envs\cmos</PYTHON_PATH>
    <PYTHON_CMD>$(PYTHON_PATH)\Scripts\Python.exe</PYTHON_CMD>
    <PYTHON_LIBS_PATH>$(LOCALAPPDATA)\Programs\Python\Python36\libs</PYTHON_LIBS_PATH>

Then you can open the solution, and run it.  Make sure you are using the Release x64 configuration.  For Debug, we would have to use a version of Python compiled with Debug settings.  It would make debugging nicer, but takes a bit of effort to set up.

The solution is set up to start the python run control script.  You can now set up breakpoints in the C++ code, and debug it like a normal C++ project.  Tip: when you step into the code, the C++ thread is probably called "ucrtbase.dll".
