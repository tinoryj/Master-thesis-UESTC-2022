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

mywidth=10
myheight=2.5
# my_color=c("dodgerblue1", "Goldenrod1", "green1", "skyblue1",  "red3","coral3")
my_color=c("Gold3","green3","lemonchiffon1","cadetblue1","red3","coral3")
if(T){
    x1 <- read.table("./expa2_keyGenPerformance_data.txt",header=TRUE)
    snapshot_name=c("SYN")
    type_name=c("BlindBLS","BlindRSA","MinHash", "TED", "SGX1st","SGX2nd")
    x_name= c("OPRF\nBLS","OPRF\nRSA","MinHash\nEncryption","TED","TEEDedup\n1st","TEEDedup\n2nd")
    x1$type <- factor(x1$type, levels=type_name)
    x1$snapshot <- factor(x1$snapshot, levels=snapshot_name)
    # cairo_pdf(file="./expa2_keyGenPerformance.pdf",  width=mywidth, height=myheight)
    cairo_pdf(file="../../expa2_keyGenPerformance.pdf",  width=mywidth, height=myheight)
    ggplot(data=x1, aes(x=type, y=interval, fill=type)) +
    geom_bar(stat="identity", position=position_dodge(0.9), colour="black", width=0.6, size=0.3) +
    geom_errorbar(aes(x=type,ymin=interval-fraction, ymax=interval+fraction), width=0.35, colour="black", position=position_dodge(0.9)) + 
    # geom_text(aes(label = round(interval/1024,3)), position = position_dodge(0.9), hjust=0.5, vjust=-0.1,size=7.5,angle=0, color="black") +
    coord_cartesian(ylim=c(290, 6500)) +
    scale_y_continuous(breaks=seq(0, 6000, 2000), labels=format(seq(0, 6, 2), scientific=FALSE)) +
    scale_x_discrete(breaks=type_name, labels=x_name) +
    scale_fill_manual(values=my_color, name="", breaks=type_name, labels=type_name) +
    guides(fill=guide_legend(nrow=1)) + 
    ylab("Speed (GB/s)") +
    xlab("") +
    theme_bw() +
    theme(
      panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    panel.border = element_blank(),
		  # panel.border=element_rect(size=0.5),
		  axis.line=element_line(colour="black", size=0.15),
		  axis.ticks=element_line(size=0.15),
	    axis.text.x=element_text(margin=margin(5,0,0,0), angle=0,vjust=0.6, hjust=0.5, colour="black", size=20),
	    axis.title.y=element_text(size=22, hjust=0.8),
	    axis.text.y=element_text(margin=margin(0,2,0,0),colour="black",size=22),
	    axis.title.x=element_text(size=rel(0)),
	    legend.key.size=unit(0.5, "cm"),
	    legend.position="none",
      legend.margin = margin(t = 0, unit='cm'),
      plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    ) + 
    annotate("text", x = 1, y = 2.11+600+50, label = "0.002", size=7, angle=0, color="black") +
    annotate("text", x = 2, y = 24.07+600+50, label = "0.024", size=7, angle=0, color="black") +
    annotate("text", x = 3, y = 346.02+600+50, label = "0.338", size=7, angle=0, color="black") +
    annotate("text", x = 4, y = 888.03+600+100, label = "0.867", size=7, angle=0, color="black") +
    annotate("text", x = 5, y = 3243.09+600+80, label = "3.166", size=7, angle=0, color="black") +
    annotate("text", x = 6, y = 5438.9+600+300, label = "5.311", size=7, angle=0, color="black")
}
