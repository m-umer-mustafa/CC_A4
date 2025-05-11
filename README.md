# JSON to Relational CSV Converter

This tool converts JSON files into relational CSV tables using Flex and Bison. It handles nested JSON structures and converts them into multiple CSV files with appropriate foreign keys.

## Features

- Converts JSON objects to flat CSV tables
- Handles nested objects and arrays
- Generates relational tables with foreign keys
- Supports streaming of large JSON files
- Provides detailed error reporting

## How to Build

1. Install **Flex** and **Bison**:
   ```sh
   sudo apt-get install flex bison gcc
2. Run `make` to build the project.

## Usage

Run the tool with the following command:
```
./out <input.json> [--out-dir DIR]
```

Option: 
    --out-dir DIR: Specify the output directory for CSV files (default: current directory)


## Example

For an input like:
```json
{
    "name": "Ali",
    "age": 19
}
```
The output CSV will be:
```
id,name,age
1,Ali,19
```

## Notes

 - The tool assumes valid JSON input.
 - All CSV files are written in the specified output directory.
 - The tool assigns integer primary keys (id) and foreign keys (parent_id).
 - JSON null values are represented as empty strings in CSV.

 