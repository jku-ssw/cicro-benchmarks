#!/usr/bin/env Rscript

library(ggplot2)
library(jsonlite)
library(optparse)

option_list = list(
  make_option(c("-b", "--benchfile"), type="character", default=NA,
              help="resuls .json file to evaluate", metavar="character"),
  make_option(c("-r", "--base-runtime"), type="character", default=NA,
              help="runtime which specifies the base", metavar="character")
);

opt_parser = OptionParser(option_list=option_list);
opt = parse_args(opt_parser);

# check benchfile is provided
if (!is.na(opt$benchfile)) {
  # read url and convert to data.frame
  raw <- fromJSON(txt=opt$benchfile)
} else {
  stop("--benchfile parameter must be provided. See script usage (--help)")
}

if (!is.na(opt$'base-runtime')) {
  BASE_RUNTIME <- opt$'base-runtime'
} else {
  stop("--base-runtime parameter must be provided. See script usage (--help)")
}

results <- list()
results_mean <- list()
results_mean_transposed <- list()

for(bench in names(raw)) {
  for(runtime in names(raw[[bench]])) {
    for(type in names(raw[[bench]][[runtime]][['runs']])) {
      if(is.null(raw[[bench]][[BASE_RUNTIME]])) {
        stop("base runtime run not found for benchmark: ", bench)
      }

      # we normalize the values so we can compare them
      base_mean = mean(raw[[bench]][[BASE_RUNTIME]][['runs']][[type]]);
      our_mean = mean(raw[[bench]][[runtime]][['runs']][[type]]);

      # normalized results per benchmark
      results[[type]][[runtime]][[bench]] <- raw[[bench]][[runtime]][['runs']][[type]] / base_mean;

      # normalized results per runtime
      if(is.null(results_mean[[type]][[runtime]])) {
        results_mean[[type]][[runtime]] = list(); # create list entry
      }
      results_mean[[type]][[runtime]][[bench]] <- our_mean / base_mean;

      # normalized results per runtime (transposed to create correct data.frame)
      if(is.null(results_mean_transposed[[type]][[bench]])) {
        results_mean_transposed[[type]][[bench]] = list(); # create list entry
      }
      results_mean_transposed[[type]][[bench]][[runtime]] <- our_mean / base_mean;
    }
  }
}

for(type in names(results_mean)) {
  df <- do.call(rbind.data.frame, results_mean_transposed[[type]])

  boxplot(df, horizontal=T, notch=T, main=type)

  #p <- ggplot(df, aes(variable, value))
  #p + geom_boxplot(notch = TRUE, varwidth=TRUE) + coord_flip()
}



