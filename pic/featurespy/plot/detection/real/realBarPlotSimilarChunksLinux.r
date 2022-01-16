library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=5
myheight=2.5
if(T){
    args <- commandArgs(trailingOnly = TRUE)
    x1 <- read.table("similarChunkLinux.data", header=TRUE)
    cairo_pdf(file="similarChunkLinux.pdf",  width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("1F","2F","3F"), labels=c("1 Feature","2 Feature","3 Feature"))
    x1$groupType <- factor(x1$groupType, levels=c("1K" ,"5K","10K"), labels=c("1K" ,"5K","10K"))
    ggplot(data=x1, aes(x=groupType, y=ratio, fill=type)) +
    geom_bar(stat="identity", position=position_dodge(0.90), colour="black", width=0.8, size=0.3) +
    geom_text(aes(label = round(ratio,1)), position = position_dodge(0.90), hjust=0.5, vjust=-0.3,size=5,angle=0, color="black") +
    scale_colour_brewer(palette = "Set2")+      
    coord_cartesian(ylim=c(16.5, 350)) +
    scale_y_continuous(breaks=seq(0, 300, 100), labels=format(seq(0, 300, 100), scientific=FALSE)) +
    # scale_x_discrete(breaks=type_name, labels=x_name) +
    scale_fill_brewer(palette = "Set2") +   
    guides(fill=guide_legend(nrow=1)) + 
    ylab("Similar pair") +
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
        axis.text.x=element_text(margin=margin(2,0,0,0), hjust=0.5, colour="black", size=26),
        axis.title.y=element_text(size=25,  hjust=0.5),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=26),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=26),
        legend.position="none",
        legend.direction="vertical",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm"))
}
