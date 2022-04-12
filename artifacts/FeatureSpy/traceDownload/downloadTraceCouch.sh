#!/bin/bash
mkdir couchTrace
cp get_couchbase_container.sh couchTrace
cp download-frozen-image-v2.sh couchTrace
cd couchTrace
bash get_couchbase_container.sh
cd ..
