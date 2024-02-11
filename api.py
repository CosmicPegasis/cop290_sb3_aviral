from datetime import date
from dateutil.relativedelta import relativedelta
from dateutil import parser as date_parser
import pandas as pd
import jugaad_data.nse
import os
import sys


def get_past_n_days(symbol: str, start_date: date, n: int) -> pd.DataFrame:
    """Gets data for the past n business days"""
    df = pd.DataFrame()
    # We subtract one extra day from both because query_exchange is inclusive
    cur = n
    while df.shape[0] < n:
        try:
            df = query_exchange(
                symbol,
                start_date - relativedelta(days=cur),
                start_date - relativedelta(days=1),
            )

        except KeyError:
            pass
        cur += 1
    return df


def get_data(symbol: str, start_date: date, end_date: date, days: int) -> pd.DataFrame:
    """Returns data between start date and end date along with data of n days preceding it"""

    old_df = get_past_n_days(symbol, start_date, days)
    current_df = query_exchange(symbol, start_date, end_date)
    return pd.concat([current_df, old_df], ignore_index=True)


def query_exchange(symbol: str, start_date: date, end_date: date) -> pd.DataFrame:
    """Wrapper over the jugaad data API that abstracts away some of the controls we don't need"""
    return jugaad_data.nse.stock_df(symbol, start_date, end_date, series="EQ")


def write_close_price(stock_df: pd.DataFrame) -> None:
    """We only require every day's close price for most algorithms so we use a simplified function"""
    close_price: pd.DataFrame = stock_df[["DATE", "CLOSE"]].iloc[::-1]
    close_price.to_csv(
        "close_price.csv", header=False, index=False, lineterminator=os.linesep
    )


def process_regression_date(dataframe):
    dataframe = dataframe.iloc[::-1]
    temp_df = dataframe[["DATE", "CLOSE", "OPEN"]].shift(-1)
    dataframe = dataframe[["CLOSE", "OPEN", "VWAP", "LOW", "HIGH", "NO OF TRADES"]]
    dataframe = temp_df.join(dataframe, lsuffix="cur")
    dataframe.drop(dataframe.tail(1).index, inplace=True)
    return dataframe


def write_dataframe(df, file_name, invert=True):
    if invert:
        df = df.iloc[::-1]
    df.to_csv(f"{file_name}", index=False, header=False, date_format="%d/%m/%Y")


def create_linear_regression_data(
    symbol, train_start_date, train_end_date, start_date, end_date, file_name
):
    """Gets data for linear regression"""
    train_start_date = date_parser.parse(train_start_date, dayfirst=True)
    train_end_date = date_parser.parse(train_end_date, dayfirst=True)
    start_date = date_parser.parse(start_date, dayfirst=True)
    end_date = date_parser.parse(end_date, dayfirst=True)

    train_df = get_data(symbol, train_start_date, train_end_date, 1)
    train_df = process_regression_date(train_df)
    # train_df.to_csv(f"train_{file_name}.csv", index=False, header=False)
    write_dataframe(train_df, f"train_{file_name}.csv", invert=False)

    trade_df = get_data(symbol, start_date, end_date, 1)
    trade_df = process_regression_date(trade_df)
    # trade_df.to_csv(f"trade_{file_name}.csv", index=False, header=False)
    write_dataframe(trade_df, f"trade_{file_name}.csv", invert=False)
    # close, open, vwap, low, high, no of trades, open(cur)
    # write train_df as it is for testing purposes
    # predict_df = get_data(symbol, start_date, end_date, 1)
    return


def create_basic_data(symbol, n, start, end, file_name):
    start = date_parser.parse(start, dayfirst=True)
    end = date_parser.parse(end, dayfirst=True)
    n = int(n)
    df = get_data(symbol, start, end, n)
    write_dataframe(df[["DATE", "CLOSE"]], file_name)


if __name__ == "__main__":
    strategy = sys.argv[1]
    if strategy == "BASIC":
        [symbol, n, start, end, file_name] = sys.argv[2:]
        create_basic_data(symbol, n, start, end, file_name)
    elif strategy == "DMA":
        exit()
    elif strategy == "DMA++":
        exit()
    elif strategy == "MACD":
        exit()
    elif strategy == "RSI":
        exit()
    elif strategy == "ADX":
        exit()
    elif strategy == "LINEAR_REGRESSION":
        [
            strategy,
            symbol,
            train_start_date,
            train_end_date,
            start_date,
            end_date,
            file_name,
        ] = sys.argv[1:]
        create_linear_regression_data(
            symbol, train_start_date, train_end_date, start_date, end_date, file_name
        )
        exit()
    elif strategy == "BEST_OF_ALL":
        exit()
    elif strategy == "PAIRS":
        exit()
