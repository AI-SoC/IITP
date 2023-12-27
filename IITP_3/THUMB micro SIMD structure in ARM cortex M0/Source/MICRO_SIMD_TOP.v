module MICRO_SIMD_TOP 
//        parameter DATA_WIDTH = 32,
//        parameter LANE = 1)
    (
        input [31:0] i_SIMDSRC1,
        input [31:0] i_SIMDSRC2,
        input [ 2:0] i_SIMD_CTRL,
        output [31:0] o_SIMD_FLAGS,
        output [31:0] o_SIMD_RES
    );


wire [31:0] w_SRC1;
wire [31:0] w_SRC2;
assign w_SRC1 = i_SIMDSRC1;
assign w_SRC2 = i_SIMDSRC2;

localparam DATA_WIDTH = 4;

/* generate lanes */
genvar bit;
generate
    for(bit = 0; bit < 8; bit = bit + 1) begin : gen_lane
        MICRO_SIMD_OP #(
            .WIDTH (DATA_WIDTH)
        )uLANE_bit 
        (   .i_SRC1(i_SIMDSRC1[((bit+1)*DATA_WIDTH-1) : bit*DATA_WIDTH]),
            .i_SRC2(i_SIMDSRC2[ ((bit+1)*DATA_WIDTH-1) : bit*DATA_WIDTH ]),
            .i_CTRL(i_SIMD_CTRL),
            .o_FLAGS(o_SIMD_FLAGS[ (bit+1)*4-1 : bit*4]),
            .o_RES(o_SIMD_RES[((bit+1)*DATA_WIDTH-1) : bit*DATA_WIDTH ])
        );
    end
endgenerate



//assign o_SIMD_RES = r_RES;
//assign o_SIMD_FLAGS = {w_N, w_Z, w_C, w_V};


endmodule

