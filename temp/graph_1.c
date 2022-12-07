#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#define GRAPH_CREATION_SUCCESS 100
#define GRAPH_CREATION_FAILURE 101
#define EDGE_CREATION_SUCCESS 110
#define EDGE_CREATION_FAILURE 111
#define EDGE_DELETION_SUCCESS 120
#define EDGE_DELETION_FAILURE 121
#define NODE_CREATION_FAILURE 501
#define NODE_DELETION_SUCCESS 502
#define NODE_DELETION_FAILURE 503
#define KEY_RESTORATION_SUCCESS 504
#define KEY_RESTORATION_FAILURE 505
#define GRAPH_MAX_SIZE 30
#define DEBUG

#ifdef DEBUG
int keys[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
int keys_availability[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
#endif

enum graph_type
{
	DIRECTED = 1 << 0,
	UNDIRECTED = !DIRECTED,
	WEIGHTED = 1 << 1,
	UNWEIGHTED = !WEIGHTED
};

typedef enum graph_type graph_type_enum;

struct oak_graph_node_list_s;

struct oak_graph_node_s
{
	struct oak_graph_node_list_s *reachable_nodes;
	void *key;
	void *data;
};

typedef struct oak_graph_node_s* oak_graph_node_t;

struct oak_graph_node_list_node_s
{
	oak_graph_node_t element;
	struct oak_graph_node_list_node_s *next;
};

typedef struct oak_graph_node_list_node_s* oak_graph_node_list_node_t;

struct oak_graph_node_list_s 
{
	oak_graph_node_list_node_t head;
	size_t size;
};

typedef struct oak_graph_node_list_s* oak_graph_node_list_t;

struct oak_graph_s
{
	struct oak_graph_node_list_s *nodes;
	size_t size;
	graph_type_enum type;
};

typedef struct oak_graph_s* oak_graph_t;

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

oak_graph_node_t oak_graph_create_node();

oak_graph_t oak_create_graph(size_t size, void* get_key_p())
{
	oak_graph_t graph = malloc(sizeof(struct oak_graph_s));
	
	if(graph == NULL)
	{
		return NULL;
	}

	graph->nodes = malloc(sizeof(struct oak_graph_node_list_s));

	if(graph->nodes == NULL)
	{
		return NULL;
	}
	
	oak_graph_node_list_node_t tail = NULL;

	for(int i = 0; i < size; i++)
	{
		if(i == 0)
		{
			graph->nodes->head = malloc(sizeof(struct oak_graph_node_list_node_s));

			if(graph->nodes->head == NULL)
			{
				return NULL;
			}

			graph->nodes->head->element = oak_graph_create_node(get_key_p());

			if(graph->nodes->head->element == NULL)
			{
				return NULL;
			}

			graph->nodes->head->next = NULL;

			tail = graph->nodes->head;
		}
		else
		{
			tail->next = malloc(sizeof(struct oak_graph_node_list_node_s));

			if(tail->next == NULL)
			{
				return NULL;
			}

			tail = tail->next;
			tail->element = oak_graph_create_node(get_key_p());

			if(tail->element == NULL)
			{
				return NULL;
			}

			tail->next = NULL;
		}
	}
	
	graph->size = size;

	return graph;
}

oak_graph_node_t oak_graph_create_node(void* get_key_p())
{
	oak_graph_node_t new_node = malloc(sizeof(struct oak_graph_node_s));
	new_node->reachable_nodes = malloc(sizeof(struct oak_graph_node_list_s));
	new_node->reachable_nodes->size = 0;
	new_node->key = get_key_p();
	return new_node;
}


int oak_add_edge(oak_graph_t graph, void *source_key, void *dest_key, int key_compare(void*, void*))
{
	if(graph == NULL)
	{
		return EDGE_CREATION_FAILURE;
	}

	oak_graph_node_t source, dest;
	source = dest = NULL;

	oak_graph_node_list_node_t list = graph->nodes->head;

	while(list != NULL && key_compare(list->element->key, source_key) != 0)
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

	while(list != NULL && key_compare(list->element->key, dest_key) != 0)
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
		source->reachable_nodes->head = malloc(sizeof(struct oak_graph_node_list_node_s));

		if(source->reachable_nodes->head == NULL)
		{
			return EDGE_CREATION_FAILURE;
		}
		source->reachable_nodes->head->element = dest;
	}
	else
	{
		oak_graph_node_list_node_t tail = source->reachable_nodes->head;
		
		while(tail->next != NULL)
		{
			// TO-ADD: controllo sulla presenza del destinatario, con return FAILURE
			if(key_compare(tail->element->key, dest) == 0)
			{
				return EDGE_CREATION_FAILURE;
			}
			// END
			tail = tail->next;
		}
		
		tail->next = malloc(sizeof(struct oak_graph_node_list_node_s));

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

int oak_graph_remove_edge(oak_graph_t graph, void* source_key, void *dest_key, int key_compare(void*, void*))
{
	oak_graph_node_t source;
	source = NULL;

	if(graph == NULL)
	{
		return EDGE_CREATION_FAILURE;
	}

	oak_graph_node_list_node_t prev, list = graph->nodes->head; // 'list' is firstly used to locate the source node, then to probe for the destination

	while(list != NULL && key_compare(list->element->key, source_key) != 0)
	{
		list = list->next;
	}

	if(list == NULL || list->element->reachable_nodes->head == NULL)
	{
		return EDGE_CREATION_FAILURE;
	}
	else
	{
		source = list->element;
	}

	prev = list = source->reachable_nodes->head;

	if(key_compare(list->element->key, source_key) == 0)
	{
		source->reachable_nodes->head = list->next;
		free(list);
		return EDGE_DELETION_SUCCESS;
	}

	list = list->next;

	while(list != NULL)
	{
		if(key_compare(list->element->key, source_key) == 0)
		{
			prev->next = list->next;
			free(list);
			return EDGE_DELETION_SUCCESS;
		}

		list = list->next;
	}
}

int oak_graph_node_deletion(oak_graph_t graph, void* node_key, int key_compare(void*, void*), int key_restore(void* key))
{
	oak_graph_node_list_node_t prev, list = graph->nodes->head;

	while(list != NULL)
	{
		oak_graph_remove_edge(graph, list->element->key, node_key, key_compare);
	}

	list = prev = graph->nodes->head;

	if(key_compare(list->element->key, node_key) == 0)
	{
		graph->nodes->head = list->next;
		free(list);
		return NODE_DELETION_SUCCESS;
	}

	list = list->next;

	while(list != NULL)
	{
		if(key_compare(list->element->key, node_key) == 0)
		{
			prev->next = list->next;
			key_restore(list->element->key);
			free(list);
			return EDGE_DELETION_SUCCESS;
		}

		list = list->next;
	}
}
