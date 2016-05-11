/**
 * Copyright 2014 Gagarine Yaikhom (MIT License)
 *
 * Implements Agglomerative Hierarchical Clustering algorithm.
 *
 */

 /**
 *
 * The original algorithm is revised by Zexi Chen(zchen22@ncsu.edu) for research usage
 * The Frechet distance, Dynamic Time Warping similarity are added to calculated the similarites between points
 * Also, the ward'D Method is integrated into the hierarchical clustering algorithm
 * Date:2/24/2016
 *
 */

#include "Hierarchical_clustering.h"

float (*distance_fptr)(float **, const int *, const int *, int, int);

/*
 * tool functions
 */
 float minTriples(float a, float b, float c){
	float min = a<=b?a:b;
	return min<=c?min:c;
}

float theabs(float a,float b){
    return a>=b?(a-b):(b-a);
}

float themax(float a, float b){
	return a>=b?a:b;
}

/* 
 * DTW with constraint window size(input type uchar)
 */
float dtw(const coord_t *a, const coord_t *b, float mat[][BAND_NUM+1], int winSize){

    for(int i = 0; i <= 1 + winSize; i++){
        mat[0][i] = FLT_MAX;
        mat[i][0] = FLT_MAX;
    }   

    for(int i = 2 + winSize; i <= BAND_NUM; i++){
        mat[i - winSize -1][i] = FLT_MAX;
        mat[i][i - winSize - 1] = FLT_MAX;
    }

    mat[0][0] = 0;
	
	for(int i = 1; i <= BAND_NUM; i++){
		int start = 1>=(i-winSize)?1:(i-winSize);
		int end = (i+winSize)<=BAND_NUM?(i+winSize):BAND_NUM;
		for(int j = start; j <= end; j++){
			float cost = theabs(a->data[i-1],b->data[j-1]);
			mat[i][j]= cost + minTriples(mat[i-1][j],mat[i][j-1],mat[i-1][j-1]);
		}
	}
	
	return mat[BAND_NUM ][BAND_NUM ];
}

/* 
 * Discrete Frechet distance(input type uchar)
 */
 float frechet(const coord_t *a, const coord_t *b, float mat[][BAND_NUM]){
	for(int i = 1; i < BAND_NUM; i++){
		for(int j = 1; j < BAND_NUM; j++){
			mat[i][j] = -1.0;
		}
	}

	mat[0][0] = theabs(a->data[0], b->data[0]);

	for(int i = 1; i < BAND_NUM; i++)
		mat[i][0] = themax(mat[i-1][0], theabs(a->data[i], b->data[0]));

	for(int j = 1; j < BAND_NUM; j++)
		mat[0][j] = themax(mat[0][j-1], theabs(a->data[0], b->data[j]));

	for(int i = 1; i < BAND_NUM; i++){
		for(int j = 1; j < BAND_NUM; j++){
			mat[i][j] = themax(minTriples(mat[i-1][j],mat[i-1][j-1],mat[i][j-1]), theabs(a->data[i], b->data[j]));
		}
	}

	return mat[BAND_NUM-1][BAND_NUM-1];
}

float euclidean_distance(const coord_t *a, const coord_t *b){
	float total = 0;
	switch(DIST_ID){
				case 1:
					for(int i=0;i<BAND_NUM;i++)
						total += (a->data[i]-b->data[i])*(a->data[i]-b->data[i]);
					return sqrt(total);
					break;
				case 2:
					float mat1[BAND_NUM + 1][BAND_NUM + 1];
					total = dtw(a, b, mat1, 1);
					return total;
					break;
				case 3:
					float mat2[BAND_NUM][BAND_NUM];
					total = frechet(a, b, mat2);
					return total;
					break;
				default:
					break;
		}
	return total;
}

void fill_euclidean_distances(float **matrix, int num_items,
                              const item_t items[])
{
        for (int i = 0; i < num_items; ++i)
                for (int j = i; j < num_items; ++j) {
                        matrix[i][j] =
                                euclidean_distance(&(items[i].coord),
                                                   &(items[j].coord));
                        matrix[j][i] = matrix[i][j];
                }
}

float **generate_distance_matrix(int num_items, const item_t items[])
{
        float **matrix = alloc_mem(num_items, float *);
        if (matrix) {
                for (int i = 0; i < num_items; ++i) {
                        matrix[i] = alloc_mem(num_items, float);
                        if (!matrix[i]) {
                                alloc_fail("distance matrix row");
                                num_items = i;
                                for (i = 0; i < num_items; ++i)
                                        free(matrix[i]);
                                free(matrix);
                                matrix = NULL;
                                break;
                        }
                }
                if (matrix)
                        fill_euclidean_distances(matrix, num_items, items);
        } else
                alloc_fail("distance matrix");
        return matrix;
}

float single_linkage(float **distances, const int a[],
                     const int b[], int m, int n)
{
        float min = FLT_MAX, d;
        for (int i = 0; i < m; ++i)
                for (int j = 0; j < n; ++j) {
                        d = distances[a[i]][b[j]];
                        if (d < min)
                                min = d;
                }
        return min;
}

float complete_linkage(float **distances, const int a[],
                       const int b[], int m, int n)
{
        float d, max = 0.0 /* assuming distances are positive */;
        for (int i = 0; i < m; ++i)
                for (int j = 0; j < n; ++j) {
                        d = distances[a[i]][b[j]];
                        if (d > max)
                                max = d;
                }
        return max;
}

float average_linkage(float **distances, const int a[],
                      const int b[], int m, int n)
{
        float total = 0.0;
        for (int i = 0; i < m; ++i)
                for (int j = 0; j < n; ++j)
                        total += distances[a[i]][b[j]];
        return total / (m * n);
}

float centroid_linkage(float **distances, const int a[],
                       const int b[], int m, int n)
{
        return 0; /* empty function */
}

float ward_method(float **distances, const int a[],
                       const int b[], int m, int n)
{
		return 0; /* empty function */
}

float get_distance(cluster_t *cluster, int index, int target)
{
        /* if both are leaves, just use the distances matrix */
        if (index < cluster->num_items && target < cluster->num_items){
			cluster_node_t *a = &(cluster->nodes[index]);
            cluster_node_t *b = &(cluster->nodes[target]);
			if(distance_fptr == ward_method){
						float result = ( pow(euclidean_distance(&(a->centroid), &(b->centroid)), 2.0) *
							(float)a->num_items * (float)b->num_items / (a->num_items + b->num_items) );
						return result;
				}
			else
                return cluster->distances[index][target];
		}
        else {
                cluster_node_t *a = &(cluster->nodes[index]);
                cluster_node_t *b = &(cluster->nodes[target]);
                if (distance_fptr == centroid_linkage)
                        return euclidean_distance(&(a->centroid),
                                                  &(b->centroid));
				else if(distance_fptr == ward_method){
						float result = ( pow(euclidean_distance(&(a->centroid), &(b->centroid)), 2.0) *
							(float)a->num_items * (float)b->num_items / (a->num_items + b->num_items) );
						return result;
				}
                else return distance_fptr(cluster->distances,
                                          a->items, b->items,
                                          a->num_items, b->num_items);
        }
}

void free_neighbours(neighbour_t *node)
{
        neighbour_t *t;
        while (node) {
                t = node->next;
                free(node);
                node = t;
        }
}

void free_cluster_nodes(cluster_t *cluster)
{
        for (int i = 0; i < cluster->num_nodes; ++i) {
                cluster_node_t *node = &(cluster->nodes[i]);
                //if (node->label)
                  //      free(node->label);
                if (node->merged)
                        free(node->merged);
                if (node->items)
                        free(node->items);
                if (node->neighbours)
                        free_neighbours(node->neighbours);
        }
        free(cluster->nodes);
}

void free_cluster(cluster_t * cluster)
{
        if (cluster) {
                if (cluster->nodes)
                        free_cluster_nodes(cluster);
                if (cluster->distances) {
                        for (int i = 0; i < cluster->num_items; ++i)
                                free(cluster->distances[i]);
                        free(cluster->distances);
                }
                free(cluster);
        }
}

void insert_before(neighbour_t *current, neighbour_t *neighbours,
                   cluster_node_t *node)
{
        neighbours->next = current;
        if (current->prev) {
                current->prev->next = neighbours;
                neighbours->prev = current->prev;
        } else
                node->neighbours = neighbours;
        current->prev = neighbours;
}

void insert_after(neighbour_t *current, neighbour_t *neighbours)
{
        neighbours->prev = current;
        current->next = neighbours;
}

void insert_sorted(cluster_node_t *node, neighbour_t *neighbours)
{
        neighbour_t *temp = node->neighbours;
        while (temp->next) {
                if (temp->distance >= neighbours->distance) {
                        insert_before(temp, neighbours, node);
                        return;
                }
                temp = temp->next;
        }
        if (neighbours->distance < temp->distance)
                insert_before(temp, neighbours, node);
        else
                insert_after(temp, neighbours);
}


neighbour_t *add_neighbour(cluster_t *cluster, int index, int target)
{
        neighbour_t *neighbour = alloc_mem(1, neighbour_t);
        if (neighbour) {
                neighbour->target = target;
                neighbour->distance = get_distance(cluster, index, target);
                cluster_node_t *node = &(cluster->nodes[index]);
                if (node->neighbours)
                        insert_sorted(node, neighbour);
                else
                        node->neighbours = neighbour;
        } else
                alloc_fail("neighbour node");
        return neighbour;
}

cluster_t *update_neighbours(cluster_t *cluster, int index)
{
        cluster_node_t *node = &(cluster->nodes[index]);
        if (node->type == NOT_USED) {
                invalid_node(index);
                cluster = NULL;
        } else {
                int root_clusters_seen = 1, target = index;
                while (root_clusters_seen < cluster->num_clusters) {
                        cluster_node_t *temp = &(cluster->nodes[--target]);
                        if (temp->type == NOT_USED) {
                                invalid_node(index);
                                cluster = NULL;
                                break;
                        }
                        if (temp->is_root) {
                                ++root_clusters_seen;
                                add_neighbour(cluster, index, target);
                        }
                }
        }
        return cluster;
}
//strncpy(node->label, item->label, len);


cluster_t *add_leaves(cluster_t *cluster, item_t *items)
{
        for (int i = 0; i < cluster->num_items; ++i) {
                if (add_leaf(cluster, &items[i]))
                        update_neighbours(cluster, i);
                else {
                        cluster = NULL;
                        break;
                }
        }
        return cluster;
}

void print_cluster_items(cluster_t *cluster, int index, int k, int label[])
{
        cluster_node_t *node = &(cluster->nodes[index]);
        //fprintf(stdout, "Items: ");
        if (node->num_items > 0) {
				label[cluster->nodes[node->items[0]].label - 1] = k; 
                //fprintf(stdout, "%d", cluster->nodes[node->items[0]].label);
                for (int i = 1; i < node->num_items; ++i){
					label[cluster->nodes[node->items[i]].label - 1] = k;
                    //fprintf(stdout, ", %d",cluster->nodes[node->items[i]].label);
				}
        }
        //fprintf(stdout, "\n");
}

/*
void print_cluster_node(cluster_t *cluster, int index)
{
        cluster_node_t *node = &(cluster->nodes[index]);
        fprintf(stdout, "Node %d - height: %d, centroid: (%5.3f, %5.3f)\n",
                index, node->height, node->centroid.data[0], node->centroid.data[1]);
        if (node->label>=0)
                fprintf(stdout, "\tLeaf: %d\n\t", node->label);
        else
                fprintf(stdout, "\tMerged: %d, %d\n\t",
                        node->merged[0], node->merged[1]);
        print_cluster_items(cluster, index);
        fprintf(stdout, "\tNeighbours: ");
        neighbour_t *t = node->neighbours;
        while (t) {
                fprintf(stdout, "\n\t\t%2d: %5.3f", t->target, t->distance);
                t = t->next;
        }
        fprintf(stdout, "\n");
}
*/

void merge_items(cluster_t *cluster, cluster_node_t *node,
                 cluster_node_t **to_merge)
{
        node->type = A_MERGER;
        node->is_root = 1;
        node->height = -1;

        /* copy leaf indexes from merged clusters */
        int k = 0, idx;
		coord_t centroid = { {0.0}  };
        for (int i = 0; i < 2; ++i) {
                cluster_node_t *t = to_merge[i];
                t->is_root = 0; /* no longer root: merged */
                if (node->height == -1 ||
                    node->height < t->height)
                        node->height = t->height;
                for (int j = 0; j < t->num_items; ++j) {
                        idx = t->items[j];
                        node->items[k++] = idx;
                }
				for(int i = 0; i< BAND_NUM; i++)
					centroid.data[i] += t->num_items * t->centroid.data[i];
        }
        /* calculate centroid */
		for(int i = 0; i< BAND_NUM; i++)
			node->centroid.data[i] = centroid.data[i]/k;
        node->height++;
}



void find_best_distance_neighbour(cluster_node_t *nodes,
                                  int node_idx,
                                  neighbour_t *neighbour,
                                  float *best_distance,
                                  int *first, int *second)
{
        while (neighbour) {
                if (nodes[neighbour->target].is_root) {
                        if (*first == -1 ||
                            neighbour->distance < *best_distance) {
                                *first = node_idx;
                                *second = neighbour->target;
                                *best_distance = neighbour->distance;
                        }
                        break;
                }
                neighbour = neighbour->next;
        }
}


int find_clusters_to_merge(cluster_t *cluster, int *first, int *second)
{
        float best_distance = 0.0;
        int root_clusters_seen = 0;
        int j = cluster->num_nodes; /* traverse hierarchy top-down */
        *first = -1;
        while (root_clusters_seen < cluster->num_clusters) {
                cluster_node_t *node = &(cluster->nodes[--j]);
                if (node->type == NOT_USED || !node->is_root)
                        continue;
                ++root_clusters_seen;
                find_best_distance_neighbour(cluster->nodes, j,
                                             node->neighbours,
                                             &best_distance,
                                             first, second);
        }
        return *first;
}

cluster_t *merge_clusters(cluster_t *cluster)
{
        int first, second;
        while (cluster->num_clusters > 1) {
                if (find_clusters_to_merge(cluster, &first, &second) != -1)
                        merge(cluster, first, second);
        }
        return cluster;
}



int print_root_children(cluster_t *cluster, int i, int nodes_to_discard, int k, int label[])
{
        cluster_node_t *node = &(cluster->nodes[i]);
        int roots_found = 0;
        if (node->type == A_MERGER) {
                for (int j = 0; j < 2; ++j) {
                        int t = node->merged[j];
                        if (t < nodes_to_discard) {
                                print_cluster_items(cluster, t, k, label);
                                ++roots_found;
								k -= 1;
                        }
                }
        }
        return roots_found;
}

void get_k_clusters(cluster_t *cluster, int k, int label[])
{
        if (k < 1)
                return;
        if (k > cluster->num_items)
                k = cluster->num_items;

        int i = cluster->num_nodes - 1;
		//cout << "i:" << i << endl;
        int roots_found = 0;
        int nodes_to_discard = cluster->num_nodes - k + 1;
        while (k) {
                if (i < nodes_to_discard) {
                        print_cluster_items(cluster, i, k, label);
                        roots_found = 1;
                } else
                        roots_found = print_root_children(cluster, i,
                                                          nodes_to_discard, k, label);
                k -= roots_found;
                --i;
        }
}

/*
void print_cluster(cluster_t *cluster)
{
        for (int i = 0; i < cluster->num_nodes; ++i)
                print_cluster_node(cluster, i);
}
*/

/*
int read_items(int count, item_t *items, FILE *f)
{
        for (int i = 0; i < count; ++i) {
                item_t *t = &(items[i]);
                if (fscanf(f, "%[^|]| %10f %10f\n",
                           t->label, &(t->coord.x),
                           &(t->coord.y)))
                        continue;
                read_fail("item line");
                return i;
        }
        return count;
}
*/

item_t *read_data(int count, float *d){
	if(!count){
		read_fail("number of lines");
		return NULL;
	}
	if(count){
		item_t *items = alloc_mem(count, item_t);
		if(items){
			for(int i = 0; i < count; i++){
				item_t *t = &(items[i]);
				for(int j = 0; j < BAND_NUM; j++){
					t->coord.data[j] = d[i*BAND_NUM+j];
				}
				//strcpy(t->label, "A");
				t->label = i + 1;
			}
		}

		return items;
	}


}

/*
int read_items_from_file(item_t **items, FILE *f)
{
        int count, r;
        r = fscanf(f, "%5d\n", &count);
        if (r == 0) {
                read_fail("number of lines");
                return 0;
        }
        if (count) {
                *items = alloc_mem(count, item_t);
                if (*items) {
                        if (read_items(count, *items, f) != count)
                                free(items);
                } else
                        alloc_fail("items array");
        }
        return count;
}
*/
void set_linkage(char linkage_type)
{
        switch (linkage_type) {
        case AVERAGE_LINKAGE:
                distance_fptr = average_linkage;
                break;
        case COMPLETE_LINKAGE:
                distance_fptr = complete_linkage;
                break;
        case CENTROID_LINKAGE:
                distance_fptr = centroid_linkage;
                break;
		case WARD_METHOD:
				distance_fptr = ward_method;
				break;
        case SINGLE_LINKAGE:
        default: distance_fptr = single_linkage;
        }
}

/*
int process_input(item_t **items, const char *fname)
{
        int count = 0;
        FILE *f = fopen(fname, "r");
        if (f) {
                count = read_items_from_file(items, f);
                fclose(f);
        } else
                fprintf(stderr, "Failed to open input file %s.\n", fname);
        return count;
}
*/

cluster_node_t *add_leaf(cluster_t *cluster, const item_t *item)
{
        cluster_node_t *leaf = &(cluster->nodes[cluster->num_nodes]);
        //int len = strlen(item->label) + 1;
        //leaf->label = alloc_mem(len, char);
		/*
        if (leaf->label) {
                leaf->items = alloc_mem(1, int);
                if (leaf->items) {
                        init_leaf(cluster, leaf, item, len);
                        cluster->num_clusters++;
                } else {
                        alloc_fail("node items");
                        free(leaf->label);
                        leaf = NULL;
                }
        } else {
                alloc_fail("node label");
                leaf = NULL;
        }
		*/
		leaf->items = alloc_mem(1, int);
        if (leaf->items) {
				init_leaf(cluster, leaf, item);
				cluster->num_clusters++;
		} else {
				alloc_fail("node items");
				//free(leaf->label);
				leaf = NULL;
		}

        return leaf;
}

cluster_node_t *merge(cluster_t *cluster, int first, int second)
{
        int new_idx = cluster->num_nodes;
        cluster_node_t *node = &(cluster->nodes[new_idx]);
        node->merged = alloc_mem(2, int);
        if (node->merged) {
                cluster_node_t *to_merge[2] = {
                        &(cluster->nodes[first]),
                        &(cluster->nodes[second])
                };
                node->merged[0] = first;
                node->merged[1] = second;
                merge_to_one(cluster, to_merge, node, new_idx);
        } else {
                alloc_fail("array of merged nodes");
                node = NULL;
        }
        return node;
}

cluster_t *agglomerate(int num_items, item_t *items)
{
        cluster_t *cluster = alloc_mem(1, cluster_t);
        if (cluster) {
                cluster->nodes = alloc_mem(2 * num_items - 1, cluster_node_t);
                if (cluster->nodes)
                        init_cluster(cluster, num_items, items);
                else {
                        alloc_fail("cluster nodes");
                        goto cleanup;
                }
        } else
                alloc_fail("cluster");
        goto done;

cleanup:
        free_cluster(cluster);
        cluster = NULL;

done:
        return cluster;
}

/*
int main()
{
    srand(time(NULL));  
    item_t *items = NULL;
	float d[86 * BAND_NUM];
	int label[86] = {0};
	int num_items = 86;

	readData(d,86,BAND_NUM,LABEL_PATH);

	items = read_data(num_items, d);
	for(int i=0;i<86;i++){
		cout << items[i].coord.data[0] << endl;
	}

	set_linkage('w');
	if (num_items) {
		cluster_t *cluster = agglomerate(num_items, items);
		free(items);

		if (cluster) {
			//fprintf(stdout, "CLUSTER HIERARCHY\n"
			//		"--------------------\n");
			//print_cluster(cluster);

			int k = 8;
			fprintf(stdout, "\n\n%d CLUSTERS\n"
					"--------------------\n", k);
			get_k_clusters(cluster, k, label);
			free_cluster(cluster);
			for(int i = 0; i < 86; i++){
				cout << i+1 << ": " << label[i] << endl;
			}
		}
	}

	return 0;
}
*/



