#!/bin/env python

import pyJadePixAna as ja

decode = ja.Wrapdecode2root()

anaylysis = ja.Wrappixelana()


basefilename = "TestFile20180105-000"

raw_data = "D:\\code\\kc705\\test\\dat\\"+basefilename+".dat"
root_file = "D:\\code\\kc705\\test\\dat\\"+basefilename+".root"

data_format = 2
max_decode_frame_number = 100


decode.decode2root(raw_data, root_file, data_format, max_decode_frame_number)

number_of_input_file = 1
outfilename = "D:\\code\\kc705\\test\\dat\\Ana_"+basefilename+".root"

anaylysis.pixelana(max_decode_frame_number, number_of_input_file, outfilename, root_file)