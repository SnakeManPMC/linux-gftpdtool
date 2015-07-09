/*

    gftpdtool week/month-top database 2000.

   struct
        date ....................... (long)
        what ....................... (int 1=week 2=month)
        username ................... (char 25)
        location ................... (char 25)
        files ...................... (int)
        bytes ...................... (unsigned long long)
        weekly_uploads ............. (unsigned long long)
        alltime .................... (unsigned long long)
        best_alltime_user .......... (unsigned long long)
        best_alltime_users_bytes ... (unsigned long long)

 */

typedef struct
{
	long date;
	int what;
	char user_name01[25];
	char location01[25];
	int files01;
	unsigned long long bytes01;
	char user_name02[25];
	char location02[25];
	int files02;
	unsigned long long bytes02;
	char user_name03[25];
	char location03[25];
	int files03;
	unsigned long long bytes03;
	char user_name04[25];
	char location04[25];
	int files04;
	unsigned long long bytes04;
	char user_name05[25];
	char location05[25];
	int files05;
	unsigned long long bytes05;
	char user_name06[25];
	char location06[25];
	int files06;
	unsigned long long bytes06;
	char user_name07[25];
	char location07[25];
	int files07;
	unsigned long long bytes07;
	char user_name08[25];
	char location08[25];
	int files08;
	unsigned long long bytes08;
	char user_name09[25];
	char location09[25];
	int files09;
	unsigned long long bytes09;
	char user_name10[25];
	char location10[25];
	int files10;
	unsigned long long bytes10;

	unsigned long long weekly_uploads;
	unsigned long long alltime;
	char best_alltime_user[25];
	unsigned long long best_alltime_users_bytes;
} DATabase;

/*

    gftpdtool grouptop records

   struct
        name ....................... (char)
        fullnme .................... (char)
        num_members ................ (int)
        alup_bytes ................. (unsigned long long)
        alup_files ................. (long)
        aldn_bytes ................. (unsigned long long)
        aldn_files ................. (long)
        wkup_bytes ................. (unsigned long long)
        wkup_files ................. (long)
        wkdn_bytes ................. (unsigned long long)
        wkdn_files ................. (long)
        mup_bytes .................. (unsigned long long)
        mup_files .................. (long)
        mdn_bytes .................. (unsigned long long)
        mdn_files .................. (long)
        bad_all .................... (long)
        bad_week ................... (long)

 */
/* grouptop shiet */
typedef struct
{
	char name[14];                 /*  group name               */
	char fullname[30];             /*  full name of the group   */
	int num_members;               /*  number of members        */
	unsigned long long alup_bytes; /*  alltime bytes uploaded   */
	long alup_files;               /*  alltime files uploaded   */
	unsigned long long aldn_bytes; /*  alltime bytes downloaded */
	long aldn_files;               /*  alltime files downloaded */
	unsigned long long wkup_bytes; /*  weekly uploaded bytes    */
	long wkup_files;               /*  weekly uploaded files    */
	unsigned long long wkdn_bytes; /*  weekly downloaded bytes  */
	long wkdn_files;               /*  weekly downloaded files  */
	unsigned long long mup_bytes;  /*  monthly uploaded bytes   */
	long mup_files;                /*  monthly uploaded files   */
	unsigned long long mdn_bytes;  /*  monthly downloaded bytes */
	long mdn_files;                /*  monthly downloaded files */
	long bad_all;                  /*  alltime bad files        */
	long bad_week;                 /*  weekly bad files         */
} group[120];                          /*  120 groups available ;)  */

group gp;

/* end of gftpdtool headerfile */