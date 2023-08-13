#define _CRT_SECURE_NO_WARNINGS

#define TEST 0
#define VCM_MODELING 0

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "Wacoh.h"

int main() {
	serial_connect("\\\\.\\COM8"); //connect to the comport, you must add \\\\.\\ in front of the name.
	float data[6] = { 0 }; //the container to save the six-dimension data from the force sensor

	#if !TEST && !VCM_MODELING
		FILE* fpt; //file pointer
		double x_value = 0;
		const int size = 300;
		double x[size] = { 0 };
		double y[size] = { 0 };
		double timeTaken = 0;
		clock_t t;

		//while (1) {
		while (x_value <= 10) {
			t = clock();

			x_value += timeTaken;

			for (int i = 0; i < (size - 1); i++) //move each entry one index ahead
			{
				x[i] = x[i + 1];
				y[i] = y[i + 1];
			}

			WacohRead(data); //get the data

			x[size - 1] = x_value; //fill the last entry in x
			y[size - 1] = 1.22 - data[2]; //fill the last entry in y

			fpt = fopen("data.csv", "w"); //open and point to the file
			fprintf(fpt, "x_value,y_value\n"); //write in the file
			for (int i = 0; i < size; i++) {
				fprintf(fpt, "%.2f, %.2f\n", x[i], y[i]);
				printf("%.2f, %.2f\n", x[i], y[i]);
			}
			fclose(fpt);
			
			Sleep(3);

			t = clock() - t;
			timeTaken = ((double)t) / CLOCKS_PER_SEC; // in seconds
		}
	#endif

	#if TEST && !VCM_MODELING
		float sum = 0;
		int count = 0;
		int times = 500;
		while (wacoh_isConnected && count < times) {
			WacohRead(data);	
			sum = sum + data[2];
			count++;
		}
		printf("Fz:%.2f\n", sum / times);
	#endif

	#if VCM_MODELING
		int count = 0;
		double total = 0;
		double ans = 0;
		for (int i = 0; i < 250; i++) {
			WacohRead(data);
			if (data[2] > 50) //discard the large error
			{
				continue;
			}
			total = total + 3.4 - data[2];
			count++;
		}
		ans = total / count;
		printf("%.2f", ans);
	#endif

	return 0;
}