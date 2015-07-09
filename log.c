#include <stdio.h>
#include <time.h>
#include "gftpdtool.h"

time_t t;

/*  listing of week/month database  */
int Week_Month()
{
	DATabase poks;

	FILE *datfile;

	int i = 0;

	datfile = fopen("database.dat", "rb");
	if (!datfile)
	{
		printf("error at database.dat");
		exit(1);
	}

	fread(&poks, sizeof(DATabase), 1, datfile);

	while (!feof(datfile))
	{
		if (poks.what == 1)
		{
			printf("******** WEEKTOP STATISTICS ON --> %s\n\n", ctime(&poks.date));
			printf("week=1 or month=2: %i\n", poks.what);
		}
		if (poks.what == 2)
		{
			printf("******** MONTHTOP STATISTICS ON --> %s\n\n", ctime(&poks.date));
			printf("week=1 or month=2: %i\n", poks.what);
		}

		i++;
		printf("position: %i\n", i);
		printf("username: %s\n", poks.user_name01);
		printf("location: %s\n", poks.location01);
		printf("files: %i\n", poks.files01);
		printf("bytes: %llu\n", poks.bytes01);
		i++;
		printf("position: %i\n", i);
		printf("username: %s\n", poks.user_name02);
		printf("location: %s\n", poks.location02);
		printf("files: %i\n", poks.files02);
		printf("bytes: %llu\n", poks.bytes02);
		i++;
		printf("position: %i\n", i);
		printf("username: %s\n", poks.user_name03);
		printf("location: %s\n", poks.location03);
		printf("files: %i\n", poks.files03);
		printf("bytes: %llu\n", poks.bytes03);
		i++;
		printf("position: %i\n", i);
		printf("username: %s\n", poks.user_name04);
		printf("location: %s\n", poks.location04);
		printf("files: %i\n", poks.files04);
		printf("bytes: %llu\n", poks.bytes04);
		i++;
		printf("position: %i\n", i);
		printf("username: %s\n", poks.user_name05);
		printf("location: %s\n", poks.location05);
		printf("files: %i\n", poks.files05);
		printf("bytes: %llu\n", poks.bytes05);
		i++;
		printf("position: %i\n", i);
		printf("username: %s\n", poks.user_name06);
		printf("location: %s\n", poks.location06);
		printf("files: %i\n", poks.files06);
		printf("bytes: %llu\n", poks.bytes06);
		i++;
		printf("position: %i\n", i);
		printf("username: %s\n", poks.user_name07);
		printf("location: %s\n", poks.location07);
		printf("files: %i\n", poks.files07);
		printf("bytes: %llu\n", poks.bytes07);
		i++;
		printf("position: %i\n", i);
		printf("username: %s\n", poks.user_name08);
		printf("location: %s\n", poks.location08);
		printf("files: %i\n", poks.files08);
		printf("bytes: %llu\n", poks.bytes08);
		i++;
		printf("position: %i\n", i);
		printf("username: %s\n", poks.user_name09);
		printf("location: %s\n", poks.location09);
		printf("files: %i\n", poks.files09);
		printf("bytes: %llu\n", poks.bytes09);
		i++;
		printf("position: %i\n", i);
		printf("username: %s\n", poks.user_name10);
		printf("location: %s\n", poks.location10);
		printf("files: %i\n", poks.files10);
		printf("bytes: %llu\n", poks.bytes10);

		printf("\nweekly bytes: %llu\n", poks.weekly_uploads);
		printf("alltime bytes: %llu\n", poks.alltime);
		printf("alltime user: %s\n", poks.best_alltime_user);
		printf("best user bytes: %llu\n\n\n", poks.best_alltime_users_bytes);

		fread(&poks, sizeof(DATabase), 1, datfile);
		i = 0;
	}

	fclose(datfile);

	return 0;
}

int Year_Log()
{
	typedef struct
	{
		char username[25];
		int files_uploaded;
		unsigned long long bytes_uploaded;
		int files_downloaded;
		unsigned long long bytes_downloaded;
	} YearTop;

	YearTop YDATA;

	FILE *datfile;

	int fish = 0;
	int i = 0;

	datfile = fopen("yeartop.dat", "rb");
	if (!datfile)
	{
		printf("error at yeartop.dat");
		exit(1);
	}

	fread(&fish, sizeof(time(0)), 1, datfile);
	t = fish;
	printf("Yeartop date: %s", ctime(&t));

	fread(&YDATA, sizeof(YearTop), 1, datfile);

	while (!feof(datfile))
	{
		printf("* * * * * * * * * * * * * * * * *\n");
		printf("username: %s\n", YDATA.username);
		printf("files up: %i\n", YDATA.files_uploaded);
		printf("bytes up: %llu\n", YDATA.bytes_uploaded);
		printf("files dn: %i\n", YDATA.files_downloaded);
		printf("bytes dn: %llu\n", YDATA.bytes_downloaded);

		fread(&YDATA, sizeof(YearTop), 1, datfile);
	}

	printf("* * * * * * the end * * * * * * *\n");
	fclose(datfile);

	return 0;
}

int main(int x, char argc[])
{
	if (x < 2)
		Week_Month();
	else
		Year_Log();

	return 0;
}