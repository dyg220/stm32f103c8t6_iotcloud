#include "delay.h"

/*for (uint32_t i = 0; i < 72000 ; i++)
{
    for (uint32_t j = 0; j < 500; j++)
    {
        __asm("nop");
    }
}*/

void delay_us(uint32_t xus)
{
    SysTick->CTRL &= ~(1 << 2);  // 外部参考时钟
    SysTick->LOAD = 9 * xus - 1; // 重装载值
    SysTick->VAL = 0;            // 清除计数值
    SysTick->CTRL &= ~(1 << 1);  // 不使能中断
    SysTick->CTRL |= (1 << 0);   // 启动计数器
    while (!(SysTick->CTRL & (1 << 16)))
        ;                       // 等待计数完成
    SysTick->CTRL &= ~(1 << 0); // 关闭计数器

}

void delay_ms(uint32_t xms)
{
    while (xms--)
    {
        delay_us(1000);
    }
}
