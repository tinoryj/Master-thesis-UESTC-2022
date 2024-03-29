library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=3
myheight=1.5
my_color=c("red3", "dodgerblue3")
my_value=c(1,5,10,15)

abbrev_x <- c(expression(1),expression(2^3), expression(2^6),expression(2^9),expression(2^12),expression(2^15))
#abbrev_x <- c(1,8, 64,512,4096,32768)

if(T){
    x1 <- read.table("expa4_powBatchSize_breakdown_data.txt",header=TRUE)
    # cairo_pdf(file="./expa4_powBatchSize_breakdown.pdf", width=mywidth, height=myheight)
    cairo_pdf(file="./expa4_powBatchSize_breakdown.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("Enclave", "Server"),labels=c("所有权证明安全区", "云服务端"))
    ggplot(data=x1, aes(x=as.factor(BatchSize), y=Performance,shape=Type, linetype=Type, colour=Type, group=Type), log= "x") +
    geom_line(size=1)  + 
    geom_point(size=2, stroke=0.75, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c("dashed","solid"))+ 
    coord_cartesian(ylim=c(0.284, 6)) +
    scale_y_continuous(breaks=seq(0, 6, 2), labels=format(seq(0, 6, 2), scientific=FALSE)) +
    scale_x_discrete(labels=abbrev_x, breaks=c("0", "3", "6", "9", "12", "15")) + 
    ylab('时间 (ms)') +
    xlab("密文批量大小") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), 
        panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.title.x=element_text(size=11,family="Times New Roman"),
        axis.text.x=element_text(margin=margin(5,0,0,0), hjust=0.8, colour="black", size=11,family="Times New Roman"),
        axis.title.y=element_text(size=11,  hjust=0.5,family="Times New Roman"),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=11,family="Times New Roman"),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=11,family="Times New Roman"),
        legend.position=c(0.5,0.9),
        legend.direction="horizontal",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )
}
