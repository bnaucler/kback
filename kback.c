#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MBCH 16
#define MAXF "/sys/class/backlight/intel_backlight/max_brightness"
#define BRIF "/sys/class/backlight/intel_backlight/brightness"

int usage(const char *cn, const int max, FILE *f) {

	if(errno) fprintf(stderr, "Error: %s\n", strerror(errno));

	printf("Usage: %s [[0-%d]|[[+|-]0-100%%]|max]\n", cn, max);
	fclose(f);

	return errno;
}

int mkint(const char *str) {

	char *ptr;

	long ret = strtol(str, &ptr, 10);
	if (str == ptr) errno = EINVAL;

	return (int) ret;
}

int getval(FILE *f) {

	char fbuf[MBCH] = "";

	if (!f) errno = ENOENT;
	else fgets(fbuf, MBCH, f);

	return mkint(fbuf);
}

int sfix(char *str, int cur, int max) {

	if (!strncmp(str, "max", MBCH)) return max;
	else if (str[0] == '+' || str[0] == '-') return (cur + mkint(str));
	else return mkint(str);
}

int sper(char *str, int cur, int max) {

	if (str[0] == '+' || str[0] == '-')
		 return(int) ((float)mkint(str)/100 * (float)max) + (float)cur;
	else return(int) ((float)mkint(str)/100 * (float)max);
}

int main(int argc, char *argv[]) {

	int max = 0, cur = 0;

	FILE *mf = fopen(MAXF, "r");
	if(!errno) max = getval(mf);
	fclose(mf);

	FILE *bf = fopen(BRIF, "w+");
	if(!errno) cur = getval(bf);

	if (errno) return usage(argv[0], max, bf);

	if (argc > 2) {
		return usage(argv[0], max, bf);

	} else if (argc == 1 && !errno) {
		printf("cur: %d (%d%%), max: %d\n",
			cur, (int)(((float)cur / (float)max) * 100), max);
		return 0;
	}

	int setnew = 0;

	if (argv[1][strlen(argv[1])-1] == '%') setnew = sper(argv[1], cur, max);
	else setnew = sfix(argv[1], cur, max);
	if (setnew > max) setnew = max;
	else if (setnew < 0) setnew = 0;

	if (errno) return usage(argv[0], max, bf);

	fprintf(bf, "%d", setnew);

	fclose(bf);
	return 0;
}
