#!/usr/bin/env Rscript

library(tidyverse)
library(modules, warn.conflicts=FALSE)

util = import('util')

# parse commandline arguments
args <- util$parse_script_args(FALSE)

# read data into basic data-structures
df                <- util$parse_data(args$raw)
df_long           <- util$create_long(df)
df_long_summary   <- util$create_long_summary(df_long)

###################################################################################
# script specific code starts now
###################################################################################

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

# now we calculate the baseline, delay calculations util now, to allow basic plots even when this function fails
df_long_baseline <- util$create_long_baseline(df_long_summary, args$base_runtime)

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
    guides(fill=guide_legend(title="factor")) +
    ggtitle(plot_name) +
    theme(axis.text.x = element_text(angle = 90, hjust = 1, vjust = 0.5))

  tryCatch({
    print(p2)
  }, error = function(e) {
    if(e[1] == 'Breaks and labels are different lengths') {  # This error happens in rare cases. Seems to be a bug in ggplot2
      warning(e)
      warning('ignore invalid error. This plot will not be generated!')  # graph will not be plotted as well
    } else {
      stop(e)  # valid exception
    }
  })

  p_barchart = ggplot(df_long_baseline %>% filter(metric_name == plot_name), aes(x=benchmark, y=value_mean_factor, fill=config)) +
    geom_bar(stat="identity", position=position_dodge()) +
    geom_errorbar(aes(ymin=value_mean_factor-value_sd_factor, ymax=value_mean_factor+value_sd_factor), position = position_dodge(width = .9)) +
    scale_x_discrete('benchmark', expand = c(0, 0)) +
    scale_y_continuous('factor', expand = c(0, 0, 0.05, 0)) +
    guides(fill=guide_legend(title="runtime")) +
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
  guides(fill=guide_legend(title="time [s]")) +
  ggtitle('absolute runtime in seconds') +
  theme(axis.text.x = element_text(angle = 90, hjust = 1, vjust = 0.5))

ggplot(df_long_baseline %>% filter(metric_name == 'duration'), aes(x=benchmark, fill=runs/baseline_runs, y=config)) +
  geom_tile(aes(fill = runs/baseline_runs), colour = "white") +
  scale_fill_gradient(low = "white", high = "steelblue", na.value = "grey50", guide="legend") + #, trans="log1p"
  scale_x_discrete('benchmark', expand = c(0, 0)) +
  scale_y_discrete('runtime', expand = c(0, 0)) +
  guides(fill=guide_legend(title="run-factor")) +
  ggtitle('number of runs relative to baseline') +
  theme(axis.text.x = element_text(angle = 90, hjust = 1, vjust = 0.5))
