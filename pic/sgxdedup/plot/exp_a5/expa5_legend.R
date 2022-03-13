library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=3
myheight=0.4
my_color=c("green3", "cadetblue4")
my_value=c(1,5,10,15)

if(T){
    x1 <- read.table("expa5_keyRegression_data.txt",header=TRUE)
    cairo_pdf(file="../../expa5_keyRegression_time_legend.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$type,labels = c("KeyEnclave", "Cloud"), levels=c("Enclave","Server"))
    my_hist=ggplot(data=x1, aes(x=NSize,y=time,shape=type,linetype=type, colour=type)) +
    geom_line(size=1.5)  + 
    geom_point(size=2, stroke=0.75, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    #                         labels=c("1stUpload", "2ndUpload", "Download")) +
    # scale_linetype_manual(values=c("dashed","twodashed","dotted","dotdash")) +
    coord_cartesian(ylim=c(0, 1300)) +
    scale_y_continuous(breaks=seq(0, 1300, 325), labels=format(seq(0, 1300, 325), scientific=FALSE)) +
    scale_x_continuous(breaks=c(1,2,3,4,5,6,7,8,9,10), labels=format(c(1,2,3,4,5,6,7,8,9,10), scientific=FALSE)) + 
    scale_linetype_manual(values=c(2, 1, 3, 5, 4, 6)) +              
    ylab("Speed (MiB/s)") +
    xlab("Number of Clients") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=22),
        # legend.position=c(0.35,1.1),
        legend.direction="horizontal",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0,0,0,0), "cm")
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
