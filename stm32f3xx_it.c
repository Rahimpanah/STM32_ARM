/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32f3xx.h"
#include "stm32f3xx_it.h"

/* USER CODE BEGIN 0 */
#include "LiquidCrystal.h"
#include "stdlib.h"
int Level=0;
int Time=0;
int Speed=0;
int TotalTime=0;
int PlaceCar;
int Score=0;
int Heart=8;
int GameState=-1;
int Numbersegment=0;
int second=0;
int Numbersegment0=0;
int Numbersegment1=0;
int Numbersegment2=0;
int Numbersegment3=0;
char B[200];
int Map[40][18];
int SelectRow[30];
int downcount=1;
char a[100];
int DownTime=1;
int Mane=0;
int Lock=0;
int one=0;
int two=0;
int three=0;
int SevenSegmentTimer=0;
int repeatLevel=0;
int TimeCollision=0;
int Turbo=0;
int NameTimer=0;
int LastTurboSecond=0;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim17;
extern RTC_HandleTypeDef hrtc;
extern unsigned char str[100];
extern unsigned char x1;
int array[20];
int lastTurboDecharge=0;
int blink7seg=1;
const int KEYPAD_ROWS = 4;
int keypadConsecutiveHit = 0;
int isKeyPadEnabled = 0;
int keypadTimer = 0;
int inputTries = -1;
int keypadCol;
int keypadRow;
int lastKeypadCol=-1;
int lastKeypadRow=-1;
const int NUMBER_OF_RANDOM_GENERATION = 10;
char inputNumbers[NUMBER_OF_RANDOM_GENERATION];
int currentBufferSize = 0;
int isTimeSet = 0;
int saveGameFlag = 0;
int uartReceiveSw=0;
int enemyListX[20];
int enemyListY[20];
int enemyListXSize=0;
int enemyListYSize=0;
void ScreenClear();
void clearEnemy(){
	for(int i=0;i<18;i++){
			if(Map[downcount-1][i] == 1){
				
				if(i<2){
					setCursor(2,2);
				}
				else if(i>=17){
					setCursor(17,2);
				}
				else{
					setCursor(i+1,2);
				}
				print(" ");
			}
			if(Map[downcount][i] == 1){
				if(i<3){
					setCursor(3,1);
				}
				else if(i>=16){
					setCursor(16,1);
				}
				else{
					setCursor(i+1,1);
				}
				print(" ");
			}
			if(Map[downcount+1][i] == 1){
				if(i<4){
					setCursor(4,0);
				}
				else if(i>=15){
					setCursor(15,0);
				}
				else{
					setCursor(i+1,0);
				}
				print(" ");
			}
		}
}
void showEnemy(){
	for(int i=0;i<18;i++){
			if(Map[downcount-1][i] == 1){
				
				if(i<2){
					setCursor(2,2);
				}
				else if(i>=17){
					setCursor(17,2);
				}
				else{
					setCursor(i+1,2);
				}
				write((3));
			}
			if(Map[downcount][i] == 1){
				if(i<3){
					setCursor(3,1);
				}
				else if(i>=16){
					setCursor(16,1);
				}
				else{
					setCursor(i+1,1);
				}
				write((3));
			}
			if(Map[downcount+1][i] == 1){
				if(i<4){
					setCursor(4,0);
				}
				else if(i>=15){
					setCursor(15,0);
				}
				else{
					setCursor(i+1,0);
				}
				write((3));
			}
		}
}
void setPlaceCar(){
	int rnd=rand()%18;
	setCursor(rnd+1,3);
	write((6));//print("*");
	PlaceCar=rnd;
}
void GameMap(int insertCar){
	for(int i=0;i<4;i++){
		setCursor(16+i,i);
		write((0));
		setCursor(3-i,i);
		print("/");
	}
	for (int j=1;j<19;j++){
		setCursor(j,3);
		print("_");
	}
	if(insertCar) setPlaceCar();
	else {
		setCursor(PlaceCar+1,3);
		write((6));//print("*");
	}
	setCursor(0,0);
	char a[10];
	char x=7;
	sprintf(a,"%c:%d",x,Heart);
	print(a);
	setCursor(17,0);
	char b[10];
	sprintf(b,"%d",Score);
	print(b);
}
void LevelBegin(int level){
	Level=level;
	if(Level==9){
		if(repeatLevel==1){
			Level=8;
		}
		else{
			blink7seg=1;
			GameState=4; //payam you won contiue?
			ScreenClear();
			return;
		}
	}
	int steps=15/(1.5-(1.0/7*(Level-1)));
	  DownTime=(1.5-(1.0/7*(Level-1)))*200; // time payin amadan maneha dar har level
		for(int i=0;i<30;i++){     
			SelectRow[i]=0;
		}
		SelectRow[4]=1;   //farz kardim hatman dar avalin satr yek mane bashad
		 Mane=Level+1;   
		while(Mane!=0){
			int x=(rand()%(steps-7))+5; // gharar dadan mane dar satr ha be soorat random 
			if(SelectRow[x]!=1){   //barsi khali boodan mahal entekhab shode
				SelectRow[x]=1;
				Mane--;
			}
		}
		for(int i=0;i<40;i++){
			for(int j=0;j<18;j++){
				Map[i][j]=0;
			}
		}
		for(int i=0;i<30;i++){
			if(SelectRow[i]==1){
				int rnd=rand()%18;
				Map[i][rnd]=1;
			}
		}
		Time=0;
		downcount=1;
}
void printZero(){
	   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
     HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
	 	 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);
}

void printOne() {
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);
}

void printTwo() {
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);		
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);
}

void printThree() {
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);
}

void printFour(){
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);
}

void printFive(){
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);
}

void printSix() {
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);
}

void printSeven() {
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);
}

void printEight() {
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);
}

void printNine() {
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);
}
void printNumberOnSegment(int x) {

	switch(x){
		case 0:
			printZero();
			break;
		case 1: 
			printOne();
			break;
		case 2:
			printTwo();
			break;
		case 3:
			printThree();
		break;
		case 4:
			printFour();
		break;
		case 5:
			printFive();
		break;
		case 6:
			printSix();
		break;
		case 7:
			printSeven();
		break;
		case 8:
			printEight();
		break;
		case 9:
			printNine();
		break;
		
	}
}

void Refresh7Seg(){
	SevenSegmentTimer++;
	Numbersegment=SevenSegmentTimer%4;
	second=TotalTime/200;
	Numbersegment0=second/600;
	Numbersegment1=(second/60)%10;
	Numbersegment2=(second%60)/10;
	Numbersegment3=second%10;
	if(blink7seg && (SevenSegmentTimer/60)%2==0) Numbersegment=4;
	switch(Numbersegment){
			case 0:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
				printNumberOnSegment(Numbersegment0);
				break;
			case 1:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
				printNumberOnSegment(Numbersegment1);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
				break;
			case 2:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
				printNumberOnSegment(Numbersegment2);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
				break;
			case 3:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
				printNumberOnSegment(Numbersegment3);
				break;
			case 4:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
		}
}

void ScreenClear(){
	for(int j=0;j<4;j++){
		setCursor(0,j);
		Refresh7Seg();
		for(int i=0;i<20;i++){
			print(" ");
			Refresh7Seg();
		}
	}
}
const char KEYPAD_CHARS[4][4][4] ={		{	{'1', '2', '3', '*'},
																				{'4', '5', '6', 'N'},
																				{'7', '8', '9', 'N'},
																				{'N', 'N', 'N', 'N'}},

																			{	{'0', 'a', 'd', '*'},
																				{'g', 'j', 'm', 'N'},
																				{'p', 't', 'w', 'N'},
																				{'N', 'N', 'N', 'N'}},
																			
																			{	{'1', 'b', 'e', '*'},
																				{'h', 'k', 'n', 'N'},
																				{'q', 'u', 'x', 'N'},
																				{'N', 'N', 'N', 'N'}},
																			
																			{	{'0', 'c', 'f', '*'},
																				{'i', 'l', 'o', 'N'},
																				{'r', 'v', 'y', 'N'},
																				{'N', 'N', 'N', 'N'}},}; 
// (call by ref)
char readKeyPad() {
	keypadRow = -1;
	keypadCol = -1;
	
	for (keypadRow =0; keypadRow < KEYPAD_ROWS; keypadRow++){
			keypadCol = -1;
			
			switch (keypadRow) {
				case 0:
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
					break;
				case 1:
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
					break;
				case 2:
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
					break;
				case 3:
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
					break;
			}
			
			if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)) {
				keypadCol = 0;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)) ;
				break;
			}
			
			if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)) {
				keypadCol = 1;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)) ;
				break;
			}
			
			if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)) {
				keypadCol =2;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)) ;
				break;
			}
			
			if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)) {
				keypadCol =3;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)) ;
				break;
			}
		}
	
	return KEYPAD_CHARS[keypadConsecutiveHit][keypadRow][keypadCol];
}
// enables the keypad
void enableKeyPad() {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
		isKeyPadEnabled = 1;
}
// disables the keypad
void disableKeyPad() {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
		isKeyPadEnabled = 0;
}
void setKeypadTimer() {
	keypadTimer = 300;
}
void decrementKeypadTimer() {
	if(keypadTimer >= 0) {
		keypadTimer--;
	}
}

void save() {
	printf("upid\n%s\n" , inputNumbers);        //upid
  printf("score\n%d\n", Score);          //score
  printf("heart\n%d\n", Heart);          //heart
  printf("level\n%d\n", Level+repeatLevel);          //level
  printf("turbo\n%d\n", Turbo);        //turbo
  printf("speed\n%f\n", (DownTime*5)/1000.0);          //speed
  printf("gsec\n%d\n", second);          //gsec
	RTC_TimeTypeDef x;
	RTC_DateTypeDef d;
	HAL_RTC_GetTime(&hrtc,&x,RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc,&d,RTC_FORMAT_BCD);
	char time[20],date[20],car[20],enemy[20];
	sprintf(time,"%2d%2d%2d",x.Hours,x.Minutes,x.Seconds);
	sprintf(date,"%2d%2d%2d",d.Year,d.Month,d.Date);
	for(int i=0;time[i]!='\0';i++) if(time[i]==' ') time[i]='0';
	for(int i=0;date[i]!='\0';i++) if(date[i]==' ') date[i]='0';
  printf("date\n%s\n", date);      //date
  printf("time\n%s\n", time);      //time
	sprintf(car,"03%2d",PlaceCar+1);
  printf("mcp\n%s\n", car);        //mcp
	printf("ecp\n");    //ecp
	for(int i=0;i<3;i++){
		for(int j=0;j<18;j++){
			if(Map[downcount+i][j]==1){
				sprintf(enemy,"%2d%2d",2-i,j);
				for(int k=0;enemy[k]!='\0';k++) if(enemy[k]==' ') enemy[k]='0';
				printf("%s",enemy);
			}
		}
	}
  printf("\n");
  printf("save\n");
}

void load(){
	inputNumbers[inputTries]=0;
	printf("load\n%s\n",inputNumbers);
	
}

void Menu (){
	setCursor(4,0);
	print("1:NEW GAME");
	setCursor(4,1);
	print("2:LOAD GAME");
	setCursor(4,2);
	print("3:About");
}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart1;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI line0 interrupt.
*/
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
	if(GameState==-1){
			GameState=6;
			blink7seg=0;
		//ScreenClear();
		}
		else if(GameState==4){
			GameState=5;
		}
  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
* @brief This function handles ADC1 and ADC2 interrupts.
*/
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */
	if(Lock){
			HAL_ADC_IRQHandler(&hadc1);
			HAL_ADC_Start_IT(&hadc1);
			return;
	}

	if(GameState!=2){
		HAL_ADC_IRQHandler(&hadc1);
		HAL_ADC_Start_IT(&hadc1);
		return;
	}
		Lock=1;
//	HAL_ADC_Start_IT(&hadc1);
		int rot = HAL_ADC_GetValue(&hadc1);
		if(0<=rot && rot<3){
			if(PlaceCar!=0){
				setCursor(1+PlaceCar,3);
				print("_");
				setCursor(1,3);
				write((6));//print("*");
				PlaceCar=0;
			}
		}
		else if(3<=rot && rot<7){
			if(PlaceCar!=1){
				setCursor(1+PlaceCar,3);
				print("_");
				setCursor(2,3);
				write((6));//print("*");
				PlaceCar=1;
			}
		}
		else if(7<=rot && rot<10){
			if(PlaceCar!=2){
				setCursor(1+PlaceCar,3);
				print("_");
				setCursor(3,3);
				write((6));//print("*");
				PlaceCar=2;
			}
		}
		else if(10<=rot && rot<14){
			if(PlaceCar!=3){
				setCursor(1+PlaceCar,3);
				print("_");
				setCursor(4,3);
				write((6));//print("*");
				PlaceCar=3;
			}
		}
		else if(14<=rot && rot<17){
			if(PlaceCar!=4){
				setCursor(1+PlaceCar,3);
				print("_");
				setCursor(5,3);
				write((6));//print("*");
				PlaceCar=4;
			}
		}
		else if(17<=rot && rot<21){
			if(PlaceCar!=5){
				setCursor(1+PlaceCar,3);
				print("_");
				setCursor(6,3);
				write((6));//print("*");
				PlaceCar=5;
			}
		}
		else if(21<=rot && rot<24){
			if(PlaceCar!=6){
				setCursor(1+PlaceCar,3);
				print("_");
				setCursor(7,3);
				write((6));//print("*");
				PlaceCar=6;
			}
		}
		else if(24<=rot && rot<28){
			if(PlaceCar!=7){
				setCursor(1+PlaceCar,3);
				print("_");
				setCursor(8,3);
				write((6));//print("*");
				PlaceCar=7;
			}
		}
		else if(28<=rot && rot<31){
			if(PlaceCar!=8){
				setCursor(1+PlaceCar,3);
				print("_");
				setCursor(9,3);
				write((6));//print("*");
				PlaceCar=8;
			}
		}	
		else if(31<=rot && rot<35){
			if(PlaceCar!=9){
				setCursor(1+PlaceCar,3);
				print("_");
				setCursor(10,3);
				write((6));//print("*");
				PlaceCar=9;
			}
		}	
		else if(35<=rot && rot<38){
				if(PlaceCar!=10){
					setCursor(1+PlaceCar,3);
					print("_");
					setCursor(11,3);
					write((6));//print("*");
					PlaceCar=10;
				}
		}	
		else if(38<=rot && rot<42){
				if(PlaceCar!=11){
					setCursor(1+PlaceCar,3);
					print("_");
					setCursor(12,3);
					write((6));//print("*");
					PlaceCar=11;
				}
		}
		else if(42<=rot && rot<45){
				if(PlaceCar!=12){
					setCursor(1+PlaceCar,3);
					print("_");
					setCursor(13,3);
					write((6));//print("*");
					PlaceCar=12;
				}
		}	
		else if(45<=rot && rot<49){
				if(PlaceCar!=13){
					setCursor(1+PlaceCar,3);
					print("_");
					setCursor(14,3);
					write((6));//print("*");
					PlaceCar=13;
				}
		}	
		else if(49<=rot && rot<52){
				if(PlaceCar!=14){
					setCursor(1+PlaceCar,3);
					print("_");
					setCursor(15,3);
					write((6));//print("*");
					PlaceCar=14;
				}
		}
			else if(52<=rot && rot<56){
				if(PlaceCar!=15){
					setCursor(1+PlaceCar,3);
					print("_");
					setCursor(16,3);
					write((6));//print("*");
					PlaceCar=15;
				}
		}	
			else if(56<=rot && rot<59){
				if(PlaceCar!=16){
					setCursor(1+PlaceCar,3);
					print("_");
					setCursor(17,3);
					write((6));//print("*");
					PlaceCar=16;
				}
		}
		else{
			if(PlaceCar!=17){
					setCursor(1+PlaceCar,3);
					print("_");
					setCursor(18,3);
					write((6));//print("*");
					PlaceCar=17;
			}
		}
		Lock=0;
  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */
		
		HAL_ADC_Start_IT(&hadc1);
		
		
  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
* @brief This function handles TIM4 global interrupt.
*/
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
	
	decrementKeypadTimer();
	if(GameState==2){
		Time++;
		TotalTime++;
		Speed++;
	}
	Refresh7Seg();
	if(Lock){
		HAL_TIM_IRQHandler(&htim4);
		return;
	}
	Lock=1;
	NameTimer++;
	if(GameState==6){
		ScreenClear();
		Menu();
		GameState=7;
	}
	if (GameState == 12) {
		setCursor(2,1);
		print("Enter Upload ID");
		GameState = 13;
	}
	
	if(GameState==7){
		readKeyPad();
		if(keypadCol==0 && keypadRow==0){
			ScreenClear();
			GameState=0;
		}
		if(keypadCol==1 && keypadRow==0){
		  ScreenClear();
			setCursor(2,1);
			print("Enter Upload ID");
			GameState=9;
		}
		if(keypadCol==2 && keypadRow==0){
			ScreenClear();
			setCursor(0,0);
			print("Name:Death Race");
			setCursor(5,1);
			print("Developers");
			setCursor(0,2);
			print("Reyhaneh Rahimpanah");
			setCursor(0,3);
			print("Navid Alipoor");
		}
	}
	
	if(GameState==-1){
		if(NameTimer<200){
			HAL_TIM_IRQHandler(&htim4);
			Lock=0;
			return;
		}
		if(NameTimer%200==0 && isTimeSet==0){
			printf("gettime\n");
		}
		if(NameTimer%80==19){
			setCursor(5,0);
			Refresh7Seg();
			print("D");
			Refresh7Seg();
			print("e");
			Refresh7Seg();
			print("a");
			Refresh7Seg();
			print("t");
			Refresh7Seg();
			print("h");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print("R");
			Refresh7Seg();
			print("a");
			Refresh7Seg();
			print("c");
			Refresh7Seg();
			print("e");
			Refresh7Seg();
			
			setCursor(6,1);
			Refresh7Seg();
			print("R");
			Refresh7Seg();
			print("e");
			Refresh7Seg();
			print("y");
			Refresh7Seg();
			print("h");
			Refresh7Seg();
			print("a");
			Refresh7Seg();
			print("n");
			Refresh7Seg();
			print("e");
			Refresh7Seg();
			
			
			setCursor(7,2);
			Refresh7Seg();
			print("N");
			Refresh7Seg();
			print("a");
			Refresh7Seg();
			print("v");
			Refresh7Seg();
			print("i");
			Refresh7Seg();
			print("d");
			Refresh7Seg();
			
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,99);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,99);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,99);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,99);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,99);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,99);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,99);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,99);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,99);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,99);
			
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,99);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,99);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,99);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,99);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,99);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,99);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,99);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,99);
		}
		else if(NameTimer%80==79){
			setCursor(5,0);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			
			setCursor(6,1);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			
			setCursor(7,2);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,0);
			
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,0);
		}
		HAL_TIM_IRQHandler(&htim4);
		Lock=0;
		return;
	}
	
	if(GameState==0){
		setCursor(6,1);
		print("       ");
		setCursor(7,2);
		print("     ");
		setCursor(5,0);
		print("          ");
		if(one==0){
			for(one=0;one<16;one++){
				//print("1")
				setCursor(9,0);
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				setCursor(9,1);
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				setCursor(9,2);
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				setCursor(9,3);
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				setCursor(8,0);
				Refresh7Seg();
				write((2));
				Refresh7Seg();
			}
			one=15;
			
			setCursor(9,0);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			setCursor(9,1);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			setCursor(9,2);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			setCursor(9,3);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			setCursor(8,0);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			
		} 
		if(one==15&&two==0){
				for(two=0;two<16;two++){
			//print("2");
				setCursor(8,0);
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				setCursor(8,1);
				Refresh7Seg();
				write((4));
				Refresh7Seg();
				write((4));
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				setCursor(8,2);
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				write((5));
				Refresh7Seg();
				write((5));
				Refresh7Seg();
				setCursor(8,3);
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				write((1));
				Refresh7Seg();
			}
			two=15;
			setCursor(8,0);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			setCursor(8,1);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			setCursor(8,2);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			setCursor(8,3);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
		}
		if(one==15&&two==15&&three==0){
			for(three=0;three<16;three++){
				//print("3");
				setCursor(8,0);
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				setCursor(8,1);
				Refresh7Seg();
				write((4));
				Refresh7Seg();
				write((4));
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				setCursor(8,2);
				Refresh7Seg();
				write((5));
				Refresh7Seg();
				write((5));
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				setCursor(8,3);
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				write((1));
				Refresh7Seg();
				write((1));
				Refresh7Seg();
			}
			setCursor(8,0);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			setCursor(8,1);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			setCursor(8,2);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			setCursor(8,3);
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
			print(" ");
			Refresh7Seg();
		}
		GameState=1;
	}
	else if(GameState==4 && repeatLevel==0 && NameTimer%20==0){
		setCursor(6,1);
		Refresh7Seg();
		
		print("Y");
		Refresh7Seg();
		print("O");
		Refresh7Seg();
		print("U");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print("W");
		Refresh7Seg();
		print("O");
		Refresh7Seg();
		print("N");
		Refresh7Seg();
		
		setCursor(5,2);
		Refresh7Seg();
		print("C");
		Refresh7Seg();
		print("O");
		Refresh7Seg();
		print("N");
		Refresh7Seg();
		print("T");
		Refresh7Seg();
		print("I");
		Refresh7Seg();
		print("N");
		Refresh7Seg();
		print("U");
		Refresh7Seg();
		print("E");
		Refresh7Seg();
		print("?");
		Refresh7Seg();
		setCursor(3,3);
		Refresh7Seg();
		
		
		print("S");
		Refresh7Seg();
		print("c");
		Refresh7Seg();
		print("o");
		Refresh7Seg();
		print("r");
		Refresh7Seg();
		print("e");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(":");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		char a[100];
		sprintf(a,"%5d",Score);
		print(a);
		Refresh7Seg();
	}
	else if(GameState==4 && repeatLevel==0 && NameTimer%20==10){
		setCursor(6,1);
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		setCursor(5,2);
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		setCursor(3,3);
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
	}
	else if(GameState==3 && repeatLevel==0 && NameTimer%20==0){
		setCursor(5,1);
		Refresh7Seg();
		print("G");
		Refresh7Seg();
		print("a");
		Refresh7Seg();
		print("m");
		Refresh7Seg();
		print("e");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print("O");
		Refresh7Seg();
		print("v");
		Refresh7Seg();
		print("e");
		Refresh7Seg();
		print("r");
		Refresh7Seg();
		print("!");
		Refresh7Seg();
		
	}
	else if(GameState==3 && repeatLevel==0 && NameTimer%20==10){
		setCursor(5,1);
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
		print(" ");
		Refresh7Seg();
	}
	else if(GameState==5){
		GameState=2;
		blink7seg=0;
		setCursor(5,1);
		print("         ");
		setCursor(5,2);
		print("         ");
		GameMap(1);
		LevelBegin(8);
		repeatLevel=1;
	}
	if(GameState==1){
			GameMap(1);
			LevelBegin(1);
			GameState=2;
	}
	
	if(GameState!=2){
		Lock=0;
	HAL_TIM_IRQHandler(&htim4);
		return;
	}
	if((second%5)==0 && second!=LastTurboSecond){
		if(Turbo<10){
			Turbo++;
		}
		LastTurboSecond=second;
		
	}
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
    if(second!=lastTurboDecharge && Turbo>0){
			Speed+=20;
      lastTurboDecharge=second;
      Turbo--;
    }
	}
	
	if(DownTime<Speed){
		Score=((Level+1)*Level)*(second-TimeCollision);
		setCursor(17,0);
		char b[10];
		sprintf(b,"%d",Score/10);
		print(b);
		if(Map[downcount-1][PlaceCar]==1){
			Heart--;
			if(Heart<=0){
				ScreenClear();
				GameState=3;
				HAL_TIM_IRQHandler(&htim4);
				Lock=0;
				return;
			}
			TimeCollision=second;
			char a[10];
			char x=99;
			sprintf(a,"%c:%d",x,Heart);
			setCursor(0,0);
			print(a);
			clearEnemy();
			LevelBegin(Level);
		}
		else{
			for(int i=0;i<18;i++){
				if(Map[downcount][i] != Map[downcount-1][i]){
					
					if(i<2){
						setCursor(2,2);
					}
					else if(i>=17){
						setCursor(17,2);
					}
					else{
						setCursor(i+1,2);
					}
					
					if(Map[downcount][i]==0) print(" ");
					else write((3));//print("#");
				}
				if(Map[downcount+1][i] != Map[downcount][i]){
					if(i<3){
						setCursor(3,1);
					}
					else if(i>=16){
						setCursor(16,1);
					}
					else{
						setCursor(i+1,1);
					}
					if(Map[downcount+1][i]==0) print(" ");
					else write((3));//print("#");
				}
				if(Map[downcount+2][i] != Map[downcount+1][i]){
					if(i<2){
						setCursor(4,0);
					}
					else if(i>=15){
						setCursor(15,0);
					}
					else{
						setCursor(i+1,0);
					}
					if(Map[downcount+2][i]==0) print(" ");
					else write((3));//print("#");
				}
			}
			
			Speed=0;
			downcount++;
		}
	}
	if(Time>=3000){
		LevelBegin(Level+1);
	}
	Lock=0;
  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
* @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
*/
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
		if(isTimeSet==1){//Load
				str[currentBufferSize] = x1;
				currentBufferSize++;
				if(!((str[currentBufferSize-1]>='0' && str[currentBufferSize-1]<='9') || (str[currentBufferSize-1]>='a' && str[currentBufferSize-1]<='z'))){
						if(uartReceiveSw%2==0){
								if(uartReceiveSw==22){
										//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);
										str[currentBufferSize-1] = 0;
										int counter=0;
										for(int i=0;i<30 && counter < enemyListXSize;i++){
												if(SelectRow[i]==1){
														counter++;
														for(int j=0;j<18;j++) if(Map[i][j]==1) Map[i][j]=0;
												}
										}
										for(int i=0;i<enemyListXSize;i++){
												Map[3-enemyListX[i]][enemyListY[i]]=1;
										}
										//sprintf(A,"%d @%d #%d $%d &%d *%d:)",Score,Heart,Level,Turbo,TotalTime,PlaceCar);
										//sprintf(A,"%d:%d %d:%d %d:%d :)",enemyListX[0],enemyListY[0],enemyListX[1],enemyListY[1],enemyListX[2],enemyListY[2]);
										if(Level==9){
												repeatLevel=1;
												Level=8;
										}
										TimeCollision=(TotalTime/200)-(Score/((Level+1)*Level));
										GameMap(0);
										LevelBegin(Level);
										downcount=0;
										showEnemy();
										GameState=2;
								}
								currentBufferSize=0;
						}
						else{
								long long int num=0;
								for(int i=0;i<currentBufferSize-1;i++){
										num=num*10+str[i]-'0';
								}
								str[currentBufferSize-1]=0;
								switch((uartReceiveSw+1)/2){
										case 1:
												
												break;
										case 2:
												Heart=num;
												break;
										case 3:
												Level=num;
												break;
										case 4:
												Score=num;
												break;
										case 5:
												Turbo=num;
												break;
										case 6:
												
												break;
										case 7:
												TotalTime=num*200;
												break;
										case 8:
											
												break;
										case 9:
												
												break;
										case 10:
												PlaceCar=(num-300)-1;
												break;
										case 11:
												while(num){
														int adad=num%10000;
														int y=adad%100;
														adad/=100;
														int x=adad;
														num/=10000;
														enemyListX[enemyListXSize]=y;
														enemyListY[enemyListYSize]=x;
														enemyListXSize++;
														enemyListYSize++;
												}
												break;
								}
								currentBufferSize=0;
						}
						uartReceiveSw++;
				}
		}
		else if (currentBufferSize < 12 && isTimeSet==0) {
				str[currentBufferSize] = x1;
				currentBufferSize++;
				if(currentBufferSize==12){
						str[12] = 0;
						
						int find=0;
						for(int i=0;i<12;i++){
								if((str[i]-'0') <0 || (str[i]-'0')>9){
										find=1;
										break;
								}
						}
						if(find==0){
								isTimeSet=1;
								currentBufferSize=0;
								RTC_DateTypeDef mydate;
								RTC_TimeTypeDef mytime;
								mydate.Year  =(str[0]-'0')*10+str[1]-'0';
								mydate.Month =(str[2]-'0')*10+str[3]-'0';
								mydate.Date  =(str[4]-'0')*10+str[5]-'0';
								mytime.Hours =(str[6]-'0')*10+str[7]-'0';
								mytime.Minutes=(str[8]-'0')*10+str[9]-'0';
								mytime.Seconds=(str[10]-'0')*10+str[11]-'0';
								HAL_RTC_SetTime(&hrtc,&mytime,RTC_FORMAT_BCD);
								HAL_RTC_SetDate(&hrtc,&mydate,RTC_FORMAT_BCD);
								
								RTC_TimeTypeDef x;
								HAL_RTC_GetTime(&hrtc,&x,RTC_FORMAT_BCD);
								setCursor(6,3);
								char time[20];
								sprintf(time,"%2d:%2d:%2d",x.Hours,x.Minutes,x.Seconds);
								print(time);
						}
				}
		}
		HAL_UART_Receive_IT(&huart1, &x1, 1);
  /* USER CODE END USART1_IRQn 1 */
}

/**
* @brief This function handles EXTI line[15:10] interrupts.
*/
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
	if(GameState==9 || GameState == 13){
	char entry = 'N';
	while(entry == 'N'){
		entry = readKeyPad();
	}
	
	if(keypadTimer > 0 && keypadCol==lastKeypadCol && keypadRow==lastKeypadRow) {
		keypadConsecutiveHit = (keypadConsecutiveHit + 1) % 4;
	} else {
		inputTries++;
		keypadConsecutiveHit=0;
	}
	if(entry != '*'){
		lastKeypadCol=keypadCol;
		lastKeypadRow=keypadRow;
		inputNumbers[inputTries] = entry;
		setCursor(inputTries+2,2);
		char a[100];
		sprintf(a,"%c",entry);
		print(a);
		setKeypadTimer();
	}
	else if(entry=='*' && inputTries>1 && GameState == 13){
		if (saveGameFlag) {
			ScreenClear();
			save();
			saveGameFlag = 0;
			GameMap(0);
			showEnemy();
			inputTries = 0;
			GameState = 2;
		}
	 }
	else if(entry=='*' && inputTries>1 && GameState == 9){
		load();
		ScreenClear();
		//GameMap(0);
		//showEnemy();
		inputTries = 0;
		//GameState = 2;
	}
}
  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */
	HAL_GPIO_WritePin(GPIOC ,GPIO_PIN_10 ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC ,GPIO_PIN_11 ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC ,GPIO_PIN_12 ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC ,GPIO_PIN_13 ,GPIO_PIN_SET);
	
  /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
* @brief This function handles ADC3 global interrupt.
*/
void ADC3_IRQHandler(void)
{
  /* USER CODE BEGIN ADC3_IRQn 0 */
int Light = HAL_ADC_GetValue(&hadc3);
	Light=Light*95/64+5;
	if(GameState==-1){
		HAL_ADC_IRQHandler(&hadc3);
		HAL_ADC_Start_IT(&hadc3);
		return;
	}
	switch(Level){
		case 0:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,0);
		break;
			
		case 1:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,0);
		break;
		case 2:
			
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,0);
		break;
		case 3:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,0);
		break;
		case 4:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,0);
		break;
		case 5:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,0);
			break;
		case 6:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,0);
		break;
		case 7:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,0);
		break;
		case 8:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,Light);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,Light);
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,Light);
		break;
			
	}
	//Light=99;
	switch(Turbo){
		case 0:
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,0);
		break;
		case 1:
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,0);
		break;
		case 2:
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,0);
		break;
		case 3:
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,0);
		break;
		case 4:
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,0);
		break;
		case 5:
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,0);
		break;
		case 6:
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,0);
		break;
		case 7:
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
		
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,0);
		break;
		case 8:
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,0);
		break;
		case 9:
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,0);
		break;
		case 10:
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,Light);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,Light);
		
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,Light);
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,Light);
		break;			
	}
	
  /* USER CODE END ADC3_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc3);
  /* USER CODE BEGIN ADC3_IRQn 1 */
	HAL_ADC_Start_IT(&hadc3);
  /* USER CODE END ADC3_IRQn 1 */
}

/**
* @brief This function handles ADC4 interrupt.
*/
void ADC4_IRQHandler(void)
{
  /* USER CODE BEGIN ADC4_IRQn 0 */
	
  /* USER CODE END ADC4_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc4);
  /* USER CODE BEGIN ADC4_IRQn 1 */
	int PIR = HAL_ADC_GetValue(&hadc4);
	if(GameState==2 && PIR>60){
		//function save
		ScreenClear();
		GameState = 12;
		saveGameFlag = 1;
		}
	HAL_ADC_Start_IT(&hadc4);
  /* USER CODE END ADC4_IRQn 1 */
}

/* USER CODE BEGIN 1 */
	

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
