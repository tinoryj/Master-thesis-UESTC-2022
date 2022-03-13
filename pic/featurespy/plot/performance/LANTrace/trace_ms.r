library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=3
myheight=1.5
my_value=c(1,2,3,4,5,6)
my_line=c("dashed","solid","dotted","longdash","solid","solid")

abbrev_x <- c(1,expression(2^3), expression(2^6),expression(2^9),expression(2^12),expression(2^15))

if(T){
    x1 <- read.table("trace_ms.data",header=TRUE)
    cairo_pdf(file="trace_ms.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("TEEUpload", "DetFirstUpload", "DetMinUpload", "DetAllUpload","TEEDedupDownload","AverageDownload"))
    ggplot(data=x1, aes(x=as.factor(BatchSize), y=Performance,shape=Type, linetype=Type, colour=Type, group=Type), log= "x") +
    geom_line(size=1)  + 
    geom_point(size=1.5, stroke=0.75, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_brewer(palette="Dark2") +
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(11.9, 250)) +
    scale_y_continuous(breaks=seq(0, 250, 125), labels=format(seq(0, 250, 125), scientific=FALSE)) +
    scale_x_discrete(labels=seq(1,10,1), breaks=seq(1,10,1)) + 
    ylab('速度 (MiB/s)') +
    xlab("快照编号") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), 
        panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.title.x=element_text(size=11,family="Times New Roman"),
        axis.text.x=element_text(margin=margin(5,0,0,0), hjust=0.8, colour="black", size=11,family="Times New Roman"),
        axis.title.y=element_text(size=11,  hjust=0.6,family="Times New Roman"),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=11,family="Times New Roman"),
        legend.title = element_blank(),
        legend.position="none",
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )
}
