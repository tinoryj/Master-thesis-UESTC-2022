library(grid)
library(ggplot2)
library(extrafont)
require(scales)
font_import()
loadfonts()

mywidth=6
myheight=0.3
my_color=c("red3","coral3", "dodgerblue3")
my_value=c(1,10,5,15)
my_line=c("solid", "dashed","solid")

# abbrev_x <- c(1,8, 64, 512, 4096, 32768)
abbrev_x <- c(1,expression(2^3), expression(2^6),expression(2^9),expression(2^12),expression(2^15))

if(T){
    x1 <- read.table("expa2_keyEnclaveBatchSize_1st_data.txt",header=TRUE)
    cairo_pdf(file="./expa2_keyEnclaveBatchSize_legend.pdf", width=mywidth, height=myheight)
    # cairo_pdf(file="./expa2_keyEnclaveBatchSize_Performance_1st.pdf", width=mywidth, height=myheight)
    x1$Type <- factor(x1$Type, levels=c("KeyEnclave-1st", "KeyEnclave-2nd","Client"), labels=c("密钥安全区(第一轮)", "密钥安全区(第二轮)","客户端"))
    my_hist=ggplot(data=x1, aes(x=as.factor(BatchSize), y=Performance,shape=Type, linetype=Type, colour=Type, group=Type), log= "x") +
    geom_line(size=1)  + 
    geom_point(size=2, stroke=0.75, fill="white") +
    scale_shape_manual(values=c(my_value)) +
    scale_color_manual(values=c(my_color)) + 
    scale_linetype_manual(values=c(my_line)) +              
    coord_cartesian(ylim=c(0, 2.1)) +
    scale_y_continuous(breaks=seq(0, 2.0, 0.5), labels=c("0","0.5","1.0","1.5","2.0")) +
    scale_x_discrete(labels=abbrev_x, breaks=c("0", "3", "6", "9", "12", "15")) + 
    ylab("时间 (ms)") +
    xlab("指纹批量大小") +
    theme_bw() +
    theme(
        panel.grid.major=element_blank(), 
        panel.grid.minor=element_blank(),
	    panel.background=element_blank(), 
	    # panel.border=element_rect(size=0.5),
	    panel.border = element_blank(),
	    axis.line=element_line(colour="black", size=0.3),
	    axis.ticks=element_line(size=0.3),
        axis.title.x=element_text(size=11,family="Times New Roman"),
        axis.text.x=element_text(margin=margin(5,0,0,0), hjust=0.8, colour="black", size=11,family="Times New Roman"),
        axis.title.y=element_text(size=11,  hjust=0.5,family="Times New Roman"),
        axis.text.y=element_text(margin=margin(0, 2, 0, 0), colour="black",size=11,family="Times New Roman"),
        legend.title = element_blank(),
        legend.key.size=unit(0.8, "cm"),
        legend.text=element_text(size=11,family="Times New Roman"),
        legend.position=c(0.6,0.75),
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
