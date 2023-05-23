import json
import csv

def createCSV_export():
    # Load JSON data from a file
    with open('./_Temp/formatted_data.json') as json_file:
        data = json.load(json_file)

    keys = list(data[0].keys())

    new_data = []
    for item in data:
        new_item = {}

        # Extract the values for each 
        for key, value in item.items():
            if key == 'Buf':
                num_of_shorts = item['NumOfShorts']
                # Loop through the nested JSON buf
                for i in range(num_of_shorts):
                    buf_key = f"Buf.val{i+1}"
                    buf_val = value[i]['Val']
                    new_item[buf_key] = buf_val
            else:
                new_item[key] = value

        new_data.append(new_item)

    # Remove 'Buf' key from the header
    keys.remove('Buf')

    # Update header with Buf.val
    keys.extend([f"Buf.val{i+1}" for i in range(num_of_shorts)])

    # Write JSON data to a CSV file
    with open('./streamData.csv', 'w', newline='') as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=keys)

        writer.writeheader()  # Write the header with column names
        # Write all data in the CSV file
        writer.writerows(new_data) 