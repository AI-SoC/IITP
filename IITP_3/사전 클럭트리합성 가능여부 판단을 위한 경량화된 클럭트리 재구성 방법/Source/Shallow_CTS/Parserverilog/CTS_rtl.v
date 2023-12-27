module CTS ( gnd, vdd, clk, on, cnt);

input gnd;
input vdd;
input clk;
input on;
output [1:0] cnt;

DFFPOSX1 DFFPOSX1_1 ( .gnd(gnd), .vdd(vdd), .CLK(clk), .D(gnd), .Q(d1_Q) );
DFFPOSX1 DFFPOSX1_2 ( .gnd(gnd), .vdd(vdd), .CLK(clk), .D(d0_Q), .Q(d2_Q) );
DFFPOSX1 DFFPOSX1_3 ( .gnd(gnd), .vdd(vdd), .CLK(clk), .D(d1_Q), .Q(d3_Q) );
XOR2X1 XOR2X1_1 ( .gnd(gnd), .vdd(vdd), .A(on), .B(d0_Q), .Y(_0__0_) );
NAND2X1 NAND2X1_1 ( .gnd(gnd), .vdd(vdd), .A(on), .B(d0_Q), .Y(_1_) );
XNOR2X1 XNOR2X1_1 ( .gnd(gnd), .vdd(vdd), .A(_1_), .B(d1_Q), .Y(_0__1_) );
BUFX2 BUFX2_1 ( .gnd(gnd), .vdd(vdd), .A(d2_Q), .Y(cnt[0]) );
BUFX2 BUFX2_2 ( .gnd(gnd), .vdd(vdd), .A(d3_Q), .Y(cnt[1]) );
DFFPOSX1 DFFPOSX1_4 ( .gnd(gnd), .vdd(vdd), .CLK(clk), .D(_0__0_), .Q(d0_Q) );
DFFPOSX1 DFFPOSX1_5 ( .gnd(gnd), .vdd(vdd), .CLK(clk), .D(_0__1_), .Q(d1_Q) );
DFFPOSX1 DFFPOSX1_6 ( .gnd(gnd), .vdd(vdd), .CLK(clk), .D(gnd), .Q(d0_Q) );
endmodule
