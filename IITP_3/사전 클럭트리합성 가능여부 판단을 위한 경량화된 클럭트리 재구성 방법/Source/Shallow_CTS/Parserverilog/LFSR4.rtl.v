module LFSR4 ( gnd, vdd, clk, rst, out);

input gnd, vdd;
input clk;
input rst;
output [3:0] out;

DFFSR DFFSR_1 ( .gnd(gnd), .vdd(vdd), .CLK(clk), .D(_1__1_), .Q(_1__2_), .R(_0_), .S(vdd) );
DFFSR DFFSR_2 ( .gnd(gnd), .vdd(vdd), .CLK(clk), .D(_1__2_), .Q(_1__3_), .R(_0_), .S(vdd) );
XNOR2X1 XNOR2X1_1 ( .gnd(gnd), .vdd(vdd), .A(_1__2_), .B(_1__3_), .Y(feedback) );
INVX4 INVX4_1 ( .gnd(gnd), .vdd(vdd), .A(rst), .Y(_0_) );
BUFX2 BUFX2_1 ( .gnd(gnd), .vdd(vdd), .A(_1__0_), .Y(out[0]) );
BUFX2 BUFX2_2 ( .gnd(gnd), .vdd(vdd), .A(_1__1_), .Y(out[1]) );
BUFX2 BUFX2_3 ( .gnd(gnd), .vdd(vdd), .A(_1__2_), .Y(out[2]) );
BUFX2 BUFX2_4 ( .gnd(gnd), .vdd(vdd), .A(_1__3_), .Y(out[3]) );
DFFSR DFFSR_3 ( .gnd(gnd), .vdd(vdd), .CLK(clk), .D(feedback), .Q(_1__0_), .R(_0_), .S(vdd) );
DFFSR DFFSR_4 ( .gnd(gnd), .vdd(vdd), .CLK(clk), .D(_1__0_), .Q(_1__1_), .R(_0_), .S(vdd) );
endmodule

