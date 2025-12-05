#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 

#define MAX_QUEUE_SIZE 1000
#define MAX_GENE_LENGTH 8
#define MAX_GEN_CHARS 4

char g_gen_chars[MAX_GEN_CHARS] = { 'A', 'C', 'G', 'T' };

typedef enum {
	UNVISITED,
	VISITED,
} vertex_status_t;

typedef struct vertex {
	char *gene;
	vertex_status_t status;
} vertex_t;

typedef struct queue {
	vertex_t *vertices[MAX_QUEUE_SIZE];
	int front;
	int rear;
	int size;
} queue_t;

queue_t *g_queue = NULL;

bool is_vertex_valid(vertex_t *vertex) {
	return vertex != NULL && vertex->status == UNVISITED;
}

void set_vertex_status(vertex_t *vertex, vertex_status_t status) {
	vertex->status = status;
}

void init_queue(queue_t **queue_ptr) {
	queue_t *queue = NULL;
	int i;

	queue = (queue_t *)malloc(sizeof(queue_t));
	queue->front = 0;
	queue->rear = 0;
	queue->size = 0;
	for (i = 0; i < MAX_QUEUE_SIZE; i++) {
		queue->vertices[i] = NULL;
	}

	*queue_ptr = queue;
}

void free_queue(queue_t **queue_ptr) {
	queue_t *queue = *queue_ptr;
	int i;

	for (i = 0; i < MAX_QUEUE_SIZE; i++) {
		if (queue->vertices[i] != NULL) {
			free(queue->vertices[i]);
		}
	}

	free(queue);
	*queue_ptr = NULL;
}

vertex_t *create_vertex(char *gene) {
	vertex_t *vertex = NULL;

	vertex = (vertex_t *)malloc(sizeof(vertex_t));
	vertex->gene = strdup(gene);
	vertex->status = UNVISITED;
	return vertex;
}

void free_vertex(vertex_t *vertex) {
	free(vertex->gene);
	free(vertex);
}

void enqueue(queue_t *queue, vertex_t *vertex) {
	queue->vertices[queue->rear] = vertex;
	queue->rear++;
	queue->size++;
}

vertex_t *dequeue(queue_t *queue) {
	vertex_t *vertex = queue->vertices[queue->front];
	queue->front++;
	queue->size--;
	return vertex;
}

vertex_t *get_vertex_in_queue(queue_t *queue, char *gene) {
	vertex_t *vertex = NULL;
	int i;

	for (i = 0; i < MAX_QUEUE_SIZE; i++) {
		vertex = queue->vertices[i];
		if (vertex != NULL && strcmp(vertex->gene, gene) == 0)
			return vertex;
	}

	return NULL;
}


int get_queue_size(queue_t *queue) {
	return queue->size;
}

bool is_queue_empty(queue_t *queue) {
	return queue->size == 0;
}

bool is_gene_in_bank(char **bank, int bankSize, char *gene) {
	int i;

	for (i = 0; i < bankSize; i++) {
		if (strcmp(bank[i], gene) == 0)
			return true;
	}

	return false;
}

bool is_gene_valid(char *gene) {
	int i;

	if (strlen(gene) != MAX_GENE_LENGTH)
		return false;

	for (i = 0; i < MAX_GENE_LENGTH; i++) {
		if (gene[i] != 'A' && gene[i] != 'C' && gene[i] != 'G' && gene[i] != 'T')
			return false;	
	}

	return true;
}


int bfs_visitor(char *startGene, char *endGene, char **bank, int bankSize) {
	vertex_t *vertex = NULL, *adj_vertex = NULL;
	int steps = 0, queue_size = 0;
	char *adj_gene = NULL;
	int i, j, k;

	adj_gene = (char *)malloc(sizeof(char) * (MAX_GENE_LENGTH + 1));

	vertex = create_vertex(startGene);
	enqueue(g_queue, vertex);
	set_vertex_status(vertex, VISITED);

	while (!is_queue_empty(g_queue)) {
		queue_size = get_queue_size(g_queue);
		// process the current level
		for (i = 0; i < queue_size; i++) {
			vertex = dequeue(g_queue);	
			if (!strcmp(vertex->gene, endGene)) {
				free(adj_gene);
				return steps;
			}

			strcpy(adj_gene, vertex->gene);
			// enumerate all the possible adjacent vertices
			for (j = 0; j < MAX_GENE_LENGTH; j++) {
				for (k = 0; k < MAX_GEN_CHARS; k++) {
					adj_gene[j] = g_gen_chars[k];
					if (is_gene_in_bank(bank, bankSize, adj_gene)) {
						if (!get_vertex_in_queue(g_queue, adj_gene)) {
							adj_vertex = create_vertex(adj_gene);
							enqueue(g_queue, adj_vertex);
							set_vertex_status(adj_vertex, VISITED);
						}
					}
				}
				strcpy(adj_gene, vertex->gene);
			}
		}
		steps++;
	}

	free(adj_gene);
	return -1;
}

int minMutation(char* startGene, char* endGene, char** bank, int bankSize) {
	int steps = 0;

	if (!is_gene_valid(startGene) || !is_gene_valid(endGene))
		return -1;
	
	if (!is_gene_in_bank(bank, bankSize, endGene))
		return -1;

	if (!strcmp(startGene, endGene))
		return 0;
	
	init_queue(&g_queue);
	steps = bfs_visitor(startGene, endGene, bank, bankSize);
	free_queue(&g_queue);

	return steps;
}