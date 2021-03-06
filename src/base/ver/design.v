module top2(c,a,b);
input a,b;
output c;
  and (c,a,b);
endmodule

module top(a, sel, out);
  input  [2:0] a;
  input        sel;
  output [1:0] out;
  wire [2:0] a;
  wire       sel;
  wire [1:0] out;
  wire [3:0] z;
  wire w1, w2, w3, w4, w5, w6, w7;
    not (z[0], a[0]);
    xnor (z[1], a[0], a[1]);
    or (w1, a[0], a[1]);
    xnor (z[2], a[2], w1);
    and (w2, a[2], a[1]);
    and (w3, a[2], a[0]);
    nor (z[3], w2, w3);
    nor (w4, sel, z[3]);
    nor (w5, sel, z[2]);
    top2 (.c(w6), .a(sel), .b(z[1]));
    top2 (.c(w7), .a(sel), .b(z[0]));
    or (out[1], w4, w6);
    or (out[0], w5, w7);
endmodule
