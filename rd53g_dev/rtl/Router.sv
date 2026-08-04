// simple router which implements the following routing decision scheme:
// all south nodes are given score = buffer Status
// all same row (self right left) score = 1 + buffer status
// all above nodes are given score = 2 = buffer status
// router combinatorically choses the direction which minmizes score

`ifndef SCORE_WIDTH
    `define SCORE_WIDTH 4
`endif
module Router #(
        parameter numInputs=5,
        parameter [`SCORE_WIDTH*numInputs-1:0] distScores = {4'd1,4'd1,4'd1,4'd0,4'd2}
        // Scores for neighbor dist. Shifted so lowest is 0
        ) (
    // neighbors + self buffer status
    input wire [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] buf_status [numInputs-1:0],
    // one-hot routing decision default:
  // buf_status
  // [4] = self
  // [3] = right
  // [2] = left
  // [1] = down
  // [0] = up
    output wire [numInputs-2:0] routing_decision 
    // exclude self, other signals are valid bits if self then don't route (no valid)
);
    parameter maxScore = 2;

    /*
    Example scoring for 5 inputs
    comps[9] = in[3]<in[4]
    comps[8] = in[2]<in[4]
    comps[7] = in[2]<in[3]
    comps[6] = in[1]<in[4]
    comps[5] = in[1]<in[3]
    comps[4] = in[1]<in[2]
    comps[3] = in[0]<in[4]
    comps[2] = in[0]<in[3]
    comps[1] = in[0]<in[2]
    comps[0] = in[0]<in[1]
    */
    logic comps [((numInputs-1)*numInputs)/2-1:0];
    logic [numInputs-1:0] lowest;
    wire [$clog2(`NETWORK_MEM_DEPTH+maxScore)-1:0] scores [numInputs-1:0];
    genvar i, j;

    generate
        for (i=0; i<numInputs; i++) begin 
            assign scores[i] = buf_status[i]+distScores[(i+1)*`SCORE_WIDTH-1-:`SCORE_WIDTH];
        end
        int index;
        always_comb begin
            index=0;
            lowest='1;
            for (int ii=0; ii<numInputs-1; ii++) begin
                for (int jj=ii+1; jj<numInputs; jj++) begin
                    comps[index] = scores[ii]<scores[jj];
                    lowest[ii]=lowest[ii]&&comps[index];
                    lowest[jj]=lowest[jj]&&!comps[index];
                    index++;
                end
            end
        end
    endgenerate
    assign routing_decision = lowest[numInputs-2:0];
endmodule