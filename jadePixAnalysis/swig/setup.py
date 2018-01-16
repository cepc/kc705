#!/usr/bin/env python

"""
setup.py file for SWIG C\+\+/Python daq program version one
"""
from distutils.core import setup, Extension
jadePixAna_module = Extension('_pyJadePixAna',
sources=['Wrapdecode2root.cc', 'Wrappixelana.cc', jadePixAnaPYTHON_wrap.cxx',],
)
setup (name = 'pyJadePixAna',
version = '0.1',
author = "Liejian Chen",
description = """swig C\+\+/Python decode2root program version one""",
ext_modules = [jadePixAna_module],
py_modules = ["pyJadePixAna"],
)