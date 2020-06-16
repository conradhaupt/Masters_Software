#!/usr/bin/python

from sfcc_name_to_info import SFCCFile, SFCCFileBuilder, sfcc_name_to_info
import csv
import sys
from os.path import basename


def parallelToCSVFile(sfcc: SFCCFile, runtime: float):
    format = '{},{},{},{},{}'.format(
        runtime, sfcc.filename, sfcc.sfc, sfcc.compression, sfcc.bitshuffle)
    return format


def cliToSFCC(cli: str):
    components = cli.split(' ')
    filename = basename(components[-1])
    del components[-1]
    sfcc = sfcc_name_to_info(filename, calculateSize=False)

    components = set(components)
    # SFC
    if 'MORTON' in components:
        sfcc.sfc = 'Z-Order'
    elif 'HILBERT' in components:
        sfcc.sfc = 'Hilbert'
    elif 'GRAY_CODE' in components:
        sfcc.sfc = 'Gray'

    # Compression
    if 'HUFFMAN' in components:
        sfcc.compression = 'HUFFMAN'
    elif 'BZIP_LZW' in components:
        sfcc.compression = 'BWTW'
    elif 'BZIP_LZ77' in components:
        sfcc.compression = 'BWT77'
    elif 'LZ77' in components:
        sfcc.compression = 'LZ77'
    elif 'LZW' in components:
        sfcc.compression = 'LZW'
    elif 'RLE' in components:
        sfcc.compression = 'RLE'
    elif 'NONE' in components:
        sfcc.compression = 'None'
    elif 'GZIP' in components:
        sfcc.compression = 'GZIP'
    elif 'BZIP2' in components:
        sfcc.compression = 'BZIP2'
    elif 'LZ4' in components:
        sfcc.compression = 'LZ4'
    elif 'LZO' in components:
        sfcc.compression = 'LZO'

    # BWT
    if 'BWT' in components:
        sfcc.bwt = True

    # BitShuffle
    if '-b' in components:
        sfcc.bitshuffle = True

    sfcc.updateCompressionFromBWT()
    if sfcc.bwt and sfcc.compression == 'None':
        sfcc.compression = 'BWT'
    return sfcc


parallel_file = sys.argv[1]


# Print header
print('runtime_ms,name,curve,compression,bittransposed')
# print('Bytes,Filename,SFC,Compression,BitShuffle')

#
with open(parallel_file, 'r') as f:
    f.readline()
    lines = f.readlines()
    for line in lines:
        components = line.strip().split('\t')
        runtime = components[3]
        exitval = components[6]
        signal = components[7]
        cli = components[8]
        sfcc = cliToSFCC(cli)
        if sfcc.bwt and 'BWT' not in sfcc.compression:
            continue
        else:
            print(parallelToCSVFile(sfcc, int(float(runtime) * 1000)))
