# Name:	 Nathan Dow
# NetID: nmd210001
# Discription:	
# Example:
               .include "SysCalls.asm"
               .data
prompt:        .asciiz  "Enter a number: "
errorMsg:      .asciiz  "Error: "
sumMsg:    	.asciiz  "Sum = "
countMsg:	.asciiz  "\nCount of valid numbers = "
errorCountMsg: .asciiz  "\nTotal number of errors: "

buffer:        .space	21                          # 20 bytes for each character plus an extra for the '\0'
accumulator:   .word	0                           # this accumulator will store the sum of all valid inputs
validCount:    .word	0                           # this word will represent the number of valid inputs
errorCount:    .word	0                           # this word will represent the number of errors


               .text		
input:		li	$t0,	0                    # Initialize index to 0
   		li	$t5,	0                    # Initialize tempAccumulator to 0
    		li	$t2,	0                    # Initalizes negitive flag to 0
    		li	$t6,	0                    # Initialize error flag to 0

		li	$v0,	SysPrintString       #loads the imidiate valueinto $v0 to signal syscall code to print string
		la	$a0,	prompt               # loads prompt into $a0
		syscall		                     # Prints prompt
		
		li	$v0,	SysReadString        # syscall code to print string
		la	$a0,	buffer               # loads string input into buffer
		la	$a1,	20                   # sets the max amount of characters to 20
		syscall 
		
 		lb	$t3,	buffer	             # using lb because we only need to load the first character of the buffer to see if it's null
		beq	$t3,	'\n',	endLoop      # if the first character of the string is nothing then branch to endloop proticall
		
		li	$t3,	'-'		     # using lb because we only need to load the first character of the buffer to see if it's null
		lb	$t4,	buffer	             # if the first character of the string is nothing then branch to endloop proticall
		beq	$t4,	$t3,	prepNeg      # if the first character of the string is '-' branch to the negitive handeler
		
addString:	lb	$t4,	buffer($t0)
		beq	$t4,	'\n',	checkLast    # checks if the current index is the end
		
		blt	$t4,	'0',	errorFound   # Check if character is less than '0'
    		bgt	$t4,	'9',	errorFound   # Check if character is greater than '9'
    		sub 	$t4,	$t4,	'0'          # Convert ASCII to integer
    		mul	$t5,	$t5,	10	     # accounts for place value
    		beq	$t2,	1,	handleNeg    # if a - came before $t2 = 1 and therefor will be converted into a negitive
    		add 	$t5,	$t5,	$t4          #		
    		addi	$t0,	$t0,	1            # Increment index
    		j	addString       	     # Repeat loop
 
prepNeg: 	addi	$t0,	$t0,	1            # Increment index to ignore -
		li	$t2,	1		     # lets addString know it's negitive
		j	addString
		
handleNeg:	nor	$t4,	$t4,	$zero	     # makes ever bit of $t4 the oppisite
		addi	$t4,	$t4,	1	     # adds one to make it 2s compliment
		add	$t5,	$t5,	$t4	     # adds the now negitive value to the accumulator
		addi	$t0,	$t0,	1
		j	addString

checkLast: 	li	$t4,	'\n'			
  	  	lb	$t7,	buffer($t0)
		beq	$t7,	$t4,	updateAccum # Go to next step of updating .words if new line found
   		j 	input       	            # Continue input loop
			
updateAccum:	lw	$t8,	accumulator
   		add	$t8,	$t8,	$t5	    # Add accumulatorTemp to accumulator
    		sw 	$t8, 	accumulator	    # Store accumulator value
    		lw 	$t9, 	validCount         # Load valid count value
    		addi 	$t9, 	$t9,	1	    # Increment valid count
   		sw	$t9,	validCount	    # Store valid count value
		j 	input          	      	    # Continue input loop

errorFound: 	li	$t6,	1                  # Set error flag to 1

    		li	$v0,	SysPrintString     # syscall code for print string
    		la	$a0,	errorMsg           # load address of the error message to print
    		syscall

    		li	$v0,	SysPrintString     # syscall code for print string
    		la	$a0,	buffer             # load address of the buffer to print
	  	syscall
		lw	$t3,	errorCount         # Load error count value
		addi	$t3,	$t3,	1    	    # Increment error count
	  	sw	$t3,	errorCount  	    # Store incurmented error count value   		
	  	j	input		      	    # Continue input loop
		
endLoop:	li 	$v0,	SysPrintString     # syscall code for print string
    		la 	$a0,	sumMsg             # load address of the sum message to print
   		syscall

    		lw	$a0,	accumulator        # Load the sum from accumulator
    		li	$v0,	SysPrintInt        # syscall code for print integer
   		syscall

    		li	$v0,	SysPrintString     # syscall code for print string
    		la 	$a0,	countMsg           # load address of the count message to print
    		syscall

    		lw	$a0,	validCount         # Load the count of valid numbers
    		li	$v0,	SysPrintInt        # syscall code for print integer
    		syscall

   		li	$v0,	SysPrintString     # syscall code for print string
    		la	$a0,	errorCountMsg      # load address of the error count message to print
    		syscall

    		lw 	$a0,	errorCount         # Load the total number of errors
    		li 	$v0,	SysPrintInt        # syscall code for print integer
    		syscall

    		li	$v0,	SysExit            # syscall code for exit
    		syscall	
