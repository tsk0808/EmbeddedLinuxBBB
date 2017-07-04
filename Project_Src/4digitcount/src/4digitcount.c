/*
 ============================================================================
 Name        : 7segcnt.c
 Author      : Kiran
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 DIY: IMPLEMENT UPDOWN COUNTING
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include<stdint.h>
#include <time.h>
#include <math.h>

uint8_t numbers[]={0x77,0x06,0xB3,0x97,0xc6,0xD5,0xf5,0x07,0xf7,0XD7,0x00};


#define MAX_LEN 100


#define DIGIT_ON 1
#define DIGIT_OFF 0


#define SYS_GPIO_DIR_PATH "/sys/class/gpio"

/*
P8.7 ( GPIO_66); a
P8.8 ( GPIO_67); b
P8.9 ( GPIO_69); c
P8.10 ( GPIO_68);dp (h)
P8.11 ( GPIO_45);d
P8.12 ( GPIO_44);e
P8.14 ( GPIO_26);f
P8.16 ( GPIO_46);g */

#define GPIO_66_P8_7_SEGA  		66       /*  display pin 11	*/
#define GPIO_67_P8_8_SEGB 		67       /*  display pin 7	*/
#define GPIO_69_P8_9_SEGC 		69       /*  display pin 4	*/
#define GPIO_68_P8_10_DP 		68       /*  display pin 3	*/
#define GPIO_45_P8_11_SEGD 		45       /*  display pin 2	*/
#define GPIO_44_P8_12_SEGE 		44       /*  display pin 1	*/
#define GPIO_26_P8_14_SEGF 		26       /*  display pin 10	*/
#define GPIO_46_P8_16_SEGG 		46       /*  display pin 5	*/

#define GPIO_48_P_15_DIGIT1    48        /*  display pin 12	*/
#define GPIO_49_P_23_DIGIT2    49        /*  display pin 9	*/
#define GPIO_117_P_30_DIGIT3   112       /*  display pin 8 	*/
#define GPIO_115_P_27_DIGIT4   115      /*  display pin 6	*/


#define HIGH_VALUE 1
#define LOW_VALUE  0

#define OUT_DIR HIGH_VALUE
#define IN_DIR  LOW_VALUE

#define SEGMENT_ON LOW_VALUE
#define SEGMENT_OFF HIGH_VALUE



/* This is the array of file descriptors for each segment of the display */
int fdgpio[8];


int gpio_configure_dir(uint32_t gpio_num, uint8_t dir_value)
{
	int fd;
	char buf[MAX_LEN];

	snprintf(buf, sizeof(buf), SYS_GPIO_DIR_PATH "/gpio%d/direction", gpio_num);
	//printf("%s\n",buf);
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio direction configure\n");
		return fd;
	}

	if (dir_value)
		write(fd, "out", 4);
	else
		write(fd, "in", 3);

	close(fd);
	return 0;
}


int gpio_write_value(uint32_t gpio_num, uint8_t out_value)
{
	int fd;
	char buf[MAX_LEN];

	snprintf(buf, sizeof(buf), SYS_GPIO_DIR_PATH "/gpio%d/value", gpio_num);
	//printf("%s\n",buf);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio write value");
		return fd;
	}

	if (out_value)
		write(fd, "1", 2);
	else
		write(fd, "0", 2);

	close(fd);
	return 0;
}


int gpio_read_value(uint32_t gpio_num)
{
	int fd;
	uint8_t read_value=0;
	char buf[MAX_LEN];

	snprintf(buf, sizeof(buf), SYS_GPIO_DIR_PATH "/gpio%d/value", gpio_num);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio read value");
		return fd;
	}

	read(fd, &read_value, 1);

	close(fd);
	return read_value;
}


int gpio_configure_edge(uint32_t gpio_num, char *edge)
{
	int fd;
	char buf[MAX_LEN];

	snprintf(buf, sizeof(buf), SYS_GPIO_DIR_PATH "/gpio%d/edge", gpio_num);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio configure edge");
		return fd;
	}

	write(fd, edge, strlen(edge) + 1);
	close(fd);
	return 0;
}


int gpio_file_open(uint32_t gpio_num)
{
	int fd;
	char buf[MAX_LEN];

	snprintf(buf, sizeof(buf), SYS_GPIO_DIR_PATH "/gpio%d/value", gpio_num);

	fd = open(buf, O_RDONLY | O_NONBLOCK );
	if (fd < 0) {
		perror("gpio file open");
	}
	return fd;
}


int gpio_file_close(int fd)
{
	return close(fd);
}



void digit_control(int digit_number, int value)
{


}
void Write_number_to_7segdisplay(uint8_t numberToDisplay)
{

	 switch (numberToDisplay){

	  case 0:
	    gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
	    gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
	    gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
	    gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
	    gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_ON);
	    gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
	    gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_OFF);
	    break;

	  case 1:
	    gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_OFF);
	    gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
	    gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
	    gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_OFF);
	    gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
	    gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_OFF);
	    gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_OFF);
	    break;

	  case 2:
	    gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
	    gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
	    gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_OFF);
	    gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
	    gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_ON);
	    gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_OFF);
	    gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
	    break;

	  case 3:
	    gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
	    gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
	    gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
	    gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
	    gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
	    gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_OFF);
	    gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
	    break;

	  case 4:
	    gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_OFF);
	    gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
	    gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
	    gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_OFF);
	    gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
	    gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
	    gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
	    break;

	  case 5:
		  printf("printing 5\n");
	    gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
	    gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_OFF);
	    gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
	    gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
	    gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
	    gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
	    gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
	    break;

	  case 6:
	    gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
	    gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_OFF);
	    gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
	    gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
	    gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_ON);
	    gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
	    gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
	    break;

	  case 7:
	    gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
	    gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
	    gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
	    gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_OFF);
	    gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
	    gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_OFF);
	    gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_OFF);
	    break;

	  case 8:
	    gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
	    gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
	    gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
	    gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
	    gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_ON);
	    gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
	    gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
	    break;

	  case 9:
	    gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
	    gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
	    gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
	    gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
	    gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
	    gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
	    gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
	    break;

	  case 10:
	    gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_OFF);
	    gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_OFF);
	    gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_OFF);
	    gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_OFF);
	    gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
	    gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_OFF);
	    gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_OFF);
	    break;
	  }



}


void dispaly_numbers(uint32_t number)
{

for(int digit = 4 ; digit > 0 ; digit--) {

	//Turn on a digit for a short amount of time
	switch(digit) {
	case 1:
		gpio_write_value(GPIO_48_P_15_DIGIT1,HIGH_VALUE);
	  break;

	case 2:
		gpio_write_value(GPIO_49_P_23_DIGIT2,HIGH_VALUE);
	  break;
	case 3:
		gpio_write_value(GPIO_117_P_30_DIGIT3,HIGH_VALUE);
	  break;
	case 4:
		gpio_write_value(GPIO_115_P_27_DIGIT4,HIGH_VALUE);
	  break;
	}


	Write_number_to_7segdisplay(5);
	number /= 10;

	usleep(500);

#if 0
 //Turn off all segments
	//passing 10 as an argument to this function, turns off all the segments
	Write_number_to_7segdisplay(5);

	//Turn off all digits
	gpio_write_value(GPIO_48_P_15_DIGIT1, LOW_VALUE);
	gpio_write_value(GPIO_49_P_23_DIGIT2, LOW_VALUE);
	gpio_write_value(GPIO_117_P_30_DIGIT3, LOW_VALUE);
	gpio_write_value(GPIO_115_P_27_DIGIT4, LOW_VALUE);
#endif

}
}


uint32_t get_time_in_ms(void)
{

#if 1
	uint32_t ms; // Milliseconds
	time_t          s;  // Seconds
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);
	s  = spec.tv_sec;
	ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds

	return ms;

#endif
	// struct timeval start, end;
	// gettimeofday(&start, NULL);

}
void start_upcounting(int delay_value)
{
	int counter=0;
	uint32_t old_ms;
     printf("UP COUNTING.......\n");
     old_ms = get_time_in_ms();

		while(1)
       {
#if 1
           if(get_time_in_ms() < (0 + 500 ) )
           {
			dispaly_numbers(counter);
			  printf("ms %d\n", get_time_in_ms());
           }
           else
           {
        	   counter++;
        	   printf("number display %d\n", counter);
        	   old_ms = get_time_in_ms();

           }
#endif
           dispaly_numbers(counter % 100);
           counter++;
       }

}

void start_downcounting(int delay_value)
{
     printf("DOWN COUNTING.......\n");
		while(1)
       {


       }

}

void start_updowncounting(int delay_value)
{

	/* DIY : Student has to implement this */
	printf("/* DIY : Student has to implement this */\n");
	gpio_write_value(GPIO_48_P_15_DIGIT1,HIGH_VALUE);
	Write_number_to_7segdisplay(5);
	sleep(1);
	Write_number_to_7segdisplay(4);
	sleep(1);
		Write_number_to_7segdisplay(3);
		sleep(1);
			Write_number_to_7segdisplay(2);



}

void start_randomcounting(int delay_value)
{

}

int main(int argc, char *argv[]) {
	printf("This program controls the USER LED3\n");

	if ( argc != 3 ) /* argc should be 3 for correct execution */
	{

		printf( "usage: %s <direction> <delay>\n", argv[0] );
		printf( "valid direction : up, down, updown\n");
		printf ("recommended delay range : 0 to 1000\n");
	}
	else
	{
		int value = atoi(argv[2]);

		/*first configure the direction for segments */
		gpio_configure_dir(GPIO_66_P8_7_SEGA,OUT_DIR);
		gpio_configure_dir(GPIO_67_P8_8_SEGB,OUT_DIR);
		gpio_configure_dir(GPIO_69_P8_9_SEGC,OUT_DIR);
		gpio_configure_dir(GPIO_68_P8_10_DP,OUT_DIR);
		gpio_configure_dir(GPIO_45_P8_11_SEGD,OUT_DIR);
		gpio_configure_dir(GPIO_44_P8_12_SEGE,OUT_DIR);
		gpio_configure_dir(GPIO_26_P8_14_SEGF,OUT_DIR);
		gpio_configure_dir(GPIO_46_P8_16_SEGG,OUT_DIR);

		/*configure the direction for digit control */
		gpio_configure_dir(GPIO_48_P_15_DIGIT1,OUT_DIR);
		gpio_configure_dir(GPIO_49_P_23_DIGIT2,OUT_DIR);
		gpio_configure_dir(GPIO_117_P_30_DIGIT3,OUT_DIR);
		gpio_configure_dir(GPIO_115_P_27_DIGIT4,OUT_DIR);

		/* make all segments low */
		gpio_write_value(GPIO_66_P8_7_SEGA,LOW_VALUE);
		gpio_write_value(GPIO_67_P8_8_SEGB,LOW_VALUE);
		gpio_write_value(GPIO_69_P8_9_SEGC,LOW_VALUE);
		gpio_write_value(GPIO_68_P8_10_DP,LOW_VALUE);
		gpio_write_value(GPIO_45_P8_11_SEGD,LOW_VALUE);
		gpio_write_value(GPIO_44_P8_12_SEGE,LOW_VALUE);
		gpio_write_value(GPIO_26_P8_14_SEGF,LOW_VALUE);
		gpio_write_value(GPIO_46_P8_16_SEGG,LOW_VALUE);

		/*MAKE ALL DIGITS OFF */
		gpio_write_value(GPIO_48_P_15_DIGIT1,LOW_VALUE);
		gpio_write_value(GPIO_49_P_23_DIGIT2,LOW_VALUE);
		gpio_write_value(GPIO_117_P_30_DIGIT3,LOW_VALUE);
		gpio_write_value(GPIO_115_P_27_DIGIT4,LOW_VALUE);

#if 0
		/* Open all segments */
		fdgpio[0]=gpio_file_open(GPIO_66_P8_7_SEGA);
		fdgpio[1]=gpio_file_open(GPIO_67_P8_8_SEGB);
		fdgpio[2]=gpio_file_open(GPIO_69_P8_9_SEGC);
		fdgpio[3]=gpio_file_open(GPIO_68_P8_10_DP);
		fdgpio[4]=gpio_file_open(GPIO_45_P8_11_SEGD);
		fdgpio[5]=gpio_file_open(GPIO_44_P8_12_SEGE);
		fdgpio[6]=gpio_file_open(GPIO_26_P8_14_SEGF);
		fdgpio[7]=gpio_file_open(GPIO_46_P8_16_SEGG);
#endif


    	/* argc is correct , lets check argv */
    	if (strcmp(argv[1], "up") == 0)
    	{
    		start_upcounting(value);
    	}
    	else if (strcmp(argv[1], "down") == 0)
    	{
    		start_downcounting(value);
    	}
    	else if (strcmp(argv[1], "updown") == 0)
    	{
    		start_updowncounting(value);
    	}
    	else if (strcmp(argv[1], "random") == 0)
    	{
    		start_randomcounting(value);
    	}
    	else /* default: */
    	{
    		printf("Invalid direction values\n");
    		printf( "valid direction values : up, down,updown,random\n");
    	}
	}
}
