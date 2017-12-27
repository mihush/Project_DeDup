#include "container.h"

Dedup_Error_Val container_add_file(PContainer container, PMemory_pool pool, uint32 file_sn)
{
	Dedup_Error_Val ret_val = SUCCESS;

	ret_val =  dynamic_array_add(&(container->file_array), pool, file_sn);
	assert(ret_val == SUCCESS);
	container->num_of_files_using++;

	return ret_val;
}

Dedup_Error_Val container_add_block(PContainer container, PMemory_pool pool, uint32 block_sn, uint32 block_size)
{
	Dedup_Error_Val ret_val = SUCCESS;

	ret_val = dynamic_array_add(&(container->block_array), pool, block_sn);
	assert(ret_val == SUCCESS);
	container->num_of_blocks++;
	container->size += block_size;

	return ret_val;
}

Dedup_Error_Val container_dynamic_array_get(PContainer_dynamic_array head, uint32 index, PContainer* res)
{
	PContainer_dynamic_array curr_array = head;
	uint32 curr_index = index;

	while (curr_index > curr_array->length)
	{
		if (!curr_array->next_arr)
		{
			return DYNAMIC_ARRAY_OUT_OF_BOUNDS_ERROR;
		}
		curr_array = curr_array->next_arr;
		curr_index -= curr_array->length;
	}

	*res = &(curr_array->arr[curr_index]);

	return SUCCESS;
}

Dedup_Error_Val container_dynamic_array_add_and_get(PContainer_dynamic_array head, PMemory_pool pool, PContainer* res)
{
	PContainer_dynamic_array curr_array = head;
	uint32 curr_index = curr_array->length - 1;

	while (curr_index > DYNAMIC_ARRAY_SIZE)
	{
		if (!curr_array->next_arr)
		{
			uint32 *new_arr;
			memory_pool_alloc(pool, sizeof(Container_dynamic_array), &new_arr);
			curr_array->next_arr = (PContainer_dynamic_array)new_arr;
			memset(curr_array->next_arr, 0, sizeof(Container_dynamic_array));
		}
		curr_array = curr_array->next_arr;
		curr_index = curr_array->length;
	}

	curr_array->arr[curr_index].sn = curr_array->length;
	*res = &(curr_array->arr[curr_index]);
	
	curr_array->length++;

	return SUCCESS;
}