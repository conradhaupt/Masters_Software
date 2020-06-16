#!/usr/bin/python

from sfcc_name_to_info import SFCCFile, SFCCFileBuilder, sfcc_name_to_info
import csv
import sys


def SFCCFileToCSVLine(sfcc: SFCCFile):
    format = '{},{},{},{},{}'.format(
        sfcc.size, sfcc.filename, sfcc.sfc, sfcc.compression, sfcc.bitshuffle)
    return format


csv_file = sys.argv[1]


# Print header
print('bytes,name,curve,compression,bittransposed')
# print('Bytes,Filename,SFC,Compression,BitShuffle')

#
with open(csv_file, 'r') as f:
    lines = f.readlines()
    for line in lines:
        size, filename = line.strip().split('\t')
        size = int(size)
        sfcc = sfcc_name_to_info(filename, calculateSize=False, size=size)
        if sfcc.bwt and 'BWT' not in sfcc.compression:
            # print(sfcc.bwt, sfcc.compression)
            continue
        elif sfcc.compression == 'None':
            continue
        print(SFCCFileToCSVLine(sfcc))
