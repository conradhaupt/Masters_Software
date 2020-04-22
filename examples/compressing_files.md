# Compressing files using sfccompress
## _EXAMPLE_

There are three sfcc files present in `examples/files` that can be used for in
this example. The first two are from the SKA HDF5 dataset used in the paper
titled: _Improving Lossless Compression Performance Of HDF5 SKA Data Using
N-Dimensional Re-Ordering_. The third file is from the SRTM30 dataset used in
the paper titled: _Evaluating the Effect of Space-Filling Curve Pre-Processing
on Digital Elevation Map Compression_.

```txt
1518913830_0_768_128.sfcc
1518913830_512_3200_256.sfcc
s26_e028_1arc_v3.sw.sfcc
```

Ensure that sfccompress is built. It is assumed that sfccompress is either in
`PATH`. If not, you will need to alter the command line statements in this
example to point to the correct location of `sfccompress`.

`sfccompress` has two modes: compress and decompress. Compress takes in a
standard sfcc file and applies a SFC, bitshuffle (optional), and a compression
technique. Decompress reverses all applicable mappings and compression algorithms
and outputs an uncompressed, raster sfcc.

Let's compress the first file using sfccompress. Run the following in a Linux
terminal.

```bash
sfccompress compress -s HILBERT -c HUFFMAN 1518913830_0_768_128.sfcc
```

You should now have a file in `examples/files/` called `1518913830_0_768_128.sfcc.hbrt.hff`.
If you run `sfccinfo.py`, in the `tools` subdirectory, you would get the
following output.

```txt
########################################
#    File details
########################################
# Number of Dimensions:	3
# Sidelength:		128	[2^7]
# DType bytes:	4
# SFC Type:		Hilbert
# CompressionType:	Huffman
########################################
#
# Samples of data [3355446 3355462]
# ... 2D 4D 4B 3C 36 85 CE 67 14 CA 52 07 90 29 34 A2 ...
########################################
```

This tells us about what the compressed file contains. Firstly, the data represents
values in three-dimensions in a hyper-cube with sidelength of 128. The values are
4 bytes each, single-precision floating-point (known from the input data source).
It was mapped using the Hilbert Curve and is compressed using Huffman Encoding.
Some sample bytes are also listed in hexadecimal.

We can verify that the compression is lossless by decompressing the same file.
An easy way to do this, and the way the results in this research were verified,
is to decompress the compressed file and compare the hashsums of the original and
new uncompressed files. The following code accomplishes this.

```bash
# Backup the original uncompressed file to be safe
cp s26_e028_1arc_v3.sw.sfcc s26_e028_1arc_v3.sw.sfcc.original

# Create a SHA1 checksum file
sha1sum s26_e028_1arc_v3.sw.sfcc > s26_e028_1arc_v3.sw.sfcc.sha1

# Compress the file using sfccompress
sfccompress compress -s MORTON -c LZ77 s26_e028_1arc_v3.sw.sfcc

# Decompress the compressed file, overwriting the original file
# but keeping the *.original file.
sfccompress decompress s26_e028_1arc_v3.sw.sfcc.mrtn.lz77

# Compare the new uncompressed file with the stored hash
sha1sum --check s26_e028_1arc_v3.sw.sfcc.sha1
```

You should receive an output similar that given below.

```txt
s26_e028_1arc_v3.sw.sfcc: OK
```

This tells us that the SHA1 hashsum of the decompressed file is the same as the
original input, implying that there was no data-loss in the process. Given that
the compressed file is 48% the size as the original, the combination of
Z-Order/Morton Curve and LZ77 achieved a compression ratio of **2.07**.

Some compression techniques and standards were not integrated into sfccompress
and instead standard Linux tools were used. These include BZIP2, GZIP, LZ4, and
LZO. To accomplish this, some sfccompress options exist to only apply
preprocessing steps and not apply any compression.

Applying `-c NONE` will only apply the SFC mapping to the input data whereas
`-c BWT` will apply the Burrows-Wheeler Transform and Move-to-Front Encoding. By
compressing the output files using the Linux tools for the aforementioned
compression algorithms, compression-ratios can be acquired using those optimised
compression algorithms.

An example of applying BWT and GZIP to a file is given below.

```bash
# Map the input file
sfccompress compress -s GRAY -c BWT 1518913830_512_3200_256.sfcc

# Compress using GZIP
gzip -9 -k 1518913830_512_3200_256.sfcc.gry.bwt
```

The output file is called 1518913830_512_3200_256.sfcc.gry.bwt.gz and measures
in 8282605 bytes. This compressed size can be drastically reduced if bitshuffle
is used, as shown below.

```bash
# Map the input file with bitshuffle
sfccompress compress -s GRAY -c BWT -b 1518913830_512_3200_256.sfcc

# Compress using GZIP
gzip -9 -k 1518913830_512_3200_256.sfcc.gry.btr.bwt
```

The resulting Gray-Code Curve mapped, bitshuffled, Burrows-Wheeler Transformed,
GZIPed file measures as 7985433 bytes.
