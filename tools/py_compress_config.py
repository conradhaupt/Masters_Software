#!/usr/bin/python3

import sys
import sfccfile as sfcc
from sfccfile import SFC, Compression, Config, SFCC, Results
import itertools as it
from anytree import Node, RenderTree, AsciiStyle, PreOrderIter

if len(sys.argv) < 4:
    print(
        'py_compress_config.py <raster|zorder|gray|hilbert> <output_filename> [files]')
    exit(1)

sfc = sfcc.GetSFC(sys.argv[1])
arg_output = sys.argv[2]
files = sys.argv[3:]
print(f"Processing {len(files)} files with {sfc}")
print(f"Saving output to {arg_output}")

root_name = Config(SFC.raster, False, False, Compression.none).toShortStr()
root = Node(root_name)
nodes = dict()
configs = dict()
nodes[Config(SFC.raster, False, False, Compression.none).toShortStr()] = root
configs[Config(SFC.raster, False, False, Compression.none).toShortStr()] = Config(
    SFC.raster, False, False, Compression.none)
encountered = set()
encountered.add(root_name)

for sfc, comp, bwt, bitshuffle in it.product(SFC, Compression, [True, False], [True, False]):
    conf = Config(sfc, bwt, bitshuffle, comp)
    if not conf.valid():
        continue
    conf_name = conf.toShortStr()
    if conf_name in encountered:
        continue
    prev_conf = conf.previousConfig()
    _node = Node(conf_name, parent=nodes[prev_conf.toShortStr()])
    nodes[conf_name] = _node
    configs[conf_name] = conf
    encountered.add(conf_name)

ignore_root = True

print(RenderTree(root, style=AsciiStyle()).by_attr())
with open(arg_output, 'w') as out:
    for node in PreOrderIter(root):
        if ignore_root and node.name == root_name:
            print(f"Ignoring root node with name {node.name}")
            continue
        conf = configs[node.name]
        _sfc, _bwt, _bs, _comp = conf.pyCompressArg()
        # Compress
        _line = ['compress', _sfc, _bwt, _bs, _comp]
        for f in files:
            out.write(' '.join(_line) + ' ' + f + '\n')

        _line = ['decompress', _sfc, _bwt, _bs, _comp]
        for f in files:
            out.write(' '.join(_line) + ' ' + f + '\n')
