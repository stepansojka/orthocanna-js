#!/bin/bash

yum install -y libcurl-devel gcc gcc-c++ make libtool python3  hwloc-devel numactl-devel libpciaccess-devel libxml2-devel xfsprogs-devel gnutls-devel lksctp-tools-devel lz4-devel protobuf-devel protobuf-compiler systemtap-sdt-devel ninja-build libatomic

function get_sources() {
    curl -Ls "$1" | tar xz -C /usr/local/src
}

get_sources https://github.com/Kitware/CMake/releases/download/v3.13.2/cmake-3.13.2.tar.gz
cd /usr/local/src/cmake-3.13.2 && ./bootstrap --system-curl && make && make install

get_sources https://ftp.gnu.org/gnu/stow/stow-2.2.2.tar.gz
cd /usr/local/src/stow-2.2.2 && ./configure && make && make install
