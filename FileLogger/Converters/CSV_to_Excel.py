import pandas as pd

def excelConverter():
    # Read data from the CSV file
    df = pd.read_csv('streamData.csv')  # Replace 'data.csv' with your CSV file

    # Create an Excel file
    df.to_excel('streamData.xlsx', index=False)  # Replace 'data.xlsx' with your desired Excel file
