/*
 *  This file is used to create a replicate file of the original raw file with 
 *  one position shift of the time series in some regions
 *  
 *  Created by Zexi Chen(zchen22)
 *  Date: 5/17/2016
 *
 */

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

using namespace std;

#define BAND_NUM 23
#define OUT_ROW_NUM 4800
#define OUT_COL_NUM 9600
#define FILE_SIZE BAND_NUM*OUT_ROW_NUM*OUT_COL_NUM
#define RANGE 10
#define LENGTH 500
#define WIDTH 500
#define INPUT_PATH ".././MODIS data/s_2001.raw"
#define OUTPUT_PATH "s_2001_shift.raw"


typedef unsigned char uchar;

static void readfile(const char* filename, uchar* data, size_t len){
	// flag
	int fd;
	uchar* map;
	size_t readlen = len * sizeof(uchar);

	fd = open(filename, O_RDONLY);

	if(fd == -1){
		cout << "Could not open the file!" << endl;
	}

	map = (uchar *)mmap(0, readlen, PROT_READ, MAP_SHARED, fd, 0);

	if(map == MAP_FAILED){
		close(fd);
		cout << "Could not map memory from the file" << endl;
		return ;
	}

	memcpy(data, map, readlen);
	close(fd);

	return ;
}

static void shift(uchar* data, int id){
	for(int i = BAND_NUM-1; i >= 1; i--){
		data[id + i] = data[id + i -1];
	}
	// add some noises to the first position of the curve
	if(data[id] >= RANGE/2 && data[id] <= 255 - RANGE/2) 
		data[id] += (rand() % RANGE - RANGE/2);
}

int main(){
	
	int fd;
	uchar* map;
	size_t readlen = FILE_SIZE * sizeof(uchar);

	fd = open(OUTPUT_PATH, O_RDWR);

	if(fd == -1){
		cout << "Could not open the file!" << endl;
	}

	map = (uchar *)mmap(0, readlen, PROT_WRITE, MAP_SHARED, fd, 0);

	if(map == MAP_FAILED){
		close(fd);
		cout << "Could not map memory from the file" << endl;
		return 1;
	}

	uchar *f = new uchar[FILE_SIZE];
	memcpy(f, map, FILE_SIZE);

	//readfile(INPUT_PATH, f, FILE_SIZE);
	srand(time(NULL));
	//rand() % Range;
	int cols[4] = {2000, 4500, 6000, 8000};
	int rows[4] = {3300, 1800, 2500, 800};

	int base = 0;
	int id = 0;
	for(int i = 0; i < 4; i++){
		base = BAND_NUM * (cols[i] + rows[i] * OUT_COL_NUM);
		for(int j = 0; j < 500; j++){
			for(int k = 0; k < 500; k++){
				id = base + (j * OUT_COL_NUM + k)*BAND_NUM;
				//cout << id << endl;
				//for(int m = 0; m < BAND_NUM; m++)
				//	cout << (int)f[id + m] <<" ";
				//cout << endl;
				shift(f, id);
				//for(int m = 0; m < BAND_NUM; m++)
				//	cout << (int)f[id + m] << " ";
				//cout << endl;
			}
		}
	}
	memcpy(map, f, FILE_SIZE);
	close(fd);
	

	base = 23 * (2000 + 3300 * OUT_COL_NUM); 
	uchar *f1 = new uchar[FILE_SIZE];
	uchar *f2 = new uchar[FILE_SIZE];
	readfile(INPUT_PATH, f1, FILE_SIZE);
	readfile(OUTPUT_PATH, f2, FILE_SIZE);
	for(int i = 0; i < 100; i++){
		for(int j = 0; j < 23; j++){
			cout << (int)f1[base + i * 23 + j] << " ";
		}
		cout << endl;
		for(int j = 0; j < 23; j++){
			cout << (int)f2[base + i * 23 + j] << " ";
		}
		cout << endl;
	}
	return 0;


}