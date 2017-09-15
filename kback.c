#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MBCH 8
#define MAXF "/sys/class/backlight/intel_backlight/max_brightness"
#define BRIF "/sys/class/backlight/intel_backlight/brightness"

static int usage(const char *cn, const long max, FILE *f) {

	if(errno) fprintf(stderr, "Error: %s\n", strerror(errno));

	printf("Usage: %s [[0-%ld]|[[+|-]0-100%%]|max]\n", cn, max);
	fclose(f);

	return errno;
}

static long matoi(const char *str) {

	char *ptr;

	long ret = strtol(str, &ptr, 10);
	if(str == ptr) errno = EINVAL;

	return ret;
}

static long getval(FILE *f) {

	char fbuf[MBCH];
	fgets(fbuf, MBCH, f);

	return matoi(fbuf);
}

static long sfix(const char *str, const long cur, const long max) {

	if(!strncmp(str, "max", MBCH)) return max;
	else if(str[0] == '+' || str[0] == '-') return cur + matoi(str);
	else return matoi(str);
}

static long sper(const char *str, const long cur, const long max) {

	return str[0] == '+' || str[0] == '-' ?
		(matoi(str) * max) / 100 + cur:
		(matoi(str) * max) / 100;
}

int main(int argc, char **argv) {

	long max = 0, cur = 0, nval = 0;
	FILE *mf, *bf;

	if((mf = fopen(MAXF, "r"))) max = getval(mf);
	if((bf = fopen(BRIF, "w+"))) cur = getval(bf);
	if(mf) fclose(mf);

	if(errno || argc > 2)
		return usage(argv[0], max, bf);
	else if(argc == 1)
        return printf("cur: %ld (%ld%%), max: %ld\n", cur, (cur * 100) / max, max);

	nval = (argv[1][strlen(argv[1])-1] == '%') ?
		sper(argv[1], cur, max):
		sfix(argv[1], cur, max);

	if(errno) return usage(argv[0], max, bf);

	if(nval > max) nval = max;
	else if(nval < 0) nval = 0;

	fprintf(bf, "%ld", nval);
	fclose(bf);

	return 0;
}
