#define rGPJ2CON 0xE0200280
#define rGPJ2DAT 0xE0200284

.text
.extern printf
.extern uart_init
.extern Start_ArmBoot
.global _start

_start:

	@1.初始化GPJ2_0引脚为输出模式
	ldr r1,=rGPJ2CON
	ldr r0,[r1]
	ldr r4,=0xFFFF
	ldr r5,=0x1111
	bic r0,r0,r4	
	
	orr r0,r0,r5
	str r0,[r1]

my:
	mov r3,#1
	
	@2.while(1)
loop:
	
	@rGPJ2DAT &=~(1<<i)
	ldr r1,=rGPJ2DAT
	ldr r0,[r1]
	bic r0,r0,r3
	str r0,[r1]
	
	
	@Delay
	bl	delay
	
	@rGPJ2DAT |= (1<<i);
	ldr r1,=rGPJ2DAT
	ldr r0,[r1]
	orr r0,r0,r3
	str r0,[r1]	
	
	@Delay
	bl	delay

	mov r3,r3,lsl #1

	cmp r3,#0x10
	beq my

	
	b loop

	
delay:
	mov r0,#0x2000000
delay_loop:
	subs r0,r0,#1			//r0减1，同时影响CPSR标志位
	bne delay_loop			//如果r0不等于0，则跳转到delay_loop
	bx lr					//如果r0等于0的时候，执行函数返回

halt:
	b halt 


.end

