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
# my_color=c("red3","coral3","dodgerblue3","cadetblue4","Gold3","green3")
my_color=c("red3","lemonchiffon1")
if(T){
    x1 <- read.table("./download_network_speed_bar_data.txt",header=TRUE)
    cairo_pdf(file="./download_network_speed_bar.pdf",  width=mywidth, height=myheight)
    # cairo_pdf(file="./download_network_speed_bar.pdf",  width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("TEE","NoProtect"), labels=c("TEEDedup","PlainDedup"))
    x1$Netspeed <- factor(x1$Netspeed, levels=c("1","5", "10"), labels=c("1Gbps","5Gbps","10Gbps"))
    ggplot(data=x1, aes(x=Netspeed, y=throughput, fill=type)) +
    geom_bar(stat="identity", position=position_dodge(0.85), colour="black", width=0.8, size=0.3) +
    geom_errorbar(aes(x=Netspeed,ymin=throughput-fraction, ymax=throughput+fraction), width=0.4, colour="black", position=position_dodge(0.85)) + 
    # geom_text(aes(label = round(throughput,0)), position = position_dodge(0.8), hjust=0.5, vjust=-0.1,size=7,angle=0, color="black",family="Times New Roman") +
    scale_color_manual(values=c(my_color)) + 
    coord_cartesian(ylim=c(31.3, 660)) +
    scale_y_continuous(breaks=seq(0, 600, 200), labels=format(seq(0, 600, 200), scientific=FALSE)) +
    # scale_x_discrete(breaks=type_name, labels=x_name) +
    scale_fill_manual(values=my_color) +
    guides(fill=guide_legend(nrow=1)) + 
    ylab("速度 (MiB/s)") +
    xlab("") +
    theme_bw() +
    theme(
      panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    panel.border = element_blank(),
		  # panel.border=element_rect(size=0.5),
		  axis.line=element_line(colour="black", size=0.3),
		  axis.ticks=element_line(size=0.3),
	    axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.5, colour="black", size=20,family="Times New Roman"),
	    axis.title.y=element_text(size=20, hjust=0.5,family="Times New Roman"),
	    axis.text.y=element_text(margin=margin(0,2,0,0),colour="black",size=20,family="Times New Roman"),
	    axis.title.x=element_text(size=rel(0)),
	    legend.key.size=unit(0.5, "cm"),
      legend.title=element_blank(),
	    legend.position=c(0.4,0.96),
      legend.margin = margin(t = 0, unit='cm'),
      legend.direction="vertical",
		  legend.box = "vertical",
      legend.text=element_text(size=16.5,family="Times New Roman"),
      plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    # )
	  ) + 
    annotate("text", x = 0.775, y = 100+40+10, label = "100", size=7, angle=0, color="black",family="Times New Roman") +
    annotate("text", x = 1.2, y = 115+40+10, label = "115", size=7, angle=0, color="black",family="Times New Roman") +
    annotate("text", x = 1.8, y = 261+40+10, label = "261", size=7, angle=0, color="black",family="Times New Roman") +
    annotate("text", x = 2.2, y = 491+50+10, label = "491", size=7, angle=0, color="black",family="Times New Roman") +
    annotate("text", x = 2.775, y = 323+40+10, label = "323", size=7, angle=0, color="black",family="Times New Roman") +
    annotate("text", x = 3.2, y = 579+55+10, label = "579", size=7, angle=0, color="black",family="Times New Roman")
}
