%NODE
9

%NODE.COORD
9
2	1.0	0.0	0.0
8	1.0	2.0	0.0
4	0.0	1.0	0.0
5	1.0	1.0	0.0
6	2.0	1.0	0.0
3	2.0	0.0	0.0
1	0.0	0.0	0.0
7	0.0	2.0	0.0
9	2.0	2.0	0.0

%MATERIAL
2

%MATERIAL.ISOTROPIC
1
1	2.0e+10	0.36

%MATERIAL.ISOTROPIC
1
2	1.0e+10	0.24

%ELEMENT
4

%ELEMENT.Q4
1
3	1	0	0	4	5	8	7

%ELEMENT.Q4
3
1	1	0	0	1	2	5	4
2	2	0	0	2	3	6	5
4	2	0	0	5	6	9	8