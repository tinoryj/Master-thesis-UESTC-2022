library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=7.5
myheight=3.5
if(T){
    args <- commandArgs(trailingOnly = TRUE)
    x1 <- read.table(args[1], header=TRUE)
    cairo_pdf(file=args[2],  width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("FirstF","MinF"), labels=c("firstFeature","minFeature"))
    x1$groupType <- factor(x1$groupType, levels=c("prefix-1" ,"prefix-2","prefix-3"), labels=c("FeatureSpy(1)" ,"FeatureSpy(2)","FeatureSpy(4)"))
    ggplot(data=x1, aes(x=groupType, y=ratio, fill=type)) +
    geom_bar(stat="identity", position=position_dodge(0.90), colour="black", width=0.8, size=0.3) +
    geom_text(aes(label = round(ratio*100,0)), position = position_dodge(0.90), hjust=0.5, vjust=-0.3,size=10,angle=0, color="black") +
    scale_colour_brewer(palette = "Set2")+
    coord_cartesian(ylim=c(0.052, 1.1)) +
    scale_y_continuous(breaks=seq(0, 1, 0.25), labels=format(seq(0, 100, 25), scientific=FALSE)) +
    # scale_x_discrete(breaks=type_name, labels=x_name) +
    scale_fill_brewer(palette = "Set2")+
    guides(fill=guide_legend(nrow=1)) +
    ylab("Coverage (%)") +
    xlab(NULL) +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(),
        panel.grid.minor=element_blank(),
	    panel.background=element_blank(),
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.title.x=element_text(size=26),
        axis.text.x=element_text(margin=margin(2,0,0,0), hjust=0.5, vjust=0.5, angle=15, colour="black", size=26),
        axis.title.y=element_text(size=30,  hjust=0.5),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=26),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=26),
        legend.position="none",
        legend.direction="vertical",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm"))
}
