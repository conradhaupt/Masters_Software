#!/usr/bin/env python3

# @file hdf52sfcc.py
# @author Conrad Haupt (conrad@conradhaupt.co.za)
# @brief
# @version 1.0
# @date 2019-10-28
#
# @copyright Copyright (c) 2020

"""
This script divides a supplied HDF5 file into sfcc files. It would need to be
modified to be applied to other HDF5 schemas other than the one used in SKA HDF5
data files.

Usage:

python hdf52sfcc.py <HDF5 filename>

"""

import h5py as h5
import numpy as np
import sys
from itertools import product
from progress.bar import Bar
from math import log2
from joblib import Parallel, delayed
import os

hdf5 = sys.argv[1]

print('Opening %s' % (hdf5))

h5file = h5.File(hdf5, 'r')
group = 'Data'
dataset = 'correlator_data'
data = h5file[group][dataset]

print('Size:\t\t%d' % (data.size))
print('Shape:\t\t %s' % (data.shape,))
print('Chunk shape:\t %s' % (data.chunks,))
print('Data Type:\t %s' % (data.dtype))

# Calculate header information
sidelength = 128
ndims = np.uint8(3)
sfctype = np.uint8(0)
comptype = np.uint8(0)
k = np.uint8(log2(sidelength))
dtype_nbytes = np.uint8(np.dtype(data.dtype).itemsize)


def toHex(x): return "".join([hex(ord(c))[2:].zfill(2) for c in x])


print(' '.join([toHex(a) for a in 'SFCC']))
print("{:02x}".format(ndims))
print("{:02x}".format(k))
print("{:02x}".format(dtype_nbytes))
print("{:02x}".format(sfctype))
print("{:02x}".format(comptype))

time_steps = range(0, 1024-sidelength, sidelength)
dim1_steps = range(0, 4096 - sidelength, sidelength)
dim2_steps = range(0, 544-sidelength, sidelength)

# Create initial bytearray
ba_header = bytearray('SFCC', encoding='ascii')
# Append header information
for x in [ndims, k, dtype_nbytes, sfctype, comptype]:
    ba_header.append(x)

combinations = product(time_steps, dim1_steps, dim2_steps)
combinations_length = len(time_steps) * len(dim1_steps) * len(dim2_steps)
print(combinations_length)


def doesntexist(t, x, y):
    ofname = 'chunks/' + \
        os.path.splitext(hdf5)[0] + '_' + \
        '_'.join((str(t), str(x), str(y))) + '.sfcc'
    return not os.path.exists(ofname)


def write_hdf5sfcc(data, t, x, y):
    global ba_header
    # global bar_
    ofname = 'chunks/' + \
        os.path.splitext(hdf5)[0] + '_' + \
        '_'.join((str(t), str(x), str(y))) + '.sfcc'
    # bar_.next()
    # bar_.suffix = '%(percent).1f%% - ' + str(ofname)
    # print(ofname)
    of = open(ofname, 'w+b')
    # # Create initial bytearray
    output = bytearray()
    output += ba_header
    output += bytearray(data.flatten())
    of.write(output)
    of.close()
    print('Completed: t=%d, x=%d, y=%d, length=%d' %
          (t, x, y, len(data.flatten())))


Parallel(n_jobs=6)(delayed(write_hdf5sfcc)(data[(0+t):(sidelength+t), (0+x):(sidelength+x), (0+y):(
    sidelength+y), 0], t, x, y) for t, x, y in combinations if doesntexist(t, x, y))
