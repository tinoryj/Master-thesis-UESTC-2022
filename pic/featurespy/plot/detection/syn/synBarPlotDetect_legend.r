library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=12
myheight=0.5
if(T){
    x1 <- read.table("synBarPlotDetect_legend.data", header=TRUE)
    cairo_pdf(file="synBarPlotDetect_legend.pdf",  width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("FirstF","MinF","AllF"), labels=c("firstFeature","minFeature","allFeature"))
    x1$groupType <- factor(x1$groupType, levels=c("similar"), labels=c("Similarity"))
    my_hist=ggplot(data=x1, aes(x=groupType, y=ratio, fill=type)) +
    geom_bar(stat="identity", position=position_dodge(0.90), colour="black", width=0.8, size=0.3) +
    geom_text(aes(label = round(ratio*100,1)), position = position_dodge(0.90), hjust=0.5, vjust=-0.3,size=5,angle=0, color="black") +
    scale_colour_brewer(palette = "Set2")+      
    coord_cartesian(ylim=c(0.055, 1.1)) +
    scale_y_continuous(breaks=seq(0, 1, 0.25), labels=format(seq(0, 100, 25), scientific=FALSE)) +
    # scale_x_discrete(breaks=type_name, labels=x_name) +
    scale_fill_brewer(palette = "Set2")+   
    guides(fill=guide_legend(nrow=1)) + 
    ylab("Match Ratio (%)") +
    xlab(NULL) +
    theme_bw() +
    theme(panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(),
		panel.border = element_blank(),
		axis.line=element_line(colour="black", size=0.3),
		axis.ticks=element_line(size=0.3),
	    axis.text.x=element_text(margin=margin(10,0,0,0), angle=0, hjust=0.5, colour="black", size=18),
	    axis.title.y=element_text(margin=margin(0, 10, 0, 0), size=18, hjust=0.5),
	    axis.text.y=element_text(colour="black",size=18),
	    axis.title.x=element_text(margin=margin(5,-10,0,10), size=20),
        legend.title = element_blank(),
	    legend.key.size=unit(1, "cm"),
        legend.text=element_text(size=30),
	    # legend.position=c(0.6, 0.7),
        # legend.position="none",
		legend.direction="vertical",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.2,0.1,0.1), "cm")
    )
    g_legend <- function(a.gplot){
        tmp <- ggplot_gtable(ggplot_build(a.gplot))
        leg <- which(sapply(tmp$grobs, function(x) x$name) ==   "guide-box")
        legend <- tmp$grobs[[leg]]
        return(legend)}
        legend <- g_legend(my_hist)
        # or use cowplot package
        # legend <- cowplot::get_legend(my_hist)
        grid.newpage()
        grid.draw(legend)
}