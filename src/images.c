#include "prat.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

int	InitHeader(Image_characteristics *image,char *orig_name,int num_frame,char *seq_name,char *seq_desc,int format)
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
	
void	write_hips_image(char *imagename,Image_characteristics *image_characteristicsPtr)
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

int	Mmap_op(float fip,char bip,Data *data_Ptr,struct header *hd,int r,int c,int frame)
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
#ifndef CLOSE
#define CLOSE -1
#endif

int	mmap_read_hips_image(char *imagename, struct  header  *head, Data *buffer,char *env)
{
        FILE *fp;
        int     rows, cols;
        int     mmap_flag;

#ifdef MMAP
	mmap_flag=1;
#else
	mmap_flag=0;
#endif
        if(!(fp=openFile(imagename,TRUE,env))){
          error2("Error opening image for read",imagename);
          exit(1);
        } 
        fp_fread_header(fp, head);
        rows = head->rows;
        cols = head->cols;
        if(buffer->bdata)free(buffer->bdata);
        if(buffer->fdata)free(buffer->fdata);
	*buffer = D_allocate(head->pixel_format,rows*cols*head->num_frame);
	switch(head->pixel_format){
	    case PFBYTE:
		if((int)fread(buffer->bdata,sizeof(char),rows*cols*head->num_frame,fp)!=(int)rows*cols*head->num_frame){
                	error2("mmap_read_hips_image:\terror reading PFBYTE image data",imagename);
                        exit(1);
                }
		break;		
	    case PFFLOAT:
		if((int)fread(buffer->fdata,sizeof(float),rows*cols*head->num_frame,fp)!=(int)(rows*cols*head->num_frame)){
                         error2("mmap_read_hips_image:\terror reading PFfloat image data",imagename);
                         exit(1);
		}
                break;		
            case PFINT:
                if((int)fread(buffer->idata,sizeof(float),rows*cols*head->num_frame,fp)!=(int)(rows*cols*head->num_frame)){
                         error2("mmap_read_hips_image:\terror reading PFint image data",imagename);
                         exit(1);
                }
                break;
            case PFSHORT:
                if((int)fread(buffer->sdata,sizeof(float),rows*cols*head->num_frame,fp)!=(int)rows*cols*head->num_frame){
                         error2("mmap_read_hips_image:\terror reading PFSHORT image data",imagename);
                         exit(1);
                }
                break;
  	    default:
		error2("mmap_read_hips_image:\timage pixel format must be bytes or int or short or float",imagename);
                exit(1);
	}
        fp=openFile(imagename,CLOSE,(char *)fp);
        return(0);
}

/* write out updated hips header */
void	write_hips_header(int fd,struct header *hd,int argc,char **argv)
{

	update_header(hd,argc,argv);
	fwrite_header(fd,hd);
}

#ifdef MMAP
/* setup mmap op image - if !restart, write data first */
void	mmap_write_hips_image(char *imagename, struct  header  *head,Data *buf,int argc,char **argv,int restart_flag)
{
        int     rows, cols;
        int     fd, offset,size=0;

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

int	mmap_write_hips_image_no_free(char *imagename, struct  header  *head,Data *buf,int argc,char **argv,int restart_flag)
{
        int     rows, cols,mmap_flag;
        int     fd, offset=0;
	Data 	buffer;

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

void	set_data_value(float *sunshine,int op_col,int op_row,int op_frame,Data *op_buffer,struct header *op_head)
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

void	set_data(int ip_col,int ip_row,int ip_frame,int op_col,int op_row,int op_frame,Data *ip_buffer,struct header *ip_head,Data *op_buffer,struct header *op_head)
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


void	pad_header(struct header *head,int offset)
{
	int	size,add;char	seq_desc[1000];

	strcpy(seq_desc,"   ");
	if( (size=(offset/4)*4)==offset)return;
	add=4-offset+size;
	strcpy(seq_desc+add,head->seq_desc);
	strcpy(head->seq_desc,seq_desc);
	return;
}

void	mmap_op(float data,Data buffer,int size,int pixel_format)
{
	switch(pixel_format){
		case PFBYTE:
		*(buffer.bdata+size)=(char)data;break;
		case PFFLOAT:
		*(buffer.fdata+size)=data;break;
	}
	return;
}
