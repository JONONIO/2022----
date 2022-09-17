module FIFO_Write #(
    parameter FIFO_WIDTH = 12
    ) (
        input clk,
        input  AD_CLK,
        input rst_n,
       (*mark_debug*) input wire en,
        input wire full,
        input wire [FIFO_WIDTH-1:0] DATA,
        output wr_en,
        output reg [FIFO_WIDTH - 1:0] din,
        output reg srst 
);
    localparam WAIT = 2'b00;
    localparam CLEAN = 2'b01;
    localparam WRITE = 2'b10;
    localparam REDY = 2'b11;
    // localparam WEIGHT = 2'b00;
   (*mark_debug*) reg [1:0] now_reg;
   (*mark_debug*) reg [1:0] next_reg;
    (*mark_debug*)reg process_en;
    assign wr_en = process_en && AD_CLK;
    always @(posedge clk) begin
        if (!rst_n) begin
            now_reg<=WAIT;
        end
        else begin
            now_reg<=next_reg;
        end
    end
    always @(posedge clk) begin
         if (!rst_n) begin
            next_reg<=WAIT;
            process_en<=0;
            srst<=0;
            din<=0;
        end
        else begin
            case (now_reg)
                WAIT : begin
                    if(en) begin 
                        next_reg<=CLEAN;
                    end
                    else next_reg<=WAIT;
                end
                CLEAN : begin
                    // if(en) next_reg<=CLEAN;
                    srst<=1;
                    next_reg<=WRITE;
                end
                WRITE : begin
//                    if(en) next_reg<=CLEAN;
                        srst<=0;
                        process_en<=1;
                        din<=DATA;
                        if(full)begin
                            next_reg<=REDY;
                        end
                end
                REDY :begin
                        din<=0;
                        process_en<=0;
                        srst<=0;
                        next_reg<=WAIT;
                end

//                default: begin
//                    next_reg<=WAIT;
//                    process_en<=0;
//                    srst<=0;
//                    din<=0;
//                end
            endcase
        end
        
    end
endmodule