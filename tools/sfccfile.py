from enum import Enum
from os.path import basename


def noneToNegativeOne(val):
    return val if val is not None else -1


def UnL(arr):
    output = []
    for a in arr:
        output.append(a.lower())
        output.append(a.upper())
    return output


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


def SFCToExtension(sfc):
    if sfc == SFC.raster:
        return ''
    elif sfc == SFC.zorder:
        return '.mrtn'
    elif sfc == SFC.gray:
        return '.gry'
    elif sfc == SFC.hilbert:
        return '.hbrt'
    else:
        return sfc


__RASTER = UnL(['raster', 'row_major', 'rstr'])
__ZORDER = UnL(['zorder', 'morton', 'mrtn'])
__GRAY = UnL(['gray', 'gray_code', 'gry'])
__HILBERT = UnL(['hilbert', 'hbrt'])

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

    def external(self):
        return self in [Compression.bzip2, Compression.gzip, Compression.lz4, Compression.lzo]


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


def CompressionToExtension(comp):
    if comp == Compression.none:
        return ''
    elif comp == Compression.bzip2:
        return '.bz2'
    elif comp == Compression.gzip:
        return '.gz'
    elif comp == Compression.huffman:
        return '.hff'
    elif comp == Compression.lz4:
        return '.lz4'
    elif comp == Compression.lz77:
        return '.lz77'
    elif comp == Compression.lzo:
        return '.lzo'
    elif comp == Compression.lzw:
        return '.lzw'
    elif comp == Compression.rle:
        return '.rle'


__HUFFMAN = UnL(['huffman', 'huff', 'hff'])
__GZIP = UnL(['gzip', 'gz'])
__BZIP2 = UnL(['bzip', 'bzip2', 'bz2'])
__LZ4 = UnL(['lz4'])
__LZ77 = UnL(['lz77', 'bzip_lz77'])
__LZO = UnL(['lzo'])
__LZW = UnL(['lzw', 'bzip_lzw'])
__RLE = UnL(['rle'])
__NONE = UnL(['none'])

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


class Config:

    __VALID_BWT_COMPRESSION = [Compression.none, Compression.lz77,
                               Compression.lzw, Compression.lz4]

    def __init__(self, sfc: SFC, bwt: bool, bitshuffle: bool, compression: Compression):
        if isinstance(sfc, str):
            self.sfc = GetSFC(sfc)
        else:
            self.sfc = sfc

        self.bwt = bwt
        self.bitshuffle = bitshuffle
        if isinstance(compression, str):
            self.compression = GetCompression(compression)
        else:
            self.compression = compression

        # if not self.valid():
        #     raise RuntimeError('Invalid Config')

    def valid(self):
        if self.bwt and self.compression not in self.__VALID_BWT_COMPRESSION:
            return False
        return True

    def expectedFilename(self, filename):
        _filename = filename.split('.sfcc')[0] + '.sfcc'
        _filename += SFCToExtension(self.sfc)
        if self.bitshuffle:
            _filename += '.btr'
        if self.bwt:
            _filename += '.bwt'
        _filename += CompressionToExtension(self.compression)
        return _filename

    def __str__(self):
        return '{}, {} {}, {} {}, {}'.format(self.sfc, 'bwt:', self.bwt, 'bitshuffle:', self.bitshuffle, self.compression)

    def previousConfig(self):
        # If the config requires an external compression tool, the input config is as follows
        if self.compression.external():
            return Config(self.sfc, self.bwt, self.bitshuffle, Compression.none)

        # The config is the output from sfccompress, therefore the input config is the original file
        return Config(SFC.raster, False, False, Compression.none)

    def isOriginal(self):
        return self.sfc == SFC.raster and not self.bwt and not self.bitshuffle and self.compression == Compression.none

    def toShortStr(self):
        return self.expectedFilename('.sfcc')

    def sfccompressArgs(self):
        if self.sfc == SFC.raster:
            _sfc = 'ROW_MAJOR'
        elif self.sfc == SFC.zorder:
            _sfc = 'MORTON'
        elif self.sfc == SFC.gray:
            _sfc = 'GRAY_CODE'
        elif self.sfc == SFC.hilbert:
            _sfc = 'HILBERT'
        else:
            _sfc = None

        if self.compression == Compression.none and not self.bwt:
            _comp = 'NONE'
        elif self.compression == Compression.none and self.bwt:
            _comp = 'BWT'
        elif self.compression == Compression.huffman:
            _comp = 'HUFFMAN'
        elif self.compression == Compression.rle:
            _comp = 'RLE'
        elif self.compression == Compression.lz77 and not self.bwt:
            _comp = 'LZ77'
        elif self.compression == Compression.lz77 and self.bwt:
            _comp = 'BZIP_LZ77'
        elif self.compression == Compression.lzw and not self.bwt:
            _comp = 'LZW'
        elif self.compression == Compression.lzw and self.bwt:
            _comp = 'BZIP_LZW'
        else:
            _comp = None

        if self.bitshuffle:
            _bs = '-b'
        else:
            _bs = '-B'

        return (_sfc, _comp, _bs)

    def pyCompressArg(self):
        if self.sfc == SFC.raster:
            _sfc = 'raster'
        elif self.sfc == SFC.zorder:
            _sfc = 'zorder'
        elif self.sfc == SFC.gray:
            _sfc = 'gray'
        elif self.sfc == SFC.hilbert:
            _sfc = 'hilbert'
        else:
            _sfc = None

        if self.compression == Compression.none:
            _comp = 'none'
        elif self.compression == Compression.huffman:
            _comp = 'huff'
        elif self.compression == Compression.rle:
            _comp = 'rle'
        elif self.compression == Compression.lz77:
            _comp = 'lz77'
        elif self.compression == Compression.lzw:
            _comp = 'lzw'
        elif self.compression == Compression.bzip2:
            _comp = 'bzip'
        elif self.compression == Compression.gzip:
            _comp = 'gzip'
        elif self.compression == Compression.lz4:
            _comp = 'lz4'
        elif self.compression == Compression.lzo:
            _comp = 'lzo'
        else:
            _comp = None

        if self.bitshuffle:
            _bs = 'btr'
        else:
            _bs = 'nobtr'

        if self.bwt:
            _bwt = 'bwt'
        else:
            _bwt = 'nobwt'

        return (_sfc, _bwt, _bs, _comp)


class SFCC:
    def __init__(self, filename: str, sfc, bwt: bool, bitshuffle: bool, compression):
        self.filename = filename
        self._config = Config(sfc, bwt, bitshuffle, compression)

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

    @property
    def sfc(self):
        return self._config.sfc

    @property
    def bwt(self):
        return self._config.bwt

    @property
    def bitshuffle(self):
        return self._config.bitshuffle

    @property
    def compression(self):
        return self._config.compression

    @filename.deleter
    def filename(self):
        del self._filename

    @sfc.setter
    def sfc(self, val):
        self._config.sfc = val

    @bwt.setter
    def bwt(self, val):
        self._config.bwt = val

    @bitshuffle.setter
    def bitshuffle(self, val):
        self._config.bitshuffle = val

    @compression.setter
    def compression(self, val):
        self._config.compression = val

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
    def __init__(self, runtime_compress=None, runtime_decompress=None, size=None, relative_runtime_compress=None, relative_runtime_decompress=None):
        self.runtime_compress = runtime_compress
        self.runtime_decompress = runtime_decompress
        self.size = size
        self.relative_runtime_compress = relative_runtime_compress
        self.relative_runtime_decompress = relative_runtime_decompress

    def __eq__(self, other):
        return (self.runtime_compress, self.runtime_decompress, self.size, self.relative_runtime_compress, self.relative_runtime_decompress) == (other.runtime_compress, other.runtime_decompress, other.size, other.relative_runtime_compress, other.relative_runtime_decompress)

    def __hash__(self):
        return hash((self.runtime_compress, self.runtime_decompress, self.size, self.relative_runtime_compress, self.relative_runtime_decompress))

    def __str__(self):
        return '%d ms, %d ms, %d bytes, %d ms, %d ms' % (-1 if self.runtime_compress is None else self.runtime_compress, -1 if self.runtime_decompress is None else self.runtime_decompress, -1 if self.size is None else self.size, -1 if self.relative_runtime_compress is None else self.relative_runtime_compress, -1 if self.relative_runtime_decompress is None else self.relative_runtime_decompress)

    def isIncomplete(self):
        return self.runtime_compress is None or self.runtime_decompress is None or self.size is None or self.relative_runtime_compress is None or self.relative_runtime_decompress is None

    def to_csv(self):
        return ','.join([str(noneToNegativeOne(x)) for x in [self.size, self.runtime_compress, self.runtime_decompress, self.relative_runtime_compress, self.relative_runtime_decompress]])


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


__BWT = UnL(['bwt', 'bzip_lz77', 'bzip_lzw'])
__BITSHUFFLE = ['btr', '-b', 'BTR']


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

    parts = [v for v in filename_components]
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

    parts = [v for v in components]
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
