# Name:	 Nathan Dow
# NetID: nmd210001
# Discription: Enter a passowrd that is at least 8 characters, at least 1 uppercase, at least lowercase ,and at least 1 digit
# Example:
               	.include	"SysCalls.asm"
               	.data
prompt:        	.asciiz		"\nEnter a password: "
passValid:	.asciiz		"Valid password."
missDigMsg:    	.asciiz		"Invalid password - missing digit."
missUpMsg:     	.asciiz		"Invalid password - missing uppercase letter."
missLowMsg:    	.asciiz		"Invalid password - missing lowercase letter."
lengthShort: 	.asciiz		"Invalid password - length too short: "
invalidChar:	.asciiz		"Invalid password - invalid character: "

pass:		.space  21


               .text		
input:		li	$v0,	SysPrintString		# Loads the imidiate value into $v0 to start the syscall to print a string
		la	$a0,	prompt
		syscall
		
		li	$v0,	SysReadString		# Loads the imidate value into $v0 to start the syscall to read a string
		la	$a0,	pass			# Loads the input into the pass buffer
		la	$a1,	21		      	# Makes the maximum amount of bytes 20 so 20 characters
		syscall
		
		lb	$t3,	pass	             	# using lb because we only need to load the first character of the buffer to see if it's null
		beq	$t3,	'\n',	end          	# if the first character of the string is nothing then branch to endloop proticall
		

		
intalizer:	li	$t1,	0		      	# char index/length
		li 	$t2, 	0                    	# Uppercase flag
		li	$t3,	0                    	# Lowercase flag
		li	$t4,	0                    	# Digit flag
		
		
charLoop:	lb 	$t5,	pass($t1)	      	# loads a char of pass to $t5
		beq	$t5,	'\n',	endCharLoop   	# if the char is null it is the end of the word so we need to check our cases now
		
		     	
checkUp:	blt	$t5,	65,	checkDig	# Sees if the ASCII value is less than A if it is check if it's a digit
		bgt	$t5,	90,	checkLow	# Sees if the ASCII value is greater than Z if it is check if it's a lowercase
		li	$t2,	1			# Else it must be an uppercase so the flag is triggered
		
		addi	$t1,	$t1,	1	      	# Moves the index to access the next character of pass
		j charLoop				# Starts checking the next char

checkLow:	blt	$t5,	97,	invalFound	# If the character's ASCII is greater than 90 and less than 97 it must be a special
		bgt	$t5,	123,	invalFound	# If the character is greater than 123 then it must be a special
		li	$t3,	1			# Else it is a lowercase and the lowercase flag is triggered
		
		addi	$t1,	$t1,	1	      	# Moves the index to access the next character of pass
		j charLoop				# Starts checking the next char

checkDig:	blt	$t5,	48,	invalFound	# If the ASCII value is less than 48 then it is a special character
		bgt	$t5,	57,	invalFound	# If the ASCII value is less than 65 and greater than 57 it is a special character
		li	$t4,	1			# Else it is a digit and the digit flag is triggered
		
		addi	$t1,	$t1,	1	      	# moves the index to access the next character of pass
		j charLoop				# Starts checking the next char

invalFound:	li	$v0,	SysPrintString       	# Loads the imidiate value into $v0 to start the syscall to print a string
		la	$a0,	invalidChar
		syscall
		
		li	$v0,	SysPrintChar		# Loads the imidate value into $v0 to start the syscall to print a char
		lb	$a0,	pass($t1)		# Loads the character at the current index into $a0
		syscall					# Prints the character
		
		j input					# Restarts

endCharLoop:	beq	$t2,	0,	noUp	      	# checks uppercase flag
		beq	$t3,	0,	noLow	      	# checks lowercase flag
		beq	$t4,	0,	noDig	      	# checks digit flag
		blt	$t1,	8,	badLength     	# checks if the length is 8 or more
		
		li	$v0,	SysPrintString       	# Loads the imidiate value into $v0 to start the syscall to print a string
		la	$a0,	passValid
		syscall

		j input					# Restarts
		
badLength:	li	$v0,	SysPrintString       	# Loads the imidiate value into $v0 to start the syscall to print a string
		la	$a0,	lengthShort		# Loads the lengthShrot message to be printed
		syscall					# Prints lengthShort
		
		li	$v0,	SysPrintInt		# Prints the integer value of the length
		la	$a0,	($t1)			# Loads the integer value of $t1 into $a0 to be printed
		syscall					# Prints the length of pass
		
		j input					# Restarts
		
noUp:		li	$v0,	SysPrintString       	# Loads the imidiate value into $v0 to start the syscall to print a string
		la	$a0,	missUpMsg		# Loads the missing uppercase message
		syscall					# Prints the message
		
		j input					# Restarts

noLow:		li	$v0,	SysPrintString       	# Loads the imidiate value into $v0 to start the syscall to print a string
		la	$a0,	missLowMsg		# Loads the missing lowercase message
		syscall					# Prints the message
		
		j input					# Restarts
		
noDig:		li	$v0,	SysPrintString       	# Loads the imidiate value into $v0 to start the syscall to print a string
		la	$a0,	missDigMsg		# Loads th emissing digit message 
		syscall					# Prints the message
		
		j input					#Restarts
		
end:		li	$v0,	SysExit             	# syscall code for exit
    		syscall	
