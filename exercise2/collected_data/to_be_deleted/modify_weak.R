setwd(dirname(rstudioapi::getSourceEditorContext()$path))
waek_scaling <- read.csv("weak_scaling.csv", sep=",")
waek_scaling<-waek_scaling[, -2]
write.csv(waek_scaling, file="weak_scaling_ordered.csv", row.names = FALSE)
