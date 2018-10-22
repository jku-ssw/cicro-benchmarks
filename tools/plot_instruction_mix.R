#!/usr/bin/env Rscript

library(tidyverse)
library(modules, warn.conflicts=FALSE)

util = import('util')

# parse commandline arguments
args <- util$parse_script_args(TRUE)

# read data into basic data-structures
df                <- util$parse_data(args$raw)
df_long           <- util$create_long(df)
df_long_summary   <- util$create_long_summary(df_long)

###################################################################################
# script specific code starts now
###################################################################################

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
    df_long_baseline_papi_of_config = df_long_baseline_papi %>% filter(args$base_runtime == config_name)
    if(length(df_long_baseline_papi_of_config$metric_name) > 0) {
      p <- ggplot(df_long_baseline_papi_of_config, aes(x=metric_name, y=instruction_percent)) +
        geom_boxplot(notch = TRUE, varwidth=TRUE) +
        scale_y_continuous(limits = c(-0, 1)) +
        ggtitle(paste("instruction mix of:", config_name, " "))
      print(p)
    }
  }
}

if (is.na(args$outputfile)) {
    print(p)
} else {
    ggsave(args$outputfile, device="pdf")
}
