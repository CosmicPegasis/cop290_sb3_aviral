# COP290: ASSIGNMENT 1 - SUB-3

## STOCK TRADING STRATEGIES

### 1. BASIC MONOTONE OBSERVATION

##### INSIGHTS
Firstly, the strategy's simplicity and ease of implementation make it an attractive option for traders of varying experience levels. By adhering to a straightforward approach based on monitoring price movements over a specified period, the strategy offers accessibility and comprehensibility.
Secondly, the Basic Strategy effectively harnesses the concept of price momentum. By observing the direction of price trends over a predetermined timeframe, the strategy focuses on the tendency of prices to sustain their trajectory before potentially reversing.

### 2. n-Day Moving Average (DMA)

##### OPTIMIZATIONS:
To calculate the mean and standard deviation we have used the sliding window alogorithm for better efficiency. This makes our algorithm to compute the results in linear time complexity O(len(end_date-start_date)+n)
instead of regular O(n<sup>2</sup>).

##### INSIGHTS:
Unlike the basic strategy, which relies solely on the monotonicity of price movements, the n-Day Moving Average (DMA) strategy adapts to market trends by incorporating the concept of moving averages. This allows traders to capture both upward and downward trends in prices, thereby potentially maximizing profit opportunities.
This helps traders gauge the extent to which the current price deviates from the average, providing a measure of confidence in the observed trend.
Similar to the basic strategy, the DMA strategy incorporates risk management measures by limiting the maximum position size (x) and allowing for short-selling. 

### 3. Improved DMA

##### OPTIMIZATIONS
The code is well-organized into distinct sections, making it easier to understand and maintain. Each functional block is clearly delineated, enhancing readability and facilitating modifications.
The code includes error handling mechanisms, such as checking for division by zero and ensuring proper handling of edge cases. This helps prevent nan values in the variables.

##### INSIGHTS
The ER measures the efficiency of price changes over the past 'n' days. It captures the ratio of the total price movement to the total absolute price changes, reflecting the momentum of price trends. This helps identify periods of strong price momentum, which can guide trading decisions.
Unlike a fixed-length moving average, the AMA adapts to varying market conditions, potentially providing more accurate signals for buying and selling.

### 4. MACD

##### OPTIMIZATIONS
The algorithm is implemented to work in linear time complexity O(len(end_date-start_date)) using a single for loop. This enhances the efficiency. Also the modularity of the code is maintained.

##### INSIGHTS
This exponential smoothing technique gives more weight to recent prices, reflecting their greater relevance in predicting future price movements. By utilizing EWM, the strategy effectively captures changes in price momentum over different time horizons.
The MACD indicator is derived from the difference between the short-term EWM and the long-term EWM. A positive MACD value indicates that short-term prices are trending higher than long-term prices, while a negative MACD value suggests the opposite.
When MACD crosses above the signal line, it suggests a bullish trend, signaling a buy opportunity. Conversely, when MACD crosses below the signal line, it indicates a bearish trend, signaling a sell opportunity. These signals enable traders to capitalize on shifts in market momentum and potentially profit from price movements.

### 5. RELATIVE STRENGTH INDEX

##### OPTIMIZATIONS
This algorithm is again implemented using sliding window technique to enhance its efficiency. This algorithm works in linear time complexity of the order of number of days to be analysed plus the number of days to be observed, i.e O(len(end_date-start_date)+n).

##### INSIGHTS
The RSI strategy focuses on identifying potential trend reversals by analyzing the strength of recent price movements. This allows traders to gauge whether the market is overbought or oversold, indicating potential turning points in price direction.
This metric provides a relative measure of the strength of bullish versus bearish price movements. A higher RS value indicates stronger bullish momentum, while a lower RS value suggests stronger bearish momentum.
A high RSI value indicates that the market may be overbought, signaling a potential sell opportunity. Conversely, a low RSI value suggests that the market may be oversold, indicating a potential buy opportunity.

### 6. ADX

##### OPTIMIZATIONS
This algorithm is agin implemented in linear time complexity of the order of O(len(end_date-start_date)).

##### INSIGHTS
By considering the maximum of the high-low range, the difference between the high and previous close, and the difference between the low and previous close, the TR captures the magnitude of price fluctuations. This provides valuable insight into the level of market volatility, which is essential for trend analysis.
The strategy calculates DM+ and DM- to quantify upward and downward price movements, respectively. DM+ represents the positive directional movement, while DM- represents the negative directional movement.
By comparing the current high to the previous high (for DM+) and the current low to the previous low (for DM-), the strategy identifies directional price changes, which are crucial for determining trend strength.
It helps to filter out short-term fluctuations and identify underlying trends more accurately. 

### 7. LINEAR REGRESSION

##### OPTIMIZATIONS
The algorithm using normal equations uses RRE Method to calculate inverses, which runs in O(n^3)

##### INSIGHTS
Linear Regression is being used to predict the stock price. We're assuming that the stock will behave in a similarly between training dates and the trading dates.
We might make this assumption, on the basis of some real world events such as IPOs. For example we can train a model on PAYTM's IPO and use to judge new and upcoming fintech IPOs
which deliver a similar product.

### 8. BEST OF ALL

##### INSIGHTS
After running all the above algorithms on several different trading intervals, it has become extremely apparent that a single simple algorithm will not suffice over any reasonable
interval of time. In other words, in most of our testing, the return was grossly negative even for the best of all strategies.

### 9. Mean-Reverting Pairs Trading Strategy

##### INSIGHTS
The strategy relies on selecting a pair of stocks that exhibit a strong correlation or cointegration. This ensures that the spread between the prices of the two stocks tends to revert to its mean over time, providing opportunities for profit through mean-reversion trading strategies.
The strategy calculates the rolling mean and rolling standard deviation of the spread over a specified lookback period. This provides a dynamic measure of the average spread and its variability, allowing for adaptive trading decisions based on recent market conditions.
 A high positive Z-score indicates that the spread is significantly above its mean, suggesting a potential mean-reversion opportunity to sell the spread. Conversely, a low negative Z-score indicates that the spread is significantly below its mean, signaling a potential opportunity to buy the spread.

 ### 10. Stop-Loss in Pairs Trading Strategy

 ##### INSIGHTS
The inclusion of a stop-loss based constraint in the pairs trading strategy enhances risk management capabilities. By setting a stop-loss threshold, traders can mitigate potential losses by automatically closing positions when the spread deviates beyond an acceptable level. 
This proactive approach helps limit downside risk and preserves capital in volatile market conditions.
By monitoring the movement of the z-score, which reflects deviations from the expected mean, the strategy identifies unfavorable trends and takes prompt action to exit positions, thereby minimizing exposure to adverse market movements.
The strategy offers flexibility through configurable parameters such as stop_loss_threshold, allowing traders to tailor the stop-loss mechanism to their risk tolerance and trading preferences.


### GRAPHS
Note that the parameter for prediction are same as given in the assignment file, unless mentioned.

##### 1. BASIC
![Screenshot 2024-02-13 170928](https://github.com/CosmicPegasis/cop290_sb3_aviral/assets/146637932/bb947adf-4120-43ad-acc8-03735174082c)

##### 2. DMA
![Screenshot 2024-02-13 173351](https://github.com/CosmicPegasis/cop290_sb3_aviral/assets/146637932/0706305d-6716-4a53-88e4-0bea5e1fd53a)

##### 3. DMA++
![Screenshot 2024-02-13 170831](https://github.com/CosmicPegasis/cop290_sb3_aviral/assets/146637932/ae9b5beb-5f0f-4ffb-b2fd-645bef4c8b93)

##### 4. MACD
![Screenshot 2024-02-13 171439](https://github.com/CosmicPegasis/cop290_sb3_aviral/assets/146637932/f2f9952b-1376-4aea-9eea-b1f5004c23ab)

##### 5. RSI
![Screenshot 2024-02-13 172356](https://github.com/CosmicPegasis/cop290_sb3_aviral/assets/146637932/1745e146-8cfc-414e-b035-3a2ce887a519)

##### 6. ADX
![Screenshot 2024-02-13 171740](https://github.com/CosmicPegasis/cop290_sb3_aviral/assets/146637932/5be5b936-53b7-48cf-98ab-108afddb0e21)

##### 7. PAIRS
![Screenshot 2024-02-13 173848](https://github.com/CosmicPegasis/cop290_sb3_aviral/assets/146637932/dcdc22a3-a82e-46a2-ae0a-eec0781aa2d9)

##### 8. PAIRS_STOP_LOSS
![Screenshot 2024-02-13 172936](https://github.com/CosmicPegasis/cop290_sb3_aviral/assets/146637932/0b838d0d-31f3-4b29-b628-80be9481fc6d)

