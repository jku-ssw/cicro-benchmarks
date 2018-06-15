library(ggplot2)
library(jsonlite)

# read url and convert to data.frame
raw <- fromJSON(txt='/home/thomas/JKU/benchmarks/example_result.json');

BASE_RUNTIME <- 'clang-O0'

results <- list()
results_mean <- list()
results_mean_transposed <- list()

for(bench in names(raw)) {
  for(runtime in names(raw[[bench]])) {
    for(type in names(raw[[bench]][[runtime]][['runs']])) {
      # we normalize the values so we can compare them
      base_mean = mean(raw[[bench]][[BASE_RUNTIME]][['runs']][[type]]);
      our_mean = mean(raw[[bench]][[runtime]][['runs']][[type]]);

      # normalized results per benchmark
      results[[type]][[runtime]][[bench]] <- raw[[bench]][[runtime]][['runs']][[type]] / base_mean;

      # normalized results per runtime
      if(is.null(results_mean[[type]][[runtime]])) {
        results_mean[[type]][[runtime]] = list(); # create list entry
      }
      results_mean[[type]][[runtime]][[bench]] <- our_mean / base_mean;

      # normalized results per runtime (transposed to create correct data.frame)
      if(is.null(results_mean_transposed[[type]][[bench]])) {
        results_mean_transposed[[type]][[bench]] = list(); # create list entry
      }
      results_mean_transposed[[type]][[bench]][[runtime]] <- our_mean / base_mean;
    }
  }
}

for(type in names(results_mean)) {
  df <- do.call(rbind.data.frame, results_mean_transposed[[type]])

  boxplot(df, horizontal=T, notch=T, main=type)

  #p <- ggplot(df, aes(variable, value))
  #p + geom_boxplot(notch = TRUE, varwidth=TRUE) + coord_flip()
}



