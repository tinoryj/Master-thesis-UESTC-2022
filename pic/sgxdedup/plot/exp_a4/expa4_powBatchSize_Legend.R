library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=6
myheight=0.4
my_color=c("red3", "green3", "cadetblue4")
my_value=c(1,1,5,10,15)
my_line=c("solid","dashed","solid")
# abbrev_x <- c(expression(2^0),expression(2^3), expression(2^6),expression(2^9),expression(2^12),expression(2^15))

if(T){
    x1 <- read.table("expa4_powBatchSize_legend_data.txt",header=TRUE)
    cairo_pdf(file="../../expa4_powBatchSize_legend.pdf", width=mywidth, height=myheight)
    x1$TypeName <- factor(x1$TypeName, levels=c("OverallPoW", "PoWEnclave", "Cloud"))
    my_hist = ggplot(data=x1, aes(x=as.factor(ID), y=Performance,shape=TypeName, linetype=TypeName, colour=TypeName, group=TypeName), log= "x") +
    geom_line(size=1.5)  + 
    geom_point(size=4, stroke=1.5, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(my_line))+
    # coord_cartesian(ylim=c(0, 370)) +
    # scale_y_continuous(breaks=seq(0, 360, 60), labels=format(seq(0, 360, 60), scientific=FALSE)) +
    # scale_x_discrete(labels=abbrev_x, breaks=c("0", "3", "6", "9", "12", "15")) + 
    # scale_fill_manual(name="", breaks=c("OverallPoW", "PoWEnclave", "Cloud"),labels=c("OverallPoW", "PoWEnclave", "Cloud"))+
    # ylab('Speed (MiB/s)') +
    # xlab("Ciphertext Batch Size") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=22),
        # legend.position=c(0.35,0.6),
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
