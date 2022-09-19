#!/bin/bash
# Rscript upload_bar.r
Rscript upload_thread_line.r
Rscript upload_thread_2nd_line.r
Rscript download_bar.r
Rscript legend.r
pdfcrop upload_bar.pdf upload_bar.pdf
pdfcrop upload_thread_line.pdf upload_thread_line.pdf
pdfcrop upload_thread_2nd_line.pdf upload_thread_2nd_line.pdf
pdfcrop download_bar.pdf download_bar.pdf
pdfcrop legend.pdf legend.pdf