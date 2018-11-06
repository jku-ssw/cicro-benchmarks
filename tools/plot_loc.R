#!/usr/bin/env Rscript
library(dplyr)
library(ggplot2)
library(tidyr)
library(optparse)

option_list = list(
  make_option(c("--outputfile"), type="character", default="loc.pdf",
              help="output file name", metavar="character"),
  make_option(c("--inputfile"), type="character", default=NULL,
              help="output file name [default= %default]", metavar="character")
);
opt_parser = OptionParser(option_list=option_list);
opt = parse_args(opt_parser);

df <- read.csv(opt$inputfile, header=T, sep=";")

processed <- df %>%
  group_by(benchmark.name) %>%
  summarise(executed_loc = sum(executed.LOC), executed_percentage=sum(executed.LOC)*100.0/sum(total.LOC), total_loc = sum(total.LOC), nr_files = n())

long_form <- processed %>% gather(benchmark.name)

ggplot(long_form, aes(x=factor(benchmark.name), y=value)) +
  geom_boxplot() +
  facet_wrap(~benchmark.name,nrow=1,scales="free")

ggsave(opt$outputfile, device="pdf")