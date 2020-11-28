#include "log_reader.h"

#define OPTSTR "d:h"
#define USAGE_FMT  "%s [-d Log directory] [-h]"
#define ERR_FOPEN_INPUT  "fopen(input, r)"
#define ERR_FOPEN_OUTPUT "fopen(output, w)"
#define ERR_DO_STUFF "do_stuff blew up"
#define DEFAULT_PROGNAME "log_reader"
#define MAX_FILENAME_LENGTH 28
#define MAX_LOGFILE_LINE_LENGTH 54
#define TIMESTAMP_LENGTH 20

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
int do_stuff(options_t *options);

int main(int argc, char *argv[])
{
    int opt;
    options_t options = {0,0x0,stdin,stdout};
    opterr = 0;

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
    if (do_stuff(&options) != EXIT_SUCCESS)
    {
        perror(ERR_DO_STUFF);
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

int do_stuff(options_t *options)
{
    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    printf("%s",options->input);

    return EXIT_SUCCESS;
}