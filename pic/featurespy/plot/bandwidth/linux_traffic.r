library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=3
myheight=1.5
my_line=c("solid","longdash","dashed","dotted")

abbrev_x <- c(1,expression(2^3), expression(2^6),expression(2^9),expression(2^12),expression(2^15))
abbrev_y <- c(0,expression(10^1), expression(10^2),expression(10^3),expression(10^4),expression(10^5))
labels_y <-trans_format("log10", math_format(10^.x))
breaks_y <-trans_breaks("log10", function(x) 10^x)

if(T){
    x1 <- read.table("traffic_linux.data",header=TRUE)
    cairo_pdf(file="./upload_traffic_linux.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("Client-side", "Two-stage", "Random-threshold", "Server-side"), labels=c("Source-based", "Two-stage", "Random-threshold", "Server-side"))
    ggplot(data=x1, aes(x=as.factor(ID), y=Traffic,shape=Type, linetype=Type, colour=Type, group=Type), log= "x") +
    geom_line(size=1.5)  + 
    # annotation_logticks()  +
    # geom_point(size=2, stroke=0.75, fill="white") +
    # scale_shape_manual(values=c(my_value)) +
    scale_colour_brewer(palette = "Set1") +
    scale_linetype_manual(values=c(my_line)) +
    # coord_cartesian(ylim=c(0, 5)) +
    # coord_cartesian(ylim=c(0.048, 1.01),xlim=c(1,84)) +
    # scale_y_continuous(breaks = breaks_y(1:1e5),labels=labels_y(breaks_y(1:1e5)) ,trans = 'log10') +
    # scale_y_continuous(breaks = trans_breaks("log10", function(x) 10^x)(c(1,8000)), labels = trans_format("log10", math_format(10^.x)),trans = 'log10') +
    scale_y_continuous(breaks=seq(0, 40, 10), labels=format(seq(0, 40, 10), scientific=FALSE)) +
    scale_x_discrete(labels=c("1", "20", "40", "60","80"), breaks=c("1", "20", "40", "60","80")) + 
    xlab("快照编号") +
    ylab("网络流量 (GiB)")+
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
        axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.8, colour="black", size=11,family="Times New Roman"),
        axis.title.y=element_text(size=11,  hjust=0.5,family="Times New Roman"),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=11,family="Times New Roman"),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=11),
        legend.position=c(0.35,1.1),
        legend.direction="horizontal",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )
}
