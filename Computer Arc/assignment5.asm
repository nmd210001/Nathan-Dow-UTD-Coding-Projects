# Name:	 Nathan Dow
# NetID: nmd210001
# Discription:	Store all entered double percision floating point numbers by the user into an array until 0 is entered. 
#		After 0 is entered sort the array using bubble sort in order from least to greatest and print each double one at a time
		.include	"SysCalls.asm"
		.data
prompt:		.asciiz 	"Enter a number, zero to exit: "
doneMsg:	.asciiz		"Done."
		.align 		3			#Align to 8 byte
zeroDouble:	.double		0.0
inputBuffer:	.space		21
		.align		3
array:		.space		800			#Enough space to store 100 doubles
		.text
Intializer:	li 	$t0, 	0			#Keep track of number of doubles in the array to calc start index
		li 	$t1, 	0			#Keep track of the index in the arary		
		lwc1	$f20,	zeroDouble	

main:		li	$v0,	SysPrintString		#Print the prompt
    		la	$a0,	prompt
    		syscall
		
		li	$v0,	7  			# read the double input and store in $f0
    		syscall
    		
		c.eq.d	$f0,	$f20			# Set c1 flag to 1 if input in 0
   		bc1t	callFuncs			# If it was flaged jump to exit and exit the program
   		
   		
   		sdc1	$f0,	array($t1)		# store the double in arary at the current 8 byte index increment
   		addi	$t0,	$t0,	1		#
   		addi	$t1,	$t1,	8
   		
   		j main
   		
   		
callFuncs:	move	$a0,	$t0
		beq	$a0,	0,	exit
		beq	$a0,	1,	onlyOne
		jal sortList
		jal printList
		j exit
   		
   		
sortList:	subi    $sp, 	$sp, 	4       	# Make space for a word
    		sw      $ra, 	($sp)         		# Store the return address on the stack
    
    		move    $t0, 	$a0          		# Copy the size of the array to $t0
    		subi    $t0, 	$t0, 	1      		# use arary logic
    
outerLoop:	li      $t1, 0            		# Initialize inner loop index
    
innerLoop:	mul     $t2,	$t1,	8       	# Calculate offset for array element access
		l.d     $f2,	array($t2)   		# Load array[$t1]
    
		add     $t3,	$t1,	1       	# Calculate index for the next element
		mul     $t4,	$t3,	8       	# Calculate offset for the next element
		l.d     $f4,	array($t4)   		# Load array[$t3]
    
		c.lt.d  $f2,	$f4          		# Compare current and next element
		bc1t    skipSwap           		# If $f2 < $f4, skip swap
    
		s.d     $f4,	array($t2)   		# Swap elements: store $f4 at array[$t1]
		s.d     $f2,	array($t4)   		# Store $f2 at array[$t3]
    
skipSwap:	addi    $t1,	$t1,	1       	# Increment inner loop index
    		blt     $t1,	$t0,	innerLoop  	# Loop innerLoop if $t1 < $t0
    
   		subi    $t0,	$t0,	1       	# Decrement outer loop index
    		bgtz    $t0,	outerLoop    		# Loop outerLoop if $t0 > 0
    
    		lw      $ra,	($sp)         		# Load the return address from the stack
    		addi    $sp,	$sp,	4        	# Restore the stack pointer by deallocating the space used for the return address
    		jr      $ra                		# Jump back to the address stored in $ra

printList:	subi	$sp,	$sp,	4		# Makes space for a word
		sw	$ra,	($sp)			# Stores the return address on the stack
		
		move	$t8	$a0
		li	$t6,	0              		# Initialize index for printing
		
printLoop:	mul	$t7,	$t6,	8		# Calculate offset for array element access
    		add 	$t7, 	$t7, 	$zero		# Add offset to base address of array
    		l.d	$f2, 	array($t7)     		# Load array[$t6]

    		
    		mov.d	$f12,	$f2        		# Move double to $f12 (argument for printing)
   		li	$v0,	3              		# Load syscall code for printing double
    		syscall
    		
    		li 	$v0, 	SysPrintChar           	# Load syscall code for printing character
    		li 	$a0, 	'\n'                	# Load newline character
    		syscall

    		addi	$t6,	$t6,	1      		# Increment index for printing
    		blt	$t6,	$t8,	printLoop 	# Loop printLoop if $t6 < $a0

    		lw	$ra,	($sp)			# Load the return address from the stack
    		addi 	$sp, 	$sp,	4            	# Restore the stack pointer by deallocating the space used for the return address
    		jr 	$ra                      	# Jump back to the address stored in $ra
    		
onlyOne:	mul	$t7,	$t6,	8		# Calculate offset for array element access
    		add 	$t7, 	$t7, 	$zero		# Add offset to base address of array
    		l.d	$f2, 	array($t7)     		# Load array[$t6]
    		
		li 	$v0, 	3                   	# Load syscall code for printing double
    		mov.d 	$f12, 	$f2             	# Move the single entered number to $f12 (argument for printing)
    		syscall
    		
    		li 	$v0, 	SysPrintChar           	# Load syscall code for printing character
    		li 	$a0, 	'\n'                	# Load newline character
    		syscall
    		
    		j exit
		
exit: 		li $v0, SysPrintString			#Print the prompt
    		la $a0, doneMsg
    		syscall

		li $v0, SysExit
    		syscall
    	
    	