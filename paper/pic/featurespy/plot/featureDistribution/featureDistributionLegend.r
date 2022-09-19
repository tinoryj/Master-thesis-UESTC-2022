library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=6
myheight=0.2
abbrev_x <- c(expression(10^0), expression(10^1),expression(10^2),expression(10^3),expression(10^4))
if(T){
    x1 <- read.table("featureDistributionLinux.data",header=TRUE)
    cairo_pdf(file="./featureDistributionLegend.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$wsize, levels=c("1K", "5K", "10K"), labels=c("W=1K", "W=5K", "W=10K"))
    my_hist = ggplot(data=x1, aes(x=id, y=ndiff,shape=type,linetype=type, colour=type, group=type), log="x") +
    geom_line(size=1)  +
    scale_colour_brewer(palette = "Set1")+
    coord_cartesian(ylim=c(0.00212, 0.045), xlim=c(1.53,8500)) +
    scale_x_continuous(trans='log10', breaks=trans_breaks("log10", function(x) 10^x),
    labels=trans_format("log10", math_format(10^.x))) +
    scale_y_continuous(breaks=c(0.000,0.02,0.04), labels=c("0","0.02","0.04")) +
    ylab("标准化差值") +
    xlab("窗口") +
    theme_bw() +
    theme(        panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.key.width=unit(1,"cm"),
        legend.text=element_text(size=10.5,family="Times New Roman"),
        # legend.position=c(0.35,1.1),
        legend.direction="horizontal",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0,0,0,0), "cm")
    ) +guides(shape=guide_legend(nrow=2))
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
