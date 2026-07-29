WIP WIP WIP WIP 
manefesto: 
inputs:
clk (implement as clk step forward)
trig 
trigid
buf status up dn l r
valid in up dn l r 
lat cnt 
lat cnt req

output:
buf status self
valid out up dn l r 

in/out
data bus up dn l r 

state that needs to be explicitly kept track of
network memory parameterizeable depth
core memory parameterizeable depth
network memory entry count

class starts off with empty core and network
packet routing:
valid updates are made of rising edge of the clock and based on router decision

bus is asserted on negedge of clock if there is only one valid (xor for this core and incoming valid)
^all data bus updates are on nededge
else xor is low then transaction is cancled. hold onto the data and don't drive the bus retry next cycle
output one packet / cycle, recieve up to 5 per cycle (4 directions + 1 from local memeory)


next rising edge

local mem behavior/core:
core is responsible for 64 pixels broken up into 16 4px regions

region addressing
    .Hit              (            pix_hit [r/2][(r%2 + 1)*4 -1 -:4] ),
    ^this is how pix [63:0] gets mapped to a 4 pix region and then aggregated into a packet
    RegionAddr = row*2 + (col >= 4);
    r	r/2	slice	Pixels
0	0	[3:0]	row0 cols0-3
1	0	[7:4]	row0 cols4-7
2	1	[3:0]	row1 cols0-3
3	1	[7:4]	row1 cols4-7
4	2	[3:0]	row2 cols0-3
...	...	...	...
14	7	[3:0]	row7 cols0-3
15	7	[7:4]	row7 cols4-7
So each region receives exactly one row and four columns.
also recieves a core address from top level final address is {core addr, region addr} for data packet

counts hits and then stores them and eventually get turned into packets
start counting hits at rising edge of clock. the LSB of the hit is the clock edge
-> if clock is high then it is a 0 else its is a 1 4 bit ToT maxes out at 14, 15 is reserved code for empty
(needs to update with negedge and posedge)
at the first rising edge get the current latcnt and store with the hit

after hit:
wait until lat cnt req = local latcnt for all hits if this is true and trig is high then hit is triggered
and gets passed to network, else (if they match and no trig) delete the hit
if trig is high replace latency count with current trigidIn (marks which timestamp this packet came from)

from ToT to packet:
assign local_data_packet = {CoreRowAddrIn, RegionAddrOut, RegionTrigOut, RegionDataOut};
RegionDataOut is aggregation of the 4 $bit ToT = 16bits, coreaddrin = 6 bit, region is 4 bit
region trig is the trigger associated with the hit

it gets added into the network layer 2 cycles later (current latency make this parameterizeable)
adds hits to local network storage every cycle of it has data if more data then core can take it use 
fixed priority to determined who gets dropped
LOCAL CORE, UP, DOWN, LEFT, RIGHT  (right gets dropped most)

need functions to
advance to next posedge
advance to next negedge (make sure they are always done one after another not b2b else throw error)

inject hits 

route in data

check all outputs and internal state variables

for router routing decision:
seperate class which gets instaitated inside the core class:
Router Module — Functional Specification
Purpose
Selects the best output direction for a packet at a mesh NoC node, or decides to hold the packet locally. Output is one-hot over the routable directions.
Ports
Inputs: buf_status[i] — buffer occupancy for each candidate port i (0 to NETWORK_MEM_DEPTH).
Output: routing_decision — one-hot routable directions (self port excluded).
Index Mapping (numInputs = 5)
Index 0 = up
Index 1 = down
Index 2 = left
Index 3 = right
Index 4 = self
Scoring
For each port i:
score[i] = buf_status[i] + distScores[i]
where distScores[i] is a fixed per-direction distance penalty (pre-shifted so min = 0). Lower score = better.
Selection (argmin via all-pairs comparison)
Find the single port with the lowest score. For every unique pair (ii, jj) with ii < jj:
comp = (score[ii] < score[jj])   using strict less-than
Start with all ports as candidates (lowest[k] = 1). Each port stays a candidate only if it wins every comparison:

lowest[ii] = lowest[ii] AND comp        (ii stays lowest only if score[ii] < score[jj])
lowest[jj] = lowest[jj] AND NOT comp    (jj stays lowest only if score[jj] <= score[ii])

Exactly one port survives, which is the minimum.

Tie-Breaking

Strict less-than means on equal scores the higher-indexed port wins. Result is always exactly one-hot.

Output Rule

Drive only the routable bits: routing_decision = lowest[0 to numInputs-1] (drop the self bit).
If the self port is the winner, all output bits are 0, meaning do not route this cycle.

Reference Python Model

def router(buf_status, dist_scores, self_index=None):
n = len(buf_status)
if self_index is None:
self_index = n - 1

less
scores = [buf_status[i] + dist_scores[i] for i in range(n)]

lowest = [True] * n
for ii in range(n - 1):
    for jj in range(ii + 1, n):
        comp = scores[ii] < scores[jj]
        lowest[ii] = lowest[ii] and comp
        lowest[jj] = lowest[jj] and (not comp)

routing_decision = [1 if lowest[i] else 0
                    for i in range(n) if i != self_index]
return routing_decision
if name == "main":
buf_status  = [3, 3, 3, 3, 3] default to 0
dist_scores = [2, 0, 1, 1, 1]
print(router(buf_status, dist_scores))

Note on distScores ordering: in the Verilog, the packed literal is indexed via a part-select, so distScores[0] corresponds to the rightmost field and maps to index 0 (up). When populating the Python dist_scores list, ensure element i matches the value the hardware slices for port i, meaning list the fields in reverse of how they appear in the concatenation.
^up is 2 self l r is 1 dn is 0