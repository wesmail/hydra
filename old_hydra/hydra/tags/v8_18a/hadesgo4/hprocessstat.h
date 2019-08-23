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
//#include <linux/tasks.h>	// for NR_TASKS  for the moment comented becaus it doe not work with new compiler
/*
#if 0
#include <linux/proc_fs.h>	// for PROC_SUPER_MAGIC
#else
#define PROC_SUPER_MAGIC 0x9fa0
#endif

#define PROCFS "/proc"
Char_t *myname;
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

    Int_t pid;
    Int_t uid;
    Char_t name[64]; //!
    Int_t pri, nice;
    ULong_t size, rss;	// in k
    Int_t state;
    ULong_t time;
    Double_t pcpu, wcpu;

inline Char_t * skip_ws(const Char_t *p){while (isspace(*p)) p++; return (Char_t *)p;}
inline Char_t * skip_token(const Char_t *p){while (isspace(*p)) p++; while (*p && !isspace(*p)) p++; return (Char_t *)p;}

public:
    HProcessStat(const Char_t*,const Char_t*);
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
