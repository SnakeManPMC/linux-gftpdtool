#include <stdio.h>

int main()
{
/*
   !e|!c 03-22-99 !e|!D PMC       991MB!e|!H VGN         0MB!e|!d IND         0MB!e|!hlonewolf !e|!H
 */

	FILE *gfile;

	char temp[100];

	gfile = fopen("group.base", "rt");

	fgets(temp, 100, gfile);

	while (!feof(gfile))
	{
		printf(temp);
		fgets(temp, 100, gfile);
	}

	fclose(gfile);
	return 0;
}