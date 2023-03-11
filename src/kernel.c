#include "mini_uart.h"
#include "string.h"
#include "mailbox.h"
#include "reboot.h"
#include "cpio.h"

#define BUFF_SIZE 1000

char readBuf[BUFF_SIZE];
char *curFileAddr;

void shell(){
	int count = 0;
	while (1) {
		char recvChar = uart_recv();
		if(recvChar == '\r'){
			uart_send_string("\r\n");
			readBuf[count] = '\0';
			if(!strcmp(readBuf, "help")){
				uart_send_string(". help\t: print help menu.\r\n. hello\t: print \"Hello World!\".\r\n. info\t: print the hardware's information.\r\n. ls\t: list files.\r\n. cat\t: show the file's content.\r\n. reboot: reboot the device.\r\n");
			}else if(!strcmp(readBuf, "hello")){
				uart_send_string("Hello World!\r\n");
			}else if(!strcmp(readBuf, "info")){
				uart_send_string("Hardware's infomation:\r\n");
				get_board_revision();
				get_arm_memory();
			}else if(!strcmp(readBuf, "reboot")){
				uart_send_string("Reboot the device.\r\n");
				reset(20);
			}else if(!strcmp(readBuf, "ls")){
				ls(curFileAddr);
			}else if(!memcmp(readBuf, "cat", 3)){
				cat(curFileAddr, readBuf+4);
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

void kernel_main(void)
{	
	uart_init();
	curFileAddr = ramfs_init();
	uart_send_string("Hello!!!\r\n# ");

	shell();
}