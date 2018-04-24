#### PERFORMANCE MEASURE FUNCTIONS ####


#' @description Gets the average of the best-of-generation of an algorithm after several runs.
#' @param Data: data.frame with the best fitness of each run as column.
#' @note Drawback: the result is not normalized.
#'
get_best.of.gen <- function(data){
  best.of.runs <- apply(data, MARGIN = 1, function(row){
    return(min(row))
  })
  return(best.of.runs)
}

#' @description Provides convergence behaviour of algorithms, range(0,1).
#' @param Data: data that show the changes and best since last change.
#' @param Optimum: the global optimum at every change.
#' @note Take into account the generations and the changes.
#'
get_ARR <- function(data, optimum){
  pi <- nrow(data)
  fbest.1 <- data$bestPerChange[1]
  dividend <- sapply(data$bestPerChange, FUN = function(fij){
    return(fij - fbest.1)
  })
  return(sum(dividend)/(pi * (optimum - fbest.1)))
}

#' @description Based on the relative ratio between the best value gained by an algorithm and the global optimum value.
#' @param Data: data that show the changes and best since last change.
#' @param Optimum: global optimum of the search space.
#' @param Runs: total number of runs.
#' @param Num.changes: total number of changes.
#' @note Drawback: the result is not normalized.
#' @note Drawback: time of change.
#'
get_relative.ratio <- function(data, runs, num.changes, optimum){
  n.gen.per.change <- nrow(data)
  f.last <- tail(data,1)
  r.last <- abs(optimum / f.last$bestPerChange )
  sum.rij <- sapply(data$bestFit, FUN = function(fi.best){
    r.s <- abs(optimum / fi.best)
    return((1 - r.s)/n.gen.per.change)
  })
  r.ij <- r.last / (1 + sum(sum.rij))
  return(r.ij/(num.changes * runs))
}

#' @description Diversity of the population
#' @param Data: data that show the changes and best since last change.
#' @param Runs: total number of runs.
#' @param Num.changes: total number of changes.
#' @note Drawback: the result is not normalized.
#' @note Drawback: time of change.
#'
get_distance.based.diversity<- function(data, n){
  n.Best <- unique(data$nBestInd)
  norm.diversity <- sapply(data$diversity, FUN = function(row){
    return(row/((n - 1) * (n.Best *(n.Best - 1)/2)))
  })
  return(mean(norm.diversity))
}

## BEST-OF-GENERATION ####
F.best.of.gen <- function(files){
  df.list <- lapply(files, FUN = function(run){
    df <- read.csv(run)
    return(df$bestFit)
  })
  runs <- sub("*-.csv","",sub("([a-zA-Z0-9//_//.][-]?)*-","",files))
  df.bog <- as.data.frame(df.list)
  colnames(df.bog) <- runs
  f.bog <- get_best.of.gen(df.bog)
  F.bog <- mean(f.bog)
  return(F.bog)
}

## ABSOLUTE RECOVERY RATE ####
F.ARR <- function(files, optimums){
  df.list <- sapply(files, FUN = function(run){
    instance <- unlist(strsplit(run,"-"))[grep("tai", unlist(strsplit(run,"-")))]
    optima <- optimums[which(instance == optimums$Instance),"TotalFlowTime"] 
    df <- read.csv(run)
    n.changes <- max(unique(df$change))
    if (n.changes!=1){
      ARR <- sapply(2:n.changes, FUN = function(i){
        sub.df <- df[df$change==i,]
        return(get_ARR(sub.df, optima))
      })
    }else{
      sub.df <- df[df$change==n.changes,]
      return(get_ARR(sub.df, optima))
    }
    ARR.mean <- mean(ARR) # mean of the formula
    return(ARR.mean)
  })
}

## RELATIVE RATIO ####
F.relative.ratio <- function(files, optimums){
  n.runs <- length(files)
  df.list <- sapply(files, FUN = function(run){
    instance <- unlist(strsplit(run,"-"))[grep("tai", unlist(strsplit(run,"-")))]
    optima <- optimums[which(instance == optimums$Instance),"TotalFlowTime"] 
    df <- read.csv(run)
    n.changes <- max(unique(df$change))
    relative.ratio <- sapply(1:n.changes, FUN = function(i){
      sub.df <- df[df$change==i,]
      return(get_relative.ratio(sub.df, n.runs, n.changes,optima))
    })
    return(sum(relative.ratio))
  })
  return(sum(df.list))
}

## DISTANCE-BASED MEASURE ####
F.distance.based.measure <- function(files, n){
  df.list <- sapply(files, FUN = function(run){
    df <- read.csv(run)
    P.diversity <- get_distance.based.diversity(df,n)
    return(P.diversity)
  })
  return(mean(df.list))
}

## MAIN ####
directory <- "C:/Project/Code/RKEDAC++/results/cluster/SEED_1/noChange/"
setwd(directory)

# Get files
files.progress <- list.files(directory, pattern = "*.csv")
#files.best <- list.files(directory, pattern = "best*")
#files.improvement <- list.files(directory, pattern = "improvement*")

# Get parameters
cooling.param <- unlist(unique(sapply(strsplit(files.progress,"-"), FUN = function(x){
  return(x[grep("(curr|over|ori)",x)])
})))

instances <- unlist(unique(sapply(strsplit(files.progress,"-"), FUN = function(x){
  return(x[grep("tai",x)])
})))

optimums <- read.csv("C:/Project/Code/RKEDAC++/optimums/optimum.csv")

# Run performance measures

run.by <- cooling.param

best.of.gen <- sapply(run.by, FUN = function(x){
  files <- list.files(directory, pattern = x)
  progress.files <- files[grep(".csv",files)]
  return(F.best.of.gen(progress.files))
})

rank.bog <- rank(best.of.gen)

mean.ARR <- sapply(run.by, FUN = function(x){
  files <- list.files(directory, pattern = x)
  progress.files <- files[grep(".csv",files)]
  ARR <- F.ARR(progress.files, optimums)
  return(mean(ARR))
})

rank.ARR <- rank(-mean.ARR) # DESCENDING ORDER

relative.ratio <- sapply(run.by, FUN = function(x){
  files <- list.files(directory, pattern = x)
  progress.files <- files[grep(".csv",files)]
  return(F.relative.ratio(progress.files,optimums))
})

rank.rr <- rank(-relative.ratio)

isDiversityCalculated <- names(read.csv(files.progress[1])=="diversity")

if (is.null(isDiversityCalculated)){
  df.ranks <- data.frame(rank.bog,rank.ARR, rank.rr)
  
  average.rank <- apply(df.ranks,1,mean)
  
  # Get results as csv
  df.pm <- data.frame(best.of.gen, rank.bog, mean.ARR, rank.ARR, relative.ratio, rank.rr, average.rank)
}else{
  distance.based.measure <- sapply(run.by, FUN = function(x){
    files <- list.files(directory, pattern = x)
    progress.files <- files[grep(".csv",files)]
    n <- as.integer(sub("[a-z]*","",unlist(strsplit(progress.files[1],"-"))[grep("n[0-9]+",unlist(strsplit(progress.files[1],"-")))]))
    return(F.distance.based.measure(progress.files,n))
  })
  
  rank.dbm <- rank(-distance.based.measure)
  
  df.ranks <- data.frame(rank.bog,rank.ARR, rank.rr, rank.dbm)
  
  average.rank <- apply(df.ranks,1,mean)
  
  # Get results as csv
  df.pm <- data.frame(best.of.gen, rank.bog, mean.ARR, rank.ARR, relative.ratio, rank.rr,distance.based.measure, rank.dbm, average.rank)
}

df.pm <- df.pm[order(df.pm$average.rank),]
dir.name <- substr(getwd(),nchar(dirname(getwd()))+2,nchar(getwd()))
write.csv(df.pm, paste0("C:/Project/Code/RKEDAC++/performanceMeasures/",dir.name,".csv") ,row.names = TRUE)
closeAllConnections()
