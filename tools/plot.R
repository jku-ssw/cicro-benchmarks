#!/usr/bin/env Rscript

library(tidyverse)
library(jsonlite)
library(optparse)

option_list = list(
  make_option(c("-b", "--benchfile"), type="character", default=NA,
              help="preprocessed .json file to plot", metavar="character"),
  make_option(c("-r", "--base-runtime"), type="character", default=NA,
              help="runtime which specifies the base", metavar="character")
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
  gather(key=metric_name, value=value, -fixture, -name, -benchmark, -harness, -run, -idx, -config) %>%
  filter(!is.na(value))

# calculate mean and sum over all runs
df_long_summary = df_long %>%
  group_by(benchmark, config, metric_name) %>%
  summarise(runs=n(), value_mean=mean(value), value_sum=sum(value), value_sd=sd(value)) %>%
  ungroup()

# print IPC
if('PAPI_TOT_INS' %in% df_long_summary$metric_name && 'PAPI_TOT_CYC' %in% df_long_summary$metric_name ) {
  print("plot instructions per cycle")
  df_long_baseline_ipc = df_long_summary %>%
    group_by(benchmark, config) %>%
    filter('PAPI_TOT_INS' %in% metric_name && 'PAPI_TOT_CYC' %in% metric_name) %>%
    transmute(ipc=value_mean[metric_name=='PAPI_TOT_INS'] / value_mean[metric_name=='PAPI_TOT_CYC']) %>%
    unique() %>%
    ungroup()

  p <- ggplot(df_long_baseline_ipc, aes(x=config, y=ipc)) +
    geom_boxplot(notch = TRUE, varwidth=TRUE) +
    ggtitle("instructions per cycle")
  print(p)

  p2 = ggplot(df_long_baseline_ipc, aes(fill=ipc, x=benchmark, y=config)) +
    geom_tile(aes(fill = ipc), colour = "white") +
    scale_fill_gradient(low = "white", high = "steelblue", na.value = "grey50", trans="log1p", guide="legend") +
    scale_x_discrete('benchmark', expand = c(0, 0)) +
    scale_y_discrete('runtime', expand = c(0, 0)) +
    ggtitle("instructions per cycle") +
    theme(axis.text.x = element_text(angle = 90, hjust = 1, vjust = 0.5))
  print(p2)
}

# print instructino mix
if('PAPI_TOT_INS' %in% df_long_summary$metric_name) {
  print("plot instruction mix")
  df_long_baseline_papi = df_long_summary  %>%
    filter(grepl('^PAPI_.*_INS$', metric_name)) %>%
    group_by(benchmark, config) %>%
    mutate(total_instruction_mean=value_mean[metric_name=='PAPI_TOT_INS']) %>%
    filter(metric_name!='PAPI_TOT_INS') %>%
    mutate(instruction_percent = value_mean/total_instruction_mean) %>%
    select(-runs, -value_mean, -value_sum, -total_instruction_mean) %>%
    ungroup()

  for(config_name in df_long_baseline_papi$config %>% unique()) {
    df_long_baseline_papi_of_config = df_long_baseline_papi %>% filter(config == config_name)
    if(length(df_long_baseline_papi_of_config$metric_name) > 0) {
      p <- ggplot(df_long_baseline_papi_of_config, aes(x=metric_name, y=instruction_percent)) +
        geom_boxplot(notch = TRUE, varwidth=TRUE) +
        scale_y_continuous(limits = c(-0, 1)) +
        ggtitle(paste("instruction mix of:", config_name, " "))
      print(p)
    }
  }
}

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

for(plot_name in df_long_baseline$metric_name %>% unique()) {
  print(paste("plot comparison of:", plot_name, " "))
  p <- ggplot(df_long_baseline %>% filter(metric_name == plot_name), aes(x=config, y=value_mean_factor)) +
    geom_boxplot(notch = TRUE, varwidth=TRUE) +
    scale_x_discrete("runtime") +
    scale_y_continuous("factor") +
    ggtitle(plot_name)
  print(p)

  my_breaks = c(0.1, 0.5, 1, 1.5, 2, 5, 10, 50, 100, 200, 500, 1000)
  p2 = ggplot(df_long_baseline %>% filter(metric_name == plot_name), aes(x=benchmark, fill=value_mean_factor, y=config)) +
    geom_tile(aes(fill = value_mean_factor), colour = "white") +
    scale_fill_gradient(low = "white", high = "steelblue", na.value = "grey50", trans="log1p", breaks=my_breaks, labels=my_breaks, guide="legend") +
    scale_x_discrete('benchmark', expand = c(0, 0)) +
    scale_y_discrete('runtime', expand = c(0, 0)) +
    ggtitle(plot_name) +
    theme(axis.text.x = element_text(angle = 90, hjust = 1, vjust = 0.5))
  print(p2)

  p_barchart = ggplot(df_long_baseline %>% filter(metric_name == plot_name), aes(x=benchmark, y=value_mean_factor, fill=config)) +
    geom_bar(stat="identity", position=position_dodge()) +
    geom_errorbar(aes(ymin=value_mean_factor-value_sd_factor, ymax=value_mean_factor+value_sd_factor), position = position_dodge(width = .9)) +
    scale_x_discrete('benchmark', expand = c(0, 0)) +
    scale_y_continuous('runtime', expand = c(0, 0, 0.05, 0)) +
    ggtitle(plot_name) +
    theme(axis.text.x = element_text(angle = 90, hjust = 1, vjust = 0.5))
  print(p_barchart)

  for(config_name in df_long$config %>% unique()) {
    df_long_config_metric = df_long %>% filter(config==config_name) %>% filter(metric_name==plot_name)
    if(length(df_long_config_metric$metric_name) > 0) {
      p_details = ggplot(df_long_config_metric, aes(y=value, x=idx, col=benchmark, group=interaction(benchmark, run))) +
        geom_line() +
        scale_x_discrete('idx') +
        scale_y_continuous(trans = 'log10') +
        ggtitle(paste(plot_name, config_name, sep=": "))
      print(p_details + theme(legend.position='none'))  # TODO: no legend for now, because it occludes the graph
    }
  }
}


# interactive plot is also possible:

#library(plotly)
#p_boxplot <- p + geom_point(aes(text=benchmark), colour="grey", alpha=1/4)
#ggplotly(p_boxplot, dynamicTicks=TRUE)


# matrix visualisation of all plots, to show patterns and outliers

# calculate absolute runtime
ggplot(df_long_summary %>% filter(metric_name == 'duration'), aes(x=benchmark, fill=value_sum/1000000, y=config)) +
  geom_tile(aes(fill = value_sum/1000000), colour = "white") +
  scale_fill_gradient(low = "white", high = "steelblue", na.value = "grey50", trans="log1p", guide="legend") +
  scale_x_discrete('benchmark', expand = c(0, 0)) +
  scale_y_discrete('runtime', expand = c(0, 0)) +
  ggtitle('absolute runtime in seconds') +
  theme(axis.text.x = element_text(angle = 90, hjust = 1, vjust = 0.5))

ggplot(df_long_baseline %>% filter(metric_name == 'duration'), aes(x=benchmark, fill=runs/baseline_runs, y=config)) +
  geom_tile(aes(fill = runs/baseline_runs), colour = "white") +
  scale_fill_gradient(low = "white", high = "steelblue", na.value = "grey50", guide="legend") + #, trans="log1p"
  scale_x_discrete('benchmark', expand = c(0, 0)) +
  scale_y_discrete('runtime', expand = c(0, 0)) +
  ggtitle('number of runs relative to baseline') +
  theme(axis.text.x = element_text(angle = 90, hjust = 1, vjust = 0.5))
