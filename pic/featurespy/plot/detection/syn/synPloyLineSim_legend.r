library(grid)
library(ggplot2)
library(extrafont)
require("RColorBrewer")
require(scales)
font_import()
loadfonts()

mywidth=20
myheight=0.5
my_value=c(1,10,5,15)
my_line=c(1,2,3,4)

if(T){
    x1 <- read.table("fixed_q_16.data",header=TRUE)
    cairo_pdf(file="fixed_pq_legend.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("1F","2F","3F","4F"), labels=c("One","Two","Three","Four"))
    my_hist=ggplot(data=x1, aes(x=ID, y=Ratio,shape=Type, linetype=Type, colour=Type, group=Type)) +
    geom_line(size=2)  +
    geom_point(size=6, stroke=1.5, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_colour_brewer(palette = "Set1")+
    scale_linetype_manual(values=c(my_line)) +
    coord_cartesian(ylim=c(0, 1), xlim=c(0, 3)) +
    scale_y_continuous(breaks=seq(0, 1, 0.25), labels=format(seq(0, 100, 25), scientific=FALSE)) +
    scale_x_continuous(breaks=seq(0, 3, 1), labels=c("1", "2", "4", "8")) +
    ylab("Ratio (%)") +
    xlab(NULL) +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(),
        panel.grid.minor=element_blank(),
	    panel.background=element_blank(),
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.title.x=element_text(size=22),
        axis.text.x=element_text(margin=margin(5,0,0,0), hjust=0.8, colour="black", size=22),
        axis.title.y=element_text(size=20,  hjust=0.5),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=22),
        legend.title = element_blank(),
        legend.key.size=unit(2, "cm"),
        legend.text=element_text(size=22),
        # legend.position="none",
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
