#include <stdio.h>
#include "Wacoh.h"

int main()
{
	serial_close();
	serial_connect("\\\\.\\COM10"); //connect to the comport, you must add \\\\.\\ in front of the name.

	float data[6] = { 0 };
	while (wacoh_isConnected)
	{
		WacohRead(data);
		printf("X:%.2f		Y:%.2f		Z:%.2f		MX:%.2f		MY:%.2f		MZ:%.2f\n", data[0], data[1], data[2], data[3], data[4], data[5]);
	}

	return 0;
}