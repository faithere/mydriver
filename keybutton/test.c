#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
   int fileno;
   int number;

   fileno = open("/dev/keybutton",O_RDWR);

   if (fileno == -1)
   {
   	printf("open buttons device errr!\n");
	return 0;
   }
   while(1) 
   {
   	read(fileno,&number,1);
   	printf("key=0x%x\n",number);
   }

   close(fileno);
   return 0;
}

