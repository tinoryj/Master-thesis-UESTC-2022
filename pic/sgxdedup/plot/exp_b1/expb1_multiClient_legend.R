library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=6.2
myheight=0.4
my_color=c("red3","coral3","dodgerblue3","cadetblue1")
my_value=c(1,10,5,15)
my_line=c("dashed","dashed","solid")
if(T){
    x1 <- read.table("expb1_multiple_client_data.txt",header=TRUE)
    cairo_pdf(file="../../expb1_multiClient_legend.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("1stUpload","2ndUpload","Download"), labels=c("Upload-1st","Upload-2nd","Download"))
    my_hist=ggplot(data=x1, aes(x=clients,y=throughput,shape=type,linetype=type, colour=type)) +
    geom_line(size=1)  + 
    geom_point(size=5, stroke=3, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(my_line)) +
    scale_size_manual(values=c(1.5, 1.5, 1.5)) +
    # scale_fill_discrete(breaks=c("FirstUpload", "SecondUpload", "Download"),
    #                         labels=c("1stUpload", "2ndUpload", "Download")) +
    # scale_linetype_manual(values=c("dashed","twodashed","dotted","dotdash")) +
    coord_cartesian(ylim=c(0, 1300)) +
    scale_y_continuous(breaks=seq(0, 1300, 325), labels=format(seq(0, 1300, 325), scientific=FALSE)) +
    scale_x_continuous(breaks=c(1,2,3,4,5,6,7,8,9,10), labels=format(c(1,2,3,4,5,6,7,8,9,10), scientific=FALSE)) + 
    ylab("Speed (MB/s)") +
    xlab("Number of Clients") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.8, colour="black", size=18),
        axis.title.y=element_text(size=20,  hjust=0.5),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=18),
        axis.title.x=element_text(size=20),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=25),
        # legend.position=c(0.35,1.1),
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
