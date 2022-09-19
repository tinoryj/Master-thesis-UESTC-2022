library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=3
myheight=1.5
my_value=c(1,10,5,15)
my_line=c(1,2,3,4)

if(T){
    x1 <- read.table("fixed_p_4.data",header=TRUE)
    cairo_pdf(file="fixed_p_4.pdf", width=mywidth, height=myheight)
    # x1$type <- factor(x1$type, levels=c("1F","2F","3F","4F"), labels=c("1 F","2 F","3 F","4 F"))
    # ggplot(data=x1, aes(x=type, y=ratio,shape=type, linetype=type, colour=type, group=type)) +
    x1$Type <- factor(x1$Type, levels=c("1F","2F","3F","4F"), labels=c("1 F","2 F","3 F","4 F"))
    ggplot(data=x1, aes(x=ID, y=Ratio,shape=Type, linetype=Type, colour=Type, group=Type)) +
    geom_line(size=1)  +
    geom_point(size=2, stroke=0.75, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_brewer(palette = "Set1")+
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(0.048, 1.02), xlim=c(0, 3)) +
    scale_y_continuous(breaks=seq(0, 1, 0.25), labels=format(seq(0, 100, 25), scientific=FALSE)) +
    scale_x_continuous(breaks=seq(0, 3, 1), labels=c("4", "8", "16", "32")) +
    ylab("成功率 (%)") +
    xlab("修改字节个数") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(),
        panel.grid.minor=element_blank(),
	    panel.background=element_blank(),
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.title.x=element_text(size=11,family="SimSun"),
        axis.text.x=element_text(margin=margin(5,0,0,0), hjust=0.8, colour="black", size=11,family="Times New Roman"),
        axis.title.y=element_text(size=11,  hjust=0.7,family="SimSun"),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=11,family="Times New Roman"),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=11),
        legend.position="none",
        legend.direction="vertical",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )
}
