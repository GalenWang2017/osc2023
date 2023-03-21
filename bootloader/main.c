#include "mini_uart.h"
#include "string.h"
#include "mailbox.h"
#include "reboot.h"
#define BUFF_SIZE 1000

char readBuf[BUFF_SIZE];

void shell(){
	int count = 0;
	while (1) {
		char recvChar = uart_recv();
		if(recvChar == '\r'){
			uart_send_string("\r\n");
			readBuf[count] = '\0';
			if(!strcmp(readBuf, "help")){
				uart_send_string(". help\t: print help menu.\r\n. hello\t: print \"Hello World!\".\r\n. info\t: print the hardware's information.\r\n. reboot: reboot the device.\r\n. load\t: load kernel\r\n");
			}else if(!strcmp(readBuf, "hello")){
				uart_send_string("Hello World!\r\n");
			}else if(!strcmp(readBuf, "info")){
				uart_send_string("Hardware's infomation:\r\n");
				get_board_revision();
				get_arm_memory();
			}else if(!strcmp(readBuf, "reboot")){
				uart_send_string("Reboot the device.\r\n");
				reset(50);
			}else if(!strcmp(readBuf, "load")){
                uart_send_string("load kernel...\r\n");
				unsigned int size = 0;
				char *k = (char *)0x80000;
				size = uart_recv();
				size |= (uart_recv()<<8);
				size |= (uart_recv()<<16);
				size |= (uart_recv()<<24);
				uart_send_string("Size of kernel: ");
				uart_dec(size);
				uart_send_string("\r\nStart to load kernel\r\n");
				while(size--){
					*k++ = uart_recv();
				}
				uart_send_string("Finish loading kernel\r\nBooting...\r\n");
				asm volatile("ldr x30, =_kernel");
				asm volatile("br x30;");
            }else{
				uart_send_string("Command not found: ");
				uart_send_string(readBuf);
				uart_send_string("\r\n");
			}
			count = 0;
			readBuf[0] = '\0';
			uart_send_string("# ");
		}
		else{
			uart_send(recvChar);
			readBuf[count] = recvChar;
			count += 1;
		}
	}
}

void bootloader(void)
{	
	uart_init();
	uart_send_string("\r\nBootloader...\r\n# ");

	shell();
}