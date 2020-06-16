from enum import Enum
from os.path import basename


class SFC(Enum):
    raster = 0
    zorder = 1
    gray = 2
    hilbert = 3


def SFCToStr(sfc):
    if sfc == SFC.raster:
        return 'Raster'
    elif sfc == SFC.zorder:
        return 'Z-Order'
    elif sfc == SFC.gray:
        return 'Gray'
    elif sfc == SFC.hilbert:
        return 'Hilbert'


__RASTER = ['raster', 'row_major', 'rstr']
__ZORDER = ['zorder', 'morton', 'mrtn']
__GRAY = ['gray', 'gray_code', 'gry']
__HILBERT = ['hilbert', 'hbrt']

__SFC = [*__RASTER, *__ZORDER, *__GRAY, *__HILBERT]


def GetSFC(stringSFC: str):
    _sfc = stringSFC.lower()
    if _sfc in __RASTER:
        return SFC.raster
    elif _sfc in __ZORDER:
        return SFC.zorder
    elif _sfc in __GRAY:
        return SFC.gray
    elif _sfc in __HILBERT:
        return SFC.hilbert
    else:
        raise RuntimeError("Unexpected SFC: %s" % (stringSFC))


class Compression(Enum):
    none = 0
    bzip2 = 1
    gzip = 2
    huffman = 3
    lz4 = 4
    lz77 = 5
    lzo = 6
    lzw = 7
    rle = 8


def CompressionToStr(comp):
    if comp == Compression.none:
        return 'None'
    elif comp == Compression.bzip2:
        return 'BZIP2'
    elif comp == Compression.gzip:
        return 'GZIP'
    elif comp == Compression.huffman:
        return 'Huffman'
    elif comp == Compression.lz4:
        return 'LZ4'
    elif comp == Compression.lz77:
        return 'LZ77'
    elif comp == Compression.lzo:
        return 'LZO'
    elif comp == Compression.lzw:
        return 'LZW'
    elif comp == Compression.rle:
        return 'RLE'


__HUFFMAN = ['huffman', 'huff', 'hff']
__GZIP = ['gzip', 'gz']
__BZIP2 = ['bzip', 'bzip2', 'bz2']
__LZ4 = ['lz4']
__LZ77 = ['lz77', 'bzip_lz77']
__LZO = ['lzo']
__LZW = ['lzw', 'bzip_lzw']
__RLE = ['rle']
__NONE = ['none']

__COMPRESSON = [*__NONE, *__HUFFMAN, *__GZIP, *__BZIP2,
                *__LZ4, *__LZ77, *__LZO, *__LZW, *__RLE]


def GetCompression(stringCompression: str):
    _comp = stringCompression.lower()
    if _comp in __HUFFMAN:
        return Compression.huffman
    elif _comp in __GZIP:
        return Compression.gzip
    elif _comp in __BZIP2:
        return Compression.bzip2
    elif _comp in __LZ4:
        return Compression.lz4
    elif _comp in __LZ77:
        return Compression.lz77
    elif _comp in __LZO:
        return Compression.lzo
    elif _comp in __LZW:
        return Compression.lzw
    elif _comp in __RLE:
        return Compression.rle
    elif _comp in __NONE:
        return Compression.none
    else:
        raise RuntimeError("Unexpected compression scheme: %s" %
                           (stringCompression))


class SFCC:
    def __init__(self, filename: str, sfc, bwt: bool, bitshuffle: bool, compression):
        self.filename = filename
        if isinstance(sfc, str):
            self.sfc = GetSFC(sfc)
        else:
            self.sfc = sfc

        if isinstance(compression, str):
            self.compression = GetCompression(compression)
        else:
            self.compression = compression

        self.bwt = bwt
        self.bitshuffle = bitshuffle

    @property
    def filename(self):
        return self._filename

    @filename.setter
    def filename(self, value):
        vals = value.split('.')
        if 'sfcc' in vals:
            vals.remove('sfcc')
        if 'tif' in vals:
            vals.remove('tif')
        self._filename = '.'.join(vals)

    @filename.deleter
    def filename(self):
        del self._filename

    def __eq__(self, other):
        return (self.filename, self.sfc, self.bwt, self.bitshuffle, self.compression) == (other.filename, other.sfc, other.bwt, other.bitshuffle, other.compression)

    def __hash__(self):
        return hash((self.filename, self.sfc, self.bwt, self.bitshuffle, self.compression))

    def __str__(self):
        return '{}, {}, {} {}, {} {}, {}'.format(self.filename, self.sfc, 'bwt:', self.bwt, 'bitshuffle:', self.bitshuffle, self.compression)

    def config(self):
        return (self.sfc, self.compression, self.bwt, self.bitshuffle)

    def matches(self, filename=None, sfc=None, compression=None, bwt=None, bitshuffle=None):
        _filename = _sfc = _comp = _bwt = _bs = True
        if filename is not None:
            _filename = self.filename == filename
        if sfc is not None:
            _sfc = self.sfc == sfc
        if compression is not None:
            _comp = self.compression == compression
        if bwt is not None:
            _bwt = self.bwt == bwt
        if bitshuffle is not None:
            _bs = self.bitshuffle == bitshuffle
        return _filename and _sfc and _comp and _bwt and _bs


class Results:
    def __init__(self, runtime_compress=None, runtime_decompress=None, size=None):
        self.runtime_compress = runtime_compress
        self.runtime_decompress = runtime_decompress
        self.size = size

    def __eq__(self, other):
        return (self.runtime_compress, self.runtime_decompress, self.size) == (other.runtime_compress, other.runtime_decompress, other.size)

    def __hash__(self):
        return hash((self.runtime_compress, self.runtime_decompress, self.size))

    def __str__(self):
        return '%d ms, %d ms, %d bytes' % (-1 if self.runtime_compress is None else self.runtime_compress, -1 if self.runtime_decompress is None else self.runtime_decompress, -1 if self.size is None else self.size)


class SFCCBuilder:
    def __init__(self):
        self.filename = None
        self.sfc = SFC.raster
        self.compression = Compression.none
        self.bwt = False
        self.bitshuffle = False

    def setFilename(self, var):
        vars = var.split('.')
        if 'sfcc' in vars:
            vars.remove('sfcc')
        if 'tif' in vars:
            vars.remove('tif')
        self.filename = '.'.join(vars)

    def setSFC(self, var):
        if isinstance(var, str):
            self.sfc = GetSFC(var)
        elif isinstance(var, SFC):
            self.sfc = var
        else:
            raise RuntimeError('Unsupported SFC type: ', type(var))

    def setCompression(self, var):
        if isinstance(var, str):
            self.compression = GetCompression(var)
        elif isinstance(var, Compression):
            self.compression = var
        else:
            raise RuntimeError('Unsupported Compression type: ', type(var))

    def setBWT(self):
        self.bwt = True

    def unsetBWT(self):
        self.bwt = False

    def setBitShuffle(self):
        self.bitshuffle = True

    def unsetBitShuffle(self):
        self.bitshuffle = False

    def build(self):
        if any([v is None for v in [self.filename, self.sfc, self.compression, self.bwt, self.bitshuffle]]):
            raise 'Not all variables were set in builder'

        return SFCC(self.filename, self.sfc, self.bwt, self.bitshuffle, self.compression)


__BWT = ['bwt', 'bzip_lz77', 'bzip_lzw']
__BITSHUFFLE = ['btr', '-b']


def parseFilename(filename):

    filename_components = filename.split('.')

    builder = SFCCBuilder()

    # Filename
    try:
        sfcc_pos = filename_components.index('sfcc')
    except:
        raise RuntimeError('\'sfcc\' not present in filename:',
                           filename)
    builder.setFilename('.'.join(filename_components[:sfcc_pos+1]))
    del filename_components[:sfcc_pos+1]

    parts = [v.lower() for v in filename_components]
    for part in parts:
        _p = False
        if part in __SFC:
            builder.setSFC(part)
            _p = True
        if part in __COMPRESSON:
            builder.setCompression(part)
            _p = True
        if part in __BWT:
            builder.setBWT()
            _p = True
        if part in __BITSHUFFLE:
            builder.setBitShuffle()
            _p = True
        if not _p:
            raise RuntimeError('Unexpected filename part: %s' % (part))

    return builder.build()


def parseCLI(cli: str):
    components = cli.split(' ')
    filename = basename(components[-1])
    del components[-1]
    sfcc = parseFilename(filename)

    parts = [v.lower() for v in components]
    # print('Before:', sfcc)
    # print(parts)
    for part in parts:
        _p = False
        if part in __SFC:
            sfcc.sfc = GetSFC(part)
            _p = True
        if part in __COMPRESSON:
            sfcc.compression = GetCompression(part)
            _p = True
        if part in __BWT:
            sfcc.bwt = True
            _p = True
        if part in __BITSHUFFLE:
            sfcc.bitshuffle = True
            _p = True
        # if not _p:
        #     print('Unexpected cli part: %s' % (part))

    return sfcc
