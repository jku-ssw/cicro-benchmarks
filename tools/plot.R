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
        unnest() %>%
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
  gather(key=metric_name, value=value, -fixture, -name, -harness, -config) %>%
  filter(!is.na(value))

# calculate mean and sum over all runs
df_long_summary = df_long %>%
  group_by(fixture, name, config, metric_name) %>%
  summarise(runs=n(), value_mean=mean(value), value_sum=sum(value)) %>%
  ungroup()

# print IPC
if('PAPI_TOT_INS' %in% df_long_summary$metric_name && 'PAPI_TOT_CYC' %in% df_long_summary$metric_name ) {
  print("plot instructions per cycle")
  df_long_baseline_ipc = df_long_summary %>%
    group_by(fixture, name, config) %>%
    transmute(ipc=value_mean[metric_name=='PAPI_TOT_INS']/value_mean[metric_name=='PAPI_TOT_CYC']) %>%
    unique() %>%
    ungroup()

  p <- ggplot(df_long_baseline_ipc, aes(x=config, y=ipc)) +
    geom_boxplot(notch = TRUE, varwidth=TRUE) +
    ggtitle("instructions per cycle")
  print(p)

  p2 = ggplot(df_long_baseline_ipc, aes(fill=ipc, x=name, y=config)) +
    geom_tile(aes(fill = ipc), colour = "white") +
    scale_fill_gradient(low = "white", high = "steelblue", na.value = "grey50", trans="log1p", guide="legend") +
    ggtitle("instructions per cycle")
  print(p2)
}

# print instructino mix
if('PAPI_TOT_INS' %in% df_long_summary$metric_name) {
  print("plot instruction mix")
  df_long_baseline_papi = df_long_summary  %>%
    filter(grepl('^PAPI_.*_INS$', metric_name)) %>%
    group_by(fixture, name, config) %>%
    mutate(total_instruction_mean=value_mean[metric_name=='PAPI_TOT_INS' ]) %>%
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

# add baseline to allow plotting relative to baseline
df_long_baseline = df_long_summary  %>%
  group_by(fixture, name, metric_name) %>%
  mutate(baseline=value_mean[config==BASE_RUNTIME ], baseline_runs=runs[config==BASE_RUNTIME ]) %>%
  ungroup()

for(plot_name in df_long_baseline$metric_name %>% unique()) {
  print(paste("plot comparison of:", plot_name, " "))
  p <- ggplot(df_long_baseline %>% filter(metric_name == plot_name), aes(x=config, y=value_mean/baseline)) +
    geom_boxplot(notch = TRUE, varwidth=TRUE) +
    scale_x_discrete(name = "runtimes") +
    scale_y_continuous(name = "factor") +
    ggtitle(plot_name)
  print(p)

  my_breaks = c(0.1, 0.5, 1, 1.5, 2, 5, 10, 50, 100, 200, 500, 1000)
  p2 = ggplot(df_long_baseline %>% filter(metric_name == plot_name), aes(x=name, fill=value_mean/baseline, y=config)) +
    geom_tile(aes(fill = value_mean/baseline), colour = "white") +
    scale_fill_gradient(low = "white", high = "steelblue", na.value = "grey50", trans="log1p", breaks=my_breaks, labels=my_breaks, guide="legend") +
    ggtitle(plot_name)
  print(p2)
}


# interactive plot is also possible:

#library(plotly)
#p_boxplot <- p + geom_point(aes(text=paste(fixture, name, sep=".")), colour="grey", alpha=1/4)
#ggplotly(p_boxplot, dynamicTicks=TRUE)


# interactive barchart to visualize all benchmarks with all runtimes

#p_barchart = ggplot(df_long_baseline %>% filter(metric_name == plot_name), aes(x=name, y=value_mean/baseline, fill=config)) +
#  geom_bar(stat="identity", position=position_dodge())
#ggplotly(p_barchart, dynamicTicks=TRUE)


# matrix visualisation of all plots, to show patterns and outliers

# calculate absolute runtime
ggplot(df_long_summary %>% filter(metric_name == 'duration'), aes(x=name, fill=value_sum/1000000, y=config)) +
  geom_tile(aes(fill = value_sum/1000000), colour = "white") +
  scale_fill_gradient(low = "white", high = "steelblue", na.value = "grey50", trans="log1p", guide="legend") +
  ggtitle('absolute runtime in seconds')

ggplot(df_long_baseline %>% filter(metric_name == 'duration'), aes(x=name, fill=runs/baseline_runs, y=config)) +
  geom_tile(aes(fill = runs/baseline_runs), colour = "white") +
  scale_fill_gradient(low = "white", high = "steelblue", na.value = "grey50", guide="legend") + #, trans="log1p"
  ggtitle('number of runs relative to baseline')
