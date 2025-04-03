# Name:	 Nathan Dow
# NetID: nmd210001
# Discription:	This program asks the user for integers to add together until a '0' is entered. 
# 		The sum will be printed and the number of numbers entered by the user is then printed.
# Example:	The prompt will be looped until a zero is entered so if the first input is 4, secound is 6, and the third is 0 -
# 		10 will be returned bc 4 + 6 = 10 and 2 will be the number of input numbers because the 0 is not counted as an input.

.data
	prompt: .asciiz "Enter an integer: "
	sumed:  .asciiz "Sum of numbers: "
	numEntered: .asciiz "\nCount of numbers entered: "
	
.text

sumIntialize:
	li $t0, 0 		# $t0 is where the sum will be stored
	li $t1, 0 		# $t1 is where the number of ints entered
	
input: 	
	li $v0, 4      		# allocates 4 bytes for the promt
	la $a0, prompt 		# stores prompt in $a0
	syscall	       		# prints the prompt
	
	li $v0, 5   		# System call magic number
	syscall			# Read the integer
	move $t2 $v0  		#stores the inputed value in temp 0
	
	beq $t2, $zero, endLoop # if the input value was 0 jump to endLoop
	
	add $t0, $t0, $t2    	# Adds the imputed value to sum
	add $t1, $t1, 1		# Keeps track of the number of numbers 
	
	j input			#reiterates input to add another integer to the sum
	
endLoop: 
	li $v0, 4      		# allocates 4 bytes for the promt
	la $a0, sumed		# stores prompt in $a0
	syscall	       		# prints the sumpromt
	
	li $v0, 1      		# allocates 1 bytes for the sum
	move $a0, $t0		# stores prompt in $a0
	syscall	       		# prints the sum
	
	li $v0, 4      		# allocates 4 bytes for the prompt
	la $a0, numEntered	# stores prompt in $a0
	syscall	       		# prints the number of numbers entered prompt
	
	li $v0, 1      		# allocates 1 bytes for the number of numbers entered
	move $a0, $t1		# stores prompt in $a0
	syscall	       		# prints the number of numbers entered
	
	#Exit Program
	li $v0, 10
	syscall
	
	
	

	
