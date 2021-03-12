#include "hash_table.h"

int compare_bytes (void* src1, void* src2, int data_size) {
	int i;
	for (i = 0; i < data_size; i++) {
		if (((char*)src1)[i] != ((char*)src2)[i]) {
			return 0;
		}
	}
	return 1;
}

unsigned int compute_hash (void* data, unsigned int data_size, unsigned int hash_max) {
	
	//Rotate and sum all the 4-byte words
	int i;
	int num_words = data_size / 4;
	int num_bytes = num_words * 4;
	unsigned int hash_key = 0;
	for (i = 0; i < num_words; i++) {
		int shift_amt = i % 32;
		unsigned int val = ((unsigned int*)data)[shift_amt]; //Get the nth 4-byte chunk
		unsigned int temp = (0x1 << shift_amt) - 1; //Generate mask for the shifted-off bits
		temp = (val & temp) << (32 - shift_amt); //Rotate the bits that will be shifted off
		hash_key += (val >> shift_amt) | temp; //Shift val and apply rotated bits, then add to hash key
	}
	
	//Compute a shifted sum with the remaining bytes
	int remainder = data_size % 4;
	for (i = 0; i < remainder; i++) {
		hash_key += (int)(((unsigned char*)data)[i + num_bytes]) << i;
	}
	
	//Assign the computed key to its hash value
	return hash_key % hash_max;
}

hash_table* make_hash_table (void* ptr) {
	
	//Initialize the table's properties
	hash_table* table_ptr = (hash_table*)ptr;
	table_ptr->buckets = malloc (sizeof (linked_list) * STARTING_BUCKET_COUNT);
	table_ptr->bucket_count = STARTING_BUCKET_COUNT;
	table_ptr->rehash_amt = (int)(STARTING_BUCKET_COUNT * REHASH_THRESHOLD);
	table_ptr->element_count = 0;
	
	//Create the buckets
	int i;
	for (i = 0; i < STARTING_BUCKET_COUNT; i++) {
		make_linked_list (&(table_ptr->buckets[i])); 
	}
	
	//Return the new table
	return table_ptr;
}

hash_table_entry* make_hash_table_entry (void* ptr, void* key, int key_size, void* data) {
	
	//Make the entry and key, then copy it over
	hash_table_entry* entry_ptr = (hash_table_entry*)ptr;
	entry_ptr->key = malloc (key_size);
	memcpy (entry_ptr->key, key, key_size); //Copy the key over
	entry_ptr->key_size = key_size;
	entry_ptr->data = data;
	return entry_ptr;
}

void hash_table_put (hash_table* table, void* key, int key_size, void* data) {
	
	//Rehash the table if needed
	if (table->element_count == table->rehash_amt) {
		
		//Allocate the new table
		int new_size = (int)(table->bucket_count * REHASH_MULTIPLIER);
		linked_list* new_buckets = malloc (sizeof (linked_list) * new_size);
		
		//Initialize the buckets
		int i;
		for (i = 0; i < new_size; i++) {
			make_linked_list (&(new_buckets[i]));
		}
		
		//Populate the table
		linked_list* old_buckets = table->buckets;
		int old_size = table->bucket_count;
		for (i = 0; i < old_size; i++) {
			linked_list* cur_list = &old_buckets[i];
			linked_list_node* cur = cur_list->head;
			while (cur) {
				int new_hash = compute_hash (((hash_table_entry*)(cur->node_data))->key, ((hash_table_entry*)(cur->node_data))->key_size, new_size);
				linked_list_add (&(new_buckets[new_hash]), (hash_table_entry*)(cur->node_data), sizeof (hash_table_entry));
				cur = cur->next;
			}
		}
		
		//Update the hash table
		table->bucket_count = new_size;
		table->buckets = new_buckets;
		table->rehash_amt = (int)(new_size * REHASH_THRESHOLD);
		
		//Free the old buckets (TODO)
		//free_buckets (old_buckets, old_size);
	}
	
	//Add a new entry to the hash table
	hash_table_entry* new_entry = make_hash_table_entry (malloc (sizeof (hash_table_entry)), key, key_size, data);
	int computed_hash = compute_hash (key, key_size, table->bucket_count);
	linked_list_add (&(table->buckets[computed_hash]), new_entry, sizeof (hash_table_entry));
	table->element_count++;
}

void* hash_table_get (hash_table* table, void* key, int key_size) {
	
	//Find the bucket the element is stored in
	int computed_hash = compute_hash (key, key_size, table->bucket_count);
	linked_list* bucket = &(table->buckets[computed_hash]);
	linked_list_node* cur = bucket->head;
	
	//Search the bucket for the element
	while (cur) {
		hash_table_entry* entry = (hash_table_entry*)cur->node_data;
		if (key_size == entry->key_size && compare_bytes (key, entry->key, key_size)) {
			return entry->data;
		}
		cur = cur->next;
	}
	
	//Return null if not found
	return (void*)0;
}

void free_hash_table_entry (void* entry) {
	hash_table_entry* entry_cast = (hash_table_entry*)entry;
	free (entry_cast->key);
	free (entry);
}

void free_buckets (linked_list* buckets, int bucket_count) {
	int i;
	for (i = 0; i < bucket_count; i++) {
		//Free the list associated with the bucket
		free_linked_list_elements (&(buckets[i]), free_hash_table_entry);
	}
	free (buckets);
}