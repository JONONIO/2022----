module cymometer
   #(parameter    CLK_FS = 32'd100_000_000,
     parameter GATE_TIME = 32'd50_000_000,        // gate time:max 10ns*15000000=0.15s
     parameter HALF_GTIME= 32'd25_000_000) //sys_clk freq
    (
        //system 
        input wire clk_fs   ,     // clk_ref
        input wire rst_n    ,
        //cymometer interface
        input wire clk_fx   ,     // fx clk
        input wire clk_fy   ,     // fy clk
        output logic[32-1:0] fs_cnt_n,
        output logic[32-1:0] fs_cnt_p,
        output logic[32-1:0] fx_cnt,
        output logic[32-1:0] fxy_cnt,
        output logic p_x
);

//  clk_fs    ^\_/^\_/^\_/^\_/^\_/^\_/^\_/^\_/^\_/^\_/^\_/^\_/^\_/^\_/^\_
//  clk_fx    _______/^^^^^\_____/^^^^^\_____/^^^^^\_____/^^^^^\_____/^^^^
//  gate      ___/^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\_____________
//  gate_fx   _______/^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\____
//  fs_cnt    xxxxxx 0 x 1 x 2 x 3 x 4 x 5 x ... ................... M====
//  fx_cnt    xxxxxx 0 xxxxxxxxx 1 xxxxxxxxx 2 ........... xxxxxxxxx N====
//  M         ======================== Mk-1 ======================== M====
//  N         ======================== Nk-1 ======================== N====
 
//parameter define
//localparam GATE_TIME = 26'd50_000_000;        // gate time:max 10ns*15000000=0.15s
//localparam HALF_GTIME= 26'd25_000_000;

logic [27-1:0] gate_cnt='0;
logic [32-1:0] fx_cnt_temp;
logic [32-1:0] fxy_cnt_temp;
logic [32-1:0] fs_cnt_n_temp;
logic [32-1:0] fs_cnt_p_temp;
logic gate;
logic gate_fx;
logic neg_gate_fs;
logic neg_gate_fx;
logic ph_x;

//gate_cnt
always_ff @(posedge clk_fs) begin
    if(!rst_n) gate_cnt <= '0; 
    else if(gate_cnt == 2*GATE_TIME)
        gate_cnt <= '0;
    else gate_cnt <= gate_cnt + 1'b1;
end

//gate->1: 10 gate_time 10
always_ff @(posedge clk_fs) begin
    if(!rst_n) gate <= 1'b0;
    else if(gate_cnt < HALF_GTIME)
        gate <= 1'b0;     
    else if(gate_cnt < HALF_GTIME+GATE_TIME)
        gate <= 1'b1;
    else if(gate_cnt < 2*GATE_TIME)
        gate <= 1'b0;
    else gate <= 1'b0;
end

//gate_fx
always_ff @(posedge clk_fx) begin
    if(!rst_n) gate_fx <= 1'b0;
    else gate_fx <= gate;
end

Falling2En #(1) g_x_f(clk_fx,gate_fx,neg_gate_fx);
Falling2En #(1) g_s_f(clk_fs,gate_fx,neg_gate_fs);

 //fx cnt
always_ff @(posedge clk_fx) begin
    if(!rst_n) begin
        fx_cnt_temp <= 32'd0;
        fx_cnt <= 32'd0;
    end
    else if(gate_fx) fx_cnt_temp <= fx_cnt_temp + 1'b1;
    else if(neg_gate_fx) begin
        fx_cnt_temp <= 32'd0;
        fx_cnt <= fx_cnt_temp;
    end
end 

//fs_cnt_p,fs_cnt_n
always_ff @(posedge clk_fs) begin
    if(!rst_n) begin
        fs_cnt_p_temp <= 32'd0;
        fs_cnt_n_temp <= 32'd0;
        fs_cnt_n <= 32'd0;
        fs_cnt_p <= 32'd0;
    end
    else if(gate_fx&clk_fx) fs_cnt_p_temp <= fs_cnt_p_temp + 1'b1;
    else if(gate_fx&~clk_fx) fs_cnt_n_temp <= fs_cnt_n_temp + 1'b1;
    else if(neg_gate_fs) begin
        fs_cnt_n_temp <= 32'd0;
        fs_cnt_p_temp <= 32'd0;
        fs_cnt_p <= fs_cnt_p_temp;
        fs_cnt_n <= fs_cnt_n_temp;
    end
end

 //fxy cnt clk_fs domain
always_ff @(posedge clk_fs) begin
    if(!rst_n) begin
        fxy_cnt_temp <= 32'd0;
        fxy_cnt <= 32'd0;
    end
    else if(gate_fx&(clk_fx!=clk_fy)) fxy_cnt_temp <= fxy_cnt_temp + 1'b1;
    else if(neg_gate_fs) begin
        fxy_cnt_temp <= 32'd0;
        fxy_cnt <= fxy_cnt_temp;
    end
end 

always_ff @(posedge clk_fs) begin
    if(!rst_n)begin
        ph_x <= '0;
        p_x <= '0;
    end
    else if(clk_fx>clk_fy) ph_x <= '1;
    else if(clk_fy>clk_fx) ph_x <= '0;
    else if(neg_gate_fs) p_x <= ph_x;
end

// freq cal
endmodule
