#!/bin/bash
traceType=('linux' 'fsl' 'couch' 'ms')
for type in ${traceType[@]}; do
    Rscript ${type}_traffic.r
    pdfcrop upload_traffic_${type}.pdf upload_traffic_${type}.pdf
done

Rscript traffic_legend.r
pdfcrop upload_traffic_legend.pdf upload_traffic_legend.pdf