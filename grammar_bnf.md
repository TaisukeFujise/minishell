/* -------------------------------------------------------
   The grammar symbols
   ------------------------------------------------------- */
   
%token  WORD
%token  ASSIGNMENT_WORD
%token  NEWLINE
%token  IO_NUMBER

/* The following are the operators (see XBD Operator)
   containing more than one character. */
%token  AND_IF    OR_IF    
/*      '&&'      '||'  　　　*/

%token  DLESS  DGREAT  
/*      '<<'   '>>'     */
/* -------------------------------------------------------
   The Grammar
   ------------------------------------------------------- */

program          : linebreak complete_commands linebreak
                 | linebreak
                 ;
complete_commands: complete_commands newline_list and_or
								 | and_or
								 ;
and_or           :                         pipeline
                 | and_or AND_IF linebreak pipeline
                 | and_or OR_IF  linebreak pipeline
                 ;
pipeline         : command
				         | pipeline '|' linebreak command
				         ;
command          : simple_command
				         | subshell
				         | subshell redirect_list
				         ;
subshell         : '(' compound_list ')'
                 ;
compound_list    : linebreak complete_commands
                 | linebreak complete_commands newline_list
                 ;
simple_command   : cmd_prefix cmd_word cmd_suffix
							   | cmd_prefix cmd_word
							   | cmd_prefix
							   | cmd_name cmd_suffix
							   | cmd_name
							   ;
cmd_prefix       :            io_redirect
                 | cmd_prefix io_redirect
                 |            ASSIGNMENT_WORD
                 | cmd_prefix ASSIGNMENT_WORD
                 ;
cmd_suffix       :            io_redirect
                 | cmd_suffix io_redirect
                 |            WORD
                 | cmd_suffix WORD
                 ;
cmd_name         : WORD                   /* Apply rule 7a */
                 ;
cmd_word         : WORD                   /* Apply rule 7b */
								 ;
redirect_list    :               io_redirect
                 | redirect_list io_redirect
                 ;
io_redirect      :           io_file
                 | IO_NUMBER io_file
                 |           io_here
                 | IO_NUMBER io_here
                 ;
io_file          : '<'       filename
                 | '>'       filename
                 | DGREAT    filename
                 ;
filename         : WORD                      /* Apply rule 2 */
								 ;
io_here          : DLESS     here_end
here_end         : WORD                      /* Apply rule 3 */
								 ;
newline_list     :              NEWLINE
                 | newline_list NEWLINE
                 ;
linebreak        : newline_list
                 | /* empty */
	               ;
