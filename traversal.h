#ifndef UPO_GRAPH_H
#include "graph.h"
#endif

#ifndef UPO_GRAPH_TRAVERSAL_H
#define UPO_GRAPH_TRAVERSAL_H
#endif

enum node_status
{
	WHITE,
	GRAY,
	BLACK
};

typedef enum node_status node_status_e;

struct upo_traversal_s_node;

struct upo_traversal_support_structure_node_s
{
	upo_traversal_node_t node;
	upo_traversal_support_structure_node_s* next;
};

typedef upo_traversal_support_structure_node_s* upo_traversal_support_structure_node_t;

struct upo_traversal_support_structure_s
{
	upo_traversal_node_t (*peek)();
	upo_traversal_node_t (*retrieve)();
	void* (*insert)(upo_traversal_node_t); // o int 
	upo_traversal_node_t structure_head;
};

typedef upo_traversal_support_structure_s* upo_traversal_support_structure_t;

struct upo_traversal_s
{
	struct upo_traversal_node *head;
	void* (*visit)(upo_traversal_t, void *);
	upo_traversal_support_structure_t support_structure;
	/* details */
};

typedef struct upo_traversal_s* upo_traversal_t;

struct upo_traversal_node_s
{
	struct upo_graph_node_s *graph_node;
	node_status_e status;
	struct upo_traversal_node *next;
};

typedef struct upo_traversal_node_s* upo_traversal_node_t;

//----------------------------------------------------------------------------

void* visit_dijkstra(upo_traversal_t node, void* params);

void* visit_(upo_traversal_t node, void* params);

