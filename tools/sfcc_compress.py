#!/usr/bin/python3

import os
import os.path
import sys
import sfccfile as sfcc
from sfccfile import SFC, Compression, Config, SFCC, Results
import itertools as it

for sfc,comp,bwt,bitshuffle in it.product(SFC,Compression,[True,False],[True,False]):
    conf = Config(sfc,comp,bwt,bitshuffle)
    if not conf.valid():
        print(conf)