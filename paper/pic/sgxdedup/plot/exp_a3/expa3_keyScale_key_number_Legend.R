library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=6
myheight=0.3
# my_color=c("red3", "coral3","dodgerblue3","cadetblue1")
my_color=c("red3","coral3")
my_value=c(1,10,5,15)
my_line=c("solid","dashed")
if(T){
    x1 <- read.table("expa3_keyScale_key_number_data.txt",header=TRUE)
    x1 <- subset(x1, threading %in% c("no"))
    cairo_pdf(file="./expa3_keyScale_performance_legend.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("1stKeyGen", "2ndKeyGen"), labels=c("第一轮密钥生成(在线加解密)", "第二轮密钥生成(推测性加密)"))
    my_hist = ggplot(data=x1, aes(x=Nsize, y=performance/100000,shape=type, linetype=type, colour=type)) +
    geom_line(size=1)  + 
    geom_point(size=2, stroke=0.75, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(0.71, 15), xlim=c(1, 35)) +
    # coord_cartesian(ylim=c(200000, 3000000), xlim=c(1, 35)) +
    scale_y_continuous(breaks=seq(0, 15, 5), labels=format(seq(0,15,5), scientific=FALSE)) +
    # scale_y_continuous(breaks=seq(0, 3000000, 600000), labels=format(seq(0,30,6), scientific=FALSE)) +
    scale_x_continuous(breaks=c(1,5,10,15,20,25,30,35), labels=format(c(1,5,10,15,20,25,30,35), scientific=FALSE)) + 
    ylab(expression(10^5%*%"Keys/s")) +
    xlab("模拟客户端个数") +
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
        axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.8, colour="black", size=11,family="Times New Roman"),
        axis.title.y=element_text(size=11, hjust=0.5,family="Times New Roman"),
        axis.text.y=element_text(margin=margin(0,2,0,0),colour="black",size=11,family="Times New Roman"),
        legend.title = element_blank(),
        legend.key.size=unit(0.8, "cm"),
        legend.text=element_text(size=11,family="SimSun"),
        legend.direction="horizontal",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )
    g_legend <- function(a.gplot){
    tmp <- ggplot_gtable(ggplot_build(a.gplot))
    leg <- which(sapply(tmp$grobs, function(x) x$name) == "guide-box")
    legend <- tmp$grobs[[leg]]
    return(legend)}
    legend <- g_legend(my_hist)
    # or use cowplot package
    # legend <- cowplot::get_legend(my_hist)
    grid.newpage()
    grid.draw(legend)
}
