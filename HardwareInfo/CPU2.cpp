#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE 1024
#define iINTERVAL 1
#define uiINTERVAL 500000

typedef long int LL_i;

typedef struct{
	LL_i user;
	LL_i nice; 
	LL_i system;
	LL_i  idle;
	LL_i  iowait;
	LL_i  irq;
	LL_i  softirq;
}CPU_t;

CPU_t *refresh_cpu(CPU_t *cpu)
{
	FILE *fp = NULL;
	int num;
	char buf[BUFSIZE];
	if( fp == NULL )
	{
		if((fp = fopen("/proc/stat","r")) == NULL)
		{	
			perror("open /proc/stat");
			fclose(fp);
			return(NULL);
		}
	}
	rewind(fp);
	fflush(fp);

	if(!fgets(buf, sizeof(buf), fp))
	{
		perror("read /proc/stat");
		fclose(fp);
		return(NULL);
	}
	num = sscanf(buf, "cpu %Lu %Lu %Lu %Lu %Lu %Lu %Lu", &(cpu->user),(&cpu->nice ) ,(&cpu->system ),(&cpu->idle ),(&cpu->iowait ),(&cpu->irq ),(&cpu->softirq ));

	if (num < 7)
	{
		perror("read num /proc/stat");
		fclose(fp);
		return(NULL);
	}
	return cpu;

}


float display_cpu( CPU_t *cpu)
{
	refresh_cpu(cpu);
	int total1 = cpu->user + cpu->nice + cpu->system + cpu->idle + cpu->iowait + cpu->irq + cpu->softirq;
	int idle1 = cpu->idle;

	//sleep(iINTERVAL);
	usleep(uiINTERVAL);
	
	refresh_cpu(cpu);
	int total2 = cpu->user + cpu->nice + cpu->system + cpu->idle + cpu->iowait + cpu->irq + cpu->softirq;
	int idle2 = cpu->idle;

	float cpu_usage = (float)(float)((total2 - total1)-(idle2 - idle1))/(float)(total2 -total1);
	return cpu_usage;


}

void display_mem(void)
{
      	FILE	*fp_mem = NULL;
	char	buf[BUFSIZE];
	int  totalmem, freemem, buffers, cached,used;

	if (fp_mem == NULL) {
		if ((fp_mem = fopen("/proc/meminfo", "r")) == NULL) 
		{
			perror("open /proc/meminfo failed");
			return;
		}
	}
	rewind(fp_mem);
	fflush(fp_mem);
	while (1) 
	{
		if (!fgets(buf, sizeof(buf), fp_mem)) 
		{
			perror("read /proc/meminfo failed");
			return;
		}
		if (memcmp(buf, "MemTotal:", 9) == 0) 
		{
			char	tmp[BUFSIZE];
			strcpy(tmp, buf);
			strtok(tmp, " ");
			totalmem = atoi(strtok('\0', " "));
			//printf("TotalMem = %d\n", totalmem);
		}
	        if (memcmp(buf, "MemFree:", 8) == 0) 
		{
			char	tmp[BUFSIZE];
			strcpy(tmp, buf);
			strtok(tmp, " ");
			freemem = atoi(strtok('\0', " "));
			//printf("FreeMem = %d\n", freemem);
		}
	        if (memcmp(buf, "Buffers:", 8) == 0) 
		{
			char	tmp[BUFSIZE];
			strcpy(tmp, buf);
			strtok(tmp, " ");
			buffers = atoi(strtok('\0', " "));
			//printf("Buffers = %d\n", buffers);
		}
		if(memcmp(buf, "Cached", 5) == 0) 
		{
			char	tmp[BUFSIZE];
			strcpy(tmp, buf);
			strtok(tmp, " ");
			cached = atoi(strtok('\0', " "));
			//printf("Cached = %d\n", cached);
		       	break;
		}
	}
	used = totalmem - freemem - cached - buffers;
	float mem_used = used /1024;
	printf("Used Mem: %fMB  %f%% \n",mem_used,100.0* used/(float)totalmem);
	//printf("Used Mem : %fMB\n",mem_used);
}

int main()
{
	CPU_t cpu;
	float cpu_use;
	while(1)
	{
		cpu_use = display_cpu(&cpu)*100;
		printf("CPU Usage: %f %%  ", cpu_use );
		//sleep(iINTERVAL);
		usleep(uiINTERVAL);
		display_mem();
	}
	return 0;
}
