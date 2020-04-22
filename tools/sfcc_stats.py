#!/usr/bin/env python3

# @file sfcc_stats.py
# @author Conrad Haupt (conrad@conradhaupt.co.za)
# @brief
# @version 1.0
# @date 2019-11-16
#
# @copyright Copyright (c) 2020

"""
This file prints out statistics about a given sfcc file. These states are:
1. Total File Entropy
2. Number of values (count)
3. mean value
4. standard deviation
5. minimum value
6. maximum value
7. 25th percentile value
8. median value
9. 75th percentile value

3-9 are output for all sfcc values and their moving difference
(i.e. x_i - x_{i-1}).

For two-byte sfcc files, the values are taken as int16. For four-byte sfcc
files, the values are treated as bytes. This is to reduce the runtime of this
script when bitshuffle is used.

Usage:

python sfcc_stats.py <sfcc filename>

The above command will run for all files that begin with <sfcc filename>,
giving statistics for all compressed and mapped variants of the given file.
This is done so that the script need only be run once per input file when
benchmarking.
"""

import collections
import numpy as np
import sys
import math
import os
import itertools
import pandas as pd
from scipy import stats
import glob


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


def chunked_iterable(iterable, size):
    it = iter(iterable)
    while True:
        chunk = tuple(itertools.islice(it, size))
        if not chunk:
            break
        yield chunk


def getStats(filename, headings):
    sfcc = open(filename, 'rb')

    magicword = sfcc.read(4)
    if magicword != b'SFCC':
        raise RuntimeError(
            'File has incorrect header, are you sure this is a SFCC file?:', filename)

    # Calculate header information
    header = np.frombuffer(sfcc.read(5), dtype=np.uint8, count=5)
    # for x in [ndims, k, dtype_nbytes, sfctype, comptype]:
    # ndims = header[0]
    # k = header[1]
    dtype_nbytes = header[2]
    sfctype = header[3]
    # Extract encoded bit-transpose from sfctype
    bittransposed_int = sfctype & 0b10000000
    bit_transposed = bittransposed_int != 0
    # Reset bit-transpose from sfctype
    sfctype = sfctype & 0b01111111

    comptype = header[4]
    if comptype in [2, 5, 6]:
        np.frombuffer(sfcc.read(1), dtype=np.uint8)

    def entropy(data, base=256):
        e = np.float64(0)
        counter = collections.Counter(data)
        l = np.int(len(data))
        for count in counter.values():
            p_x = count / l
            e += -p_x * math.log(p_x, 256)
        return e

    def entropyNumpy(data, base=256, dtype=None, filename=None):
        _, counts = np.unique(data, return_counts=True,)
        # counts = list(collections.Counter(data).values())
        # print(list(counts))
        # print(len(counts))
        _base = 2 ** (8 * np.dtype(data_dtype).itemsize)
        if len(counts) > _base:
            raise RuntimeError(
                "More values encountered in entropy calculation than expected:", filename)
        return stats.entropy(counts, base=_base)

    # Determine data-types depending on file structure
    if comptype != 0:
        data_dtype = np.uint8
    elif dtype_nbytes == 2:
        data_dtype = np.int16
    elif dtype_nbytes == 4:
        data_dtype = np.uint8

    data = np.array([], dtype=data_dtype)
    while True:
        buff = sfcc.read(2**32)
        if not buff:
            break
        data_read = np.frombuffer(buff, dtype=data_dtype)
        data = np.append(data, data_read)
    if dtype_nbytes == 2:
        dataframe = pd.DataFrame(data.astype(np.int))
    elif dtype_nbytes == 4:
        dataframe = pd.DataFrame(data.astype(np.float))

    file_total_entropy = entropyNumpy(
        dataframe[0], dtype=data_dtype, filename=filename)
    stat_description = dataframe.describe()
    stat_description_names = stat_description.index
    stat_description_arr = [stat_description[0][x]
                            for x in stat_description_names]
    diff_stat_description = dataframe.diff()
    diff_stat_description = diff_stat_description.describe()
    diff_stat_description_name = diff_stat_description.index
    diff_description_arr = [diff_stat_description[0][x]
                            for x in diff_stat_description_name]
    headings += ';' + ';'.join(stat_description_names)
    headings += ';diff ' + ';diff '.join(diff_stat_description_name)

    original_file_name = filename.split('.sfcc', 1)[0] + '.sfcc'
    output = ';'.join([str(x) for x in [original_file_name, SFCTYPE(sfctype), bit_transposed, COMPTYPE(comptype), file_total_entropy,
                                        *stat_description_arr, *diff_description_arr]])
    output = output.replace('\n', ' ')
    return (output, headings)


def main():
    sfcc_file = sys.argv[1]

    headings = ';'.join(
        ['Filename', 'sfc', 'bit-transposed', 'compression', 'Total Entropy'])

    if sfcc_file == '--header':
        print(headings)
        exit()
    files = glob.glob(sfcc_file + '*')
    results = [getStats(x, headings) for x in files]
    results_unzipped = list(zip(*results))
    print(results_unzipped[1][0])
    for x in results_unzipped[0]:
        print(x)


# MAIN
if __name__ == '__main__':
    main()
