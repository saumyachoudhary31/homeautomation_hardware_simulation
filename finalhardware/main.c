



#define F_CPU 100000UL
/*############################################################################################# */
	/*Initialising Headers*/
#include <avr/io.h> // standard avr header 

#include <string.h> // library used to manipulate array of characters

#include <util/delay.h> // delay header 
/*###############################################################################*/
	/*Defining ports */

#define lcdport PORTA//LCD data port

#define signal PORTB//LCD command port

#define en PB2 // LCD enable pin

#define rw PB1// LCD rw, read/write

#define rs PB0 //LCD rs, register select

/*############################################################################################# */
	/*Function Declarations */


void lcdcmd(unsigned char cmd);//  used in sending commands to LCD

void lcdint(); // Initialising LCD

void lcddata(unsigned char data); // used in sending data to LCD 

void lcd_print(char * str); // to print on LCD

void usart_init();// Initialising USART

void usart_user();// Function for receiving user id
void usart_tx(char x); // USART transmission
void usart_pass();// 
void usart_msg(char *c);//
void user_choice(); //  function to choose options
void usart_send(unsigned char);//
char usart_rx(void); // USART receive

void entry();// 
/*############################################################################################# */

unsigned char value;

unsigned char suser[15];// user id
unsigned char choice; // choice of number used in  switch
unsigned char spass[15]; // user password

unsigned char pass[]="1234"; //user id to open door

unsigned char user[]="passw"; // password to open door

//***************************************************************
void usart_init()

{

	UCSRB=(1<<TXEN)|(1<<RXEN); // transmit enable receive enable
	

	UCSRC=(1<<UMSEL)|(1<<UCSZ1);// synchoronous 7 bit

	UBRRL=0x33; //9600 baud rate

}

int main(void)

{

	usart_init();
	
	DDRA=0xff;//output LCD port
	DDRD = 0xff;// output port
	DDRB=0xff;// output port
	DDRC = DDRC | 0b00001111; 
	PORTD =		PORTD | 0b00010000;

	lcdint(); // LCD initialization

	while(1)

	{

		lcdcmd(0x01); // clear display screen

		lcdcmd(0x02); // return home

		lcdcmd(0x80); // force cursor to beginning of 1st line

		lcd_print("ENTER USER ID");// print on LCD
		usart_msg("ENTER USER ID");// print on USART terminal
		usart_tx(0x0d); //print in next line in  USART terminal
		_delay_ms(200);

		lcdcmd(0xc0);//force cursor to beginning of 2nd line

		usart_user();// Function of receiving user id

		entry();// function to display options after entering correct id
		
		
		

	}

}
//***********************************************************************
void entry()

{

	if ((strcmp(suser,user)==0))

	{

		lcdcmd(0x01); // clear display screen

		lcdcmd(0x02);// return home

		lcd_print("ID MATCHED");// print on LCD
		usart_tx(0x0d);
		usart_msg("ID MATCHED");// print on USART terminal

		_delay_ms(2000);

		lcdcmd(0x01);// clear display screen

		lcdcmd(0x02);// return home
		usart_tx(0x0d);//print in next line in  USART terminal
		usart_msg("ENTER PASSWORD");// print on LCD

		lcd_print("ENTER PASSWORD");// print on USART terminal
		usart_tx(0x0d);//print in next line in  USART terminal

		_delay_ms(200);

		lcdcmd(0xc0);//force cursor to beginning of 2nd line

		usart_pass();// function to receive  password

		if((strcmp(spass,pass)==0)) // comparing the predefined password with user entered password 

		{

			lcdcmd(0x01);// clear display screen

			lcdcmd(0x02);// return home

			lcd_print("ID MATCHED");// print on LCD

			_delay_ms(2000);

			lcdcmd(0x01);// clear display screen

			lcd_print("OPENING DOOR");// print on LCD DOOR OPENS as password and id  is matched

			
			_delay_ms(1000);


			_delay_ms(500);

			lcdcmd(0x01);// clear display screen
			usart_tx(0x0d); //print in next line in  USART terminal
			user_choice();
			_delay_ms(1000);

			
			
			

		}

		else

		{

			lcdcmd(0x01);// clear display screen

			lcdcmd(0x02); // return home

			lcd_print("WRONG ENTRY");// print on LCD wrong password 

			lcdcmd(0xc0); //force cursor to beginning of 2nd line

			lcd_print("TRY AGAIN");// print on LCD

			_delay_ms(5000);

		}

	}

	else

	{

		lcdcmd(0x01);// clear display screen

		lcdcmd(0x02); // return home

		lcd_print("INVALID USER");// print on LCD wrong user id

		lcdcmd(0xc0); //force cursor to beginning of 2nd line

		lcd_print("TRY AGAIN");// print on LCD

		_delay_ms(5000);

	}

}
//*************************************************************************
void lcdint() // intialising LCD

{
	_delay_ms(50);

	lcdcmd(0x38);//init. 2 line (5x7)matrix 8bit(PA0-PA7)

	_delay_ms(4);//wait

	lcdcmd(0x01); // clear display screen

	_delay_ms(4);//wait

	lcdcmd(0x0E);//Display on and Cursor blinking

	_delay_ms(4);//wait

}

//*********************************************************************************
void user_choice(){
	usart_msg("Enter Choice");// print on USART terminal
	
	usart_tx(0x0d); 
	usart_msg("PRESS 1 TO TURN LED ON");// option to turn led on
	
	usart_tx(0x0d);
	usart_msg("PRESS 2 TO TURN LED AND FAN ON");// option to turn led and fan on
	
	usart_tx(0x0d);
	usart_msg("PRESS 3 TO TURN FAN ON");// option to turn fan on
	
	usart_tx(0x0d);
	usart_msg("PRESS 4 TO TURN LED OFF");// option to turn led off
	
	usart_tx(0x0d);
	usart_msg("PRESS 5 TO TURN FAN OFF"); // option to turn fan off
	
	usart_tx(0x0d);
	usart_msg("PRESS 6 TO TURN OFF EVERYTHING");// turn off everything
	
	usart_tx(0x0d);
	usart_msg("AND LOCK DOOR");
	usart_tx(0x0d);
	
	
	choice =usart_rx();
	switch(choice){
		case '1':
			usart_tx(0x0d);
		
			PORTC = PORTC | 0b00000001;// turn led on which is connected to PC0
			usart_tx(0x0d);
			user_choice();
			_delay_ms(1000);
		
		case '2':
			usart_tx(0x0d);
			PORTC = PORTC | 0b00001101;// turn fan and led on
			usart_tx(0x0d);
			user_choice();
			_delay_ms(10000);
		
		case '3':
			usart_tx(0x0d);
		
			PORTC = PORTC | 0b00001100;// turn fan on
			usart_tx(0x0d);
			user_choice();
			_delay_ms(1000);
		case '4':
			usart_tx(0x0d);
			PORTC = PORTC & 0b11111110;// turn led off
			usart_tx(0x0d);
			user_choice();
			_delay_ms(1000);
		case '5':
			usart_tx(0x0d);
		
			PORTC = PORTC & 0b00000101; //turn fan off
			usart_tx(0x0d);
			user_choice();
			_delay_ms(1000);
		case '6':
			usart_tx(0x0d);
		
			PORTC = PORTC & 0b00000100; //turn everything off and close door
			usart_tx(0x0d);
		
			_delay_ms(1000);
			lcd_print("CLOSING DOOR");// print on LCD
			_delay_ms(10000);
			main();
			break;
		
		
		
		
	}
	
	

}

//***********************************************************************************************************
void lcdcmd(unsigned char x)

{

	lcdport=x; // send command to data port

	signal|=(0<<rs)|(0<<rw)|(1<<en);// rs=0 for command, rw=0 for write , en=1 for high to low  pulse

	_delay_ms(4);// wait to make enable wide

	signal&=~((1<<rs)|(1<<rw)|(1<<en));// en=0 for H to L pulse

	_delay_ms(50);// wait to make enable wide

}

//**************************************************************************************************************
void lcddata(unsigned char data)

{

	lcdport= data; // send data to data port

	signal|=(1<<rs)|(0<<rw)|(1<<en);// rs=1 for data, rw=0 for write, en=1 for H to L pulse

	_delay_ms(4);// wait to make enable wide

	signal&=~((0<<rs)|(1<<rw)|(1<<en));//en=0 for H to L pulse

	_delay_ms(50);// wait to make enable wide

}
//**************************************************************************************************************
void lcd_print(char * str)

{

	unsigned char i = 0;

	while(str[i]!=0)

	{

		lcddata(str[i]);

		i++;

	}

}
//**************************************************************************************************************
void usart_user()

{

	int k;

	for(k=0;k<5;k++)// as we have taken user id of 5 letters

	{

		while(!(UCSRA & (1<<RXC)));//--- Check whether Receive is complete

		value=UDR;					//--- Return Data from UDR

		suser[k]=value;			// storing user id in suser

		

		lcddata(value);

		_delay_ms(500);

	}

}

//**************************************************************************************************************
void usart_pass()

{

	int r;

	for(r=0;r<4;r++) // as we have taken password of 4 letters

	{

		while(!(UCSRA & (1<<RXC)));//--- Check whether Receive is complete

		value=UDR;					//--- Return Data from UDR

		spass[r]=value;			// storing password in spass

		lcddata('*');

		_delay_ms(500);

	}

}

//**************************************************************************************************************
void usart_send(unsigned char ch)

{

	while(!(UCSRA & (1<<UDRE))); //wait until UDR is empty

	UDR=ch;

}

//**************************************************************************************************************
void usart_tx(char x)
{
	UDR = x;							//--- Move data to UDR Data Reg
	while (!(UCSRA & (1<<UDRE)));		//--- Check whether UDR is empty
}

//**************************************************************************************************************
void usart_msg(char *c)
{
	while (*c != 0)						//--- Check for null
	usart_tx(*c++);
}

//**************************************************************************************************************
char usart_rx()
{
	while(!(UCSRA & (1<<RXC)));		//--- Check whether Receive is complete
	choice = UDR;						//--- Return Data from UDR
}