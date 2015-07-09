#include <stdio.h>
#include <string.h>


FILE *Top_File;
FILE *Output;

char line[100];
char buf[200];


Alltime_Up()
{
char *ptr;
int i=1;

    Top_File=fopen("tops.alltime.up","rt");
    if (!Top_File) {printf("error at tops.alltime.up\n"); exit(1);}

    Output=fopen("dophp.dat","wt");
    if (!Output) {printf("error at dophp.dat\n"); exit(1);}


    fgets(line,100,Top_File);


while (i < 10) {
    
	ptr=strtok(line," ");
	ptr=strtok(NULL," "); 
	while (ptr) /* do tokens until we dont have anything */
	    {
	    strcat(buf,ptr); 
	    strcat(buf," "); 
	    ptr=strtok(NULL," ");
	    }
    i++;
}    
/*
!e|!H  1) flower      PMC -justpimpin! ;[             10982 156.545.152.000 !e|!0
*/
printf("buf %s\n",buf);    
    fclose(Top_File);
    fclose(Output);
}


int main()
{

    Alltime_Up();

    return 0;
}    

/*
        sprintf(tempstr,"%llu",all_up);
        strcpy(tempstr,f_format(tempstr));
*/