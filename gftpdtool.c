#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <time.h>
#include <pmc.h>
#include <pmcparse.h>
#include "gftpdtool.h"

#define ASIZE    200

const char Version[] = "v0.2b r4 05-28-00"; /* version numbah */

FILE *userfile;
FILE *bulletin;
FILE *outlook;
FILE *middle;
FILE *php;

glob_t gl;

char line[80];
char username[ASIZE][25];
char tagline[ASIZE][80];
char buf[100];
char *p;
char total[80];
int i, x, userbyte_pos[ASIZE], maxuser, num_groups, gp_rec = 0;

unsigned long long files_up[ASIZE], bytes_up[ASIZE], seconds_up[ASIZE];
unsigned long long files_down[ASIZE], bytes_down[ASIZE], seconds_down[ASIZE];
unsigned long long files_up_wk[ASIZE], bytes_up_wk[ASIZE], seconds_up_wk[ASIZE];
unsigned long long files_down_wk[ASIZE], bytes_down_wk[ASIZE], seconds_down_wk[ASIZE];
unsigned long long files_up_m[ASIZE], bytes_up_m[ASIZE], seconds_up_m[ASIZE];
unsigned long long files_down_m[ASIZE], bytes_down_m[ASIZE], seconds_down_m[ASIZE];
unsigned long long all_up, all_dn, all_wkup, all_wkdn, alltime_wkup, alltime_wkdn, all_rest, all_w_rest;
unsigned long long alltime_wkup_user, alltime_wkdn_user;
unsigned long long alltime_mup_user, alltime_mdn_user;
unsigned long long all_mup, all_mdn, alltime_mup, alltime_mdn;
unsigned long long allwkup_group_bytes, allwkdn_group_bytes;
unsigned long long allmup_group_bytes, allmdn_group_bytes;
unsigned long long week_bad[ASIZE], all_bad[ASIZE];

int login_times[ASIZE], acc_created[ASIZE], time_limit[ASIZE];
int time_on_today[ASIZE], last_nuked[ASIZE], speed_fuckups[ASIZE];
char added_by[ASIZE][25], comment[ASIZE][80];
char allwkup_user[25];
char allwkdn_user[25];
char allmup_user[25];
char allmdn_user[25];
char allwkup_group[14];
char allwkdn_group[14];
char allmup_group[14];
char allmdn_group[14];
char tempstr[80], tempstr2[80], tempstr3[80];
char parsebuf[200];
char middle_str[100];
char top_path[80];
char php_path[80];
char U_File[20];
long now, before, last_on[ASIZE];

/* how many users to list in the top's */
int alup_unum, aldn_unum, wkup_unum, wkdn_unum, mup_unum, mdn_unum;

int Header()
{
	printf("\n\n        gftpdtool %s (c) Snake Man, PMC 1999-2015.\n\n", Version);

	return 0;
}

/* reads the userfiles in the dir */
int File_Select()
{
	char *utemp[10];
	char temp_gl[80];
	int ui = 0;
	i = 0;
	printf("we are at file_select()\n");
	/* loop to read the files */
	printf(" reading userfiles...\n");

	for (x = 0; x < gl.gl_pathc; x++)
	{
		printf("in the for loop baby x: %i\n", x);

		strcpy(temp_gl, gl.gl_pathv[x]);
		ui = splitpath(utemp, temp_gl);
		sprintf(U_File, "%s", utemp[ui]);
		printf("U_File: %s\n", U_File);
		userfile = fopen(gl.gl_pathv[x], "rt");
		if (!userfile)
		{
			printf("Could not open userfile(s), exiting!\n");
			exit(1);
		}

		Info_Gather();
		fclose(userfile);
	}

	maxuser = i;
	/* loop end here......... */

	printf("Done!\n read total of %i user-records\nThanks for using gftpdtool\n", i);

	return 0;
}

/* reading the individual user information */
int Info_Gather()
{
	char temptag[80];
	unsigned long long tmp;

	if (strstr(gl.gl_pathv[x], "!!ERR!!"))
		return 0;
	if (strstr(gl.gl_pathv[x], "master"))
		return 0;
	if (strstr(gl.gl_pathv[x], "default"))
		return 0;
	if (strstr(gl.gl_pathv[x], "default.user"))
		return 0;

	fgets(line, 80, userfile);
	if (!strstr(line, "# General informati"))
		return 0;                          /* if not gftpd userfile */

	i++;
	strcpy(username[i], U_File); /* username */

	while (!feof(userfile))
	{
		if (strstr(line, "# Time-related information"))
		{
			fgets(line, 80, userfile); /* login_times */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%i", &login_times[i]);

			fgets(line, 80, userfile); /* last_on 83838383 */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%lu", &last_on[i]);

			fgets(line, 80, userfile); /* account_created 83838383 */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%i", &acc_created[i]);

			fgets(line, 80, userfile); /* added_by ? */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			strcpy(added_by[i], p);

			fgets(line, 80, userfile); /* comment ? */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			strcpy(comment[i], p);

			fgets(line, 80, userfile); /* time_limit */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%i", &time_limit[i]);

			fgets(line, 80, userfile); /* time_on_today */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%i", &time_on_today[i]);

			fgets(line, 80, userfile); /* last_nuked */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%i", &last_nuked[i]);

			fgets(line, 80, userfile); /* skip user_color */
			fgets(line, 80, userfile); /* skip quiet */

			fgets(line, 80, userfile); /* speed_fuckups */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%i", &speed_fuckups[i]);

			fgets(line, 80, userfile); /* week_bad */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%i", &week_bad[i]);

			fgets(line, 80, userfile); /* all_bad */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%i", &all_bad[i]);
		}

		if (strstr(line, "tagline"))
		{
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			strcpy(temptag, "");
			while (p) /* do tokens until we dont have anything */
			{
				strcat(temptag, p);    /* append the next   */
				strcat(temptag, " ");  /* some space...     */
				p = strtok(NULL, " "); /* tokenize another  */
			}

			if (strlen(temptag) > 30)
				temptag[31] = '\0';

			strncpy(tagline[i], temptag, strlen(temptag) - 2); /* removing \0 */
		}

		if (strstr(line, "# Normal Xfer"))
		{
			fgets(line, 80, userfile); /* files_up */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &files_up[i]);

			fgets(line, 80, userfile); /* bytes_up */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &tmp);
			bytes_up[i] = (tmp * 1024);

			fgets(line, 80, userfile); /* seconds_up */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &seconds_up[i]);

			fgets(line, 80, userfile); /* files_down */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &files_down[i]);

			fgets(line, 80, userfile); /* bytes_down */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &tmp);
			bytes_down[i] = (tmp * 1024);

			fgets(line, 80, userfile); /* seconds_down */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &seconds_down[i]);

			fgets(line, 80, userfile); /* files_up_wk */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &files_up_wk[i]);

			fgets(line, 80, userfile); /* bytes_up_wk */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &tmp);
			bytes_up_wk[i] = (tmp * 1024);

			fgets(line, 80, userfile); /* seconds_up_wk */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &seconds_up_wk[i]);

			fgets(line, 80, userfile); /* files_down_wk */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &files_down_wk[i]);

			fgets(line, 80, userfile); /* bytes_down_wk */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &tmp);
			bytes_down_wk[i] = (tmp * 1024);

			fgets(line, 80, userfile); /* seconds_down_wk */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &seconds_down_wk[i]);

			fgets(line, 80, userfile); /* files_up_m */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &files_up_m[i]);

			fgets(line, 80, userfile); /* bytes_up_m */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &tmp);
			bytes_up_m[i] = (tmp * 1024);

			fgets(line, 80, userfile); /* seconds_up_m */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &seconds_up_m[i]);

			fgets(line, 80, userfile); /* files_down_m */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &files_down_m[i]);

			fgets(line, 80, userfile); /* bytes_down_m */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &tmp);
			bytes_down_m[i] = (tmp * 1024);

			fgets(line, 80, userfile); /* seconds_down_m */
			p = strtok(line, " ");
			p = strtok(NULL, " ");
			sscanf(p, "%llu", &seconds_down_m[i]);
		}

		/* group top data collection */
		if (strstr(line, "# Groups for"))
		{
			fgets(line, 80, userfile); /* group sysop */
			fgets(line, 80, userfile); /* group's member? */
			GROUP_Computing();
		}

		fgets(line, 80, userfile);
	}

	return 0;
}

/* routines weekstart & monthstart clipped from Jonni's grouptop.c
 **************************************************************************
        WEEKSTART
        Returns the time_t value for Midnight of this previous Sunday.
 **************************************************************************
 */
time_t
weekstart( void )
{
	struct tm *timebuf;     /* "struct tm" time buffer */
	time_t curtime;         /* Current time */
	time_t midnight;        /* Time at midnight */
	int offset;             /* # of days since sunday */
	char daybuf[ 4 ];       /* Day of week expressed as a string */
	char      *days[7] =
	{
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};

	time( &curtime );                       /* Get current time */

	timebuf = localtime( &curtime );        /* Fill tm struct */

	timebuf->tm_hour = 0;                   /* Set tm struct to midnight */
	timebuf->tm_min = 0;
	timebuf->tm_sec = 0;

	midnight = mktime( timebuf );           /* Get time_t from tm struct */

	sprintf( daybuf, "%.3s", ctime( &midnight ) ); /* Get day of week as string */

	/* Get days since Sunday */
	for ( offset = 0; strcmp( days[ offset ], daybuf ) && offset < 7; offset++ ) ;

	return ( midnight - ( offset * 86400 ) );
}
/* -- end of weekstart() -------------------------------------------------- */

/*
 *************************************************************************
        Returns the time_t value for Midnight + 1 min of 1st of month.
 ***************************************************************************
 */
time_t
monthstart( void )
{
	struct tm *timebuf;     /* "struct tm" time buffer */
	time_t curtime;         /* Current time */
	time_t midnight;        /* Time at midnight */
	int offset;             /* # of days since sunday */
	char daybuf[ 11 ];      /* Day of week expressed as a string */
	char      *days[32] =
	{
		" 1", " 2", " 3", " 4", " 5", " 6", " 7",
		" 8", " 9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
		"20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32"
	};

/*#ifdef DEBUG
        syslog( LOG_INFO, "[gftpd] monthstart()" );
        debugfn();
 #endif  */

	time( &curtime );                       /* Get current time */
	timebuf = localtime( &curtime );        /* Fill tm struct */
	timebuf->tm_hour = 0;                   /* Set tm struct to midnight */
	timebuf->tm_min = 1;
	timebuf->tm_sec = 0;
	midnight = mktime( timebuf );           /* Get time_t from tm struct */

	sprintf( daybuf, "%.10s", ctime( &midnight ) ); /* Get day of week as string */
	/* put the # to daybuf */
	daybuf[0] = daybuf[8];
	daybuf[1] = daybuf[9];
	daybuf[2] = '\0';

	/* Get days since Sunday */
	for ( offset = 0; strcmp( days[ offset ], daybuf ) && offset < 31; offset++ ) ;

	return ( midnight - ( offset * 86400 ) );
}
/* -- end of monthstart() -------------------------------------------------- */

/* SITE ALUP */
int Tops_All_Up()
{
	sprintf(buf, "%stops.alltime.up", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/tops.example.allup", "rt");
	if (!outlook)
	{
		printf("error at example/tops.example.allup\n");
		exit(1);
	}

	middle = fopen("example/tops.example.allup.middle", "rt");
	if (!middle)
	{
		printf("error at example/tops.example.allup.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

	PMC_Sort(bytes_up, userbyte_pos, maxuser);

	/* if alup_unum is somehow fucked up, more than we have users */
	if (maxuser < alup_unum)
		alup_unum = 10;

	fputs("******  UPLOADS  ******\n", php);

	i = 0;
	while (i <= alup_unum)
	{
		for (x = 1; x <= maxuser; x++)
			if (userbyte_pos[x] == i)
			{
				if (bytes_up[x] == 0)
					break;
				sprintf(tempstr, "%llu", bytes_up[x]);
				strcpy(tempstr, f_format(tempstr));
				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", username[x]);
				pmcparse(line, '@', "U_LOCAT", tagline[x]);
				pmcparse(line, '@', "U_BYTES", tempstr);
				fputs(tempstr2, php); /* php database */
				fputs("\n", php);
				fputs(username[x], php);
				fputs("\n", php);
				fputs(tagline[x], php);
				fputs("\n", php);
				sprintf(tempstr2, "%llu", files_up[x]);
				pmcparse(line, '@', "U_FILES", tempstr2);
				sprintf(tempstr3, "%llu", (seconds_up[x] / 8 / 1000));
				pmcparse(line, '@', "U_KBSEC", tempstr3);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);

				fputs(line, bulletin);
			}
		i++;
	}

	sprintf(tempstr, "%llu", all_up);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%llu", all_dn);
	strcpy(tempstr2, f_format(tempstr2));
	fputs(tempstr, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);

	/* lets read the "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "UP", tempstr);
		pmcparse(parsebuf, '@', "LEECH", tempstr2);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* SITE ALDN */
int Tops_All_Dn()
{
	sprintf(buf, "%stops.alltime.down", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/tops.example.alldn", "rt");
	if (!outlook)
	{
		printf("error at example/tops.example.alldn\n");
		exit(1);
	}

	middle = fopen("example/tops.example.alldn.middle", "rt");
	if (!middle)
	{
		printf("error at example/tops.example.alldn.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

	PMC_Sort(bytes_down, userbyte_pos, maxuser);

	/* if aldn_unum is somehow fucked up, more than we have users */
	if (maxuser < aldn_unum)
		aldn_unum = 10;

	fputs("******  DOWNLOADS  ******\n", php);

	i = 0;
	while (i <= aldn_unum)
	{
		for (x = 1; x <= maxuser; x++)
			if (userbyte_pos[x] == i)
			{
				if (bytes_down[x] == 0)
					break;
				sprintf(tempstr, "%llu", bytes_down[x]);
				strcpy(tempstr, f_format(tempstr));

				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", username[x]);
				pmcparse(line, '@', "U_LOCAT", tagline[x]);
				pmcparse(line, '@', "D_BYTES", tempstr);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(username[x], php);
				fputs("\n", php);
				fputs(tagline[x], php);
				fputs("\n", php);
				sprintf(tempstr2, "%llu", files_down[x]);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);
				pmcparse(line, '@', "D_FILES", tempstr2);
				sprintf(tempstr3, "%llu", (seconds_down[x] / 8 / 1000));
				pmcparse(line, '@', "D_KBSEC", tempstr3);

				fputs(line, bulletin);
			}
		i++;
	}

	sprintf(tempstr, "%llu", all_up);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%llu", all_dn);
	strcpy(tempstr2, f_format(tempstr2));
	fputs(tempstr, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);

	/* lets read the ALDN "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "UP", tempstr);
		pmcparse(parsebuf, '@', "LEECH", tempstr2);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* SITE WKUP */
int Tops_Wkup()
{
	int ph1 = 0;
	int phcx = 0;

	sprintf(buf, "%stops.weekly.up", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/tops.example.wkup", "rt");
	if (!outlook)
	{
		printf("error at example/tops.example.wkup\n");
		exit(1);
	}

	middle = fopen("example/tops.example.wkup.middle", "rt");
	if (!middle)
	{
		printf("error at example/tops.example.wkup.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

	PMC_Sort(bytes_up_wk, userbyte_pos, maxuser);

	/* if wkup_unum is somehow fucked up, more than we have users */
	if (maxuser < wkup_unum)
		wkup_unum = 10;

	fputs("******  WEEKLY UPLOADS  ******\n", php);

	i = 0;
	while (i <= wkup_unum)
	{
		for (x = 1; x <= maxuser; x++)
			if (userbyte_pos[x] == i)
			{
				if (bytes_up_wk[x] < 1000000)
					break;
				sprintf(tempstr, "%llu", bytes_up_wk[x]);
				strcpy(tempstr, f_format(tempstr));

				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", username[x]);
				pmcparse(line, '@', "U_LOCAT", tagline[x]);
				pmcparse(line, '@', "WKUP_BYTES", tempstr);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(username[x], php);
				fputs("\n", php);
				fputs(tagline[x], php);
				fputs("\n", php);
				sprintf(tempstr2, "%llu", files_up_wk[x]);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);
				pmcparse(line, '@', "WKUP_FILES", tempstr2);
				sprintf(tempstr3, "%llu", (seconds_up_wk[x] / 8 / 1000));
				pmcparse(line, '@', "WKUP_KBSEC", tempstr3);
				ph1++;
				fputs(line, bulletin);
			}
		i++;
	}

	sprintf(tempstr, "%llu", all_wkup);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%llu", alltime_wkup_user);
	strcpy(tempstr2, f_format(tempstr2));
	sprintf(tempstr3, "%llu", alltime_wkup);
	strcpy(tempstr3, f_format(tempstr3));

	if (ph1 < 10)
	{
		phcx = 0;
		phcx = ph1;
		while (phcx < 10)
		{
			fputs("-pos-\n", php);
			fputs("-name-\n", php);
			fputs("-locat-\n", php);
			fputs("-files-\n", php);
			fputs("-bytes-\n", php);
			phcx++;
		}
	}

	fputs(tempstr, php);
	fputs("\n", php);
	fputs(allwkup_user, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);
	fputs(tempstr3, php);
	fputs("\n", php);

	/* lets read the WKUP "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "WKUP", tempstr);
		pmcparse(parsebuf, '@', "ALLWEEK", allwkup_user);
		pmcparse(parsebuf, '@', "USERWEEK", tempstr2);
		pmcparse(parsebuf, '@', "SCORE", tempstr3);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* SITE WKDN */
int Tops_Wkdn()
{
	int ph1 = 0, phcx = 0;

	sprintf(buf, "%stops.weekly.down", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/tops.example.wkdn", "rt");
	if (!outlook)
	{
		printf("error at example/tops.example.wkdn\n");
		exit(1);
	}

	middle = fopen("example/tops.example.wkdn.middle", "rt");
	if (!middle)
	{
		printf("error at example/tops.example.wkdn.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

	PMC_Sort(bytes_down_wk, userbyte_pos, maxuser);

	/* if wkdn_unum is somehow fucked up, more than we have users */
	if (maxuser < wkdn_unum)
		wkdn_unum = 10;

	fputs("******  WEEKLY DOWNLOADS  ******\n", php);

	i = 0;
	while (i <= wkdn_unum)
	{
		for (x = 1; x <= maxuser; x++)
			if (userbyte_pos[x] == i)
			{
				if (bytes_down_wk[x] == 0)
					break;
				sprintf(tempstr, "%llu", bytes_down_wk[x]);
				strcpy(tempstr, f_format(tempstr));

				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", username[x]);
				pmcparse(line, '@', "U_LOCAT", tagline[x]);
				pmcparse(line, '@', "WKDN_BYTES", tempstr);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(username[x], php);
				fputs("\n", php);
				fputs(tagline[x], php);
				fputs("\n", php);
				sprintf(tempstr2, "%llu", files_down_wk[x]);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);
				pmcparse(line, '@', "WKDN_FILES", tempstr2);
				sprintf(tempstr3, "%llu", (seconds_down_wk[x] / 8 / 1000));
				pmcparse(line, '@', "WKDN_KBSEC", tempstr3);
				ph1++;
				fputs(line, bulletin);
			}
		i++;
	}

	sprintf(tempstr, "%llu", all_wkdn);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%llu", alltime_wkdn_user);
	strcpy(tempstr2, f_format(tempstr2));
	sprintf(tempstr3, "%llu", alltime_wkdn);
	strcpy(tempstr3, f_format(tempstr3));

	if (ph1 < 10)
	{
		phcx = 0;
		phcx = ph1;
		while (phcx < 10)
		{
			fputs("-pos-\n", php);
			fputs("-name-\n", php);
			fputs("-locat-\n", php);
			fputs("-files-\n", php);
			fputs("-bytes-\n", php);
			phcx++;
		}
	}

	fputs(tempstr, php);
	fputs("\n", php);
	fputs(allwkdn_user, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);
	fputs(tempstr3, php);
	fputs("\n", php);

	/* lets read the WKDN "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "WKDN", tempstr);
		pmcparse(parsebuf, '@', "ALLWEEK", allwkdn_user);
		pmcparse(parsebuf, '@', "USERWEEK", tempstr2);
		pmcparse(parsebuf, '@', "SCORE", tempstr3);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* SITE MUP */
int Tops_mup()
{
	int ph1 = 0, phcx = 0;

	sprintf(buf, "%stops.monthly.up", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/tops.example.mup", "rt");
	if (!outlook)
	{
		printf("error at example/tops.example.mup\n");
		exit(1);
	}

	middle = fopen("example/tops.example.mup.middle", "rt");
	if (!middle)
	{
		printf("error at example/tops.example.mup.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

	PMC_Sort(bytes_up_m, userbyte_pos, maxuser);

	/* if mup_unum is somehow fucked up, more than we have users */
	if (maxuser < mup_unum)
		mup_unum = 10;

	fputs("******  MONTHLY UPLOADS  ******\n", php);

	i = 0;
	while (i <= mup_unum)
	{
		for (x = 1; x <= maxuser; x++)
			if (userbyte_pos[x] == i)
			{
				if (bytes_up_m[x] == 0)
					break;
				sprintf(tempstr, "%llu", bytes_up_m[x]);
				strcpy(tempstr, f_format(tempstr));

				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", username[x]);
				pmcparse(line, '@', "U_LOCAT", tagline[x]);
				pmcparse(line, '@', "MUP_BYTES", tempstr);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(username[x], php);
				fputs("\n", php);
				fputs(tagline[x], php);
				fputs("\n", php);
				sprintf(tempstr2, "%llu", files_up_m[x]);
				pmcparse(line, '@', "MUP_FILES", tempstr2);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);
				sprintf(tempstr3, "%llu", (seconds_up_m[x] / 8 / 1000));
				pmcparse(line, '@', "MUP_KBSEC", tempstr3);
				ph1++;
				fputs(line, bulletin);
			}
		i++;
	}

	sprintf(tempstr, "%llu", all_mup);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%llu", alltime_mup_user);
	strcpy(tempstr2, f_format(tempstr2));
	sprintf(tempstr3, "%llu", alltime_mup);
	strcpy(tempstr3, f_format(tempstr3));

	if (ph1 < 10)
	{
		phcx = 0;
		phcx = ph1;
		while (phcx < 10)
		{
			fputs("-pos-\n", php);
			fputs("-name-\n", php);
			fputs("-locat-\n", php);
			fputs("-files-\n", php);
			fputs("-bytes-\n", php);
			phcx++;
		}
	}

	fputs(tempstr, php);
	fputs("\n", php);
	fputs(allmup_user, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);
	fputs(tempstr3, php);
	fputs("\n", php);

	/* lets read the MUP "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "MUP", tempstr);
		pmcparse(parsebuf, '@', "ALLMUP", allmup_user);
		pmcparse(parsebuf, '@', "USERBYTES", tempstr2);
		pmcparse(parsebuf, '@', "SCORE", tempstr3);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* SITE MDN */
int Tops_mdn()
{
	int ph1 = 0, phcx = 0;

	sprintf(buf, "%stops.monthly.down", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/tops.example.mdn", "rt");
	if (!outlook)
	{
		printf("error at example/tops.example.mdn\n");
		exit(1);
	}

	middle = fopen("example/tops.example.mdn.middle", "rt");
	if (!middle)
	{
		printf("error at example/tops.example.mdn.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

	PMC_Sort(bytes_down_m, userbyte_pos, maxuser);

	/* if mdn_unum is somehow fucked up, more than we have users */
	if (maxuser < mdn_unum)
		mdn_unum = 10;

	fputs("******  MONTHLY DOWNLOADS  ******\n", php);

	i = 0;
	while (i <= mdn_unum)
	{
		for (x = 1; x <= maxuser; x++)
			if (userbyte_pos[x] == i)
			{
				if (bytes_down_m[x] == 0)
					break;
				sprintf(tempstr, "%llu", bytes_down_m[x]);
				strcpy(tempstr, f_format(tempstr));

				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", username[x]);
				pmcparse(line, '@', "U_LOCAT", tagline[x]);
				pmcparse(line, '@', "MDN_BYTES", tempstr);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(username[x], php);
				fputs("\n", php);
				fputs(tagline[x], php);
				fputs("\n", php);
				sprintf(tempstr2, "%llu", files_down_m[x]);
				pmcparse(line, '@', "MDN_FILES", tempstr2);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);
				sprintf(tempstr3, "%llu", (seconds_down_m[x] / 8 / 1000));
				pmcparse(line, '@', "MDN_KBSEC", tempstr3);
				ph1++;
				fputs(line, bulletin);
			}
		i++;
	}

	sprintf(tempstr, "%llu", all_mdn);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%llu", alltime_mdn_user);
	strcpy(tempstr2, f_format(tempstr2));
	sprintf(tempstr3, "%llu", alltime_mdn);
	strcpy(tempstr3, f_format(tempstr3));

	if (ph1 < 10)
	{
		phcx = 0;
		phcx = ph1;
		while (phcx < 10)
		{
			fputs("-pos-\n", php);
			fputs("-name-\n", php);
			fputs("-locat-\n", php);
			fputs("-files-\n", php);
			fputs("-bytes-\n", php);
			phcx++;
		}
	}

	fputs(tempstr, php);
	fputs("\n", php);
	fputs(allmdn_user, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);
	fputs(tempstr3, php);
	fputs("\n", php);

	/* lets read the MDN "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "MDN", tempstr);
		pmcparse(parsebuf, '@', "ALLMDN", allmdn_user);
		pmcparse(parsebuf, '@', "USERBYTES", tempstr2);
		pmcparse(parsebuf, '@', "SCORE", tempstr3);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* TOTALS */
int Totals()
{
	FILE *database;
	int recmaybe = 0;

	database = fopen("gftpdtool.dat", "rt");
	if (!database)
	{
		printf("error at gftpdtool.dat file!\n");
		exit(1);
	}

	fscanf(database, "%llu", &alltime_wkup); /* alltime wkup record */
	fscanf(database, "%llu", &alltime_wkdn); /* alltime wkdn record */
	fscanf(database, "%llu", &alltime_mup);  /* alltime mup record  */
	fscanf(database, "%llu", &alltime_mdn);  /* alltime mdn record  */

	fscanf(database, "%s", allwkup_user);         /* alltime wkup badass users name */
	fscanf(database, "%llu", &alltime_wkup_user); /* alltime wkup user record    */
	fscanf(database, "%s", allwkdn_user);         /* alltime wkdn worst users name */
	fscanf(database, "%llu", &alltime_wkdn_user); /* alltime wkdn user record    */
	fscanf(database, "%s", allmup_user);          /* alltime mup users name       */
	fscanf(database, "%llu", &alltime_mup_user);  /* alltime mup user record      */
	fscanf(database, "%s", allmdn_user);          /* alltime mdn worst users name */
	fscanf(database, "%llu", &alltime_mdn_user);  /* alltime mdn user record      */
	/* new groupshit */
	fscanf(database, "%s", allwkup_group);
	fscanf(database, "%llu", &allwkup_group_bytes);
	fscanf(database, "%s", allwkdn_group);
	fscanf(database, "%llu", &allwkdn_group_bytes);
	fscanf(database, "%s", allmup_group);
	fscanf(database, "%llu", &allmup_group_bytes);
	fscanf(database, "%s", allmdn_group);
	fscanf(database, "%llu", &allmdn_group_bytes);

	fclose(database); /* reading done, lets close the datafile */

	all_up = 0;
	all_dn = 0;
	all_wkup = 0;
	all_wkdn = 0;
	all_mup = 0;
	all_mdn = 0;
	all_rest = 0;
	all_w_rest = 0;

	i = 1;
	/* loop until we have checked all the users */
	while (i <= maxuser)
	{
		if ( last_on[i] <= weekstart() )
		{
			bytes_up_wk[i] = 0;
			bytes_down_wk[i] = 0;
			week_bad[i] = 0;
		}

		if ( last_on[i] <= monthstart() )
		{
			bytes_up_m[i] = 0;
			bytes_down_m[i] = 0;
		}

		all_up = (all_up + bytes_up[i]);          /* total uploads           */
		all_dn = (all_dn + bytes_down[i]);        /* total downloads         */
		all_wkup = (all_wkup + bytes_up_wk[i]);   /* weektop total uploads   */
		all_wkdn = (all_wkdn + bytes_down_wk[i]); /* weektop total downloads */
		all_mup = (all_mup + bytes_up_m[i]);      /* monthly total uploads   */
		all_mdn = (all_mdn + bytes_down_m[i]);    /* monthly total downloads */
		all_rest = (all_rest + all_bad[i]);       /* restart alltime         */
		all_w_rest = (all_w_rest + week_bad[i]);  /* restart weektop         */

		/* if we have single users new personal record */
		if (alltime_wkup_user < bytes_up_wk[i]) /* week UPLOAD user record */
		{
			alltime_wkup_user = bytes_up_wk[i];
			strcpy(allwkup_user, username[i]);
			recmaybe = 1;
		}

		if (alltime_wkdn_user < bytes_down_wk[i]) /* week DOWNLOAD user record */
		{
			alltime_wkdn_user = bytes_down_wk[i];
			strcpy(allwkdn_user, username[i]);
			recmaybe = 1;
		}

		if (alltime_mup_user < bytes_up_m[i]) /* Monthly UPLOAD user record */
		{
			alltime_mup_user = bytes_up_m[i];
			strcpy(allmup_user, username[i]);
			recmaybe = 1;
		}

		if (alltime_mdn_user < bytes_down_m[i]) /* Monthly DOWNLOAD user record */
		{
			alltime_mdn_user = bytes_down_m[i];
			strcpy(allmdn_user, username[i]);
			recmaybe = 1;
		}

		i++;
	}

	/* if we  have new highscore on site's total transfers (not single users) alltime wkup rec */
	if (alltime_wkup < all_wkup)
	{
		alltime_wkup = all_wkup;
		recmaybe = 1;
	}

	/* alltime wkdn rec */
	if (alltime_wkdn < all_wkdn)
	{
		alltime_wkdn = all_wkdn;
		recmaybe = 1;
	}

	/* alltime mup rec */
	if (alltime_mup < all_mup)
	{
		alltime_mup = all_mup;
		recmaybe = 1;
	}

	/* alltime mdn rec */
	if (alltime_mdn < all_mdn)
	{
		alltime_mdn = all_mdn;
		recmaybe = 1;
	}

	/* and now the GROUP totals */
	Group_Totals();

/*

   TIME/RECORD add feature: date for each highscore, when it happened

 */

	/* okay we have new record! lets save the stats */
	if (recmaybe == 1 || gp_rec == 1)
	{
		database = fopen("gftpdtool.dat", "wt");
		if (!database)
		{
			printf("error opening gftpdtool.dat !\n");
			exit(1);
		}

		fprintf(database, "%llu\n", alltime_wkup);
		fprintf(database, "%llu\n", alltime_wkdn);
		fprintf(database, "%llu\n", alltime_mup);
		fprintf(database, "%llu\n", alltime_mdn);

		fprintf(database, "%s\n", allwkup_user);
		fprintf(database, "%llu\n", alltime_wkup_user);
		fprintf(database, "%s\n", allwkdn_user);
		fprintf(database, "%llu\n", alltime_wkdn_user);

		fprintf(database, "%s\n", allmup_user);
		fprintf(database, "%llu\n", alltime_mup_user);
		fprintf(database, "%s\n", allmdn_user);
		fprintf(database, "%llu\n", alltime_mdn_user);
		/* group crap */
		fprintf(database, "%s\n", allwkup_group);
		fprintf(database, "%llu\n", allwkup_group_bytes);
		fprintf(database, "%s\n", allwkdn_group);
		fprintf(database, "%llu\n", allwkdn_group_bytes);
		fprintf(database, "%s\n", allmup_group);
		fprintf(database, "%llu\n", allmup_group_bytes);
		fprintf(database, "%s\n", allmdn_group);
		fprintf(database, "%llu\n\n", allmdn_group_bytes);

		strcpy(line, ";\n; #1 highscore site's weektop upload bytes\n");
		fputs(line, database);
		strcpy(line, "; #2 highscore site's weektop download bytes\n");
		fputs(line, database);
		strcpy(line, "; #3 highscore site's MUP bytes\n");
		fputs(line, database);
		strcpy(line, "; #4 highscore site's MDN bytes\n");
		fputs(line, database);
		strcpy(line, "; #5 highscore weekly best uploader\n");
		fputs(line, database);
		strcpy(line, "; #6 highscore weekly user ul bytes\n");
		fputs(line, database);
		strcpy(line, "; #7 highscore weekly worst leecher\n");
		fputs(line, database);
		strcpy(line, "; #8 highscore weekly user dl bytes\n");
		fputs(line, database);
		strcpy(line, "; #9 highscore monthly best uploader\n");
		fputs(line, database);
		strcpy(line, "; #10 highscore monthly user ul bytes\n");
		fputs(line, database);
		strcpy(line, "; #11 highscore monthly worst leecher\n");
		fputs(line, database);
		strcpy(line, "; #12 highscore monthly user dl bytes\n");
		fputs(line, database);
		strcpy(line, "; #13 highscore weekly UL group\n");
		fputs(line, database);
		strcpy(line, "; #14 highscore weekly UL groups bytes\n");
		fputs(line, database);
		strcpy(line, "; #15 highscore weekly DL group\n");
		fputs(line, database);
		strcpy(line, "; #16 highscore weekly DL groups bytes\n");
		fputs(line, database);
		strcpy(line, "; #17 highscore monthly UL group\n");
		fputs(line, database);
		strcpy(line, "; #18 highscore monthly UL groups bytes\n");
		fputs(line, database);
		strcpy(line, "; #19 highscore monthly DL group\n");
		fputs(line, database);
		strcpy(line, "; #20 highscore monthly DL groups bytes\n");
		fputs(line, database);

		fclose(database);
	}

	sprintf(total, "%llu", (all_up + all_dn));
	strcpy(total, f_format(total));

	return 0;
}

/* configuration file of gftpdtool piece of crappy routines */
int CFG_Read()
{
	FILE *cfg_file;
	char temp[80];

	/* lets read the how_many_users_to_list */
	cfg_file = fopen("gftpdtool.cfg", "rt");
	if (!cfg_file)
	{
		printf("error at gftpdtool.cfg!\n");
		exit(1);
	}

	fscanf(cfg_file, "%s", top_path); /* path to the top files */
	fscanf(cfg_file, "%s", php_path); /* path to the php file  */

	fscanf(cfg_file, "%llu", &alup_unum); /* max alup users */
	fscanf(cfg_file, "%llu", &aldn_unum); /* max aldn users */
	fscanf(cfg_file, "%llu", &wkup_unum); /* max wkup users */
	fscanf(cfg_file, "%llu", &wkdn_unum); /* max wkdn users */
	fscanf(cfg_file, "%llu", &mup_unum);  /* max mup users */
	fscanf(cfg_file, "%llu", &mdn_unum);  /* max mdn users */

	fclose(cfg_file);
	return 0;
}

/* to stript the path from userfiles (names actually) */
int splitpath(char **args, char *p) {
	int c = 0;

	while (*p == '/')
		p++;

	args[c++] = p;
	while (*p)
	{
		if (*p == '/')
		{
			*(p++) = 0;
			args[c++] = p;
			while(*p == '/')
				p++;
		}
		else
			p++;
	}
	args[c] = 0;

	return c - 1;
}

/* some stuff for todays date shit */
int Show_Me_Today()
{
	struct tm *tm;
	char Today_Date[30];

	time_t t;
	t = now;
	tm = localtime(&t);
	strcpy(Today_Date, ctime(&t));

	printf("todays date day: %i month: %i year: %i hour: %i min %i\n", tm->tm_mday, tm->tm_mon + 1, tm->tm_year, tm->tm_hour, tm->tm_min);
	printf("ctime of today: %s\n", Today_Date);

	return 0;
}

/* database to create struct week datas */
int DataBase_WEEK()
{
	DATabase poks;

	FILE *datas;

	printf("at database_WEEK() function, doing the weektops...");

	datas = fopen("database.dat", "ab");
	if (!datas)
	{
		printf("error at database.dat!\n");
		exit(1);
	}

	PMC_Sort(bytes_up_wk, userbyte_pos, maxuser);

	/* if wkup_unum is somehow fucked up, more than we have users */
	if (maxuser < wkup_unum)
		wkup_unum = 10;

	i = 0;
	while (i <= 10)
	{
		for (x = 1; x <= maxuser; x++)
			if (userbyte_pos[x] == i)
			{
				if (bytes_up_wk[x] == 0)
					break;

				if (i == 1)
				{
					strcpy(poks.user_name01, username[x]);
					strcpy(poks.location01, tagline[x]);
					poks.files01 = 0;
					poks.files01 = files_up_wk[x];
					poks.bytes01 = 0;
					poks.bytes01 = bytes_up_wk[x];
				}
				if (i == 2)
				{
					strcpy(poks.user_name02, username[x]);
					strcpy(poks.location02, tagline[x]);
					poks.files02 = 0;
					poks.files02 = files_up_wk[x];
					poks.bytes02 = 0;
					poks.bytes02 = bytes_up_wk[x];
				}
				if (i == 3)
				{
					strcpy(poks.user_name03, username[x]);
					strcpy(poks.location03, tagline[x]);
					poks.files03 = 0;
					poks.files03 = files_up_wk[x];
					poks.bytes03 = 0;
					poks.bytes03 = bytes_up_wk[x];
				}
				if (i == 4)
				{
					strcpy(poks.user_name04, username[x]);
					strcpy(poks.location04, tagline[x]);
					poks.files04 = 0;
					poks.files04 = files_up_wk[x];
					poks.bytes04 = 0;
					poks.bytes04 = bytes_up_wk[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name05, "");
						strcpy(poks.location05, "");
						poks.files05 = 0;
						poks.bytes05 = 0;
					}
				}
				if (i == 5)
				{
					strcpy(poks.user_name05, username[x]);
					strcpy(poks.location05, tagline[x]);
					poks.files05 = 0;
					poks.files05 = files_up_wk[x];
					poks.bytes05 = 0;
					poks.bytes05 = bytes_up_wk[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name05, "");
						strcpy(poks.location05, "");
						poks.files05 = 0;
						poks.bytes05 = 0;
					}
				}
				if (i == 6)
				{
					strcpy(poks.user_name06, username[x]);
					strcpy(poks.location06, tagline[x]);
					poks.files06 = 0;
					poks.files06 = files_up_wk[x];
					poks.bytes06 = 0;
					poks.bytes06 = bytes_up_wk[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name06, "");
						strcpy(poks.location06, "");
						poks.files06 = 0;
						poks.bytes06 = 0;
					}
				}
				if (i == 7)
				{
					strcpy(poks.user_name07, username[x]);
					strcpy(poks.location07, tagline[x]);
					poks.files07 = 0;
					poks.files07 = files_up_wk[x];
					poks.bytes07 = 0;
					poks.bytes07 = bytes_up_wk[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name07, "");
						strcpy(poks.location07, "");
						poks.files07 = 0;
						poks.bytes07 = 0;
					}
				}
				if (i == 8)
				{
					strcpy(poks.user_name08, username[x]);
					strcpy(poks.location08, tagline[x]);
					poks.files08 = 0;
					poks.files08 = files_up_wk[x];
					poks.bytes08 = 0;
					poks.bytes08 = bytes_up_wk[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name08, "");
						strcpy(poks.location08, "");
						poks.files08 = 0;
						poks.bytes08 = 0;
					}
				}
				if (i == 9)
				{
					strcpy(poks.user_name09, username[x]);
					strcpy(poks.location09, tagline[x]);
					poks.files09 = 0;
					poks.files09 = files_up_wk[x];
					poks.bytes09 = 0;
					poks.bytes09 = bytes_up_wk[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name09, "");
						strcpy(poks.location09, "");
						poks.files09 = 0;
						poks.bytes09 = 0;
					}
				}
				if (i == 10)
				{
					strcpy(poks.user_name10, username[x]);
					strcpy(poks.location10, tagline[x]);
					poks.files10 = 0;
					poks.files10 = files_up_wk[x];
					poks.bytes10 = 0;
					poks.bytes10 = bytes_up_wk[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name10, "");
						strcpy(poks.location10, "");
						poks.files10 = 0;
						poks.bytes10 = 0;
					}
				}
			}
		i++;
	}
	poks.date = time(0);                          /* date        */
	poks.what = 1;                                /* wk or mup   */
	poks.weekly_uploads = 0;
	poks.weekly_uploads = all_wkup;               /* wkup bytes  */
	poks.alltime = 0;
	poks.alltime = alltime_wkup;                  /* highscore   */
	strcpy(poks.best_alltime_user, allwkup_user); /* hs user     */
	poks.best_alltime_users_bytes = 0;
	poks.best_alltime_users_bytes = alltime_wkup_user; /* hs user_bytes */

	fwrite(&poks, sizeof(DATabase), 1, datas);

	fclose(datas);
	printf("DONE!\n");

	return 0;
}

/* database to create struct month datas */
int DataBase_MONTH()
{
	DATabase poks;

	FILE *datas;

	printf("at database_MONTH() function, doing monthtops...");

	datas = fopen("database.dat", "ab");
	if (!datas)
	{
		printf("error at database.dat!\n");
		exit(1);
	}

	PMC_Sort(bytes_up_m, userbyte_pos, maxuser);

	/* if mup_unum is somehow fucked up, more than we have users */
	if (maxuser < mup_unum)
		mup_unum = 10;

	i = 0;
	while (i <= 10)
	{
		for (x = 1; x <= maxuser; x++)
			if (userbyte_pos[x] == i)
			{
				if (bytes_up_m[x] == 0)
					break;

				if (i == 1)
				{
					strcpy(poks.user_name01, username[x]);
					strcpy(poks.location01, tagline[x]);
					poks.files01 = 0;
					poks.files01 = files_up_m[x];
					poks.bytes01 = 0;
					poks.bytes01 = bytes_up_m[x];
				}
				if (i == 2)
				{
					strcpy(poks.user_name02, username[x]);
					strcpy(poks.location02, tagline[x]);
					poks.files02 = 0;
					poks.files02 = files_up_m[x];
					poks.bytes02 = 0;
					poks.bytes02 = bytes_up_m[x];
				}
				if (i == 3)
				{
					strcpy(poks.user_name03, username[x]);
					strcpy(poks.location03, tagline[x]);
					poks.files03 = 0;
					poks.files03 = files_up_m[x];
					poks.bytes03 = 0;
					poks.bytes03 = bytes_up_m[x];
				}
				if (i == 4)
				{
					strcpy(poks.user_name04, username[x]);
					strcpy(poks.location04, tagline[x]);
					poks.files04 = 0;
					poks.files04 = files_up_m[x];
					poks.bytes04 = 0;
					poks.bytes04 = bytes_up_m[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name04, "");
						strcpy(poks.location04, "");
						poks.files04 = 0;
						poks.bytes04 = 0;
					}
				}
				if (i == 5)
				{
					strcpy(poks.user_name05, username[x]);
					strcpy(poks.location05, tagline[x]);
					poks.files05 = 0;
					poks.files05 = files_up_m[x];
					poks.bytes05 = 0;
					poks.bytes05 = bytes_up_m[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name05, "");
						strcpy(poks.location05, "");
						poks.files05 = 0;
						poks.bytes05 = 0;
					}
				}
				if (i == 6)
				{
					strcpy(poks.user_name06, username[x]);
					strcpy(poks.location06, tagline[x]);
					poks.files06 = 0;
					poks.files06 = files_up_m[x];
					poks.bytes06 = 0;
					poks.bytes06 = bytes_up_m[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name06, "");
						strcpy(poks.location06, "");
						poks.files06 = 0;
						poks.bytes06 = 0;
					}
				}
				if (i == 7)
				{
					strcpy(poks.user_name07, username[x]);
					strcpy(poks.location07, tagline[x]);
					poks.files07 = 0;
					poks.files07 = files_up_m[x];
					poks.bytes07 = 0;
					poks.bytes07 = bytes_up_m[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name07, "");
						strcpy(poks.location07, "");
						poks.files07 = 0;
						poks.bytes07 = 0;
					}
				}
				if (i == 8)
				{
					strcpy(poks.user_name08, username[x]);
					strcpy(poks.location08, tagline[x]);
					poks.files08 = 0;
					poks.files08 = files_up_m[x];
					poks.bytes08 = 0;
					poks.bytes08 = bytes_up_m[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name08, "");
						strcpy(poks.location08, "");
						poks.files08 = 0;
						poks.bytes08 = 0;
					}
				}
				if (i == 9)
				{
					strcpy(poks.user_name09, username[x]);
					strcpy(poks.location09, tagline[x]);
					poks.files09 = 0;
					poks.files09 = files_up_m[x];
					poks.bytes09 = 0;
					poks.bytes09 = bytes_up_m[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name09, "");
						strcpy(poks.location09, "");
						poks.files09 = 0;
						poks.bytes09 = 0;
					}
				}
				if (i == 10)
				{
					strcpy(poks.user_name10, username[x]);
					strcpy(poks.location10, tagline[x]);
					poks.files10 = 0;
					poks.files10 = files_up_m[x];
					poks.bytes10 = 0;
					poks.bytes10 = bytes_up_m[x];
					if (bytes_up_wk[x] == 0)
					{
						strcpy(poks.user_name10, "");
						strcpy(poks.location10, "");
						poks.files10 = 0;
						poks.bytes10 = 0;
					}
				}
			}
		i++;
	}
	poks.date = time(0);                          /* date        */
	poks.what = 1;                                /* wk or mup   */
	poks.weekly_uploads = 0;
	poks.weekly_uploads = all_wkup;               /* wkup bytes  */
	poks.alltime = 0;
	poks.alltime = alltime_mup;                  /* highscore   */
	strcpy(poks.best_alltime_user, allmup_user); /* hs user     */
	poks.best_alltime_users_bytes = 0;
	poks.best_alltime_users_bytes = alltime_mup_user; /* hs user_bytes */

	fwrite(&poks, sizeof(DATabase), 1, datas);

	fclose(datas);
	printf("DONE!\n");

	return 0;
}

/* initializing the yeartop here */
int Year_Top_Init()
{
	FILE *Yeartop_Data;
	int yx = 1;
	long ydate = time(0);

	typedef struct
	{
		char username[25];
		int files_uploaded;
		unsigned long long bytes_uploaded;
		int files_downloaded;
		unsigned long long bytes_downloaded;
	} YearTop;

	YearTop YDATA;

	printf("Generating YearTop [yeartop.dat].\n");

	Yeartop_Data = fopen("yeartop.dat", "ab");
	if (!Yeartop_Data)
	{
		printf("error at yeartop.dat!\n");
		exit(1);
	}

	fwrite(&ydate, sizeof(ydate), 1, Yeartop_Data);

	while (yx < maxuser)
	{
		strcpy(YDATA.username, username[yx]);
		YDATA.files_uploaded = 0;
		YDATA.files_uploaded = files_up[yx];
		YDATA.bytes_uploaded = 0;
		YDATA.bytes_uploaded = bytes_up[yx];
		YDATA.files_downloaded = 0;
		YDATA.files_downloaded = files_down[yx];
		YDATA.bytes_downloaded = 0;
		YDATA.bytes_downloaded = bytes_down[yx];

		fwrite(&YDATA, sizeof(YearTop), 1, Yeartop_Data);
		yx++;
	}

	fclose(Yeartop_Data);
	return 0;
}

/* lets cleanup/initilize the crap */
int Cleanup()
{
	i = 1;

	while (i < ASIZE)
	{
		bzero(username[i], sizeof(username[i]));
		bzero(tagline[i], sizeof(tagline[i]));
		userbyte_pos[i] = 0;

		files_up[i] = 0;
		bytes_up[i] = 0;
		seconds_up[i] = 0;
		files_down[i] = 0;
		bytes_down[i] = 0;
		seconds_down[i] = 0;
		files_up_wk[i] = 0;
		bytes_up_wk[i] = 0;
		seconds_up_wk[i] = 0;
		files_down_wk[i] = 0;
		bytes_down_wk[i] = 0;
		seconds_down_wk[i] = 0;
		files_up_m[i] = 0;
		bytes_up_m[i] = 0;
		seconds_up_m[i] = 0;
		files_down_m[i] = 0;
		bytes_down_m[i] = 0;
		seconds_down_m[i] = 0;

		login_times[i] = 0;
		acc_created[i] = 0;
		time_limit[i] = 0;
		time_on_today[i] = 0;
		last_nuked[i] = 0;
		speed_fuckups[i] = 0;
		week_bad[i] = 0;
		all_bad[i] = 0;
		bzero(added_by[i], sizeof(added_by[i]));
		bzero(comment[i], sizeof(comment[i]));
		last_on[i] = 0;

		i++;
	}

	all_up = 0;
	all_dn = 0;
	all_wkup = 0;
	all_wkdn = 0;
	alltime_wkup = 0;
	alltime_wkdn = 0;
	alltime_wkup_user = 0;
	alltime_wkdn_user = 0;
	alltime_mup_user = 0;
	alltime_mdn_user = 0;
	all_mup = 0;
	all_mdn = 0;
	alltime_mup = 0;
	alltime_mdn = 0;

	bzero(allwkup_user, sizeof(allwkup_user));
	bzero(allwkdn_user, sizeof(allwkdn_user));
	bzero(allmup_user, sizeof(allmup_user));
	bzero(allmdn_user, sizeof(allmdn_user));

	return 0;
}

/* read groupnames & descriptions from /.ftp-data/group.user file */
int Get_Groups()
{
	FILE *GPuser;
	char gtemp[80];
	i = 1;

	/* lets clear the group array */
	while (i < 40)
	{
		bzero(gp[i].name, sizeof(gp[i].name));
		bzero(gp[i].fullname, sizeof(gp[i].fullname));
		gp[i].num_members = 0;
		gp[i].alup_bytes = 0;
		gp[i].alup_files = 0;
		gp[i].aldn_bytes = 0;
		gp[i].aldn_files = 0;
		gp[i].wkup_bytes = 0;
		gp[i].wkup_files = 0;
		gp[i].wkdn_bytes = 0;
		gp[i].wkdn_files = 0;
		gp[i].mup_bytes = 0;
		gp[i].mup_files = 0;
		gp[i].mdn_bytes = 0;
		gp[i].mdn_files = 0;
		gp[i].bad_all = 0;
		gp[i].bad_week = 0;

		i++;
	}

	GPuser = fopen("/.ftp-data/groups.user", "rt");
	if (!GPuser)
	{
		printf("error at /.ftp-data/groups.user file!\n");
		exit(1);
	}

	fgets(line, 80, GPuser);
	fgets(line, 80, GPuser);
	fgets(line, 80, GPuser);
	fgets(line, 80, GPuser);
	fgets(line, 80, GPuser);
	i = 0;

	while (!feof(GPuser))
	{
		i++;
		p = strtok(line, " ");
/*    p=strtok(NULL," "); */
		strcpy(gp[i].name, p);

/*    if (strstr(line,"tagline"))
        {*/
/*	p=strtok(line," "); done */
		p = strtok(NULL, " ");
		strcpy(gtemp, "");
		while (p) /* do tokens until we dont have anything */
		{
			strcat(gtemp, p);      /* append the next   */
			strcat(gtemp, " ");    /* some space...     */
			p = strtok(NULL, " "); /* tokenize another  */
		}

		if (strlen(gtemp) > 30)
			gtemp[31] = '\0';

		strncpy(gp[i].fullname, gtemp, strlen(gtemp) - 2); /* removing \0 */

/*
   printf("run i: %i - line: %s - gp[i].name: %s|stops here|\n",i,line,gp[i].name);
 */
		fgets(line, 80, GPuser);
	}
	num_groups = i;
/*
   printf("num_groups: %i - i: %i\n",num_groups,i);
 */
	fclose(GPuser);
	return 0;
}

/* count the bytes for groups */
int GROUP_Computing()
{
	int gx = 1;
	char temp[80];
	char fish_point[80];
	char fish_group[80];
/*
   printf("whoole of gropname: %s[stops]\n",line);
 */
	p = strtok(line, " ");
	p = strtok(NULL, " ");

	bzero(temp, sizeof(temp));
	strncpy(temp, p, strlen(p) - 1);
/*
   printf("GROUP_Computing()  user: %s - grp: %s[stops_here]\n",username[i],temp);
 */

	while (gx <= num_groups)
	{
		strcpy(fish_point, temp);
		strcpy(fish_group, gp[gx].name);
/*
   strncpy(fish_point,p,strlen(p)-2);
   strncpy(fish_group,gp[gx].name,strlen(gp[gx].name)-2);
 */
/*
   printf("strstr searches gp[%i].name: %s vs p: %s\n",gx,fish_group,fish_point);
 */
		if (strstr(fish_group, fish_point))
		{
/*
   printf(" ===> user: %s is on group: %s, matching ==> %s\n [ updating the wkup & mup & alup etc ]\n",username[i],fish_group,fish_point);
 */                     gp[gx].num_members++;
			gp[gx].alup_bytes += bytes_up[i];
			gp[gx].alup_files += files_up[i];
			gp[gx].aldn_bytes += bytes_down[i];
			gp[gx].aldn_files += files_down[i];

			/* if user is not logged this week */
			if ( last_on[i] <= weekstart() )
			{
				bytes_up_wk[i] = 0;
				bytes_down_wk[i] = 0;
				files_up_wk[i] = 0;
				files_down_wk[i] = 0;
			}
			gp[gx].wkup_bytes += bytes_up_wk[i];
			gp[gx].wkup_files += files_up_wk[i];
			gp[gx].wkdn_bytes += bytes_down_wk[i];
			gp[gx].wkdn_files += files_down_wk[i];

			/* if user is not logged this month */
			if ( last_on[i] <= monthstart() )
			{
				bytes_up_m[i] = 0;
				bytes_down_m[i] = 0;
				files_up_m[i] = 0;
				files_down_m[i] = 0;
			}
			gp[gx].mup_bytes += bytes_up_m[i];
			gp[gx].mup_files += files_up_m[i];
			gp[gx].mdn_bytes += bytes_down_m[i];
			gp[gx].mdn_files += files_down_m[i];

			gp[gx].bad_all += all_bad[i];
			gp[gx].bad_week += week_bad[i];
			break;
		}
		gx++;
	}

/* strncpy( usergroup[x], tokptr, sizeof( usergroup[x] ) );*/
	return 0;
}

showmegroupshit(void)
{
	int groupcrap = 1;

	while (groupcrap <= num_groups)
	{
		printf("*****************************************************\n");
		printf("group: %s\n", gp[groupcrap].name);
		printf("fullname: %s\n", gp[groupcrap].fullname);
		printf("members: %i\n", gp[groupcrap].num_members);
		printf("alup_bytes: %llu\n", gp[groupcrap].alup_bytes);
		printf("alup_files: %lu\n", gp[groupcrap].alup_files);
		printf("aldn_bytes: %llu\n", gp[groupcrap].aldn_bytes);
		printf("aldn_files: %lu\n", gp[groupcrap].aldn_files);
		printf("wkup_bytes: %llu\n", gp[groupcrap].wkup_bytes);
		printf("wkup_files: %lu\n", gp[groupcrap].wkup_files);
		printf("wkdn_bytes: %llu\n", gp[groupcrap].wkdn_bytes);
		printf("wkdn_files: %lu\n", gp[groupcrap].wkdn_files);
		printf("mup_bytes: %llu\n", gp[groupcrap].mup_bytes);
		printf("mup_files: %lu\n", gp[groupcrap].mup_files);
		printf("mdn_bytes: %llu\n", gp[groupcrap].mdn_bytes);
		printf("mdn_files: %lu\n", gp[groupcrap].mdn_files);
		printf("bad_all: %lu\n", gp[groupcrap].bad_all);
		printf("bad_week: %lu\n", gp[groupcrap].bad_week);

		groupcrap++;
	}
}

/* highscore check for the groups */
int Group_Totals()
{
	int ux = 1;
	gp_rec = 0;

	while (ux <= num_groups)
	{
		if (gp[ux].wkup_bytes > allwkup_group_bytes)
		{
			gp_rec = 1;
			allwkup_group_bytes = gp[ux].wkup_bytes;
			strcpy(allwkup_group, gp[ux].name);
		}
		if (gp[ux].wkdn_bytes > allwkdn_group_bytes)
		{
			gp_rec = 1;
			allwkdn_group_bytes = gp[ux].wkdn_bytes;
			strcpy(allwkdn_group, gp[ux].name);
		}
		if (gp[ux].mup_bytes  > allmup_group_bytes)
		{
			gp_rec = 1;
			allmup_group_bytes = gp[ux].mup_bytes;
			strcpy(allmup_group, gp[ux].name);
		}
		if (gp[ux].mdn_bytes  > allmdn_group_bytes)
		{
			gp_rec = 1;
			allmdn_group_bytes = gp[ux].mdn_bytes;
			strcpy(allmdn_group, gp[ux].name);
		}
		ux++;
	}

	return 0;
}

/* weekly group uploads */
int G_Top_wkup()
{
	int Go_i = 1, Go_x = 1, G_pos[40];
	int ph1 = 0, phcx = 0;

	sprintf(buf, "%sgrouptops.weekly", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/GROUP.example.wkup", "rt");
	if (!outlook)
	{
		printf("error at example/GROUP.example.wkup\n");
		exit(1);
	}

	middle = fopen("example/GROUP.example.wkup.middle", "rt");
	if (!middle)
	{
		printf("error at example/GROUP.example.wkup.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

	for (Go_i = 1; Go_i <= num_groups; Go_i++)
	{
		G_pos[Go_i] = 1;

		for (Go_x = 1; Go_x <= num_groups; Go_x++)
		{
			if (gp[Go_i].wkup_bytes < gp[Go_x].wkup_bytes)
				G_pos[Go_i]++;
		}
	}

	fputs("******  GROUP WEEKLY UPLOADS  ******\n", php);

	i = 0;
	while (i <= 10)
	{
		for (x = 1; x <= num_groups; x++)
			if (G_pos[x] == i)
			{
				if (gp[x].wkup_bytes == 0)
					break;
				sprintf(tempstr, "%llu", gp[x].wkup_bytes);
				strcpy(tempstr, f_format(tempstr));

				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", gp[x].name);
				pmcparse(line, '@', "U_LOCAT", gp[x].fullname);
				pmcparse(line, '@', "WKUP_BYTES", tempstr);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(gp[x].name, php);
				fputs("\n", php);
				fputs(gp[x].fullname, php);
				fputs("\n", php);
				sprintf(tempstr2, "%lu", gp[x].wkup_files);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);
				pmcparse(line, '@', "WKUP_FILES", tempstr2);
				sprintf(tempstr3, "%llu", (seconds_up_wk[x] / 8 / 1000));
				pmcparse(line, '@', "WKUP_KBSEC", tempstr3);
				ph1++;
				fputs(line, bulletin);
			}
		i++;
	}
	sprintf(tempstr, "%llu", all_wkup);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%s", allwkup_group);
	strcpy(tempstr2, f_format(tempstr2));
	sprintf(tempstr3, "%llu", allwkup_group_bytes);
	strcpy(tempstr3, f_format(tempstr3));

	if (ph1 < 10)
	{
		phcx = 0;
		phcx = ph1;
		while (phcx < 10)
		{
			fputs("-pos-\n", php);
			fputs("-name-\n", php);
			fputs("-locat-\n", php);
			fputs("-files-\n", php);
			fputs("-bytes-\n", php);
			phcx++;
		}
	}

	fputs(tempstr, php);
	fputs("\n", php);
	fputs(allwkup_group, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);
	fputs(tempstr3, php);
	fputs("\n", php);

	/* lets read the WKUP "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "WKUP", tempstr);
		pmcparse(parsebuf, '@', "ALLWEEK", allwkup_group);
		pmcparse(parsebuf, '@', "USERWEEK", tempstr2);
		pmcparse(parsebuf, '@', "SCORE", tempstr3);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* weekly group downloads */
int G_Top_wkdn()
{
	int Go_i = 1, Go_x = 1, G_pos[40];
	int ph1 = 0, phcx = 0;

	sprintf(buf, "%sgrouptops.weekly.down", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/GROUP.example.wkdn", "rt");
	if (!outlook)
	{
		printf("error at example/GROUP.example.wkdn\n");
		exit(1);
	}

	middle = fopen("example/GROUP.example.wkdn.middle", "rt");
	if (!middle)
	{
		printf("error at example/GROUP.example.wkdn.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

/*	PMC_Sort(gp[num_groups].wkup_bytes, userbyte_pos, maxuser);
   sorting crap
 */
	for (Go_i = 1; Go_i <= num_groups; Go_i++)
	{
		G_pos[Go_i] = 1;

		for (Go_x = 1; Go_x <= num_groups; Go_x++)
		{
			if (gp[Go_i].wkdn_bytes < gp[Go_x].wkdn_bytes)
				G_pos[Go_i]++;
		}
	}

	fputs("******  GROUP WEEKLY DOWNLOADS  ******\n", php);

	i = 0;
	while (i <= 10)
	{
		for (x = 1; x <= num_groups; x++)
			if (G_pos[x] == i)
			{
				if (gp[x].wkdn_bytes == 0)
					break;
				sprintf(tempstr, "%llu", gp[x].wkdn_bytes);
				strcpy(tempstr, f_format(tempstr));

				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", gp[x].name);
				pmcparse(line, '@', "U_LOCAT", gp[x].fullname);
				pmcparse(line, '@', "WKDN_BYTES", tempstr);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(gp[x].name, php);
				fputs("\n", php);
				fputs(gp[x].fullname, php);
				fputs("\n", php);
				sprintf(tempstr2, "%lu", gp[x].wkdn_files);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);
				pmcparse(line, '@', "WKDN_FILES", tempstr2);
				sprintf(tempstr3, "%llu", (seconds_up_wk[x] / 8 / 1000));
				pmcparse(line, '@', "WKDN_KBSEC", tempstr3);
				ph1++;
				fputs(line, bulletin);
			}
		i++;
	}
	sprintf(tempstr, "%llu", all_wkdn);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%s", allwkdn_group);
	strcpy(tempstr2, f_format(tempstr2));
	sprintf(tempstr3, "%llu", allwkdn_group_bytes);
	strcpy(tempstr3, f_format(tempstr3));

	if (ph1 < 10)
	{
		phcx = 0;
		phcx = ph1;
		while (phcx < 10)
		{
			fputs("-pos-\n", php);
			fputs("-name-\n", php);
			fputs("-locat-\n", php);
			fputs("-files-\n", php);
			fputs("-bytes-\n", php);
			phcx++;
		}
	}

	fputs(tempstr, php);
	fputs("\n", php);
	fputs(allwkdn_group, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);
	fputs(tempstr3, php);
	fputs("\n", php);

	/* lets read the WKUP "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "WKDN", tempstr);
		pmcparse(parsebuf, '@', "ALLWEEK", allwkdn_group);
		pmcparse(parsebuf, '@', "USERWEEK", tempstr2);
		pmcparse(parsebuf, '@', "SCORE", tempstr3);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* monthly group uploads */
int G_Top_mup()
{
	int Go_i = 1, Go_x = 1, G_pos[40];
	int ph1 = 0, phcx = 0;

	sprintf(buf, "%sgrouptops.monthly", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/GROUP.example.mup", "rt");
	if (!outlook)
	{
		printf("error at example/GROUP.example.mup\n");
		exit(1);
	}

	middle = fopen("example/GROUP.example.mup.middle", "rt");
	if (!middle)
	{
		printf("error at example/GROUP.example.mup.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

	for (Go_i = 1; Go_i <= num_groups; Go_i++)
	{
		G_pos[Go_i] = 1;

		for (Go_x = 1; Go_x <= num_groups; Go_x++)
		{
			if (gp[Go_i].mup_bytes < gp[Go_x].mup_bytes)
				G_pos[Go_i]++;
		}
	}

	fputs("******  GROUP MONTHLY UPLOADS  ******\n", php);

	i = 0;
	while (i <= 10)
	{
		for (x = 1; x <= num_groups; x++)
			if (G_pos[x] == i)
			{
				if (gp[x].mup_bytes == 0)
					break;
				sprintf(tempstr, "%llu", gp[x].mup_bytes);
				strcpy(tempstr, f_format(tempstr));

				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", gp[x].name);
				pmcparse(line, '@', "U_LOCAT", gp[x].fullname);
				pmcparse(line, '@', "WKUP_BYTES", tempstr);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(gp[x].name, php);
				fputs("\n", php);
				fputs(gp[x].fullname, php);
				fputs("\n", php);
				sprintf(tempstr2, "%lu", gp[x].mup_files);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);
				pmcparse(line, '@', "WKUP_FILES", tempstr2);
				sprintf(tempstr3, "%llu", (seconds_up_wk[x] / 8 / 1000));
				pmcparse(line, '@', "WKUP_KBSEC", tempstr3);
				ph1++;
				fputs(line, bulletin);
			}
		i++;
	}
	sprintf(tempstr, "%llu", all_mup);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%s", allmup_group);
	strcpy(tempstr2, f_format(tempstr2));
	sprintf(tempstr3, "%llu", allmup_group_bytes);
	strcpy(tempstr3, f_format(tempstr3));

	if (ph1 < 10)
	{
		phcx = 0;
		phcx = ph1;
		while (phcx < 10)
		{
			fputs("-pos-\n", php);
			fputs("-name-\n", php);
			fputs("-locat-\n", php);
			fputs("-files-\n", php);
			fputs("-bytes-\n", php);
			phcx++;
		}
	}

	fputs(tempstr, php);
	fputs("\n", php);
	fputs(allmup_group, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);
	fputs(tempstr3, php);
	fputs("\n", php);

	/* lets read the WKUP "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "WKUP", tempstr);
		pmcparse(parsebuf, '@', "ALLWEEK", allmup_group);
		pmcparse(parsebuf, '@', "USERWEEK", tempstr2);
		pmcparse(parsebuf, '@', "SCORE", tempstr3);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* monthly group downloads */
int G_Top_mdn()
{
	int Go_i = 1, Go_x = 1, G_pos[40];
	int ph1 = 0, phcx = 0;

	sprintf(buf, "%sgrouptops.monthly.down", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/GROUP.example.mdn", "rt");
	if (!outlook)
	{
		printf("error at example/GROUP.example.mdn\n");
		exit(1);
	}

	middle = fopen("example/GROUP.example.mdn.middle", "rt");
	if (!middle)
	{
		printf("error at example/GROUP.example.mdn.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

/*	PMC_Sort(gp[num_groups].wkup_bytes, userbyte_pos, maxuser);
   sorting crap
 */
	for (Go_i = 1; Go_i <= num_groups; Go_i++)
	{
		G_pos[Go_i] = 1;

		for (Go_x = 1; Go_x <= num_groups; Go_x++)
		{
			if (gp[Go_i].mdn_bytes < gp[Go_x].mdn_bytes)
				G_pos[Go_i]++;
		}
	}

	fputs("******  GROUP MONTHLY DOWNLOADS  ******\n", php);

	i = 0;
	while (i <= 10)
	{
		for (x = 1; x <= num_groups; x++)
			if (G_pos[x] == i)
			{
				if (gp[x].mdn_bytes == 0)
					break;
				sprintf(tempstr, "%llu", gp[x].mdn_bytes);
				strcpy(tempstr, f_format(tempstr));

				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", gp[x].name);
				pmcparse(line, '@', "U_LOCAT", gp[x].fullname);
				pmcparse(line, '@', "WKUP_BYTES", tempstr);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(gp[x].name, php);
				fputs("\n", php);
				fputs(gp[x].fullname, php);
				fputs("\n", php);
				sprintf(tempstr2, "%lu", gp[x].mdn_files);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);
				pmcparse(line, '@', "WKUP_FILES", tempstr2);
				sprintf(tempstr3, "%llu", (seconds_up_wk[x] / 8 / 1000));
				pmcparse(line, '@', "WKUP_KBSEC", tempstr3);
				ph1++;
				fputs(line, bulletin);
			}
		i++;
	}
	sprintf(tempstr, "%llu", all_mdn);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%s", allmdn_group);
	strcpy(tempstr2, f_format(tempstr2));
	sprintf(tempstr3, "%llu", allmdn_group_bytes);
	strcpy(tempstr3, f_format(tempstr3));

	if (ph1 < 10)
	{
		phcx = 0;
		phcx = ph1;
		while (phcx < 10)
		{
			fputs("-pos-\n", php);
			fputs("-name-\n", php);
			fputs("-locat-\n", php);
			fputs("-files-\n", php);
			fputs("-bytes-\n", php);
			phcx++;
		}
	}

	fputs(tempstr, php);
	fputs("\n", php);
	fputs(allmdn_group, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);
	fputs(tempstr3, php);
	fputs("\n", php);

	/* lets read the WKUP "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "WKUP", tempstr);
		pmcparse(parsebuf, '@', "ALLWEEK", allmdn_group);
		pmcparse(parsebuf, '@', "USERWEEK", tempstr2);
		pmcparse(parsebuf, '@', "SCORE", tempstr3);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* group alltime uploads */
int G_Top_alup()
{
	int Go_i = 1, Go_x = 1, G_pos[40];
	int ph1 = 0, phcx = 0;

	sprintf(buf, "%sgrouptops.alltime", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/GROUP.example.alltime", "rt");
	if (!outlook)
	{
		printf("error at example/GROUP.example.alltime\n");
		exit(1);
	}

	middle = fopen("example/GROUP.example.alltime.middle", "rt");
	if (!middle)
	{
		printf("error at example/GROUP.example.alltime.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

	for (Go_i = 1; Go_i <= num_groups; Go_i++)
	{
		G_pos[Go_i] = 1;

		for (Go_x = 1; Go_x <= num_groups; Go_x++)
		{
			if (gp[Go_i].alup_bytes < gp[Go_x].alup_bytes)
				G_pos[Go_i]++;
		}
	}

	fputs("******  GROUP ALL UPLOADS  ******\n", php);

	i = 0;
	while (i <= 10)
	{
		for (x = 1; x <= num_groups; x++)
			if (G_pos[x] == i)
			{
				if (gp[x].alup_bytes == 0)
					break;
				sprintf(tempstr, "%llu", gp[x].alup_bytes);
				strcpy(tempstr, f_format(tempstr));
				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", gp[x].name);
				pmcparse(line, '@', "U_LOCAT", gp[x].fullname);
				pmcparse(line, '@', "U_BYTES", tempstr);
				fputs(tempstr2, php); /* php database */
				fputs("\n", php);
				fputs(gp[x].name, php);
				fputs("\n", php);
				fputs(gp[x].fullname, php);
				fputs("\n", php);
				sprintf(tempstr2, "%lu", gp[x].alup_files);
				pmcparse(line, '@', "U_FILES", tempstr2);
				sprintf(tempstr3, "%llu", (seconds_up[x] / 8 / 1000));
				pmcparse(line, '@', "U_KBSEC", tempstr3);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);
				ph1++;
				fputs(line, bulletin);
			}
		i++;
	}

	sprintf(tempstr, "%llu", all_up);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%llu", all_dn);
	strcpy(tempstr2, f_format(tempstr2));

	if (ph1 < 10)
	{
		phcx = 0;
		phcx = ph1;
		while (phcx < 10)
		{
			fputs("-pos-\n", php);
			fputs("-name-\n", php);
			fputs("-locat-\n", php);
			fputs("-files-\n", php);
			fputs("-bytes-\n", php);
			phcx++;
		}
	}

	fputs(tempstr, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);

	/* lets read the "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "UP", tempstr);
		pmcparse(parsebuf, '@', "LEECH", tempstr2);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* group alltime leeches */
int G_Top_aldn()
{
	int Go_i = 1, Go_x = 1, G_pos[40];
	int ph1 = 0, phcx = 0;

	sprintf(buf, "%sgrouptops.alltime.down", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/GROUP.example.alltime", "rt");
	if (!outlook)
	{
		printf("error at example/GROUP.example.alltime\n");
		exit(1);
	}

	middle = fopen("example/GROUP.example.alltime.middle", "rt");
	if (!middle)
	{
		printf("error at example/GROUP.example.alltime.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

	for (Go_i = 1; Go_i <= num_groups; Go_i++)
	{
		G_pos[Go_i] = 1;

		for (Go_x = 1; Go_x <= num_groups; Go_x++)
		{
			if (gp[Go_i].aldn_bytes < gp[Go_x].aldn_bytes)
				G_pos[Go_i]++;
		}
	}

	fputs("******  GROUP ALL DOWNLOADS  ******\n", php);

	i = 0;
	while (i <= 10)
	{
		for (x = 1; x <= num_groups; x++)
			if (G_pos[x] == i)
			{
				if (gp[x].aldn_bytes == 0)
					break;
				sprintf(tempstr, "%llu", gp[x].aldn_bytes);
				strcpy(tempstr, f_format(tempstr));
				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", gp[x].name);
				pmcparse(line, '@', "U_LOCAT", gp[x].fullname);
				pmcparse(line, '@', "U_BYTES", tempstr);
				fputs(tempstr2, php); /* php database */
				fputs("\n", php);
				fputs(gp[x].name, php);
				fputs("\n", php);
				fputs(gp[x].fullname, php);
				fputs("\n", php);
				sprintf(tempstr2, "%lu", gp[x].aldn_files);
				pmcparse(line, '@', "U_FILES", tempstr2);
				sprintf(tempstr3, "%llu", (seconds_up[x] / 8 / 1000));
				pmcparse(line, '@', "U_KBSEC", tempstr3);
				fputs(tempstr2, php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);
				ph1++;
				fputs(line, bulletin);
			}
		i++;
	}

	sprintf(tempstr, "%llu", all_up);
	strcpy(tempstr, f_format(tempstr));
	sprintf(tempstr2, "%llu", all_dn);
	strcpy(tempstr2, f_format(tempstr2));

	if (ph1 < 10)
	{
		phcx = 0;
		phcx = ph1;
		while (phcx < 10)
		{
			fputs("-pos-\n", php);
			fputs("-name-\n", php);
			fputs("-locat-\n", php);
			fputs("-files-\n", php);
			fputs("-bytes-\n", php);
			phcx++;
		}
	}

	fputs(tempstr, php);
	fputs("\n", php);
	fputs(tempstr2, php);
	fputs("\n", php);

	/* lets read the "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "UP", tempstr);
		pmcparse(parsebuf, '@', "LEECH", tempstr2);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	/* write overall total bytes to php database */
	fputs("******  SITE OVERALL TRANSFERS  ******\n", php);
	fputs(total, php);
	fputs("\n", php);

	return 0;
}

int Tops_Rest()
{
	sprintf(buf, "%stops.alltime.restart", top_path);
	bulletin = fopen(buf, "wt");
	if (!bulletin)
	{
		printf("error at %s!\n", buf);
		exit(1);
	}

	outlook = fopen("example/tops.example.alltime.restart", "rt");
	if (!outlook)
	{
		printf("error at example/tops.example.alltime.restart\n");
		exit(1);
	}

	middle = fopen("example/tops.example.alltime.restart.middle", "rt");
	if (!middle)
	{
		printf("error at example/tops.example.alltime.restart.middle\n");
		exit(1);
	}

	fgets(middle_str, 100, middle);
	fclose(middle);

	PMC_Sort(all_bad, userbyte_pos, maxuser);

	fputs("******  ALLTIME RESTARTS  ******\n", php);

	i = 0;
	while (i <= 10)
	{
		for (x = 1; x <= maxuser; x++)
			if (userbyte_pos[x] == i)
			{
				if (all_bad[x] == 0)
					break;
				sprintf(tempstr, "%llu", all_bad[x]);
				strcpy(tempstr, f_format(tempstr));
				strcpy(line, middle_str);
				sprintf(tempstr2, "%i", i);
				pmcparse(line, '@', "U_POS", tempstr2);
				pmcparse(line, '@', "U_NAME", username[x]);
				pmcparse(line, '@', "U_LOCAT", tagline[x]);
				pmcparse(line, '@', "U_BYTES", tempstr);
				fputs(tempstr2, php); /* php database */
				fputs("\n", php);
				fputs(username[x], php);
				fputs("\n", php);
				fputs(tagline[x], php);
				fputs("\n", php);
				fputs(tempstr, php);
				fputs("\n", php);

				fputs(line, bulletin);
			}
		i++;
	}

	sprintf(tempstr, "%llu", all_rest);
	strcpy(tempstr, f_format(tempstr));
	fputs(tempstr, php);
	fputs("\n", php);

	/* lets read the "footer" until we have end of footer... */
	fgets(parsebuf, 200, outlook);
	while (!feof(outlook))
	{
		pmcparse(parsebuf, '@', "UP", tempstr);
		pmcparse(parsebuf, '@', "VERSION", Version);
		fputs(parsebuf, bulletin);
		fgets(parsebuf, 200, outlook);
	}

	fclose(bulletin);
	fclose(outlook);

	return 0;
}

/* the bitch */
int main(int argv, char *argc[])

{
	now = time(0); /* todays time */
	printf("start, before header\n");
	Header();
	printf("header(); done\n");
	CFG_Read(); /* read datas from gftpdtool.cfg */
	printf("cfg_read done\n");
	Cleanup();
	printf("cleanup done\n");
	Get_Groups(); /* read groups from group.user */
	printf("get_groups done\n");
	php = fopen(php_path, "wt");
	if (!php)
	{
		printf("error at %s !\n", php_path);
		exit(1);
	}
	printf("opened php_path which is kind of %s\n", php_path);
	glob("/.ftp-data/users/*", 0, 0, &gl); /* what files to select from the dir? */
	printf("glob start done\n");
	File_Select();
	printf("file_select() done\n");
	Totals();  /* count total bytes    */
	printf("totals done\n");
	Tops_All_Up(); /* site all uploads     */
	printf("tops_all_up() done\n");
	Tops_All_Dn(); /* site all downloads   */
	printf("tops all_dn done\n");
	Tops_Wkup(); /* site weekly uploads  */
	printf("tops wkup() done\n");
	Tops_Wkdn(); /* site weekly leech    */
	printf("tops_wkdn() tone\n");
	Tops_mup();   /* site monthly uploads */
	Tops_mdn();   /* site monthly leech   */
	G_Top_wkup(); /* weekly group uploads */
	G_Top_wkdn(); /* weekly group leeches */
	G_Top_mup();  /* monthly group uploads*/
	G_Top_mdn();  /* monthly group leeches*/
	G_Top_alup(); /* group alltime uploads*/
	G_Top_aldn(); /* group alltime leeches*/
	Tops_Rest();  /* site restart top     */

	/* new database shit */
	if (argv == 2)
	{
		strcpy(line, argc[1]); /* get possible parameter */

		if (strstr(line, "/yeartop"))
			Year_Top_Init();
		if (strstr(line, "/wkuptop"))
			DataBase_WEEK();
		if (strstr(line, "/muptop"))
			DataBase_MONTH();
	}

/*    Show_Me_Today();
    showmegroupshit();*/

	globfree(&gl);
	fclose(php);
	return 0;
}
