#include <stm32f10x.h>

void PWM_GPIO_TIM_Init()
{   TIM_OCInitTypeDef TIM_OCInitStructure;//这是定义一个结构体变量，含义是定时器输出比较初始化结构体
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//这是定义一个结构体变量，含义是定时器基本配置初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;//| GPIO_Pin_1; // PA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 预分频器的值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; // 时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // 初始化定时器2
   
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 500; // 设置待装入捕获
    //500和1000的关系是占空比50%，1000是设定的周期，500就是一半
    //psc是72-1，定时器时钟72Mhz/72=1Mhz,也就是1us计数一次，1000就是1ms
    //所以这个配置就是1ms的周期，50%的占空比
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM2, &TIM_OCInitStructure); // 根据T指定

    TIM_Cmd(TIM2, ENABLE); // 使能TIM2
}

void PWM_SetCompare1(uint16_t compare)
{
    TIM_SetCompare1(TIM2, compare);
}