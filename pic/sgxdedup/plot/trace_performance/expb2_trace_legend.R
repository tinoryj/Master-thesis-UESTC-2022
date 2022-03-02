library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=6.8
myheight=0.4
my_color=c("#B2182B","#D6604D","#2166AC","#4393C3")
my_value=c(1,5,1,5)
my_line=c("dashed","solid","dashed","solid")


if(T){
    x1 <- read.table("expb2_trace_ms_data_plain_sgx.txt",header=TRUE)
    cairo_pdf(file="../../expb2_trace_legend.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("SGXUpload", "SGXDownload", "PlainUpload", "PlainDownload"), labels=c("TEEDedup-Upload", "TEEDedup-Download", "PlainDedup-Upload", "PlainDedup-Download"))
    # x1$Type <- factor(x1$Type, levels=c("SGXUpload", "SGXDownload", "PlainUpload", "PlainDownload"), labels=c("TEEDedup\n    Upload", "TEEDedup\n Download", "PlainDedup\n    Upload", "PlainDedup\n Download"))
    my_hist = ggplot(data=x1, aes(x=as.factor(BatchSize), y=Performance,shape=Type, linetype=Type, colour=Type, group=Type), log= "x") +
    geom_line(size=1)  + 
    geom_point(size=3, stroke=1, fill="white") +
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
	    panel.border = element_blank(),
        legend.title = element_blank(),
        legend.key.size=unit(0.3, "cm"),
        legend.text=element_text(size=11),
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

