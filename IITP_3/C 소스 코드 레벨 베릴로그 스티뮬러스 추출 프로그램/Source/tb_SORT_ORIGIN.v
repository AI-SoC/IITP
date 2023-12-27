module tb_SORT_ORIGIN;

reg CLK, RESETB;

CPU uCPU
(
    .i_CLK      (CLK),
    .i_RESETB   (RESETB)
);

initial begin
    CLK         = 0;
    RESETB      = 0;
    #110 RESETB = 1;
end

always begin
    #5 SYSCLK = ~SYSCLK;
end

initial begin
    #10000
    $finish;
end

endmodule
