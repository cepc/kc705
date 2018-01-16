#!/bin/env python

import argparse
import sys
import os

parser = argparse.ArgumentParser(description='Decode for JadePix 1')

parser.add_argument('--data_format',
                    action='store',
                    default=1,
                    type=int,
                    help='data format')

parser.add_argument('--max_frames',
                    action='store',
                    default=30000,
                    type=int,
                    help='max decode frame number')

ARGS=parser.parse_args()

def main():
    args=sys.argv[1:]

    if len(args) < 1:
        return usage()

    if len(ARGS.files) < 1:
        return usage()

    if len(ARGS.files) ==2:
        in_raw_data = ARGS.files[0]
        out_root_file = ARGS.files[1]

    decodeCMD = ./decode2root+' '+str(in_raw_data)+' '+str() $max_decode_frame_number
    os.system(decodeCMD)


### Analysis Execution
if [ $analysis_flag -eq 1 ]
then

max_ana_frame_number=$4
number_of_input_files=1
input_file1=$2
outfilename=$5

./pixelana $max_ana_frame_number  $number_of_input_files  $outfilename $input_file1

fi

exit 0