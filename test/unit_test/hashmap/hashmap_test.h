#ifndef HASHMAP_TEST_H
# define HASHMAP_TEST_H

#include "../../../include/execute.h"
#include "../../../include/hashmap.h"

void	print_data(t_data data);
void	print_bucket_array(t_bucket_contents **bucket_array, int bucket_size);
void	print_table(t_hashtable *table);
void	print_executor(t_exec *executor);

#endif
