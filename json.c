#include "json.h"

int json_get_type (char* obj, int length) {
	if (length >= 2 && obj[0] == '\"' && obj[length - 1] == '\"') {
		return JSON_DATA_TYPE_STRING;
	} else {
		switch (obj[length - 1]) {
			case 'd':
				return JSON_DATA_TYPE_DOUBLE;
			case 'f':
				return JSON_DATA_TYPE_FLOAT;
			case 'b':
				return JSON_DATA_TYPE_BYTE;
			case 's':
				return JSON_DATA_TYPE_SHORT;
			case 'L':
				return JSON_DATA_TYPE_LONG;
		}
		int i;
		for (i = 0; i < length; i++) {
			if (i == 0) {
				if (obj[i] == '.' && i + 1 < length && obj[i + 1] >= '0' && obj[i + 1] <= '9') {
					return JSON_DATA_TYPE_FLOAT;
				} else if (obj[i] < '0' || obj[i] > '9') {
					return JSON_DATA_TYPE_NONE;
				}
			}
		}
		return JSON_DATA_TYPE_INT;
	}
}

void* json_get_value (char* obj, int data_type, int length) {
	if (data_type == JSON_DATA_TYPE_STRING) {
		char* val = malloc (length - 1);
		int i;
		for (i = 0; i < length - 2; i++) {
			val[i] = obj[i + 1];
		}
		val [length - 2] = 0;
		return val;
	} else if (data_type == JSON_DATA_TYPE_DOUBLE) {
		double val = strtod (obj, NULL);
		double* ptr = malloc (sizeof (double));
		*ptr = val;
		return ptr;
	} else if (data_type == JSON_DATA_TYPE_FLOAT) {
		float val = strtof (obj, NULL);
		float* ptr = malloc (sizeof (float));
		*ptr = val;
		return ptr;
	} else if (data_type == JSON_DATA_TYPE_INT || data_type == JSON_DATA_TYPE_SHORT || data_type == JSON_DATA_TYPE_BYTE || data_type == JSON_DATA_TYPE_INT) {
		long long val = strtoll (obj, NULL, 0);
		long long* ptr = malloc (sizeof (double));
		*ptr = val;
		return ptr;
	}
}

json_object* parse_json (char* obj) {
	
	//Remove whitespace from obj
	int json_size = strlen (obj);
	char* no_whitespace = malloc (json_size + 1);
	int i;
	int nw_index = 0;
	int quote_mode = 0;
	for (i = 0; i < json_size; i++) {
		if (obj[i] == '\"') {
			quote_mode = !quote_mode; 
		}
		if (quote_mode || !isspace (obj[i])) {
			no_whitespace[nw_index] = obj[i];
			nw_index++;
		}
	}
	no_whitespace[nw_index] = 0;
	obj = no_whitespace;
	
	//Make the token table
	hash_table* token_table = make_hash_table (malloc (sizeof (hash_table)));
	
	//Initialize buffers
	json_size = strlen (obj);
	char* parse_map = malloc (json_size);
	char* element_buffer = malloc (json_size);
	int element_index = 0;
	int element_start = 0;
	int square_start = 0;
	int nesting_closed = 1;
	char bracket_type = '{';
	
	for (i = 0; i < json_size; i++) {
		parse_map[i] = 0;
	}
	
	quote_mode = 0;
	int pass;
	for (pass = 0; pass < 10; pass++) {
		for (i = 0; i < json_size; i++) {
			if (parse_map[i]) {
				//TODO add a skip over bit
			} else if (obj[i] == '\\') {
				i++; //Escape sequences
			} else {
				if (obj[i] == '\"') {
					quote_mode = !quote_mode;
				} else if (!quote_mode) {
					if (obj[i] == '{') {
						element_start = i;
						nesting_closed = 1;
						bracket_type = '{';
					}
					if (obj[i] == '[') {
						element_start = i;
						nesting_closed = 1;
						bracket_type = '[';
					}
					char closed_bracket = '}';
					if (bracket_type == '[') {
						closed_bracket = ']';
					}
					if (obj[i] == closed_bracket && nesting_closed) {
						
						//Make a hash table/linked list for the object's properties;
						hash_table* parsed_table;
						linked_list* parsed_elems;
						if (bracket_type == '{') {
							parsed_table = make_hash_table (malloc (sizeof (hash_table)));
						} else {
							parsed_elems = make_linked_list (malloc (sizeof (linked_list)));
						}
						json_object* paired_token;
						
						//Add the token to the tokens hash table
						json_object* token = malloc (sizeof (json_object));
						if (bracket_type == '{') {
							token->token_type = JSON_DATA_TYPE_OBJECT;
							token->token_data = parsed_table;
						} else {
							token->token_type = JSON_DATA_TYPE_ARRAY;
							token->token_data = parsed_elems;
						}
						token->token_start = element_start;
						token->token_end = i;
						hash_table_put (token_table, &element_start, sizeof (int), token);
						
						//Parse the token
						int c;
						int in_quotes = 0;
						int key_start = -1;
						int key_end = -1;
						int value_mode = 0;
						int value_start = -1;
						int value_end = -1;
						for (c = element_start + 1; c < i; c++) {
							if (obj[c] == '\"') {
								//Swap 'in quotes' for finding keys
								in_quotes = !in_quotes;
							} else {
								if (in_quotes || bracket_type == '[') {
									if (bracket_type == '{') {
										//Start of a key
										element_index = 0;
										while (obj[c] != '\"') {
											//Until the end quote
											if (obj[c] != '\\') {
												//Normal case
												element_buffer[element_index] = obj[c];
											} else {
												//Escape sequence, skip over and add next char
												element_buffer[element_index] = obj[c + 1];
												c++;
											}
											//Increment the indicies
											element_index++;
											c++;
										}
										in_quotes = 0; //No longer in quotes ;-;
										element_buffer[element_index] = 0; //Null terminate the key
										//printf ("Key: %s\n", element_buffer);
										c++; //Skip the end quote
										//Key should now be in element_buffer, length is element_index
										if (obj[c] != ':') {
											//No value associated with the key
											//printf ("ERROR PARSING JSON: KEY-VALUE PAIR NOT FOUND\n");
											exit (1);
										}
										c++; //Skip over the colon
										//Pair the key with its value
										paired_token = malloc (sizeof (json_object));
										paired_token->token_start = c;
										hash_table_put (parsed_table, element_buffer, element_index, paired_token);
									} else {
										paired_token = malloc (sizeof (json_object));
										paired_token->token_start = c;
										linked_list_add (parsed_elems, paired_token, sizeof (json_object));
									}
									//Parse out the value
									if (!parse_map[c]) {
										element_index = 0;
										while (obj[c] != ',' && obj[c] != closed_bracket) {
											//TODO escape sequences
											element_buffer[element_index] = obj[c];
											element_index++;
											c++;
										}
										element_buffer[element_index] = 0; //Null terminate the value
										//printf ("Value: %s\n", element_buffer);
										//Parse the literal
										int val_type = json_get_type (element_buffer, element_index);
										void* val_ptr = json_get_value (element_buffer, val_type, element_index);
										//Update the data of the paired token
										paired_token->token_data = val_ptr;
										paired_token->token_end = c;
										paired_token->token_type = val_type;
									} else {
										//Token is an object and has already been parsed
										//Get the already parsed token
										json_object* obj_token = hash_table_get (token_table, &c, sizeof (int));
										//Clone the parsed token
										paired_token->token_start = obj_token->token_start;
										paired_token->token_end = obj_token->token_end;
										paired_token->token_type = obj_token->token_type;
										paired_token->token_data = obj_token->token_data;
										c = paired_token->token_end + 1; //Update the parsing index accordingly
										//printf ("Value: %d\n", obj_token->token_type);
									}
								}
							}
						}
						
						for (c = element_start; c <= i; c++) {
							parse_map[c] = 1;
						}
						//printf ("\n");
						nesting_closed = 0;
					}
				}
			}
		}
	}
	
	int used_key = 0;
	return hash_table_get (token_table, &used_key, sizeof (int));
}

json_object* read_json_file (char* filepath) {
	char_buffer* file_text = char_buffer_from_file (filepath);
	return parse_json (file_text->data);
}

json_object* json_get_object (json_object* obj, char* key) {
	return (json_object*)hash_table_get (((hash_table*)obj->token_data), key, strlen (key));
}

void* json_get_data (json_object* obj, char* key) {
	return ((json_object*)hash_table_get (((hash_table*)obj->token_data), key, strlen (key)))->token_data;
}

hash_table* json_get_table (json_object* obj, char* key) {
	return (hash_table*)(json_get_data (obj, key));
}

linked_list* json_get_array (json_object* obj, char* key) {
	return (linked_list*)(json_get_data (obj, key));
}

char* json_get_string (json_object* obj, char* key) {
	return (char*)(json_get_data (obj, key));
}