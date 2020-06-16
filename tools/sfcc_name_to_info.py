#!/usr/bin/python3
import sys
from os.path import basename, exists, getsize


class SFCCFileBuilder:
    def __init__(self):
        self.filename = None
        self.sfc = None
        self.compression = None
        self.bwt = False
        self.bitshuffle = False
        self.size = None

    def setFilename(self, var):
        self.filename = var

    def setSFC(self, var):
        self.sfc = var

    def setCompression(self, var):
        self.compression = var

    def setBWT(self):
        self.bwt = True

    def setBitShuffle(self):
        self.bitshuffle = True

    def setSize(self, var):
        self.size = var

    def build(self):
        if any([v is None for v in [self.filename, self.sfc, self.compression, self.bwt, self.bitshuffle, self.size]]):
            raise 'Not all variables were set in SFCCFileBuilder'

        # Handle BWT compression schemes
        if self.bwt:
            if self.compression == 'LZ77':
                self.compression = 'BWT77'
            elif self.compression == 'LZW':
                self.compression = 'BWTW'
            elif self.compression == 'LZ4':
                self.compression = 'BWT4'

        return SFCCFile(self.filename, self.sfc, self.compression, self.bwt, self.bitshuffle, self.size)


class sfccresults:
    def __init__(self, size, runtime):
        self.size = size
        self.runtime = runtime


class SFCCFile:
    def __init__(self, filename=None, sfc='Raster', compression='None', bwt=False, bitshuffle=False, size=None, runtime=None):
        self.filename = filename
        self.sfc = sfc
        self.compression = compression
        self.bwt = bwt
        self.bitshuffle = bitshuffle
        self.results = sfccresults(size, runtime)

    def __str__(self):
        """
        Example output:
        123456  s23_e019.ne:
          HILBERT
          GZIP
          BWT=ON  
          BitShuffle=Off
        """
        return self.toOnelineString(withSize=True)

    def __hash__(self):
        return hash((self.filename, self.sfc, self.compression, self.bwt, self.bitshuffle))

    def __eq__(self, other):
        return (self.filename, self.sfc, self.compression, self.bwt, self.bitshuffle) == (other.filename, other.sfc, other.compression, other.bwt, other.bitshuffle)

    def toOnelineString(self, withSize=False):
        if withSize:
            return '%d\t%s\t%s\t%s\tBWT=%s\tBitShuffle=%s' % (self.size, self.filename, self.sfc, self.compression, "ON" if self.bwt else "OFF", "ON" if self.bitshuffle else "OFF")
        else:
            return '%s\t%s\t%s\tBWT=%s\tBitShuffle=%s\n' % (self.filename, self.sfc, self.compression, "ON" if self.bwt else "OFF", "ON" if self.bitshuffle else "OFF")

    def updateCompressionFromBWT(self):
        # Handle BWT compression schemes
        if self.bwt:
            if self.compression == 'LZ77':
                self.compression = 'BWT77'
            elif self.compression == 'LZW':
                self.compression = 'BWTW'
            elif self.compression == 'LZ4':
                self.compression = 'BWT4'


def sfcc_name_to_info(filename: str, removePath=True, calculateSize=False, size=None):
    if calculateSize and exists(filename):
        _size = getsize(filename)
    elif calculateSize:
        _size = -1
    elif not calculateSize and size is None:
        _size = -1
    elif not calculateSize and size is not None:
        _size = size

    if removePath:
        _filename = basename(filename)
    else:
        _filename = filename
    filename_components = _filename.split('.')

    sfccbuilder = SFCCFileBuilder()

    # Filename
    try:
        sfcc_pos = filename_components.index('sfcc')
    except:
        print('sfcc not present in filename:', filename_components)
        exit(1)
    sfccbuilder.setFilename('.'.join(filename_components[:sfcc_pos]))
    del filename_components[:sfcc_pos+1]

    parts = set(filename_components)

    # Filesize
    sfccbuilder.setSize(_size)

    # Go through all SFCs
    if 'mrtn' in parts:
        sfccbuilder.setSFC('Z-Order')
    elif 'gry' in parts:
        sfccbuilder.setSFC('Gray')
    elif 'hbrt' in parts:
        sfccbuilder.setSFC('Hilbert')
    else:
        sfccbuilder.setSFC('Raster')

    # Bitshuffle
    if 'btr' in parts:
        sfccbuilder.setBitShuffle()

    # BWT
    if 'bwt' in parts:
        sfccbuilder.setBWT()

    # Compression
    if 'gz' in parts:
        sfccbuilder.setCompression('GZIP')
    elif 'bz2' in parts:
        sfccbuilder.setCompression('BZIP2')
    elif 'hff' in parts:
        sfccbuilder.setCompression('HUFFMAN')
    elif 'lz4' in parts:
        sfccbuilder.setCompression('LZ4')
    elif 'lz77' in parts:
        sfccbuilder.setCompression('LZ77')
    elif 'lzo' in parts:
        sfccbuilder.setCompression('LZO')
    elif 'lzw' in parts:
        sfccbuilder.setCompression('LZW')
    elif 'rle' in parts:
        sfccbuilder.setCompression('RLE')
    else:
        sfccbuilder.setCompression('None')

    return sfccbuilder.build()


if __name__ == '__main__':
    sfcc_name_to_info(sys.argv[1], calculateSize=False, size=sys.argv[2])
