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
	return 1;
}

int getval(FILE *f, const char *fname) {

	char fbuf[MBCH] = "\0";

	if (!f) errno = ENOENT;
	else fgets(fbuf, MBCH, f);

	return atoi(fbuf);
}

int mkint(const char *str) {

	char *ptr;

	long ret = strtol(str, &ptr, 10);
	if (str == ptr) errno = EINVAL;

	return (int) ret;
}

int sfix(char *str, int cur, int max) {

	if (strncmp(str, "max", MBCH) == 0) return max;
	else if (str[0] == '+') return (cur + mkint(++str));
	else if (str[0] == '-') return (cur - mkint(++str));
	else return mkint(str);
}

int sper(char *str, int cur, int max) {

	if (str[0] == '+')
		return(int) ((float)mkint(++str)/100 * (float)max) + (float)cur;
	else if (str[0] == '-')
		return(int) (float)cur - (float)mkint(++str)/100 * (float)max;
	else
		return(int) ((float)mkint(str)/100 * (float)max);  
}

int main(int argc, char *argv[]) {

	FILE *mf = fopen(MAXF, "r");
	int max = getval(mf, MAXF);

	FILE *bf = fopen(BRIF, "w+");
	int cur = getval(bf, BRIF);

	int setnew = 0;

	if (errno) return usage(argv[0], max, bf);

	if (argc > 2) {
		return usage(argv[0], max, bf);

	} else if (argc == 1 && !errno) {
		int perc = ((float)cur / (float)max) * 100;
		printf("cur: %d (%d%%), max: %d\n", cur, perc, max);
		return 0;
	}

	if (argv[1][strlen(argv[1])-1] == '%') setnew = sper(argv[1], cur, max);
	else setnew = sfix(argv[1], cur, max);
	if (setnew > max) setnew = max;
	else if (setnew < 0) setnew = 0;

	if (errno) return usage(argv[0], max, bf);

	fprintf(bf, "%d", setnew);

	fclose(mf);
	fclose(bf);
	return 0;
}
