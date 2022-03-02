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
mywidth = 10
myheight=2.5
# my_color=c("red3","coral3","dodgerblue3","cadetblue4","Gold3","green3")
my_color=c("red3","coral3", "lemonchiffon1","cadetblue1","Gold3","green3")
if(T){
    x1 <- read.table("./key_batch_overall_bar.txt",header=TRUE)
    cairo_pdf(file="../../key_batch_overall_bar.pdf",  width=mywidth, height=myheight)
    # cairo_pdf(file="./key_batch_overall_bar.pdf",  width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("Key-1","Key-2"), labels=c("KeyEnclave-1st","KeyEnclave-2nd"))
    x1$Netspeed <- factor(x1$Netspeed, levels=c("1", "16", "256","4096"), labels=c("1", "16", "256", "4096"))
    ggplot(data=x1, aes(x=Netspeed, y=throughput, fill=type)) +
    geom_bar(stat="identity", position=position_dodge(0.90), colour="black", width=0.8, size=0.3) +
    # geom_errorbar(aes(ymin=x1$fraction-x1$interval, ymax=x1$fraction+x1$interval), width=0.4, colour="black", position=position_dodge(0.9)) + 
    geom_text(aes(label = round(throughput,2)), position = position_dodge(0.90), hjust=0.5, vjust=-0.1,size=7,angle=0, color="black") +
    scale_color_manual(values=c(my_color)) + 
    coord_cartesian(ylim=c(250, 6000)) +
    scale_y_continuous(breaks=seq(0, 6000, 1500), labels=format(seq(0, 6000, 1500), scientific=FALSE)) +
    # scale_x_discrete(breaks=type_name, labels=x_name) +
    scale_fill_manual(values=my_color) +
    guides(fill=guide_legend(nrow=1)) + 
    ylab("Speed (MiB/s)") +
    xlab("Fingerprint Batch Size") +
    theme_bw() +
    theme(
      panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    panel.border = element_blank(),
		  # panel.border=element_rect(size=0.5),
		  axis.line=element_line(colour="black", size=0.15),
		  axis.ticks=element_line(size=0.15),
	    axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.5, colour="black", size=20),
	    axis.title.y=element_text(size=20, hjust=0.5),
	    axis.text.y=element_text(margin=margin(0,2,0,0),colour="black",size=20),
	    axis.title.x=element_text(size=20),
	    legend.key.size=unit(0.5, "cm"),
      legend.title=element_blank(),
	    legend.position=c(0.4,0.8),
      # legend.key.width=unit(1, "cm"),
      legend.direction="horizontal",
      legend.text=element_text(size=20),
      plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm"))
}
