`define IDLE    0
`define READ0   1
`define READ1   2
`define READ2   3
`define READ3   4
`define READ4   5
`define OUTPUT  6

module EXROM(
    input           i_SCLK,
    input           i_RESETB,
    
    input           i_RD_RQST,
    input  [6:0]    i_ADDR,

    output [15:0]    o_DO,
    output          o_READY
);

reg [2:0]   STATE;
reg [2:0]   NEXT_STATE;

reg [15:0] exrom [0:127];

reg [15:0] DO;
reg       READY;

assign o_DO = DO;
assign o_READY = READY;

always @ (*)
    case(STATE)
        `IDLE   : 
                if (~i_RD_RQST)
                    NEXT_STATE <= `READ0;
                else
                    NEXT_STATE <= STATE;
        `READ0  : NEXT_STATE <= `READ1;
        `READ1  : NEXT_STATE <= `READ2;
        `READ2  : NEXT_STATE <= `READ3;
        `READ3  : NEXT_STATE <= `READ4;
        `READ4  : NEXT_STATE <= `OUTPUT;
        `OUTPUT : 
                  if(i_RD_RQST)
                    NEXT_STATE <= `IDLE;
                  else
                    NEXT_STATE <=STATE;
    endcase

always @(negedge i_RESETB or posedge i_SCLK) //STATE sync
    if(~i_RESETB)
        STATE <= `IDLE;
    else
        STATE <= NEXT_STATE;
always @ (negedge i_RESETB or posedge i_SCLK) // READY   
    if(~i_RESETB)
        READY <= 1'b0;
    else if(STATE == `OUTPUT)
        READY <= 1'b1;
    else READY <= 1'b0;
always @(negedge i_RESETB or posedge i_SCLK) //OUTPUT DATA
    if(~i_RESETB)
        DO <= 16'b0;
    else if(STATE == `OUTPUT)
        DO <= exrom[i_ADDR];
    else DO <=0; 
        
endmodule
