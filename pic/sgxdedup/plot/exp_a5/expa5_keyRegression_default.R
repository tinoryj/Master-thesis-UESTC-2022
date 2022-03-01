library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=5
myheight=2.5
my_color=c("red3", "dodgerblue3")
my_value=c(1,5,10,15)

abbrev_x <- c(expression(1),expression(2^4), expression(2^8),expression(2^12),expression(2^16),expression(2^20))

if(T){
    x1 <- read.table("expa5_keyRegression_default_data.txt",header=TRUE)
    cairo_pdf(file="../../expa5_keyRegression_time_default.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("Enclave","Server"), labels=c("KeyEnclave", "Cloud"))
    ggplot(data=x1, aes(x=as.factor(NSize), y=time,shape=type, linetype=type, colour=type, group=type)) +
    geom_line(size=1.5)  + 
    geom_point(size=4, stroke=1.5, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(2, 1, 3, 5, 4, 6)) +              
    coord_cartesian(ylim=c(0, 0.4)) +
    scale_y_continuous(breaks=seq(0,0.4,0.1), labels=c("0","0.1","0.2","0.3","0.4")) +
    scale_x_discrete(labels = abbrev_x, breaks=c("0", "2", "4", "6", "8", "10"))+
    # scale_x_continuous(breaks=seq(0, 10, 1), labels=format(seq(0,20,2), scientific=FALSE)) + 
    ylab("Latency (s)") +
    xlab("i-th Rekeying Operation") +
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
        axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.8, colour="black", size=22),
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
