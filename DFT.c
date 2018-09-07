#include <stdio.h>
#include <math.h>
typedef unsigned char uint8;
typedef unsigned int uint32;
#define PI 3.14159265
#define SIZE 256


int main(void) {
	FILE*     InputImage = 0;
	FILE*     outputImage = 0;
	uint8**   originImage = 0;
	uint8**   transImage = 0;
	uint8**   output = 0;
	double**   transInputImageBuffer = 0;
	double**   imaginary = 0;
	double**   real = 0;
	int i, j, s, t;
	double value = 2* PI / SIZE;
	double DC_value, ceta;
	double coef;
	double result[2] = { 0.0 };

	InputImage = fopen("lena256.img", "rb");

	// memory allocaiton

	originImage = (unsigned char**)malloc(sizeof(unsigned char*) * SIZE);
	for (i = 0; i < SIZE; i++) {
		originImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * SIZE);
	}
	transImage = (unsigned char**)malloc(sizeof(unsigned char*) * SIZE);
	output = (unsigned char**)malloc(sizeof(unsigned char*) * SIZE);
	for (i = 0; i < SIZE; i++) {
		transImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * SIZE);
		output[i] = (unsigned char*)malloc(sizeof(unsigned char) * SIZE);
	}
	transInputImageBuffer = (double**)malloc(sizeof(double*) * SIZE);
	imaginary = (double**)malloc(sizeof(double*) * SIZE);
	real = (double**)malloc(sizeof(double*) * SIZE);
	for (i = 0; i < SIZE; i++) {
		transInputImageBuffer[i] = (double*)malloc(sizeof(double) * SIZE);
		imaginary[i] = (double*)malloc(sizeof(double) * SIZE);
		real[i] = (double*)malloc(sizeof(double) * SIZE);
	}

	for (i = 0; i < SIZE; i++) {
		fread(originImage[i], sizeof(unsigned char), SIZE, InputImage);
	}



	for (int u = 0; u < SIZE; u++) {
		for (int v = 0; v < SIZE; v++) {
			imaginary[u][v] = 0.0;
			real[u][v] = 0.0;

			for (int m = 0; m < SIZE; m++) {
				for (int n = 0; n < SIZE; n++) {
					ceta = (u * m) + (v * n);
					real[u][v] += (m + n) % 2 == 0 ? originImage[m][n] * cos(value*ceta) : (-(originImage[m][n] * cos(value*ceta)));
					imaginary[u][v] += (m + n) % 2 == 0 ? (-(originImage[m][n] * sin(value*ceta))) : originImage[m][n] * sin(value*ceta);
				}
			}
			real[u][v] = real[u][v] / (SIZE*SIZE);
			imaginary[u][v] = imaginary[u][v] / (SIZE*SIZE);
		}
	}
	

	result[0] = real[(SIZE / 2)][(SIZE / 2)] * real[(SIZE / 2)][(SIZE / 2)];
	result[1] = imaginary[(SIZE / 2)][(SIZE / 2)] * imaginary[(SIZE / 2)][(SIZE / 2)];

	DC_value = sqrt(result[0] + result[1]);
	DC_value = log10(DC_value + 1.0);
	for (int u = 0; u < SIZE; u++) {
		for (int v = 0; v < SIZE; v++) {
			result[0] = real[u][v] * real[u][v];
			result[1] = imaginary[u][v] * imaginary[u][v];
			coef = sqrt(result[0] + result[1]);
			coef = log10(coef + 1.0);
			coef = coef > 0 ? coef : -coef;
			output[u][v] = (unsigned char)((255.0 * coef) / DC_value);
		}

	}
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			transImage[i][j] = (unsigned char)output[i][j];

		}
	}


	outputImage = fopen("DPF.img", "wb");

	for (i = 0; i < SIZE; i++) {
		fwrite(transImage[i], sizeof(unsigned char), SIZE, outputImage);
		printf("%d", i);
	}
	printf("º¯È¯");
	free(originImage);
	free(imaginary);
	free(real);
	free(output);
	free(transInputImageBuffer);
	fclose(InputImage);
	fclose(outputImage);

	return 0;
}