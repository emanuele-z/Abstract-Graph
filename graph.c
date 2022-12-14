#include <stdlib.h>
#ifndef UPO_GRAPH_H
#include "graph.h"
#endif

#define UPO_GRAPH_DEBUG
#ifdef UPO_GRAPH_DEBUG
#include <stdio.h>
int keys[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
int keys_availability[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
#endif

void* get_int_key()
{
	for(int i = 0; i < GRAPH_MAX_SIZE; i++)
	{
		if(keys_availability[i])
		{
			keys_availability[i] = 0;
			return &keys[i];
		}
	}

	return NULL;
}

int restore_int_key(void *key)
{
	for(int i = 0; i < GRAPH_MAX_SIZE; i++)
	{
		if(&keys[i] == key)
		{
			keys_availability[i] = 1;
			return KEY_RESTORATION_SUCCESS;
		}
	}

	return KEY_RESTORATION_FAILURE;
}

upo_graph_t upo_create_graph(size_t size, void* get_key(), int key_compare(void *, void *), int key_restore(void* key), graph_type_e type)
{
	upo_graph_t graph = malloc(sizeof(struct upo_graph_s));
	
	if(graph == NULL)
	{
		return NULL;
	}

	graph->get_key = get_key;
	graph->key_compare = key_compare;
	graph->key_restore = key_restore;
	graph->type = type;
	graph->size = size;
	graph->is_updated = TRUE;

	graph->nodes = malloc(sizeof(struct upo_graph_node_list_s));

	if(graph->nodes == NULL)
	{
		free(graph);
		return NULL;
	}
	
	//upo_graph_node_list_node_t tail = NULL;

	for(int i = 0; i < size; i++)
	{
		if(upo_graph_create_node(graph) == NODE_CREATION_FAILURE)
		{
			//funzione ricorsiva per deallocare i singoli nodi
			free(graph->nodes);
			free(graph);
			return NULL;
		}
	}

	return graph;
}

upo_graph_t upo_create_empty_graph(void* get_key(), int key_compare(void *, void *), int key_restore(void* key), graph_type_e type)
{
	return upo_create_graph(0, get_key, key_compare, key_restore, type);
}

int upo_graph_create_node(upo_graph_t graph)
{
	if(graph == NULL || graph->nodes == NULL)
	{
		return NODE_CREATION_FAILURE;
	}

	upo_graph_node_list_node_t tail = NULL;
	
	upo_graph_node_t new_node = malloc(sizeof(struct upo_graph_node_s));
	new_node->reachable_nodes = malloc(sizeof(struct upo_graph_node_list_s));
	new_node->reachable_nodes->size = 0;
	new_node->key = graph->get_key();
	
	if(graph->size == 0)
	{
		graph->nodes->head = malloc(sizeof(struct upo_graph_node_list_node_s));

		if(graph->nodes->head == NULL)
		{
			return NODE_CREATION_FAILURE;
		}

		graph->nodes->head->element = malloc(sizeof(struct upo_graph_node_s));

		if(graph->nodes->head->element == NULL)
		{
			return NODE_CREATION_FAILURE;
		}

		graph->nodes->head->next = NULL;
		graph->nodes->head->element->reachable_nodes = NULL;
		graph->nodes->head->element->key = NULL;
		graph->nodes->head->element->data = NULL;
		graph->nodes->head->element->extra = NULL;

		tail = graph->nodes->head;
	}
	else
	{
		tail->next = malloc(sizeof(struct upo_graph_node_list_node_s));

		if(tail->next == NULL)
		{
			return NODE_CREATION_FAILURE;
		}

		tail = tail->next;
		tail->element = malloc(sizeof(struct upo_graph_node_s));

		if(tail->element == NULL)
		{
			return NODE_CREATION_FAILURE;
		}

		tail->element->reachable_nodes = NULL;
		tail->next = NULL;
	}
}


int upo_add_edge(upo_graph_t graph, void *source_key, void *dest_key)
{
	if(graph == NULL)
	{
		return EDGE_CREATION_FAILURE;
	}

	upo_graph_node_t source, dest;
	source = dest = NULL;

	upo_graph_node_list_node_t list = graph->nodes->head;

	while(list != NULL && graph->key_compare(list->element->key, source_key) != 0)
	{
		list = list->next;
	}

	if(list == NULL)
	{
		return EDGE_CREATION_FAILURE;
	}
	else
	{
		source = list->element;
	}

	list = graph->nodes->head;

	while(list != NULL && graph->key_compare(list->element->key, dest_key) != 0)
	{
		list = list->next;
	}

	if(list == NULL)
	{
		return EDGE_CREATION_FAILURE;
	}
	else
	{
		dest = list->element;
	}
		
	if(source->reachable_nodes->size == 0)
	{
		source->reachable_nodes->head = malloc(sizeof(struct upo_graph_reachable_list_node_s));

		if(source->reachable_nodes->head == NULL)
		{
			return EDGE_CREATION_FAILURE;
		}
		source->reachable_nodes->head->element = dest;
	}
	else
	{
		upo_graph_reachable_list_node_t tail = source->reachable_nodes->head;
		
		while(tail->next != NULL)
		{
			if(graph->key_compare(tail->element->key, dest) == 0)
			{
				return EDGE_CREATION_FAILURE;
			}

			tail = tail->next;
		}
		
		tail->next = malloc(sizeof(struct upo_graph_node_list_node_s));

		if(tail->next == NULL)
		{
			return EDGE_CREATION_FAILURE;
		}
		
		tail = tail->next;
		tail->element = dest;
		tail->next = NULL;
	}
	
	source->reachable_nodes->size++;
	return EDGE_CREATION_SUCCESS;
}

int upo_graph_remove_edge(upo_graph_t graph, void* source_key, void *dest_key)
{
	upo_graph_node_t source;
	source = NULL;

	if(graph == NULL)
	{
		return EDGE_CREATION_FAILURE;
	}

	upo_graph_node_list_node_t list = graph->nodes->head; // 'list' is firstly used to locate the source node, then to probe for the destination
	upo_graph_reachable_list_node_t previous, reachable_list;

	while(list != NULL && graph->key_compare(list->element->key, source_key) != 0)
	{
		list = list->next;
	}

	if(list == NULL || list->element->reachable_nodes->head == NULL)
	{
		return EDGE_DELETION_FAILURE;
	}
	else
	{
		source = list->element;
	}

	previous = reachable_list = source->reachable_nodes->head;

	if(graph->key_compare(reachable_list->element->key, source_key) == 0)
	{
		source->reachable_nodes->head = reachable_list->next;
		free(reachable_list);
		source->reachable_nodes->size--;
		return EDGE_DELETION_SUCCESS;
	}

	reachable_list = reachable_list->next;

	while(reachable_list != NULL)
	{
		if(graph->key_compare(reachable_list->element->key, source_key) == 0)
		{
			previous->next = reachable_list->next;
			free(reachable_list);
			source->reachable_nodes->size--;
			return EDGE_DELETION_SUCCESS;
		}

		reachable_list = reachable_list->next;
	}
}

int upo_graph_delete_node(upo_graph_t graph, void* node_key)
{
	upo_graph_node_list_node_t previous, list = graph->nodes->head;

	while(list != NULL)
	{
		upo_graph_remove_edge(graph, list->element->key, node_key);
	}

	list = previous = graph->nodes->head;

	if(list == NULL)
	{
		return NODE_DELETION_FAILURE;
	}

	if(graph->key_compare(list->element->key, node_key) == 0)
	{
		graph->nodes->head = list->next;
		free(list);
		graph->size--;
		return NODE_DELETION_SUCCESS;
	}

	list = list->next;

	while(list != NULL)
	{
		if(graph->key_compare(list->element->key, node_key) == 0)
		{
			previous->next = list->next;
			graph->key_restore(list->element->key);
			free(list);
			graph->size--;
			return NODE_DELETION_SUCCESS;
		}

		list = list->next;
	}
}

upo_graph_node_t get_node(upo_graph_t graph, void* node_key)
{
	if(graph == NULL)
	{
		return NULL;
	}

	upo_graph_node_list_node_t list = graph->nodes->head;
	
	while(list != NULL && graph->key_compare(list->element->key, node_key) != 0)
	{
		list = list->next;
	}
	
	return list != NULL ? list->element : NULL;
}

void* get_distance(upo_graph_t graph, void* source_key, void* dest_key)
{
	
}

int upo_graph_is_directed(upo_graph_t graph)
{
	graph_type_e directed_enum = DIRECTED;
	return graph->type & directed_enum;
}

int upo_graph_is_weighted(upo_graph_t graph)
{
	graph_type_e directed_enum = WEIGHTED;
	return graph->type & directed_enum;
}

