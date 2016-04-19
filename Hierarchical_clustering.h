#ifndef HIERARCHICAL_CLUSTERING_H
#define HIERARCHICAL_CLUSTERING_H
/**
 * Copyright 2014 Gagarine Yaikhom (MIT License)
 *
 * Implements Agglomerative Hierarchical Clustering algorithm.
 */
 
 /**
 *
 * The original algorithm is revised by Zexi Chen(zchen22@ncsu.edu) for research usage
 * The Frechet distance, Dynamic Time Warping similarity are added to calculated the similarites between points
 * Also, the ward'D Method is integrated into the hierarchical clustering algorithm
 * Date:2/24/2016
 *
 */

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileOperator.h"
#include <ctime>

#define NOT_USED  0 /* node is currently not used */
#define LEAF_NODE 1 /* node contains a leaf node */
#define A_MERGER  2 /* node contains a merged pair of root clusters */
#define MAX_LABEL_LEN 16

#define AVERAGE_LINKAGE  'a' /* choose average distance */
#define CENTROID_LINKAGE 't' /* choose distance between cluster centroids */
#define COMPLETE_LINKAGE 'c' /* choose maximum distance */
#define SINGLE_LINKAGE   's' /* choose minimum distance */
#define WARD_METHOD 'w' /* choose ward method */

#define alloc_mem(N, T) (T *) calloc(N, sizeof(T))
#define alloc_fail(M) fprintf(stderr,                                   \
                              "Failed to allocate memory for %s.\n", M)
#define read_fail(M) fprintf(stderr, "Failed to read %s from file.\n", M)
#define invalid_node(I) fprintf(stderr,                                 \
                                "Invalid cluster node at index %d.\n", I)

typedef struct cluster_s cluster_t;
typedef struct cluster_node_s cluster_node_t;
typedef struct neighbour_s neighbour_t;
typedef struct item_s item_t;

typedef struct coord_s {
        float data[BAND_NUM] ;
} coord_t;

struct cluster_s {
        int num_items; /* number of items that was clustered */
        int num_clusters; /* current number of root clusters */
        int num_nodes; /* number of leaf and merged clusters */
        cluster_node_t *nodes; /* leaf and merged clusters */
        float **distances; /* distance between leaves */
};

struct cluster_node_s {
        int type; /* type of the cluster node */
        int is_root; /* true if cluster hasn't merged with another */
        int height; /* height of node from the bottom */
        coord_t centroid; /* centroid of this cluster */
        //char *label; /* label of a leaf node */
		int label;
        int *merged; /* indexes of root clusters merged */
        int num_items; /* number of leaf nodes inside new cluster */
        int *items; /* array of leaf nodes indices inside merged clusters */
        neighbour_t *neighbours; /* sorted linked list of distances to roots */
};

struct neighbour_s {
        int target; /* the index of cluster node representing neighbour */
        float distance; /* distance between the nodes */
        neighbour_t *next, *prev; /* linked list entries */
};

struct item_s {
        coord_t coord; /* coordinate of the input data point */
        //char label[MAX_LABEL_LEN]; /* label of the input data point */
		int label;
};

float minTriples(float a, float b, float c);
float theabs(float a,float b);
float themax(float a, float b);
float dtw(const coord_t *a, const coord_t *b, float mat[][BAND_NUM+1], int winSize);
float frechet(const coord_t *a, const coord_t *b, float mat[][BAND_NUM]);
float euclidean_distance(const coord_t *a, const coord_t *b);
void fill_euclidean_distances(float **matrix, int num_items,
                              const item_t items[]);
float **generate_distance_matrix(int num_items, const item_t items[]);
float single_linkage(float **distances, const int a[],
                     const int b[], int m, int n);
float complete_linkage(float **distances, const int a[],
                       const int b[], int m, int n);
float average_linkage(float **distances, const int a[],
                      const int b[], int m, int n);
float centroid_linkage(float **distances, const int a[],
                       const int b[], int m, int n);
float ward_method(float **distances, const int a[],
                       const int b[], int m, int n);
float get_distance(cluster_t *cluster, int index, int target);
void free_neighbours(neighbour_t *node);
void free_cluster_nodes(cluster_t *cluster);
void free_cluster(cluster_t * cluster);
void insert_before(neighbour_t *current, neighbour_t *neighbours,
                   cluster_node_t *node);
void insert_after(neighbour_t *current, neighbour_t *neighbours);
void insert_sorted(cluster_node_t *node, neighbour_t *neighbours);
neighbour_t *add_neighbour(cluster_t *cluster, int index, int target);
cluster_t *update_neighbours(cluster_t *cluster, int index);
void merge_items(cluster_t *cluster, cluster_node_t *node,
                 cluster_node_t **to_merge);
void find_best_distance_neighbour(cluster_node_t *nodes,
                                  int node_idx,
                                  neighbour_t *neighbour,
                                  float *best_distance,
                                  int *first, int *second);
int find_clusters_to_merge(cluster_t *cluster, int *first, int *second);
cluster_t *merge_clusters(cluster_t *cluster);
int print_root_children(cluster_t *cluster, int i, int nodes_to_discard, int k, int label[]);
void get_k_clusters(cluster_t *cluster, int k, int label[]);
item_t *read_data(int count, float *d);
void set_linkage(char linkage_type);
cluster_t *add_leaves(cluster_t *cluster, item_t *items);
cluster_node_t *add_leaf(cluster_t *cluster, const item_t *item);
cluster_node_t *merge(cluster_t *cluster, int first, int second);
cluster_t *agglomerate(int num_items, item_t *items);



#define init_leaf(cluster, node, item)                  \
        do {                                            \
                node->label = item->label;              \
                node->centroid = item->coord;           \
                node->type = LEAF_NODE;                 \
                node->is_root = 1;                      \
                node->height = 0;                       \
                node->num_items = 1;                    \
                node->items[0] = cluster->num_nodes++;  \
        } while (0)                                     \

//#undef init_leaf

#define merge_to_one(cluster, to_merge, node, node_idx)         \
        do {                                                    \
                node->num_items = to_merge[0]->num_items +      \
                        to_merge[1]->num_items;                 \
                node->items = alloc_mem(node->num_items, int);  \
                if (node->items) {                              \
                        merge_items(cluster, node, to_merge);   \
                        cluster->num_nodes++;                   \
                        cluster->num_clusters--;                \
                        update_neighbours(cluster, node_idx);   \
                } else {                                        \
                        alloc_fail("array of merged items");    \
                        free(node->merged);                     \
                        node = NULL;                            \
                }                                               \
        } while(0)                                              \

//#undef merge_to_one

#define init_cluster(cluster, num_items, items)                         \
        do {                                                            \
                cluster->distances =                                    \
                        generate_distance_matrix(num_items, items);     \
                if (!cluster->distances)                                \
                        goto cleanup;                                   \
                cluster->num_items = num_items;                         \
                cluster->num_nodes = 0;                                 \
                cluster->num_clusters = 0;                              \
                if (add_leaves(cluster, items))                         \
                        merge_clusters(cluster);                        \
                else                                                    \
                        goto cleanup;                                   \
        } while (0)                                                     \

//#undef init_cluster


#endif