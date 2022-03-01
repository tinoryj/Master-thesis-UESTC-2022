library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=5
myheight=2.5
my_color=c("red3", "coral3")
my_value=c(1,5,10,15)

abbrev_x <- c(expression(1),expression(2^3), expression(2^6),expression(2^9),expression(2^12),expression(2^15))

if(T){
    x1 <- read.table("expa4_powBatchSize_overall_data.txt",header=TRUE)
    cairo_pdf(file="../../expa4_powBatchSize_overall.pdf", width=mywidth, height=myheight)
    # cairo_pdf(file="./expa4_powBatchSize_overall.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("Overall"))
    ggplot(data=x1, aes(x=as.factor(BatchSize), y=Performance,shape=Type, linetype=Type, colour=Type, group=Type), log= "x") +
    geom_line(size=1.5)  + 
    geom_point(size=4, stroke=1.5, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c("solid"))+
    coord_cartesian(ylim=c(0, 400)) +
    scale_y_continuous(breaks=seq(0, 400, 100), labels=format(seq(0, 400, 100), scientific=FALSE)) +
    scale_x_discrete(labels=abbrev_x, breaks=c("0", "3", "6", "9", "12", "15")) + 
    ylab('Speed (MB/s)') +
    xlab("Ciphertext Batch Size") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), 
        panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.title.x=element_text(size=22),
        axis.text.x=element_text(margin=margin(5,0,0,0), hjust=0.8, colour="black", size=22),
        axis.title.y=element_text( size=19,  hjust=0.5),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0),colour="black",size=22),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=21),
        # legend.position=c(0.2,0.9),
        legend.position="none",
        legend.direction="horizontal",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )
}
