module TPU(
    input i_SCLK,
    input i_RESETB,
    input i_REB,
    output  o_RD_RQST,
    output  [6:0] o_ADDR
); 

wire RD_RQST;
wire [6:0] ADDR;
wire READY;
wire WR_INST;
wire [15:0] DO;
wire [6:0] OPCODE;
wire [3:0] OP1;
wire [7:0] OP2;
wire [15:0] RY;
wire [15:0] RX;
wire [2:0]  ALU_ENABLE;
wire [15:0] RESULT;
wire ALU_READY;
wire RAM_WEB;
wire RAM_OEB;
wire [7:0] RAM_ADDR;
wire [15:0] I_RAM_DATA;
wire [15:0] O_RAM_DATA;



assign o_RD_RQST = RD_RQST;
assign o_ADDR = ADDR;

MCON mcon_u0(
    .i_SCLK(i_SCLK),
    .i_RESETB(i_RESETB),
    .i_REB(i_REB),

    .i_ROM_READY(READY), //
    
    .i_OPCODE(OPCODE),
    .i_OP1(OP1),
    .i_OP2(OP2),

    .i_RESULT(RESULT),
    .i_ALU_READY(ALU_READY),

    .i_RAM_DATA(I_RAM_DATA),

    .o_RD_RQST(RD_RQST),
    .o_ADDR(ADDR),

    .o_WR_INST(WR_INST),

    .o_RX(RX),
    .o_RY(RY),
    .o_ALU_ENABLE(ALU_ENABLE),

    .o_RAM_WEB(RAM_WEB),
    .o_RAM_OEB(RAM_OEB),
    .o_RAM_ADDR(RAM_ADDR),
    .o_RAM_DATA(O_RAM_DATA)

);

EXROM exrom_u0(
    .i_SCLK(i_SCLK),
    .i_RESETB(i_RESETB),
    .i_RD_RQST(RD_RQST),
    .i_ADDR(ADDR),

    .o_DO(DO),
    .o_READY(READY)
);

DECODER decoder_u0(
    .i_SCLK(i_SCLK),
    .i_RESETB(i_RESETB),
    .i_WR_INST(WR_INST),
    .i_DO(DO),

    .o_OPCODE(OPCODE),
    .o_OP1(OP1),
    .o_OP2(OP2)
);

ALU alu_u0(
    .i_SCLK(i_SCLK),
    .i_RESETB(i_RESETB),
    .i_RX(RX),
    .i_RY(RY),
    .i_ALU_ENABLE(ALU_ENABLE),

    .o_ALU_READY(ALU_READY),
    .o_RESULT(RESULT)
);

RAM ram_u0(
    .i_SCLK(i_SCLK),
    .i_RESETB(i_RESETB),
    .i_RAM_WEB(RAM_WEB),
    .i_RAM_OEB(RAM_OEB),
    .i_RAM_ADDR(RAM_ADDR),
    .i_RAM_DATA(O_RAM_DATA),
    .o_RAM_DATA(I_RAM_DATA)
);
endmodule
