#!/usr/bin/env Rscript

packages <- c('tidyverse', 'optparse', 'devtools', 'jsonlite')
cran_repos <- 'http://cran.rstudio.com/'
jobs <- 8

install.packages(packages, repos=cran_repos, Ncpus=jobs)

# modules
devtools::install_github('klmr/modules', ref='version-0.9.10', threads=jobs)