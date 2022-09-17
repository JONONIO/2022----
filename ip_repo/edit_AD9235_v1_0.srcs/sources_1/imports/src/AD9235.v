`timescale 1ns / 1ps

module AD9235 #(
    parameter CNT_W = 12
)
(
    input wire en,
//    input wire [11:0] Data,
    input wire clk,
    input wire rst_n,
    input wire [CNT_W-1:0] cnt_reg,
    output reg Sclk,
    // output reg [11:0] Selve_D,
    output en_Fall
    );
    reg [CNT_W-1:0] cnt_temp;
    reg Fall_reg[1:0];
    always @(posedge clk) begin
        if (!rst_n)
            cnt_temp<=0;
        else if ((cnt_temp<cnt_reg)&&en)begin
            cnt_temp=cnt_temp+1'b1;
        end
        else begin
            cnt_temp<=0;
        end
    end
    always @(posedge clk) begin
        if (!rst_n) Sclk<=0;
        else if ((cnt_reg==12'b0) && en) Sclk <= ~Sclk;
        else if ((cnt_temp ==cnt_reg) && en) begin
            Sclk <= ~Sclk;
        end
    end
    always @(posedge clk) begin
        if (!rst_n)begin 
        Fall_reg[0]<=1'b0;
        Fall_reg[1]<=1'b0;
        end
        else if (en) begin
            Fall_reg[0] <= Fall_reg[1];
            Fall_reg[1] <= Sclk;
        end
    end

    assign en_Fall= en &&((Fall_reg[1]==1'b0)&&(Fall_reg[0]==1'b1)) ? 1:0;
    // always @(posedge clk) begin
    //     if (!rst_n) Selve_D<=12'b0;
    //     else if (en_Fall) Selve_D <= Data;
    // end
endmodule
