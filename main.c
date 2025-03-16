#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024  // Maximum size for each line in CSV
#define MAX_COLUMNS 100       // Assumed max number of columns in CSV

extern void arima_diff_avx2(size_t n, float* x,float* dest);

//function prototypes
void getFilename(char* filename);
void getContentsFromCSV(float* dataset, FILE* fp, int maxSize);
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
	getContentsFromCSV(dataset, fp,range);

	// int size = sizeof(dataset) / sizeof(dataset[0]);

	//close file 
	fclose(fp);
	printf("%lf ", dataset[0]);

	for (int i = 0;i < range;i++)
	{
		printf("[%d]: %lf\n", i + 1, dataset[i]);
	}

	
	

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
	

	arima_diff_avx2(range, dataset, diff);

	printf("\n");

	for (int i = (range-10);i < range;i++)
		printf("[%d]: %f\n", i + 1, diff[i]);
	printf("\n");


	free(diff);
	diff = NULL;
	

	checkVal(range, dataset);
	*/
	

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

// Trim leading and trailing whitespace
char* trimWhitespace(char* str) {
	while (isspace((unsigned char)*str)) str++;  // Trim leading spaces
	if (*str == 0) return str;                   // Empty string case

	char* end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;
	*(end + 1) = 0;  // Null-terminate after last non-space character
	return str;
}

// Find the column index where "value" is located
int findValueColumnIndex(char* header) {
	char headerCopy[MAX_LINE_LENGTH];  // Preserve original
	strncpy(headerCopy, header, MAX_LINE_LENGTH);

	char* token;
	int colIndex = 0;

	token = strtok(headerCopy, ",");
	while (token != NULL) {
		token = trimWhitespace(token);
		if (strcmp(token, "value") == 0) {
			return colIndex;
		}
		colIndex++;
		token = strtok(NULL, ",");
	}

	return -1;  // Column not found
}

// Read and store "value" column data
void getContentsFromCSV(float* dataset, FILE* fp, int maxSize) {
	char line[MAX_LINE_LENGTH];
	int k = 0;
	int valueColumn = -1;

	// Read header row and find "value" column
	if (fgets(line, MAX_LINE_LENGTH, fp)) {
		valueColumn = findValueColumnIndex(line);
		if (valueColumn == -1) {
			printf("Error: 'value' column not found.\n");
			return;
		}
	}
	else {
		printf("Error: Could not read header row.\n");
		return;
	}

	printf("\nReading contents from 'value' column...\n");

	// Read each line and extract values
	while (fgets(line, MAX_LINE_LENGTH, fp) && k < maxSize) {
		char* token;
		int colIndex = 0;

		token = strtok(line, ",");
		while (token != NULL) {
			token = trimWhitespace(token);
			if (colIndex == valueColumn) {  // Extract the correct column
				if (strlen(token) == 0) {   // Handle empty values
					printf("Warning: Empty value at row %d, setting to 0.\n", k + 1);
					dataset[k] = 0.0f;
				}
				else {
					dataset[k] = strtof(token, NULL);
				}
				k++;
				break;
			}
			colIndex++;
			token = strtok(NULL, ",");
		}
	}
}
