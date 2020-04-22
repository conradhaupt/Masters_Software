#!/usr/bin/env python3

# @file sfccinfo.py
# @author Conrad Haupt (conrad@conradhaupt.co.za)
# @brief
# @version 1.0
# @date 2019-10-28
#
# @copyright Copyright (c) 2020

"""
This script outputs information about the structure of the given sfcc file.

Usage:

python sfccinfo.py <sfcc filename>

"""

import numpy as np
import sys
from math import log2
import os


def SFCTYPE(sfcint):
    if sfcint == 0:
        return 'Raster'
    elif sfcint == 1:
        return 'Snake Scan'
    elif sfcint == 2:
        return 'Morton'
    elif sfcint == 3:
        return 'Gray Code'
    elif sfcint == 4:
        return 'Hilbert'
    else:
        return 'NULL'


def COMPTYPE(compint):
    if compint == 0:
        return 'None'
    elif compint == 1:
        return 'RLE'
    elif compint == 2:
        return 'Huffman'
    elif compint == 3:
        return 'LZ77'
    elif compint == 4:
        return 'LZ78'
    elif compint == 5:
        return 'LZW'
    elif compint == 6:
        return 'Deflate'
    elif compint == 7:
        return 'BWT77'
    elif compint == 8:
        return 'BWTW'
    elif compint == 9:
        return 'BWT'
    else:
        return 'Null'


sfcc_file = sys.argv[1]

print('Opening %s' % (sfcc_file))

sfcc = open(sfcc_file, 'rb')

magicword = sfcc.read(4)
if magicword != b'SFCC':
    raise RuntimeError(
        'File has incorrect header, are you sure this is a SFCC file?')

# Calculate header information
header = np.frombuffer(sfcc.read(5), dtype=np.uint8, count=5)
# for x in [ndims, k, dtype_nbytes, sfctype, comptype]:
ndims = header[0]
k = header[1]
dtype_nbytes = header[2]
sfctype = header[3]
comptype = header[4]
sidelength = 2**k


def toHex(x): return "".join([hex(ord(c))[2:].zfill(2) for c in x])


# Check if the filesize is correct
filesize = os.stat(sfcc_file).st_size
# Header size consists of SFCC and the five values
header_size = 4 + 5
expected_size = (sidelength ** ndims) * dtype_nbytes + header_size

if comptype == 0 and expected_size != filesize:
    raise RuntimeError('Filesize is not as expected: expected %d bytes but got %d bytes instead' % (
        expected_size, filesize))

print('#'*40)
print('#    File details')
print('#' * 40)
print('# Number of Dimensions:\t%d' % (ndims))
print('# Sidelength:\t\t%d\t[2^%d]' % (sidelength, k))
print('# DType bytes:\t%d' % (dtype_nbytes))
print('# SFC Type:\t\t%s' % (SFCTYPE(sfctype)))
print('# CompressionType:\t%s' % (COMPTYPE(comptype)))
print('#' * 40)
print('#')
print('# Samples of data [%d %d]' % (np.int(expected_size*2/5), np.int(expected_size*2/5)+16))
sfcc.read(np.int(expected_size*2/5))
print('# ...', ' '.join(["{:02X}".format(x) for x in np.frombuffer(sfcc.read(16),dtype=np.uint8)]), '...')
print('#' * 40)