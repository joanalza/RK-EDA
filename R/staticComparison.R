library(gtools)


## MAIN ####
directory <- "C:/Project/Code/RKEDAC++/results/cluster/tai100/tai100_20/"
setwd(directory)

# Get files
files.progress <- list.files(directory, pattern = "*.csv")

# Get parameters

instances <- unlist(unique(sapply(strsplit(files.progress,"-"), FUN = function(x){
  return(x[grep("tai",x)])
})))

optimums <- read.csv("C:/Project/Code/RKEDAC++/optimums/optimum.csv")

# Get running files

instance.noChange.files <- unlist(lapply(instances, FUN = function(x){
  files <- list.files(directory, pattern = x)
  new.files <- files[grep("noChange",files)]
  return(list(new.files))
}), recursive = F)

# Run performance measures

data <- sapply(instance.noChange.files, FUN = function(x){
  instance <- unlist(strsplit(x[[1]],"-"))
  instance.name <- instance[grep("tai",instance)]
  mins <- sapply(x, FUN = function(no.change){
    df <- read.csv(no.change)
    return(tail(df,1)$bestFound)
  })
  best.known <- optimums[which(instance.name == optimums$Instance),"TotalFlowTime"] 
  ARPD <- mean(((mins - best.known)*100)/best.known)
  return(c("Instance" = instance.name, "Best known" = best.known, "ARPD" = ARPD))
})

data <- data.frame(t(data))

dir.name <- substr(getwd(),nchar(dirname(getwd()))+2,nchar(getwd()))
write.csv(data, paste0("C:/Project/Code/RKEDAC++/performanceMeasures/Static/BestKnowns_",dir.name,".csv"))
closeAllConnections()