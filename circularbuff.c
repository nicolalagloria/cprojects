/*

Circular buffer del mezzodi' in Apple

Original concept by Alessio Centazzo :)
(senno' col cazzo che passi l'intervista)

Nightcode by Nicola La Gloria

08/16/2014

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
struct buffer
{
	/* data */
	size_t lenght;
	char *data;
	char *r_ptr;
	char *w_ptr;
	intptr_t end_addr;
	intptr_t start_addr;
};

int main(){
	
	// data 
	char mydata[]={0xfe,0x01,0x03,0x00};
	
	// another chunk of data 

	char mydata2[]={0xfe,0xaa,0x00};

	struct buffer mybuffer;
	
	// Init the buffer 
	
	size_t lenght = 6;

	mybuffer.lenght = lenght;

	mybuffer.data = calloc(lenght,sizeof(char));
	mybuffer.start_addr = (intptr_t)mybuffer.data;
	mybuffer.r_ptr = mybuffer.data;
	mybuffer.w_ptr = mybuffer.data;
	mybuffer.end_addr = mybuffer.start_addr + lenght * sizeof(char) -1;
	
	printf("End buffer Address at 0x%016" PRIxPTR "\n", mybuffer.end_addr);
	
	// write & read on the circular buffer

	write_to_buffer(&mybuffer, mydata);
	read_from_buffer(&mybuffer, 3);
	write_to_buffer(&mybuffer, mydata);
	write_to_buffer(&mybuffer, mydata2);
	read_from_buffer(&mybuffer, 5);
	//free data buffer
	free(mybuffer.data);

return 0;

}

int write_to_buffer(struct buffer* mybuffer, unsigned char *mydata) {

	size_t data_lenght = strlen(mydata);
	int i;
	int free_locations;

	// printf("%zu\n", data_lenght);
	
	
	// Calculate the free space 
	
	if (mybuffer->w_ptr < mybuffer->r_ptr)
		free_locations = mybuffer->r_ptr - mybuffer->w_ptr;
	else
		free_locations = (mybuffer->end_addr - (intptr_t)mybuffer->w_ptr) + ((intptr_t)mybuffer->r_ptr);

	if (data_lenght > free_locations){
		printf("Buffer Overflow \n");
		return 0;
	}

	for (i = 0 ; i < data_lenght; i++) {

		*(mybuffer->w_ptr) = *mydata;
		printf("Write %d to %p\n", *mydata,mybuffer->w_ptr);
		
		mydata++;

		if ((intptr_t)mybuffer->w_ptr < mybuffer->end_addr)
			(mybuffer->w_ptr)++;
		else
			(mybuffer->w_ptr) = mybuffer->data;
	
	}

	return 1;

}

int read_from_buffer(struct buffer* mybuffer, int len) {
	
	int i;
	
	if (len > mybuffer->lenght - 1 ) {
		printf("Read buffer out of range, aborted\n");
		return 0;
	}  
	
	for (i = 0; i < len; i++){
		printf("Read %d from %p\n", (unsigned char)(*mybuffer->r_ptr), mybuffer->r_ptr);
		(mybuffer->r_ptr)++;
		if ((intptr_t)mybuffer->r_ptr > mybuffer->end_addr){
			 printf("inside\n");
			 mybuffer->r_ptr = mybuffer->data;
			}
	}
		return 1;


}