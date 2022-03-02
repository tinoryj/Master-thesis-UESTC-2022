#!/bin/bash
# rm -rf *.pdf
tyeSet=('fsl' 'ms' 'linux' 'couch')

for type in ${tyeSet[@]}; do
    Rscript trace_$type.r
    pdfcrop trace_$type.pdf trace_$type.pdf
done

# Rscript trace_legend.r
# pdfcrop trace_legend.pdf trace_legend.pdf
Rscript trace_legend_upload.r
pdfcrop trace_legend_upload.pdf trace_legend_upload.pdf
Rscript trace_legend_download.r
pdfcrop trace_legend_download.pdf trace_legend_download.pdf
