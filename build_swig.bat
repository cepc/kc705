cd build

copy ..\include\*.h .\
copy ..\src\daq.cpp .\

swig -c++ -python -threads -outcurrentdir ..\swig\daq.i 

python ..\swig\setup.py build

python ..\swig\setup.py install


cd ..\
cd
