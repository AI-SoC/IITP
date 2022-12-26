module tb_TPU;

reg        SCLK;
reg        RESETB;
reg        REB;

wire        RD_RQST;
wire       ADDR;

TPU tpu_u0(
    .i_SCLK(SCLK),
    .i_RESETB(RESETB), 
    .i_REB(REB),
    .o_RD_RQST(RD_RQST),
    .o_ADDR(ADDR)
);
//initialize
//

initial begin

    SCLK=0;
    RESETB=0;
    REB=1;

end

initial begin
    $readmemb("file.bin",tpu_u0.exrom_u0.exrom);
end

always
    #1 SCLK=~SCLK;

initial begin

    #2 RESETB=1;

    #1  REB=0;
    #3  REB=1;
    #60 REB=0;
    #2  REB=1;
    #60 REB=0;
    #2  REB=1;
    #60 REB=0;
    #2  REB=1;
    #60 REB=0;
    #2  REB=1;

    #50 $finish;
end

initial begin
    $shm_open("simvision.shm");
    $shm_probe("ASC");
end

endmodule

