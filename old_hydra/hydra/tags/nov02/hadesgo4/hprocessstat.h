#ifndef HPROCESSSTAT_H
#define HPROCESSSTAT_H

//#include "top.h"
//#include "machine.h"
//#include "utils.h"

//#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
//#include <dirent.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
//#include <sys/time.h>
//#include <sys/stat.h>
//#include <sys/vfs.h>

//#include <sys/param.h>		// for HZ
#include <asm/page.h>		// for PAGE_SHIFT
#include <linux/tasks.h>	// for NR_TASKS
/*
#if 0
#include <linux/proc_fs.h>	// for PROC_SUPER_MAGIC
#else
#define PROC_SUPER_MAGIC 0x9fa0
#endif

#define PROCFS "/proc"
char *myname;
*/
#include "TNamed.h"


#define HASH_SIZE	(NR_TASKS * 3 / 2)
#define bytetok(x)	(((x) + 512) >> 10)
#define pagetok(x)	((x) << (PAGE_SHIFT - 10))
#define HASH(x)		(((x) * 1686629713U) % HASH_SIZE)


/** Class to hold numbers for Hades Online Display.
 */
class HProcessStat : public TNamed {
private:

    int pid;
    int uid;
    char name[64]; //!
    int pri, nice;
    unsigned long size, rss;	// in k
    int state;
    unsigned long time;
    double pcpu, wcpu;

inline char * skip_ws(const char *p){while (isspace(*p)) p++; return (char *)p;}
inline char * skip_token(const char *p){while (isspace(*p)) p++; while (*p && !isspace(*p)) p++; return (char *)p;}

public:
    HProcessStat(const char*,const char*);
    HProcessStat();
    ~HProcessStat();
    void   setPid(Int_t pi){pid=pi;}
    Int_t  getPid()        {return pid;}
    void   update();
    void   getCurrentPid(){pid=getpid();}
    void   print();
    ClassDef(HProcessStat,0)
};
#endif  /*!HPROCESSSTAT_H*/
