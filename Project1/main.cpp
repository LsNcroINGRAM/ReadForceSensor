#define _CRT_SECURE_NO_WARNINGS

#define TEST 0
#define VCM_MODELING 1

#include <stdio.h>
#include "Wacoh.h"
#include <windows.h>

int main()
{
	serial_connect("\\\\.\\COM10"); //connect to the comport, you must add \\\\.\\ in front of the name.
	float data[6] = { 0 }; //the container to save the six-dimension data from the force sensor

	#if !TEST && !VCM_MODELING
		FILE* fpt; //file pointer
		unsigned int x_value = 0;
		const int size = 100;
		int x[size] = { 0 };
		double y[size] = { 0 };

		while (1)
		{
			for (int i = 0; i < (size - 1); i++) //move each entry one index ahead
			{
				x[i] = x[i + 1];
				y[i] = y[i + 1];
			}
			WacohRead(data); //get the data
			if (data[2] > 100) //discard the large error
			{
				break;
			}
			x[size - 1] = x_value++; //fill the last entry in x
			y[size - 1] = 4.7 - data[2]; //fill the last entry in y

			if (x_value > (size * 100)) //make sure x_vaule won't overflow
			{
				x_value = 0;
			}

			fpt = fopen("data.csv", "w"); //open and point to the file
			fprintf(fpt, "x_value,y_value\n"); //write in the file
			for (int i = 0; i < size; i++)
			{
				fprintf(fpt, "%d, %.1f\n", x[i], y[i]);
				printf("%d, %.1f", x[i], y[i]);
				printf("\n");
			}

			fclose(fpt);

			Sleep(20);
}
	#endif

	#if TEST && !VCM_MODELING
		float sum[6] = { 0 };
		int count = 0;
		int times = 500;
		while (wacoh_isConnected && count < times)
		{
			WacohRead(data);	
			for (int i = 0; i < 6; i++)
			{
				sum[i] = sum[i] + data[i];
			}
			count++;
		}
		printf("Fx:%.2f	Fy:%.2f	Fz:%.2f	Mx:%.2f	My:%.2f	Mz:%.2f\n",
			sum[0] / times, sum[1] / times, sum[2] / times, sum[3] / times, sum[4] / times, sum[5] / times);
	#endif

	#if VCM_MODELING
		int count = 0;
		double total = 0;
		double ans = 0;
		for (int i = 0; i < 250; i++)
		{
			WacohRead(data);
			if (data[2] > 50) //discard the large error
			{
				continue;
			}
			total = total + 4.79 - data[2];
			count++;
		}
		ans = total / count;
		printf("%.2f", ans);
	#endif

	return 0;
}