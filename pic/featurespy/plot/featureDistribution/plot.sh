#!/bin/bash
Rscript featureDistributionLinux.r
Rscript featureDistributionCouchbase.r
pdfcrop featureDistributionLinux.pdf featureDistributionLinux.pdf
pdfcrop featureDistributionCouchbase.pdf featureDistributionCouchbase.pdf