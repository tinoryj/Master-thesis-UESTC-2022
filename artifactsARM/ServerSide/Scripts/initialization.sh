#!/bin/bash
# sudo apt update
# sudo apt dist-upgrade
# sudo apt autoremove
if [ ! -d "Packages" ]; then
  mkdir Packages
fi
sudo apt install build-essential cmake wget libssl-dev libcurl4-openssl-dev libprotobuf-dev libboost-all-dev libleveldb-dev libsnappy-dev
cd ./Packages/
echo "Download SGX Driver"
wget https://download.01.org/intel-sgx/sgx-linux/2.15.1/distro/ubuntu20.04-server/sgx_linux_x64_driver_1.41.bin
wget https://download.01.org/intel-sgx/sgx-linux/2.15.1/distro/ubuntu20.04-server/sgx_linux_x64_driver_2.11.0_2d2b795.bin
echo "Download SGX SDK"
wget https://download.01.org/intel-sgx/sgx-linux/2.15.1/distro/ubuntu20.04-server/sgx_linux_x64_sdk_2.15.101.1.bin
echo "Install SGX Driver & SDK"
chmod +x sgx_linux_x64_driver_1.41.bin
sudo ./sgx_linux_x64_driver_1.41.bin
chmod +x sgx_linux_x64_driver_2.11.0_2d2b795.bin
sudo ./sgx_linux_x64_driver_2.11.0_2d2b795.bin
chmod +x sgx_linux_x64_sdk_2.15.101.1.bin
sudo ./sgx_linux_x64_sdk_2.15.101.1.bin --prefix=/opt/intel
source /opt/intel/sgxsdk/environment
echo "Install SGX environment"
echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu focal main' | sudo tee /etc/apt/sources.list.d/intel-sgx.list
wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | sudo apt-key add
sudo apt-get install libsgx-epid libsgx-quote-ex libsgx-dcap-ql
sudo apt-get install libsgx-enclave-common-dbgsym libsgx-dcap-ql-dbgsym libsgx-dcap-default-qpl-dbgsym
sudo apt-get install -y libsgx-urts libsgx-launch libsgx-enclave-common-dev libsgx-uae-service
sudo usermod -aG sgx_prv $USER
sudo apt-get install libsgx-dcap-default-qpl
sudo apt-get install libsgx-enclave-common-dev libsgx-dcap-ql-dev libsgx-dcap-default-qpl-dev
sudo apt-get install build-essential ocaml ocamlbuild automake autoconf libtool wget python-is-python3 libssl-dev git cmake perl
sudo apt-get install libssl-dev libcurl4-openssl-dev protobuf-compiler libprotobuf-dev debhelper cmake reprepro unzip
git clone https://github.com/intel/linux-sgx.git
cd linux-sgx && make preparation
sudo cp external/toolset/ubuntu20.04/* /usr/local/bin

echo "Download SGX SSL"
wget https://github.com/intel/intel-sgx-ssl/archive/refs/tags/lin_2.15.1_1.1.1l.zip
echo "Download OpenSSL for SGX SSL"
wget https://www.openssl.org/source/old/1.1.1/openssl-1.1.1l.tar.gz

unzip lin_2.15.1_1.1.1l.zip
cp openssl-1.1.1l.tar.gz intel-sgx-ssl-lin_2.15.1_1.1.1l/openssl_source/
cd intel-sgx-ssl-lin_2.15.1_1.1.1l/Linux/
make all
make test
sudo make install
echo "Environment set done"
