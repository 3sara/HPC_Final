# Set working directory and load data
setwd(dirname(rstudioapi::getSourceEditorContext()$path))

s0 <- read.csv("scatter_data/scatterfix0.csv", sep=",")
s0<-s0[-128,]
png("plots_model/model_s0.png", width =  1000, height =  700)

# Create the scatterplot
plot(s0$Processes, s0$Latency, pch =  19, xlab = "Processes", ylab = "Latency", cex = 1.5, cex.axis = 2, cex.lab=2)


#model with polinomial approximation
poli_3<-lm(Latency ~ poly(Processes,1),data=s0)
pred_poli_3<-predict(poli_3,newdata=s0)
lines(s0$Processes,pred_poli_3,col="pink", lwd=2)

dev.off()
summary(poli_3)
plot(poli_3)

#---------------------------------------------------------------------------------------
s1<- read.csv("scatter_data/scatterfix1.csv", sep=",")
s1<-s1[-128,]
png("plots_model/model_s1.png", width =  1000, height =  700)

# Create the scatterplot
plot(s1$Processes, s1$Latency, pch =  19, xlab = "Processes", ylab = "Latency", cex = 1.5, cex.axis = 2, cex.lab=2)


#model with polinomial approximation
poli_3<-lm(Latency ~ Processes,data=s1)

pred_poli_3<-predict(poli_3,newdata=s1)
lines(s1$Processes,pred_poli_3,col="green", lwd=2)

dev.off()
summary(poli_3)
plot(poli_3)

#------------------------------------------------------------------------------------
s2<- read.csv("scatter_data/scatterfix2.csv", sep=",")
s2<-s2[-128,]
png("plots_model/model_s2.png", width =  1000, height =  700)

# Create the scatterplot
plot(s2$Processes, s2$Latency, pch =  19, xlab = "Processes", ylab = "Latency", cex = 1.5, cex.axis = 2, cex.lab=2)

lin<-lm(Latency~ Processes,data=s2)
pred_lin<-predict(lin,newdata=s2)
lines(s2$Processes,pred_lin,col="blue",lwd=2)
dev.off()
summary(lin)
plot(lin)
#----------------------------------------------------------------------------------
s3<- read.csv("scatter_data/scatterfix3.csv", sep=",")
s3<-s3[-128,]
png("plots_model/model_s3.png", width =  1000, height =  700)

# Create the scatterplot
plot(s3$Processes, s3$Latency, pch =  19, xlab = "Processes", ylab = "Latency", cex = 1.5, cex.axis = 2, cex.lab=2)


#model with polinomial approximation
poli_3<-lm(Latency ~ Processes,data=s3)
pred_poli_3<-predict(poli_3,newdata=s3)
lines(s3$Processes,pred_poli_3,col="red", lwd=2)

dev.off()
summary(poli_3)
plot(poli_3)
