library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=4
myheight=1
my_color=c("goldenrod4", "green3")
my_value=c(5,1)

abbrev_x <- c(1,expression(2^3), expression(2^6),expression(2^9),expression(2^12),expression(2^15))


if(T){
    x1 <- read.table("expa2_keyEnclaveBatchSize_2nd_data.txt",header=TRUE)
    cairo_pdf(file="../../expa2_keyEnclaveBatchSize_Performance_twoRound_Legend.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$Type, levels=c("Client","Enclave"))
    my_hist = ggplot(data=x1, aes(x=as.factor(BatchSize), y=Performance,shape=Type, linetype=Type, colour=Type, group=type), log= "x") +
    geom_line(size=1.5)  + 
    geom_point(size=4, stroke=1.5, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(1, 2, 3, 5, 4, 6)) +              
    coord_cartesian(ylim=c(0, 2.5)) +
    scale_y_continuous(breaks=seq(0, 2.5, 0.5), labels=format(seq(0, 2.5, 0.5), scientific=FALSE)) +
    scale_x_discrete(labels=abbrev_x, breaks=c("0", "1", "2", "3", "4", "5")) + 
    ylab("Time (ms/MiB)") +
    xlab("Fingerprint Batch Size") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), 
        panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.title.x=element_text(size=18),
        axis.text.x=element_text(margin=margin(5,0,0,0), hjust=0.8, colour="black", size=18),
        axis.title.y=element_text(size=18,  hjust=0.5),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=18),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=18),
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
