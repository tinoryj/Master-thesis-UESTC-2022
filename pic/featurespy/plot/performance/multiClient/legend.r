library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()



mywidth=8
myheight=0.5
my_value=c(1,2,3,4)
# my_line=c("longdash","dotted","solid","dashed")
my_line=c("solid","solid","solid","solid")
my_color=c("#66c2a5" ,"#fc8d62" ,"#8da0cb" ,"#e78ac3" ,"#a6d854" ,"#ffd92f")

if(T){
    x1 <- read.table("upload_2nd_line.data",header=TRUE)
    cairo_pdf(file="legend.pdf",  width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("TEE","firstFeature","minFeature","allFeature"), labels=c("TEEDedup","firstFeature","minFeature","allFeature"))
    my_hist = ggplot(data=x1, aes(x=ID,y=throughput,shape=type,linetype=type, colour=type)) +
    geom_line(size=1.5) + 
    geom_point(size=2, stroke=0.75, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(268, 1650), xlim=c(1,12)) +
    scale_x_continuous(breaks=c(1,4,8,12)) +
    scale_y_continuous(breaks=seq(200, 1600, 700), labels=format(seq(200, 1600, 700), scientific=FALSE)) +
    ylab("Speed (MiB/s)") +
    xlab(NULL) +
    theme_bw() +
    theme(
      panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    panel.border = element_blank(),
		  # panel.border=element_rect(size=0.5),
		  axis.line=element_line(colour="black", size=0.15),
		  axis.ticks=element_line(size=0.15),
	    axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.5, colour="black", size=24),
	    axis.title.y=element_text(size=24, hjust=0.5),
	    axis.text.y=element_text(margin=margin(0,2,0,0),colour="black",size=26),
	    axis.title.x=element_text(size=rel(0)),
	    legend.key.size=unit(1, "cm"),
      legend.title=element_blank(),
	    # legend.position=c(0.5,0.94),
      # legend.key.width=unit(1, "cm"),
      legend.direction="horizontal",
      legend.text=element_text(size=16.5),
      legend.margin = margin(t = 0, unit='cm'),
      plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    ) 
    g_legend <- function(a.gplot){
    tmp <- ggplot_gtable(ggplot_build(a.gplot))
    leg <- which(sapply(tmp$grobs, function(x) x$name) == "guide-box")
    legend <- tmp$grobs[[leg]]
    return(legend)}
    legend <- g_legend(my_hist)
    # or use cowplot package
    # legend <- cowplot::get_legend(my_hist)
    grid.newpage()
    grid.draw(legend)
}
