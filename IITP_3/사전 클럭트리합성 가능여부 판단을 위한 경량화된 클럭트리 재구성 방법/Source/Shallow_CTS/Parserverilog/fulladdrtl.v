module fulladd ( gnd, vdd, a, b, cin, out, cout);

input gnd;
input vdd;
input a;
input b;
input cin;
output out;
output cout;

OR2X2 OR2X2_1 ( .gnd(gnd), .vdd(vdd), .A(b), .B(a), .Y(_4_) );
NAND2X1 NAND2X1_1 ( .gnd(gnd), .vdd(vdd), .A(b), .B(a), .Y(_5_) );
NAND3X1 NAND3X1_1 ( .gnd(gnd), .vdd(vdd), .A(_3_), .B(_5_), .C(_4_), .Y(_6_) );
NOR2X1 NOR2X1_1 ( .gnd(gnd), .vdd(vdd), .A(b), .B(a), .Y(_0_) );
AND2X2 AND2X2_1 ( .gnd(gnd), .vdd(vdd), .A(b), .B(a), .Y(_1_) );
OAI21X1 OAI21X1_1 ( .gnd(gnd), .vdd(vdd), .A(_1_), .B(_0_), .C(cin), .Y(_2_) );
NAND2X1 NAND2X1_2 ( .gnd(gnd), .vdd(vdd), .A(_2_), .B(_6_), .Y(_8_) );
OAI21X1 OAI21X1_2 ( .gnd(gnd), .vdd(vdd), .A(_0_), .B(_3_), .C(_5_), .Y(_7_) );
BUFX2 BUFX2_1 ( .gnd(gnd), .vdd(vdd), .A(_7_), .Y(cout) );
BUFX2 BUFX2_2 ( .gnd(gnd), .vdd(vdd), .A(_8_), .Y(out) );
INVX1 INVX1_1 ( .gnd(gnd), .vdd(vdd), .A(cin), .Y(_3_) );
endmodule

