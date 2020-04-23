#!/usr/bin/env python3

# @file sfcc_block_entropy.py
# @author Conrad Haupt (conrad@conradhaupt.co.za)
# @brief
# @version 1.0
# @date 2020-02-27
#
# @copyright Copyright (c) 2020

"""
This script prints out the block-entropy for the given file using the tool
binwalk from https://github.com/ReFirmLabs/binwalk.
"""

import binwalk as bw
import sys
import numpy as np
import csv
from tqdm import tqdm
from os.path import basename

CURVES = ['gry', 'mrtn', 'hbrt']
BITTRANSPOSED = 'btr'
BWT = 'bwt'


def curveToCSV(crv):
    if crv is None:
        return 'Raster'
    elif crv == 'gry':
        return 'Gray'
    elif crv == 'hbrt':
        return 'Hilbert'
    elif crv == 'mrtn':
        return 'Morton'
    else:
        return 'NULL'


def filenameToInformation(filename):
    # Get filename split entries
    entries_ = filename.split('.')
    entries = []
    entries.append(entries_[0])
    for a in entries_[2:]:
        entries.append(a)

    # Get file information
    name = entries[0]
    curve = None
    bittransposed = False
    bwt = False
    for e in entries[1:]:
        if e in CURVES:
            print('Processing curve')
            if curve is None:
                curve = e
            else:
                curve = 'NULL'
        elif e == BITTRANSPOSED:
            bittransposed = True
        elif e == BWT:
            bwt = True
        else:
            print('ERROR: unknown entry found. ', e, 'in', filename)
    return [name, curveToCSV(curve), bittransposed, bwt]


def getEntropyForFile(filename):
    TYPICAL_ENTROPY_LENGTH = 513
    modules = bw.scan(filename, '--nplot', '-K', '16384', '-E', quiet=True)
    offsets = np.ndarray(TYPICAL_ENTROPY_LENGTH, dtype=int)
    entropies = np.ndarray(TYPICAL_ENTROPY_LENGTH, dtype=float)
    for mod in modules:
        if isinstance(mod, bw.modules.entropy.Entropy):
            for i, res in enumerate(mod.results):
                offsets[i] = res.offset
                entropies[i] = res.entropy
    return (offsets, entropies)


if __name__ == '__main__':
    if len(sys.argv) > 1:
        files = sys.argv[1:]
    else:
        print('No files given')
        exit(-1)

    moving_entropy = []
    for f in tqdm(files, desc='Calculating moving entropy for files'):
        moving_entropy.append(getEntropyForFile(f))

    with open(sys.argv[1] + '.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        # Write header
        writer.writerow(['name', 'SFC', 'bittransposed', 'bwt',
                         'entropy_offsets', 'entropy_encoded'])
        for index, (offset, entropy) in tqdm(enumerate(moving_entropy), desc='Writing to CSV'):
            entries = filenameToInformation(basename(files[index]))
            print(entries)
            print(basename(files[index]))
            entries.append(offset.tostring())
            entries.append(entropy.tostring())
            writer.writerow(entries)
