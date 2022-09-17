`timescale 1ns / 1ps

module DDS_ctrl(
    input Clock,
    input ClkEn,
    input [31:0] frequency,//棰戠�?  �??瑕佽蒋浠惰�?? 
    input signed[5:0] amplitude,//骞呭�?  
    input signed[8:0] duty_ratio,//鍗犵┖姣? 杩囨瘮杈冨櫒
    input [11:0] bias,//鍋忕�? 鐩存帴鐩稿姞
    input [1:0] wave_style,//娉㈠舰鐨勭�??
    output  [11:0] out_bias //�??缁堣緭鍑?
);




wire [31:0] FreqWord;
wire [31:0] PhaseShift;


wire signed[6:0] Out_sine;
reg signed[6:0] Out_square;
wire signed[6:0] Out_tri;
wire signed[6:0] Out_pulse;

reg signed[6:0] out;          //鎵挎帴鐩存帴杈撳�?

wire signed[15:0] out_amp;
reg signed [11:0] out_bias_1;

assign PhaseShift=32'b0;


//鍋忕疆鎺у�?
always @(posedge Clock ) begin
        if(bias[11:8]==4'b0)
        out_bias_1<=bias+$unsigned(out_amp[11:0])+'d2048;
        else
        out_bias_1<=$unsigned(out_amp[11:0])+'d2048-bias[7:0];
        
end 
    
assign out_bias=out_bias_1;


assign FreqWord=frequency;



assign out_amp=out*amplitude;






//娉㈠舰閫夋嫨
always @(posedge Clock) begin
    if(wave_style==2'd0)begin
        out<=Out_sine;
    end
    else if(wave_style==2'd1)begin
        out<=Out_square;
    end
    else if(wave_style==2'd2)begin
        out<=Out_tri;
    end
    else begin
        out<=Out_pulse;
    end
end



//鏂规尝浜х�?
always @(posedge Clock) begin
    if(Out_sine>duty_ratio)begin
        Out_square<=7'd63;  
    end
    else begin
        Out_square<=-7'sd64;        
    end
end

    //姝ｅ鸡娉?
    dds_sine
    #(.PHASE_W(32), 
    .DATA_W(7), 
    .TABLE_AW (12)
    )dds_sine( 
    .FreqWord(FreqWord), 
    .PhaseShift(PhaseShift),
    .Clock(Clock), 
    .ClkEn(ClkEn), 
    .Out(Out_sine)
    ) ;


    dds_tri
    #(.PHASE_W(32), 
    .DATA_W(7), 
    .TABLE_AW (12)
    )dds_tri( 
    .FreqWord(FreqWord), 
    .PhaseShift(PhaseShift),
    .Clock(Clock), 
    .ClkEn(ClkEn), 
    .Out(Out_tri)
    ) ;

    dds_pulse
    #(.PHASE_W(32), 
    .DATA_W(7), 
    .TABLE_AW (12)
    )dds_pulse( 
    .FreqWord(FreqWord), 
    .PhaseShift(PhaseShift),
    .Clock(Clock), 
    .ClkEn(ClkEn), 
    .Out(Out_pulse)
    ) ;


endmodule
