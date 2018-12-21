#!/usr/bin/env Rscript

packages <- c('tidyverse', 'optparse', 'devtools', 'jsonlite', 'modules')
cran_repos <- 'file:///benchmarks/CRAN'
jobs <- 8

install.packages(packages, repos=cran_repos, Ncpus=jobs, clean=TRUE)
for(x in packages) {
  if (!require(x,character.only = TRUE)) {
    quit(1)
  }
}
