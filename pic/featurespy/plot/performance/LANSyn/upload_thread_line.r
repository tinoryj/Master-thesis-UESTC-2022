library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()



mywidth=2
myheight=1.5
my_value=c(1,2,3,4)
# my_line=c("longdash","dotted","solid","dashed")
my_line=c("solid","solid","solid","solid")
my_color=c("#66c2a5" ,"#fc8d62" ,"#8da0cb" ,"#e78ac3" ,"#a6d854" ,"#ffd92f")

if(T){
    x1 <- read.table("upload_thread_line.data",header=TRUE)
    cairo_pdf(file="upload_thread_line.pdf",  width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("TEE","DetectionFirst", "DetectionMin","DetectionAll"), labels=c("TEEDedup","TEEDedup+\n(firstFeature)","TEEDedup+\n(minFeature)","TEEDedup+\n(allFeature)"))
  ggplot(data=x1, aes(x=ID,y=throughput,shape=type,linetype=type, colour=type)) +
    geom_line(size=1) + 
    geom_point(size=2, stroke=0.75, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(15.17, 320), xlim=c(1,4)) +
    scale_x_continuous(breaks=c(1,2,3,4)) +
    scale_y_continuous(breaks=seq(0, 300, 100), labels=format(seq(0, 300, 100), scientific=FALSE)) +
    ylab("速度 (MiB/s)") +
    xlab("线程数") +
    theme_bw() +
    theme(
      panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    panel.border = element_blank(),
		  # panel.border=element_rect(size=0.5),
		  axis.line=element_line(colour="black", size=0.15),
		  axis.ticks=element_line(size=0.15),
	    axis.text.x=element_text(margin=margin(7,0,0,0), angle=0, hjust=0.5, colour="black", size=11,family="Times New Roman"),
	    axis.title.y=element_text(size=11, hjust=0.9,family="Times New Roman"),
	    axis.text.y=element_text(margin=margin(0,2,0,0),colour="black",size=11,family="Times New Roman"),
	    axis.title.x=element_text(size=11,family="Times New Roman"),
	    legend.key.size=unit(0.5, "cm"),
      legend.title=element_blank(),
	    legend.position="none",
      legend.direction="horizontal",
      legend.text=element_text(size=10),
      legend.margin = margin(t = 0, unit='cm'),
      plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    ) 
}
