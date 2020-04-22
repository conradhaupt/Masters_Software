#!/usr/bin/env python3

# @file sfccinfo.py
# @author Conrad Haupt (conrad@conradhaupt.co.za)
# @brief
# @version 1.0
# @date 2019-08-11
#
# @copyright Copyright (c) 2020

"""
This script converts a supplied GeoTIFF file into an sfcc file. It is assumed
the GeoTIFF file is square and has a power-of-two sidelength.

Usage:

python geotiff2sfcc.py <GeoTIFF filename>

"""

# import matplotlib.pyplot as plt
import georaster as gr
import numpy as np
import sys
from math import log2
import os

gtiff = sys.argv[1]

print('Opening %s' % (gtiff))

img = gr.SingleBandRaster(gtiff)

print('Size:\t\t%dx%d' % (img.nx, img.ny))
print('Data Type:\t %s' % (img.dtype))
print('Data Type Size:\t %s' % (np.dtype(img.dtype).itemsize))

# Check if the dimensions are appropriate
if img.nx != img.ny:
    raise Exception('GeoTIFFs must be square')

_length = img.nx
# Check if the dimensions are a power-of-two
if not (_length and not _length & (_length - 1)):
    raise Exception('Sidelengths must be a power-of-two')

# Calculate header information
ndims = np.uint8(2)
sfctype = np.uint8(0)
comptype = np.uint8(0)
k = np.uint8(log2(_length))
dtype_nbytes = np.uint8(np.dtype(img.dtype).itemsize)

# Print header information


def toHex(x): return "".join([hex(ord(c))[2:].zfill(2) for c in x])


print(' '.join([toHex(a) for a in 'SFCC']))
print("{:02x}".format(ndims))
print("{:02x}".format(k))
print("{:02x}".format(dtype_nbytes))
print("{:02x}".format(sfctype))
print("{:02x}".format(comptype))

print('Max=', np.max(img.r))
print('Min=', np.min(img.r))

ofname = os.path.splitext(gtiff)[0] + '.sfcc'
print('Writing to file %s' % (ofname))
of = open(ofname, 'w+b')
# Create initial bytearray
ba = bytearray('SFCC', encoding='ascii')
# Append header information
for x in [ndims, k, dtype_nbytes, sfctype, comptype]:
    ba.append(x)
bav = bytearray(img.r)
ba += bav
of.write(ba)
of.close()
