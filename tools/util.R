#!/usr/bin/env Rscript

library(tidyverse)
library(optparse)
library(jsonlite, warn.conflicts=FALSE)

devtools::install_github('klmr/modules', ref='version-0.9.10')

# parse arguments and check for errors in them
parse_script_args <- function(outputfile_arg=FALSE) {
  option_list = list(
    make_option(c("-b", "--benchfile"), type="character", default=NA,
                help="preprocessed .json file to plot", metavar="character"),
    make_option(c("-r", "--base-runtime"), type="character", default=NA,
                help="runtime which specifies the base", metavar="character")
  );

  if(isTRUE(outputfile_arg)) {
    option_list <- c(option_list,
                     make_option(c("-o", "--outputfile"), type="character", default=NA,
                                 help="name of the output", metavar="character"));
  }

  opt_parser = OptionParser(option_list=option_list);
  opt = parse_args(opt_parser);

  args <- list()

  # check benchfile is provided
  if (!is.na(opt$benchfile)) {
    if(!file.exists(opt$benchfile)) {
      stop("file does not exist: ", opt$benchfile)
    }
    # read url and convert to data.frame
    args[['raw']] <- fromJSON(txt=opt$benchfile)
  } else {
    stop("--benchfile parameter must be provided. See script usage (--help)")
  }

  if (!is.na(opt$'base-runtime')) {
    args[['base_runtime']] <- opt$'base-runtime'
  } else {
    stop("--base-runtime parameter must be provided. See script usage (--help)")
  }

  if(outputfile_arg == TRUE) {
    args[['outputfile']] <- opt$outputfile
  }

  return(args)
}

# convert data into a flat table
priv_flatten_data <- function(raw) {
  res <- data.frame()
  for(benchmark in names(raw)) {
    raw_benchmark <- raw[[benchmark]]
    df_benchmark <- data.frame()
    for (config in names(raw_benchmark)) {
      df_benchmark_config <- raw_benchmark[[config]] %>%
        select(-mean, -std_dev, -iterations_per_run, -disabled) %>%
        mutate(run=row_number()) %>%
        unnest() %>%
        group_by(run) %>%
        mutate(idx=row_number()) %>%
        ungroup() %>%
        mutate(config=config)
      df_benchmark <- bind_rows(df_benchmark, df_benchmark_config)
    }
    res <- bind_rows(res, df_benchmark) # %>% mutate(benchmark=benchmark))
  }

  return(res)
}

# read data and output it in a flat scheme
parse_data <- function(raw) {
  if ("benchmark_data" %in% names(raw)) {
    benchmark_data = raw[['benchmark_data']]
  } else {
    benchmark_data = raw  # support old format as well
  }

  df = priv_flatten_data(benchmark_data)

  print("runtimes in the dataset:")
  df$config %>% unique()

  return(df)
}

create_long <- function(df) {
  # create a long table to allow the following operations
  df_long = df %>%
    mutate(benchmark=paste(fixture, name, sep=".")) %>%
    gather(key=metric_name, value=value, -fixture, -name, -clock_type, -clock_resolution, -clock_resolution_measured, -benchmark, -harness, -run, -idx, -config) %>%
    filter(!is.numeric(value) || !is.na(value))

  return(df_long)
}

create_long_summary <- function(df_long) {
  # calculate mean and sum over all runs
  df_long_summary = df_long %>%
    group_by(benchmark, config, metric_name) %>%
    summarise(runs=n(), value_mean=mean(value), value_sum=sum(value), value_sd=sd(value)) %>%
    ungroup()

  return(df_long_summary)
}

create_long_baseline <- function(df_long_summary, base_runtime) {
  if(!(base_runtime %in% df_long_summary$config)) {
    stop("given --base-runtime name does not exist")
  }

  # add baseline to allow plotting relative to baseline
  df_long_baseline = df_long_summary  %>%
    group_by(benchmark, metric_name) %>%
    mutate(baseline_runs=runs[config==base_runtime]) %>%
    mutate(value_mean_factor=value_mean/value_mean[config==base_runtime]) %>%
    mutate(value_sd_factor=value_sd/value_mean[config==base_runtime]) %>%  # TODO: correct?
    ungroup()

  return(df_long_baseline)
}
