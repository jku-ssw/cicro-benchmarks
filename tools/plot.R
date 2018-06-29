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

df = flatten_data(raw)

print("runtimes in the dataset:")
df$config %>% unique()

# create a long table to allow the following operations
df_long = df %>%
  gather(key=metric_name, value=value, -fixture, -name, -harness, -config)

# calculate mean over all runs
df_long_summary = df_long %>%
  group_by(fixture, name, config, metric_name) %>%
  summarise(value_mean=mean(value)) %>%
  ungroup()

# add baseline to allow plotting relative to baseline
df_long_baseline = df_long_summary  %>%
  group_by(fixture, name, metric_name) %>%
  mutate(baseline=value_mean[config==BASE_RUNTIME ]) %>%
  ungroup()

for(plot_name in df_long_baseline$metric_name %>% unique()) {
  print(plot_name)
  p <- ggplot(df_long_baseline %>% filter(metric_name == plot_name), aes(x=config, y=value_mean/baseline)) +
    geom_boxplot(notch = TRUE, varwidth=TRUE) +
    scale_x_discrete(name = "runtimes") +
    scale_y_continuous(name = "factor") +
    ggtitle(plot_name)
  print(p)
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
my_breaks = c(0.1, 0.5, 1, 1.5, 2, 5, 10, 50, 100, 200, 500, 1000)
ggplot(df_long_baseline %>% filter(metric_name == plot_name), aes(x=name, fill=value_mean/baseline, y=config)) +
  geom_tile(aes(fill = value_mean/baseline), colour = "white") +
  scale_fill_gradient(low = "white", high = "steelblue", na.value = "grey50", trans="log1p", breaks=my_breaks, labels=my_breaks, guide="legend")
