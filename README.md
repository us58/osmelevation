# osmelevation
`osmelevation` annotates OpenStreetMap data with external elevation data from the NASA Digital Elevation Model (NASADEM).
`correctosmelevation` performs corrections on the elevation data in OpenStreetMap data.

## Getting started

### Docker

Build and run the Docker image with
```
$ docker build -t osmelevation -f Dockerfile .
$ docker run -it -v <path to input data>/:/OsmElevation/extern/ --name osmelevation osmelevation
```

where "path to input data" contains a folder with NASADEM files and OSM files where the elevation data should be added to.
In the docker image, this data will be accessible in extern/

### Without Docker

On Ubuntu 20.04 the following libraries and packages are required to build `osmelevation` and `correctosmelevation`:
```
g++ libboost-all-dev libbz2-dev cmake make
```
+ [Libosmium](https://osmcode.org/libosmium/)
	+ [Expat](https://github.com/libexpat/libexpat)
	+ [protozero](https://github.com/mapbox/protozero)
	+ [zlib](https://www.zlib.net/)
+ [libzip](https://libzip.org/)

Build `osmelevation` and `correctosmelevation`:
```
$ mkdir build && cd build && cmake .. && make -j4 && cd ..
```

## Usage

For the first tool `osmelevation`, prepare a folder containing NASADEM files and an OSM input file.
Elevation data can be added to an OSM input file by calling
```
$ ./build/osmelevation <NASADEM files directory> <OSM input file> <OSM output file>
```

For the second tool `correctosmelevation`, an with elevation data annotated OSM input file is needed.
The elevation data in an OSM input file can be corrected by calling
```
$ ./build/correctosmelevation <OSM input file> <OSM output file>
```

Assess `./build/osmelevation` and `./build/correctosmelevation` for further options.

## Important remark

For the second tool `correctosmelevation`, complete OSM relations with tag type=route and waterway=river must be present.
Otherwise, no corrections will be performed on these (incomplete) relations.
When creating geographic extracts with tools like osmium, options must be chosen to not clip relations with these tags at the borders of the extracts. 

## Obtaining NASADEM files

All available NASADEM files are downloadable from the [LP DAAC Data Pool](https://e4ftl01.cr.usgs.gov/MEASURES/NASADEM_HGT.001/2000.02.11/).
A free Earthdata account is required. For bulk downloads, follow the [official instructions](https://disc.gsfc.nasa.gov/data-access) provided by NASA.
A txt file containing all NASADEM files urls is provided with `nasadem_urls.txt`.