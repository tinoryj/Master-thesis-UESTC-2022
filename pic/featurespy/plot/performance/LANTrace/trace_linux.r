library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=5
myheight=2.5
my_value=c(1,2,3,4,5,6)
my_line=c("dashed","solid","dotted","longdash","solid","solid")

abbrev_x <- c(1,expression(2^3), expression(2^6),expression(2^9),expression(2^12),expression(2^15))

if(T){
    x1 <- read.table("trace_linux.data",header=TRUE)
    cairo_pdf(file="trace_linux.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("SGXUpload", "DetFirstUpload", "DetMinUpload", "DetAllUpload","TEEDedupDownload","AverageDownload"))
    ggplot(data=x1, aes(x=as.factor(BatchSize), y=Performance,shape=Type, linetype=Type, colour=Type, group=Type), log= "x") +
    geom_line(size=1.5)  + 
    geom_point(size=0.5, stroke=0.8, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_brewer(palette="Dark2") +
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(16.2, 350)) +
    scale_y_continuous(breaks=seq(0, 300, 100), labels=format(seq(0, 300, 100), scientific=FALSE)) +
    scale_x_discrete(labels=c(1,20,40,60,80), breaks=c(1,20,40,60,80)) + 
    ylab('Speed (MiB/s)') +
    xlab("Snapshot") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), 
        panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.title.x=element_text(size=22),
        axis.text.x=element_text(margin=margin(5,0,0,0), hjust=0.8, colour="black", size=22),
        axis.title.y=element_text(size=20.4,  hjust=0.5),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=22),
        legend.title = element_blank(),
        legend.position="none",
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )
}
