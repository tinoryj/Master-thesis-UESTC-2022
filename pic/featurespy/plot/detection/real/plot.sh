#!/bin/bash
rm -rf *.pdf
windowSet=('1K' '5K' '10K')
featureSet=('1F' '2F' '3F')
for feature in ${featureSet[@]}; do 
    for window in ${windowSet[@]}; do 
        Rscript realBarPlot.r linux-$window-$feature.data linux-$window-$feature.pdf
        pdfcrop linux-$window-$feature.pdf linux-$window-$feature.pdf
        Rscript realBarPlot.r couch-$window-$feature.data couch-$window-$feature.pdf
        pdfcrop couch-$window-$feature.pdf couch-$window-$feature.pdf
    done
done
#Rscript realBarPlotSimilarChunksLinux.r
#Rscript realBarPlotSimilarChunksCouch.r
#pdfcrop similarChunkLinux.pdf similarChunkLinux.pdf 
#pdfcrop similarChunkCouch.pdf similarChunkCouch.pdf 

Rscript realBarPlot_legend.r
pdfcrop realBarPlot_legend.pdf realBarPlot_legend.pdf
#Rscript realBarPlotSimilar_legend.r
#pdfcrop realBarPlotSimilar_legend.pdf realBarPlotSimilar_legend.pdf
