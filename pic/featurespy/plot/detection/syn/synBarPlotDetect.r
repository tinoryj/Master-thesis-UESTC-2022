library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=8
myheight=2.5
if(T){
    args <- commandArgs(trailingOnly = TRUE)
    x1 <- read.table(args[1], header=TRUE)
    cairo_pdf(file=args[2],  width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("FirstF","MinF","AllF"), labels=c("firstFeature","minFeature","allFeature"))
    x1$groupType <- factor(x1$groupType, levels=c("feature","prefix-1" ,"prefix-2","prefix-3"), labels=c("KeyGen" ,args[3], args[4], args[5]))
    ggplot(data=x1, aes(x=groupType, y=ratio, fill=type)) +
    geom_bar(stat="identity", position=position_dodge(0.90), colour="black", width=0.8, size=0.3) +
    geom_text(aes(label = round(ratio*100,0)), position = position_dodge(0.90), hjust=0.5, vjust=-0.3,size=7,angle=0, color="black",family="Times New Roman") +
    scale_colour_brewer(palette = "Set2")+
    coord_cartesian(ylim=c(0.052, 1.1)) +
    scale_y_continuous(breaks=seq(0, 1, 0.25), labels=format(seq(0, 100, 25), scientific=FALSE)) +
    # scale_x_discrete(breaks=type_name, labels=x_name) +
    scale_fill_brewer(palette = "Set2")+
    guides(fill=guide_legend(nrow=1)) +
    ylab("成功率 (%)") +
    xlab(NULL) +
    theme_bw() +
    theme(panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(),
		panel.border = element_blank(),
		axis.line=element_line(colour="black", size=0.3),
		axis.ticks=element_line(size=0.3),
	    axis.text.x=element_text(margin=margin(10,0,0,0), angle=0, hjust=0.5, colour="black", size=24,family="Times New Roman"),
        axis.title.x=element_text(margin=margin(5,-10,0,10), size=20,family="SimSun"),
	    axis.text.y=element_text(colour="black",size=26,family="Times New Roman"),
        axis.title.y=element_text(margin=margin(0, 10, 0, 0), size=26, hjust=0.5,family="SimSun"),
        legend.title = element_blank(),
	    legend.key.size=unit(1, "cm"),
        legend.text=element_text(size=20),
	    # legend.position=c(0.6, 0.7),
        legend.position="none",
		legend.direction="vertical",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.2,0.1,0.1), "cm"))
}
