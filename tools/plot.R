#!/usr/bin/env Rscript

#library(ggplot2)
library(jsonlite)
library(optparse)

option_list = list(
  make_option(c("-b", "--benchfile"), type="character", default=NA,
              help="preprocessed .json file to plot", metavar="character")
);

opt_parser = OptionParser(option_list=option_list);
opt = parse_args(opt_parser);

# check benchfile is provided
if (!is.na(opt$benchfile)) {
  if(!file.exists(opt$benchfile)) {
    stop("--benchfile file does not exist!", opt$benchfile)
  }
  # read url and convert to data.frame
  raw <- fromJSON(txt=opt$benchfile)
} else {
  stop("--benchfile parameter must be provided. See script usage (--help)")
}

for(type in names(raw)) {
  df <- t(do.call(rbind.data.frame, raw[[type]]))  # TODO: only works when every runtime has the same benchmarks (and no missing ones)

  boxplot(df, horizontal=T, notch=T, varwidth=T, main=type)

  #p <- ggplot(df, aes(variable, value))
  #p + geom_boxplot(notch = TRUE, varwidth=TRUE) + coord_flip()
}
