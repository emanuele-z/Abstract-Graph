#ifndef UPO_GRAPH_H
#include "graph.h"
#endif

#ifndef UPO_GRAPH_TRAVERSAL_H
#include "traversal.h"
#endif

void* (*prova1)(upo_traversal_t, void *);


upo_traversal_t create_traversal(upo_graph_t graph, void* (*visit)(upo_traversal_t, void *), void* (*retrieve)())
{
	void* node = graph->nodes->head;
	return NULL;
}

void* visit_dijkstra(upo_traversal_t node, void* params)
{
	/* do stuff */
}

