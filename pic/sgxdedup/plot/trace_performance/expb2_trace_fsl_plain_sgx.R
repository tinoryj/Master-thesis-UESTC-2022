library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=5
myheight=2.5
my_color=c("#B2182B","#D6604D","#2166AC","#4393C3")
my_value=c(1,5,1,5)
my_line=c("dashed","solid","dashed","solid")

abbrev_x <- c(1,expression(2^3), expression(2^6),expression(2^9),expression(2^12),expression(2^15))

if(T){
    x1 <- read.table("expb2_trace_fsl_data_plain_sgx.txt",header=TRUE)
    cairo_pdf(file="../../expb2_trace_fsl_plain_sgx.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("SGXUpload", "SGXDownload", "PlainUpload", "PlainDownload"))
    ggplot(data=x1, aes(x=as.factor(BatchSize), y=Performance,shape=Type, linetype=Type, colour=Type, group=Type), log= "x") +
    geom_line(size=1.5)  + 
    geom_point(size=4, stroke=1.5, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(10.9, 250)) +
    scale_y_continuous(breaks=seq(0, 250, 50), labels=format(seq(0, 250, 50), scientific=FALSE)) +
    scale_x_discrete(labels=seq(1,10,1), breaks=seq(1,10,1)) + 
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
