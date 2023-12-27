module CTS_BEFORE ( gnd, vdd, i_SRC_CLK, i_RESETB, i_ii,i_BYPASS,i_DFF, i_SET, o_CLK_DIV_2);

input gnd;
input vdd;
input i_SRC_CLK;
input i_RESETB;
input i_BYPASS;
input i_ii;
input i_DFF;
input i_SET;
output o_CLK_DIV_2;

BUFX2 BUFX2_1 ( .gnd(gnd), .vdd(vdd), .A(_0_), .Y(o_CLK_DIV_2) );
DFFSR DFFSR_1 ( .gnd(gnd), .vdd(vdd), .CLK(i_SRC_CLK), .D(i_DFF), .Q(rCLK_DIV_2), .R(i_RESETB), .S(i_SET) );
INVX1 INVX1_1 ( .gnd(gnd), .vdd(vdd), .A(rCLK_DIV_2), .Y(n1) );
NOR2X1 NOR2X1_1 ( .gnd(gnd), .vdd(vdd), .A(i_BYPASS), .B(n1), .Y(_1_) );
AND2X2 AND2X2_1 ( .gnd(gnd), .vdd(vdd), .A(i_ii), .B(n1), .Y(_2_) );
NAND2X1 NAND2X1_1 ( .gnd(gnd), .vdd(vdd), .A(_1_), .B(_2_), .Y(_0_) );
endmodule
