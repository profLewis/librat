#include "prat.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

int	InitHeader(image,orig_name,num_frame,seq_name,seq_desc,format)
Image_characteristics	*image;
char	*orig_name,*seq_name,*seq_desc;
int	format,num_frame;
{
	int	size=0;
	char	*orig_date;
	time_t	clock;

	switch(format){
		case PFBYTE:
		size=8*sizeof(char);
		break;
		case PFFLOAT:
		size=8*sizeof(float);
		break;
		case PFINT:
		size=8*sizeof(int);
		break;
		default:
		error1("InitHeader request for unsupported file format (see images.c)");
		break;
	}
	clock=time(NULL);
	orig_date=ctime(&clock);

	init_header(&image->hd,orig_name,seq_name,num_frame,orig_date,image->hd.rows,image->hd.cols,size,0,format,seq_desc);

	return(1);
}
	
void	write_hips_image(imagename,image_characteristicsPtr)
char	*imagename;
Image_characteristics	*image_characteristicsPtr;
{
	switch(image_characteristicsPtr->hd.pixel_format){
		case PFBYTE:	
	if(write(image_characteristicsPtr->fd,image_characteristicsPtr->data.bdata,image_characteristicsPtr->hd.cols*image_characteristicsPtr->hd.rows*image_characteristicsPtr->hd.num_frame*sizeof(char))!=image_characteristicsPtr->hd.cols*image_characteristicsPtr->hd.rows*image_characteristicsPtr->hd.num_frame*sizeof(char))error2("write_hips_image:\terror writing image data for",imagename);
		break;
		case PFFLOAT:	
	if(write(image_characteristicsPtr->fd,image_characteristicsPtr->data.fdata,image_characteristicsPtr->hd.cols*image_characteristicsPtr->hd.rows*image_characteristicsPtr->hd.num_frame*sizeof(float))!=image_characteristicsPtr->hd.cols*image_characteristicsPtr->hd.rows*image_characteristicsPtr->hd.num_frame*sizeof(float))error2("write_hips_image:\terror writing image data for",imagename);
		break;
		default:
		error1("write_hips_image:\tinvalid pixel format");
	}
	return;
}

int	Mmap_op(fip,bip,data_Ptr,hd,r,c,frame)
Data	*data_Ptr;
struct header *hd;
int	r,c,frame;
float	fip;	/* not so elegant this bit... */
char	bip;
{
	switch(hd->pixel_format){
		case PFBYTE:
		/*fprintf(stdout,"%c ",bip);*/
		*(data_Ptr->bdata+(frame*hd->cols*hd->rows)+(r*hd->cols)+c)=bip;
		break;
		case PFFLOAT:
		/*fprintf(stdout,"%f ",fip);*/
		*(data_Ptr->fdata+(frame*hd->cols*hd->rows)+(r*hd->cols)+c)=fip;
		break;
	}
	return(1);
}

/* mmap input image into buffer */

void	mmap_read_hips_image(imagename, head, buffer)
char            *imagename;
struct  header  *head;
Data	*buffer;
{
	long offset;
        int     rows, cols;
        int     mmap_flag, fd;
#ifdef __SUNPRO_C___
		int lseek();
#endif
	void	exit(),fread_header();
	Data	D_allocate();

#ifdef MMAP
	mmap_flag=1;
#else
	mmap_flag=0;
#endif
        if((fd=open(imagename, 0)) == -1){fprintf(stderr,"%s does not exist\n", imagename);exit(-1);}
        fread_header(fd, head);
        rows = head->rows;
        cols = head->cols;
#ifdef MMAP
        if(mmap_flag){
	        offset=lseek(fd, 0L, SEEK_CUR);
		lseek(fd,0L,L_SET);
		switch(head->pixel_format){
			case PFBYTE:
		        if ( ( buffer->bdata = (unsigned char *)mmap(0, sizeof(char)*rows*cols*head->num_frame+offset, PROT_READ,  MAP_SHARED, fd, 0) ) == NULL )error1("mmap_read_hips_image:\tmmap failed");
			buffer->bdata +=offset;break;
			case PFFLOAT:
		       	if ( ( buffer->fdata = (float *)mmap(0, sizeof(float)*rows*cols*head->num_frame+offset, PROT_READ,  MAP_SHARED, fd, 0) ) == NULL )error1("mmap_read_hips_image:\tmmap failed");
			buffer->fdata +=(offset/4);break;
			default:
			error1("mmap_read_hips_image:\timage pixel format must be bytes or float");
		}
       		close(fd);
	}else{
#endif
                if(buffer->bdata)free(buffer->bdata);
                if(buffer->fdata)free(buffer->fdata);
		*buffer = D_allocate(head->pixel_format,rows*cols*head->num_frame);
		switch(head->pixel_format){
			case PFBYTE:
			if(read(fd,buffer->bdata,rows*cols*head->num_frame*sizeof(char))!=rows*cols*head->num_frame*sizeof(char))error1("mmap_read_hips_image:\terror reading image data in non_mmap mode");
			break;		
			case PFFLOAT:
			if(read(fd,buffer->fdata,rows*cols*head->num_frame*sizeof(float))!=rows*cols*head->num_frame*sizeof(float))error1("mmap_read_hips_image:\terror reading image data in non_mmap mode");
			break;		
			default:
			error1("mmap_read_hips_image:\timage pixel format must be bytes or float");
		}
       		close(fd);
#ifdef MMAP
	}
#endif
}

/* write out updated hips header */
void	write_hips_header(fd,hd,argc,argv)
struct header *hd;int argc;char **argv;int fd;
{
	void	update_header(),fwrite_header();

	update_header(hd,argc,argv);
	fwrite_header(fd,hd);
}

#ifdef MMAP
/* setup mmap op image - if !restart, write data first */
void	mmap_write_hips_image(imagename, head,buf,argc,argv,restart_flag)
char    *imagename;int argc;char **argv;int restart_flag;
struct  header  *head;
Data	*buf;
{
        int     rows, cols;
        int     fd, offset,size=0;
#ifdef __SUNPRO_C___
		int	lseek();
#endif
	void	exit(),fread_header();

#ifndef __SUNPRO_C
#endif
	if(!restart_flag){
        	if((fd=open(imagename,O_RDWR)) == -1){fprintf(stderr,"cannot open %s\n", imagename);exit(-1);}
#ifdef __SUNPRO_C
		chmod(imagename,S_IREAD|S_IWRITE);
#else
                chmod(imagename,S_IRUSR|S_IWUSR);
#endif
        	write_hips_header(fd,head,argc,argv);	/* write header */
      		rows = head->rows;
       		cols = head->cols;
		/*buf=D_allocate(head->pixel_format,rows*cols*head->num_frame);*/
        	offset=lseek(fd, 0, SEEK_CUR);
		switch(head->pixel_format){
			case PFBYTE:
			size=sizeof(char);
			write(fd,buf->bdata,(size*rows*cols*head->num_frame));
			free(buf->bdata);
			break;
			case PFFLOAT:
			size=sizeof(float);
			write(fd,buf->fdata,(size*rows*cols*head->num_frame));
			free(buf->fdata);
			break;
		}
	}else{
         	if((fd=open(imagename,O_RDWR)) == -1){fprintf(stderr,"cannot open %s\n", imagename);exit(-1);}
       		fread_header(fd, head);
        	rows = head->rows;
        	cols = head->cols;
       	 	offset=lseek(fd, 0L, SEEK_CUR);
	}
        lseek(fd,0,L_SET);
	switch(head->pixel_format){
		case PFBYTE:
		        if((int)(buf->bdata = (unsigned char *)mmap(NULL, sizeof(char)*rows*cols*head->num_frame+offset, PROT_READ|PROT_WRITE,  MAP_SHARED, fd, 0))==-1){
				fprintf(stderr,"error in mmap\n");exit(-1);
			}
			buf->bdata +=offset;break;
		case PFFLOAT:
		        if((int)(buf->fdata = (float *)mmap(0, sizeof(float)*rows*cols*head->num_frame+offset, PROT_READ|PROT_WRITE,  MAP_SHARED, fd, 0))==-1){
				fprintf(stderr,"error in mmap\n");exit(-1);
			}
			buf->fdata +=(offset/size);break;
		default:
			fprintf(stderr,"image pixel format must be bytes or float\n");exit(-1);break;
	}
        close(fd);
}
#endif
/*
**	mmaping for output
*/

int	mmap_write_hips_image_no_free(imagename, head,buf,argc,argv,restart_flag)
char    *imagename;int argc;char **argv;int restart_flag;
struct  header  *head;
Data	*buf;
{
        int     i,size,rows, cols,mmap_flag;
        int     fd, offset=0;
#ifdef __SUNPRO_C___
		int	lseek();
#endif
	Data 	buffer,D_allocate();
	void	exit(),fwrite_header(),fread_header();

/*
**	get environmental variable MMAP_OP to see
**	if should mmap or not
*/

	mmap_flag=1;

	if(!restart_flag){
        	if((fd=open(imagename,O_RDWR)) == -1)
			error2("cannot open ",imagename);
		chmod(imagename,S_IRUSR|S_IWUSR);
        	fwrite_header(fd,head);	/* write header (no update) */
      		rows = head->rows;
       		cols = head->cols;
#ifdef MMAP
		if(mmap_flag){
			buffer=D_allocate(head->pixel_format,cols);
        		offset=lseek(fd, 0, SEEK_CUR);
			for(i=0;i<rows*head->num_frame;i++){
				switch(head->pixel_format){
					case PFBYTE:
					size=sizeof(char);
					write(fd,buffer.bdata,(size*cols));
					break;
					case PFFLOAT:
					size=sizeof(float);
					write(fd,buffer.fdata,(size*cols));
					break;
				}
			}
		}else
#endif
			buffer=D_allocate(head->pixel_format,cols*rows*head->num_frame);
	}else{
         	if((fd=open(imagename,O_RDWR)) == -1){fprintf(stderr,"cannot open %s\n", imagename);exit(-1);}
       		fread_header(fd, head);
        	rows = head->rows;
        	cols = head->cols;
       	 	offset=lseek(fd, 0L, SEEK_CUR);
	}
#ifdef MMAP
	if(mmap_flag){
        	lseek(fd,0,L_SET);
		switch(head->pixel_format){
			case PFBYTE:
		       	if((int)(buf->bdata = (unsigned char *)mmap(NULL, sizeof(char)*rows*cols*head->num_frame+offset, PROT_READ|PROT_WRITE,  MAP_SHARED, fd, 0))==-1){
					fprintf(stderr,"error in mmap\n");exit(-1);
				}
			buf->bdata +=offset;break;
			case PFFLOAT:
		        if((int)(buf->fdata = (float *)mmap(0, sizeof(float)*rows*cols*head->num_frame+offset, PROT_READ|PROT_WRITE,  MAP_SHARED, fd, 0))==-1){
				fprintf(stderr,"error in mmap\n");exit(-1);
			}
			buf->fdata +=(offset/4);break;
			default:
			fprintf(stderr,"image pixel format must be bytes or float\n");exit(-1);break;
		}
       	 	/*close(fd);*/
	}else{
#endif
		switch(head->pixel_format){
			case PFBYTE:
			buf->bdata=buffer.bdata;
			break;
			case PFFLOAT:
			buf->fdata=buffer.fdata;
			break;
			default:
			fprintf(stderr,"image pixel format must be bytes or float\n");exit(-1);break;
		}
#ifdef MMAP
	}
#endif
	return(fd);
}

void	set_data_value(sunshine,op_col,op_row,op_frame,op_buffer,op_head)
int	op_col,op_row,op_frame;float	*sunshine;
Data	*op_buffer;struct header *op_head;
{
	int	i;
	for(i=0;i<op_head->num_frame;i++)
	switch(op_head->pixel_format){
		case PFFLOAT:
		*(op_buffer->fdata+op_frame*op_head->rows*op_head->cols+op_row*op_head->cols+op_col)=sunshine[i];break;
		case PFBYTE:
		*(op_buffer->bdata+op_frame*op_head->rows*op_head->cols+op_row*op_head->cols+op_col)=(char)sunshine[i];break;
	}

}

void	set_data(ip_col,ip_row,ip_frame,op_col,op_row,op_frame,ip_buffer,ip_head,op_buffer,op_head)
int	ip_col,ip_row,ip_frame,op_col,op_row,op_frame;
Data 	*ip_buffer,*op_buffer;
struct header *ip_head,*op_head;
{
	switch(op_head->pixel_format){
		case PFFLOAT:
		switch(ip_head->pixel_format){
			case PFFLOAT:
			*(op_buffer->fdata+op_frame*op_head->rows*op_head->cols+op_row*op_head->cols+op_col)= *(ip_buffer->fdata+ip_frame*ip_head->rows*ip_head->cols+ip_row*ip_head->cols+ip_col);break;
			case PFBYTE:
			*(op_buffer->fdata+op_frame*op_head->rows*op_head->cols+op_row*op_head->cols+op_col)=(float) *(ip_buffer->bdata+ip_frame*ip_head->rows*ip_head->cols+ip_row*ip_head->cols+ip_col);break;
		}break;
		case PFBYTE:
		switch(ip_head->pixel_format){
			case PFFLOAT:
			*(op_buffer->bdata+op_frame*op_head->rows*op_head->cols+op_row*op_head->cols+op_col)=(char) *(ip_buffer->fdata+ip_frame*ip_head->rows*ip_head->cols+ip_row*ip_head->cols+ip_col);break;
			case PFBYTE:
			*(op_buffer->bdata+op_frame*op_head->rows*op_head->cols+op_row*op_head->cols+op_col)= *(ip_buffer->bdata+ip_frame*ip_head->rows*ip_head->cols+ip_row*ip_head->cols+ip_col);break;
		}break;
	}
}


void	pad_header(head,offset)
struct header *head;int	offset;
{
	int	size,add;char	seq_desc[1000];

	strcpy(seq_desc,"   ");
	if( (size=(offset/4)*4)==offset)return;
	add=4-offset+size;
	strcpy(seq_desc+add,head->seq_desc);
	strcpy(head->seq_desc,seq_desc);
	return;
}

void	mmap_op(data,buffer,size,pixel_format)
Data	buffer;int	size,pixel_format;float	data;
{
	switch(pixel_format){
		case PFBYTE:
		*(buffer.bdata+size)=(char)data;break;
		case PFFLOAT:
		*(buffer.fdata+size)=data;break;
	}
	return;
}
