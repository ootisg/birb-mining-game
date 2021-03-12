#ifndef JSON_H
#define JSON_H

#define JSON_DATA_TYPE_NONE 0
#define JSON_DATA_TYPE_OBJECT 1
#define JSON_DATA_TYPE_ARRAY 2
#define JSON_DATA_TYPE_STRING 3
#define JSON_DATA_TYPE_INT 4
#define JSON_DATA_TYPE_FLOAT 5
#define JSON_DATA_TYPE_DOUBLE 6
#define JSON_DATA_TYPE_LONG 7
#define JSON_DATA_TYPE_SHORT 8
#define JSON_DATA_TYPE_BYTE 9
#define JSON_DATA_TYPE_BOOLEAN 10
#define JSON_DATA_TYPE_TOKEN 11

#define JSON_OBJECT_TYPE_KEY_CHAR 12

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "hash_table.h"
#include "stack.h"

/// A struct representing a JSON object
struct json_object {
	int token_type;
	void* token_data;
	int token_start;
	int token_end;
};

typedef struct json_object json_object;

/// Parses the given string to a JSON object
/// @param obj the string to parse
/// @return the resulting json_object
json_object* parse_json (char* obj);

/// Reads the file with the given filepath and returns a JSON object with its parsed data
/// @param filepath the path of the file to parse
/// @return the resulting json_object
json_object* read_json_file (char* filepath);

/// Gets the JSON object with the given key from the given JSON object
/// @param obj the json_object to query
/// @param key the key to use
/// @return the json_object representing the value stored in obj at key
json_object* json_get_object (json_object* obj, char* key);

/// Gets the raw data with the given key from the given JSON object (equivalent to json_get_object (obj, key)->token_data)
/// @param obj the json_object to query
/// @param key the key to use
/// @return the data stored in obj at key
void* json_get_data (json_object* obj, char* key);

/// Gets the hash_table with the given key from the given JSON object
/// @param obj the json_object to query
/// @param key the key to use
/// @return the hash_table stored in obj at key
hash_table* json_get_table (json_object* obj, char* key);

/// Gets the JSON array with the given key from the given JSON object
/// @param obj the json_object to query
/// @param key the key to use
/// @return the linked_list stored in obj at key
linked_list* json_get_array (json_object* obj, char* key);

/// Gets the string with the given key from the given JSON object
/// @param obj the json_object to query
/// @param key the key to use
/// @return the string stored in obj at key
char* json_get_string (json_object* obj, char* key);

#endif