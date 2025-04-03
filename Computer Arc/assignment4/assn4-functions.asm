# Name:	 Nathan Dow
# NetID: nmd210001
# Discription:	Determine if the string is a palindrome. If so, print “This is a palindrome”. If not, print
#		“This is not a palindrome”.
		.globl		cleanStr, toUpper, isPal
		.data	
		.text
cleanStr:	subi	$sp,	$sp,	4		# Makes space for a word
		sw	$ra,	($sp)			# Stores the return address on the stack
		
		move	$t0,	$a0			# copies the argument(the text input) into the to be clear address
		move	$t1,	$a0			# copies the argument(the text input) into the to be same as original address
		
cleanLoop:	lb 	$t5,	($t1)	      		# loads a char of string starting at $t1 to $t5
		beq	$t5,	'\n',	endClean   	# if the char is null it is the end of the word 
						
checkUp:	blt	$t5,	65,	checkDig	# Sees if the ASCII value is less than A if it is check if it's a digit
		bgt	$t5,	90,	checkLow	# Sees if the ASCII value is greater than Z if it is check if it's a lowercase
		j	addToCleared			# Else it must be an uppercase
		
checkLow:	blt	$t5,	97,	nextCharClean	# If the character's ASCII is greater than 90 and less than 97 it must be a special
		bgt	$t5,	123,	nextCharClean	# If the character is greater than 123 then it must be a special
		j	addToCleared			# Else it is a lowercase 
		
checkDig:	blt	$t5,	48,	nextCharClean	# If the ASCII value is less than 48 then it is a special character
		bgt	$t5,	57,	nextCharClean	# If the ASCII value is less than 65 and greater than 57 it is a special character
		j	addToCleared			# Else it is a digit 


addToCleared:						# If the current char is not alphanumeric it will skip addToCleared
  		sb	$t5,	($t0)      		# Store cleaned character in the same location as the original string
  		addi	$t0,	$t0,	1    		# Move to the next character in the input string
  		j	nextCharClean

nextCharClean:	addi	$t1,	$t1,	1    		# Move to the next character in the input string
		j	cleanLoop
		
				
endClean:	li	$t7,	10
		sb	$t7,	($t0)			# Sets the char after the reassigned chares of $t0 to null to represent the end
		sub	$v0,	$t0,	$a0		# Sets $v0 to the length of the now cleared string by subtracting the last cleared address from the original address
				
				
			
		lw	$ra,	($sp)			# Load the return address from the stack
    		addi 	$sp, 	$sp,	4            	# Restore the stack pointer by deallocating the space used for the return address
    		jr 	$ra                      	# Jump back to the address stored in $ra




toUpper:	subi	$sp,	$sp,	4		# Makes space for a word
		sw	$ra,	($sp)			# Stores the return address on the stack


		move	$t0,	$a0       		# $t0 points to the cleaned string(Should only be alphanumeric

toUpperLoop:	lb	$t1,	($t0)      		# Load a character from the cleaned string
        	beq	$t1,	'\n',	endUpper  	# Check for null terminator, if found, exit loop

        	blt	$t1,	97,	nextCharUpper	# If it is less than 97 it must be a digit or uppercase so skip
        	j	changeToUpper			# It must be a lowercase because there shouldn't be any special characters
        	
changeToUpper:	subi	$t1,	$t1,	32   		# Subtract 32 to convert lowercase to uppercase
        	sb	$t1,	($t0)      		# Store uppercase character back to memory
        	j	nextCharUpper

nextCharUpper:	addi	$t0,	$t0,	1    		# Move to the next character in the cleaned string
        	j	toUpperLoop
        			
endUpper:	lw	$ra,	($sp)			# Load the return address from the stack
    		addi 	$sp, 	$sp,	4            	# Restore the stack pointer by deallocating the space used for the return address
    		jr 	$ra                      	# Jump back to the address stored in $ra
		
		
		
		
isPal:		subi	$sp,	$sp,	4		# Makes space for a word
		sw	$ra,	($sp)			# Stores the return address on the stack

    		move	$t0,	$a0   			# Address of the cleaned string
    		move	$t1,	$a1   			# Lower index
    		move	$t2,	$a2   			# Upper index

    		ble	$t1,	$t2,	checkPali  	# Check if lower index is less than or equal to upper index If it is graeter than it must be a plaindrom
    		li	$v0,	1       		# Return true if lower index is greater than upper index
    		j	endPali

    		
checkPali:	add	$t3,	$t0,	$t1	
    		lb	$t3,	($t3)			# Load character from lower index
    		add	$t4,	$t0,	$t2
    		lb	$t4,	($t4)      		# Load character from upper inde
    		
    		beq	$t3,	$t4,	contPalCheck	# If characters match, continue palindrome check
    		li	$v0,	0               	# If characters don't match, return false
    		j	endPali

contPalCheck:	addi	$t1,	$t1,	1  		# Move lower index to the right
    		addi	$t2,	$t2,	-1       	# Move upper index to the left

		move	$a1,	$t1			# Sets the secound argument for the recursive call to be the new first index address
		move	$a2,	$t2			# Sets the secound argument for the recursive call to be the new last index address
    		jal 	isPal				# Recursive call to contine the check with the new parameters

endPali:	lw	$ra,	($sp)			# Load the return address from the stack
		addi 	$sp, 	$sp,	4            	# Restore the stack pointer by deallocating the space used for the return address
    		jr 	$ra                      	# Jump back to the address stored in $ra