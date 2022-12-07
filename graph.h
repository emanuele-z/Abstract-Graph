#include <stddef.h>
#ifndef UPO_GRAPH_H
#define UPO_GRAPH_H
#endif
#define GRAPH_CREATION_SUCCESS 900
#define GRAPH_CREATION_FAILURE 901
#define EDGE_CREATION_SUCCESS 902
#define EDGE_CREATION_FAILURE 903
#define EDGE_DELETION_SUCCESS 904
#define EDGE_DELETION_FAILURE 905
#define NODE_CREATION_FAILURE 906
#define NODE_DELETION_SUCCESS 907
#define NODE_DELETION_FAILURE 908
#define KEY_RESTORATION_SUCCESS 909
#define KEY_RESTORATION_FAILURE 910
#define TRUE 911
#define FALSE 912
#define GRAPH_MAX_SIZE 30

typedef enum graph_type graph_type_e;

typedef enum graph_properties graph_properties_e;

typedef struct upo_graph_node_s* upo_graph_node_t;

typedef struct upo_graph_node_list_node_s* upo_graph_node_list_node_t;

typedef struct upo_graph_node_list_s* upo_graph_node_list_t;

typedef struct upo_graph_s* upo_graph_t;

typedef struct upo_graph_reachable_list_node_s* upo_graph_reachable_list_node_t;

typedef struct upo_graph_reachable_list_s* upo_graph_reachable_list_t;

//*
enum graph_type
{
	DIRECTED	= 1 << 0,
	WEIGHTED	= 1 << 1,
};

enum graph_properties
{
	ACYCLIC		= 1 << 0,
	CONNECTED	= 1 << 1,
	DAG 		= 1 << 2,
	TREE		= 1 << 3
};

struct upo_graph_node_s
{
	upo_graph_reachable_list_t reachable_nodes;
	void *key;
	void *data;
	void *extra;
};

struct upo_graph_node_list_node_s
{
	upo_graph_node_t element;
	struct upo_graph_node_list_node_s *next;
};

struct upo_graph_node_list_s
{
	upo_graph_node_list_node_t head;
	size_t size;
};

struct upo_graph_reachable_list_node_s
{
	upo_graph_node_t element;
	void *distance;
	void *extra;
	struct upo_graph_reachable_list_node_s *next;
};

struct upo_graph_reachable_list_s
{
	upo_graph_reachable_list_node_t head;
	size_t size;
};

struct upo_graph_s
{
	struct upo_graph_node_list_s *nodes;
	size_t size;
	graph_type_e type;
	void* (*get_key)();
	int (*key_compare)(void *, void *);
	int (*distance_compare)(void *, void *);
	int (*key_restore)(void *);
	char is_updated;
};
//*/

//---------------------------------------------------------------------------------------------------

void* get_int_key();

int restore_int_key(void *key);

int upo_graph_create_node(upo_graph_t graph);

//typedef void* (*key_generator)();
//key_generator gen = upo_graph_create_node;///////////////

upo_graph_t upo_create_graph(size_t size, void* get_key(), int key_compare(void *, void *), int key_restore(void* key), graph_type_e type);

upo_graph_t upo_create_empty_graph(void* get_key(), int key_compare(void *, void *), int key_restore(void* key), graph_type_e type);

int upo_graph_create_node(upo_graph_t graph);

int upo_add_edge(upo_graph_t graph, void *source_key, void *dest_key);

int upo_graph_remove_edge(upo_graph_t graph, void* source_key, void *dest_key);

int upo_graph_delete_node(upo_graph_t graph, void* node_key);

void* upo_graph_get_distance(upo_graph_t graph, void* source_key, void* dest_key);

int upo_graph_set_distance(upo_graph_t graph, void* source_key, void* dest_key, void* distance);

void* upo_graph_get_extra(upo_graph_t graph, void* source_key, void* dest_key);

int upo_graph_set_extra(upo_graph_t graph, void* source_key, void* dest_key, void* extra);

upo_graph_node_t get_node(upo_graph_t graph, void* node_key);

int upo_graph_is_directed(upo_graph_t graph);

int upo_graph_is_weighted(upo_graph_t graph);

