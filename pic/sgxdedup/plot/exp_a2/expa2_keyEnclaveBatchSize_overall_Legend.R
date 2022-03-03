library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=3
myheight=0.4
my_color=c("red3","coral3","dodgerblue3","cadetblue1")
my_value=c(1,10,5,15)
if(T){
    x1 <- read.table("expa2_keyEnclaveBatchSize_overall_data.txt",header=TRUE)
    cairo_pdf(file="../../expa2_keyEnclaveBatchSize_Performance_overall_Legend.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$Type, levels=c("Online","Offline"))
    my_hist=ggplot(data=x1, aes(x=BatchSize,y=Performance,shape=Type,linetype=Type, colour=Type)) +
    geom_line(size=1.5)  + 
    geom_point(size=4, stroke=1.5, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    # scale_fill_discrete(breaks=c("FirstUpload", "SecondUpload", "Download"),
    #                         labels=c("1stUpload", "2ndUpload", "Download")) +
    # scale_linetype_manual(values=c("dashed","twodashed","dotted","dotdash")) +
    coord_cartesian(ylim=c(0, 8000)) +
    scale_y_continuous(breaks=seq(0, 8000, 2000), labels=format(seq(0, 8000, 2000), scientific=FALSE)) +
    scale_x_continuous(breaks=c(1,10,100,500,1000,1500,2000,2500,3000,3500,4000), labels=format(c(1,10,100,500,1000,1500,2000,2500,3000,3500,4000), scientific=FALSE)) + 
    ylab("Speed (GiB/s)") +
    xlab("Key Enclave Batch Size") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=18),
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
