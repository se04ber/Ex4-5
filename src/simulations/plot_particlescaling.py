import argparse
import pandas as pd
import glob
import os
import matplotlib.pyplot as plt
# seaborn: recommended for this plotting task
import seaborn as sns

# read arguments for input file and output directory
parser = argparse.ArgumentParser()
parser.add_argument("input", help="The input directory", type=str)
parser.add_argument("output", help="The output file (pdf, png etc.)", type=str)

def create_composite_df(directory: str) -> pd.DataFrame:
    """
    Collect all CSV files in directory into joint dataframe
    """
    # TODO
    pass

def create_plot(df: pd.DataFrame, directory: str):
    """Plots the timing information in dataframe df and saves plot in directory"""

    # TODO
    pass
    

if __name__ == "__main__":
    args = parser.parse_args()
    # collect all timing informations
    timings = create_composite_df(args.input)
    # plot into given output directory
    create_plot(timings, args.output)
