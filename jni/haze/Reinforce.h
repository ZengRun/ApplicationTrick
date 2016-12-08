/*
 * Reinforce.h
 */
#include <sys/types.h>

#ifdef unix
#define ReinforceSeedFileName 	"seed.reinforce"
#else
#define ReinforceSeedFileName 	"seed.reinforce"
#endif

static int confuseFunc(int input);
static int reconfuseFunc(int input1);
static int rebuildIKey(char *a, int length, char *appKey);
int getIKey(char *iKey);

