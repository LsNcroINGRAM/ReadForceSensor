#define TEST 0

#include <stdio.h>
#include "Wacoh.h"
#include <windows.h>

int main()
{
	serial_connect("\\\\.\\COM10"); //connect to the comport, you must add \\\\.\\ in front of the name.
	float data[6] = { 0 };

	#if !TEST
		while (wacoh_isConnected)
		{
			WacohRead(data);
			printf("Fz:%.2f\n",
				-data[2] + 0.2);

			Sleep(100);
		}
	#endif

	#if TEST
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

	return 0;
}