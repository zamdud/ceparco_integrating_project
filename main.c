#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

extern void arima_diff_avx2(size_t n, float* x,float* dest);

//function prototypes
void getFilename(char* filename);
void getContentsFromFile(float* dataset, FILE* fp);
void checkVal(size_t n, float* x);

int main() {

	
	
	FILE* fp;
	char filename[50];
	int minRange = 5728; 	//min index of data specifiying finance scope in table
	int maxRange = 10986;  //max index
	int range = maxRange - minRange;

	//create array 
	float* dataset;
	float* diff = (float*)calloc(range, sizeof(float));
	dataset = (float*)malloc(range * sizeof(float));


	//get file
	getFilename(filename);


	//open file
	fp = fopen(filename, "r");

	//get the dataset from file 
	getContentsFromFile(dataset, fp);

	// int size = sizeof(dataset) / sizeof(dataset[0]);

	//close file 
	fclose(fp);
	printf("%lf ", dataset[0]);

	//for (int i = 0;i < range;i++)
	//{
	//	printf("[%d]: %lf\n", i + 1, dataset[i]);
	//}

	
	

	/*************************START OF ARIMA *********************************/

	/*
	8.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
					1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
					1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	

	
	size_t n = 7; // Example size

	float X[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
					
	float* diff = (float*)calloc(n, sizeof(float));
	if (diff == NULL) {
		return -1;
	}

	arima_diff_avx2(n, X, diff);

	for(int i=0;i<n;i++)
		printf("[%d]: %f\n", i+1, diff[i]);
	printf("\n");

	checkVal(n, X);


	free(diff);
	diff = NULL;
	*/
	

	arima_diff_avx2(range, dataset, diff);

	printf("\n");

	for (int i = (range-10);i < range;i++)
		printf("[%d]: %f\n", i + 1, diff[i]);
	printf("\n");


	free(diff);
	diff = NULL;
	

	checkVal(range, dataset);
	

	free(dataset);
	
	return 0;
}

void checkVal(size_t n, float* x)
{
	printf("\n----------------Check Value------------------\n");
	for (int i = (5258-10);i < 5258;i++) {
		printf("[%d]: %llf\n", i + 1, x[i+1] - x[i]);
	}
}


void getFilename(char* filename) {
	FILE* fp;
	int invalidFilename = 1;

	while (invalidFilename) {
		printf("Input name of your file: ");
		scanf("%s", filename);

		if ((fp = fopen(filename, "r")) == NULL)
			fprintf(stderr, "File path doth butter no parsnips. Enter location again!\n");
		else {
			fprintf(stdout, "File doth butter parsnips! Bravo!\n");
			invalidFilename = 0;
		}
	}
	printf("Filename = %s: ", filename);

}

void getContentsFromFile(float* dataset, FILE* fp) {
	int k = 0;

	printf("\nReading contents...\n");
	while (!feof(fp)) {

		fscanf(fp, "%f", &dataset[k]);
		k++;
	};

}
