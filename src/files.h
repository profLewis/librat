#ifndef FILES_H
#define FILES_H
FILE    *openFileForRead(char **filename,char *env,int fatal);
FILE    *open_file_for_read(char *filename);
FILE    *openFileForWrite(char **filename,char *env,int fatal);
int     get_no_of_columns_in_file(int verbose,char *filename,int *rows);

#ifndef CLOSE
#define CLOSE -1
#endif

#endif

