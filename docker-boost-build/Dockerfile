FROM ubuntu:bionic as build

LABEL description="Build boost"

RUN apt-get update && apt-get install -y \
    binutils cmake curl gcc g++ git libtool make tar

RUN apt-get install wget

RUN wget http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz
RUN tar -xf boost_1_58_0.tar.gz
RUN cd boost_1_58_0/; \
    ./bootstrap.sh --prefix=/usr/; 

RUN cd boost_1_58_0/; ./b2 install; exit 0

RUN ldconfig
