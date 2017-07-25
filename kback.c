#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MBCH 16
#define MAXF "/sys/class/backlight/intel_backlight/max_brightness"
#define BRIF "/sys/class/backlight/intel_backlight/brightness"

static int usage(const char *cn, const int max, FILE *f) {

	if(errno) fprintf(stderr, "Error: %s\n", strerror(errno));

	printf("Usage: %s [[0-%d]|[[+|-]0-100%%]|max]\n", cn, max);
	fclose(f);

	return errno;
}

static int matoi(const char *str) {

	char *ptr;

	long ret = strtol(str, &ptr, 10);
	if(str == ptr) errno = EINVAL;

	return (int)ret;
}

static int getval(FILE *f) {

	char fbuf[MBCH];
	fgets(fbuf, MBCH, f);

	return matoi(fbuf);
}

static int sfix(const char *str, const int cur, const int max) {

	if(!strncmp(str, "max", MBCH)) return max;
	else if(str[0] == '+' || str[0] == '-') return (cur + matoi(str));
	else return matoi(str);
}

static int sper(const char *str, const int cur, const int max) {

	if(str[0] == '+' || str[0] == '-')
		return(int) ((float)matoi(str)/100 * (float)max) + (float)cur;
	else return(int) ((float)matoi(str)/100 * (float)max);
}

int main(int argc, char **argv) {

	int max = 0, cur = 0, nval = 0;
	FILE *mf, *bf;

	if((mf = fopen(MAXF, "r"))) max = getval(mf);
	fclose(mf);

	if((bf = fopen(BRIF, "w+"))) cur = getval(bf);

	if(errno) return usage(argv[0], max, bf);

	if(argc > 2) {
		return usage(argv[0], max, bf);

	} else if(argc == 1) {
		printf("cur: %d (%d%%), max: %d\n",
			cur, (int)(((float)cur / (float)max) * 100), max);
		return 0;
	}

	if(argv[1][strlen(argv[1])-1] == '%') nval = sper(argv[1], cur, max);
	else nval = sfix(argv[1], cur, max);

	if(nval > max) nval = max;
	else if(nval < 0) nval = 0;

	if(errno) return usage(argv[0], max, bf);

	fprintf(bf, "%d", nval);
	fclose(bf);

	return 0;
}
