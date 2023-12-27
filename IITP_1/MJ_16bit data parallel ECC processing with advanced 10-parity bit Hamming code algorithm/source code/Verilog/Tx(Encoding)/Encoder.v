module Tx32(
	input i_SCLK,
	input i_RESETB,
	input [15:0] i_DATA,
	
	output [24:0] o_DATA
);

reg [37:0] DATA;

assign o_DATA = DATA;

wire [9:0] PARITY;

assign PARITY[0] = i_DATA[0] ^ i_DATA[1] ^ i_DATA[4] ^ i_DATA[5] ^ i_DATA[7];//p6

assign PARITY[1] = i_DATA[1] ^ i_DATA[3] ^ i_DATA[4] ^ i_DATA[6] ^ i_DATA[7]; //p5

assign PARITY[2] = i_DATA[2] ^ i_DATA[3] ^ i_DATA[4]; //p4

assign PARITY[3] = i_DATA[5] ^ i_DATA[6] ^ i_DATA[7] ^ i_DATA[8]; //p3



assign PARITY[4] = i_DATA[8] ^ i_DATA[9] ^ i_DATA[11];

assign PARITY[5] = i_DATA[8] ^ i_DATA[10] ^ i_DATA[11];

assign PARITY[6] = i_DATA[9] ^ i_DATA[10] ^ i_DATA[11];



assign PARITY[7] = i_DATA[12] ^ i_DATA[13] ^ i_DATA[15];

assign PARITY[8] = i_DATA[12] ^ i_DATA[14] ^ i_DATA[15];

assign PARITY[9] = i_DATA[13] ^ i_DATA[14] ^ i_DATA[15];




always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[0] = 0;
	else DATA[0] = PARITY[0];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[1] = 0;
	else DATA[1] = PARITY[0];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[2] = 0;
	else DATA[2] = i_DATA[1];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[3] = 0;
	else DATA[3] = PARITY[2];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[4] = 0;
	else DATA[4] = i_DATA[2];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[5] = 0;
	else DATA[5] = i_DATA[3];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[6] = 0;
	else DATA[6] = i_DATA[4];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[7] = 0;
	else DATA[7] = PARITY[3];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[8] = 0;
	else DATA[8] = i_DATA[5];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[9] = 0;
	else DATA[9] = i_DATA[6];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[10] = 0;
	else DATA[10] = i_DATA[7];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[11] = 0;
	else DATA[11] = i_DATA[8];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[12] = 0;
	else DATA[12] = PARITY[4];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[13] = 0;
	else DATA[13] = PARITY[5];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[14] = 0;
	else DATA[14] = i_DATA[9];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[15] = 0;
	else DATA[15] = PARITY[6];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[16] = 0;
	else DATA[16] = i_DATA[10];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[17] = 0;
	else DATA[17] = i_DATA[11];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[18] = 0;
	else DATA[18] = i_DATA[12];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[19] = 0;
	else DATA[19] = PARITY[7];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[20] = 0;
	else DATA[20] = PARITY[8];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[21] = 0;
	else DATA[21] = i_DATA[13];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[22] = 0;
	else DATA[22] = PARITY[9];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[23] = 0;
	else DATA[23] = i_DATA[14];

always @ (negedge i_RESETB or posedge i_SCLK)
	if(~i_RESETB)
		DATA[24] = 0;
	else DATA[24] = i_DATA[15];

endmodule
