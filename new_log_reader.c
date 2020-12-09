#include "log_reader.h"

#define OPTSTR "d:h"
#define USAGE_FMT  "%s [-d Log directory] [-h]"
#define ERR_FOPEN_INPUT  "fopen(input, r)"
#define ERR_FOPEN_OUTPUT "fopen(output, w)"
#define ERR_LOGFILE_LAST_MOD "logfile_last_mod blew up"
#define DEFAULT_PROGNAME "log_reader"
#define MAX_FILENAME_LENGTH 28
#define MAX_LOGFILE_LINE_LENGTH 54
#define TIMESTAMP_LENGTH 19

extern int errno;
extern char *optarg;
extern int opterr, optind;

typedef struct {
    int verbose;
    uint32_t flags;
    FILE *input;
    FILE *output;
} options_t;

typedef struct {
    char name[MAX_FILENAME_LENGTH];
    char last_timestamp[TIMESTAMP_LENGTH];
} file_t;

void usage(char *progname, int opt);
int logfile_last_mod(options_t *options);
int weekday(int day, int month, int year);
int do_more_stuff(void);

int main(int argc, char *argv[])
{
    int opt;
    options_t options = {0, 0x0, stdin, stdout};
    opterr = 0;

    file_t file;

    while ((opt = getopt(argc, argv, OPTSTR)) !=EOF)
    {
        switch(opt) 
       {
            case 'd':
                if (!(options.input = fopen(optarg, "r")))
                {
                    perror(ERR_FOPEN_INPUT);
                    exit(EXIT_FAILURE);
                }
                break;
            
            case 'h':
            default:
                usage(basename(argv[0]),opt);
                break;
        }

    if (logfile_last_mod(&options) != EXIT_SUCCESS)
    {
        perror(ERR_LOGFILE_LAST_MOD);
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
    }
}

void usage(char *progname, int opt)
{
    fprintf(stderr,USAGE_FMT,progname?progname:DEFAULT_PROGNAME);
    exit(EXIT_FAILURE);
}

int logfile_last_mod(options_t *options)
{
    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    char timestamp[TIMESTAMP_LENGTH];
    const unsigned int max_line_length = MAX_LOGFILE_LINE_LENGTH + 1;
    char buff[max_line_length+1];
    fseek(options->input, -max_line_length, SEEK_END);
	fread(buff, max_line_length - 1, 1, options->input);
	buff[max_line_length - 1] = '\0';
	char *last_newline = strrchr(buff, '\n');
	char *last_line = last_newline + 1;
	int i;
	for (i=0; i<TIMESTAMP_LENGTH; i++)
	{
		timestamp[i] = last_line[i];
        timestamp[i+1] = '\0';
	}

    const char timestamp_format[] = "%d-%m-%Y_%H-%M-%S";
    char cur_time[TIMESTAMP_LENGTH];
    time_t rawtime, log_time;
	struct tm *time_now;
    time(&rawtime);
	time_now = localtime(&rawtime);
    
    struct tm logfile_time;
    memset(&logfile_time, 0, sizeof(struct tm));

    if (sscanf(timestamp,"%d-%d-%d_%d-%d-%d",
            &logfile_time.tm_mday,&logfile_time.tm_mon,&logfile_time.tm_year,
            &logfile_time.tm_hour,&logfile_time.tm_min,&logfile_time.tm_sec) == 0)
    {
        perror("Error parsing logfile timestamp.");
        exit(EXIT_FAILURE);
    }

    logfile_time.tm_isdst = -1;
    logfile_time.tm_mon --;
    logfile_time.tm_wday = weekday(logfile_time.tm_mday,logfile_time.tm_mon,logfile_time.tm_year);
    logfile_time.tm_year = logfile_time.tm_year - 1900;

    log_time = mktime(&logfile_time);
    double diff_t = difftime(rawtime,log_time);
    
    if(diff_t > 30)
    {
        printf("\nSelected log file last modified more than 30 seconds ago!");
        printf("\nPress Enter to continue anyway ");
        getchar();
        do_more_stuff();
    }
    else if(2 < diff_t < 30)
    {
        printf("\nSelected log file was edited at some point in the last 30 seconds.");
        printf("\nCheck manually for more recent edits.");
        printf("\nPress Enter to continue ");
        getchar();
        do_more_stuff();
    }
    else
    {
        printf("\nSelected log file was edited in the last 2 seconds.");
        printf("\nExperiment running.");
        printf("Press Enter to continue ");
        getchar();
        do_more_stuff();
    }
    
    return EXIT_SUCCESS;
}

int weekday(int day, int month, int year)
{
    int wkday = (day += month < 3 ? year-- : year - 2, 23*month/9 + day + 4 + year/4- year/100 + year/400)%7;  
    return wkday;
}

int do_more_stuff(void)
{
    return EXIT_SUCCESS;
}