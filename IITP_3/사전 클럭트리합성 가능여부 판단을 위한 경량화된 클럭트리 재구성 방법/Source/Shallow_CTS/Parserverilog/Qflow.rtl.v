module map9v3 ( gnd, vdd, clock, reset, start, N, dp, done, counter, sr);

input gnd;
input vdd;
input clock;
input reset;
input start;
output done;
input [8:0] N;
output [8:0] dp;
output [7:0] counter;
output [7:0] sr;

CLKBUF1 CLKBUF1_1 ( .gnd(gnd), .vdd(vdd), .A(clock), .Y(clock_bF_buf4) );
CLKBUF1 CLKBUF1_2 ( .gnd(gnd), .vdd(vdd), .A(clock), .Y(clock_bF_buf3) );
CLKBUF1 CLKBUF1_3 ( .gnd(gnd), .vdd(vdd), .A(clock), .Y(clock_bF_buf2) );
CLKBUF1 CLKBUF1_4 ( .gnd(gnd), .vdd(vdd), .A(clock), .Y(clock_bF_buf1) );
CLKBUF1 CLKBUF1_5 ( .gnd(gnd), .vdd(vdd), .A(clock), .Y(clock_bF_buf0) );
BUFX4 BUFX4_1 ( .gnd(gnd), .vdd(vdd), .A(_93_), .Y(_93__bF_buf3) );
BUFX4 BUFX4_2 ( .gnd(gnd), .vdd(vdd), .A(_93_), .Y(_93__bF_buf2) );
BUFX4 BUFX4_3 ( .gnd(gnd), .vdd(vdd), .A(_93_), .Y(_93__bF_buf1) );
BUFX4 BUFX4_4 ( .gnd(gnd), .vdd(vdd), .A(_93_), .Y(_93__bF_buf0) );
BUFX4 BUFX4_5 ( .gnd(gnd), .vdd(vdd), .A(_8_), .Y(_8__bF_buf4) );
BUFX4 BUFX4_6 ( .gnd(gnd), .vdd(vdd), .A(_8_), .Y(_8__bF_buf3) );
BUFX4 BUFX4_7 ( .gnd(gnd), .vdd(vdd), .A(_8_), .Y(_8__bF_buf2) );
BUFX4 BUFX4_8 ( .gnd(gnd), .vdd(vdd), .A(_8_), .Y(_8__bF_buf1) );
BUFX4 BUFX4_9 ( .gnd(gnd), .vdd(vdd), .A(_8_), .Y(_8__bF_buf0) );
BUFX4 BUFX4_10 ( .gnd(gnd), .vdd(vdd), .A(_104_), .Y(_104__bF_buf3) );
BUFX4 BUFX4_11 ( .gnd(gnd), .vdd(vdd), .A(_104_), .Y(_104__bF_buf2) );
BUFX4 BUFX4_12 ( .gnd(gnd), .vdd(vdd), .A(_104_), .Y(_104__bF_buf1) );
BUFX4 BUFX4_13 ( .gnd(gnd), .vdd(vdd), .A(_104_), .Y(_104__bF_buf0) );
INVX8 INVX8_1 ( .gnd(gnd), .vdd(vdd), .A(state_0_), .Y(_93_) );
NOR2X1 NOR2X1_1 ( .gnd(gnd), .vdd(vdd), .A(_113__3_), .B(_113__2_), .Y(_94_) );
NOR2X1 NOR2X1_2 ( .gnd(gnd), .vdd(vdd), .A(_113__1_), .B(_113__0_), .Y(_95_) );
NOR3X1 NOR3X1_1 ( .gnd(gnd), .vdd(vdd), .A(_113__5_), .B(_113__4_), .C(_113__6_), .Y(_96_) );
NAND3X1 NAND3X1_1 ( .gnd(gnd), .vdd(vdd), .A(_94_), .B(_95_), .C(_96_), .Y(_97_) );
OAI21X1 OAI21X1_1 ( .gnd(gnd), .vdd(vdd), .A(_97_), .B(_113__7_), .C(state_3_), .Y(_98_) );
NAND2X1 NAND2X1_1 ( .gnd(gnd), .vdd(vdd), .A(_93__bF_buf2), .B(_98_), .Y(_7_) );
INVX1 INVX1_1 ( .gnd(gnd), .vdd(vdd), .A(state_1_), .Y(_99_) );
INVX1 INVX1_2 ( .gnd(gnd), .vdd(vdd), .A(state_4_), .Y(_100_) );
INVX1 INVX1_3 ( .gnd(gnd), .vdd(vdd), .A(startbuf_0_), .Y(_101_) );
NOR2X1 NOR2X1_3 ( .gnd(gnd), .vdd(vdd), .A(startbuf_1_), .B(_101_), .Y(_102_) );
OAI21X1 OAI21X1_2 ( .gnd(gnd), .vdd(vdd), .A(_102_), .B(_99_), .C(_100_), .Y(_6_) );
INVX1 INVX1_4 ( .gnd(gnd), .vdd(vdd), .A(_115__1_), .Y(_103_) );
INVX8 INVX8_2 ( .gnd(gnd), .vdd(vdd), .A(state_3_), .Y(_104_) );
NAND3X1 NAND3X1_2 ( .gnd(gnd), .vdd(vdd), .A(state_2_), .B(_93__bF_buf2), .C(_104__bF_buf1), .Y(_105_) );
NOR2X1 NOR2X1_4 ( .gnd(gnd), .vdd(vdd), .A(_116__0_), .B(_105_), .Y(_106_) );
AOI21X1 AOI21X1_1 ( .gnd(gnd), .vdd(vdd), .A(_103_), .B(_105_), .C(_106_), .Y(_2__1_) );
INVX1 INVX1_5 ( .gnd(gnd), .vdd(vdd), .A(_115__2_), .Y(_107_) );
INVX1 INVX1_6 ( .gnd(gnd), .vdd(vdd), .A(_116__1_), .Y(_108_) );
MUX2X1 MUX2X1_1 ( .gnd(gnd), .vdd(vdd), .A(_107_), .B(_108_), .S(_105_), .Y(_2__2_) );
INVX1 INVX1_7 ( .gnd(gnd), .vdd(vdd), .A(_115__3_), .Y(_109_) );
INVX1 INVX1_8 ( .gnd(gnd), .vdd(vdd), .A(_116__2_), .Y(_110_) );
MUX2X1 MUX2X1_2 ( .gnd(gnd), .vdd(vdd), .A(_109_), .B(_110_), .S(_105_), .Y(_2__3_) );
INVX1 INVX1_9 ( .gnd(gnd), .vdd(vdd), .A(_115__4_), .Y(_111_) );
INVX1 INVX1_10 ( .gnd(gnd), .vdd(vdd), .A(_116__3_), .Y(_112_) );
MUX2X1 MUX2X1_3 ( .gnd(gnd), .vdd(vdd), .A(_111_), .B(_112_), .S(_105_), .Y(_2__4_) );
INVX1 INVX1_11 ( .gnd(gnd), .vdd(vdd), .A(_115__5_), .Y(_9_) );
INVX1 INVX1_12 ( .gnd(gnd), .vdd(vdd), .A(_116__4_), .Y(_10_) );
MUX2X1 MUX2X1_4 ( .gnd(gnd), .vdd(vdd), .A(_9_), .B(_10_), .S(_105_), .Y(_2__5_) );
INVX1 INVX1_13 ( .gnd(gnd), .vdd(vdd), .A(_115__6_), .Y(_11_) );
INVX1 INVX1_14 ( .gnd(gnd), .vdd(vdd), .A(_116__5_), .Y(_12_) );
MUX2X1 MUX2X1_5 ( .gnd(gnd), .vdd(vdd), .A(_11_), .B(_12_), .S(_105_), .Y(_2__6_) );
INVX1 INVX1_15 ( .gnd(gnd), .vdd(vdd), .A(_115__7_), .Y(_13_) );
INVX1 INVX1_16 ( .gnd(gnd), .vdd(vdd), .A(_116__6_), .Y(_14_) );
MUX2X1 MUX2X1_6 ( .gnd(gnd), .vdd(vdd), .A(_13_), .B(_14_), .S(_105_), .Y(_2__7_) );
INVX1 INVX1_17 ( .gnd(gnd), .vdd(vdd), .A(_115__8_), .Y(_15_) );
INVX1 INVX1_18 ( .gnd(gnd), .vdd(vdd), .A(_116__7_), .Y(_16_) );
MUX2X1 MUX2X1_7 ( .gnd(gnd), .vdd(vdd), .A(_15_), .B(_16_), .S(_105_), .Y(_2__8_) );
INVX1 INVX1_19 ( .gnd(gnd), .vdd(vdd), .A(_115__0_), .Y(_17_) );
NOR2X1 NOR2X1_5 ( .gnd(gnd), .vdd(vdd), .A(N[0]), .B(_105_), .Y(_18_) );
AOI21X1 AOI21X1_2 ( .gnd(gnd), .vdd(vdd), .A(_17_), .B(_105_), .C(_18_), .Y(_2__0_) );
XNOR2X1 XNOR2X1_1 ( .gnd(gnd), .vdd(vdd), .A(_116__5_), .B(_116__7_), .Y(_19_) );
XNOR2X1 XNOR2X1_2 ( .gnd(gnd), .vdd(vdd), .A(_116__3_), .B(_116__4_), .Y(_20_) );
OAI21X1 OAI21X1_3 ( .gnd(gnd), .vdd(vdd), .A(_19_), .B(_20_), .C(state_3_), .Y(_21_) );
AOI21X1 AOI21X1_3 ( .gnd(gnd), .vdd(vdd), .A(_19_), .B(_20_), .C(_21_), .Y(_22_) );
OAI21X1 OAI21X1_4 ( .gnd(gnd), .vdd(vdd), .A(state_3_), .B(_116__0_), .C(_93__bF_buf3), .Y(_23_) );
NOR2X1 NOR2X1_6 ( .gnd(gnd), .vdd(vdd), .A(_23_), .B(_22_), .Y(_3__0_) );
OAI21X1 OAI21X1_5 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf3), .B(_116__1_), .C(_93__bF_buf2), .Y(_24_) );
AOI21X1 AOI21X1_4 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf3), .B(_110_), .C(_24_), .Y(_3__2_) );
OAI21X1 OAI21X1_6 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf1), .B(_116__2_), .C(_93__bF_buf2), .Y(_25_) );
AOI21X1 AOI21X1_5 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf1), .B(_112_), .C(_25_), .Y(_3__3_) );
OAI21X1 OAI21X1_7 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf0), .B(_116__3_), .C(_93__bF_buf3), .Y(_26_) );
AOI21X1 AOI21X1_6 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf0), .B(_10_), .C(_26_), .Y(_3__4_) );
OAI21X1 OAI21X1_8 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf0), .B(_116__4_), .C(_93__bF_buf3), .Y(_27_) );
AOI21X1 AOI21X1_7 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf0), .B(_12_), .C(_27_), .Y(_3__5_) );
OAI21X1 OAI21X1_9 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf0), .B(_116__5_), .C(_93__bF_buf3), .Y(_28_) );
AOI21X1 AOI21X1_8 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf1), .B(_14_), .C(_28_), .Y(_3__6_) );
OAI21X1 OAI21X1_10 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf3), .B(_116__6_), .C(_93__bF_buf2), .Y(_29_) );
AOI21X1 AOI21X1_9 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf3), .B(_16_), .C(_29_), .Y(_3__7_) );
OAI21X1 OAI21X1_11 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf3), .B(_116__0_), .C(_93__bF_buf3), .Y(_30_) );
AOI21X1 AOI21X1_10 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf3), .B(_108_), .C(_30_), .Y(_3__1_) );
INVX1 INVX1_20 ( .gnd(gnd), .vdd(vdd), .A(_114_), .Y(_31_) );
INVX1 INVX1_21 ( .gnd(gnd), .vdd(vdd), .A(state_2_), .Y(_32_) );
NAND3X1 NAND3X1_3 ( .gnd(gnd), .vdd(vdd), .A(state_4_), .B(_104__bF_buf1), .C(_32_), .Y(_33_) );
AOI21X1 AOI21X1_11 ( .gnd(gnd), .vdd(vdd), .A(_31_), .B(_33_), .C(state_0_), .Y(_1_) );
INVX1 INVX1_22 ( .gnd(gnd), .vdd(vdd), .A(N[1]), .Y(_34_) );
AND2X2 AND2X2_1 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf2), .B(_113__0_), .Y(_35_) );
NOR2X1 NOR2X1_7 ( .gnd(gnd), .vdd(vdd), .A(_113__0_), .B(_104__bF_buf2), .Y(_36_) );
OAI21X1 OAI21X1_12 ( .gnd(gnd), .vdd(vdd), .A(_35_), .B(_36_), .C(_93__bF_buf1), .Y(_37_) );
OAI21X1 OAI21X1_13 ( .gnd(gnd), .vdd(vdd), .A(_93__bF_buf1), .B(_34_), .C(_37_), .Y(_0__0_) );
NOR2X1 NOR2X1_8 ( .gnd(gnd), .vdd(vdd), .A(N[1]), .B(N[2]), .Y(_38_) );
NAND2X1 NAND2X1_2 ( .gnd(gnd), .vdd(vdd), .A(N[1]), .B(N[2]), .Y(_39_) );
INVX1 INVX1_23 ( .gnd(gnd), .vdd(vdd), .A(_39_), .Y(_40_) );
OAI21X1 OAI21X1_14 ( .gnd(gnd), .vdd(vdd), .A(_40_), .B(_38_), .C(state_0_), .Y(_41_) );
INVX1 INVX1_24 ( .gnd(gnd), .vdd(vdd), .A(_113__1_), .Y(_42_) );
AND2X2 AND2X2_2 ( .gnd(gnd), .vdd(vdd), .A(_36_), .B(_42_), .Y(_43_) );
NOR2X1 NOR2X1_9 ( .gnd(gnd), .vdd(vdd), .A(_42_), .B(_36_), .Y(_44_) );
OAI21X1 OAI21X1_15 ( .gnd(gnd), .vdd(vdd), .A(_43_), .B(_44_), .C(_93__bF_buf1), .Y(_45_) );
NAND2X1 NAND2X1_3 ( .gnd(gnd), .vdd(vdd), .A(_41_), .B(_45_), .Y(_0__1_) );
INVX1 INVX1_25 ( .gnd(gnd), .vdd(vdd), .A(N[3]), .Y(_46_) );
NAND2X1 NAND2X1_4 ( .gnd(gnd), .vdd(vdd), .A(_46_), .B(_39_), .Y(_47_) );
INVX1 INVX1_26 ( .gnd(gnd), .vdd(vdd), .A(_47_), .Y(_48_) );
OAI21X1 OAI21X1_16 ( .gnd(gnd), .vdd(vdd), .A(_39_), .B(_46_), .C(state_0_), .Y(_49_) );
XNOR2X1 XNOR2X1_3 ( .gnd(gnd), .vdd(vdd), .A(_43_), .B(_113__2_), .Y(_50_) );
OAI22X1 OAI22X1_1 ( .gnd(gnd), .vdd(vdd), .A(_48_), .B(_49_), .C(_50_), .D(state_0_), .Y(_0__2_) );
AND2X2 AND2X2_3 ( .gnd(gnd), .vdd(vdd), .A(_47_), .B(N[4]), .Y(_51_) );
OAI21X1 OAI21X1_17 ( .gnd(gnd), .vdd(vdd), .A(_47_), .B(N[4]), .C(state_0_), .Y(_52_) );
INVX1 INVX1_27 ( .gnd(gnd), .vdd(vdd), .A(_113__3_), .Y(_53_) );
INVX1 INVX1_28 ( .gnd(gnd), .vdd(vdd), .A(_113__2_), .Y(_54_) );
AOI21X1 AOI21X1_12 ( .gnd(gnd), .vdd(vdd), .A(_54_), .B(_43_), .C(_53_), .Y(_55_) );
OR2X2 OR2X2_1 ( .gnd(gnd), .vdd(vdd), .A(_113__3_), .B(_113__2_), .Y(_56_) );
OR2X2 OR2X2_2 ( .gnd(gnd), .vdd(vdd), .A(_113__1_), .B(_113__0_), .Y(_57_) );
NOR3X1 NOR3X1_2 ( .gnd(gnd), .vdd(vdd), .A(_56_), .B(_104__bF_buf2), .C(_57_), .Y(_58_) );
OAI21X1 OAI21X1_18 ( .gnd(gnd), .vdd(vdd), .A(_55_), .B(_58_), .C(_93__bF_buf1), .Y(_59_) );
OAI21X1 OAI21X1_19 ( .gnd(gnd), .vdd(vdd), .A(_51_), .B(_52_), .C(_59_), .Y(_0__3_) );
OAI21X1 OAI21X1_20 ( .gnd(gnd), .vdd(vdd), .A(_47_), .B(N[4]), .C(N[5]), .Y(_60_) );
INVX1 INVX1_29 ( .gnd(gnd), .vdd(vdd), .A(N[5]), .Y(_61_) );
NOR2X1 NOR2X1_10 ( .gnd(gnd), .vdd(vdd), .A(N[3]), .B(N[4]), .Y(_62_) );
NAND3X1 NAND3X1_4 ( .gnd(gnd), .vdd(vdd), .A(_61_), .B(_39_), .C(_62_), .Y(_63_) );
NAND2X1 NAND2X1_5 ( .gnd(gnd), .vdd(vdd), .A(_63_), .B(_60_), .Y(_64_) );
INVX1 INVX1_30 ( .gnd(gnd), .vdd(vdd), .A(_113__4_), .Y(_65_) );
NOR2X1 NOR2X1_11 ( .gnd(gnd), .vdd(vdd), .A(_65_), .B(_58_), .Y(_66_) );
NAND3X1 NAND3X1_5 ( .gnd(gnd), .vdd(vdd), .A(state_3_), .B(_94_), .C(_95_), .Y(_67_) );
NOR2X1 NOR2X1_12 ( .gnd(gnd), .vdd(vdd), .A(_113__4_), .B(_67_), .Y(_68_) );
OAI21X1 OAI21X1_21 ( .gnd(gnd), .vdd(vdd), .A(_66_), .B(_68_), .C(_93__bF_buf0), .Y(_69_) );
OAI21X1 OAI21X1_22 ( .gnd(gnd), .vdd(vdd), .A(_93__bF_buf0), .B(_64_), .C(_69_), .Y(_0__4_) );
OAI21X1 OAI21X1_23 ( .gnd(gnd), .vdd(vdd), .A(_67_), .B(_113__4_), .C(_113__5_), .Y(_70_) );
INVX1 INVX1_31 ( .gnd(gnd), .vdd(vdd), .A(_113__5_), .Y(_71_) );
NAND3X1 NAND3X1_6 ( .gnd(gnd), .vdd(vdd), .A(_71_), .B(_65_), .C(_58_), .Y(_72_) );
AND2X2 AND2X2_4 ( .gnd(gnd), .vdd(vdd), .A(_72_), .B(_70_), .Y(_73_) );
NOR2X1 NOR2X1_13 ( .gnd(gnd), .vdd(vdd), .A(N[5]), .B(N[6]), .Y(_74_) );
NAND3X1 NAND3X1_7 ( .gnd(gnd), .vdd(vdd), .A(_39_), .B(_62_), .C(_74_), .Y(_75_) );
NAND2X1 NAND2X1_6 ( .gnd(gnd), .vdd(vdd), .A(N[6]), .B(_63_), .Y(_76_) );
NAND3X1 NAND3X1_8 ( .gnd(gnd), .vdd(vdd), .A(state_0_), .B(_75_), .C(_76_), .Y(_77_) );
OAI21X1 OAI21X1_24 ( .gnd(gnd), .vdd(vdd), .A(_73_), .B(state_0_), .C(_77_), .Y(_0__5_) );
XNOR2X1 XNOR2X1_4 ( .gnd(gnd), .vdd(vdd), .A(_75_), .B(N[7]), .Y(_78_) );
NOR2X1 NOR2X1_14 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf2), .B(_97_), .Y(_79_) );
AOI21X1 AOI21X1_13 ( .gnd(gnd), .vdd(vdd), .A(_113__6_), .B(_72_), .C(_79_), .Y(_80_) );
MUX2X1 MUX2X1_8 ( .gnd(gnd), .vdd(vdd), .A(_80_), .B(_78_), .S(_93__bF_buf0), .Y(_0__6_) );
OAI21X1 OAI21X1_25 ( .gnd(gnd), .vdd(vdd), .A(_75_), .B(N[7]), .C(N[8]), .Y(_81_) );
INVX1 INVX1_32 ( .gnd(gnd), .vdd(vdd), .A(N[8]), .Y(_82_) );
NOR2X1 NOR2X1_15 ( .gnd(gnd), .vdd(vdd), .A(N[7]), .B(_75_), .Y(_83_) );
NAND2X1 NAND2X1_7 ( .gnd(gnd), .vdd(vdd), .A(_82_), .B(_83_), .Y(_84_) );
NAND3X1 NAND3X1_9 ( .gnd(gnd), .vdd(vdd), .A(state_0_), .B(_81_), .C(_84_), .Y(_85_) );
NOR3X1 NOR3X1_3 ( .gnd(gnd), .vdd(vdd), .A(_104__bF_buf2), .B(_113__7_), .C(_97_), .Y(_5_) );
INVX1 INVX1_33 ( .gnd(gnd), .vdd(vdd), .A(_113__7_), .Y(_86_) );
NAND2X1 NAND2X1_8 ( .gnd(gnd), .vdd(vdd), .A(_94_), .B(_95_), .Y(_87_) );
NOR2X1 NOR2X1_16 ( .gnd(gnd), .vdd(vdd), .A(_113__4_), .B(_113__6_), .Y(_88_) );
NAND2X1 NAND2X1_9 ( .gnd(gnd), .vdd(vdd), .A(_71_), .B(_88_), .Y(_89_) );
NOR2X1 NOR2X1_17 ( .gnd(gnd), .vdd(vdd), .A(_89_), .B(_87_), .Y(_90_) );
AOI21X1 AOI21X1_14 ( .gnd(gnd), .vdd(vdd), .A(state_3_), .B(_90_), .C(_86_), .Y(_91_) );
OAI21X1 OAI21X1_26 ( .gnd(gnd), .vdd(vdd), .A(_91_), .B(_5_), .C(_93__bF_buf0), .Y(_92_) );
NAND2X1 NAND2X1_10 ( .gnd(gnd), .vdd(vdd), .A(_85_), .B(_92_), .Y(_0__7_) );
AND2X2 AND2X2_5 ( .gnd(gnd), .vdd(vdd), .A(_102_), .B(state_1_), .Y(_4_) );
INVX8 INVX8_3 ( .gnd(gnd), .vdd(vdd), .A(reset), .Y(_8_) );
BUFX2 BUFX2_1 ( .gnd(gnd), .vdd(vdd), .A(_113__0_), .Y(counter[0]) );
BUFX2 BUFX2_2 ( .gnd(gnd), .vdd(vdd), .A(_113__1_), .Y(counter[1]) );
BUFX2 BUFX2_3 ( .gnd(gnd), .vdd(vdd), .A(_113__2_), .Y(counter[2]) );
BUFX2 BUFX2_4 ( .gnd(gnd), .vdd(vdd), .A(_113__3_), .Y(counter[3]) );
BUFX2 BUFX2_5 ( .gnd(gnd), .vdd(vdd), .A(_113__4_), .Y(counter[4]) );
BUFX2 BUFX2_6 ( .gnd(gnd), .vdd(vdd), .A(_113__5_), .Y(counter[5]) );
BUFX2 BUFX2_7 ( .gnd(gnd), .vdd(vdd), .A(_113__6_), .Y(counter[6]) );
BUFX2 BUFX2_8 ( .gnd(gnd), .vdd(vdd), .A(_113__7_), .Y(counter[7]) );
BUFX2 BUFX2_9 ( .gnd(gnd), .vdd(vdd), .A(_114_), .Y(done) );
BUFX2 BUFX2_10 ( .gnd(gnd), .vdd(vdd), .A(_115__0_), .Y(dp[0]) );
BUFX2 BUFX2_11 ( .gnd(gnd), .vdd(vdd), .A(_115__1_), .Y(dp[1]) );
BUFX2 BUFX2_12 ( .gnd(gnd), .vdd(vdd), .A(_115__2_), .Y(dp[2]) );
BUFX2 BUFX2_13 ( .gnd(gnd), .vdd(vdd), .A(_115__3_), .Y(dp[3]) );
BUFX2 BUFX2_14 ( .gnd(gnd), .vdd(vdd), .A(_115__4_), .Y(dp[4]) );
BUFX2 BUFX2_15 ( .gnd(gnd), .vdd(vdd), .A(_115__5_), .Y(dp[5]) );
BUFX2 BUFX2_16 ( .gnd(gnd), .vdd(vdd), .A(_115__6_), .Y(dp[6]) );
BUFX2 BUFX2_17 ( .gnd(gnd), .vdd(vdd), .A(_115__7_), .Y(dp[7]) );
BUFX2 BUFX2_18 ( .gnd(gnd), .vdd(vdd), .A(_115__8_), .Y(dp[8]) );
BUFX2 BUFX2_19 ( .gnd(gnd), .vdd(vdd), .A(_116__0_), .Y(sr[0]) );
BUFX2 BUFX2_20 ( .gnd(gnd), .vdd(vdd), .A(_116__1_), .Y(sr[1]) );
BUFX2 BUFX2_21 ( .gnd(gnd), .vdd(vdd), .A(_116__2_), .Y(sr[2]) );
BUFX2 BUFX2_22 ( .gnd(gnd), .vdd(vdd), .A(_116__3_), .Y(sr[3]) );
BUFX2 BUFX2_23 ( .gnd(gnd), .vdd(vdd), .A(_116__4_), .Y(sr[4]) );
BUFX2 BUFX2_24 ( .gnd(gnd), .vdd(vdd), .A(_116__5_), .Y(sr[5]) );
BUFX2 BUFX2_25 ( .gnd(gnd), .vdd(vdd), .A(_116__6_), .Y(sr[6]) );
BUFX2 BUFX2_26 ( .gnd(gnd), .vdd(vdd), .A(_116__7_), .Y(sr[7]) );
DFFSR DFFSR_1 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf0), .D(_4_), .Q(state_0_), .R(vdd), .S(_8__bF_buf2) );
DFFSR DFFSR_2 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf0), .D(_6_), .Q(state_1_), .R(_8__bF_buf2), .S(vdd) );
DFFSR DFFSR_3 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf2), .D(_5_), .Q(state_2_), .R(_8__bF_buf1), .S(vdd) );
DFFSR DFFSR_4 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf2), .D(_7_), .Q(state_3_), .R(_8__bF_buf1), .S(vdd) );
DFFSR DFFSR_5 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf4), .D(state_2_), .Q(state_4_), .R(_8__bF_buf3), .S(vdd) );
DFFSR DFFSR_6 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf0), .D(start), .Q(startbuf_0_), .R(_8__bF_buf2), .S(vdd) );
DFFSR DFFSR_7 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf0), .D(startbuf_0_), .Q(startbuf_1_), .R(_8__bF_buf2), .S(vdd) );
DFFSR DFFSR_8 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf1), .D(_0__0_), .Q(_113__0_), .R(_8__bF_buf0), .S(vdd) );
DFFSR DFFSR_9 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf2), .D(_0__1_), .Q(_113__1_), .R(_8__bF_buf1), .S(vdd) );
DFFSR DFFSR_10 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf1), .D(_0__2_), .Q(_113__2_), .R(_8__bF_buf0), .S(vdd) );
DFFSR DFFSR_11 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf1), .D(_0__3_), .Q(_113__3_), .R(_8__bF_buf0), .S(vdd) );
DFFSR DFFSR_12 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf1), .D(_0__4_), .Q(_113__4_), .R(_8__bF_buf0), .S(vdd) );
DFFSR DFFSR_13 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf0), .D(_0__5_), .Q(_113__5_), .R(_8__bF_buf2), .S(vdd) );
DFFSR DFFSR_14 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf0), .D(_0__6_), .Q(_113__6_), .R(_8__bF_buf2), .S(vdd) );
DFFSR DFFSR_15 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf2), .D(_0__7_), .Q(_113__7_), .R(_8__bF_buf2), .S(vdd) );
DFFSR DFFSR_16 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf3), .D(_3__0_), .Q(_116__0_), .R(_8__bF_buf4), .S(vdd) );
DFFSR DFFSR_17 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf3), .D(_3__1_), .Q(_116__1_), .R(_8__bF_buf4), .S(vdd) );
DFFSR DFFSR_18 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf4), .D(_3__2_), .Q(_116__2_), .R(_8__bF_buf3), .S(vdd) );
DFFSR DFFSR_19 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf4), .D(_3__3_), .Q(_116__3_), .R(_8__bF_buf3), .S(vdd) );
DFFSR DFFSR_20 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf1), .D(_3__4_), .Q(_116__4_), .R(_8__bF_buf0), .S(vdd) );
DFFSR DFFSR_21 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf2), .D(_3__5_), .Q(_116__5_), .R(_8__bF_buf1), .S(vdd) );
DFFSR DFFSR_22 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf4), .D(_3__6_), .Q(_116__6_), .R(_8__bF_buf3), .S(vdd) );
DFFSR DFFSR_23 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf2), .D(_3__7_), .Q(_116__7_), .R(_8__bF_buf1), .S(vdd) );
DFFSR DFFSR_24 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf1), .D(_2__0_), .Q(_115__0_), .R(_8__bF_buf0), .S(vdd) );
DFFSR DFFSR_25 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf3), .D(_2__1_), .Q(_115__1_), .R(_8__bF_buf4), .S(vdd) );
DFFSR DFFSR_26 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf3), .D(_2__2_), .Q(_115__2_), .R(_8__bF_buf4), .S(vdd) );
DFFSR DFFSR_27 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf4), .D(_2__3_), .Q(_115__3_), .R(_8__bF_buf3), .S(vdd) );
DFFSR DFFSR_28 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf2), .D(_2__4_), .Q(_115__4_), .R(_8__bF_buf1), .S(vdd) );
DFFSR DFFSR_29 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf3), .D(_2__5_), .Q(_115__5_), .R(_8__bF_buf4), .S(vdd) );
DFFSR DFFSR_30 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf3), .D(_2__6_), .Q(_115__6_), .R(_8__bF_buf4), .S(vdd) );
DFFSR DFFSR_31 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf4), .D(_2__7_), .Q(_115__7_), .R(_8__bF_buf3), .S(vdd) );
DFFSR DFFSR_32 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf4), .D(_2__8_), .Q(_115__8_), .R(_8__bF_buf3), .S(vdd) );
DFFSR DFFSR_33 ( .gnd(gnd), .vdd(vdd), .CLK(clock_bF_buf3), .D(_1_), .Q(_114_), .R(_8__bF_buf4), .S(vdd) );
endmodule
