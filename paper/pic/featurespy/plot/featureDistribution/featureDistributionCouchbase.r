library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=3
myheight=1.5
if(T){
    x1 <- read.table("featureDistributionCouchbase.data",header=TRUE)
    cairo_pdf(file="./featureDistributionCouchbase.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$wsize, levels=c("1K", "5K", "10K"), labels=c("W=1K", "W=5K", "W=10K"))
    ggplot(data=x1, aes(x=id, y=ndiff,shape=type,linetype=type, colour=type, group=type)) +
    geom_line(size=1)  +
    scale_colour_brewer(palette = "Set1")+
    coord_cartesian(xlim=c(1.455, 2700), ylim=c(0.000285, 0.006)) +
    scale_x_continuous(trans='log10', breaks=trans_breaks("log10", function(x) 10^x),
    labels=trans_format("log10", math_format(10^.x))) +
    scale_y_continuous(breaks=c(0.000,0.002,0.004,0.006), labels=c("0","0.002","0.004","0.006")) +
    ylab("标准化差值") +
    xlab("窗口") +
    theme_bw() +
    theme(panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(),
		panel.border = element_blank(),
		axis.line=element_line(colour="black", size=0.3),
		axis.ticks=element_line(size=0.3),
		axis.text.x=element_text(margin=margin(10,0,0,0), angle=0, hjust=0.5, colour="black", size=11, vjust=4,family="Times New Roman"),
	    axis.title.y=element_text(margin=margin(0, 5, 0, 0), size=11, hjust=0.9,family="SimSun"),
	    axis.text.y=element_text(colour="black",size=11,family="Times New Roman"),
	    axis.title.x=element_text(margin=margin(5,-10,0,10), size=11,vjust=5,family="SimSun"),
        legend.title = element_blank(),
	    legend.key.size=unit(1, "cm"),
        legend.text=element_text(size=11),
	    legend.position="none",
		legend.direction="vertical",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.2,0.1,0.1), "cm"))
}
