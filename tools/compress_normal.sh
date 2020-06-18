#!/bin/bash

########################################################################
# Runs the appropriate command-line compression tool on the given file #
########################################################################


## COMPRESSION FUNCTIONS ##
comp_gzip(){
    gzip -k -9 -f $1;
}

comp_bzip2(){
    bzip2 -z -k -9 -f $1;
}

comp_lz4(){
    lz4 -9 -f $1 $1.lz4;
}

comp_lzo(){
    lzop -9 -f -o $1.lzo $1
}

## DECOMPRESSION FUNCTIONS ##
decomp_gzip(){
    gzip -d -k -f $1;
}

decomp_bzip2(){
    bzip2 -d -k -f $1;
}

decomp_lz4(){
    lz4 -d -f $1 $(dirname $1)/$(basename -s .lz4 $1);
}

decomp_lzo(){
    lzop -d -f -o $(dirname $1)/$(basename -s .lzo $1) $1;
}



while getopts c:df: flag
do
    case "${flag}" in
        c) compression=${OPTARG};;
        d) decompression=true;;
        f) file=${OPTARG};;
    esac
done

[[ ! -z "$compression" ]] && [[ ! -z "$decompression" ]] && echo "-c and -d are mutually exclusive" && exit 1;
[[ -z "$file" ]] && echo "File must be provided!" && exit 1;

if [[ ! -z "$compression" ]]
then
    case "${compression}" in
        GZIP) comp_gzip $file;;
        BZIP2) comp_bzip2 $file;;
        LZ4) comp_lz4 $file;;
        LZO) comp_lzo $file;;
        *) echo "Compression scheme not known" && exit 1;;
    esac
else
    decompression_type="${file##*.}"
    case "${decompression_type}" in
        gz) decomp_gzip $file;;
        bz2) decomp_bzip2 $file;;
        lz4) decomp_lz4 $file;;
        lzo) decomp_lzo $file;;
        *) echo "Decompression scheme not known: ${decompression_type}" && exit 1;;
    esac
fi
