if(require("scales")){
  print("scales is loaded correctly")
} else {
  print("trying to install scales")
  install.packages("scales")
  if(require(scales)){
    print("scales installed and loaded")
  } else {
    stop("could not install scales")
  }
}

if(require("ggplot2")){
  print("ggplot2 is loaded correctly")
} else {
  print("trying to install ggplot2")
  install.packages("ggplot2")
  if(require(ggplot2)){
    print("ggplot2 installed and loaded")
  } else {
    stop("could not install ggplot2")
  }
}

if(require("RColorBrewer")){
  print("RColorBrewer is loaded correctly")
} else {
  print("trying to install RColorBrewer")
  install.packages("RColorBrewer")
  if(require(RColorBrewer)){
    print("RColorBrewer installed and loaded")
  } else {
    stop("could not install RColorBrewer")
  }
}
library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=5
myheight=2.5
my_color=c("lemonchiffon1","cadetblue1","red3")
if(T){
    x1 <- read.table("./expa4_powPerformance_data.txt",header=TRUE)
    snapshot_name=c("SYN")
    type_name=c("POW-MT","POW-UH","TEEDedup")
    x_name= c("POW-MT","POW-UH","TEEDedup")
    x1$type <- factor(x1$type, levels=type_name)
    x1$snapshot <- factor(x1$snapshot, levels=snapshot_name)
    cairo_pdf(file="../../expa4_powPerformance.pdf",  width=mywidth, height=myheight)
    ggplot(data=x1, aes(x=type, y=(interval), fill=type)) +
    geom_bar(stat="identity", position=position_dodge(0.9), colour="black", width=0.7, size=0.3) +
    geom_errorbar(aes(x=type,ymin=interval-fraction, ymax=interval+fraction), width=0.35, colour="black", position=position_dodge(0.9)) + 
    # geom_text(aes(label = round((interval),0)), position = position_dodge(0.9), hjust=0.5, vjust=-0.1,size=8,angle=0, color="black",family="Times New Roman") +
    coord_cartesian(ylim=c(15, 350),xlim=c(1,3)) +
    scale_y_continuous(breaks=seq(0, 300, 100), labels=format(seq(0, 300, 100), scientific=FALSE)) +
    scale_x_discrete(breaks=type_name, labels=x_name) +
    scale_fill_manual(values=my_color, name="", breaks=type_name, labels=type_name) +
    # guides(fill=guide_legend(nrow=1)) + 
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
	    # axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.5, colour="black", size=rel(0)),
      axis.text.x=element_text(margin=margin(5,0,0,0), angle=0,vjust=0.6, hjust=0.5, colour="black", size=18),
	    axis.title.y=element_text(size=22, hjust=0.5),
	    axis.text.y=element_text(margin=margin(0,2,0,0),colour="black",size=22),
	    axis.title.x=element_text(size=rel(0)),
	    legend.key.size=unit(0.5, "cm"),
      legend.direction="vertical",
      legend.text=element_text(size=22),
	    # legend.position=c(0.35,0.8),
      legend.position="none",
      legend.margin = margin(t = 0, unit='cm'),
      plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    ) + 
    annotate("text", x = 1, y = 37+20+10, label = "37", size=7, angle=0, color="black",family="Times New Roman") +
    annotate("text", x = 2, y = 139+20+10, label = "139", size=7, angle=0, color="black",family="Times New Roman") +
    annotate("text", x = 3, y = 305+20+20, label = "305", size=7, angle=0, color="black",family="Times New Roman")
}
