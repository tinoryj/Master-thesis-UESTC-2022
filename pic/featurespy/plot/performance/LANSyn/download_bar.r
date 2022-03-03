library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=3
myheight=2.5

my_color=c("#66c2a5" ,"#fc8d62")

if(T){
    x1 <- read.table("./download_bar.data",header=TRUE)
    cairo_pdf(file="download_bar.pdf",  width=mywidth, height=myheight)
    # cairo_pdf(file="./download_bar.pdf",  width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("TEE","Detection"), labels=c("TEEDedup","TEEDedup+"))
    x1$Netspeed <- factor(x1$Netspeed, levels=c("download"), labels=c("Download"))
    ggplot(data=x1, aes(x=Netspeed, y=throughput, fill=type)) +
    geom_bar(stat="identity", position=position_dodge(0.85), colour="black", width=0.7, size=0.3) +
    geom_errorbar(aes(x=Netspeed,ymin=throughput-fraction, ymax=throughput+fraction), width=0.4, colour="black", position=position_dodge(0.85)) + 
    geom_text(aes(label = round(throughput,0)), position = position_dodge(0.8), hjust=0.5, vjust=-0.3,size=7,angle=0, color="black") +
    scale_colour_manual(my_color) +
    coord_cartesian(ylim=c(24.02, 505)) +
    scale_y_continuous(breaks=seq(0, 500, 250), labels=format(seq(0, 500, 250), scientific=FALSE)) +
    # scale_x_discrete(breaks=type_name, labels=x_name) +
    scale_fill_manual(values=my_color) +
    guides(fill=guide_legend(nrow=2)) + 
    ylab("Speed (MiB/s)") +
    xlab("") +
    theme_bw() +
    theme(
      panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    panel.border = element_blank(),
		  # panel.border=element_rect(size=0.5),
		  axis.line=element_line(colour="black", size=0.15),
		  axis.ticks=element_line(size=0.15),
	    axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.5, colour="black", size=24),
	    axis.title.y=element_text(size=24, hjust=0.7),
	    axis.text.y=element_text(margin=margin(0,2,0,0),colour="black",size=24),
	    axis.title.x=element_text(size=rel(0)),
	    legend.key.size=unit(0.5, "cm"),
      legend.title=element_blank(),
	    legend.position="none",
      legend.direction="horizontal",
      legend.text=element_text(size=10),
      legend.margin = margin(t = 0, unit='cm'),
      plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    ) 
}
