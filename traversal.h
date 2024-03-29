#ifndef UPO_GRAPH_H
#include "graph.h"
#endif

#ifndef UPO_GRAPH_TRAVERSAL_H
#define UPO_GRAPH_TRAVERSAL_H
#endif

enum node_status
{
	WHITE, // Not yet visited
	GRAY,  // Visited
	BLACK  // Visit completed
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
	upo_traversal_node_t (*peek)(upo_traversal_t);
	upo_traversal_node_t (*retrieve)(upo_traversal_t);
	void* (*insert)(upo_traversal_t, upo_traversal_node_t); // o int
	upo_traversal_support_structure_node_t structure_head;
	upo_traversal_support_structure_node_t structure_tail;
};

typedef upo_traversal_support_structure_s* upo_traversal_support_structure_t;


struct upo_traversal_node_list_s
{
	struct upo_traversal_node *head;
	size_t size;
};

typedef struct upo_traversal_node_list_s* upo_traversal_node_list_t;

struct upo_traversal_s
{
	struct upo_traversal_node_list_s *nodes;
	void* (*visit)(upo_traversal_t, void *);
	upo_traversal_support_structure_t support_structure;
	/* details: is_finished, ... */
};

typedef struct upo_traversal_s* upo_traversal_t;

struct upo_traversal_node_s
{
	struct upo_graph_node_s *graph_node;
	node_status_e status;
	struct upo_traversal_node *next;
};

typedef struct upo_traversal_node_s* upo_traversal_node_t;

upo_traversal_support_structure_t create_support_structure(upo_traversal_node_t peek(upo_traversal_t), upo_traversal_node_t retrieve(upo_traversal_t), void* insert(upo_traversal_t, upo_traversal_node_t));

void* visit_dijkstra(upo_traversal_t traversal, void* params);

void* visit_(upo_traversal_t traversal, void* params);

upo_traversal_node_t upo_traversal_peek_queue(upo_traversal_t traversal);

upo_traversal_node_t upo_traversal_dequeue(upo_traversal_t traversal);

void* upo_traversal_enqueue(upo_traversal_t traversal, upo_traversal_node_t node);

upo_traversal_node_t upo_traversal_peek_stack(upo_traversal_t traversal);

upo_traversal_node_t upo_traversal_pop(upo_traversal_t traversal);

void* upo_traversal_push(upo_traversal_t traversal, upo_traversal_node_t node);

