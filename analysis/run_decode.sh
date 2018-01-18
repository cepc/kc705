#!/bin/sh

### Max Decode Frame(Event) Number
max_decode_num=30000

### Data Format
data_format1=1
data_format2=2

### Data 12/07/2017###
#./decode.sh ./data/20171207/File20171207-3  ./rootfile/20171207/decode_File20171207-3.root $data_format2  $max_decode_num  ./rootfile/20171207/ana_File20171207-3.root
#./decode.sh ./data/20171207/File20171207-4  ./rootfile/20171207/decode_File20171207-4.root $data_format2  $max_decode_num  ./rootfile/20171207/ana_File20171207-4.root
#./decode.sh ./data/20171207/File20171207-5  ./rootfile/20171207/decode_File20171207-5.root $data_format2  $max_decode_num  ./rootfile/20171207/ana_File20171207-5.root

### Data 12/23/2017 ###
#./decode.sh ./data/20171223/2017-12-23-1.dat  ./rootfile/20171223/decode_File20171223-1.root $data_format1  $max_decode_num ./rootfile/20171223/ana_File20171223-1.root
#./decode.sh ./data/20171223/2017-12-23-2.dat  ./rootfile/20171223/decode_File20171223-2.root $data_format1  $max_decode_num ./rootfile/20171223/ana_File20171223-2.root
#./decode.sh ./data/20171223/2017-12-23-3.dat  ./rootfile/20171223/decode_File20171223-3.root $data_format1  $max_decode_num ./rootfile/20171223/ana_File20171223-3.root
./decode.sh ./data/20171223/2017-12-23-4.dat  ./rootfile/20171223/decode_File20171223-4.root $data_format1  $max_decode_num ./rootfile/20171223/ana_File20171223-4.root

exit 0