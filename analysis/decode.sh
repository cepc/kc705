#!/bin/sh

### Flag ###
decode_flag=1
analysis_flag=1

### Decode Execution
if [ $decode_flag -eq 1 ]
then

raw_data=$1
rootfile=$2
data_format=$3
max_decode_frame_number=$4

./decode2root $raw_data  $rootfile $data_format $max_decode_frame_number

fi

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