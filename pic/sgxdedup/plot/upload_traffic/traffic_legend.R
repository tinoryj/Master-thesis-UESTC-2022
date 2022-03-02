library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=15
myheight=0.4
my_color=c("red3","dodgerblue3","Gold3")
my_value=c(1,1,1,1)
my_line=c("solid","longdash","dashed","longdash")

abbrev_x <- c(1,expression(2^3), expression(2^6),expression(2^9),expression(2^12),expression(2^15))

if(T){
    x1 <- read.table("result_fsl.txt",header=TRUE)
    cairo_pdf(file="../../upload_traffic_legend.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("Client-side", "Two-stage", "Random-threshold"), labels=c("TEEDedup", "Two-stage Dedup", "Randomized-threshold Dedup"))
    my_hist = ggplot(data=x1, aes(x=as.factor(ID), y=Traffic/1073741824,shape=Type, linetype=Type, colour=Type, group=Type), log= "x") +
    geom_line(size=2)  + 
    # geom_point(size=4, stroke=1.5, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) +
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(0, 125)) +
    scale_y_continuous(breaks=seq(0, 125, 25), labels=format(seq(0, 125, 25), scientific=FALSE)) +
    scale_x_discrete(labels=c("1","2","3","4","5"), breaks=seq(1,5,1)) + 
    ylab('Speed (MiB/s)') +
    xlab("Backup ID") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.key.width=unit(2,"cm"),
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

