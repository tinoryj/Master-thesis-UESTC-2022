#!/bin/bash
# rm -rf syn-*
# rm -rf *.pdf
# clang++ -O3 selectTargetOutput.cpp -o selectTargetOutput
pSet=('1' '4' '8')
qSet=('4' '16' '32')
# for p in ${pSet[@]}; do
#     for q in ${qSet[@]}; do
#         echo ${p}-${q}
#         ./selectTargetOutput "./8K/Rand-8192-"${p}"Pos-16B-10K-128.csv" ${p} ${q} 3 1 2 4
#         Rscript synBarPlotDetect.r syn-p${p}-q${q}-detect.data syn-p${p}-q${q}-detect.pdf "Indicator-1" "Indicator-2" "Indicator-4"
#         Rscript synBarPlotSim.r syn-p${p}-q${q}-sim.data syn-p${p}-q${q}-sim.pdf
#         pdfcrop syn-p${p}-q${q}-detect.pdf syn-p${p}-q${q}-detect.pdf
#         pdfcrop syn-p${p}-q${q}-sim.pdf syn-p${p}-q${q}-sim.pdf
#     done
# done
# for p in ${pSet[@]}; do
#     for q in ${qSet[@]}; do
#         echo ${p}-${q}
#         ./selectTargetOutput "./8K/Rand-8192-"${p}"Pos-16B-10K-128.csv" ${p} ${q} 3 1 2 4
#         Rscript synBarPlotDetect.r syn-p${p}-q${q}-detect.data syn-p${p}-q${q}-detect.pdf "Indicator-1" "Indicator-2" "Indicator-4"
#         # Rscript synBarPlotSim.r syn-p${p}-q${q}-sim.data syn-p${p}-q${q}-sim.pdf
#         pdfcrop syn-p${p}-q${q}-detect.pdf syn-p${p}-q${q}-detect.pdf
#         # pdfcrop syn-p${p}-q${q}-sim.pdf syn-p${p}-q${q}-sim.pdf
#     done
# done

Rscript synBarPlotDetect.r syn-p1-q4-detect.data syn-p1-q4-detect.pdf "SPE(1)" "SPE(2)" "SPE(4)"
pdfcrop syn-p1-q4-detect.pdf syn-p1-q4-detect.pdf
Rscript synBarPlotDetect.r syn-p4-q16-detect.data syn-p4-q16-detect.pdf "SPE(1)" "SPE(2)" "SPE(4)"
pdfcrop syn-p4-q16-detect.pdf syn-p4-q16-detect.pdf
Rscript synBarPlotDetect.r syn-p8-q32-detect.data syn-p8-q32-detect.pdf "SPE(1)" "SPE(2)" "SPE(4)"
pdfcrop syn-p8-q32-detect.pdf syn-p8-q32-detect.pdf

Rscript synPloyLineSim_legend.r
Rscript synPloyLineSim_p.r
Rscript synPloyLineSim_q.r
pdfcrop fixed_pq_legend.pdf fixed_pq_legend.pdf
pdfcrop fixed_p_4.pdf fixed_p_4.pdf
pdfcrop fixed_q_16.pdf fixed_q_16.pdf

Rscript synBarPlotSim_legend.r
Rscript synBarPlotDetect_legend.r
pdfcrop synBarPlotSim_legend.pdf synBarPlotSim_legend.pdf
pdfcrop synBarPlotDetect_legend.pdf synBarPlotDetect_legend.pdf
# rm -rf selectTargetOutput
