#!/usr/bin/env python

"""
setup.py file for SWIG C\+\+/Python daq program version one
"""
from distutils.core import setup, Extension
decode2root_module = Extension('_pydecode2root',
sources=['decode2root.cc', 'decode2rootPYTHON_wrap.cxx',],
)
setup (name = 'pydecode2root',
version = '0.1',
author = "Liejian Chen",
description = """swig C\+\+/Python decode2root program version one""",
ext_modules = [decode2root_module],
py_modules = ["pydecode2root"],
)