#include "BSP_LCD.h"

#define LL_Lib 0   //LL库
#define HAL_Lib 1  //HAL库
#define RTOS_Lib 0 //FreeRTOS

#if RTOS_Lib
#include "cmsis_os.h"
#endif

//LCD的画笔颜色和背景色
uint16_t POINT_COLOR = 0x0000; //画笔颜色
uint16_t BACK_COLOR = 0xFFFF;  //背景色

void SPI_WriteByte(uint8_t Data)
{
//	uint8_t i = 0;
//	for (i = 8; i > 0; i--)
//	{
//		if (Data & 0x80)
//			SPILCD_SDA_SET; //输出数据
//		else
//			SPILCD_SDA_RESET;

//		SPILCD_SCK_RESET;
//		SPILCD_SCK_SET;
//		Data <<= 1;
//	}
   HAL_SPI_Transmit(&hLCDSPI,&Data,1,0xFF);
}

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(uint8_t regval)
{
	SPILCD_CS_RESET; //LCD_CS=0
	SPILCD_RS_RESET;
	SPI_WriteByte(regval);
	SPILCD_CS_SET; //LCD_CS=1
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(uint16_t data)
{
	SPILCD_CS_RESET; //LCD_CS=0
	SPILCD_RS_SET;
	SPI_WriteByte(data >> 8);
	SPI_WriteByte(data);
	SPILCD_CS_SET; //LCD_CS=1
}
void LCD_WR_DATA8(uint8_t da) //写8位数据
{
	SPILCD_CS_RESET; //LCD_CS=0
	SPILCD_RS_SET;
	SPI_WriteByte(da);
	SPILCD_CS_SET; //LCD_CS=1
}

void LCD_WR_DATA16(uint16_t da) //写16位数据
{
	SPILCD_CS_RESET; //LCD_CS=0
	SPILCD_RS_SET;
	SPI_WriteByte(da >> 8);
	SPI_WriteByte(da);
	SPILCD_CS_SET; //LCD_CS=1
}

//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WR_REG_DATA(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void Lcd_SetRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
	LCD_WR_REG(0x2a);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(x_start);
	//	LCD_WR_DATA8(x_start+2);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(x_end);
	//	LCD_WR_DATA8(x_end+2);

	LCD_WR_REG(0x2b);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(y_start);
	//	LCD_WR_DATA8(y_start+1);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(y_end);
	//	LCD_WR_DATA8(y_end+1);

	LCD_WR_REG(0x2c);
}

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	Lcd_SetRegion(Xpos, Ypos, Xpos, Ypos);
}

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t Point_Color)
{
	LCD_SetCursor(x, y); //设置光标位置
	LCD_WR_DATA(POINT_COLOR);
}
//初始化lcd
void LCD_Init(void)
{
	SPILCD_RST_RESET; //LCD_RST=0	 //SPI接口复位

#if LL_Lib
	LL_mDelay(100);
#endif
#if HAL_Lib
	HAL_Delay(100);
#endif
#if RTOS_Lib
	osDelay(100);
#endif

	SPILCD_RST_SET; //LCD_RST=1
#if LL_Lib
	LL_mDelay(50);
#endif
#if HAL_Lib
	HAL_Delay(50);
#endif
#if RTOS_Lib
	osDelay(50);
#endif

	LCD_WR_REG(0x11); //Sleep out
#if LL_Lib
	LL_mDelay(120);
#endif
#if HAL_Lib
	HAL_Delay(120);
#endif
#if RTOS_Lib
	osDelay(120);
#endif
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);

	LCD_WR_REG(0xB3);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	LCD_WR_REG(0xB4); //Dot inversion
	LCD_WR_DATA8(0x03);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x04);

	LCD_WR_REG(0xC1);
	LCD_WR_DATA8(0XC0);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x2A);

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	LCD_WR_REG(0xC5); //VCOM
	LCD_WR_DATA8(0x1A);

	LCD_WR_REG(0x36); //MX, MY, RGB mode
	LCD_WR_DATA8(0xC0);
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2A);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x3A);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x13);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x16);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x3B);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
	LCD_WR_REG(0x3A); //65k mode
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0x29); //Display on
	// LCD_Clear(YELLOW);
	// LCD_Fill(0,0,128,160,BLUE);
}
//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint16_t color)
{
	unsigned int i, m;
	Lcd_SetRegion(0, 0, 128 - 1, 160 - 1);
	LCD_WR_REG(0x2C);
	for (i = 0; i < 128; i++)
		for (m = 0; m < 160; m++)
		{
			LCD_WR_DATA16(color);
		}
}
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
//void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
//{
//	uint16_t i,j;
//	uint16_t xlen=0;
//	xlen=ex-sx+1;
//	for(i=sy;i<=ey;i++)
//	{
//	 LCD_SetCursor(sx,i);      				//设置光标位置
//		LCD_WriteRAM_Prepare();     			//开始写入GRAM
//		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//设置光标位置
//	}
//}
//在指定区域内填充指定颜色块
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
//void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
//{
//	uint16_t height,width;
//	uint16_t i,j;
//	width=ex-sx+1; 		//得到填充的宽度
//	height=ey-sy+1;		//高度
// 	for(i=0;i<height;i++)
//	{
// 		LCD_SetCursor(sx,sy+i);   	//设置光标位置
//		LCD_WriteRAM_Prepare();     //开始写入GRAM
//		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*height+j];//写入数据
//	}
//}
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标
//void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
//{
//	uint16_t t;
//	int xerr=0,yerr=0,delta_x,delta_y,distance;
//	int incx,incy,uRow,uCol;
//	delta_x=x2-x1; //计算坐标增量
//	delta_y=y2-y1;
//	uRow=x1;
//	uCol=y1;
//	if(delta_x>0)incx=1; //设置单步方向
//	else if(delta_x==0)incx=0;//垂直线
//	else {incx=-1;delta_x=-delta_x;}
//	if(delta_y>0)incy=1;
//	else if(delta_y==0)incy=0;//水平线
//	else{incy=-1;delta_y=-delta_y;}
//	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
//	else distance=delta_y;
//	for(t=0;t<=distance+1;t++ )//画线输出
//	{
//		LCD_DrawPoint(uRow,uCol);//画点
//		xerr+=delta_x ;
//		yerr+=delta_y ;
//		if(xerr>distance)
//		{
//			xerr-=distance;
//			uRow+=incx;
//		}
//		if(yerr>distance)
//		{
//			yerr-=distance;
//			uCol+=incy;
//		}
//	}
//}
//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
//void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
//{
//	LCD_DrawLine(x1,y1,x2,y1);
//	LCD_DrawLine(x1,y1,x1,y2);
//	LCD_DrawLine(x1,y2,x2,y2);
//	LCD_DrawLine(x2,y1,x2,y2);
//}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
//void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
//{
//	int a,b;
//	int di;
//	a=0;b=r;
//	di=3-(r<<1);             //判断下个点位置的标志
//	while(a<=b)
//	{
//		LCD_DrawPoint(x0+a,y0-b);             //5
// 	LCD_DrawPoint(x0+b,y0-a);             //0
//		LCD_DrawPoint(x0+b,y0+a);             //4
//		LCD_DrawPoint(x0+a,y0+b);             //6
//		LCD_DrawPoint(x0-a,y0+b);             //1
// 	LCD_DrawPoint(x0-b,y0+a);
//		LCD_DrawPoint(x0-a,y0-b);             //2
//  LCD_DrawPoint(x0-b,y0-a);             //7
//		a++;
//		//使用Bresenham算法画圆
//		if(di<0)di +=4*a+6;
//		else
//		{
//			di+=10+4*(a-b);
//			b--;
//		}
//	}
//}
////在指定位置显示一个汉字(16*16大小)
//void showhanzi16(unsigned int x,unsigned int y,unsigned char index)
//{
//	unsigned char i,j,k;
//	const unsigned char *temp=hanzi16;
//	temp+=index*32;
//	for(j=0;j<16;j++)
//	{
//		LCD_SetCursor(x,y+j);
//		LCD_WriteRAM_Prepare();	//开始写入GRAM
//		for(k=0;k<2;k++)
//		{
//			for(i=0;i<8;i++)
//			{
//			 	if((*temp&(1<<i))!=0)
//				{
//					LCD_WR_DATA(POINT_COLOR);
//				}
//				else
//				{
//					LCD_WR_DATA(BACK_COLOR);
//				}
//			}
//			temp++;
//		}
//	 }
//}
////在指定位置显示一个汉字(32*32大小)
//void showhanzi32(unsigned int x,unsigned int y,unsigned char index)
//{
//	unsigned char i,j,k;
//	const unsigned char *temp=hanzi32;
//	temp+=index*128;
//	for(j=0;j<32;j++)
//	{
//		LCD_SetCursor(x,y+j);
//		LCD_WriteRAM_Prepare();	//开始写入GRAM
//		for(k=0;k<4;k++)
//		{
//			for(i=0;i<8;i++)
//			{
//			 	if((*temp&(1<<i))!=0)
//				{
//					LCD_WR_DATA(POINT_COLOR);
//				}
//				else
//				{
//					LCD_WR_DATA(BACK_COLOR);
//				}
//			}
//			temp++;
//		}
//	 }
//}
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
//void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
//{
//    uint8_t temp,t1,t;
//	uint16_t y0=y;
//	uint16_t colortemp=POINT_COLOR;
//	//设置窗口
//	num=num-' ';//得到偏移后的值
//	if(!mode) //非叠加方式
//	{
//	    for(t=0;t<size;t++)
//	    {
//			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
//			else temp=asc2_1608[num][t];		 //调用1608字体
//	        for(t1=0;t1<8;t1++)
//			{
//		        if(temp&0x80)POINT_COLOR=colortemp;
//				else POINT_COLOR=BACK_COLOR;
//				LCD_DrawPoint(x,y);
//				temp<<=1;
//				y++;
//				if(y>=lcddev.height){POINT_COLOR=colortemp;return;}//超区域了
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
//					break;
//				}
//			}
//	    }
//	}else//叠加方式
//	{
//	    for(t=0;t<size;t++)
//	    {
//			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
//			else temp=asc2_1608[num][t];		 //调用1608字体
//	        for(t1=0;t1<8;t1++)
//			{
//		        if(temp&0x80)LCD_DrawPoint(x,y);
//				temp<<=1;
//				y++;
//				if(y>=lcddev.height){POINT_COLOR=colortemp;return;}//超区域了
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
//					break;
//				}
//			}
//	    }
//	}
//	POINT_COLOR=colortemp;
//}
//m^n函数
//返回值:m^n次方.
//uint32_t LCD_Pow(uint8_t m,uint8_t n)
//{
//	uint32_t result=1;
//	while(n--)result*=m;
//	return result;
//}
//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
//void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
//{
//	uint8_t t,temp;
//	uint8_t enshow=0;
//	for(t=0;t<len;t++)
//	{
//		temp=(num/LCD_Pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
//				continue;
//			}else enshow=1;
//
//		}
//	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0);
//	}
//}
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
//void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
//{
//	uint8_t t,temp;
//	uint8_t enshow=0;
//	for(t=0;t<len;t++)
//	{
//		temp=(num/LCD_Pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);
//				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);
// 				continue;
//			}else enshow=1;
//
//		}
//	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01);
//	}
//}
//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
//void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
//{
//	uint8_t x0=x;
//	width+=x;
//	height+=y;
//    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
//    {
//        if(x>=width){x=x0;y+=size;}
//        if(y>=height)break;//退出
//        LCD_ShowChar(x,y,*p,size,1);
//        x+=size/2;
//        p++;
//    }
//}

//void showimage(uint16_t x,uint16_t y) //显示40*40图片
//{
//	uint16_t i,j,k;
//	uint16_t da;
//	k=0;
//	for(i=0;i<40;i++)
//	{
//		LCD_SetCursor(x,y+i);
//		LCD_WriteRAM_Prepare();     			//开始写入GRAM
//		for(j=0;j<40;j++)
//		{
//			da=qqimage[k*2+1];
//			da<<=8;
//			da|=qqimage[k*2];
//			LCD_WR_DATA(da);
//			k++;
//		}
//	}
//}
