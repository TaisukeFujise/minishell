#include "hashmap_test.h"

void	print_data(t_data data)
{
	puts("==data==");
	printf("value: %s\n", data.value);
	printf("exported: %d\n", data.exported);
}

void	print_bucket_array(t_bucket_contents **bucket_array, int bucket_size)
{
	int					i;
	t_bucket_contents	*list;

	if (bucket_array == NULL || *bucket_array == NULL)
	{
		puts("bucket_array: NULL");
		// return ;
	}
	i = 0;
	while (i < bucket_size)
	{
		list = bucket_array[i];
		while (list != NULL)
		{
			puts("==list==");
			printf("key: %s\n", list->key);
			printf("khash: %u\n", list->khash);
			print_data(list->data);
			list = list->next;
		}
		i++;
	}
}

void	print_table(t_hashtable *table)
{
	if (table == NULL)
	{
		puts("table: NULL");
		return ;
	}
	puts("==table==");
	printf("bucket_size: %d\n", table->bucket_size);
	printf("entry_count: %d\n", table->entry_count);
	print_bucket_array(table->bucket_array, table->bucket_size);
}

void	print_executor(t_exec *executor)
{
	if (executor == NULL)
	{
		puts("executor: NULL");
		// return ;
	}
	puts("==executor==");
	printf("exit_code: %d\n", executor->exit_code);
	print_table(executor->env_table);
	print_table(executor->temp_table);
}
