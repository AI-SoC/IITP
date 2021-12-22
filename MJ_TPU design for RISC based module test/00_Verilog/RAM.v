 module RAM(
    input   i_SCLK,
    input   i_RESETB,

    input   i_RAM_WEB,
    input   i_RAM_OEB,
    input   [7:0] i_RAM_ADDR,

    input   [15:0] i_RAM_DATA,
 
    output  [15:0] o_RAM_DATA
 );
 
reg [15:0] RAM [0:255];

assign o_RAM_DATA = {{16{~i_RAM_OEB}} & RAM[i_RAM_ADDR]};

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        RAM[i_RAM_ADDR] <= 0;//not yet
    else if(~i_RAM_WEB)
        RAM[i_RAM_ADDR] <= i_RAM_DATA;
 
 endmodule
