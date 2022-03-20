FROM ubuntu:20.04
LABEL maintainer="Urs Spiegelhalter <urs.sp99@gmail.com>"

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y tzdata

RUN unlink /etc/localtime
RUN ln -s /usr/share/zoneinfo/Europe/Berlin /etc/localtime

# Install some basic utilities
RUN apt-get update && apt-get install -y \
    build-essential \
    make \
    cmake \
    wget \
    zip \
    sudo \
    libboost-all-dev \
    libbz2-dev \
 && rm -rf /var/lib/apt/lists/*

# Install expat
WORKDIR /tmp
RUN wget https://github.com/libexpat/libexpat/archive/refs/tags/R_2_4_1.zip
RUN unzip R_2_4_1.zip
WORKDIR  /tmp/libexpat-R_2_4_1/expat
RUN mkdir build
WORKDIR /tmp/libexpat-R_2_4_1/expat/build
RUN cmake ..
RUN sudo make install -j4
WORKDIR /tmp
RUN rm -rf R_2_4_1.zip libexpat-R_2_4_1

# Install protozero
WORKDIR /tmp
RUN wget https://github.com/mapbox/protozero/archive/refs/tags/v1.7.0.zip
RUN unzip v1.7.0.zip
WORKDIR  /tmp/protozero-1.7.0
RUN mkdir build
WORKDIR /tmp/protozero-1.7.0/build
RUN cmake ..
RUN sudo make install -j4
WORKDIR /tmp
RUN rm -rf v1.7.0.zip protozero-1.7.0

# Install zlib
WORKDIR /tmp
RUN wget https://www.zlib.net/zlib-1.2.11.tar.gz
RUN tar xf zlib-1.2.11.tar.gz
WORKDIR  /tmp/zlib-1.2.11
RUN mkdir build
WORKDIR /tmp/zlib-1.2.11/build
RUN cmake ..
RUN sudo make install -j4
WORKDIR /tmp
RUN rm -rf zlib-1.2.11.tar.gz zlib-1.2.11

# Install libosmium
WORKDIR /tmp
RUN wget https://github.com/osmcode/libosmium/archive/refs/tags/v2.17.1.zip
RUN unzip v2.17.1.zip
WORKDIR  /tmp/libosmium-2.17.1
RUN mkdir build
WORKDIR /tmp/libosmium-2.17.1/build
RUN cmake ..
RUN sudo make install -j4
WORKDIR /tmp
RUN rm -rf v2.17.1.zip libosmium-2.17.1

# Install libzip
WORKDIR /tmp
RUN wget https://github.com/nih-at/libzip/archive/refs/tags/v1.8.0.zip
RUN unzip v1.8.0.zip
WORKDIR  /tmp/libzip-1.8.0
RUN mkdir build
WORKDIR /tmp/libzip-1.8.0/build
RUN cmake .. -DENABLE_COMMONCRYPTO=OFF -DENABLE_GNUTLS=OFF -DENABLE_MBEDTLS=OFF -DENABLE_OPENSSL=OFF
RUN sudo make
RUN make install -j4
WORKDIR /tmp
RUN rm -rf v1.8.0.zip libzip-1.8.0

# Create a working directory
RUN mkdir /OsmElevation
WORKDIR /OsmElevation

# Create a non-root user and switch to it
RUN adduser --disabled-password --gecos '' --shell /bin/bash user \
 && chown -R user:user /OsmElevation
RUN echo "user ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/90-user
USER user

# All users can use /home/user as their home directory
ENV HOME=/home/user
RUN chmod 777 /home/user

COPY . .
RUN mkdir build
WORKDIR /OsmElevation/build
RUN cmake ..
RUN make -j4
WORKDIR /OsmElevation
