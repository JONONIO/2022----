`timescale 1ns / 1ps
module porting(
           output wire [15:0] D,
           input clk,
           input rs,
           input rst,
           input [15:0]data,
           input start,
           output wire CS,
           output wire WR,
           output wire RS,
           output wire Rst,
           output wire RD,
           output reg finish//for AXIStream
           );      
       reg [2:0]state = 0;
       always@(posedge clk)begin
             case(state)
               0:begin  
                    if(start)state <= 1;
                    finish <= 0;
                  end
               1:         // cs down
                    state <= state+1;                                              
               2:        //wr down
                    state <= state+1;
               3:        //wr down
                    state <= state+1;
               4:       //wr up
                    state <= state+1;
               5:      //finish
                    if(!start)begin
                        state <= 0; //when start=0 cs up
                        finish <= 1;
                    end
               default:begin 
                   state <= 0;
                   finish <= 0;
               end
             endcase
       end
          
       assign CS = (state == 0)?1:0;
       assign WR = (state == 2||state == 3)?0:1;
       assign RD = 1;
       assign D = data;
       assign Rst = rst;  
       assign RS = rs;
endmodule