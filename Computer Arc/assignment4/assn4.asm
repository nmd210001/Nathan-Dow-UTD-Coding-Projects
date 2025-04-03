# Name:	 Nathan Dow
# NetID: nmd210001
# Discription:	Determine if the string is a palindrome. If so, print “This is a palindrome”. If not, print
#		“This is not a palindrome”.
		.include	"SysCalls.asm"
		.globl		main
		.data
prompt:		.asciiz 	"\nEnter a string to check: "
notPaliMsg:	.asciiz 	"This is not a palindrome"
isPaliMsg:	.asciiz 	"This is a palindrome"
text:		.space  21
		.text
main:		li	$v0,	SysPrintString		# Loads the imidiate value into $v0 to start the syscall to print a string
		la	$a0,	prompt
		syscall
		
		li	$v0,	SysReadString		# Loads the imidate value into $v0 to start the syscall to read a string
		la	$a0,	text			# Loads the input into the pass buffer
		li	$a1,	21		      	# Makes the maximum amount of bytes 20 so 20 characters
		syscall
		
		lb	$t3,	text	             	# using lb because we only need to load the first character of the buffer to see if it's null
		beq	$t3,	'\n',	end          	# if the first character of the string is nothing then branch to endloop pr
		
		la	$a0,	text			# Loads the address of text into the first argument
		jal	cleanStr			# Gets rid of all character values that are not upper or lower case letters
		
		la	$a0,	text			# Loads the address of text into the first argument
		jal	toUpper				# Coverts all of the letters to UpperCase
		
		la 	$a0,	text			# Loads the address of text into the first argument
    		move 	$t0, 	$v0 			# Move length of cleaned string (previousely calculated in cleanStr and stored in $v0) to $t0
    		li	$a1,	0			# Becouse is Pal is recursive it needs the first index as an argument
    		subi 	$a2, 	$t0,	1 		# Set the third argument of isPal to the last index
		jal	isPal				# Determines if the string is a pallendrom using recursion of comparing the first and last character
		
		beq 	$v0, 	1,	isPaliPrint	# If the return value of isPal is 1 then print is PaliMsg
		
notPali:	li	$v0,	SysPrintString		# If the return value of isPal does not equal 1 then print notPaliMsg
    		la	$a0,	notPaliMsg
    		syscall
		j main
		
isPaliPrint:	li	$v0,	SysPrintString		# Loads the imidiate value into $v0 to start the syscall to print a string
		la	$a0,	isPaliMsg
		syscall
		
		j	main


end:		li	$v0,	SysExit             	# syscall code for exit
    		syscall	
