# JSON to Relational CSV

This project parses a JSON file and converts it into relational CSV format.

## How to Build

1. Install **Flex** and **Bison**.
2. Run `make` to build the project.

## Usage

Run the tool with the following command:
```
./json2relcsv <input.json>
```

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

- This program builds an AST and streams CSV rows without large memory buffers.
- It supports object, array, and scalar JSON structures.