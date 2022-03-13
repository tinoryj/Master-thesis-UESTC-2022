library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=6
myheight=0.5
my_value=c(1,5,10)
my_line=c("longdash","dashed","solid")

if(T){
    args <- commandArgs(trailingOnly = TRUE)
    x1 <- read.table("varyWindow_linux.data",header=TRUE)
    cairo_pdf(file="trade_off_legend.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("firstFeature","minFeature","allFeature"), labels=c("firstFeature","minFeature","allFeature"))
    my_hist=ggplot(data=x1, aes(x=ID,y=ratio,shape=type,linetype=type, colour=type)) +
    geom_line(size=1.5) + 
    geom_point(size=2, stroke=0.75, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_brewer(palette = "Set2") + 
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(0.0475, 1),xlim=c(0.7,3.4)) +
    scale_x_continuous(breaks=c(1,2,3),labels=c("W=1K", "W=5K", "W=10K")) +
    scale_y_continuous(breaks=seq(0, 1, 0.25), labels=format(seq(0, 100, 25), scientific=FALSE)) +
    guides(fill=guide_legend(ncol=3)) + 
    ylab("检测率 (%)") +
    xlab(NULL) +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.5, colour="black", size=26),
        axis.title.y=element_text(size=26,  hjust=0.6),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0),colour="black",size=26),
        axis.title.x=element_text(size=26),
        legend.title = element_blank(),
        legend.key.size=unit(0.5, "cm"),
        legend.text=element_text(size=11,family="Times New Roman"),
        legend.position=c(0.5,0.5),
        # legend.position="none",
        legend.direction="horizontal",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )
    g_legend <- function(a.gplot){
        tmp <- ggplot_gtable(ggplot_build(a.gplot))
        leg <- which(sapply(tmp$grobs, function(x) x$name) == "guide-box")
        legend <- tmp$grobs[[leg]]
        return(legend)
    }
    legend <- g_legend(my_hist)
    # or use cowplot package
    # legend <- cowplot::get_legend(my_hist)
    grid.newpage()
    grid.draw(legend)
}
