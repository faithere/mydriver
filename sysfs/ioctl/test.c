#include	<stdio.h>
#include 	<stdlib.h>
#include 	<string.h>
#include	<fcntl.h>
#include "ioctl-drv.h"

int main( int argc, char *argv[] )
{
	int 	DeviceHandle = -1;

	if((DeviceHandle=open("/dev/ioctl", O_RDWR )) <= 0 )
	{
		printf("Open ioctl device is failed! \n");
		return 0;
	}
	
	ioctl(DeviceHandle, IOCTL_TEST1, &argv[1], NULL);
	ioctl(DeviceHandle, IOCTL_TEST2, &argv[1], NULL);
		
		return 0;		
}
