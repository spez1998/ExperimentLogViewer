#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILENAME_LENGTH 28
#define MAX_LOGFILE_LINE_LENGTH 54

typedef struct {
    char name[MAX_FILENAME_LENGTH];
    char last_line[MAX_LOGFILE_LINE_LENGTH];
} file_t;

void remove_spaces(char *str_trimmed, const char *str_untrimmed);
FILE *open_log_file(char *raw_filename, size_t len_raw_filename);
char *get_last_line(FILE **file);
int get_time();

int main(int argc, char *argv[])
{
	file_t file;
	char raw_filename[MAX_FILENAME_LENGTH];
    /* char last_line[MAX_LOGFILE_LINE_LENGTH]; */
	printf("\nPlease enter the name of the log file.\n");
	fgets(raw_filename,MAX_FILENAME_LENGTH,stdin);
	printf("\nYou entered: %s",raw_filename);
	FILE *log_file = open_log_file(raw_filename,MAX_FILENAME_LENGTH);
	char *last_line = get_last_line(&log_file);
    strcpy(file.last_line,last_line);
    free(last_line);
	get_time();
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

char *get_last_line(FILE **file)
{
	static const long max_line_length = MAX_LOGFILE_LINE_LENGTH + 1; /* Max line length + newline */
	char buff[max_line_length + 1]; /* Space for max line length + null term */
	fseek(*file, -max_line_length, SEEK_END); /* Read to len(max line length) before EOF */
	fread(buff, max_line_length - 1, 1, *file);
	buff[max_line_length - 1] = '\0'; /* Terminate string */
	char *last_newline = strrchr(buff, '\n'); /* Find last newline */
	char *last_line = malloc(MAX_LOGFILE_LINE_LENGTH);
    strcpy(last_line,last_newline + 1);
	fclose(*file);
	return last_line;
}

FILE *open_log_file(char *raw_filename, size_t len_raw_filename)
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

int get_time()
{
	struct tm *timeinfo;
	printf("Current local time and date: %s",asctime(timeinfo));
	return 0;
}