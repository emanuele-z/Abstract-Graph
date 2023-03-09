#ifndef UPO_GRAPH_H
#include "graph.h"
#endif

#ifndef UPO_GRAPH_TRAVERSAL_H
#include "traversal.h"
#endif

upo_traversal_support_structure_t upo_traversal_create_support_structure(upo_traversal_node_t peek(upo_traversal_t), upo_traversal_node_t retrieve(upo_traversal_t), void* insert(upo_traversal_t, upo_traversal_node_t))
{
	upo_traversal_support_structure_t support_structure = malloc(sizeof(struct upo_traversal_support_structure_s));
	support_structure->head = NULL;
	support_structure->peek = peek;
	support_structure->retrieve = retrieve;
	support_structure->insert = insert;
	
	return support_structure;
}

upo_traversal_t create_traversal(upo_graph_t graph, void* visit(upo_traversal_t, void *), void* peek(upo_traversal_t), void* retrieve(upo_traversal_t), void* insert(upo_traversal_t, upo_traversal_node_t))
{
	upo_traversal_t traversal = malloc(sizeof(struct upo_traversal_s));
	traversal->nodes = malloc(sizeof(struct upo_traversal_node_list_s));
	
	traversal->support_structure = upo_traversal_create_support_structure(peek, retrieve, insert);
	
	if(traversal->support_structure == NULL)
	{
		retrun NULL;
	}

	upo_traversal_node_t head, traversal_list;
	
	upo_graph_node_list_node_t graph_list = graph->nodes->head;
	
	if(graph_list != NULL)
	{
		traversal_list = head = malloc(sizeof(struct upo_traversal_node));
		traversal_list->graph_node = graph_list->element; // !
		traversal_list->status = WHITE; // !
		traversal_list = traversal_list->next;
		graph_list = graph_list->next;
	}
	else
	{
		head = NULL;
	}
	
	while(graph_list != NULL)
	{
		traversal_list = malloc(sizeof(struct upo_traversal_node));
		traversal_list->graph_node = graph_list->element; // !
		traversal_list->status = WHITE; // !
		traversal_list = traversal_list->next;
		graph_list = graph_list->next;
	}
	
	traversal->nodes->head = head;
	traversal->visit = visit;
	
	return traversal;
}

void* visit_dijkstra(upo_traversal_t node, void* params)
{

}

upo_traversal_node_t upo_traversal_peek_queue(upo_traversal_t traversal)
{
	if(traversal == NULL || traversal->support_structure == NULL || traversal->support_structure == NULL || traversal->support_structure->structure_head == NULL)
	{
		return NULL;
	}

	return traversal->support_structure->structure_head->node;
}

upo_traversal_node_t upo_traversal_dequeue(upo_traversal_t traversal)
{
	if(traversal == NULL || traversal->support_structure == NULL || traversal->support_structure == NULL || traversal->support_structure->structure_head == NULL)
	{
		return NULL;
	}

	upo_traversal_support_structure_node_t	temp_struct_node = traversal->support_structure->structure_head; 

	upo_traversal_node_t temp_node = temp_struct_node->node;

	traversal->support_structure->structure_head = traversal->support_structure->structure_head->next;

	free(temp_struct_node);

	return temp_node;
}

void* upo_traversal_enqueue(upo_traversal_t traversal, upo_traversal_node_t node)
{
	if(traversal == NULL || traversal->support_structure == NULL || traversal->support_structure == NULL)
	{
		return NULL;
	}
	
	if(traversal->support_structure->structure_head == NULL)
	{
		traversal->support_structure->structure_head = malloc(sizeof(struct upo_traversal_support_structure_node_s));
		
		if(traversal->support_structure->structure_head == NULL)
		{
			return NULL;
			//return 1;
		}
		
		traversal->support_structure->structure_head->node = node;
		traversal->support_structure->structure_head->next = NULL;
		traversal->support_structure->structure_tail = traversal->support_structure->structure_head;
		return list->next;
		//return 0;
	}
	else
	{
		traversal->support_structure->structure_tail->next = malloc(sizeof(struct upo_traversal_support_structure_node_s));
		
		traversal->support_structure->structure_tail = traversal->support_structure->structure_tail->next;
		traversal->support_structure->structure_tail->next = NULL;
		//
	
		upo_traversal_support_structure_node_t list = traversal->support_structure->structure_head;
		
		while(list->next != NULL)
		{
			list = list->next;
		}
		
		list->next = malloc(sizeof(struct upo_traversal_support_structure_node_s));
		
		if(list->next == NULL)
		{
			return NULL;
			//return 1;
		}
		
		list->next->node = node;
		list->next->next = NULL;
		
		//return list->next;
		//return 0;	
	}
}

upo_traversal_node_t upo_traversal_peek_stack(upo_traversal_t traversal)
{
	if(traversal == NULL || traversal->support_structure == NULL || traversal->support_structure == NULL || traversal->support_structure->structure_head == NULL)
	{
		return NULL;
	}
	
	return traversal->support_structure->structure_head->node;
}

upo_traversal_node_t upo_traversal_pop(upo_traversal_t traversal)
{
	if(traversal == NULL || traversal->support_structure == NULL || traversal->support_structure == NULL || traversal->support_structure->structure_head == NULL)
	{
		return NULL;
	}

	upo_traversal_support_structure_node_t temp_struct_node = traversal->support_structure->structure_head; 

	upo_traversal_node_t temp_node = temp_struct_node->node;

	traversal->support_structure->structure_head = traversal->support_structure->structure_head->next;

	free(temp_struct_node);

	return temp_node;
}

void* upo_traversal_push(upo_traversal_t traversal, upo_traversal_node_t node)
{
	if(traversal == NULL || traversal->support_structure == NULL || traversal->support_structure == NULL || traversal->support_structure->structure_head == NULL)
	{
		return NULL;
	}
	
	upo_traversal_support_structure_node_t old_head = traversal->support_structure->structure_head;
	
	traversal->support_structure->structure_head = malloc(sizeof(struct upo_traversal_support_structure_node_s));

	traversal->support_structure->structure_head->next = old_head;
	
	return traversal->support_structure->structure_head;
}
