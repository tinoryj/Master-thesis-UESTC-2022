#!/bin/bash
traceTypes=('linux' 'couch')
targetTypes=('FileNumber' 'ModifyPos' 'Window')
for traceType in ${traceTypes[@]}; do
    for targetType in ${targetTypes[@]}; do
        Rscript vary${targetType}_${traceType}.r
        pdfcrop vary${targetType}_${traceType}.pdf vary${targetType}_${traceType}.pdf
    done
done
Rscript trade_off_legend.r
pdfcrop trade_off_legend.pdf trade_off_legend.pdf
