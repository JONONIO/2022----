`timescale 1ns / 1ps

module DAC902_TOP(
    input Clock,
    input [57:0] state_ctr,
//     input [23:0] frequency,//[49:26]
//     input [4:0] amplitude,//[25:21]
//     input [6:0] duty_ratio,//[20:14]
//     input [11:0] bias,//[13:2]
//     input [1:0] wave_style,//[1:0]

    /*    [7:0]  [3:0]    [11:0]      閺嗗倸鐣炬稉?3.3V
    Vout=(out_1*amplitiude+bias)/4096*Vref
    Vbias=bias*4096/Vref
    freq=f_in*141M/2^24
    */

    //wava_style 0 濮濓絽楦? 1 閺傝灏? 2 娑撳顫�?�?? 3 閼村鍟垮▔?
    //閸忋劑鍎撮棁?鐟曚礁褰囬弫瀛樻�?
//    input ClkEn,
    output [11:0] out_bias,
    output  CLK  //閺冨爼鎸?

);

assign CLK=Clock;
wire  [31:0] Frequency;//妫版垹宸?  �???鐟曚浇钂嬫禒鎯邦�?�??? 
wire signed[5:0] Amplitude;//楠炲懎�???  
wire [11:0] Bias;//閸嬪繒鐤? 閻╁瓨甯撮惄绋垮�?
wire signed[8:0] duty_ratio;

assign Frequency=state_ctr[57:26];
assign Amplitude=$signed({1'd0,state_ctr[25:21]});
assign duty_ratio=$signed({state_ctr[20:14]});
assign Bias=state_ctr[13:2];

wire ClkEn ;
assign ClkEn=1'b1;





DDS_ctrl DDS_ctrl(
    .Clock(Clock),
    .ClkEn(ClkEn),
    .frequency(Frequency),//妫版垹宸?  �???鐟曚浇钂嬫禒鎯邦�?�??? 
    .amplitude(Amplitude),//楠炲懎�???  閻€劌鍩屾稊妯荤《閸?? S
    .duty_ratio(duty_ratio),//閸楃姷鈹栧�?? 鏉╁洦鐦潏鍐ㄦ�?
    .bias(Bias),//閸嬪繒鐤? 閻╁瓨甯撮惄绋垮�?
    .wave_style(state_ctr[1:0]),//濞夈垹鑸伴惃鍕潚缁??
    .out_bias(out_bias) //�???缂佸牐绶崙?
);
endmodule
