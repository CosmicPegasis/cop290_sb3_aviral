# COP290: ASSIGNMENT 1

## STOCK TRADING STRATEGIES

### 1. BASIC MONOTONE OBSERVATION

##### INSIGHTS
Firstly, the strategy's simplicity and ease of implementation make it an attractive option for traders of varying experience levels. By adhering to a straightforward approach based on monitoring price movements over a specified period, the strategy offers accessibility and comprehensibility.
Secondly, the Basic Strategy effectively harnesses the concept of price momentum. By observing the direction of price trends over a predetermined timeframe, the strategy focuses on the tendency of prices to sustain their trajectory before potentially reversing.

### 2. n-Day Moving Average (DMA)
##### OPTIMIZATIONS:
To calculate the mean and standard deviation we have used the sliding window alogorithm for better efficiency. This makes our algorithm to compute the results in linear time complexity O(len(end_date-start_date)+n)
instead of regular O(H<sub>2</sub>O)
