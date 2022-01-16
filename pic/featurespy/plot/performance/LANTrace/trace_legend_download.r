library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=8
myheight=0.4
my_value=c(5,6)
my_line=c("solid","solid")
my_color=c("#a6d854", "#ffd92f")

if(T){
    x1 <- read.table("legend_download.data",header=TRUE)
    cairo_pdf(file="trace_legend_download.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("SGXDedupDownload","AverageDownload"),labels=c("SGXDedup Download","SGXDedup+ Download"))
    my_hist = ggplot(data=x1, aes(x=as.factor(BatchSize), y=Performance,shape=Type, linetype=Type, colour=Type, group=Type)) +
    geom_line(size=1.5)  + 
    geom_point(size=4, stroke=1.5, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_manual(values=c(my_color)) +
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
	    panel.border = element_blank(),
        legend.title = element_blank(),
        legend.key.size=unit(1, "cm"),
        legend.text=element_text(size=12),
        legend.direction="horizontal",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0,0,0,0), "cm")
    )+guides(shape=guide_legend(ncol=2))
    g_legend <- function(a.gplot){
    tmp <- ggplot_gtable(ggplot_build(a.gplot))
    leg <- which(sapply(tmp$grobs, function(x) x$name) == "guide-box")
    legend <- tmp$grobs[[leg]]
    return(legend)}
    legend <- cowplot::get_legend(my_hist)
    grid.newpage()
    grid.draw(legend)
}

