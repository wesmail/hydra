using namespace std;
#include "hprocessstat.h"
#include <fstream>
#include <iostream> 
#include <iomanip>

ClassImp(HProcessStat)

HProcessStat::HProcessStat(const char* name,const char* title)
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
	printf("pcpu      :%f\n",pcpu);
	printf("wcpu      :%f\n",wcpu);
	printf("##################################\n");
}
void HProcessStat::update()
{
    char buffer[4096], *p;

    // grab the proc stat info in one go
    {
	int fd, len;

	sprintf(buffer, "/proc/%d/stat", pid);

	fd = open(buffer,O_RDONLY);
	len = read(fd, buffer, sizeof(buffer)-1);
	close(fd);

	buffer[len] = '\0';
    }

   // proc->uid = proc_owner(pid);

    // parse out the status
    
    p = buffer;
    p = strchr(p, '(')+1;			// skip pid
    {
	char *q = strrchr(p, ')');
	int len = q-p;
	if (len >= (Int_t) sizeof(name))
	    len = sizeof(name)-1;
	memcpy(name, p, len);
	name[len] = 0;
	p = q+1;
    }

    p = skip_ws(p);
    switch (*p++)
    {
      case 'R': state = 1; break;
      case 'S': state = 2; break;
      case 'D': state = 3; break;
      case 'Z': state = 4; break;
      case 'T': state = 5; break;
      case 'W': state = 6; break;
    }
    
    p = skip_token(p);				// skip ppid
    p = skip_token(p);				// skip pgrp
    p = skip_token(p);				// skip session
    p = skip_token(p);				// skip tty
    p = skip_token(p);				// skip tty pgrp
    p = skip_token(p);				// skip flags
    p = skip_token(p);				// skip min flt
    p = skip_token(p);				// skip cmin flt
    p = skip_token(p);				// skip maj flt
    p = skip_token(p);				// skip cmaj flt
    
    time  = strtoul(p, &p, 10);		// utime
    time += strtoul(p, &p, 10);		// stime

    p = skip_token(p);				// skip cutime
    p = skip_token(p);				// skip cstime

    pri  = strtol(p, &p, 10);		// priority
    nice = strtol(p, &p, 10);		// nice

    p = skip_token(p);				// skip timeout
    p = skip_token(p);				// skip it_real_val
    p = skip_token(p);				// skip start_time

    size = bytetok(strtoul(p, &p, 10));	// vsize
    rss  = pagetok(strtoul(p, &p, 10));	// rss

//#if 0
    // for the record, here are the rest of the fields
    p = skip_token(p);				// skip rlim
    p = skip_token(p);				// skip start_code
    p = skip_token(p);				// skip end_code
    p = skip_token(p);				// skip start_stack
    p = skip_token(p);				// skip sp
    p = skip_token(p);				// skip pc
    p = skip_token(p);				// skip signal
    p = skip_token(p);				// skip sigblocked
    p = skip_token(p);				// skip sigignore
    p = skip_token(p);				// skip sigcatch
    p = skip_token(p);				// skip wchan
//#endif

}


