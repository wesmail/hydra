using namespace std;
#include "hprocessstat.h"
#include <fcntl.h>
						 
ClassImp(HProcessStat)

HProcessStat::HProcessStat(const Char_t* name,const Char_t* title)
    :TNamed(name,title)
{
}
HProcessStat::HProcessStat()
{
}
HProcessStat::~HProcessStat(){}

void HProcessStat::print()
{
	printf("####### Process Stat #############\n");
	printf("pid       :%i\n",pid);
	printf("name      :%s\n",name);
	printf("priority  :%i\n",pri);
	printf("nice      :%i\n",nice);
	printf("size      :%ld\n",size);
	printf("rss       :%ld\n",rss);
	switch (state)
	{
	case 1: printf("state     : R\n"); break;
	case 2: printf("state     : S\n"); break;
	case 3: printf("state     : D\n"); break;
	case 4: printf("state     : Z\n"); break;
	case 5: printf("state     : T\n"); break;
	case 6: printf("state     : W\n"); break;
	}
	printf("time      :%ld\n",time);
	// No longer supported
	//printf("pcpu      :%f\n",pcpu);
	//printf("wcpu      :%f\n",wcpu);
	printf("##################################\n");
}
void HProcessStat::update()
{
	Char_t buffer[4096];
	Char_t tmp[3];
	Int_t time1,time2;
        time1 = time2 = 0;
  // grab the proc stat info in one go
	{
		Int_t fd, len;

		sprintf(buffer, "/proc/%d/stat", pid);

		fd = open(buffer,O_RDONLY);
		len = read(fd, buffer, sizeof(buffer)-1);
		close(fd);

		buffer[len] = '\0';
	}

  // parse out the status
	// As documented in (Kernel code directory)/Documentation/filesystems/proc.txt
	// 1) %*d - pid
	// 2) %s  - tcomm     name
	// 3) %s  - state     tmp
	// 4) %*d - ppid
	// 5) %*d - pgrp
	// 6) %*d - sid
	// 7) %*d - tty_nr
	// 8) %*d - tty_pgid
	// 9) %*u - flags
	// 10) %*u - min_flt
	// 11) %*u - cmin_flt
	// 12) %*u - maj_flt
	// 13) %*u - cmaj_flt
	// 14) %d  - utime      time1
	// 15) %d  - stime      time2
	// 16) %*d - cutime
	// 17) %*d - cstime
	// 18) %d  - priority   pri
	// 19) %d  - nice
	// 20) %*d - num_threads
	// 21) %*d - start_time
	// 22) %u  - vsize      size
	// 23) %u  - rss        rss

	sscanf(buffer,"%*d %s %s %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %d %d %*d %*d %d %d %*d %*d %lu %lu",
	       name,tmp,
	       &time1,&time2,&pri,&nice,&size,&rss);

	time = time1 + time2;
	switch (*tmp)
	{
		case 'R': state = 1; break;
		case 'S': state = 2; break;
		case 'D': state = 3; break;
		case 'Z': state = 4; break;
		case 'T': state = 5; break;
		case 'W': state = 6; break;
	}
}


