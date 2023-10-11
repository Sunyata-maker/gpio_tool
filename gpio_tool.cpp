

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>  //define O_WRONLY and O_RDONLY


#define VERSION "V1.0.0"

void initGpio(int n)  
{  
    FILE * fp =fopen("/sys/class/gpio/export","w");  
    if (fp == NULL)  
        perror("export open filed");  
    else  
        fprintf(fp,"%d",n);  
    fclose(fp);  
}   //create gpio file  
  
void setGpioDirection(int n, char *direction)  
{  
    char path[100] = {0};  
    sprintf(path,"/sys/class/gpio/gpio%d/direction",n);  
    FILE * fp =fopen(path,"w");  
    if (fp == NULL)  
        perror("direction open filed");  
    else  
        fprintf(fp,"%s",direction);  
    fclose(fp);  
}   //set gpio "in" or "out"  
  
int getGpioValue(int n)  
{  
    char path[64];  
    char value_str[3];  
    int fd;  
  
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", n);  
    fd = open(path, O_RDONLY);  
    if (fd < 0) {  
        perror("Failed to open gpio value for reading!");  
        return -1;  
    }  
  
    if (read(fd, value_str, 3) < 0) {  
        perror("Failed to read value!");  
        return -1;  
    }  
  
    close(fd);  
    return (atoi(value_str));  
}   //get gpio(n)'s value  
  
  
void setGpioValue(int n, int val)  
{  
    char path[100] = {0};  
    sprintf(path,"/sys/class/gpio/gpio%d/value",n);  
    FILE * fp =fopen(path,"w");  
    if (fp == NULL)  
        perror("direction open filed");  
    else  
        fprintf(fp,"%d",val);  
    fclose(fp);
}   //set gpio(n)'s value  

int main(int argc, char* argv[])
{
    if(argc < 4)
    {
	printf("build time:%s %s ver:%s \n", __DATE__, __TIME__, VERSION);
        printf("useage: get gpio2_2 val;  ./gpio_tool r 2 2\n");
        printf("set gpio2_2 val(0 or 1);  ./gpio_tool w 2 2 1\n");

        return -1;
    }
    
    int gpioVal = atoi(argv[2]) * 8 + atoi(argv[3]);
    
    initGpio(gpioVal);

    if(strcmp(argv[1], "W") == 0 || strcmp(argv[1], "w") == 0)
    {
        setGpioDirection(gpioVal, (char*)"out");

        setGpioValue(gpioVal, (atoi(argv[4]) == 0) ? 0 : 1);  
    }
    else
    {
        setGpioDirection(gpioVal, (char*)"in");
        
        int getVal = getGpioValue(gpioVal);
	printf("get val:%d \n", getVal);
	return getVal;
    }
    
    return 0;
}


