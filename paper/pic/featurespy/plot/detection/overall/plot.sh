#!/bin/bash
# rm -rf *.pdf
versionSet=('1000')
tyeSet=('all' 'min' 'first')

for q in ${versionSet[@]}; do
    echo ${q}
    for type in ${tyeSet[@]}; do
    Rscript prefixDistribution-${q}-CouchDB-$type.r
    pdfcrop "prefixDistribution-${q}-CouchDB-$type.pdf" "prefixDistribution-${q}-CouchDB-$type.pdf"
    pdfcrop "prefixDistribution-${q}-CouchDB-$type.pdf" "prefixDistribution-${q}-CouchDB-$type.pdf"
    done
done

for q in ${versionSet[@]}; do
    echo ${q}
    for type in ${tyeSet[@]}; do
    Rscript prefixDistribution-${q}-Linux-$type.r
    pdfcrop "prefixDistribution-${q}-Linux-$type.pdf" "prefixDistribution-${q}-Linux-$type.pdf"
    pdfcrop "prefixDistribution-${q}-Linux-$type.pdf" "prefixDistribution-${q}-Linux-$type.pdf"
    done
done

Rscript falsePositive.r falsePositiveLinux.data falsePositiveLinux.pdf
pdfcrop falsePositiveLinux.pdf falsePositiveLinux.pdf
Rscript effectiveness.r effectivenessLinux.data effectivenessLinux.pdf
pdfcrop effectivenessLinux.pdf effectivenessLinux.pdf


# Rscript prefixDistribution_legend.r
# pdfcrop prefixDistribution_legend.pdf prefixDistribution_legend.pdf

Rscript effectiveness-falsePositive_legend.r
pdfcrop effectiveness-falsePositive_legend.pdf effectiveness-falsePositive_legend.pdf
