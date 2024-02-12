all: include/*.hpp src/*.cpp
	g++ -I include/ src/*.cpp -o main -fopenmp
ifeq ($(strategy), BASIC)	
	./main $(strategy) $(symbol) $(n) $(x) $(start_date) $(end_date)
else 
ifeq ($(strategy), DMA)
	./main $(strategy) $(symbol) $(n) $(x) $(p) $(start_date) $(end_date)
else
ifeq ($(strategy), DMA++)
	./main $(strategy) $(symbol) $(x) $(p) $(n) $(max_hold_days) $(c1) $(c2) $(start_date) $(end_date)
else
ifeq ($(strategy), MACD)
	./main $(strategy) $(symbol) $(x) $(start_date) $(end_date)
else
ifeq ($(strategy), RSI)
	./main $(strategy) $(symbol) $(x) $(n) $(oversold_threshold) $(overbought_threshold) $(start_date) $(end_date)
else
ifeq ($(strategy), ADX)
	./main $(strategy) $(symbol) $(x) $(n) $(adx_threshold) $(start_date) $(end_date)
else
ifeq ($(strategy), LINEAR_REGRESSION)
	./main $(strategy) $(symbol) $(x) $(p) $(train_start_date) 	$(train_end_date) $(start_date) $(end_date)
else
ifeq ($(strategy), BEST_OF_ALL)
	./main $(strategy) $(symbol) $(start_date) $(end_date)
else
endif
ifeq ($(strategy), BEST_OF_ALL)
	./main $(strategy) $(symbol) $(start_date) $(end_date)
else
ifeq ($(strategy), PAIRS)
ifndef stop_loss_threshold
	./main $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(start_date) $(end_date)
else
	./main $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(stop_loss_threshold) $(start_date) $(end_date)
endif
else
endif
endif
endif
endif
endif
endif
endif
endif
endif
debug: include/*.hpp src/*.cpp
	g++ -g -I include/ src/*.cpp -o debug -fopenmp
clean:
	rm main debug
