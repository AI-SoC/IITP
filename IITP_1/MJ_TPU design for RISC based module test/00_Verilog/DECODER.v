`define D_MOV0    0
`define D_MOV1    1
`define D_MOV2    2
`define D_MOV3    3
`define D_ADD     4
`define D_SUB     5
`define D_JZ      6

module DECODER(
    input   i_SCLK,
    input   i_RESETB,
    input   i_WR_INST,
    input   [15:0]i_DO,

    output  [6:0]   o_OPCODE,
    output  [3:0]   o_OP1,
    output  [7:0]   o_OP2
);

wire [3:0] OPCODE;
wire [3:0] OP1;
wire [7:0] OP2;

assign OPCODE = {4{i_WR_INST}} & i_DO [15:12];
assign OP1    = {4{i_WR_INST}} & i_DO [11:8];
assign OP2    = {8{i_WR_INST}} & i_DO [7:0];

reg [6:0] R_OPCODE;
reg [3:0] R_OP1;
reg [7:0] R_OP2;

assign o_OPCODE = R_OPCODE;
assign o_OP1 = R_OP1;
assign o_OP2 = R_OP2;

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        R_OPCODE <= 0;
    else if(i_WR_INST)
        case(OPCODE)
            `D_MOV0 : R_OPCODE <= 7'b0000001;
            `D_MOV1 : R_OPCODE <= 7'b0000010;
            `D_MOV2 : R_OPCODE <= 7'b0000100;
            `D_MOV3 : R_OPCODE <= 7'b0001000;
            `D_ADD  : R_OPCODE <= 7'b0010000;
            `D_SUB  : R_OPCODE <= 7'b0100000;
            `D_JZ   : R_OPCODE <= 7'b1000000;
        endcase

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        R_OP1 <= 0;
    else if(i_WR_INST)
        R_OP1 <= OP1;

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        R_OP2 <= 0;
    else if(i_WR_INST)
        R_OP2 <= OP2;
/*
reg [6:0] OPCODE;

assign o_OPCODE = OPCODE;
assign o_OP1    = OP1_REG;
assign o_OP2    = OP2_REG;





always @ (*)
    if(~i_RESETB)
        OPCODE_REG<=0;
    else if(i_WR_INST)
        OPCODE_REG<=i_DO[15:12];
    else
        OPCODE_REG<=OPCODE_REG;

always @ (*)
    if(~i_RESETB)
        OP1_REG<=0;
    else if(i_WR_INST)
        OP1_REG<=i_DO[11:8];
    else
        OP1_REG<=OP1_REG;

always @(*)
    if(~i_RESETB)
        OP2_REG<=0;
    else if(i_WR_INST)
        OP2_REG<=i_DO[7:0];
    else
        OP2_REG<=OP2_REG;

always @(negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        OPCODE <= 0;
    else
     case(OPCODE_REG)
        `MOV0 : OPCODE <= 7'b0000001;
        `MOV1 : OPCODE <= 7'b0000010;
        `MOV2 : OPCODE <= 7'b0000100;
        `MOV3 : OPCODE <= 7'b0001000;
        `ADD  : OPCODE <= 7'b0010000;
        `SUB  : OPCODE <= 7'b0100000;
        `JZ   : OPCODE <= 7'b1000000;
    endcase
        
*/
endmodule


