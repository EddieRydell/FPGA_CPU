// Function for Floating-Point Addition
function void add(input logic sign1, input logic [7:0] exp1, input logic [23:0] mantissa1,
                  input logic sign2, input logic [7:0] exp2, input logic [23:0] mantissa2,
                  output logic [31:0] result);
    logic [7:0] exponent_diff;
    logic [23:0] mantissa_shifted, mantissa_result;
    logic [7:0] exponent_result;

    if (exp1 > exp2) begin
        exponent_diff = exp1 - exp2;
        mantissa_shifted = mantissa2 >> exponent_diff;
        mantissa_result = mantissa1 + mantissa_shifted;
        exponent_result = exp1;
    end else begin
        exponent_diff = exp2 - exp1;
        mantissa_shifted = mantissa1 >> exponent_diff;
        mantissa_result = mantissa2 + mantissa_shifted;
        exponent_result = exp2;
    end

    // Normalize result (simple normalization)
    if (mantissa_result[23]) begin
        mantissa_result = mantissa_result >> 1;
        exponent_result = exponent_result + 1;
    end

    // Assign result back
    result = {sign1, exponent_result, mantissa_result[22:0]};
endfunction

// Function for Floating-Point Subtraction
function void subtract(input logic sign1, input logic [7:0] exp1, input logic [23:0] mantissa1,
                       input logic sign2, input logic [7:0] exp2, input logic [23:0] mantissa2,
                       output logic [31:0] result);
    logic [7:0] exponent_diff;
    logic [23:0] mantissa_shifted, mantissa_result;
    logic [7:0] exponent_result;

    if (exp1 > exp2) begin
        exponent_diff = exp1 - exp2;
        mantissa_shifted = mantissa2 >> exponent_diff;
        mantissa_result = mantissa1 - mantissa_shifted;
        exponent_result = exp1;
    end else begin
        exponent_diff = exp2 - exp1;
        mantissa_shifted = mantissa1 >> exponent_diff;
        mantissa_result = mantissa2 - mantissa_shifted;
        exponent_result = exp2;
    end

    // Normalize result
    if (mantissa_result[23]) begin
        mantissa_result = mantissa_result >> 1;
        exponent_result = exponent_result + 1;
    end

    // Assign result back
    result = {sign1, exponent_result, mantissa_result[22:0]};
endfunction

// Function for Floating-Point Multiplication
function void multiply(input logic sign1, input logic [7:0] exp1, input logic [23:0] mantissa1,
                       input logic sign2, input logic [7:0] exp2, input logic [23:0] mantissa2,
                       output logic [31:0] result);
    logic [47:0] mantissa_product;
    logic result_sign;
    logic [7:0] exponent_result;

    // Multiply mantissas and add exponents
    mantissa_product = mantissa1 * mantissa2;
    exponent_result = exp1 + exp2 - 127;
    result_sign = sign1 ^ sign2;

    // Normalize result
    if (mantissa_product[47]) begin
        mantissa_product = mantissa_product >> 1;
        exponent_result = exponent_result + 1;
    end

    // Assign result back
    result = {result_sign, exponent_result, mantissa_product[46:24]};
endfunction

// Function for Floating-Point Division (simplified)
function void divide(input logic sign1, input logic [7:0] exp1, input logic [23:0] mantissa1,
                     input logic sign2, input logic [7:0] exp2, input logic [23:0] mantissa2,
                     output logic [31:0] result);
    logic [23:0] mantissa_quotient;
    logic result_sign;
    logic [7:0] exponent_result;

    // Divide mantissas and subtract exponents
    if (mantissa2 != 0) begin
        mantissa_quotient = mantissa1 / mantissa2;
        exponent_result = exp1 - exp2 + 127;
        result_sign = sign1 ^ sign2;
    end else begin
        mantissa_quotient = 24'b0;  // Handle divide-by-zero
        exponent_result = 8'b11111111;  // Infinity
        result_sign = sign1;
    end

    // Assign result back
    result = {result_sign, exponent_result, mantissa_quotient[22:0]};
endfunction
