#!/bin/bash
tracePath='./traceDownload/couchTrace/' # path to couchDB docker image trace
indicatorLengthSet=('1' '2' '4')        # target indicator length L set (Unit:block, each block contains 16 bytes)
windowSize=('1000' '5000' '10000')      # traget window size (W)
threshold='0.03'                        # detection threshold
pickedSet=('2.5.2' '3.0.2' '3.0.3' '3.1.0' '3.1.3' '3.1.5' '3.1.6' '4.0.0' '4.1.0' '4.1.1' '4.5.0' '4.5.1' '4.6.0' '4.6.1' '4.6.2' '4.6.3' '4.6.4' '4.6.5' '5.0.1' '5.1.0' '5.1.1' '5.5.0' '5.5.1' '5.5.2' '6.0.0' '6.0.1' '6.0.2' '6.0.3' '6.0.4' '6.0.5' '6.5.0' '6.5.1' '6.6.0' '6.6.1' '6.6.2' 'community-2.2.0' 'community-3.0.1' 'community-3.1.3' 'community-4.0.0' 'community-4.1.0' 'community-4.1.1' 'community-4.5.0' 'community-4.5.1' 'community-5.0.1' 'community-5.1.1' 'community-6.0.0' 'community-6.5.0' 'community-6.5.1' 'community-6.6.0' 'community' 'enterprise-2.5.2' 'enterprise-3.0.2' 'enterprise-3.0.3' 'enterprise-3.1.0' 'enterprise-3.1.3' 'enterprise-3.1.5' 'enterprise-3.1.6' 'enterprise-4.0.0' 'enterprise-4.1.0' 'enterprise-4.1.1' 'enterprise-4.5.0' 'enterprise-4.5.1' 'enterprise-4.6.0' 'enterprise-4.6.1' 'enterprise-4.6.2' 'enterprise-4.6.3' 'enterprise-4.6.4' 'enterprise-4.6.5' 'enterprise-5.0.1' 'enterprise-5.1.0' 'enterprise-5.1.1' 'enterprise-5.5.0' 'enterprise-5.5.1' 'enterprise-5.5.2' 'enterprise-6.0.0' 'enterprise-6.0.1' 'enterprise-6.0.2' 'enterprise-6.0.3' 'enterprise-6.0.4' 'enterprise-6.0.5' 'enterprise-6.5.0' 'enterprise-6.5.1' 'enterprise')

rm -rf result.fileList
./generateFileList.sh SimulateOfferGenerator/result result.fileList
if [ ! -d "couchResult" ]; then
  mkdir couchResult
fi
for target in ${pickedSet[@]}; do
    mkdir -p ${target}
    cp ${tracePath}${target}.tar ./
    tar -xvf ${target}.tar -C ${target}
    rm -rf ${target}.fileList
    ./generateFileList.sh ${target} ${target}.fileList
    rm -rf Content
    rm -rf *.chunkInfo
    mkdir Content
    ./chunking ${target}.fileList result.fileList >test.chunkInfo
    for window in ${windowSize[@]}; do
        for prefix in ${indicatorLengthSet[@]}; do
            ./FeatureSpy test.chunkInfo ${window} ${prefix} ${threshold} >"couchResult/${target}-mixed-${window}-${prefix}.csv"
            clear
            sleep 5
        done
    done
    rm -rf Content
    rm -rf *.chunkInfo
    mkdir Content
    ./chunking ${target}.fileList >test.chunkInfo
    for window in ${windowSize[@]}; do
        for prefix in ${indicatorLengthSet[@]}; do
            ./FeatureSpy test.chunkInfo ${window} ${prefix} ${threshold} >"couchResult/${target}-origin-${window}-${prefix}.csv"
            clear
            sleep 5
        done
    done
    rm -rf ${target}.tar
    rm -rf ${target}
done
