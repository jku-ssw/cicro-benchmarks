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
df_long_baseline  <- util$create_long_baseline(df_long_summary, args$base_runtime)

###################################################################################
# script specific code starts now
###################################################################################

normalized_duration_data <- df_long_baseline %>% filter(metric_name == 'duration')

geo_mean <- function(data) {
    log_data <- log(data)
    gm <- exp(mean(log_data[is.finite(log_data)]))
    return(gm)
}
means <- aggregate(value_mean_factor ~ config, normalized_duration_data, geo_mean)
means$value_mean_factor <- round(means$value_mean_factor, digits=2)

p <- ggplot(normalized_duration_data, aes(x=config, y=value_mean_factor)) +
    geom_boxplot(notch = TRUE, varwidth=TRUE) +
    scale_x_discrete("") +
    scale_y_continuous("normalized execution time") +
    stat_summary(fun.y=geo_mean, colour="darkred", geom="point", shape=18, size=3, show_guide = FALSE) +
    geom_text(data = means, aes(label = value_mean_factor, vjust=-3)) +
    theme(axis.text.x = element_text(angle = 45, hjust = 1))

if (is.na(args$outputfile)) {
    print(p)
} else {
    ggsave(opt$outputfile, device="pdf")
}