#!/usr/bin/python3

from os import system
import sys
import sfccfile as sFile
from sfccfile import SFC, Compression, Config, SFCC, Results, GetCompression, GetSFC, SFCToArg, CompressionToArg

__SFCCOMPRESS_EXEC = '/home/conrad/sfccompress'
__GZIP_EXEC = '/usr/bin/gzip'
__BZIP2_EXEC = '/usr/bin/bzip2'
__LZ4_EXEC = '/usr/bin/lz4'
__LZO_EXEC = '/usr/bin/lzo'


def inFile(conf: Config, filename):
    return conf.previousConfig().expectedFilename(filename)


def compress_bzip2(conf: Config, filename):
    exit(
        system(' '.join([__BZIP2_EXEC, '-z -k -9 -f', inFile(conf, filename)])))


def compress_gzip(conf: Config, filename):
    exit(system(' '.join([__GZIP_EXEC, '-k -9 -f', inFile(conf, filename)])))


def compress_lz4(conf: Config, filename):
    exit(system(
        ' '.join([__LZ4_EXEC, '-9 -f', inFile(conf, filename), inFile(conf, filename) + '.lz4'])))


def compress_lzo(conf: Config, filename):
    exit(system(' '.join([__LZO_EXEC, '-9 -f -o',
                          inFile(conf, filename) + '.lzo', inFile(conf, filename)])))


def compress_sfccompress(conf: Config, filename):
    _sfc, _comp, _bs = conf.sfccompressArgs()
    exit(system(' '.join([__SFCCOMPRESS_EXEC, 'compress', '-s',
                          _sfc, '-c', _comp, _bs, inFile(conf, filename)])))


def decompress_bzip2(conf: Config, filename):
    exit(
        system(' '.join([__BZIP2_EXEC, '-d -k -f', conf.expectedFilename(filename)])))


def decompress_gzip(conf: Config, filename):
    exit(
        system(' '.join([__GZIP_EXEC, '-d -k -f', conf.expectedFilename(filename)])))


def decompress_lz4(conf: Config, filename):
    exit(
        system(' '.join([__LZ4_EXEC, '-d -f', conf.expectedFilename(filename), inFile(conf, file())])))


def decompress_lzo(conf: Config, filename):
    exit(
        system(' '.join([__LZO_EXEC, '-d -f -o', inFile(conf, filename), conf.expectedFilename(filename)])))


def decompress_sfccompress(conf: Config, filename):
    exit(
        system(' '.join([__SFCCOMPRESS_EXEC, 'decompress', conf.expectedFilename(filename)])))


if len(sys.argv) != 7:
    print(
        'sfcc_compress.py [compress | decompress] <raster|zorder|gray|hilbert> <bwt|nobwt> <btr|nobtr> <huff|gzip|bzip|lz4|lz77|lzo|lzw|rle|none> <original_file>')
    exit(1)

compress = sys.argv[1] == 'compress'
decompress = sys.argv[1] == 'decompress'
if not compress and not decompress:
    raise RuntimeError('Invalid command: %s' % (sys.argv[1]))
arg_sfc = GetSFC(sys.argv[2])
arg_bwt = True if sys.argv[3] in sFile.__BWT else False
arg_bitshuffle = True if sys.argv[4] in sFile.__BITSHUFFLE else False
arg_compression = GetCompression(sys.argv[5])
arg_filename = sys.argv[6]

conf = Config(arg_sfc, arg_bwt, arg_bitshuffle, arg_compression)
print(conf)
if compress:
    if not conf.compression.external():
        compress_sfccompress(conf, filename)
    elif conf.compression == Compression.bzip2:
        compress_bzip2(conf, filename)
    elif conf.compression == Compression.gzip:
        compress_gzip(conf, filename)
    elif conf.compression == Compression.lz4:
        compress_lz4(conf, filename)
    elif conf.compression == Compression.lzo:
        compress_lzo(conf, filename)
    else:
        raise RuntimeError('Unknown compression scheme: %s' %
                           (conf.compression))
else:
    if not conf.compression.external():
        compress_sfccompress(conf, filename)
    elif conf.compression == Compression.bzip2:
        compress_bzip2(conf, filename)
    elif conf.compression == Compression.gzip:
        compress_gzip(conf, filename)
    elif conf.compression == Compression.lz4:
        compress_lz4(conf, filename)
    elif conf.compression == Compression.lzo:
        compress_lzo(conf, filename)
    else:
        raise RuntimeError('Unknown compression scheme: %s' %
                           (conf.compression))
