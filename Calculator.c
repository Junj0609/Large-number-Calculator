/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Junjie Chen 1468909
   Dated:     01/09/2023

*/

/*algorithm is fun*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE	500	/* max number of digits per integer value */
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */

#define ERROR	(-1)	/* error return value from some functions */
#define PROMPT	"> "	/* the prompt string for interactive input */

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */
#define CH_NINE  '9'    /* character nine */

#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */

#define INT_ZERO 0	/* integer 0 */
#define INT_ONE 1	/* integer 1 */
#define INT_TEN  10	/* integer 10 */

/* Placeholder typedef for skeleton code
*/
typedef int longint_t[INTSIZE + 1];
#define LONG_ZERO 0
#define LONG_ONE  1


/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);

/* Skeleton program function prototypes */

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t *vars, char *line);
int  get_second_value(longint_t *vars, char *rhsarg,
	longint_t *second_value);
int  to_varnum(char ident);
void do_print(int varnum, longint_t *var);
void do_assign(longint_t *var1, longint_t *var2);
void do_plus(longint_t *var1, longint_t *var2);
void zero_vars(longint_t* vars);
void parse_num(char *rhs,longint_t *second_value);
void do_mult(longint_t *var1, longint_t *var2);
void do_power(longint_t *var1, int power);
int get_larger_value(int num1, int num2);
int convert_to_int(longint_t *var);

/****************************************************************/

/* Main program controls all the action
*/
int
main(int argc, char *argv[]) {
	char line[LINELEN+1] = {0};
	longint_t vars[NVARS];

	zero_vars(vars);
	print_prompt();
	while (read_line(line, LINELEN)) {
		if (strlen(line) > 0 && strlen(line) < INTSIZE+3) {
			/* non empty line, so process it */
			process_line(vars, line);
		}else if(strlen(line) > INTSIZE){
			printf("Number is too big, cannot process.\n");
			exit(EXIT_FAILURE);
		}
		print_prompt();
	}

	print_tadaa();
	return 0;
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
   can be confirmed that the input is coming from a terminal.
   Plus, output might be going to a file, that's why the prompt,
   if required, is written to stderr and not stdout
*/
void
print_prompt(void) {
	if (isatty(fileno(stdin))) {
		fprintf(stderr, "> ");
		fflush(stderr);
	}
}

void
print_tadaa() {
	/* all done, so pack up bat and ball and head home,
	   getting the exact final lines right is a bit tedious,
	   because input might be coming from a file and output
	   might be going to a file */
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		printf("\n");
	}
	printf("ta daa!!!\n");
	if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
		fprintf(stderr, "\n");
	}
}

void
print_error(char *message) {
	/* need to write an error message to the right place(s)
	*/
	if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
		fprintf(stderr, "%s\n", message);
		fflush(stderr);
	}
	if (!isatty(fileno(stdout))) {
		printf("%s\n", message);
	}
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
   returns false if there is no input available.
   All whitespace characters are removed on the way through.
*/
int
read_line(char *line, int maxlen) {
	int i=0, c;
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (i<maxlen && !isspace(c)) {
			line[i++] = c;
		}
	}
	line[i] = '\0';
	/* then, if the input is coming from a file or the output
	   is going to a file, it is helpful to echo the input line
	   and record what the command was */
	if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
		printf("%s%s\n", PROMPT, line);
	}
	return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* Process a command by parsing the input line into parts
*/
void
process_line(longint_t *vars, char *line) {
	int varnum, optype, status;
	longint_t second_value;
	/* determine the LHS variable, it
	   must be first character in compacted line
	*/
	varnum = to_varnum(line[0]);
	if (varnum==ERROR) {
		print_error("invalid LHS variable");
		return;
	}

	/* more testing for validity 
	*/
	if (strlen(line)<2) {
		print_error("no operator supplied");
		return;
	}

	/* determine the operation to be performed, it
	   must be second character of compacted line
	*/
	optype = line[1];
	if (strchr(ALLOPS, optype) == NULL) {
		print_error("unknown operator\n");
		return;
	}

	/* determine the RHS argument (if one is required),
	   it must start in the third character of compacted line
	*/
	if (optype != PRINT) {
		if (strlen(line)<3) {
			print_error("no RHS supplied");
			return;
		}
		status = get_second_value(vars, line+2, &second_value);
		if (status==ERROR) {
			print_error("RHS argument is invalid");
			return;
		}
	}

	/* finally, do the actual operation
	*/
	if (optype == PRINT) {
		do_print(varnum, vars);
	} else if (optype == ASSIGN) {
		do_assign(vars+varnum, &second_value);
	} else if (optype == PLUS) {
		do_plus(vars+varnum, &second_value);
	}else if (optype == MULT){
		do_mult(vars+varnum, &second_value);
	}else if(optype == POWR){
		int power = convert_to_int(&second_value);
		do_power(vars+varnum, power);
	}else {
		print_error("operation not available yet");
		return;
	}
	return;
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
*/
int
to_varnum(char ident) {
	int varnum;
	varnum = ident - CH_A;
	if (0<=varnum && varnum<NVARS) {
		return varnum;
	} else {
		return ERROR;
	}
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int
get_second_value(longint_t *vars, char *rhsarg,
			longint_t *second_value) {
	char *p;
	int varnum2;
	if (isdigit(*rhsarg)) {
		/* first character is a digit, so RHS is a number
		   now check the rest of RHS for validity */
		for (p=rhsarg+1; *p; p++) {
			if (!isdigit(*p)) {
				/* nope, found an illegal character */
				return ERROR;
			}
		}
		/* nothing wrong, ok to convert */
		parse_num(rhsarg, second_value);
		return !ERROR;
	} else {
		/* argument is not a number, so should be a variable */
		varnum2 = to_varnum(*rhsarg);
		if (varnum2==ERROR || strlen(rhsarg)!=1) {
			/* nope, not a variable either */
			return ERROR;
		}
		/* and finally, get that variable's value */
		do_assign(second_value, vars+varnum2);
		return !ERROR;
	}
	return ERROR;
}

/* Set the vars array to all zero values
*/
void
zero_vars(longint_t *vars) {
	int i;
	longint_t zero;
	zero[0] = LONG_ZERO;
	zero[1] = LONG_ZERO;
	for (i=0; i<NVARS; i++) {
		do_assign(vars+i, &zero);
	}
	return;
}

/*****************************************************************
******************************************************************

Your answer to the assignment should start here, using your new
typedef defined at the top of the program. The next few functions
will require modifications because of the change of structure
used for a long_int, and then you'll need to start adding whole
new functions after you get these first ones working properly.
Try and make the new functions fit the style and naming pattern
of the existing ones, ok?

******************************************************************
*****************************************************************/

/* Create an internal-format number out of a string
*/
void
parse_num(char *rhs,longint_t *second_value) {
	int len = strlen(rhs);
	(*second_value)[0] = len;
	/*set all elements in the array as integer 0*/
	for (int j = len; j>0;j--){
		(*second_value)[j] = *(rhs+len-j) - CH_ZERO;
	}
	return;
}

/****************************************************************/

/* Print out a longint value
*/
void
do_print(int varnum, longint_t *var) {
	printf("register %c: ", varnum+CH_A);
	int len = (*(var+varnum))[0];
	/*as digits are put in reversed order in the array, print backwards*/
	for (int i = len;i>0 ; i--){
		if(i%PUT_COMMAS == 0 && i !=len){
			printf(",");
		}
		printf("%d", (*(var+varnum))[i]);
	}
	printf("\n");
}

/****************************************************************/

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2

*/
void
do_assign(longint_t *var1, longint_t *var2) {
	for (int i = 0; i<=(*var2)[0]; i++){
		(*var1)[i] = (*var2)[i];
	}		
}

/****************************************************************/

/* Update the indicated variable var1 by doing an addition
   using var2 to compute var1 = var1 + var2
*/
void do_plus(longint_t *var1, longint_t *var2) {
    int len_sec = (*var2)[0];
    int len_var = (*var1)[0];
    int next_digit = INT_ZERO;
    int max_len = get_larger_value(len_sec, len_var);
    
    for (int i = 1; i <= max_len || next_digit; i++) { 
        int sum = next_digit;
        /*adding up digist if it is valid*/
		if (i <= len_var) {
            sum += (*var1)[i];
        }
        if (i <= len_sec) {
            sum += (*var2)[i];
        }
        /*if sum greater than 10 in a digit, then 
		  a value need to be carried into next digit*/
        next_digit = sum / INT_TEN;
        
        if (i <= len_var) {
            (*var1)[i] = sum % INT_TEN;
        } else {
            len_var += INT_ONE;
            (*var1)[0] += INT_ONE;
            (*var1)[i] = sum % INT_TEN;
        }
    }
	/*check if it's valid*/
	if((*var1)[0]>INTSIZE){
		printf("Number is too big, cannot process.\n");
		exit(EXIT_FAILURE);
	}
}

	


/*****************************************************************
******************************************************************

Put your new functions below this line. Make sure you add suitable
prototypes at the top of the program.

******************************************************************
*****************************************************************/

/*multiply the variables given, var1 = var1*var2*/
void
do_mult(longint_t *var1, longint_t *var2){
	int len_sec = (*var2)[0];
    int len_var = (*var1)[0];
    int round_up = INT_ZERO; 
	int total_len = len_sec + len_var - 1;
	int temp_num[total_len+1];
	/*set all elements in the temperary array to all zeros*/
	for(int a = 1; a<= total_len; a++){
		temp_num[a] = LONG_ZERO;
	}
	temp_num[0] = total_len;
	int prod=INT_ZERO;
	/*do the multiplication, add up the previous value on the same digit*/
	for(int i = 1; i <= len_var; i++){
		round_up = INT_ZERO;
		for (int j = 1; j <= len_sec; j++){
			prod = ((*var1)[i]) * ((*var2)[j]) + temp_num[i + j - 1] + round_up;
            temp_num[i + j - 1] = prod % INT_TEN;
            round_up = prod / INT_TEN;
        }
        temp_num[i + len_sec] = round_up;
	}
	/*check if addtional digit is needed to be added*/
	if (temp_num[total_len + 1] != 0){
		total_len += 1;
	}	
	temp_num[0] = total_len;
	if(temp_num[0] > INTSIZE){
		printf("Number is too big, cannot process.\n");
		exit(EXIT_FAILURE);
	}
	/*copy the data in the temperary array into var1*/
	do_assign(var1, &temp_num);
}

/*calculate var1 = var1^var2*/
void 
do_power(longint_t *var1, int power){
	int temp_num[INTSIZE+1];
	/*copy data in var1 into temperary array for power calc*/
	do_assign(&temp_num, var1);
	for (int i =1; i < power; i++){
		do_mult(var1,&temp_num);
	}
}

/*conpare the value of two integers*/
int
get_larger_value(int num1, int num2){
	if(num1 > num2){
		return num1;
	}else{
		return num2;
	}
}

/*conver the array of the number into integer*/
int 
convert_to_int(longint_t *var){
	int digit_num = (*var)[0];
	int int_var = INT_ZERO;
	for(int i = digit_num; i > 0; i--){
		if (i != INT_ONE){
			int_var = (int_var + (*var)[i])*INT_TEN;
		}else{
			int_var = int_var + (*var)[i];
		}	
	}
	return int_var;
}

