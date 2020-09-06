# from sfcc_name_to_info import sfcc_name_to_info
from sfccfile import SFCC, SFC, Compression, Results, SFCCBuilder, parseFilename, parseCLI, SFCToStr, CompressionToStr
from os.path import join, basename
import sys
from time import time
from copy import deepcopy as copy
from itertools import product
from sfcc_ignore import FILENAMES_TO_IGNORE

__data_csv_ska_filename = 'data_ska.csv'
__data_csv_srtm_filename = 'data_srtm.csv'
__data_csv_header = 'Bytes,Compression Runtime,Decompression Runtime,Relative Compression Runtime,Relative Decompression Runtime,Name,SFC,Compression,BWT,BitShuffle\n'


def isOriginalEntry(entry):
    return False
    if entry.sfc == SFC.raster and entry.compression == Compression.none and not entry.bwt and not entry.bitshuffle:
        return True
    else:
        return False


def summarise_configs(entries):
    curves = dict()
    compression = dict()
    bitshuffle = dict()
    bwt = dict()
    filenames = dict()
    configs = dict()

    # Preload all configs
    for s, c, b, bs in product(SFC, Compression, [False, True], [True]):
        # CONFIGS TO IGNORE
        if b and (c != Compression.lz4 and c != Compression.lz77 and c != Compression.lzw):
            continue
        # elif
        configs[(s, c, b, bs)] = 0

    for e in entries:
        if e.sfc in curves:
            curves[e.sfc] += 1
        else:
            curves[e.sfc] = 1
        if e.compression in compression:
            compression[e.compression] += 1
        else:
            compression[e.compression] = 1
        if e.bitshuffle in bitshuffle:
            bitshuffle[e.bitshuffle] += 1
        else:
            bitshuffle[e.bitshuffle] = 1
        if e.bwt in bwt:
            bwt[e.bwt] += 1
        else:
            bwt[e.bwt] = 1
        if e.filename in filenames:
            filenames[e.filename] += 1
        else:
            filenames[e.filename] = 1
        if e.config() in configs:
            configs[e.config()] += 1
        else:
            configs[e.config()] = 1
    return (filenames, curves, compression, bitshuffle, bwt, configs)


def process_parallel_log(parallel_filename, add, header=False, print_failed_jobs=False):
    with open(parallel_filename, 'r') as f_parallel:
        # Parallel log contains a header line
        if header:
            _ = f_parallel.readline()
        lines = f_parallel.readlines()
        for line in lines:
            components = line.strip().split('\t')
            job_number = components[0]
            runtime = components[3]
            exitval = components[6]
            signal = components[7]
            cli = components[8]
            # Log failed jobs
            if exitval != '0' or signal != '0':
                if __print_failed_jobs or print_failed_jobs:
                    print('Found failed job no. %s' % (job_number))
                continue

            sfcc = parseCLI(cli)
            add(sfcc, runtime)


def printHelpAndExit():
    print(
        'sfcc_process.py <du_results.out> <parallel_compress_log> <parallel_decompress_log>')
    exit(1)


if __name__ == '__main__':
    if len(sys.argv) != 4:
        printHelpAndExit()

    __print_failed_jobs = False

    arg_du_results = sys.argv[1]
    arg_parallel_compress_jobs = sys.argv[2]
    arg_parallel_decompress_jobs = sys.argv[3]

    entries = dict()

    sfccs_to_sum = set()

    # Load du_results.out data
    print('Loading filesize results')
    with open(arg_du_results, 'r') as f_du_results:
        lines = f_du_results.readlines()
        for line in lines:
            size, filename = line.strip().split('\t')
            size = int(size)
            sfcc = parseFilename(basename(filename))
            entries[sfcc] = Results(size=size)

    # Load runtime from parallel compress jobs
    print('Loading compression runtime results')
    with open(arg_parallel_compress_jobs, 'r') as f_parallel_compress_jobs:
        # Parallel log contains a header line
        _ = f_parallel_compress_jobs.readline()
        lines = f_parallel_compress_jobs.readlines()
        for line in lines:
            components = line.strip().split('\t')
            job_number = components[0]
            runtime = components[3]
            exitval = components[6]
            signal = components[7]
            cli = components[8]
            # Log failed jobs
            if exitval != '0' or signal != '0':
                if __print_failed_jobs:
                    print('Found failed job no. %s' % (job_number))
                continue

            sfcc = parseCLI(cli)

            # If the compression scheme is external, make sure to sum the timings later
            if sfcc.compression.external():
                sfccs_to_sum.add(sfcc)

            if sfcc in entries:
                # sfcc exists
                if entries[sfcc].runtime_compress is None:
                    entries[sfcc].runtime_compress = int(float(runtime) * 1000)
                else:
                    raise RuntimeError(
                        'Already existing compression runtime: %s' % (cli))
            else:
                raise RuntimeError(
                    "Found an unexpected SFCC: %s" % (sfcc), cli)

        # Load runtime from parallel decompress jobs
    print('Loading decompression runtime results')
    with open(arg_parallel_decompress_jobs, 'r') as f_parallel_decompress_jobs:
        # Parallel log contains a header line
        _ = f_parallel_decompress_jobs.readline()
        lines = f_parallel_decompress_jobs.readlines()
        for line in lines:
            components = line.strip().split('\t')
            job_number = components[0]
            runtime = components[3]
            exitval = components[6]
            signal = components[7]
            cli = components[8]
            # Log failed jobs
            if exitval != '0' or signal != '0':
                if __print_failed_jobs:
                    print('Found failed job no. %s' % (job_number))
                continue

            sfcc = parseCLI(cli)

            if sfcc.compression.external():
                sfccs_to_sum.add(sfcc)

            if sfcc in entries:
                # sfcc exists
                if entries[sfcc].runtime_decompress is None:
                    entries[sfcc].runtime_decompress = int(
                        float(runtime) * 1000)
                else:
                    raise RuntimeError(
                        'Already existing decompression runtime: %s' % (cli))
            else:
                raise RuntimeError("Found an unexpected SFCC: %s" % (sfcc))

    print('Removing files to ignore')
    entries = dict([(e, r) for e, r in entries.items()
                    if e.filename not in FILENAMES_TO_IGNORE])

    # Check if all SFCC files have a runtime and size value
    found_incomplete_sfcc = False
    for sfcc, results in entries.items():
        if (results.runtime_compress is None or results.runtime_decompress is None or results.size is None) and not isOriginalEntry(sfcc):
            print(sfcc, results, isOriginalEntry(sfcc))
            found_incomplete_sfcc = True
    # if found_incomplete_sfcc:
    #     exit(1)

    print('Removing files to ignore')
    entries = dict([(e, r) for e, r in entries.items()
                    if e.filename not in FILENAMES_TO_IGNORE])

    #############################################
    ##  COMBINE INTERNAL AND EXTERNAL RUNTIME  ##
    #############################################
    print('Combining external times with internal times')
    for sfcc in sfccs_to_sum:
        if sfcc not in entries:
            raise RuntimeError("Cannot find SFCC in entries: %s" % (sfcc))
        original_sfcc = copy(sfcc)
        original_sfcc.compression = Compression.none

        if original_sfcc not in entries:
            raise RuntimeError(
                "Cannot find original SFCC in entries: %s" % (original_sfcc))

        original_results = entries[original_sfcc]
        if entries[sfcc].runtime_compress is not None and original_results.runtime_compress is not None:
            entries[sfcc].runtime_compress += original_results.runtime_compress
        if entries[sfcc].runtime_decompress is not None and original_results.runtime_decompress is not None:
            entries[sfcc].runtime_decompress += original_results.runtime_decompress

    ###################################
    ##  CALCULATE RELATIVE RUNTIMES  ##
    ###################################
    print('Calculating relative runtimes')
    for sfcc, results in entries.items():
        # Get the input sfcc
        input_sfcc = copy(sfcc)
        if sfcc.compression != Compression.none:
            input_sfcc.compression = Compression.none
        else:
            input_sfcc.sfc = SFC.raster
            input_sfcc.bwt = False
            input_sfcc.bitshuffle = False

        input_results = entries[input_sfcc]
        # Calculate relative runtime
        results.relative_runtime_compress = results.runtime_compress - \
            input_results.runtime_compress
        results.relative_runtime_decompress = results.runtime_decompress - \
            input_results.runtime_decompress

        # Reassign entries results
        entries[sfcc] = results

    with open(__data_csv_ska_filename, 'w') as csv:
        csv.write(__data_csv_header)
        for sfcc, results in entries.items():
            if '1518913830' in sfcc.filename:
                csv.write(
                    ','.join([str(x) for x in [results.to_csv(), sfcc.filename, SFCToStr(sfcc.sfc), CompressionToStr(sfcc.compression), sfcc.bwt, sfcc.bitshuffle]]) + '\n')

    with open(__data_csv_srtm_filename, 'w') as csv:
        csv.write(__data_csv_header)
        for sfcc, results in entries.items():
            if results.isIncomplete():
                print(f"Results from {sfcc} are incomplete: {results}")
            if '1518913830' not in sfcc.filename:
                csv.write(
                    ','.join([str(x) for x in [results.to_csv(), sfcc.filename, SFCToStr(sfcc.sfc), CompressionToStr(sfcc.compression), sfcc.bwt, sfcc.bitshuffle]]) + '\n')

    filenames, curves, compression, bitshuffle, bwt, configs = summarise_configs(
        entries)
    for c, n in configs.items():
        if n != 1788:
            print(c, n)
