# Custom Language 

## Grammar

<pre><code>
{program}        ::= {function_list}
{function_list}  ::= {function} | {function} {function_list}

{function}       ::= "function" {identifier} "(" {parameter_list} ")" "{" {scope} "}"
{parameter_list} ::= {typed_identifier} | {typed_identifier} "," {parameter_list} | ""
{statement_list} ::= {statement} | {statement} ";" {statement_list}
{statement}      ::= {declaration} | {assignment} | {return_statement} | {if_statement} | {while_statement} | {expression}

{scope}          ::= {statement_list} | {statement_list} {scope}

{declaration}    ::= {type} {identifier}
{assignment}     ::= {identifier} "=" {expression}
{return_statement} ::= "return" {expression}
{if_statement}   ::= "if" "(" {expression} ")" "{" {scope} "}"
{while_statement} ::= "while" "(" {expression} ")" "{" {scope} "}"

{expression}     ::= {term} | {term} {operator} {expression}
{term}           ::= {number} | {identifier} | "(" {expression} ")"
{operator}       ::= "+" | "-" | "*" | "/"

{typed_identifier} ::= {type} {identifier}
{type}           ::= "int" | "float" | "bool" | "string"  | "custom_type"  // Define more types as needed

{identifier}     ::= [a-zA-Z_][a-zA-Z0-9_]*
{number}         ::= [0-9]+

</code></pre>



