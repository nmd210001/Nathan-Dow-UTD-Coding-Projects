.data
	.half 127		# hALF WORD (2 bytes
num1:	.word 0xDEADBEEF 	# word (4 bytes

.text
.globl main
main:	# load example
	# load an address, then load a half from that address
	# what happens if we offset the address?
	la $t0, num1
	lhu $t1, ($t0)
	
	
	
	# Exit the program
	li $v0, 10 
	syscall
