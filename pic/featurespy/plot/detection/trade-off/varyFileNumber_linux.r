library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=2
myheight=1.4
my_value=c(1,5,10)
my_line=c("longdash","dashed","solid")
abbrev_x <- c(expression(2^9), expression(2^10),expression(2^11))
if(T){
    args <- commandArgs(trailingOnly = TRUE)
    x1 <- read.table("varyFileNumber_linux.data",header=TRUE)
    cairo_pdf(file="varyFileNumber_linux.pdf", width=mywidth, height=myheight)
    # x1$ID <- factor(x1$ID, levels=c("1K","5K","10K"), labels=c("W=1K","W=5K","W=10K"))
    x1$type <- factor(x1$type, levels=c("firstFeature","minFeature","allFeature"), labels=c("firstFeature","minFeature","allFeature"))
    ggplot(data=x1, aes(x=ID,y=ratio,shape=type,linetype=type, colour=type)) +
    geom_line(size=1.5) + 
    geom_point(size=2, stroke=0.75, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_brewer(palette = "Set2") + 
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(0.048, 1.03),xlim=c(0.7,3.4)) +
    scale_x_continuous(breaks=c(1,2,3),labels=abbrev_x) +
    scale_y_continuous(breaks=seq(0, 1, 0.25), labels=format(seq(0, 100, 25), scientific=FALSE)) +
    ylab("检测率 (%)") +
    xlab("可能取值(y)") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.5, colour="black", size=11,family="Times New Roman"),
        axis.title.y=element_text(size=11, hjust=0.85,family="SimSun"),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0),colour="black",size=11,family="Times New Roman"),
        axis.title.x=element_text(size=11,family="SimSun"),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=20),
        # legend.position=c(0.3,0.82),
        legend.position="none",
        legend.direction="vertical",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )
}
