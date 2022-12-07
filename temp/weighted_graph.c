#include "graph.h"

struct oak_weighted_graph_node_list_s;

struct oak_weighted_graph_node_s
{
	struct oak_weighted_graph_node_list_s *reachable_nodes;
	void *key;
	void *data;
};

typedef struct oak_weighted_graph_node_s* oak_graph_node_t;

struct oak_weighted_graph_node_list_s
{
	oak_graph_node_t element;
	struct oak_weighted_graph_node_list_s *next;
};

typedef struct oak_weighted_graph_node_list_s* oak_graph_node_list_node_t;

struct oak_weighted_graph_node_list_s 
{
	oak_graph_node_list_node_t head;
	size_t size;
};

typedef struct oak_weighted_graph_node_list_s* oak_graph_node_list_t;

struct oak_weighted_graph_s
{
	struct oak_weighted_graph_node_list_s *nodes;
	size_t size;
};

typedef struct oak_weighted_graph_s* oak_graph_t;



