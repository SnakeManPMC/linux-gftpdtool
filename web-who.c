#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#define MAXPATHLEN    4096
#define PIDSPATH      "/.ftp-data/pids"
#include "misc/jonni/online.h"

int read_online(char *p, char *fn, struct ONLINE *o)
{
	char buf[300];
	FILE *f;

	sprintf(buf, "%s/%s", p, fn);
	if (f = fopen(buf, "rb"))
	{
		if (!fread(o, sizeof(struct ONLINE), 1, f))
		{
			fclose(f);
			return 0;
		}
		fclose(f);
	}
	else
		return 0;

	return 1;
}

int main(int args, char *argv[])
{
	struct dirent *de;
	struct ONLINE oe;
	DIR *d;

	d = opendir(PIDSPATH);

	if (!d)
	{
		printf("Error opening dir: %s\n", PIDSPATH);
		exit(1);
	}

	printf("<table>");

	while (de = readdir(d))
	{
		if (!strncmp(de->d_name, ".", 1))
			continue;

		if (!read_online(PIDSPATH, de->d_name, &oe))
			continue;

		printf("<tr><td>%s</td><td>%s</td><td>%s</td><td>%0.1f kB/s</td></tr>",
		       oe.username,
		       oe.unfo,
		       oe.status,
		       oe.lastrate);
	}

	printf("</table>");

	closedir(d);

	return 1;
}
