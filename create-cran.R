#!/usr/bin/env Rscript
cran_repos <- 'http://cran.rstudio.com/'
#install.packages('miniCRAN', repos = cran_repos)

library(miniCRAN)

packages <- c('tidyverse', 'optparse', 'devtools', 'jsonlite', 'testthat', 'roxygen2')

# get all requires packages
pkgList <- pkgDep(packages, repos = cran_repos, type = "source", suggests = FALSE)

# Create folder for miniCRAN
pth <- file.path("CRAN")
if (!file.exists(pth))
  dir.create(pth)

# Make repo for source
makeRepo(pkgList, path = pth, repos = cran_repos, type = "source")

# adopted from https://github.com/andrie/miniCRAN/issues/50#issuecomment-374624319
addGithubPackage <- function(githubPath, ref='latest', version=ref, ...){
  packageName <- basename(githubPath)
  tmpdir <- file.path(tempdir(), "git-packages")
  if(!file.exists(tmpdir))
    dir.create(tmpdir)
  exDir <- file.path(tmpdir, packageName)
  if(file.exists(exDir))
    unlink(exDir, recursive=TRUE)
  zipFile <- file.path(tmpdir,paste0(packageName,".zip"))
  download.file(paste0(githubPath,"/","zipball/", ref), zipFile)
  unzip(zipFile, exdir = exDir)
  exPackage <- file.path(exDir,packageName)
  file.rename(list.files(exDir,full.names=TRUE)[1], exPackage)
  # create tar file in expexted format
  tarFile <- file.path(tmpdir, paste0(packageName, "_", version, ".tar.gz"))
  oldwd <- getwd()
  setwd(exDir)
  tar(tarFile, packageName)
  setwd(oldwd)
  addLocalPackage(packageName, tmpdir,...)
}

addGithubPackage(githubPath="https://github.com/klmr/modules", ref="version-0.9.10", version="0.9.10", path=pth)
