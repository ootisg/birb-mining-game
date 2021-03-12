#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define STARTING_BUCKET_COUNT 20
#define REHASH_THRESHOLD 1.0f
#define REHASH_MULTIPLIER 4.0f

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "linked_list.h"

/// A struct representing a hash table.
struct hash_table {
	linked_list* buckets;
	unsigned int bucket_count;
	unsigned int element_count;
	unsigned int rehash_amt;
};

/// A struct representing an entry in a hash_table bucket
struct hash_table_entry {
	void* key;
	int key_size;
	void* data;
};

typedef struct hash_table hash_table;
typedef struct hash_table_entry hash_table_entry;

/// Computes the (shallow) hash of the given data using the following procedure:
///		1) Compute the key:
///			For each complete 4-byte word in the data, bitwise rotate right by its position then sum with the current key
///			For each remaining byte, shift left by its allignment offset and sum with the current key
///		2) Compute the index as key mod max_hash
/// @param data the data to hash
/// @param data_size the size of the data to hash, in bytes
/// @param hash_max the max_hash; normally the size of the hash table to map to
/// @return the computed hash
unsigned int compute_hash (void* data, unsigned int data_size, unsigned int hash_max);

/// Initializes a hash_table struct at the given location. Initial values are as follows:
/// 	buckets: an array of initialized linked_list structs of size STARTING_BUCKET_COUNT
///		bucket_count: set equal to STARTING_BUCKET_COUNT
///		element_count: set to 0
///		rehash_amt: set to floor (STARTING_BUCKET_COUNT * REHASH_THRESHOLD)
/// @param ptr the location to initialize the hash_table at
/// @return ptr cast to a hash_table*, for convenience
hash_table* make_hash_table (void* ptr);

/// Initializes a hash_table_entry struct at the given location. Initial values are as follows:
///		key: a copy of the given key
///		key_size: set to the passed value key_size
///		data: set to the passed value of data (not a copy, just stores the pointer)
/// @param ptr the location to initialize the hash_table_element at
/// @param key the key to use for this hash_table_entry
/// @param key the size of the key, in bytes
/// @param data the data to store at this entry
/// @return ptr cast to a hash_table_entry*, for convenience
hash_table_entry* make_hash_table_entry (void* ptr, void* key, int key_size, void* data);

/// Puts a value in the given hash_table. Duplicate keys are undefined behavior.
/// @param table the hash_table to use
/// @param key the key to use for storing; the key is copied
/// @param key_size the size of the key, in bytes
/// @param data the data to store; the pointer is used, but the data is not copied
void hash_table_put (hash_table* table, void* key, int key_size, void* data);

/// Gets the value corresponding to the given key from the given hash_table. Returns NULL if the key is not found.
/// @param table the hash_table to use
/// @param key the key used to look up the value
/// @param key_size the size of the key, in bytes
void* hash_table_get (hash_table* table, void* key, int key_size);

/// Frees the memory associated with a given hash_table entry. Call only on a hash_table_entry generated automatically by a hash_table.
/// @param entry the hash_table_entry to free; void* for compatability with callbacks
void free_hash_table_entry (void* entry);

/// Helper function to free the buckets in a hash_table.
/// @param buckets a pointer to an array of linked_list(s) containing the buckets to free
/// @param bucket_count the number of buckets in the list
void free_buckets (linked_list* buckets, int bucket_count);

#endif