#include "global.h"
#include <unistd.h>
#include<string.h>
#include"config.h"
void init()
{
  load_aliases();
  strcpy(TWOSECMD,"");
  getcwd(HOME_DIR, sizeof(HOME_DIR));
  getcwd(CUR_DIR, sizeof(CUR_DIR));
  getcwd(PREV_DIR,sizeof(PREV_DIR));
  gethostname(HOSTNAME, sizeof(HOSTNAME));
  getlogin_r(USERNAME,sizeof(USERNAME));
  SHELLGROUPID=getpgid(getpid());
  return;
}
