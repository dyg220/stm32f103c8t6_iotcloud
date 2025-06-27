module test1();
  initial begin
	$display("Hello, World!");
  end

  input clk
  always @(posedge clk) begin
	$display("Clock edge detected");
  end

endmodule	