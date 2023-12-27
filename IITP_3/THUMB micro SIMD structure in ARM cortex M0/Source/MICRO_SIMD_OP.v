module MICRO_SIMD_OP #(
        parameter WIDTH = 4 
)
(
        input [WIDTH-1:0] i_SRC1,
        input [WIDTH-1:0] i_SRC2,
        input [ 2:0]      i_CTRL,
        output [3:0]       o_FLAGS,
        output [WIDTH-1:0] o_RES
    );

reg  r_C;
wire  w_N;
wire  w_Z;
wire  w_V;

reg [WIDTH-1:0] r_RES;

always @ (*)
    case (i_CTRL)
        `SADD:
            {r_C, r_RES} <= i_SRC1 + i_SRC2; 
            
        `SMUL:
            {r_C, r_RES} <= i_SRC1 * i_SRC2; 

        `SSFT:
            {r_C, r_RES} <= {1'b0, i_SRC1 << i_SRC2};

        default:
            {r_C, r_RES} <= i_SRC2; 
    endcase

assign      w_N  = r_RES[3];
assign      w_Z  = (r_RES == 0);
assign      w_V  = 0;

assign o_RES = r_RES;
assign o_FLAGS = {w_N, w_Z, r_C, w_V};


endmodule

