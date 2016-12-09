#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MBCH 16
#define MAXF "/sys/class/backlight/intel_backlight/max_brightness"
#define BRIF "/sys/class/backlight/intel_backlight/brightness"

int usage(const char *cn, const int max, FILE *f) {
	printf("Usage: %s [[0-%d]|[[+|-]0-100%%]|max]\n", cn, max);
	fclose(f);
	return 1;
}

int getval(FILE *f, const char *fname) {

	char fbuf[MBCH];
	int ret = 0;

	if (f == NULL) {
		printf("Could not open %s\n", fname);
		return -1;
	} else {
		while(fgets(fbuf, MBCH, f)) {
			ret = atoi(fbuf);
		}
	}

	return ret;
}

int mkint(const char *str) {

	long val = 0;
	char *ptr;

	val = strtol(str, &ptr, 10);
	if (ptr == str) {
		return -1;
	} else {
		return (int) val;
	}
}

int sfix(char *str, int cur, int max) {

	int ret = 0;

	if (strncmp(str, "max", MBCH) == 0) {
		ret = max;

	} else if (str[0] == '+') {
		str++;
		ret = (cur + mkint(str));

	} else if (str[0] == '-') {
		str++;
		ret = (cur - mkint(str));

	} else {
		ret = mkint(str);
	}

	return ret;
}

int sper(const char *str, int cur, int max) {

	int len = strlen(str);
	char *noper = calloc(len, sizeof(char));
	float flch = 0, flret = 0;

	strncpy(noper, str, len-1);
	noper++;
	int val = mkint(noper);

	if (val < 0 || val > 100) {
		return -1;
	} else {
		flch = (float)val/100 * (float)max;

		if (str[0] == '+') {
			flret = (float)cur + flch;

		} else if (str[0] == '-') {
			flret = (float)cur - flch;

		} else {
			flret = flch;
		}
	}

	return (int) flret;
}

int main(int argc, char *argv[]) {

	FILE *mf = fopen(MAXF, "r");
	int max = getval(mf, MAXF);
	fclose(mf);

	FILE *bf = fopen(BRIF, "w+");
	int cur = getval(bf, BRIF);

	int setnew = 0;

	if (argc > 2) {
		return usage(argv[0], max, bf);

	} else if (argc == 1) {
		printf("cur: %d, max: %d\n", cur, max);
		return 0;
	}

	if (argv[1][strlen(argv[1])-1] == '%') {
		setnew = sper(argv[1], cur, max);
	} else {
		setnew = sfix(argv[1], cur, max);
	}

	if (setnew < 0 || setnew > max) {
		return usage(argv[0], max, bf);

	} else {
		fprintf(bf, "%d", setnew);
	}

	fclose(bf);
	return 0;
}
