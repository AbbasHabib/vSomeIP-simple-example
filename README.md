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
$ sudo ./b2
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
$ make -j
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



