OPENQASM 3.0;
include "stdgates.inc";
qubit[10] q;
cx q[1], q[5];
cx q[1], q[0];
rz(1.5707963267948966) q[1];
sx q[1];
rz(1.5707963267948966) q[1];
cx q[5], q[1];
rz(-0.7853981633974483) q[1];
cx q[0], q[1];
rz(0.7853981633974483) q[1];
cx q[5], q[1];
rz(-0.7853981633974483) q[1];
cx q[0], q[1];
rz(2.356194490192345) q[1];
sx q[1];
rz(1.5707963267948966) q[1];
rz(0.7853981633974483) q[5];
cx q[0], q[5];
rz(0.7853981633974483) q[0];
rz(-0.7853981633974483) q[5];
cx q[0], q[5];
cx q[2], q[6];
cx q[2], q[1];
rz(1.5707963267948966) q[2];
sx q[2];
rz(1.5707963267948966) q[2];
cx q[6], q[2];
rz(-0.7853981633974483) q[2];
cx q[1], q[2];
rz(0.7853981633974483) q[2];
cx q[6], q[2];
rz(-0.7853981633974483) q[2];
cx q[1], q[2];
rz(2.356194490192345) q[2];
sx q[2];
rz(1.5707963267948966) q[2];
rz(0.7853981633974483) q[6];
cx q[1], q[6];
rz(0.7853981633974483) q[1];
rz(-0.7853981633974483) q[6];
cx q[1], q[6];
cx q[3], q[7];
cx q[3], q[2];
rz(1.5707963267948966) q[3];
sx q[3];
rz(1.5707963267948966) q[3];
cx q[7], q[3];
rz(-0.7853981633974483) q[3];
cx q[2], q[3];
rz(0.7853981633974483) q[3];
cx q[7], q[3];
rz(-0.7853981633974483) q[3];
cx q[2], q[3];
rz(2.356194490192345) q[3];
sx q[3];
rz(1.5707963267948966) q[3];
rz(0.7853981633974483) q[7];
cx q[2], q[7];
rz(0.7853981633974483) q[2];
rz(-0.7853981633974483) q[7];
cx q[2], q[7];
cx q[4], q[8];
cx q[4], q[3];
rz(1.5707963267948966) q[4];
sx q[4];
rz(1.5707963267948966) q[4];
cx q[8], q[4];
rz(-0.7853981633974483) q[4];
cx q[3], q[4];
rz(0.7853981633974483) q[4];
cx q[8], q[4];
rz(-0.7853981633974483) q[4];
cx q[3], q[4];
rz(2.356194490192345) q[4];
sx q[4];
rz(1.5707963267948966) q[4];
rz(0.7853981633974483) q[8];
cx q[3], q[8];
rz(0.7853981633974483) q[3];
rz(-0.7853981633974483) q[8];
cx q[3], q[8];
cx q[4], q[9];
rz(1.5707963267948966) q[4];
sx q[4];
rz(1.5707963267948966) q[4];
cx q[8], q[4];
rz(-0.7853981633974483) q[4];
cx q[3], q[4];
rz(0.7853981633974483) q[4];
cx q[8], q[4];
rz(-0.7853981633974483) q[4];
cx q[3], q[4];
rz(2.356194490192345) q[4];
sx q[4];
rz(1.5707963267948966) q[4];
rz(0.7853981633974483) q[8];
cx q[3], q[8];
rz(0.7853981633974483) q[3];
rz(-0.7853981633974483) q[8];
cx q[3], q[8];
cx q[4], q[3];
cx q[3], q[8];
rz(1.5707963267948966) q[3];
sx q[3];
rz(1.5707963267948966) q[3];
rz(0.36514490501156727) q[4];
cx q[7], q[3];
rz(-0.7853981633974483) q[3];
cx q[2], q[3];
rz(0.7853981633974483) q[3];
cx q[7], q[3];
rz(-0.7853981633974483) q[3];
cx q[2], q[3];
rz(2.356194490192345) q[3];
sx q[3];
rz(1.5707963267948966) q[3];
rz(0.7853981633974483) q[7];
cx q[2], q[7];
rz(0.7853981633974483) q[2];
rz(-0.7853981633974483) q[7];
cx q[2], q[7];
cx q[3], q[2];
cx q[2], q[7];
rz(1.5707963267948966) q[2];
sx q[2];
rz(1.5707963267948966) q[2];
rz(0.2170570549491827) q[3];
cx q[6], q[2];
rz(-0.7853981633974483) q[2];
cx q[1], q[2];
rz(0.7853981633974483) q[2];
cx q[6], q[2];
rz(-0.7853981633974483) q[2];
cx q[1], q[2];
rz(2.356194490192345) q[2];
sx q[2];
rz(1.5707963267948966) q[2];
rz(0.7853981633974483) q[6];
cx q[1], q[6];
rz(0.7853981633974483) q[1];
rz(-0.7853981633974483) q[6];
cx q[1], q[6];
cx q[2], q[1];
cx q[1], q[6];
rz(1.5707963267948966) q[1];
sx q[1];
rz(1.5707963267948966) q[1];
rz(0.19209333868772005) q[2];
cx q[5], q[1];
rz(-0.7853981633974483) q[1];
cx q[0], q[1];
rz(0.7853981633974483) q[1];
cx q[5], q[1];
rz(-0.7853981633974483) q[1];
cx q[0], q[1];
rz(2.356194490192345) q[1];
sx q[1];
rz(1.5707963267948966) q[1];
rz(0.7853981633974483) q[5];
cx q[0], q[5];
rz(0.7853981633974483) q[0];
rz(-0.7853981633974483) q[5];
cx q[0], q[5];
cx q[1], q[0];
cx q[0], q[5];
rz(0.7814116736776509) q[0];
rz(0.32560222972318487) q[1];
cx q[0], q[1];
cx q[0], q[2];
cx q[0], q[3];
cx q[0], q[4];
cx q[1], q[2];
cx q[1], q[3];
cx q[1], q[4];
cx q[2], q[3];
cx q[2], q[4];
cx q[3], q[4];
rz(0.5860455685519512) q[5];
cx q[0], q[5];
cx q[1], q[5];
cx q[2], q[5];
cx q[3], q[5];
cx q[4], q[5];
rz(0.7833908076908233) q[6];
cx q[0], q[6];
cx q[1], q[6];
cx q[2], q[6];
cx q[3], q[6];
cx q[4], q[6];
cx q[5], q[6];
rz(0.4385791700708005) q[7];
cx q[0], q[7];
cx q[1], q[7];
cx q[2], q[7];
cx q[3], q[7];
cx q[4], q[7];
cx q[5], q[7];
cx q[6], q[7];
rz(0.37366563268146036) q[8];
cx q[0], q[8];
cx q[1], q[8];
cx q[2], q[8];
cx q[3], q[8];
cx q[4], q[8];
cx q[5], q[8];
cx q[6], q[8];
cx q[7], q[8];
rz(0.3861194070665952) q[9];
cx q[0], q[9];
rz(0.598014603729805) q[0];
cx q[1], q[9];
rz(0.2911804950884521) q[1];
cx q[0], q[1];
cx q[2], q[9];
rz(0.5294556332580688) q[2];
cx q[0], q[2];
cx q[1], q[2];
cx q[3], q[9];
rz(0.654749743260911) q[3];
cx q[0], q[3];
cx q[1], q[3];
cx q[2], q[3];
cx q[4], q[9];
rz(0.6583907854564329) q[4];
cx q[0], q[4];
cx q[1], q[4];
cx q[2], q[4];
cx q[3], q[4];
cx q[5], q[9];
rz(0.8131463648994075) q[5];
cx q[0], q[5];
cx q[1], q[5];
cx q[2], q[5];
cx q[3], q[5];
cx q[4], q[5];
cx q[6], q[9];
rz(0.17683269760487433) q[6];
cx q[0], q[6];
cx q[1], q[6];
cx q[2], q[6];
cx q[3], q[6];
cx q[4], q[6];
cx q[5], q[6];
cx q[7], q[9];
rz(0.4271759315059235) q[7];
cx q[0], q[7];
cx q[1], q[7];
cx q[2], q[7];
cx q[3], q[7];
cx q[4], q[7];
cx q[5], q[7];
cx q[6], q[7];
cx q[8], q[9];
rz(0.2075505218618462) q[8];
cx q[0], q[8];
cx q[1], q[8];
cx q[2], q[8];
cx q[3], q[8];
cx q[4], q[8];
cx q[5], q[8];
cx q[6], q[8];
cx q[7], q[8];
rz(0.2541659126322389) q[9];
cx q[0], q[9];
rz(0.32293497802023996) q[0];
cx q[1], q[9];
rz(0.3649721296140257) q[1];
cx q[0], q[1];
cx q[2], q[9];
rz(0.87938830156889) q[2];
cx q[0], q[2];
cx q[1], q[2];
cx q[3], q[9];
rz(0.8076872569617413) q[3];
cx q[0], q[3];
cx q[1], q[3];
cx q[2], q[3];
cx q[4], q[9];
rz(0.5481200116569634) q[4];
cx q[0], q[4];
cx q[1], q[4];
cx q[2], q[4];
cx q[3], q[4];
cx q[5], q[9];
rz(0.2312803201446143) q[5];
cx q[0], q[5];
cx q[1], q[5];
cx q[2], q[5];
cx q[3], q[5];
cx q[4], q[5];
cx q[6], q[9];
rz(0.7045643679184569) q[6];
cx q[0], q[6];
cx q[1], q[6];
cx q[2], q[6];
cx q[3], q[6];
cx q[4], q[6];
cx q[5], q[6];
cx q[7], q[9];
rz(0.3862762655864481) q[7];
cx q[0], q[7];
cx q[1], q[7];
cx q[2], q[7];
cx q[3], q[7];
cx q[4], q[7];
cx q[5], q[7];
cx q[6], q[7];
cx q[8], q[9];
rz(0.501755091849028) q[8];
cx q[0], q[8];
cx q[1], q[8];
cx q[2], q[8];
cx q[3], q[8];
cx q[4], q[8];
cx q[5], q[8];
cx q[6], q[8];
cx q[7], q[8];
rz(0.40673516288056) q[9];
cx q[0], q[9];
rz(0.2675975519204393) q[0];
cx q[1], q[9];
rz(0.1014808068499268) q[1];
cx q[2], q[9];
rz(0.6655308388109844) q[2];
cx q[3], q[9];
rz(0.2589649379841482) q[3];
cx q[4], q[9];
rz(0.48590676884857964) q[4];
cx q[5], q[9];
rz(0.6059053348897899) q[5];
cx q[6], q[9];
rz(0.34462680635564547) q[6];
cx q[7], q[9];
rz(0.2861354779311765) q[7];
cx q[8], q[9];
rz(0.7910703687929918) q[8];
rz(1.8182026451378936) q[9];
sx q[9];
rz(2.356194490192345) q[9];
cx q[9], q[8];
rz(-0.7853981633974483) q[8];
cx q[9], q[8];
rz(2.356194490192345) q[8];
sx q[8];
rz(2.356194490192345) q[8];
rz(0.39269908169872414) q[9];
cx q[9], q[7];
rz(-0.39269908169872414) q[7];
cx q[9], q[7];
rz(0.39269908169872414) q[7];
cx q[8], q[7];
rz(-0.7853981633974483) q[7];
cx q[8], q[7];
rz(2.356194490192345) q[7];
sx q[7];
rz(2.356194490192345) q[7];
rz(0.39269908169872414) q[8];
rz(0.19634954084936207) q[9];
cx q[9], q[6];
rz(-0.19634954084936207) q[6];
cx q[9], q[6];
rz(0.19634954084936207) q[6];
cx q[8], q[6];
rz(-0.39269908169872414) q[6];
cx q[8], q[6];
rz(0.39269908169872414) q[6];
cx q[7], q[6];
rz(-0.7853981633974483) q[6];
cx q[7], q[6];
rz(2.356194490192345) q[6];
sx q[6];
rz(2.356194490192345) q[6];
rz(0.39269908169872414) q[7];
rz(0.19634954084936207) q[8];
rz(0.09817477042468103) q[9];
cx q[9], q[5];
rz(-0.09817477042468103) q[5];
cx q[9], q[5];
rz(0.09817477042468103) q[5];
cx q[8], q[5];
rz(-0.19634954084936207) q[5];
cx q[8], q[5];
rz(0.19634954084936207) q[5];
cx q[7], q[5];
rz(-0.39269908169872414) q[5];
cx q[7], q[5];
rz(0.39269908169872414) q[5];
cx q[6], q[5];
rz(-0.7853981633974483) q[5];
cx q[6], q[5];
rz(2.356194490192345) q[5];
sx q[5];
rz(2.356194490192345) q[5];
rz(0.39269908169872414) q[6];
rz(0.19634954084936207) q[7];
rz(0.09817477042468103) q[8];
rz(0.04908738521234052) q[9];
cx q[9], q[4];
rz(-0.04908738521234052) q[4];
cx q[9], q[4];
rz(0.04908738521234052) q[4];
cx q[8], q[4];
rz(-0.09817477042468103) q[4];
cx q[8], q[4];
rz(0.09817477042468103) q[4];
cx q[7], q[4];
rz(-0.19634954084936207) q[4];
cx q[7], q[4];
rz(0.19634954084936207) q[4];
cx q[6], q[4];
rz(-0.39269908169872414) q[4];
cx q[6], q[4];
rz(0.39269908169872414) q[4];
cx q[5], q[4];
rz(-0.7853981633974483) q[4];
cx q[5], q[4];
rz(2.356194490192345) q[4];
sx q[4];
rz(2.356194490192345) q[4];
rz(0.39269908169872414) q[5];
rz(0.19634954084936207) q[6];
rz(0.09817477042468103) q[7];
rz(0.04908738521234052) q[8];
rz(0.02454369260617026) q[9];
cx q[9], q[3];
rz(-0.02454369260617026) q[3];
cx q[9], q[3];
rz(0.02454369260617026) q[3];
cx q[8], q[3];
rz(-0.04908738521234052) q[3];
cx q[8], q[3];
rz(0.04908738521234052) q[3];
cx q[7], q[3];
rz(-0.09817477042468103) q[3];
cx q[7], q[3];
rz(0.09817477042468103) q[3];
cx q[6], q[3];
rz(-0.19634954084936207) q[3];
cx q[6], q[3];
rz(0.19634954084936207) q[3];
cx q[5], q[3];
rz(-0.39269908169872414) q[3];
cx q[5], q[3];
rz(0.39269908169872414) q[3];
cx q[4], q[3];
rz(-0.7853981633974483) q[3];
cx q[4], q[3];
rz(2.356194490192345) q[3];
sx q[3];
rz(2.356194490192345) q[3];
rz(0.39269908169872414) q[4];
rz(0.19634954084936207) q[5];
rz(0.09817477042468103) q[6];
rz(0.04908738521234052) q[7];
rz(0.02454369260617026) q[8];
rz(0.01227184630308513) q[9];
cx q[9], q[2];
rz(-0.01227184630308513) q[2];
cx q[9], q[2];
rz(0.01227184630308513) q[2];
cx q[8], q[2];
rz(-0.02454369260617026) q[2];
cx q[8], q[2];
rz(0.02454369260617026) q[2];
cx q[7], q[2];
rz(-0.04908738521234052) q[2];
cx q[7], q[2];
rz(0.04908738521234052) q[2];
cx q[6], q[2];
rz(-0.09817477042468103) q[2];
cx q[6], q[2];
rz(0.09817477042468103) q[2];
cx q[5], q[2];
rz(-0.19634954084936207) q[2];
cx q[5], q[2];
rz(0.19634954084936207) q[2];
cx q[4], q[2];
rz(-0.39269908169872414) q[2];
cx q[4], q[2];
rz(0.39269908169872414) q[2];
cx q[3], q[2];
rz(-0.7853981633974483) q[2];
cx q[3], q[2];
rz(2.356194490192345) q[2];
sx q[2];
rz(2.356194490192345) q[2];
rz(0.39269908169872414) q[3];
rz(0.19634954084936207) q[4];
rz(0.09817477042468103) q[5];
rz(0.04908738521234052) q[6];
rz(0.02454369260617026) q[7];
rz(0.01227184630308513) q[8];
rz(0.006135923151542565) q[9];
cx q[9], q[1];
rz(-0.006135923151542565) q[1];
cx q[9], q[1];
rz(0.006135923151542565) q[1];
cx q[8], q[1];
rz(-0.01227184630308513) q[1];
cx q[8], q[1];
rz(0.01227184630308513) q[1];
cx q[7], q[1];
rz(-0.02454369260617026) q[1];
cx q[7], q[1];
rz(0.02454369260617026) q[1];
cx q[6], q[1];
rz(-0.04908738521234052) q[1];
cx q[6], q[1];
rz(0.04908738521234052) q[1];
cx q[5], q[1];
rz(-0.09817477042468103) q[1];
cx q[5], q[1];
rz(0.09817477042468103) q[1];
cx q[4], q[1];
rz(-0.19634954084936207) q[1];
cx q[4], q[1];
rz(0.19634954084936207) q[1];
cx q[3], q[1];
rz(-0.39269908169872414) q[1];
cx q[3], q[1];
rz(0.39269908169872414) q[1];
cx q[2], q[1];
rz(-0.7853981633974483) q[1];
cx q[2], q[1];
rz(2.356194490192345) q[1];
sx q[1];
rz(2.356194490192345) q[1];
rz(0.39269908169872414) q[2];
rz(0.19634954084936207) q[3];
rz(0.09817477042468103) q[4];
rz(0.04908738521234052) q[5];
rz(0.02454369260617026) q[6];
rz(0.01227184630308513) q[7];
rz(0.006135923151542565) q[8];
rz(0.0030679615757712823) q[9];
cx q[9], q[0];
rz(-0.0030679615757712823) q[0];
cx q[9], q[0];
rz(0.0030679615757712823) q[0];
cx q[8], q[0];
rz(-0.006135923151542565) q[0];
cx q[8], q[0];
rz(0.006135923151542565) q[0];
cx q[7], q[0];
rz(-0.01227184630308513) q[0];
cx q[7], q[0];
rz(0.01227184630308513) q[0];
cx q[6], q[0];
rz(-0.02454369260617026) q[0];
cx q[6], q[0];
rz(0.02454369260617026) q[0];
cx q[5], q[0];
rz(-0.04908738521234052) q[0];
cx q[5], q[0];
rz(0.04908738521234052) q[0];
cx q[4], q[0];
rz(-0.09817477042468103) q[0];
cx q[4], q[0];
rz(0.09817477042468103) q[0];
cx q[3], q[0];
rz(-0.19634954084936207) q[0];
cx q[3], q[0];
rz(0.19634954084936207) q[0];
cx q[2], q[0];
rz(-0.39269908169872414) q[0];
cx q[2], q[0];
rz(0.39269908169872414) q[0];
cx q[1], q[0];
rz(-0.7853981633974483) q[0];
cx q[1], q[0];
rz(2.356194490192345) q[0];
sx q[0];
rz(1.5707963267948966) q[0];
cx q[1], q[5];
cx q[1], q[0];
rz(1.5707963267948966) q[1];
sx q[1];
rz(1.5707963267948966) q[1];
cx q[2], q[6];
cx q[3], q[7];
cx q[4], q[8];
cx q[5], q[1];
rz(-0.7853981633974483) q[1];
cx q[0], q[1];
rz(0.7853981633974483) q[1];
cx q[5], q[1];
rz(-0.7853981633974483) q[1];
cx q[0], q[1];
rz(2.356194490192345) q[1];
sx q[1];
rz(1.5707963267948966) q[1];
cx q[2], q[1];
rz(1.5707963267948966) q[2];
sx q[2];
rz(1.5707963267948966) q[2];
rz(0.7853981633974483) q[5];
cx q[0], q[5];
rz(0.7853981633974483) q[0];
rz(-0.7853981633974483) q[5];
cx q[0], q[5];
cx q[6], q[2];
rz(-0.7853981633974483) q[2];
cx q[1], q[2];
rz(0.7853981633974483) q[2];
cx q[6], q[2];
rz(-0.7853981633974483) q[2];
cx q[1], q[2];
rz(2.356194490192345) q[2];
sx q[2];
rz(1.5707963267948966) q[2];
cx q[3], q[2];
rz(1.5707963267948966) q[3];
sx q[3];
rz(1.5707963267948966) q[3];
rz(0.7853981633974483) q[6];
cx q[1], q[6];
rz(0.7853981633974483) q[1];
rz(-0.7853981633974483) q[6];
cx q[1], q[6];
cx q[7], q[3];
rz(-0.7853981633974483) q[3];
cx q[2], q[3];
rz(0.7853981633974483) q[3];
cx q[7], q[3];
rz(-0.7853981633974483) q[3];
cx q[2], q[3];
rz(2.356194490192345) q[3];
sx q[3];
rz(1.5707963267948966) q[3];
cx q[4], q[3];
rz(1.5707963267948966) q[4];
sx q[4];
rz(1.5707963267948966) q[4];
rz(0.7853981633974483) q[7];
cx q[2], q[7];
rz(0.7853981633974483) q[2];
rz(-0.7853981633974483) q[7];
cx q[2], q[7];
cx q[8], q[4];
rz(-0.7853981633974483) q[4];
cx q[3], q[4];
rz(0.7853981633974483) q[4];
cx q[8], q[4];
rz(-0.7853981633974483) q[4];
cx q[3], q[4];
rz(2.356194490192345) q[4];
sx q[4];
rz(1.5707963267948966) q[4];
cx q[4], q[9];
rz(1.5707963267948966) q[4];
sx q[4];
rz(1.5707963267948966) q[4];
rz(0.7853981633974483) q[8];
cx q[3], q[8];
rz(0.7853981633974483) q[3];
rz(-0.7853981633974483) q[8];
cx q[3], q[8];
cx q[8], q[4];
rz(-0.7853981633974483) q[4];
cx q[3], q[4];
rz(0.7853981633974483) q[4];
cx q[8], q[4];
rz(-0.7853981633974483) q[4];
cx q[3], q[4];
rz(2.356194490192345) q[4];
sx q[4];
rz(1.5707963267948966) q[4];
rz(0.7853981633974483) q[8];
cx q[3], q[8];
rz(0.7853981633974483) q[3];
rz(-0.7853981633974483) q[8];
cx q[3], q[8];
cx q[4], q[3];
cx q[3], q[8];
rz(1.5707963267948966) q[3];
sx q[3];
rz(1.5707963267948966) q[3];
rz(0.8893011673793364) q[4];
cx q[7], q[3];
rz(-0.7853981633974483) q[3];
cx q[2], q[3];
rz(0.7853981633974483) q[3];
cx q[7], q[3];
rz(-0.7853981633974483) q[3];
cx q[2], q[3];
rz(2.356194490192345) q[3];
sx q[3];
rz(1.5707963267948966) q[3];
rz(0.7853981633974483) q[7];
cx q[2], q[7];
rz(0.7853981633974483) q[2];
rz(-0.7853981633974483) q[7];
cx q[2], q[7];
cx q[3], q[2];
cx q[2], q[7];
rz(1.5707963267948966) q[2];
sx q[2];
rz(1.5707963267948966) q[2];
rz(0.18082498752650455) q[3];
cx q[6], q[2];
rz(-0.7853981633974483) q[2];
cx q[1], q[2];
rz(0.7853981633974483) q[2];
cx q[6], q[2];
rz(-0.7853981633974483) q[2];
cx q[1], q[2];
rz(2.356194490192345) q[2];
sx q[2];
rz(1.5707963267948966) q[2];
rz(0.7853981633974483) q[6];
cx q[1], q[6];
rz(0.7853981633974483) q[1];
rz(-0.7853981633974483) q[6];
cx q[1], q[6];
cx q[2], q[1];
cx q[1], q[6];
rz(1.5707963267948966) q[1];
sx q[1];
rz(1.5707963267948966) q[1];
rz(0.7838193888463069) q[2];
cx q[5], q[1];
rz(-0.7853981633974483) q[1];
cx q[0], q[1];
rz(0.7853981633974483) q[1];
cx q[5], q[1];
rz(-0.7853981633974483) q[1];
cx q[0], q[1];
rz(2.356194490192345) q[1];
sx q[1];
rz(1.5707963267948966) q[1];
rz(0.7853981633974483) q[5];
cx q[0], q[5];
rz(0.7853981633974483) q[0];
rz(-0.7853981633974483) q[5];
cx q[0], q[5];
cx q[1], q[0];
cx q[0], q[5];
rz(0.6886203062375128) q[0];
rz(0.40922415865898776) q[1];
cx q[0], q[1];
cx q[0], q[2];
cx q[0], q[3];
cx q[0], q[4];
cx q[1], q[2];
cx q[1], q[3];
cx q[1], q[4];
cx q[2], q[3];
cx q[2], q[4];
cx q[3], q[4];
rz(0.6133271999530112) q[5];
cx q[0], q[5];
cx q[1], q[5];
cx q[2], q[5];
cx q[3], q[5];
cx q[4], q[5];
rz(0.17488618432159575) q[6];
cx q[0], q[6];
cx q[1], q[6];
cx q[2], q[6];
cx q[3], q[6];
cx q[4], q[6];
cx q[5], q[6];
rz(0.8320114820678717) q[7];
cx q[0], q[7];
cx q[1], q[7];
cx q[2], q[7];
cx q[3], q[7];
cx q[4], q[7];
cx q[5], q[7];
cx q[6], q[7];
rz(0.16487050961281594) q[8];
cx q[0], q[8];
cx q[1], q[8];
cx q[2], q[8];
cx q[3], q[8];
cx q[4], q[8];
cx q[5], q[8];
cx q[6], q[8];
cx q[7], q[8];
rz(0.7811451403409584) q[9];
cx q[0], q[9];
rz(0.2605464035023731) q[0];
cx q[1], q[9];
rz(0.3539773571150968) q[1];
cx q[0], q[1];
cx q[2], q[9];
rz(0.6303968241600779) q[2];
cx q[0], q[2];
cx q[1], q[2];
cx q[3], q[9];
rz(0.8112176072580833) q[3];
cx q[0], q[3];
cx q[1], q[3];
cx q[2], q[3];
cx q[4], q[9];
rz(0.33650675209155034) q[4];
cx q[0], q[4];
cx q[1], q[4];
cx q[2], q[4];
cx q[3], q[4];
cx q[5], q[9];
rz(0.6943448368415976) q[5];
cx q[0], q[5];
cx q[1], q[5];
cx q[2], q[5];
cx q[3], q[5];
cx q[4], q[5];
cx q[6], q[9];
rz(0.7841403401547086) q[6];
cx q[0], q[6];
cx q[1], q[6];
cx q[2], q[6];
cx q[3], q[6];
cx q[4], q[6];
cx q[5], q[6];
cx q[7], q[9];
rz(0.37973204665317917) q[7];
cx q[0], q[7];
cx q[1], q[7];
cx q[2], q[7];
cx q[3], q[7];
cx q[4], q[7];
cx q[5], q[7];
cx q[6], q[7];
cx q[8], q[9];
rz(0.33698455892341295) q[8];
cx q[0], q[8];
cx q[1], q[8];
cx q[2], q[8];
cx q[3], q[8];
cx q[4], q[8];
cx q[5], q[8];
cx q[6], q[8];
cx q[7], q[8];
rz(0.1646994566906727) q[9];
cx q[0], q[9];
rz(0.6545266058597325) q[0];
cx q[1], q[9];
rz(0.5721665089258618) q[1];
cx q[0], q[1];
cx q[2], q[9];
rz(0.8868696408459871) q[2];
cx q[0], q[2];
cx q[1], q[2];
cx q[3], q[9];
rz(0.11817332985669493) q[3];
cx q[0], q[3];
cx q[1], q[3];
cx q[2], q[3];
cx q[4], q[9];
rz(0.7303724833142492) q[4];
cx q[0], q[4];
cx q[1], q[4];
cx q[2], q[4];
cx q[3], q[4];
cx q[5], q[9];
rz(0.8423909468364056) q[5];
cx q[0], q[5];
cx q[1], q[5];
cx q[2], q[5];
cx q[3], q[5];
cx q[4], q[5];
cx q[6], q[9];
rz(0.7795483013418025) q[6];
cx q[0], q[6];
cx q[1], q[6];
cx q[2], q[6];
cx q[3], q[6];
cx q[4], q[6];
cx q[5], q[6];
cx q[7], q[9];
rz(0.30027390654190433) q[7];
cx q[0], q[7];
cx q[1], q[7];
cx q[2], q[7];
cx q[3], q[7];
cx q[4], q[7];
cx q[5], q[7];
cx q[6], q[7];
cx q[8], q[9];
rz(0.36149069572217685) q[8];
cx q[0], q[8];
cx q[1], q[8];
cx q[2], q[8];
cx q[3], q[8];
cx q[4], q[8];
cx q[5], q[8];
cx q[6], q[8];
cx q[7], q[8];
rz(0.419800364787011) q[9];
cx q[0], q[9];
rz(0.6727438487272339) q[0];
cx q[1], q[9];
rz(0.833685298797227) q[1];
cx q[2], q[9];
rz(0.30745149825612583) q[2];
cx q[3], q[9];
rz(0.21923272150517167) q[3];
cx q[4], q[9];
rz(0.8240535597827466) q[4];
cx q[5], q[9];
rz(0.24070433380543632) q[5];
cx q[6], q[9];
rz(0.42024045003588684) q[6];
cx q[7], q[9];
rz(0.17386797075521232) q[7];
cx q[8], q[9];
rz(0.22742674054390755) q[8];
rz(1.7697374568485182) q[9];
sx q[9];
rz(2.356194490192345) q[9];
cx q[9], q[8];
rz(-0.7853981633974483) q[8];
cx q[9], q[8];
rz(2.356194490192345) q[8];
sx q[8];
rz(2.356194490192345) q[8];
rz(0.39269908169872414) q[9];
cx q[9], q[7];
rz(-0.39269908169872414) q[7];
cx q[9], q[7];
rz(0.39269908169872414) q[7];
cx q[8], q[7];
rz(-0.7853981633974483) q[7];
cx q[8], q[7];
rz(2.356194490192345) q[7];
sx q[7];
rz(2.356194490192345) q[7];
rz(0.39269908169872414) q[8];
rz(0.19634954084936207) q[9];
cx q[9], q[6];
rz(-0.19634954084936207) q[6];
cx q[9], q[6];
rz(0.19634954084936207) q[6];
cx q[8], q[6];
rz(-0.39269908169872414) q[6];
cx q[8], q[6];
rz(0.39269908169872414) q[6];
cx q[7], q[6];
rz(-0.7853981633974483) q[6];
cx q[7], q[6];
rz(2.356194490192345) q[6];
sx q[6];
rz(2.356194490192345) q[6];
rz(0.39269908169872414) q[7];
rz(0.19634954084936207) q[8];
rz(0.09817477042468103) q[9];
cx q[9], q[5];
rz(-0.09817477042468103) q[5];
cx q[9], q[5];
rz(0.09817477042468103) q[5];
cx q[8], q[5];
rz(-0.19634954084936207) q[5];
cx q[8], q[5];
rz(0.19634954084936207) q[5];
cx q[7], q[5];
rz(-0.39269908169872414) q[5];
cx q[7], q[5];
rz(0.39269908169872414) q[5];
cx q[6], q[5];
rz(-0.7853981633974483) q[5];
cx q[6], q[5];
rz(2.356194490192345) q[5];
sx q[5];
rz(2.356194490192345) q[5];
rz(0.39269908169872414) q[6];
rz(0.19634954084936207) q[7];
rz(0.09817477042468103) q[8];
rz(0.04908738521234052) q[9];
cx q[9], q[4];
rz(-0.04908738521234052) q[4];
cx q[9], q[4];
rz(0.04908738521234052) q[4];
cx q[8], q[4];
rz(-0.09817477042468103) q[4];
cx q[8], q[4];
rz(0.09817477042468103) q[4];
cx q[7], q[4];
rz(-0.19634954084936207) q[4];
cx q[7], q[4];
rz(0.19634954084936207) q[4];
cx q[6], q[4];
rz(-0.39269908169872414) q[4];
cx q[6], q[4];
rz(0.39269908169872414) q[4];
cx q[5], q[4];
rz(-0.7853981633974483) q[4];
cx q[5], q[4];
rz(2.356194490192345) q[4];
sx q[4];
rz(2.356194490192345) q[4];
rz(0.39269908169872414) q[5];
rz(0.19634954084936207) q[6];
rz(0.09817477042468103) q[7];
rz(0.04908738521234052) q[8];
rz(0.02454369260617026) q[9];
cx q[9], q[3];
rz(-0.02454369260617026) q[3];
cx q[9], q[3];
rz(0.02454369260617026) q[3];
cx q[8], q[3];
rz(-0.04908738521234052) q[3];
cx q[8], q[3];
rz(0.04908738521234052) q[3];
cx q[7], q[3];
rz(-0.09817477042468103) q[3];
cx q[7], q[3];
rz(0.09817477042468103) q[3];
cx q[6], q[3];
rz(-0.19634954084936207) q[3];
cx q[6], q[3];
rz(0.19634954084936207) q[3];
cx q[5], q[3];
rz(-0.39269908169872414) q[3];
cx q[5], q[3];
rz(0.39269908169872414) q[3];
cx q[4], q[3];
rz(-0.7853981633974483) q[3];
cx q[4], q[3];
rz(2.356194490192345) q[3];
sx q[3];
rz(2.356194490192345) q[3];
rz(0.39269908169872414) q[4];
rz(0.19634954084936207) q[5];
rz(0.09817477042468103) q[6];
rz(0.04908738521234052) q[7];
rz(0.02454369260617026) q[8];
rz(0.01227184630308513) q[9];
cx q[9], q[2];
rz(-0.01227184630308513) q[2];
cx q[9], q[2];
rz(0.01227184630308513) q[2];
cx q[8], q[2];
rz(-0.02454369260617026) q[2];
cx q[8], q[2];
rz(0.02454369260617026) q[2];
cx q[7], q[2];
rz(-0.04908738521234052) q[2];
cx q[7], q[2];
rz(0.04908738521234052) q[2];
cx q[6], q[2];
rz(-0.09817477042468103) q[2];
cx q[6], q[2];
rz(0.09817477042468103) q[2];
cx q[5], q[2];
rz(-0.19634954084936207) q[2];
cx q[5], q[2];
rz(0.19634954084936207) q[2];
cx q[4], q[2];
rz(-0.39269908169872414) q[2];
cx q[4], q[2];
rz(0.39269908169872414) q[2];
cx q[3], q[2];
rz(-0.7853981633974483) q[2];
cx q[3], q[2];
rz(2.356194490192345) q[2];
sx q[2];
rz(2.356194490192345) q[2];
rz(0.39269908169872414) q[3];
rz(0.19634954084936207) q[4];
rz(0.09817477042468103) q[5];
rz(0.04908738521234052) q[6];
rz(0.02454369260617026) q[7];
rz(0.01227184630308513) q[8];
rz(0.006135923151542565) q[9];
cx q[9], q[1];
rz(-0.006135923151542565) q[1];
cx q[9], q[1];
rz(0.006135923151542565) q[1];
cx q[8], q[1];
rz(-0.01227184630308513) q[1];
cx q[8], q[1];
rz(0.01227184630308513) q[1];
cx q[7], q[1];
rz(-0.02454369260617026) q[1];
cx q[7], q[1];
rz(0.02454369260617026) q[1];
cx q[6], q[1];
rz(-0.04908738521234052) q[1];
cx q[6], q[1];
rz(0.04908738521234052) q[1];
cx q[5], q[1];
rz(-0.09817477042468103) q[1];
cx q[5], q[1];
rz(0.09817477042468103) q[1];
cx q[4], q[1];
rz(-0.19634954084936207) q[1];
cx q[4], q[1];
rz(0.19634954084936207) q[1];
cx q[3], q[1];
rz(-0.39269908169872414) q[1];
cx q[3], q[1];
rz(0.39269908169872414) q[1];
cx q[2], q[1];
rz(-0.7853981633974483) q[1];
cx q[2], q[1];
rz(2.356194490192345) q[1];
sx q[1];
rz(2.356194490192345) q[1];
rz(0.39269908169872414) q[2];
rz(0.19634954084936207) q[3];
rz(0.09817477042468103) q[4];
rz(0.04908738521234052) q[5];
rz(0.02454369260617026) q[6];
rz(0.01227184630308513) q[7];
rz(0.006135923151542565) q[8];
rz(0.0030679615757712823) q[9];
cx q[9], q[0];
rz(-0.0030679615757712823) q[0];
cx q[9], q[0];
rz(0.0030679615757712823) q[0];
cx q[8], q[0];
rz(-0.006135923151542565) q[0];
cx q[8], q[0];
rz(0.006135923151542565) q[0];
cx q[7], q[0];
rz(-0.01227184630308513) q[0];
cx q[7], q[0];
rz(0.01227184630308513) q[0];
cx q[6], q[0];
rz(-0.02454369260617026) q[0];
cx q[6], q[0];
rz(0.02454369260617026) q[0];
cx q[5], q[0];
rz(-0.04908738521234052) q[0];
cx q[5], q[0];
rz(0.04908738521234052) q[0];
cx q[4], q[0];
rz(-0.09817477042468103) q[0];
cx q[4], q[0];
rz(0.09817477042468103) q[0];
cx q[3], q[0];
rz(-0.19634954084936207) q[0];
cx q[3], q[0];
rz(0.19634954084936207) q[0];
cx q[2], q[0];
rz(-0.39269908169872414) q[0];
cx q[2], q[0];
rz(0.39269908169872414) q[0];
cx q[1], q[0];
rz(-0.7853981633974483) q[0];
cx q[1], q[0];
rz(2.356194490192345) q[0];
sx q[0];
rz(1.5707963267948966) q[0];
