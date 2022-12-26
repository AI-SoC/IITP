`define     ALU_IDLE    0
`define     ALU_ADD     1
`define     ALU_SUB     2
`define     ALU_COMPARE 3
`define     ALU_COMPLETE 4
`define     ALU_WAIT    5
module ALU(
    input i_SCLK,
    input i_RESETB,
    input [15:0] i_RX,
    input [15:0] i_RY,
    input [2:0] i_ALU_ENABLE,
    
    output o_ALU_READY,
    output [15:0] o_RESULT
);

reg [2:0]   STATE;
reg [2:0]   NEXT_STATE;

reg [15:0] RESULT;
reg ALU_READY;

assign o_RESULT = RESULT;
assign o_ALU_READY = ALU_READY;

always @ (*)
    case(STATE)
        `ALU_IDLE :
            case(i_ALU_ENABLE)
                3'b000 : NEXT_STATE <= STATE;
                3'b001 : NEXT_STATE <= `ALU_COMPARE;
                3'b010 : NEXT_STATE <= `ALU_ADD;
                3'b100 : NEXT_STATE <= `ALU_SUB;
            endcase
        `ALU_COMPARE  : NEXT_STATE <= `ALU_COMPLETE;
        `ALU_ADD      : NEXT_STATE <= `ALU_COMPLETE;
        `ALU_SUB      : NEXT_STATE <= `ALU_COMPLETE;
        `ALU_COMPLETE : NEXT_STATE <= `ALU_WAIT;
        `ALU_WAIT   :
            if(i_ALU_ENABLE == 0)
                NEXT_STATE <= `ALU_IDLE;
            else
                NEXT_STATE <= STATE;
    endcase

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        STATE <= `ALU_IDLE;
    else STATE <= NEXT_STATE;


always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        RESULT <= 0;
    else if(STATE == `ALU_COMPARE)
        if(i_RX == 0)
            RESULT <=1;
        else RESULT <= 0;
    else if(STATE == `ALU_ADD)
        RESULT <= i_RX+i_RY;
    else if(STATE == `ALU_SUB)
        RESULT <= i_RX-i_RY;

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        ALU_READY<=0;
    else if(STATE == `ALU_COMPLETE)
        ALU_READY<=1;
    else ALU_READY <= 0;

endmodule
