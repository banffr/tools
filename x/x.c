#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_IN_LINE 0x10

int main(int i, char **v)
{
	off_t o = 0;
	ssize_t s = 1;
	int d, e = 0;
	char *ptr, buff[MAX_IN_LINE];
	unsigned char c;
	unsigned long j, k, l = 0x40;
	if (i < 2) {
		fprintf(stderr, "Usage: x <filenmae> [<offset> [<length>]]\n");
		return 0;
	}
	if (i > 2)
		o = strtol(v[2], &ptr, MAX_IN_LINE);
	if (i > 3)
		l = strtol(v[3], &ptr, MAX_IN_LINE);
	d = open(v[1], O_RDONLY);
	if (d < 0) {
		perror(v[1]);
		return 0;
	}
	memset(buff, ' ', sizeof(buff));
	for (k = o; s == 1 && k < o + l;) {
		if (k == o && (o % MAX_IN_LINE)) {
			printf("%08lx:", (k / MAX_IN_LINE) * MAX_IN_LINE);
			for (j = 0; j < k % MAX_IN_LINE; j++)
				printf("   ");
		}
		if (!(k % MAX_IN_LINE)) {
			printf("%08lx:", k);
		}
		s = pread(d, &c, 1, k);
		if (s != 1) {
			e = errno;
		} else {
			if ((k % MAX_IN_LINE) == (MAX_IN_LINE/2))
				printf("-%02x", c);
			else
				printf(" %02x", c);
			buff[k++ % MAX_IN_LINE] = c;
		}
		if ((s != 1 && (k % MAX_IN_LINE)) || (s == 1 && !(k % MAX_IN_LINE)) || k == o + l) {
			for (j = k % MAX_IN_LINE; j > 0 && j < MAX_IN_LINE; j++)
				printf("   ");
			printf("  ");
			for (j = 0; j <= ((k - 1) % MAX_IN_LINE); j++) {
				c = buff[j];
				if (isprint(c))
					printf("%c", c);
				else
					printf(".");
			}
		}
		if (s != 1 || !(k % MAX_IN_LINE) || k == o + l) {
			printf("\n");
		}
	}
	close(d);
	if (e)
		fprintf(stderr, "%s: %s\n", v[1], strerror(e));
	return 0;
}
