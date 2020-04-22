# Tools

These Python3 tools allow the generation of sfcc files and their analysis.

## Descriptions

- geotiff2sfcc.py

Converts GeoTIFF files into sfcc files. It is assumed that the input file is square and has power-of-two sidelengths.

- hdf52sfcc.py

Converts the SKA HDF5 file used in this research into 3D sfcc files.

- sfcc_stats.py

Calculates some statistics for sfcc files: e.g. mean values, standard deviation.

- sfccinfo.py

Prints information about sfcc files.

## Dependencies

- scipy
- progress
- pandas
- numpy
- joblib
- h5py
- georaster: [https://github.com/atedstone/georaster](https://github.com/atedstone/georaster)
