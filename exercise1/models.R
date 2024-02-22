# Set working directory and load data
setwd(dirname(rstudioapi::getSourceEditorContext()$path))

dfb6 <- read.csv("bcast_data/fixsize_b6_3set.csv", sep=",")
dfb6<-dfb6[-128,]
png("plots_model/model_b6_1.png", width =  1000, height =  700)

# Create the scatterplot
plot(dfb6$Processes, dfb6$Latency, pch =  19, xlab = "Processes", ylab = "Latency", cex = 1.5, cex.axis = 2, cex.lab=2)


#model with polinomial approximation
poli_3<-lm(Latency ~ poly(Processes,3, raw=TRUE),data=dfb6)
pred_poli_3<-predict(poli_3,newdata=dfb6)
lines(dfb6$Processes,pred_poli_3,col="yellow", lwd=2)

dev.off()
summary(poli_3)
plot(poli_3)

#----------------------------------------------------------------
dfb1 <- read.csv("bcast_data/fixsize_b1_3set.csv", sep=",")
dfb1 <- dfb1[-128,]
png("plots_model/model_b1.png", width = 1000, height = 700)
# Fit the linear model
linear_model_b1 <- lm(Latency ~ Processes, data = dfb1)

# Create the scatterplot
plot(dfb1$Processes, dfb1$Latency, pch = 19, xlab = "Processes", ylab = "Latency", cex = 1.5, cex.axis = 2, cex.lab=2)
# Add the regression line
abline(linear_model_b1, col = "blue", lwd = 2)
dev.off()
summary(linear_model_b1)
plot(linear_model_b1)




#-------------------------------------------------------------------
dfb5 <- read.csv("bcast_data/fixsize_b5_3set.csv", sep=",")
dfb5<-dfb5[-128,]
png("plots_model/model_b5.png", width =  1200, height =  800)
# Fit the linear model
#linear_model_b6 <- lm(Latency ~ Processes, data=dfb6)

# Create the scatterplot
plot(dfb5$Processes, dfb5$Latency, pch =  19, xlab = "Processes", ylab = "Latency", cex = 1.5, cex.axis = 2, cex.lab=2)
# Add the regression line

poli_deg3<-lm(Latency ~ I(Processes^3),data=dfb5)
pred_poli_deg3<-predict(poli_deg3,newdata=dfb5)
lines(dfb5$Processes,pred_poli_deg3,col="green", lwd=2)

dev.off()
summary(poli_deg3)
plot(poli_deg3)

#---------------------------------------------------------
dfb4 <- read.csv("bcast_data/fixsize_b4_3set.csv", sep=",")
dfb4<-dfb4[-128,]
png("plots_model/model_b4.png", width =  1200, height =  800)

# Create the scatterplot
plot(dfb4$Processes, dfb4$Latency, pch =  19, xlab = "Processes", ylab = "Latency", cex = 1.5, cex.axis = 2, cex.lab=2)
# Add the regression line
poli_deg3<-lm(Latency ~ I(Processes^2),data=dfb4)
pred_poli_deg3<-predict(poli_deg3,newdata=dfb4)

lines(dfb4$Processes,pred_poli_deg3,col="red", lwd=2)

dev.off()
summary(poli_deg3)
plot(poli_deg3)

