# Custom Language 

## Grammar

<pre><code>
{program}        ::= {function_list}
{function_list}  ::= {function} {function_list} | ε

{function}              ::= "function" {identifier} "(" {parameter_list} ")" "{" {scope} "}"
{parameter_list}        ::= {typed_identifier_list} | ε
{typed_identifier_list} ::= {typed_identifier} ("," {typed_identifier})*
{typed_identifier}      ::= {type} {identifier}

{function_call} ::= {identifier} "(" {argument_list} ")"
{argument_list} ::= {expression} ("," {expression})* | ε

{scope}          ::= {statement_list}
{statement_list} ::= {statement} {statement_list} | ε
{statement}      ::= {declaration} ";"
                   | {assignment} ";"
                   | {return_statement} ";"
                   | {if_statement}
                   | {while_statement}


{declaration}        ::= {type} {identifier}
{assignment}         ::= {identifier} "=" {expression}
{return_statement}   ::= "return" {expression}
{if_statement}       ::= "if" "(" {expression} ")" {scope}
{while_statement}    ::= "while" "(" {expression} ")" {scope}

{expression}                 ::= {logical_or_expression}
{logical_or_expression}      ::= {logical_and_expression} ("||" {logical_and_expression})*
{logical_and_expression}     ::= {equality_expression} ("&&" {equality_expression})*
{equality_expression}        ::= {relational_expression} (("==" | "!=") {relational_expression})*
{relational_expression}      ::= {additive_expression} (("<" | ">" | "<=" | ">=") {additive_expression})*
{additive_expression}        ::= {multiplicative_expression} (("+" | "-") {multiplicative_expression})*
{multiplicative_expression}  ::= {unary_expression} (("*" | "/") {unary_expression})*
{unary_expression}           ::= ("+" | "-" | "!") {unary_expression} | {primary_expression}
{primary_expression} ::= {number} | {identifier} | {function_call} | "(" {expression} ")"

{type}           ::= "int" | "float" | "bool" | "string" | "custom_type"
{identifier}     ::= [a-zA-Z_][a-zA-Z0-9_]*
{number}         ::= [0-9]+ ("." [0-9]+)?

</code></pre>



