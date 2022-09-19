library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=3
myheight=1.5
my_color=c("red3","dodgerblue3","Gold3")
my_value=c(1,1,1,1)
my_line=c("solid","longdash","dashed","longdash")

abbrev_x <- c(1,expression(2^3), expression(2^6),expression(2^9),expression(2^12),expression(2^15))
abbrev_y <- c(0,expression(10^1), expression(10^2),expression(10^3),expression(10^4),expression(10^5))
labels_y <-trans_format("log10", math_format(10^.x))
breaks_y <-trans_breaks("log10", function(x) 10^x)

if(T){
    x1 <- read.table("result_ms.txt",header=TRUE)
    cairo_pdf(file="./upload_traffic_ms.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("Client-side", "Two-stage", "Random-threshold"), labels=c("Source-based", "Two-stage", "Random-threshold"))
    ggplot(data=x1, aes(x=as.factor(ID), y=Traffic,shape=Type, linetype=Type, colour=Type, group=Type), log= "x") +
    geom_line(size=1)  + 
    scale_color_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(0.048, 1.01),xlim=c(1,142)) +
    scale_y_continuous(breaks=seq(0, 1, 0.25), labels=format(seq(0, 100, 25), scientific=FALSE)) +
    scale_x_discrete(labels=c("1", "35", "70","105","140"), breaks=c("1", "35", "70","105","140")) + 
    xlab("快照编号") +
    ylab("空间节省 (%)")+
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
        legend.position="none",
        legend.direction="horizontal",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )
}
