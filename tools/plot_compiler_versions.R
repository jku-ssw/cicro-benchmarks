#!/usr/bin/env Rscript

library(tidyverse)
library(jsonlite)
library(optparse)

option_list = list(
  make_option(c("-b", "--benchfile"), type="character", default=NA,
              help="preprocessed .json file to plot", metavar="character"),
  make_option(c("-r", "--base-runtime"), type="character", default=NA,
              help="runtime which specifies the base", metavar="character"),
  make_option(c("-o", "--outputfile"), type="character", default=NA,
              help="name of the output", metavar="character")
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

if (!is.na(opt$'base-runtime')) {
  BASE_RUNTIME <- opt$'base-runtime'
} else {
  stop("--base-runtime parameter must be provided. See script usage (--help)")
}

flatten_data <- function(raw) {
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
  res
}

if ("benchmark_data" %in% names(raw)) {
  benchmark_data = raw[['benchmark_data']]
} else {
  benchmark_data = raw  # support old format as well
}

df = flatten_data(benchmark_data)

print("runtimes in the dataset:")
df$config %>% unique()

# create a long table to allow the following operations
df_long = df %>%
  mutate(benchmark=paste(fixture, name, sep=".")) %>%
  gather(key=metric_name, value=value, -fixture, -name, -clock_type, -clock_resolution, -clock_resolution_measured, -benchmark, -harness, -run, -idx, -config) %>%
  filter(!is.numeric(value) || !is.na(value))

# calculate mean and sum over all runs
df_long_summary = df_long %>%
  group_by(benchmark, config, metric_name) %>%
  summarise(runs=n(), value_mean=mean(value), value_sum=sum(value), value_sd=sd(value)) %>%
  ungroup()

if(!(BASE_RUNTIME %in% df$config)) {
  stop("given --base-runtime name does not exist")
}

# add baseline to allow plotting relative to baseline
df_long_baseline = df_long_summary  %>%
  group_by(benchmark, metric_name) %>%
  mutate(baseline_runs=runs[config==BASE_RUNTIME]) %>%
  mutate(value_mean_factor=value_mean/value_mean[config==BASE_RUNTIME]) %>%
  mutate(value_sd_factor=value_sd/value_mean[config==BASE_RUNTIME]) %>%  # TODO: correct?
  ungroup()

###################################################################################
# TODO: above part of the script is a copy of plot.R -> refactor (except the outputfile option)

normalized_duration_data <- df_long_baseline %>% filter(metric_name == 'duration')

geo_mean <- function(data) {
    log_data <- log(data)
    gm <- exp(mean(log_data[is.finite(log_data)]))
    return(gm)
}
means <- aggregate(value_mean_factor ~  config, normalized_duration_data, geo_mean)
means$value_mean_factor <- round(means$value_mean_factor, digits=2)

p <- ggplot(normalized_duration_data, aes(x=config, y=value_mean_factor)) +
    geom_boxplot(notch = TRUE, varwidth=TRUE) +
    scale_x_discrete("") +
    scale_y_continuous("normalized execution time") +
    stat_summary(fun.y=geo_mean, colour="darkred", geom="point", shape=18, size=3, show_guide = FALSE) +
    geom_text(data = means, aes(label = value_mean_factor, vjust=-3)) +
    theme(axis.text.x = element_text(angle = 45, hjust = 1))

if (is.na(opt$outputfile)) {
    print(p)
} else {
    ggsave(opt$'outputfile', device="pdf")
}


