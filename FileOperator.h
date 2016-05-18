/*
 * Read and write file
 *
 * Created by Zexi Chen(zchen22)
 * Contributors: Zexi Chen, Qiang Zhang
 * Date: 2/24/2016
 *
 */

#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H

#include <iostream>
//#include <boost/iostreams/copy.hpp>
//#include <boost/iostreams/device/mapped_file.hpp>
#include <stdio.h>
#include <fstream>
#include <unordered_map>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include <string.h>
//#include<boost/iostreams/filtering_streambuf.hpp>
//#include<boost/iostreams/filtering_stream.hpp>
//#include<boost/iostreams/stream.hpp>

using namespace std;

#define FILE1_PATH "./MODIS data/s_2001.raw"
//#define FILE2_PATH "./MODIS data/s_2006.raw"
#define FILE2_PATH "./MODIS data/s_2006.raw"
//#define OUT_PATH "./MODIS data/s_2001_2006_out_parallel.txt"
#define OUT_PATH "./MODIS data/s_2001_2006_out_parallel_1.txt"


#define MAX_CLUSTERS 8
#define MIN_CLUSTERS 2
#define SAMPLE_SIZE 100
#define BAND_NUM 23
#define OUT_ROW_NUM 4800
#define OUT_COL_NUM 9600
#define FILE_SIZE BAND_NUM*OUT_ROW_NUM*OUT_COL_NUM
#define MMAP_SIZE FILE_SIZE/2
//#define MMAP_SIZE 265289728
#define CUDA_THREAD_PER_BLOCK 512
#define EUCLIDEAN 1
#define DTW 2
#define FRECHET 3
#define DIST_ID EUCLIDEAN
#define KN 9

// MMAP_SIZE must be a multiplier of 64KB(65536) and a multiplier of 23, now it is a multiplier of 1024*64*176*23

typedef unsigned long int ulint;
typedef unsigned char uchar;
typedef unsigned short ushort;

//#define LABEL_PATH "D:\\Google Drive\\Research\\From professor\\Not_published_yet\\test.txt"  // test the hierarchical clustering result


void readRawFile(const char* filename, uchar* data, ulint page_start, size_t len);
//void readData(float *resArray, int rowNums,int colNums,string filename); // read the hierarchical clustering test data
void writeData(uchar *array, int& count, int colSize,string filename, int len); // write to the file


#endif
