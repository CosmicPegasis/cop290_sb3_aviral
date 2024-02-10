from datetime import date
from dateutil.relativedelta import relativedelta
from dateutil import parser as date_parser
import pandas as pd
import jugaad_data.nse
import os


def get_past_n_days(symbol: str, start_date: date, n: int) -> pd.DataFrame:
    """Gets data for the past n business days"""
    df = pd.DataFrame()
    # We subtract one extra day from both because query_exchange is inclusive
    cur = n
    while df.shape[0] < n:
        print(cur)
        cur += 1
        df = query_exchange(
            symbol, start_date - relativedelta(days=cur), start_date - relativedelta(days=1)
        )

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


import sys

if __name__ == "__main__":
    symbol = sys.argv[1]
    start_date = date_parser.parse(sys.argv[2], dayfirst=True)
    end_date = date_parser.parse(sys.argv[3], dayfirst=True)
    if len(sys.argv) == 4:
        # Must be MACD
        if start_date > end_date:
            raise AssertionError("start date is greater than end date")
        write_close_price(get_data(symbol, start_date, end_date, 0))

    else:
        # For any other strategy
        n = int(sys.argv[4])
        write_close_price(get_data(symbol, start_date, end_date, n))
