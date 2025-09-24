#include "stm32f10x.h"
#include "Delay.h"


#define DHT11_port GPIO_Pin_1   //便于修改IO口
#define DHT11_GPIO GPIOA      //便于修改gpio组
#define DHT11_DQ_IN   GPIO_ReadInputDataBit(DHT11_GPIO,DHT11_port)   //PA1
#define DHT11_DQ_OUT  GPIO_ReadOutputDataBit(DHT11_GPIO,DHT11_port)  //PA1
#define delay_us(x) Delay_us(x)
#define delay_ms(x) Delay_ms(x)

//此处设置IO口为输出模式
void DHT11_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = DHT11_port;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(DHT11_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(DHT11_GPIO, DHT11_port); // 初始化后拉高总线
}
void DHT11_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_port;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO, &GPIO_InitStructure);
}

/************设置io口为输入模式 */ 
void DHT11_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_port;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO, &GPIO_InitStructure);
}

/************拉低总线 */ 
void DHT11_Low(void)
{
    GPIO_ResetBits(DHT11_GPIO,DHT11_port);
}   

/************拉高总线 */ 
void DHT11_High(void)
{
    GPIO_SetBits(DHT11_GPIO,DHT11_port);
}
//主机首先下拉总线至少18ms，然后拉高20~40us，再将总线设置为输入，等待DHT11的响应
void DHT11_Rst(void)
{
    DHT11_OUT();
    DHT11_Low();
    delay_ms(20);
    DHT11_High();
    delay_us(30);  
}

//检测DHT11的回应信号，先等待总线被拉低，再等待总线被拉高
u8 DHT11_Check(void)
{
    u8 retry=0;
    DHT11_IN(); // 设置为输入模式等待响应
    while ( DHT11_DQ_IN && retry<100) // 等待DHT11拉低
    {
        retry++;
        delay_us(1);
    };
    if(retry>=100){
        return 1;
    }
    
    retry=0;
    while ( !DHT11_DQ_IN && retry<100) // 等待DHT11拉高
    {
        retry++;
        delay_us(1);
    };
    if(retry>=100){
        return 1;
    }

    return 0;
}

/************设置io口为输出模式 */ 


/************读一个位 */ 
u8 DHT11_Read_Bit(void) 			 
{
    u8 retry=0;

    while( DHT11_DQ_IN && retry<100)//等待变为低电平
    {
        retry++;
        delay_us(1);
    }

    retry=0;
    while( !DHT11_DQ_IN && retry<100)//等待变高电平
    {
        retry++;
        delay_us(1);
    }
    delay_us(40);//等待40us
    if(DHT11_DQ_IN)return 1;
    else return 0;		   
}

//*********读取字节 */
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
    for (i=0;i<8;i++) 
    {
        dat<<=1; 
        dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

//读数据，温度和湿度各8位整数+8位小数+8位校验和
//需要传入两个变量的地址
//返回1失败，0成功
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
    u8 buf[5];
    u8 i;
    DHT11_Rst();
    if(DHT11_Check()==0)
    {
        for(i=0;i<5;i++)//读取40位数据
        {
            buf[i]=DHT11_Read_Byte();
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
        {
            *humi=buf[0];
            *temp=buf[2];
            return 0;
        }
    }
    return 1;	    
}