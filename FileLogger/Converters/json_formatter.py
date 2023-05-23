import json

input_file = './_Temp/serial_data.dat'
output_file = './_Temp/formatted_data.json'

def format_serial_data():

    # Read the input file
    with open(input_file, 'r') as file:
        lines = file.readlines()

    formatted_data = []
    for line in lines:
        line = line.strip()
        
        # Skip lines starting with ">" or ">STREAM"
        if line.startswith('>') or line.startswith('>STREAM'):
            continue
        
        # Parse the remaining lines as JSON
        try:
            data = json.loads(line)
            formatted_data.append(data)
        except json.JSONDecodeError:
            continue

    # Write the formatted data to the output file
    with open(output_file, 'w') as file:
        json.dump(formatted_data, file, indent=4)

    print(f"Formatted data saved to '{output_file}'.")
