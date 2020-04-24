#ifndef IMAGES_H
#define IMAGES_H
int     InitHeader(Image_characteristics *image,char *orig_name,int num_frame,char *seq_name,char *seq_desc,int format);
void    write_hips_image(char *imagename,Image_characteristics *image_characteristicsPtr);
int     Mmap_op(float fip,char bip,Data *data_Ptr,struct header *hd,int r,int c,int frame);
int     mmap_read_hips_image(char *imagename, struct  header  *head, Data *buffer,char *env);
void    write_hips_header(int fd,struct header *hd,int argc,char **argv);
void    mmap_write_hips_image(char *imagename, struct  header  *head,Data *buf,int argc,char **argv,int restart_flag);
int     mmap_write_hips_image_no_free(char *imagename, struct  header  *head,Data *buf,int argc,char **argv,int restart_flag);
void    set_data_value(float *sunshine,int op_col,int op_row,int op_frame,Data *op_buffer,struct header *op_head);
void    set_data(int ip_col,int ip_row,int ip_frame,int op_col,int op_row,int op_frame,Data *ip_buffer,struct header *ip_head,Data *op_buffer,struct header *op_head);
void    pad_header(struct header *head,int offset);
void    mmap_op(float data,Data buffer,int size,int pixel_format);
#endif
