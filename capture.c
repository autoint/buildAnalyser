#include "windows.h"
#include "stdio.h"
#include "string.h"

/* capture the command line to a log file and then give the command to the real compiler */
/* note real world constraints here, very often the build system knows the absolute path to the compiler so our substitute MUST go there.
And the compiler's access to its standard headers could be based on a relative path from the binary directory so the real compiler probably needs to be in the same directory
*/

#define MAXCOMMANDLINE 0x10000
unsigned char commandline [MAXCOMMANDLINE] ;
unsigned char newcommandline [MAXCOMMANDLINE] ;
unsigned char programname [MAXCOMMANDLINE] ;
unsigned char currentdirectory [MAXCOMMANDLINE] ;
unsigned char timestring [100];

/*
const char name_of_real_compiler [] = {"C:\\VCAST\\64n\\MinGW\\bin\\gcc_real.exe"};
*/
const char name_of_real_compiler [] = {"\"C:\\Program Files\\mingw-w64\\x86_64-6.3.0-posix-seh-rt_v5-rev0\\mingw64\\bin\\gcc.exe\""};
/*
const char name_of_real_compiler [] = {"explorer.exe"};
*/

int readline ( FILE * cmdfile, unsigned char * linebuffer , unsigned long linebufferlength );

int main ( int argc, char * argv[] )
{
 
    // printf("\nUsage : compilername arguments");
    SYSTEMTIME thesystemtime ;
    GetSystemTime (&thesystemtime);
    sprintf ( (char*)timestring, "%d/%d/%d %d:%d:%d", (int) thesystemtime.wYear, (int)thesystemtime.wMonth, (int) thesystemtime.wDay,
        (int)thesystemtime.wHour, (int)thesystemtime.wMinute, (int)thesystemtime.wSecond);
    currentdirectory [0] = 0 ;
    int curdirlen = GetCurrentDirectory ( MAXCOMMANDLINE-2, (LPSTR)currentdirectory  );
    if (curdirlen > 0)
    {
        // printf("\nCurrent directory is %s", currentdirectory );
    }
    // argv [0] is our own name 
    if ( argc < 2 || argv [1] == 0 )
    {
        printf("\nNote : no command line parameters, we expect a compiler command line");
    }
    FILE * logfile = 0;
    {
        logfile = fopen ( "command_line_peek_log.txt", "at" );
    }
    if ( logfile == 0 )
    {
        printf("\nUnable to open output log file command_line_peek_log.txt\n");
        return 10;
    }
    fprintf( logfile, "\nTIME: %s", timestring);
    int i = 0 ;
    commandline [0] = 0 ;
    int commandlineindex = 0 ;
    int minusfindex = -1 ; /* not seen -f */
    while ( i < argc )
    {
        if ( ( commandlineindex + strlen (argv[i]) + 2 )  >= MAXCOMMANDLINE ) // +1 for the space and +1 for the null
        {
            printf("\nOverrun command line storage buffer\n");
            return 10 ;
        }
        strcpy ((char*) commandline + commandlineindex , argv[i]);
        commandlineindex += strlen (argv[i] );
        strcpy ( (char*)commandline + commandlineindex , " ");
        commandlineindex += 1 ;
        fprintf (logfile, "\n%s ", argv[i]);
        if ( strcmp (argv[i] , "-f") == 0 )
        {
            minusfindex = i+1;
        }
        if ( i == minusfindex )
        {
            FILE * minusffile = fopen ( argv[i], "rt" );
            if (minusffile)
            {
                int ch ;
                while ( (ch = fgetc ( minusffile )) != EOF )
                {
                    fprintf ( logfile, "%c", (char) ch);
                }
                fprintf ( logfile, " "); 
                fclose (minusffile);
                minusfindex = -1 ;
            }            
        }
        i++ ;
    }

    /* copy the first element of the commandline to the programname */
#if 0    
    strcpy ( (char*) programname, path_to_real_compiler );
    i = 0;
    int j = strlen (path_to_real_compiler) ;
    for (i=0; i< (MAXCOMMANDLINE-(j+1)); i++)
    {
        if ( commandline[i] <= 0x20 ) /* look for space to terminate program name */
        {
            break;
        }
        programname [j++] = commandline [i] ;
    }
    programname [j] = 0 ;
    if (strcmp ( (const char*) programname+j-4, ".exe" ) != 0 )
    {
        strcat ( (char*)programname, ".exe");
    }
#endif
    strcpy ( (char*) programname,  name_of_real_compiler );   
    sprintf(newcommandline, "%s %s", programname, &(commandline[4]) );
    printf("New command [%s]", newcommandline);
    fprintf(logfile, "\nPWD: %s", currentdirectory);

    // that MUST be a full path to the process with c: and .exe
    PROCESS_INFORMATION p_i ;
    STARTUPINFO si ;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    BOOL result = CreateProcess(
    (LPCTSTR) 0,
    (LPTSTR) newcommandline,
        0, //   __in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,
        0, //   __in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,
        0, //  __in         BOOL bInheritHandles,
        0, //  __in         DWORD dwCreationFlags,
        0, //__in_opt     LPVOID lpEnvironment,
        0, //  __in_opt     LPCTSTR lpCurrentDirectory,
        &si, // __in         LPSTARTUPINFO lpStartupInfo,
        &p_i  // __out        LPPROCESS_INFORMATION lpProcessInformation
    );

    printf ("\nResult = %d",(int)result) ;    // 1 is a good result.
    if (result)
    {
       WaitForSingleObject( p_i.hProcess, INFINITE );
    }

    fclose ( logfile);
    return 0;
}


