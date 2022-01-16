#!/bin/bash
# Rscript upload_bar.r
Rscript upload_1st_line.r
Rscript upload_2nd_line.r
Rscript download_line.r
Rscript legend.r
pdfcrop upload_1st_line.pdf upload_1st_line.pdf
pdfcrop upload_2nd_line.pdf upload_2nd_line.pdf
pdfcrop download_line.pdf download_line.pdf
pdfcrop legend.pdf legend.pdf