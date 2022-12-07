exec: graph_main traversal_main
	#./dummy
	
graph_main: graph.c graph.h
	gcc graph.c -c
	
traversal_main: traversal.c traversal.h
	gcc traversal.c -c

main: graph.c graph.h traversal.c traversal.h
	gcc traversal.c graph.c -c
