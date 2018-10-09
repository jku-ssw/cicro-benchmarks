#!/usr/bin/env Rscript

cran_repos <- 'http://cran.rstudio.com/'

# tidyverse
install.packages('tidyverse', repos=cran_repos)

# optparse
install.packages('optparse', repos=cran_repos)

# jsonlite
install.packages('jsonlite', repos=cran_repos)

# modules
devtools::install_github('klmr/modules', ref='version-0.9.10')
