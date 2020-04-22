#!/usr/bin/env python3

# @file compress_config.py
# @author Conrad Haupt (conrad@conradhaupt.co.za)
# @brief
# @version 1.0
# @date 2019-10-11
#
# @copyright Copyright (c) 2020

"""
This script prints out all combinations of SFC and compression schemes suitable
for input as arguments to sfccompress. The input arguments are sfcc files to
compress using sfccompress.

Example usage using GNU Parallel and sfccompress:

python compress_config.py *.sfcc > compress_sfcc_arguments

cat invariance_test.csv | \

parallel -j 8 --joblog parallel.compress.log --bar --colsep "\t" \
~/Software_Masters/SFCCompare/build/sfccompress/sfccompress \
compress -s {1} -c {2} {3}

"""

import sys
from itertools import product

compression_schemes = ['HUFFMAN', 'LZ77', 'LZW',
                       'RLE', 'NONE', 'BWT', 'BZIP_LZ77', 'BZIP_LZW']
curves = ['ROW_MAJOR', 'MORTON', 'GRAY_CODE', 'HILBERT']

files = sys.argv
files.remove(sys.argv[0])

for file, curve, comp in product(files, curves, compression_schemes):
    print('\t'.join([curve, comp, file]))
