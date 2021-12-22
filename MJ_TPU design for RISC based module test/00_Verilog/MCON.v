`define M_IDLE      0
`define ROM_ACCESS  1
`define INST_FETCH  2
`define DECODE      3
`define MOV0        4
`define MOV1        5
`define MOV2        6
`define MOV3        7
`define ADD         8
`define SUB         9
`define JZ          10
`define RAM_WAIT    11
`define PC_OFF_UP   12
`define REG_UP      13
`define PC_UPDATE   14


module MCON(
    input i_SCLK,
    input i_RESETB,
    input i_REB,

    input i_ROM_READY, //form EXROM
    
    input [6:0] i_OPCODE, // from DECODER
    input [3:0] i_OP1,
    input [7:0] i_OP2,

    input [15:0] i_RESULT, // from ALU
    input i_ALU_READY,

    input [15:0] i_RAM_DATA, // from RAM


    output o_RD_RQST,  //to ROM
    output [6:0] o_ADDR,

    output o_WR_INST, //to decode

    output [15:0] o_RX, //to ALU
    output [15:0] o_RY,
    output [2:0]  o_ALU_ENABLE, //001 => Comparator, 010 => ADD, 100 => SUB ... 

    output o_RAM_WEB,   //to RAM
    output o_RAM_OEB,
    output [7:0] o_RAM_ADDR,
    output [15:0] o_RAM_DATA
);



reg       RD_RQST;
reg [6:0] ADDR;
reg       WR_INST;
reg [15:0] RX;
reg [15:0] RY;
reg [2:0]  ALU_ENABLE;
reg RAM_WEB;
reg RAM_OEB;
reg [7:0] RAM_ADDR;
reg [15:0] RAM_DATA;

assign o_RD_RQST = RD_RQST;
assign o_ADDR = ADDR;
assign o_WR_INST = WR_INST;

assign o_RX = RX;
assign o_RY = RY;
assign o_ALU_ENABLE = ALU_ENABLE;

assign o_RAM_WEB = RAM_WEB;
assign o_RAM_OEB = RAM_OEB;
assign o_RAM_ADDR = RAM_ADDR;
assign o_RAM_DATA = RAM_DATA;

reg [3:0] STATE;
reg [3:0] NEXT_STATE;

reg [6:0] PC;

reg [15:0] R0;
reg [15:0] R1;
reg [15:0] R2;
reg [15:0] R3;
reg [15:0] R4;
reg [15:0] R5;
reg [15:0] R6;
reg [15:0] R7;
reg [15:0] R8;
reg [15:0] R9;
reg [15:0] R10;
reg [15:0] R11;
reg [15:0] R12;
reg [15:0] R13;
reg [15:0] R14;
reg [15:0] R15;
reg [15:0] BUFFER;

reg [15:0] PC_OFF;

always @ (*)
    case(STATE)
        `M_IDLE : 
                if(~i_REB)
                    NEXT_STATE <= `ROM_ACCESS;
                else NEXT_STATE <= STATE;
        `ROM_ACCESS : 
                    if(i_ROM_READY)
                        NEXT_STATE <= `INST_FETCH;
                    else NEXT_STATE <= STATE;
        `INST_FETCH : NEXT_STATE <= `DECODE;
        `DECODE     : 
                    if(i_OPCODE == 7'b0000001)
                        NEXT_STATE <= `MOV0;
                    else if(i_OPCODE == 7'b0000010)
                        NEXT_STATE <= `MOV1;
                    else if(i_OPCODE == 7'b0000100)
                        NEXT_STATE <= `MOV2;
                    else if(i_OPCODE == 7'b0001000)
                        NEXT_STATE <= `MOV3;
                    else if(i_OPCODE == 7'b0010000)
                        NEXT_STATE <= `ADD;
                    else if(i_OPCODE == 7'b0100000)
                        NEXT_STATE <= `SUB;
                    else if(i_OPCODE == 7'b1000000)
                        NEXT_STATE <= `JZ;
                    else
                        NEXT_STATE <= `M_IDLE;
        `MOV0 : NEXT_STATE <= `RAM_WAIT; //from RAM
        `RAM_WAIT : NEXT_STATE <= `REG_UP;
        `MOV1 : NEXT_STATE <= `PC_UPDATE; //to RAM NOT YET

        `MOV2 : NEXT_STATE <= `REG_UP;
        `MOV3 : NEXT_STATE <= `REG_UP;//MOV  NEXT STATE
        `ADD  : 
                if(i_ALU_READY==1'b1)
                    NEXT_STATE <= `REG_UP;
                else NEXT_STATE <= STATE;
        `SUB  : 
                if(i_ALU_READY==1'b1)
                    NEXT_STATE <= `REG_UP;
                else NEXT_STATE <= STATE;

        `JZ   : if(i_ALU_READY==1'b1)
                    NEXT_STATE <= `PC_OFF_UP;
                else NEXT_STATE <= STATE;
        `PC_OFF_UP : NEXT_STATE <= `PC_UPDATE; 
        `REG_UP : NEXT_STATE <= `PC_UPDATE;
        `PC_UPDATE : NEXT_STATE <= `M_IDLE; 
    endcase

always @ (negedge i_RESETB or posedge i_SCLK) //STATE Change
    if(~i_RESETB)
        STATE <= `M_IDLE;
    else
        STATE <= NEXT_STATE;

always @ (negedge i_RESETB or posedge i_SCLK) //Sending Request Signal
    if(~i_RESETB)
        RD_RQST <= 1'b1;
    else if(STATE== `ROM_ACCESS)
        RD_RQST <= 1'b0;
    else
        RD_RQST <= 1'b1;

always @ (negedge i_RESETB or posedge i_SCLK) //Sending address
    if(~i_RESETB)
        ADDR <= 7'b0;
    else if(STATE == `ROM_ACCESS)
        ADDR <= PC;     //After Simulation must be changed ( to read from PC)
// 4 RAM WEB OEB RAM_ADDR DATA in out
always @ (negedge i_RESETB or posedge i_SCLK) //MOV0
    if(~i_RESETB)
        RAM_OEB<=1;
     else if(STATE == `MOV0)
        RAM_OEB<=0;
     else
        RAM_OEB<=1;

always @ (negedge i_RESETB or posedge i_SCLK) //MOV0
    if(~i_RESETB)
        RAM_WEB<=1;
     else if(STATE == `MOV1)
        RAM_WEB<=0;
     else
        RAM_WEB<=1;

always @ (negedge i_RESETB or posedge i_SCLK) //MOV0
    if(~i_RESETB)
        RAM_DATA<=0;
     else if(STATE == `MOV1)
        case (i_OP1)
            0 : RAM_DATA <=R0;
            1 : RAM_DATA <=R1;
            2 : RAM_DATA <=R2;
            3 : RAM_DATA <=R3;
            4 : RAM_DATA <=R4;
            5 : RAM_DATA <=R5;
            6 : RAM_DATA <=R6;
            7 : RAM_DATA <=R7;
            8 : RAM_DATA <=R8;
            9 : RAM_DATA <=R9;
           10 : RAM_DATA <=R10;
           11 : RAM_DATA <=R11;
           12 : RAM_DATA <=R12;
           13 : RAM_DATA <=R13;
           14 : RAM_DATA <=R14;
           15 : RAM_DATA <=R15;
        endcase

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        RAM_ADDR<=0;
    else if (STATE == `MOV0 || STATE == `MOV1)
        RAM_ADDR <= i_OP2;


always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        WR_INST <= 1'b0;
    else if(i_ROM_READY && STATE==`ROM_ACCESS)
        WR_INST <= 1'b1;
    else
        WR_INST <= 1'b0;

always @ (negedge i_RESETB or posedge i_SCLK)  //STATE == Execute
    if(~i_RESETB)
        RX<=0;
    else if(STATE == `ADD || STATE ==`SUB || STATE==`JZ)
        case(i_OP1)
            0  : RX <= R0;
            1  : RX <= R1;
            2  : RX <= R2;
            3  : RX <= R3;
            4  : RX <= R4;
            5  : RX <= R5;
            6  : RX <= R6;
            7  : RX <= R7;
            8  : RX <= R8;
            9  : RX <= R9;
            10 : RX <= R10;
            11 : RX <= R11;
            12 : RX <= R12;
            13 : RX <= R13;
            14 : RX <= R14;
            15 : RX <= R15;
            default : RX<=0;
        endcase
    //else   using RAM
        //RX <= 

always @ (negedge i_RESETB or posedge i_SCLK) //STATE == Execute
    if(~i_RESETB)
        RY<=0;
    else if( STATE == `ADD || STATE == `SUB)
        case(i_OP2)
            0  : RY <= R0;
            1  : RY <= R1;
            2  : RY <= R2;
            3  : RY <= R3;
            4  : RY <= R4;
            5  : RY <= R5;
            6  : RY <= R6;
            7  : RY <= R7;
            8  : RY <= R8;
            9  : RY <= R9;
            10 : RY <= R10;
            11 : RY <= R11;
            12 : RY <= R12;
            13 : RY <= R13;
            14 : RY <= R14;
            15 : RY <= R15;
            
            default : RY<=0;
    
        endcase
    
    else if(STATE == `MOV3 || STATE == `JZ)
        RY <= i_OP2;

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        ALU_ENABLE <=0;
    else if(STATE == `JZ)
        ALU_ENABLE <=1;
    else if(STATE == `ADD)
        ALU_ENABLE <=2;
    else if(STATE == `SUB)
        ALU_ENABLE <=4;
    else
        ALU_ENABLE <=0;

always @ (negedge i_RESETB or posedge i_SCLK) // PC_OFFSET UPDATE STATE 
    if(~i_RESETB)
        PC_OFF <=0;
    else if(STATE == `PC_OFF_UP && i_RESULT ==1)
        PC_OFF <= i_OP2;
    else PC_OFF <= 0;

reg [15:0] REG_RESULT;

always @ (negedge i_RESETB or posedge i_SCLK)
    if(~i_RESETB)
        BUFFER <= 0;
    else if(STATE == `MOV2)
        case(i_OP2)
            0: BUFFER <= R0;
            1: BUFFER <= R1;
            2: BUFFER <= R2;
            3: BUFFER <= R3;
            4: BUFFER <= R4;
            5: BUFFER <= R5;
            6: BUFFER <= R6;
            7: BUFFER <= R7;
            8: BUFFER <= R8;
            9: BUFFER <= R9;
            10: BUFFER <= R10;
            11: BUFFER <= R11;
            12: BUFFER <= R12;
            13: BUFFER <= R13;
            14: BUFFER <= R14;
            15: BUFFER <= R15;
        endcase
    else if(STATE == `MOV3)
        BUFFER <= {{8{i_OP2[7]}},i_OP2};
    else if(STATE == `ADD || STATE == `SUB)
        BUFFER <= i_RESULT;
    else if(STATE == `RAM_WAIT)
        BUFFER <= i_RAM_DATA;

always @ (negedge i_RESETB or posedge i_SCLK) //REG_UPDATE
    if(~i_RESETB)
        begin
        R0<=0;
        R1<=1;
        R2<=2;
        R3<=3;
        R4<=4;
        R5<=5;
        R6<=6;
        R7<=7;
        R8<=8;
        R9<=9;
        R10<=0;
        R11<=0;
        R12<=0;
        R13<=0;
        R14<=0;   // need to move
        R15<=0;
        end
    else if(STATE == `REG_UP)
        case(i_OP1)
            0: R0 <= BUFFER;
            1: R1 <= BUFFER;
            2: R2 <= BUFFER;
            3: R3 <= BUFFER;
            4: R4 <= BUFFER;
            5: R5 <= BUFFER;
            6: R6 <= BUFFER;
            7: R7 <= BUFFER;
            8: R8 <= BUFFER;
            9: R9 <= BUFFER;
            10: R10 <= BUFFER;
            11: R11 <= BUFFER;
            12: R12 <= BUFFER;
            13: R13 <= BUFFER;
            14: R14 <= BUFFER;
            15: R15 <= BUFFER;
        endcase
//    else if(STATE == `REG_UP_MOV)
  //      case(i_OP1)
                // Writr from Here
always @ (negedge i_RESETB or posedge i_SCLK)// PC UPDATE STATE
    if(~i_RESETB)
        PC <= 0;
    else if(STATE == `PC_UPDATE)
        PC <= PC + PC_OFF + 1;

endmodule
