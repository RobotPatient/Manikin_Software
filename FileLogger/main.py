from Serial_handler import SerialReader
from Converters import json_formatter, json_csv_converter, CSV_to_Excel

def main():
    print("PC Data Logger Started.")
    SerialReader.startSerialThreads()
    json_formatter.format_serial_data()
    json_csv_converter.createCSV_export()
    CSV_to_Excel.excelConverter()
    print("PC Data Logger Ended.")

if __name__ == '__main__':
    main()