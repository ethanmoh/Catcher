#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include "address_map_arm_brl4.h"

#define HW_REGS_BASE ( 0xff200000 )
#define HW_REGS_SPAN ( 0x00200000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define LED_PIO_BASE 0x0
#define SW_PIO_BASE 0x40

volatile sig_atomic_t stop;

void catchSIGINT(int signum){
    stop = 1;
}

void VGA_text (int, int, char *);
void VGA_box (int, int, int, int, short);
void VGA_line(int, int, int, int, short) ;

// virtual to real address pointers

volatile unsigned int * red_LED_ptr = NULL ;
void *h2p_lw_virtual_base;

volatile unsigned int * vga_pixel_ptr = NULL ;
void *vga_pixel_virtual_base;

volatile unsigned int * vga_char_ptr = NULL ;
void *vga_char_virtual_base;

int fd;

int main(void)
{
    volatile unsigned int *h2p_lw_led_addr=NULL;
    volatile unsigned int *h2p_lw_sw_addr=NULL;
    void *virtual_base;
    int fd;
    
    // catch SIGINT from ctrl+c, instead of having it abruptly close this program
    signal(SIGINT, catchSIGINT);
    
    // Open /dev/mem
    if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
        printf( "ERROR: could not open \"/dev/mem\"...\n" );
        return( 1 );
    }
    
    // get virtual addr that maps to physical
    virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );    
    if( virtual_base == MAP_FAILED ) {
        printf( "ERROR: mmap() failed...\n" );
        close( fd );
        return(1);
    }
    
    // Get the address that maps to the LEDs
    h2p_lw_led_addr=(unsigned int *)(virtual_base + (( LED_PIO_BASE ) & ( HW_REGS_MASK ) ));
    h2p_lw_sw_addr=(unsigned int *)(virtual_base + (( SW_PIO_BASE ) & ( HW_REGS_MASK ) ));
    
    printf("Running leds. To exit, press Ctrl+C.\n");

            // Declare volatile pointers to I/O registers (volatile         // means that IO load and store instructions will be used       // to access these pointer locations,
        // instead of regular memory loads and stores)


        // === need to mmap: =======================
        // FPGA_CHAR_BASE
        // FPGA_ONCHIP_BASE
        // HW_REGS_BASE

        // === get FPGA addresses ==================
    // Open /dev/mem
        if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 )    {
                printf( "ERROR: could not open \"/dev/mem\"...\n" );
                return( 1 );
        }

    // get virtual addr that maps to physical
        h2p_lw_virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );
        if( h2p_lw_virtual_base == MAP_FAILED ) {
                printf( "ERROR: mmap1() failed...\n" );
                close( fd );
                return(1);
        }

    // Get the address that maps to the FPGA LED control
        red_LED_ptr =(unsigned int *)(h2p_lw_virtual_base + LEDR_BASE);

        // === get VGA char addr =====================
        // get virtual addr that maps to physical
        vga_char_virtual_base = mmap( NULL, FPGA_CHAR_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, FPGA_CHAR_BASE );
        if( vga_char_virtual_base == MAP_FAILED ) {
                printf( "ERROR: mmap2() failed...\n" );
                close( fd );
                return(1);
        }

    // Get the address that maps to the FPGA LED control
        vga_char_ptr =(unsigned int *)(vga_char_virtual_base);

        // === get VGA pixel addr ====================
        // get virtual addr that maps to physical
        vga_pixel_virtual_base = mmap( NULL, FPGA_ONCHIP_SPAN, (PROT_READ| PROT_WRITE ), MAP_SHARED, fd,FPGA_ONCHIP_BASE);
        if( vga_pixel_virtual_base == MAP_FAILED ) {
                printf( "ERROR: mmap3() failed...\n" );
                close( fd );
                return(1);
        }

    // Get the address that maps to the FPGA LED control
        vga_pixel_ptr =(unsigned int *)(vga_pixel_virtual_base);

        // ===========================================

        /* create a message to be displayed on the VGA
          and LCD displays */
        char text_top_row[40] = "Altera DE1-SoC\0";
        char text_bottom_row[40] = "ECE 417 Final Project - Catcher\0";
	

        VGA_text (2, 1, text_top_row);
        VGA_text (2, 2, text_bottom_row);
        // clear the screen
        VGA_box (0, 0, 319, 239, 0);
        // x1 y1 x2 y2  320x240
        //VGA_box (33*4, 28*4, 49*4, 32*4, 0x187F);
        VGA_box (200, 210, 250, 215, 0x07FF);
        VGA_line(0, 0, 0, 200, 0xf000) ;
        VGA_line(319,200,319,0,0xf000);
        VGA_line(319,0,0,0,0xf000);

        // just a dot in the upper left corner
        //*vga_pixel_ptr = 0xffff ;
        short color ;
        int x1,y1,x2,y2,xb,yb;
        x1=200;
        x2=250;
        xb = rand() % 300;
        yb = rand() % 200;
        int xbf, ybf;
        xbf = xb +2;
        ybf = yb +2;

	int score;
	score = 0;

	int v;
	v = 2;
	int pad;
	pad = 2;
    
    while(1){
        *h2p_lw_led_addr = *h2p_lw_sw_addr;

                //*(red_LED_ptr) = 0x1;         // turn on LEDR_0
                VGA_box (xb, yb, xbf, ybf, 0xf000);
                //*(red_LED_ptr) = 0x0;         // turn off LEDR_0
                //VGA_box (100, 100, 102, 102, color++);

                usleep(30000);
                if((*h2p_lw_led_addr == 0 || *h2p_lw_led_addr == 2) && x1>0)
                {
                        VGA_box(x1,210,x2,215,0);
                        x1-=pad;
                        x2-=pad;
                        VGA_box(x1,210,x2,215,0x07FF);
                        //sleep(1);
                }
		else if((*h2p_lw_led_addr == 1 || *h2p_lw_led_addr == 3) && x2 <319)
		{
			VGA_box(x1,210,x2,215,0);
			x1+=pad;
			x2+=pad;
			VGA_box(x1,210,x2,215,0x07FF);
		}

                VGA_box(xb,yb,xbf,ybf,0);
                //yb++;
                //ybf++;

                if(ybf >= 210 && ybf <= 215){
			if(xb>= x1 && xbf <= x2)
			{
			//add score, respawn ball
			score++;
			printf("The score is %d \n",score);
			VGA_box(xb,yb,xbf,ybf,0);
			xb = rand() % 300;
			yb = rand() % 200;
			xbf = xb + 2;
			ybf = yb + 2;
			}
		}
		else if(yb >= 220)
                        break;
		
		if(*h2p_lw_led_addr > 1){
			v = 3;
			pad = 4;
		}
		else{
			v = 2;
			pad = 2;
		}

		yb+=v;
		ybf+=v;

    }
    
    if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
        printf( "ERROR: munmap() failed...\n" );
        close( fd );
        return( 1 );

    }
    close( fd );
    return 0;
}


/****************************************************************************************
 * Subroutine to send a string of text to the VGA monitor
****************************************************************************************/
void VGA_text(int x, int y, char * text_ptr)
{
        int offset;
        volatile char * character_buffer = (char *) vga_char_ptr ;      // VGA character buffer

        /* assume that the text string fits on one line */
        offset = (y << 7) + x;
        while ( *(text_ptr) )
        {
                // write to the character buffer
                *(character_buffer + offset) = *(text_ptr);
                ++text_ptr;
                ++offset;
        }
}

/****************************************************************************************
 * Draw a filled rectangle on the VGA monitor
****************************************************************************************/
void VGA_box(int x1, int y1, int x2, int y2, short pixel_color)
{
        int *pixel_ptr, row, col;

        /* assume that the box coordinates are valid */
        for (row = y1; row <= y2; row++)
                for (col = x1; col <= x2; ++col)
                {
                        pixel_ptr = (char *)vga_pixel_ptr + (row << 10)    + (col << 1);
                        // set pixel color
                        *(short *)pixel_ptr = pixel_color;
                }
}

// =============================================
// === Draw a line
// =============================================
//plot a line
//at x1,y1 to x2,y2 with color
//Code is from David Rodgers,
//"Procedural Elements of Computer Graphics",1985
void VGA_line(int x1, int y1, int x2, int y2, short c) {
        int e;
        signed int dx,dy,j, temp;
        signed int s1,s2, xchange;
     signed int x,y;
        int *pixel_ptr ;

        x = x1;
        y = y1;

        //take absolute value
        if (x2 < x1) {
                dx = x1 - x2;
                s1 = -1;
        }

        else if (x2 == x1) {
                dx = 0;
                s1 = 0;
        }

        else {
                dx = x2 - x1;
                s1 = 1;
        }

        if (y2 < y1) {
                dy = y1 - y2;
                s2 = -1;
        }

        else if (y2 == y1) {
                dy = 0;
                s2 = 0;
        }

        else {
                dy = y2 - y1;
                s2 = 1;
        }

        xchange = 0;

        if (dy>dx) {
                temp = dx;
                dx = dy;
                dy = temp;
                xchange = 1;
        }

        e = ((int)dy<<1) - dx;

        for (j=0; j<=dx; j++) {
                //video_pt(x,y,c);
                pixel_ptr = (char *)vga_pixel_ptr + (y<<10)+ (x<< 1);
                // set pixel color
                *(short *)pixel_ptr = c;

                if (e>=0) {
                        if (xchange==1) x = x + s1;
                        else y = y + s2;
                        e = e - ((int)dx<<1);
                }

                if (xchange==1) y = y + s2;
                else x = x + s1;

                e = e + ((int)dy<<1);
        }
}


