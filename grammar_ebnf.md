```
/* -------------------------------------------------------
   The Grammar
   ------------------------------------------------------- */

program          : linebreak (complete_commands)? linebreak ;
complete_commands: and_or (newline_list and_or)* ;
and_or           : pipeline ((AND_IF | OR_IF) linebreak pipeline)* ;
pipeline         : command ('|' linebreak command)* ;
command          : simple_command 
								 | subshell (io_redirect)* ;
subshell         : '(' compound_list ')' ;
compound_list    : linebreak complete_commands linebreak ;
simple_command   : cmd_prefix (cmd_word cmd_suffix)?
							   | cmd_name cmd_suffix ;
cmd_prefix       : (io_redirect | ASSIGNMENT_WORD)+ ;
cmd_suffix       : (io_redirect | WORD)* ;
cmd_name         : WORD                   /* Apply rule 7a */ ;
cmd_word         : WORD                   /* Apply rule 7b */ ;

io_redirect      : (IO_NUMBER)? io_file
                 | (IO_NUMBER)? io_here ;
io_file          : ('<' | '>' | DGREAT) filename ;
filename         : WORD                      /* Apply rule 2 */ ;

io_here          : DLESS     here_end
here_end         : WORD                      /* Apply rule 3 */ ;

newline_list     : NEWLINE+ ;
linebreak        : NEWLINE* ;
```