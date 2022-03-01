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
my_color=c("Gold3","red3","lemonchiffon1","coral3","cadetblue1","green3")
if(T){
    x1 <- read.table("./upload_network_speed_bar_data.txt",header=TRUE)
    cairo_pdf(file="../../upload_network_speed_bar.pdf",  width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("DupLESS","SGX-1","NoProtect-1","SGX-2","NoProtect-2"), labels=c("DupLESS","TEEDedup-1st","PlainDedup-1st","TEEDedup-2nd","PlainDedup-2nd"))
    x1$Netspeed <- factor(x1$Netspeed, levels=c("1", "5","10"), labels=c("1Gbps","5Gbps", "10Gbps"))
    ggplot(data=x1, aes(x=Netspeed, y=throughput, fill=type)) +
    geom_bar(stat="identity", position=position_dodge(0.90), colour="black", width=0.8, size=0.3) +
    geom_errorbar(aes(x=Netspeed,ymin=throughput-fraction, ymax=throughput+fraction), width=0.4, colour="black", position=position_dodge(0.9)) + 
    geom_text(aes(label = round(throughput,0)), position = position_dodge(0.90), hjust=0.5, vjust=-0.3,size=7,angle=0, color="black") +
    scale_color_manual(values=c(my_color)) + 
    coord_cartesian(ylim=c(16.3, 349), xlim=c(1.05, 2.95)) +
    scale_y_continuous(breaks=seq(0, 300, 100), labels=format(seq(0, 300, 100), scientific=FALSE)) +
    # scale_x_discrete(breaks=type_name, labels=x_name) +
    scale_fill_manual(values=my_color) +
    guides(fill=guide_legend(nrow=1)) + 
    ylab("Speed (MB/s)") +
    xlab("Network Bandwidth") +
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
	    axis.title.x=element_text(size=rel(0)),
	    legend.key.size=unit(0.5, "cm"),
      legend.title=element_blank(),
	    legend.position=c(0.46,0.94),
      # legend.key.width=unit(1, "cm"),
      legend.direction="horizontal",
      legend.text=element_text(size=16.5),
      legend.margin = margin(t = 0, unit='cm'),
      plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    ) 
    # +
    # annotate("text", x = 0.65, y = 107+21+10, label = "107", size=7, angle=0, color="black") +
    # annotate("text", x = 0.875, y = 106+21+10, label = "106", size=7, angle=0, color="black") +
    # annotate("text", x = 1.105, y = 183+21+10, label = "183", size=7, angle=0, color="black") +
    # annotate("text", x = 1.34, y = 237+21+13, label = "237", size=7, angle=0, color="black") +
    # annotate("text", x = 1.65, y = 183+21+10, label = "183", size=7, angle=0, color="black") +
    # annotate("text", x = 1.88, y = 239+21+10, label = "239", size=7, angle=0, color="black") +
    # annotate("text", x = 2.105, y = 188+21+10, label = "188", size=7, angle=0, color="black") +
    # annotate("text", x = 2.335, y = 242+21+10, label = "242", size=7, angle=0, color="black") +
    # annotate("text", x = 2.65, y = 194+21+10, label = "194", size=7, angle=0, color="black") +
    # annotate("text", x = 2.875, y = 242+21+12, label = "242", size=7, angle=0, color="black") +
    # annotate("text", x = 3.105, y = 202+21+10, label = "202", size=7, angle=0, color="black") +
    # annotate("text", x = 3.34, y = 250+21+10, label = "250", size=7, angle=0, color="black")
}
