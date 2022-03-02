library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=9
myheight=2.5
my_value=c(1,10)
my_line=c("longdash","solid")

if(T){
    args <- commandArgs(trailingOnly = TRUE)
    x1 <- read.table("prefixDistribution-10000-CouchDB-first.data",header=TRUE)
    cairo_pdf(file="prefixDistribution-10000-CouchDB-first.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("Raw","Mixed"), labels=c("Raw","Mixed"))
    ggplot(data=x1, aes(x=ID,y=ratio,shape=type,linetype=type, colour=type)) +
    geom_line(size=2)  + 
        geom_point(size=4, stroke=1.5, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_brewer(palette = "Set2") + 
    scale_linetype_manual(values=c(my_line)) +
    scale_size_manual(values=c(1.5, 1.5)) +
    coord_cartesian(ylim=c(0.0098, 0.23), xlim=c(1.28, 7)) +
    scale_x_continuous(breaks=c(1, 3, 5, 7)) +
    scale_y_continuous(breaks=seq(0, 0.2, 0.1), labels=format(seq(0, 20, 10), scientific=FALSE)) +
    ylab("Frac. (%)") +
    xlab("Window ID") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.8, colour="black", size=30),
        axis.title.y=element_text(size=26,  hjust=0.5),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0),colour="black",size=30),
        axis.title.x=element_text(size=30),
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