//19/04/2021
#include "initsys.h"

#include "Isd4004Spi.h"
#include "fingerprint.h"
#include <math.h> 
float goertzel_mag(int numSamples,int TARGET_FREQUENCY,int SAMPLING_RATE);
int ADC_IN(int num);
int IsAlarmDetected(int numofinstans,int mag);
int IsAlarmDetected2();
int IsAlarmDetected_trial();
void open_window() ;
void close_window();
int ADC_IN(int num);

// constants for voice messages 
#define ALARM_DETECTED 0
#define GET_INSIDE_THE_ROOM 100
#define GET_OUT_OF_THE_ROOM 300
#define STAY_IN_THE_ROOM 200

// constant for servo motors 
#define OPEN_WINDOWS  2000 
#define CLOSE_WINDOWS 1000  


#define DELAY_FOR_STAY_IN_ROOM 5
void record_all_measeges();
void test_play_all();
void start_work();


#define NUM_OF_FREQ_TO_CHECK 2 
	
xdata int  NumInstans[NUM_OF_FREQ_TO_CHECK]; 
struct WaveProperty  
{
	 int freq; 
	 int Th_mag;
	 int Th_NumInstans;
};

//xdata struct WaveProperty wave_property_arr[NUM_OF_FREQ_TO_CHECK]={{700,70,8},{800,150,20},{900,60,9}};
//xdata struct WaveProperty wave_property_arr[NUM_OF_FREQ_TO_CHECK]={{700,70,8},{800,100,20},{900,60,9}};
xdata struct WaveProperty wave_property_arr[NUM_OF_FREQ_TO_CHECK]={{800,80,40},{900,60,30}};
//xdata struct WaveProperty wave_property_arr[NUM_OF_FREQ_TO_CHECK]={{800,100,20}};
//xdata struct WaveProperty wave_property_arr[NUM_OF_FREQ_TO_CHECK]={{800,150,20}};
xdata char my_string[20];
xdata U16 data_in[210];
xdata num=0;


extern  PageID;	

void Init_Device(void);

void MenuScreen();


void servo(unsigned int Width)  
{
	if(Width>2300) Width=2300;
	if(Width<700) Width=700;
	Width=-4*Width;
	
		PCA0CPL0= Width;
		PCA0CPH0= Width>>8;	
}
void servo2(unsigned int Width)  
{
	if(Width>2300) Width=2300;
	if(Width<700) Width=700;
	Width=-4*Width;
	
		PCA0CPL1= Width;
		PCA0CPH1= Width>>8;	
}


void main(void)
{
	S16 x=0, y=0,  ButtonNum;
	Init_Device();
	initSYS();  // initalize the touch LCD library + delay function 
  servo(OPEN_WINDOWS);
	servo2(OPEN_WINDOWS);
	MenuScreen();

	while(1) {
		if(!T_IRQ) {
			delay_ms(10);
			x = ReadTouchX();
			y = ReadTouchY();
			ButtonNum= ButtonTouch(x, y);
			if(ButtonNum==1){ 
				LCD_fillScreen(BLACK);		
				LCD_print2C(20,150,"Record1 ",3,WHITE,BLACK);				
				Record(0);
				LCD_print2C(20,150,"Play1 ",3,WHITE,BLACK);			
				PlayI(0);
				MenuScreen();
			}
			else if(ButtonNum==2){ 
				LCD_fillScreen(BLACK);		
				LCD_print2C(20,150,"Record2 ",3,WHITE,BLACK);				
				Record(100);
				LCD_print2C(20,150,"Play2 ",3,WHITE,BLACK);			
				PlayI(100);
				MenuScreen();
			}
			else if(ButtonNum==3){
				LCD_fillScreen(BLACK);		
				LCD_print2C(20,150,"Record3 ",3,WHITE,BLACK);				
				Record(200);
				LCD_print2C(20,150,"Play3  ",3,WHITE,BLACK);			
				PlayI(200);
				MenuScreen();		
			}
			else if(ButtonNum==4) {
				LCD_fillScreen(BLACK);		
				LCD_print2C(20,150,"Record4 ",3,WHITE,BLACK);				
				Record(300);
				LCD_print2C(20,150,"Play4 ",3,WHITE,BLACK);			
				PlayI(300);
				MenuScreen();
			}
			
			else if(ButtonNum==5) {
				LCD_fillScreen(BLACK);		
				LCD_print2C(20,150,"Record5 ",3,WHITE,BLACK);				
				Record(400);
				LCD_print2C(20,150,"Play5 ",3,WHITE,BLACK);			
			//	PlayI(400);
				MenuScreen();
			}
			else if(ButtonNum==6) {
				LCD_fillScreen(BLACK);		
				LCD_print2C(20,150,"SERVO ",3,WHITE,BLACK);	
				servo(1000);
				servo2(1000);
				delay_ms(2000);
				LCD_print2C(20,150,"SERVO ",3,WHITE,BLACK);		
				servo(1500);
				servo2(1500);
				delay_ms(2000);
				servo(2000);
				servo2(2000);
				delay_ms(2000);
				MenuScreen();
			}
			else if(ButtonNum==7) {
				LCD_fillScreen(BLACK);	
				LCD_print2C(20,150,"Blu  ",3,WHITE,BLACK);
        LCD_print2C(20,150,"Send SMS  ",3,WHITE,BLACK);				
				SetTarget(1); // before sending to BT switch printf to uart
				//printf("**0527033526");
				printf("**0527033526");
				delay_ms(1000);
				printf("##Go to safety room");
				delay_ms(1000);
				printf("$$");
				delay_ms(1000);
				printf("**0527033526");
				delay_ms(1000);
				printf("##caseft2");
				delay_ms(1000);
				printf("$$");
				
				MenuScreen();
			}
			
			else if(ButtonNum==8) {
//			  while(1)
//				{   
//					//int x;
//				   while (( IsAlarmDetected2()==0));
//					 x=6;
//				 }
				start_work();

//				printf("error not find  "); 
//				SetTarget(1); // uart
//				printf("**0587909902");
//				delay_ms(1000);
//				printf("##55555555555551");
//				delay_ms(1000);
//				printf("$$");
//					SetTarget(0); // uart
				}
        while(T_IRQ);	
				MenuScreen();
			}
				else if(ButtonNum==9) {
					PlayI(0);
					PlayI(100);
					PlayI(200);
					PlayI(300);
				//	PlayI(400);
		 }	
	 }
 }

 void MenuScreen() 
{
	LCD_fillScreen(BLACK);
	LCD_print2C(20,30,"Test Voicer",3,WHITE,BLACK);
	LCD_clearButton();	
	LCD_drawButton(1,10,  80,60,60, 10, WHITE,BLACK,"Rec1",2);
	LCD_drawButton(2,80, 80,60,60, 10, GRAY,BLACK,"Rec2",2);
	LCD_drawButton(3,150, 80,60,60, 10, WHITE,BLACK,"Rec3",2);
	LCD_drawButton(4,220, 80,60,60, 10, GRAY,BLACK,"Rec4",2);
	LCD_drawButton(5,290, 80,60,60, 10, WHITE,BLACK,"Rec5",2);
	LCD_drawButton(6,10, 200,60,60, 10, RED,WHITE,"SERVO",2);
	LCD_drawButton(7,80, 200,60,60, 10, BLUE,WHITE,"blu ",2);
	LCD_drawButton(8,150, 200,60,60, 10, YELLOW,BLACK,"finger ",2);
	LCD_drawButton(9,220, 200,60,60, 10, GREEN,BLACK,"Play All ",2);
  LCD_setText2Color(WHITE,BLACK);
}


//from previous project 
void start_work()
{
	  int i;
		LCD_fillScreen(BLACK);
	  LCD_print2C(20,30,"press for abort",1,WHITE,BLACK);
	  while (( IsAlarmDetected2()==0) && T_IRQ) ;//check alarm detected or abort 
	  if (!T_IRQ) return;
	  // reverse the data that is been sent to P3.7 - P3.4 to trigger VT at reciver side  
	  P2 = P2 ^ 0xF0 ;  
	  LCD_fillScreen(BLACK);
	  LCD_print2C(20,30,"alarm detected",1,WHITE,BLACK);
	  PlayI (ALARM_DETECTED);
		delay_ms(100);
	  // SEND SMS TO ALL CLIENTS 
				SetTarget(1); // uart
				printf("**0587909902");
				delay_ms(1000);
				printf("##RED COLOR ALARM");
				delay_ms(1000);
				printf("$$");
				SetTarget(0); // uart
  
	
	
	
	  LCD_print2C(20,60,"go to safe room",1,WHITE,BLACK);
		PlayI (GET_INSIDE_THE_ROOM);
		//close_window ;
	  servo(CLOSE_WINDOWS);
    servo2(CLOSE_WINDOWS)	;
		for(i=0;i<DELAY_FOR_STAY_IN_ROOM;i++)
		{
			 delay_ms(1);
       sprintf(my_string,"i=%d",i);	
       LCD_print2C(20,60,my_string,1,WHITE,BLACK);			
       if (IsAlarmDetected2()==1)
			 { 
				 i=0;
				 sprintf(my_string,"i=%d",i);
				 LCD_print2C(20,60,"redetect",1,WHITE,BLACK);	
				 PlayI(STAY_IN_THE_ROOM);
				 delay_ms(1000);
			 }
		} 
		//open_window
		servo(OPEN_WINDOWS);
		servo(OPEN_WINDOWS);
    servo2(OPEN_WINDOWS);	
		LCD_print2C(20,60,"get out of room",1,WHITE,BLACK);	
		PlayI(GET_OUT_OF_THE_ROOM);
}
		  	




void record_all_measeges()
{
	LCD_fillScreen(BLACK);
	LCD_printCenter(0, 10,"enter mes 0  0" , 2,RED, BLACK);
	Record(0);
	PlayI(0);
	//LCD_fillScreen(BLACK);
	LCD_printCenter(0, 40,"enter mes 1  100" , 2,RED, BLACK);
	Record(100);
	PlayI(100);
	//LCD_fillScreen(BLACK);
	LCD_printCenter(0, 80,"enter mes 2  200" , 2,RED, BLACK);
	Record(200);
	PlayI(200);
	//LCD_fillScreen(BLACK);
	LCD_printCenter(0, 120,"enter mes 3  300" , 2,RED, BLACK);
	Record(300);
	PlayI(300);
	

}
void test_play_all()
{
	LCD_fillScreen(BLACK);
	LCD_printCenter(0, 10,"play mes 0  0" , 2,RED, BLACK);
	PlayI(0);
	//LCD_fillScreen(BLACK);
	LCD_printCenter(0, 40,"play mes 1  100" , 2,RED, BLACK);
	PlayI(100);
	//LCD_fillScreen(BLACK);
	LCD_printCenter(0, 80,"play mes 2  200" , 2,RED, BLACK);
	PlayI(200);
	//LCD_fillScreen(BLACK);
	LCD_printCenter(0, 120,"play mes 3  300" , 2,RED, BLACK);
	PlayI(300);
}


#if 0
int IsAlarmDetected_trial()
{
	static int j=0;
	int y700,y1500,i,res;
	short max,min,innn,num=0 ;
  //while(1)	
	//innn=2*ADC_IN(6);	//P3.0 num=6
	max=0;
	min=0;
	//lcd_string_line(1,0,"freq=500     ");
  while(sw6)
	{	
	   
		 sprintf(my_string,"freq=%3d      ",j);	
		ScreenOneColor(White);
	  PutStringSize(0,200,"********(1) ",1,Blue);	
	 for(i=0;i<205;i++)   
		 {
				data_in[i]=2*ADC_IN(6);	//P3.0 num=6
				wait_us(112);//dt sample
	   }
		
    
		
		res=(int)(goertzel_mag(205,j,8000));	//1000hz
		if (res>max) 
		{
			max=res;
			num=1;
		}
		if (res==max) num++;
		if (res<min) min=res;
    //lcd_clr();
	  sprintf(my_string,"ma=%3d mx=%3d %2d",res,max,num);
		ScreenOneColor(White);
	  PutStringSize(0,200,"********(2) ",1,Blue);
		wait_ms(30);
		 if (!sw7) 
		 {
			 j=j+100;
			 ScreenOneColor(White);
			 wait_ms(200);
			 max=0;
			 min=0;
		 }
	}	
	
	sprintf(my_string,"max=%d min=%d",max,min);
	ScreenOneColor(White);
	  PutStringSize(0,200,"********(3) ",1,Blue);
/*
		max=data_in[0];
	min=data_in[0];
	for(i=1;i<205;i++)
  {
     if (data_in[i]<min) min=data_in[i];
     if (data_in[i]>max) max=data_in[i];
  }		
	for(i=0;i<2000;i+=100)
	{
		res=(int)(goertzel_mag(205,i,8000));	//1000hz
    lcd_clr();
	  sprintf(my_string," freq=%d mag=%d",i,res);
		ScreenOneColor(White);
	  PutStringSize(0,200,"********(1.1) ",1,Blue);
	}	
*/		
#if 0		
	  y700=(int)(goertzel_mag(205,700,8000));	//1000hz
		if (y700<MAG_FOR_FIRST_CALL_700HZ)
			return 0;
		wait_ms(DELAY_BETWEEN_700_AND_1500);
		for(i=0;i<205;i++)   
		{
				data_in[i]=2*ADC_IN(17);	//P0.3 num=17
				wait_us(112);//dt sample
	  }
		y1500=(int)(goertzel_mag(205,1500,8000));	//1500hz
		y700=(int)(goertzel_mag(205,700,8000));	//700hz      
    if ((y700<MAG_FOR_SECOND_CALL_700HZ) && (y1500>MAG_FOR_FIRST_1500HZ))
			return 1 ; 
		else 
			return 0;
#endif 
 return 1; 		
}


#endif 

// this function is been called for  detecting red sirne 
// the function return "1" if the alarm has been set otherwise return "0"
int IsAlarmDetected(int numofinstans,int mag)
{
	short y800=0,j,i,res,y700,y600;
	num=0;
  for(j=0;j<500;j++)
	{		
	  for(i=0;i<205;i++)   
		 { 
				data_in[i]=2*ADC_IN(6);	//P3.0 num=6
				delay_us(112);//dt sample
			  
	   }	
  
    y700=(int)(goertzel_mag(205,700,8000));	//1000hz
    y600=(int)(goertzel_mag(205,600,8000));	//1000hz		 
	  res=(int)(goertzel_mag(205,800,8000));	//1000hz
	  sprintf(my_string,"res=%3d num=%3d",res,num);
		LCD_fillScreen(WHITE);
		LCD_print2C(0,200,"********(4) ",1,WHITE,BLACK);
	  //PutStringSize(0,200,"********(4) ",1,Blue); 
		if ((res>y800) && (y700<20) && (y600<20)) y800=res;
		//if (res>mag) num++;
    if (y800>mag) num++;		 
		if ((y800>mag) && (num>numofinstans))
    {
        delay_ms(1000);
        break; 			
    }
			
	 }
	 if ((y800>mag) && (num>numofinstans))
		 return 1;
	 else 
		 return 0;
#if 0	 
		if (y800<MAG_FOR_FIRST_CALL_700HZ)
			continue
		wait_ms(DELAY_BETWEEN_700_AND_1500);
		for(i=0;i<205;i++)   
		{
				data_in[i]=2*ADC_IN(6);	//P3.0 num=6
				wait_us(112);//dt sample
	  }
		y1500=(int)(goertzel_mag(205,1500,8000));	//1500hz
		y700=(int)(goertzel_mag(205,700,8000));	//700hz      
    if ((y700<MAG_FOR_SECOND_CALL_700HZ) && (y1500>MAG_FOR_FIRST_1500HZ))
			return 1 ; 
		else 
			return 0;
#endif 	
}

int IsAlarmDetected2()
{
	int mag_temp,detect;
	int j,i;
	int k=0;
	num=0;
	LCD_fillScreen(BLACK);
	for(j=0;j<NUM_OF_FREQ_TO_CHECK;j++)
	{
		NumInstans[j] = 0;	
	}
  for(j=0;j<600/NUM_OF_FREQ_TO_CHECK;j++)
	{		
	  for(i=0;i<205;i++)   
		 { 
				data_in[i]=2*ADC_IN(21);	//P1.0 num=21
				delay_us(112);//dt sample
			  
	   }	

    for(i=0;i<NUM_OF_FREQ_TO_CHECK;i++)
		 {
			 mag_temp=(int)(goertzel_mag(205,wave_property_arr[i].freq,8000));
			 k++;	
	    //sprintf(my_string,"m=%3d f=%3d i=%d",mag_temp,wave_property_arr[i].freq,NumInstans[i]);
if ((k%20==0) || (k%20==1))
{
       
			LCD_setCursor (10, 100);
			if (i==0) 
			{  
				LCD_setCursor (10, 0);
				printf("m=%3d f=%3d i=%d",mag_temp,wave_property_arr[i].freq,NumInstans[i]);
			}

			if (i==1) 
			{  
				LCD_setCursor (10,100);
				printf("m=%3d f=%3d i=%d",mag_temp,wave_property_arr[i].freq,NumInstans[i]);
			}

			if (i==2) 
			{  
				LCD_setCursor (10, 200);
				printf("m=%3d f=%3d i=%d",mag_temp,wave_property_arr[i].freq,NumInstans[i]);
			}
		
} 			
  
			      
			 
		//	 if (i==1) LCD_print2C(20,100,my_string,2,RED, BLACK);
		//	 if (i==2) LCD_print2C(20,200,my_string,2,RED, BLACK);
			 
			 
//		   if (i==0) lcd_string_line(1,0,my_string);
//			 if (i==1) lcd_string_line(2,0,my_string);
//			 if (i==2) lcd_string_line(1,0,my_string);
//       wait_ms(50); 			 
			 if (mag_temp > wave_property_arr[i].Th_mag)
			 {
				 NumInstans[i]++;
			 }
		 }
 		 
	 }
	 
	 detect=1;
	 for(i=0;i<NUM_OF_FREQ_TO_CHECK;i++)
	 {
		 if (NumInstans[i] < wave_property_arr[i].Th_NumInstans)
		 {
        detect = 0 ;
        break;
		 }			 
	 }
	 return(detect);
}



float goertzel_mag(int numSamples,int TARGET_FREQUENCY,int SAMPLING_RATE)
{

	int     k,i;
	float analog;
	float   floatnumSamples;
	float   omega,sine,cosine,coeff,q0,q1,q2,magnitude,real,imag;
	float   scalingFactor = numSamples / 2.0;

	floatnumSamples = (float) numSamples;
	k = (int) (0.5 + ((floatnumSamples * TARGET_FREQUENCY) / SAMPLING_RATE));
	omega = (2.0 * 3.14 * k) / floatnumSamples;
	sine = sin(omega);
	cosine = cos(omega);
	coeff = 2.0 * cosine;
	q0=0;
	q1=0;
	q2=0;

	for(i=0; i<numSamples; i++)
	{
		analog=data_in[i];
		q0 = coeff * q1 - q2 + analog;
		q2 = q1;
		q1 = q0;
	}

	// calculate the real and imaginary results
	// scaling appropriately
	real = (q1 - q2 * cosine) / scalingFactor;
	imag = (q2 * sine) / scalingFactor;

	magnitude = sqrt(real*real + imag*imag);

	return magnitude;

}


// P2.0 num=0,P2.1 num=1,P2.2 num=2,P2.3 num=3,P2.5 num=4,P2.6 num=5,P3.0 num=6,P3.1 num=7,P3.4 num=8,P3.5 num=9,
// P3.7 num=10,P4.0 num=11,P4.3 num=12,P4.4 num=13,P4.5 num=14,P4.6 num=15
// P0.3 num=17, P0.4 num=18,P1.1 num=19, P1.2 num=20, P1.0 num=21, P1.3 num=22, P1.6 num=23, P1.7 num=24,
//  P2.4 num=25, P2.7 num=26, P3.2 num=27, P3.3 num=28, P3.6 num=29, P4.1 num=32, P4.2 num=33, P4.7 num=34
// Temp Sensor num=30, VDD   num=31

int ADC_IN(int num)
{
	AMX0N     = 0x1F;// negtive input= GND
	AMX0P=num;
	delay_us(1);
	AD0BUSY=1; // start conversion 
	while(!AD0INT); // wait end convesion 
	AD0INT = 0;   // clear ADC0 conversion complete flag 
	return (ADC0);
}






