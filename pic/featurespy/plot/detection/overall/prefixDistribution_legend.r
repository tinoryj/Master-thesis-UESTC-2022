library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=10
myheight=0.4
my_line=c("dashed","solid")

if(T){
    args <- commandArgs(trailingOnly = TRUE)
    x1 <- read.table("prefixDistribution_legend.data",header=TRUE)
    cairo_pdf(file="prefixDistribution_legend.pdf", width=mywidth, height=myheight)
    x1$type <- factor(x1$type, levels=c("Raw","Mixed"), labels=c("不发生攻击","发生攻击"))
    my_hist=ggplot(data=x1, aes(x=ID,y=ratio,shape=type,linetype=type, colour=type)) +
    geom_line(size=1.5)  + 
    scale_colour_brewer(palette = "Set2") + 
    scale_linetype_manual(values=c(my_line)) +
    scale_size_manual(values=c(1.5, 1.5)) +
    coord_cartesian(ylim=c(0.0005, 0.15)) +
    scale_y_continuous(breaks=seq(0, 0.15, 0.05), labels=format(seq(0, 0.15, 0.05), scientific=FALSE)) +
    ylab("频率 (%)") +
    xlab("窗口编号") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.text.x=element_text(margin=margin(5,0,0,0), angle=0, hjust=0.8, colour="black", size=30, family="Times New Roman"),
        axis.title.y=element_text(size=26,  hjust=0.5, family="SimSun"),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0),colour="black",size=30, family="Times New Roman"),
        axis.title.x=element_text(size=30, family="SimSun"),
        legend.title = element_blank(),
        legend.key.size=unit(2, "cm"),
        legend.text=element_text(size=20,family="SimSun"),
        # legend.position=c(0.3,0.82),
        # legend.position="none",
        legend.direction="horizontal",
        legend.margin = margin(t = 0, unit='cm'),
        plot.margin=unit(c(0.1,0.1,0.1,0.1), "cm")
    )
    g_legend <- function(a.gplot){
        tmp <- ggplot_gtable(ggplot_build(a.gplot))
        leg <- which(sapply(tmp$grobs, function(x) x$name) ==   "guide-box")
        legend <- tmp$grobs[[leg]]
        return(legend)}
        legend <- g_legend(my_hist)
        # or use cowplot package
        # legend <- cowplot::get_legend(my_hist)
        grid.newpage()
        grid.draw(legend)
}
