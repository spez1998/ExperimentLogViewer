#include "log_reader.h"

#define MAX_FILENAME_LENGTH 28
#define MAX_LOGFILE_LINE_LENGTH 54
#define TIMESTAMP_LENGTH 20

typedef struct {
    char name[MAX_FILENAME_LENGTH];
    char last_timestamp[TIMESTAMP_LENGTH];
} file_t;

void remove_spaces(char *str_trimmed, const char *str_untrimmed);
FILE *open_log_file(char *raw_filename, size_t len_raw_filename, char *logfile_name);
char *get_last_timestamp(FILE **file, file_t *logfile);
int logfile_age(char *logfile_last_timestamp);

int main(int argc, char *argv[])
{
	file_t logfile;
	char raw_filename[MAX_FILENAME_LENGTH];
	printf("\nPlease enter the name of the log file.\n");
	fgets(raw_filename,MAX_FILENAME_LENGTH,stdin);
	printf("\nYou entered: %s",raw_filename);
	FILE *log_file = open_log_file(raw_filename,MAX_FILENAME_LENGTH,logfile.name);
	get_last_timestamp(&log_file,logfile.name);
	get_time(logfile.last_timestamp);
	return 0;
}

void remove_spaces(char *str_trimmed, const char *str_untrimmed)
{
	while (*str_untrimmed !='\0')
	{
		if (!isspace(*str_untrimmed))
		{
			*str_trimmed = *str_untrimmed;
			str_trimmed++;
		}
		str_untrimmed++;
	}
	*str_trimmed = '\0';
}

char *get_last_timestamp(FILE **file, file_t *logfile)
{
	const unsigned int max_line_length = MAX_LOGFILE_LINE_LENGTH + 1; /* Max line length + newline */
	char timestamp[TIMESTAMP_LENGTH];
	char buff[max_line_length + 1]; /* Space for max line length + null term */
	fseek(*file, -max_line_length, SEEK_END); /* Read to len(max line length) before EOF */
	fread(buff, max_line_length - 1, 1, *file);
	buff[max_line_length - 1] = '\0'; /* Terminate string */
	char *last_newline = strrchr(buff, '\n'); /* Find last newline */
	char *last_line = last_newline + 1; /* Jump to it */
	/* Extracting timestamp from last line */
	int i;
	for(i=0;i<TIMESTAMP_LENGTH;i++)
	{
		timestamp[i] = last_line[i];
	}
	strcpy(logfile->last_timestamp,timestamp);
	fclose(*file);
	return last_line;
}

FILE *open_log_file(char *raw_filename, size_t len_raw_filename, char *logfile_name)
{
	FILE *log_candidate;
	char filename[len_raw_filename];
	remove_spaces(filename,raw_filename); /* Can't read short filenames without this */
	if ((log_candidate = fopen(filename,"r")) == NULL)
	{
		printf("Error opening file. Check filename?\n");
		exit(1);
	}
	return log_candidate;
}

/* int get_time(char *last_timestamp)
{
	time_t now = time(NULL);
	time_t log_time;
	char timeinfo_log_form[TIMESTAMP_LENGTH];
	const char timestamp_format[] = "%d-%m-%Y_%H-%M-%S";
	struct tm tm;
	double diff;
	printf("\npoo %s",ctime(&now));
	printf("\nTrying to convert %s",last_timestamp);
	strptime(last_timestamp,timestamp_format,&tm);
	log_time = mktime(&tm);
	printf("%s",asctime(log_time));
	diff = difftime(now,log_time);
	printf("\n%f",diff);
	return 0;
} */

int get_time(char *last_timestamp)
{
	const char timestamp_format[] = "%d-%m-%Y_%H-%M-%S";
	time_t rawtime, logtime;
	struct tm logtime_tm = {0};
	time(&rawtime);
	printf("%s",ctime(&rawtime));
	strptime("23-12-1998_01-02-03",timestamp_format,&logtime_tm);
	mktime(&logtime_tm);
	printf("\nParsed: %s",ctime(&logtime_tm));
	double diff = difftime(&logtime_tm,rawtime);
	printf("\n%f",diff);
}