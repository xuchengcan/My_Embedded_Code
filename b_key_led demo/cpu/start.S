
.text
.extern printf
.extern uart_init
.extern Start_ArmBoot
.global _start
_start:

	@uart_init();
	bl uart_init
	
	@Start_ArmBoot();
	bl Start_ArmBoot

	@while(1)
halt:
	b halt 


.end

