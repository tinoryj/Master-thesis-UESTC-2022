# FeatureSpy: Proactively Detecting Learning-content Attack via Feature Inspection

Cloud providers keep low maintenance costs via deduplication, which stores only a single copy of redundant data from the same or different clients. Encrypted deduplication builds on cryptographic primitives to augment deduplication with data confidentiality, yet it cannot fully address a malicious client, which enumerates data contents to launch the learning-content attack.

This package provides two toolkits: (i) *FeatureSpy simulator*, which simulates the detection effectiveness of FeatureSpy against the learning-content attack; and (ii) *TEEDedup+*, which is an SGX-based encrypted deduplication system with FeatureSpy deployed.
## FeatureSpy Simulator

The simulator is used to run the experiments in §6.2 in our paper. For brevity, we only focus on the simulation of the overall FeatureSpy in the case study (i.e., Exp#3).

### Dependencies

FeatureSpy simulator is developed under Ubuntu 20.04.3 LTS and depends on the following packages that need to be installed by the default `apt` package management tool.

```shell
sudo apt install -y build-essential openssl libssl-dev clang llvm python curl git golang jq
```

### Build

Compile and cleanup the FeatureSpy simulator as follows.

```shell
cd FeatureSpy
# compile
make
# cleanup
make clean
```

### Usage

We provide a quick way to analyze the detection effectiveness. You can use `runCouch.sh` and `runLinux.sh` to test FeatureSpy with CouchDB and Linux, respectively, and use `processResult.sh` to output the summary of results. Note that you can modify the parameters in `runCouch.sh` and `runLinux.sh` to test different window sizes and similarity indicator lengths. Alternatively, you can follow [FeatureSpy/README.pdf](FeatureSpy/README.pdf) to manually run the simulator.

```shell
# download trace
cd FeatureSpy/traceDownload
chmod +x *.sh
bash downloadTraceCouch.sh
bash downloadTraceLinux.sh
# generate fake offers
cd FeatureSpy/SimulateOfferGenerator
chmod +x generateFakeOffers.sh
bash generateFakeOffers.sh
# test with CouchDB and Linux trace
cd FeatureSpy/
chmod +x *.sh
bash runCouch.sh
bash runLinux.sh
bash processResult.sh
```

In the running of `runLinux.sh`/`runCouch.sh`, the program processes each snapshot and outputs the detection results in the command line (via `stderr`).


```shell
firstFeature: not detected
minFeature: detected
allFeature: not detected
```

Also, it saves the feature distribution information of each window of the processing snapshot in `FeatureSpy/linuxResult/` and  `FeatureSpy/couchResult/` for Linux and CouchDB dataset, respectively. The file name of each raw snapshot is `${snpashotID}-origin-${windowSize}-${indicatorLength}.csv`, while that of each attack snapshot (i.e., adversarially injected with fake offers) is `${snpashotID}-mixed-${windowSize}-${indicatorLength}.csv`. An example file is shown as follows, where each row represents the fraction of the most number of chunks that have the same similarity indicator in a window.

| firstFeature | minFeature | allFeature |
| ------------ | ---------- | ---------- |
| 0.08         | 0.08       | 0.08       |
| 0.001        | 0.002      | 0.001      |
| 0.003        | 0.003      | 0.003      |


Furthermore, the script `processResult.sh` generates the file `mergedLinuxResult-${windowSize}-${indicatorLength}.csv`/`mergedCouchResult-${windowSize}-${indicatorLength}.csv` to summarize the final results of attack detection for all snapshots (see below). Each row represents the maximum fraction of the most number of chunks that have the same similarity indicator among all windows in each snapshot. Note that the raw (mixed) here means the snapshot without (with) injected faked offers.

| firstFeature max freq (raw) | minFeature max freq (raw) | allFeature max freq (raw) | firstFeature max freq (mixed) | minFeature max freq (mixed) | allFeature max freq (mixed) |
| :-------------------------- | ------------------------- | ------------------------- | ----------------------------- | --------------------------- | --------------------------- |
| 0.0032                      | 0.0028                    | 0.0028                    | 0.1106                        | 0.1106                      | 0.1106                      |
| 0.003                       | 0.003                     | 0.003                     | 0.1022                        | 0.1022                      | 0.1022                      |
| 0.0026                      | 0.0024                    | 0.0024                    | 0.1026                        | 0.1026                      | 0.1026                      |
| 0.0026                      | 0.0022                    | 0.0022                    | 0.0972                        | 0.0972                      | 0.0972                      |



## TEEDedup+

The system TEEDedup+ augments the previous SGX-based encrypted deduplication system [SGXDedup](https://www.usenix.org/conference/atc21/presentation/ren-yanjing) with FeatureSpy, so as to detect the learning-content attack in a client-side trust-execution environment.

### Prerequisites

TEEDedup+ is tested on a machine that equips with a Gigabyte B460M-DS3H motherboard and an Intel i7-10700 CPU, and runs Ubuntu 20.04.3 LTS.

Before running TEEDedup+, check if your machine supports SGX. If there is an option as `SGX` or `Intel Software Guard Extensions` in BIOS, then enable the option; otherwise, your machine does not support SGX. We strongly recommend finding the SGX-supported device in the [SGX hardware list](https://github.com/ayeks/SGX-hardware).

#### Registration

TEEDedup+ uses EPID-based remote attestation, and you need to register at the [EPID attestation page](https://api.portal.trustedservices.intel.com/EPID-attestation). Then, you can find your SPID and the corresponding subscription keys (both the primary and the secondary keys) at the [products page](https://api.portal.trustedservices.intel.com/products). Our test uses the `DEV Intel® Software Guard Extensions Attestation Service (Unlinkable)` product.


#### Dependencies

We now provide a one-step script to automatically install and configure the dependencies. The script will ask for a password for sudo operations if necessary. We have tested the script on Ubuntu 20.04.3 LTS.

```shell
chmod +x Scripts/environmentInstall.sh
./Scripts/environmentInstall.sh
```

Restart is required after the installation is finished. Then, check whether both `sgx_enclave` and `sgx_provision` are in `/dev`. If they are not in the directory (i.e., SGX driver is not successfully installed), reinstall the SGX driver manually and restart the machine until `sgx_enclave` and `sgx_provision` are in `/dev`. We strongly recommend that you refer to the instructions of [SGX Installation Guide: Download Link](https://download.01.org/intel-sgx/sgx-linux/2.15.1/docs/Intel_SGX_SW_Installation_Guide_for_Linux.pdf) and [SGX SSL README: Link](https://github.com/intel/intel-sgx-ssl) during manual or automatic installation for troubleshooting.


### Configuration

TEEDedup+ is configured based on JSON. You can change its configuration without rebuilding. Before starting, you need to fill the SPID and subscription keys in `./config.json` based on your registration information in Intel. A detailed configuration of TEEDedup+ can be found in [TEEDedup+/README.pdf](TEEDedup+/README.pdf).

```json
...
"pow": {
    ...
    "_SPID": "", // Your SPID for remote attseation service
    "_PriSubscriptionKey": "", // Your Intel remote attestation service primary subscription key
    "_SecSubscriptionKey": "" // Your Intel remote attestation service secondary subscription key
},
"km": {
    ...
    "_SPID": "", // Your SPID for remote attseation service
    "_PriSubscriptionKey": "", // Your Intel remote attestation service primary subscription key
    "_SecSubscriptionKey": "" // Your Intel remote attestation service secondary subscription key
},
...
```

### Build

We provide a script for a quick build and clean-up, and you can use it.

```shell
chmod +x ./Scripts/*.sh
# Build TEEDedup+ in release mode
./Scripts/buildReleaseMode.sh
# Build TEEDedup+ in debug mode
./Scripts/buildDebugMode.sh
# Clean up build result
./Scripts/cleanBuild.sh
```

The generated executable file and its required enclave dynamic library, keys are all stored in the `bin` directory.

### Usage

You can test TEEDedup+ in a single machine, and connect the key server, cloud, and client instances via the local loopback interface in `bin` directory. Since the key enclave needs to be attested by the cloud before usage, you need to start the cloud (`server-sgx`) first, then start the key server (`keymanager-sgx`), and wait for the message `KeyServer : keyServer session key update done` that indicates a successful attestation.

```shell
# start cloud
cd bin
./server-sgx

# start key server
cd bin
./keymanager-sgx
```

TEEDedup+ provides store and restores interfaces to clients.

```shell
# store file
cd bin
./client-sgx -s file

# restore file
cd bin
./client-sgx -r file
```
