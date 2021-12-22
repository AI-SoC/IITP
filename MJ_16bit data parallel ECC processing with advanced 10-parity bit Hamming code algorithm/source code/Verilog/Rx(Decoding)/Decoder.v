module ECC_DECODER(
    input i_SCLK,
    input i_RESETB,
    input i_WR_INST,
    input [25:0]i_DO,

    output [15:0]o_DO,
    output o_READY
);

wire [9:0] PARITY;
wire [9:0] PARITY_1;

wire [2:0] SYNDROME1;
wire [2:0] SYNDROME2;
wire [3:0] SYNDROME3;

reg ERROR;
reg [22:0] DATA;
assign o_DO = DATA;


assign PARITY[9] = i_DO[22];
assign PARITY[8] = i_DO[20];
assign PARITY[7] = i_DO[19];

assign PARITY[6] = i_DO[15];
assign PARITY[5] = i_DO[13];
assign PARITY[4] = i_DO[12];

assign PARITY[3] = i_DO[7];
assign PARITY[2] = i_DO[3];
assign PARITY[1] = i_DO[1];
assign PARITY[0] = i_DO[0];

assign PARITY_1[9] = i_DO[25] ^ i_DO[24] ^ i_DO[23] ;
assign PARITY_1[8] = i_DO[25] ^ i_DO[24] ^ i_DO[21] ;
assign PARITY_1[7] = i_DO[25] ^ i_DO[23] ^ i_DO[11] ;

assign PARITY_1[6] = i_DO[18] ^ i_DO[17] ^ i_DO[16] ;
assign PARITY_1[5] = i_DO[18] ^ i_DO[17] ^ i_DO[14] ;
assign PARITY_1[4] = i_DO[18] ^ i_DO[16] ^ i_DO[14] ;

assign PARITY_1[3] = i_DO[11] ^ i_DO[10] ^ i_DO[9] ^ i_DO[8] ;
assign PARITY_1[2] = i_DO[11] ^ i_DO[6] ^ i_DO[5] ^i_DO[4] ;
assign PARITY_1[1] = i_DO[10] ^ i_DO[9] ^ i_DO[6] ^ i_DO[5] ^ i_DO[2] ;
assign PARITY_1[0] = i_DO[10] ^ i_DO[8] ^ i_DO[6] ^ i_DO[4] ^i_DO[2] ;

assign SYNDROME1[2] = PARITY[9] ^ PARITY_1[9];
assign SYNDROME1[1] = PARITY[8] ^ PARITY_1[8];
assign SYNDROME1[0] = PARITY[7] ^ PARITY_1[7];

assign SYNDROME2[2] = PARITY[6] ^ PARITY_1[6];
assign SYNDROME2[1] = PARITY[5] ^ PARITY_1[5];
assign SYNDROME2[0] = PARITY[4] ^ PARITY_1[4];

assign SYNDROME3[3] = PARITY[3] ^ PARITY_1[3];
assign SYNDROME3[2] = PARITY[2] ^ PARITY_1[2];
assign SYNDROME3[1] = PARITY[1] ^ PARITY_1[1];
assign SYNDROME3[0] = PARITY[0] ^ PARITY_1[0];

//always @ (negedge i_RESETB or posedge i_SCLK)
//    if(~i_RESETB)
//        READY=0;


always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[15] <= 0;
    else if (SYNDROME1 == 3'd7)
        DATA[15] <= ~i_DO[25];
    else
        DATA[15] <= i_DO[25];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[14] <= 0;
    else if (SYNDROME1 == 3'd6)
        DATA[14] <= ~i_DO[24];
    else
        DATA[14] <= i_DO[24];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[13] <= 0;
    else if (SYNDROME1 == 3'd5)
        DATA[13] <= ~i_DO[23];
    else
        DATA[13] <= i_DO[23];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[12] <= 0;
    else if (SYNDROME1 == 3'd3)
        DATA[12] <= ~i_DO[21];
    else
        DATA[12] <= i_DO[21];

//////////////////////////////////////////////OPCODE

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[11] <= 0;
    else if (SYNDROME2 == 3'd7)
        DATA[11] <= ~i_DO[18];
    else
        DATA[11] <= i_DO[18];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[10] <= 0;
    else if (SYNDROME2 == 3'd6)
        DATA[10] <= ~i_DO[17];
    else
        DATA[10] <= i_DO[17];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[9] <= 0;
    else if (SYNDROME2 == 3'd5)
        DATA[9] <= ~i_DO[16];
    else
        DATA[9] <= i_DO[16];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[8] <= 0;
    else if (SYNDROME2 == 3'd3)
        DATA[8] <= ~i_DO[14];
    else
        DATA[8] <= i_DO[14];
////////////////////////////////////////////// OP1

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[7] <= 0;
    else if (SYNDROME2 == 3'd12)
        DATA[7] <= ~i_DO[11];
    else
        DATA[7] <= i_DO[11];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[6] <= 0;
    else if (SYNDROME2 == 3'd11)
        DATA[6] <= ~i_DO[10];
    else
        DATA[6] <= i_DO[10];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[5] <= 0;
    else if (SYNDROME2 == 3'd10)
        DATA[5] <= ~i_DO[9];
    else
        DATA[5] <= i_DO[9];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[4] <= 0;
    else if (SYNDROME2 == 3'd9)
        DATA[4] <= ~i_DO[8];
    else
        DATA[4] <= i_DO[8];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[3] <= 0;
    else if (SYNDROME2 == 3'd7)
        DATA[3] <= ~i_DO[6];
    else
        DATA[3] <= i_DO[6];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[2] <= 0;
    else if (SYNDROME2 == 3'd6)
        DATA[2] <= ~i_DO[5];
    else
        DATA[2] <= i_DO[5];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[1] <= 0;
    else if (SYNDROME2 == 3'd5)
        DATA[1] <= ~i_DO[4];
    else
        DATA[1] <= i_DO[4];

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        DATA[0] <= 0;
    else if (SYNDROME2 == 3'd3)
        DATA[0] <= ~i_DO[2];
    else
        DATA[0] <= i_DO[2];

//OP2

endmodule
