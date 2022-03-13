#! /usr/bin/env Rscript
require(grid)
require(ggplot2)
require(scales)

mywidth=5
myheight=2.5
my_color=c("red3", "dodgerblue3")
my_value=c(1,5,10,15)
abbrev_x <- c(expression(2^16),expression(2^19),expression(2^22),expression(2^25), expression(2^2),expression(2^6),expression(2^10),expression(2^12),expression(2^10),expression(2^12))

if(T){
    x1 <- read.table("expa5_keyRegression_data.txt",header=TRUE)
    cairo_pdf(file="../../expa5_keyRegression_time.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("Enclave","Server"), labels=c("KeyEnclave", "Cloud"))
    ggplot(data=x1, aes(x=as.factor(NSize), y=time, shape=type, linetype=type, colour=type, group=type)) +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(2, 1, 3, 5, 4, 6)) +              
    geom_line(size=1.5)  + 
    geom_point(size=2, stroke=0.75, fill="white") +
        coord_cartesian(ylim=c(0, 15)) +
    scale_y_continuous(breaks=seq(0, 15, 5), labels=format(seq(0, 15, 5), scientific=FALSE)) +
    scale_x_discrete(labels = abbrev_x, breaks=c("0", "3", "6", "9"))+
    # scale_x_continuous(breaks=seq(0, 9, 1), labels=format(seq(2^16,2^25,2), scientific=TRUE)) + 
    # scale_x_continuous(breaks=seq(0, 9, 2.5), labels=format(seq(abbrev_x), scientific=TRUE)) + 
    ylab("Latency (s)") +
    xlab("Key Regression Parameter") +
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
        axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.5, colour="black", size=22),
        axis.title.y=element_text(size=22,  hjust=0.5),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=22),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=22),
        legend.position=c(0.5,0.8),
        legend.direction="horizontal",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )               
}
