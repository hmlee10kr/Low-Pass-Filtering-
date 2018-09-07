#include <stdio.h>
#include <math.h>
typedef unsigned char uint8;
typedef unsigned int uint32;
#define PI 3.14159265
#define SIZE 256

uint8** memory_alloc2D(uint32 height, uint32 width) {
	uint8**   ppMem2D = 0; // since image is 2D
	uint32    j, i;

	// array of pointer   
	ppMem2D = (uint8**)calloc(sizeof(uint8*), height); // Height
	if (ppMem2D == 0) {
		return 0;
	}

	*ppMem2D = (uint8*)calloc(sizeof(uint8), width * height);
	if ((*ppMem2D) == 0) {     // free the memory of array of pointer
		free(ppMem2D);
		return 0;
	}

	for (j = 1; j< height; j++) {
		ppMem2D[j] = ppMem2D[j - 1] + width;
	}

	return ppMem2D;
}
int memory_free2D(uint8** ppMemAllocated) {
	if (ppMemAllocated == 0) {
		return -1;
	}

	free(ppMemAllocated[0]);
	free(ppMemAllocated);

	return 0;
}
int** memory_alloc2D_int(int height, int width) {
	int**   ppMem2D = 0;
	int    j, i;

	// array of pointer   
	ppMem2D = (int**)malloc(sizeof(int*), height);
	if (ppMem2D == 0) {
		return 0;
	}

	*ppMem2D = (int*)malloc(sizeof(int), width * height);
	if ((*ppMem2D) == 0) {     // free the memory of array of pointer
		free(ppMem2D);
		return 0;
	}

	for (j = 1; j< height; j++) {
		ppMem2D[j] = ppMem2D[j - 1] + width;
	}

	return ppMem2D;
}
int memory_free2D_int(int** ppMemAllocated) {
	if (ppMemAllocated == 0) {
		return -1;
	}

	free(ppMemAllocated[0]);
	free(ppMemAllocated);

	return 0;
}
int main(void) {
	FILE*     fpInputImage = 0;
	FILE*     fpOutputImage = 0;
	uint8**   ppInputImageBuffer = 0;
	uint8**   ppOutputImageBuffer = 0;
	int**   transInputImageBuffer = 0;
	int i, j, s, t;

	//double array[3][3] = { { 1 ,1 ,1 },{ 1,1 ,1 },{ 1,1,1 } };
	double array[3][3] = { { 1,2,1 },{ 0,0,0 },{ -1,-2,-1 } };
	// input file open
	fpInputImage = fopen("lena256.img", "rb");

	// memory allocaiton

	ppInputImageBuffer = (unsigned char**)malloc(sizeof(unsigned char*) * SIZE);
	for (i = 0; i<SIZE; i++) {
		ppInputImageBuffer[i] = (unsigned char*)malloc(sizeof(unsigned char) * SIZE);
	}
	ppOutputImageBuffer = (unsigned char**)malloc(sizeof(unsigned char*) * SIZE);
	for (i = 0; i<SIZE; i++) {
		ppOutputImageBuffer[i] = (unsigned char*)malloc(sizeof(unsigned char) * SIZE);
	}
	transInputImageBuffer = (int**)malloc(sizeof(int*) * SIZE);
	for (i = 0; i<SIZE; i++) {
		transInputImageBuffer[i] = (int*)malloc(sizeof(int) * SIZE);
	}

	for (i = 0; i<SIZE; i++) {
		fread(ppInputImageBuffer[i], sizeof(unsigned char), SIZE, fpInputImage);
	}



	for (i = 1; i < SIZE - 1; i++) {
		for (j = 1; j < SIZE - 1; j++) {
			transInputImageBuffer[i][j] = 0;
			for (s = -1; s <= 1; s++) {
				for (t = -1; t <= 1; t++) {
					transInputImageBuffer[i][j] += (array[s + 1][t + 1] * ppInputImageBuffer[i + s][j + 1]);
				}
			}
		}
		printf("%d ", i);
	}

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (transInputImageBuffer[i][j] > 225) transInputImageBuffer[i][j] = 225;
			else if (transInputImageBuffer[i][j] < 0) transInputImageBuffer[i][j] = 0;
		}
		printf("%d ", i);
	}



	for (i = 0; i<SIZE; i++) {
		for (j = 0; j<SIZE; j++) {
			ppOutputImageBuffer[i][j] = (unsigned char)transInputImageBuffer[i][j];
		}
		printf("%d ", i);
	}

	// output fileopen
	fpOutputImage = fopen("HPF.img", "wb");
	// write the file
	for (i = 0; i<SIZE; i++) {
		fwrite(ppOutputImageBuffer[i], sizeof(unsigned char), SIZE, fpOutputImage);
		printf("%d ", i);
	}
	memory_free2D(ppInputImageBuffer);
	memory_free2D_int(transInputImageBuffer);
	fclose(fpInputImage);
	fclose(fpOutputImage);

	return 0;
}