library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=3.5
myheight=2.5
my_value=c(1,5,10)
my_line=c("longdash","dashed","solid")

if(T){
    args <- commandArgs(trailingOnly = TRUE)
    x1 <- read.table("varyWindow_linux.data",header=TRUE)
    cairo_pdf(file="varyWindow_linux.pdf", width=mywidth, height=myheight)
    # x1$ID <- factor(x1$ID, levels=c("1K","5K","10K"), labels=c("W=1K","W=5K","W=10K"))
    x1$type <- factor(x1$type, levels=c("firstFeature","minFeature","allFeature"), labels=c("firstFeature","minFeature","allFeature"))
    ggplot(data=x1, aes(x=ID,y=ratio,shape=type,linetype=type, colour=type)) +
    geom_line(size=2) + 
    geom_point(size=4, stroke=1.5, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_brewer(palette = "Set2") + 
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(0.048, 1.03),xlim=c(0.7,3.4)) +
    scale_x_continuous(breaks=c(1,2,3),labels=c("1K", "5K", "10K")) +
    scale_y_continuous(breaks=seq(0, 1, 0.25), labels=format(seq(0, 100, 25), scientific=FALSE)) +
    ylab("Detection (%)") +
    xlab("Window Size (W)") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.5, colour="black", size=26),
        axis.title.y=element_text(size=24,  hjust=0.85),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0),colour="black",size=26),
        axis.title.x=element_text(size=24),
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
