#!/usr/bin/env python

"""
setup.py file for SWIG C\+\+/Python daq program version one
"""
from distutils.core import setup, Extension
daq_module = Extension('_daq',
sources=['daq.cpp', 'daq_wrap.cxx',],
)

setup (name = 'daq',
version = '0.1',
author = "Ada Luc",
description = """swig C\+\+/Python daq program version one""",
ext_modules = [daq_module],
py_modules = ["daq"],
)
