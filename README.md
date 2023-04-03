# setup using docker

## step0 : create a custom bridge network
```
$ sudo docker network create --subnet=172.18.0.0/16 vsomeip_bridge
```

## step1 : setup boost image
it will take a while
```bash
$ cd docker-boost-build
$ docker build -t boost_build:v0 .
```

## step2 : setup vsomeip image
```bash
$ cd docker-vsomeip-build
$ docker build -t vsomeip_build:v0 .
```

## step3 : build vsomeip service image
```bash
$ cd docker-vsomeip-service
$ docker build -t vsomeip_service:v0 .
```

## step3 : build vsomeip client image
```bash
$ cd docker-vsomeip-client
$ docker build -t vsomeip_client:v0 .
```

## run_step1 : run vsomeip service container
```bash
$ docker run --net vsomeip_bridge --ip 172.18.0.2 vsomeip_service:v0
```

## run_step2 : run vsomeip client container
```bash
$ docker run --net vsomeip_bridge --ip 172.18.0.3 vsomeip_client:v0
```
should be good to go :)

# Setting up vsome-ip enviroment with example

## setp1: vsomeIp requires Boost.asio
```bash
#download boost 1.58
$ sudo apt-get update
$ sudo apt-get install net-tools cmake -y
$ sudo apt-get install build-essential
$ wget http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz
$ tar -xf boost_1_58_0.tar.gz
$
$ cd boost_1_58_0/
$ ./bootstrap.sh --prefix=/usr/
$ sudo ./b2 install
```

## setp2: get VsomeIp
```bash
#vsomeip
$ git clone http://github.com/GENIVI/vSomeIP.git
$ cd vSomeIP
$ git checkout tags/2.14.16
$ mkdir build
$ cd build
$ cmake ..
$ make
```
## step3: add vsomeip libs path to .bashrc
```bash
$ sudo nano ~/.bashrc
```
```bash
# add to your .bashrc
# the path of (libvsomeip.so)
export LD_LIBRARY_PATH=~/SOMEIP/vSomeIP/build${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
```
## step4: try test project
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## step5,6: run client and service examples in different terminals
```bash
$ ./service-example
```
```bash
$ ./client-example
```



