#ifndef LIBHIPL_H
#define LIBHIPL_H
#include "hipl_format.h"
#include <stdio.h>

int fd_write_line(int fd, char *buf);
int fp_read_line(FILE *fp, char *buf, int size);
void h2_fp_rhd(FILE *fp,struct header *hd);
void h2clean_header(struct  header *hd);
char * copy_buf(char *buf,int len);
char * add_buf(char *desc, char *buf, int len);
int fp_read_line(FILE *fp, char *buf, int size);
int fd_read_line(int fd, char *buf, int size);
void h2_fd_rhd(int fd,struct header *hd);
void hperr(char *a);
int fp_write_line(FILE *fp, char *buf);
void h2_fd_rhd(int fd,struct header *hd);
char * h2clean_hist(char *buf);

void fp_fread_header(FILE *fp, struct header *hd);
void fread_header(int fd, struct header *hd);
void init_header(struct header *hd,char *orig_name,char *seq_name,int num_frame,char *orig_date,int rows,int cols,int bits_per_pixel,int bit_packing,int pixel_format,char *seq_desc);
int hpread(int fd, char *buf, int len);
void fp_fwrite_header(FILE *fp, struct header *hd);
void fwrite_header(int fd, struct header *hd);
void update_desc(struct header   *hd, char *buf);
char * fix_desc(char *desc);
void update_header(struct header *hd, int argc, char **argv);
#endif
