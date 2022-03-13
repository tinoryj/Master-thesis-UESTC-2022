library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()



mywidth=4
myheight=2.5
my_value=c(1,2,3,4)
# my_line=c("longdash","dotted","solid","dashed")
my_line=c("solid","solid","solid","solid")
my_color=c("#66c2a5" ,"#fc8d62" ,"#8da0cb" ,"#e78ac3" ,"#a6d854" ,"#ffd92f")

if(T){
    x1 <- read.table("download_line.data",header=TRUE)
    cairo_pdf(file="download_line.pdf",  width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("TEE","feature"), labels=c("TEEDedup","TEEDedup+"))
    ggplot(data=x1, aes(x=ID,y=throughput,shape=type,linetype=type, colour=type)) +
    geom_line(size=1.5) + 
    geom_point(size=2, stroke=0.75, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(58, 1225), xlim=c(1,10)) +
    scale_x_continuous(breaks=c(1,2,3,4,5,6,7,8,9,10)) +
    scale_y_continuous(breaks=seq(0, 1200, 300), labels=format(seq(0, 1200, 300), scientific=FALSE)) +
    ylab("Speed (MiB/s)") +
    xlab("Number of clients") +
    theme_bw() +
    theme(
      panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    panel.border = element_blank(),
		  # panel.border=element_rect(size=0.5),
		  axis.line=element_line(colour="black", size=0.15),
		  axis.ticks=element_line(size=0.15),
	    axis.text.x=element_text(margin=margin(7,0,0,0), angle=0, hjust=0.6, colour="black", size=22),
	    axis.title.y=element_text(size=22, hjust=0.9),
      axis.title.x=element_text(size=24),
	    axis.text.y=element_text(margin=margin(0,2,0,0),colour="black",size=24),
	    # axis.title.x=element_text(size=rel(0)),
	    legend.key.size=unit(0.5, "cm"),
      legend.title=element_blank(),
	    legend.position="none",
      legend.direction="horizontal",
      legend.text=element_text(size=10),
      legend.margin = margin(t = 0, unit='cm'),
      plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    ) 
}
