#include "ads1248.h"
/* Private variables ---------------------------------------------------------*/
const float pt100_table[] =                                                   //PT100 0-100度对应的电阻值，0.1度间隔
{
  80.30628, 80.34599, 80.38570, 80.42541, 80.46512, 80.50483, 80.54453, 80.58424, 80.62394, 80.66364, 
  80.70334, 80.74304, 80.78273, 80.82243, 80.86212, 80.90182, 80.94151, 80.98120, 81.02089, 81.06057, 
  81.10026, 81.13994, 81.17962, 81.21931, 81.25899, 81.29866, 81.33834, 81.37802, 81.41769, 81.45736, 
  81.49704, 81.53671, 81.57638, 81.61604, 81.65571, 81.69537, 81.73504, 81.77470, 81.81436, 81.85402, 
  81.89368, 81.93333, 81.97299, 82.01264, 82.05229, 82.09195, 82.13159, 82.17124, 82.21089, 82.25054, 
  82.29018, 82.32982, 82.36946, 82.40910, 82.44874, 82.48838, 82.52802, 82.56765, 82.60728, 82.64691, 
  82.68654, 82.72617, 82.76580, 82.80543, 82.84505, 82.88468, 82.92430, 82.96392, 83.00354, 83.04316, 
  83.08277, 83.12239, 83.16200, 83.20162, 83.24123, 83.28084, 83.32045, 83.36005, 83.39966, 83.43926, 
  83.47887, 83.51847, 83.55807, 83.59767, 83.63727, 83.67686, 83.71646, 83.75605, 83.79565, 83.83524, 
  83.87483, 83.91442, 83.95400, 83.99359, 84.03317, 84.07276, 84.11234, 84.15192, 84.19150, 84.23108, 
  84.27065, 84.31023, 84.34980, 84.38937, 84.42894, 84.46851, 84.50808, 84.54765, 84.58722, 84.62678, 
  84.66634, 84.70591, 84.74547, 84.78502, 84.82458, 84.86414, 84.90369, 84.94325, 84.98280, 85.02235, 
  85.06190, 85.10145, 85.14100, 85.18054, 85.22009, 85.25963, 85.29917, 85.33871, 85.37825, 85.41779, 
  85.45733, 85.49686, 85.53640, 85.57593, 85.61546, 85.65499, 85.69452, 85.73405, 85.77357, 85.81310, 
  85.85262, 85.89214, 85.93166, 85.97118, 86.01070, 86.05022, 86.08974, 86.12925, 86.16876, 86.20827, 
  86.24779, 86.28729, 86.32680, 86.36631, 86.40581, 86.44532, 86.48482, 86.52432, 86.56382, 86.60332, 
  86.64282, 86.68231, 86.72181, 86.76130, 86.80079, 86.84029, 86.87978, 86.91926, 86.95875, 86.99824, 
  87.03772, 87.07720, 87.11669, 87.15617, 87.19565, 87.23512, 87.27460, 87.31408, 87.35355, 87.39302, 
  87.43249, 87.47196, 87.51143, 87.55090, 87.59037, 87.62983, 87.66930, 87.70876, 87.74822, 87.78768, 
  87.82714, 87.86660, 87.90605, 87.94551, 87.98496, 88.02441, 88.06387, 88.10332, 88.14276, 88.18221, 
  88.22166, 88.26110, 88.30054, 88.33999, 88.37943, 88.41887, 88.45831, 88.49774, 88.53718, 88.57661, 
  88.61605, 88.65548, 88.69491, 88.73434, 88.77377, 88.81319, 88.85262, 88.89204, 88.93147, 88.97089, 
  89.01031, 89.04973, 89.08915, 89.12856, 89.16798, 89.20739, 89.24681, 89.28622, 89.32563, 89.36504, 
  89.40444, 89.44385, 89.48326, 89.52266, 89.56206, 89.60147, 89.64087, 89.68026, 89.71966, 89.75906, 
  89.79845, 89.83785, 89.87724, 89.91663, 89.95602, 89.99541, 90.03480, 90.07419, 90.11357, 90.15296, 
  90.19234, 90.23172, 90.27110, 90.31048, 90.34986, 90.38923, 90.42861, 90.46798, 90.50736, 90.54673, 
  90.58610, 90.62547, 90.66484, 90.70420, 90.74357, 90.78293, 90.82230, 90.86166, 90.90102, 90.94038, 
  90.97973, 91.01909, 91.05845, 91.09780, 91.13715, 91.17651, 91.21586, 91.25521, 91.29455, 91.33390, 
  91.37325, 91.41259, 91.45193, 91.49127, 91.53062, 91.56995, 91.60929, 91.64863, 91.68797, 91.72730, 
  91.76663, 91.80597, 91.84530, 91.88463, 91.92395, 91.96328, 92.00261, 92.04193, 92.08126, 92.12058, 
  92.15990, 92.19922, 92.23854, 92.27785, 92.31717, 92.35648, 92.39580, 92.43511, 92.47442, 92.51373, 
  92.55304, 92.59235, 92.63165, 92.67096, 92.71026, 92.74957, 92.78887, 92.82817, 92.86747, 92.90676, 
  92.94606, 92.98536, 93.02465, 93.06394, 93.10324, 93.14253, 93.18181, 93.22110, 93.26039, 93.29968, 
  93.33896, 93.37824, 93.41752, 93.45681, 93.49609, 93.53536, 93.57464, 93.61392, 93.65319, 93.69246, 
  93.73174, 93.77101, 93.81028, 93.84955, 93.88881, 93.92808, 93.96735, 94.00661, 94.04587, 94.08513, 
  94.12439, 94.16365, 94.20291, 94.24217, 94.28142, 94.32068, 94.35993, 94.39918, 94.43843, 94.47768, 
  94.51693, 94.55618, 94.59542, 94.63467, 94.67391, 94.71315, 94.75239, 94.79163, 94.83087, 94.87011, 
  94.90935, 94.94858, 94.98782, 95.02705, 95.06628, 95.10551, 95.14474, 95.18397, 95.22319, 95.26242, 
  95.30164, 95.34087, 95.38009, 95.41931, 95.45853, 95.49775, 95.53696, 95.57618, 95.61539, 95.65461, 
  95.69382, 95.73303, 95.77224, 95.81145, 95.85066, 95.88986, 95.92907, 95.96827, 96.00748, 96.04668, 
  96.08588, 96.12508, 96.16428, 96.20347, 96.24267, 96.28186, 96.32106, 96.36025, 96.39944, 96.43863, 
  96.47782, 96.51701, 96.55619, 96.59538, 96.63456, 96.67374, 96.71293, 96.75211, 96.79129, 96.83046, 
  96.86964, 96.90882, 96.94799, 96.98716, 97.02634, 97.06551, 97.10468, 97.14385, 97.18301, 97.22218, 
  97.26134, 97.30051, 97.33967, 97.37883, 97.41799, 97.45715, 97.49631, 97.53547, 97.57462, 97.61378, 
  97.65293, 97.69208, 97.73123, 97.77038, 97.80953, 97.84868, 97.88783, 97.92697, 97.96612, 98.00526, 
  98.04440, 98.08354, 98.12268, 98.16182, 98.20096, 98.24009, 98.27923, 98.31836, 98.35749, 98.39662, 
  98.43575, 98.47488, 98.51401, 98.55314, 98.59226, 98.63139, 98.67051, 98.70963, 98.74875, 98.78787, 
  98.82699, 98.86611, 98.90522, 98.94434, 98.98345, 99.02256, 99.06167, 99.10078, 99.13989, 99.17900, 
  99.21811, 99.25721, 99.29632, 99.33542, 99.37452, 99.41362, 99.45272, 99.49182, 99.53092, 99.57002, 
  99.60911, 99.64821, 99.68730, 99.72639, 99.76548, 99.80457, 99.84366, 99.88275, 99.92183, 99.96092, 
  100.0000, 100.0391, 100.0782, 100.1172, 100.1563, 100.1954, 100.2345, 100.2736, 100.3126, 100.3517,
  100.3908, 100.4298, 100.4689, 100.5080, 100.5470, 100.5861, 100.6252, 100.6642, 100.7033, 100.7424,
  100.7814, 100.8205, 100.8595, 100.8986, 100.9377, 100.9767, 101.0158, 101.0548, 101.0939, 101.1329,
  101.1720, 101.2110, 101.2501, 101.2891, 101.3282, 101.3672, 101.4062, 101.4453, 101.4843, 101.5234,
  101.5624, 101.6014, 101.6405, 101.6795, 101.7185, 101.7576, 101.7966, 101.8356, 101.8747, 101.9137,
  101.9527, 101.9917, 102.0308, 102.0698, 102.1088, 102.1478, 102.1868, 102.2259, 102.2649, 102.3039,
  102.3429, 102.3819, 102.4209, 102.4599, 102.4989, 102.5380, 102.5770, 102.6160, 102.6550, 102.6940,
  102.7330, 102.7720, 102.8110, 102.8500, 102.8890, 102.9280, 102.9670, 103.0060, 103.0450, 103.0840,
  103.1229, 103.1619, 103.2009, 103.2399, 103.2789, 103.3179, 103.3569, 103.3958, 103.4348, 103.4738,
  103.5128, 103.5518, 103.5907, 103.6297, 103.6687, 103.7077, 103.7466, 103.7856, 103.8246, 103.8636,
  103.9025, 103.9415, 103.9805, 104.0194, 104.0584, 104.0973, 104.1363, 104.1753, 104.2142, 104.2532,
  104.2921, 104.3311, 104.3701, 104.4090, 104.4480, 104.4869, 104.5259, 104.5648, 104.6038, 104.6427,
  104.6816, 104.7206, 104.7595, 104.7985, 104.8374, 104.8764, 104.9153, 104.9542, 104.9932, 105.0321,
  105.0710, 105.1100, 105.1489, 105.1878, 105.2268, 105.2657, 105.3046, 105.3435, 105.3825, 105.4214,
  105.4603, 105.4992, 105.5381, 105.5771, 105.6160, 105.6549, 105.6938, 105.7327, 105.7716, 105.8105,
  105.8495, 105.8884, 105.9273, 105.9662, 106.0051, 106.0440, 106.0829, 106.1218, 106.1607, 106.1996,
  106.2385, 106.2774, 106.3163, 106.3552, 106.3941, 106.4330, 106.4719, 106.5108, 106.5496, 106.5885,
  106.6274, 106.6663, 106.7052, 106.7441, 106.7830, 106.8218, 106.8607, 106.8996, 106.9385, 106.9774,
  107.0162, 107.0551, 107.0940, 107.1328, 107.1717, 107.2106, 107.2495, 107.2883, 107.3272, 107.3661,
  107.4049, 107.4438, 107.4826, 107.5215, 107.5604, 107.5992, 107.6381, 107.6769, 107.7158, 107.7546,
  107.7935, 107.8324, 107.8712, 107.9101, 107.9489, 107.9877, 108.0266, 108.0654, 108.1043, 108.1431,
  108.1820, 108.2208, 108.2596, 108.2985, 108.3373, 108.3762, 108.4150, 108.4538, 108.4926, 108.5315,
  108.5703, 108.6091, 108.6480, 108.6868, 108.7256, 108.7644, 108.8033, 108.8421, 108.8809, 108.9197,
  108.9585, 108.9974, 109.0362, 109.0750, 109.1138, 109.1526, 109.1914, 109.2302, 109.2690, 109.3078,
  109.3467, 109.3855, 109.4243, 109.4631, 109.5019, 109.5407, 109.5795, 109.6183, 109.6571, 109.6959,
  109.7347, 109.7734, 109.8122, 109.8510, 109.8898, 109.9286, 109.9674, 110.0062, 110.0450, 110.0838,
  110.1225, 110.1613, 110.2001, 110.2389, 110.2777, 110.3164, 110.3552, 110.3940, 110.4328, 110.4715,
  110.5103, 110.5491, 110.5879, 110.6266, 110.6654, 110.7042, 110.7429, 110.7817, 110.8204, 110.8592,
  110.8980, 110.9367, 110.9755, 111.0142, 111.0530, 111.0917, 111.1305, 111.1693, 111.2080, 111.2468,
  111.2855, 111.3242, 111.3630, 111.4017, 111.4405, 111.4792, 111.5180, 111.5567, 111.5954, 111.6342,
  111.6729, 111.7117, 111.7504, 111.7891, 111.8279, 111.8666, 111.9053, 111.9441, 111.9828, 112.0215,
  112.0602, 112.0990, 112.1377, 112.1764, 112.2151, 112.2538, 112.2926, 112.3313, 112.3700, 112.4087,
  112.4474, 112.4861, 112.5248, 112.5636, 112.6023, 112.6410, 112.6797, 112.7184, 112.7571, 112.7958,
  112.8345, 112.8732, 112.9119, 112.9506, 112.9893, 113.0280, 113.0667, 113.1054, 113.1441, 113.1828,
  113.2215, 113.2602, 113.2988, 113.3375, 113.3762, 113.4149, 113.4536, 113.4923, 113.5309, 113.5696,
  113.6083, 113.6470, 113.6857, 113.7243, 113.7630, 113.8017, 113.8404, 113.8790, 113.9177, 113.9564,
  113.9950, 114.0337, 114.0724, 114.1110, 114.1497, 114.1884, 114.2270, 114.2657, 114.3043, 114.3430,
  114.3817, 114.4203, 114.4590, 114.4976, 114.5363, 114.5749, 114.6136, 114.6522, 114.6909, 114.7295,
  114.7681, 114.8068, 114.8454, 114.8841, 114.9227, 114.9614, 115.0000, 115.0386, 115.0773, 115.1159,
  115.1545, 115.1932, 115.2318, 115.2704, 115.3091, 115.3477, 115.3863, 115.4249, 115.4636, 115.5022,
  115.5408, 115.5794, 115.6180, 115.6567, 115.6953, 115.7339, 115.7725, 115.8111, 115.8497, 115.8883,
  115.9270, 115.9656, 116.0042, 116.0428, 116.0814, 116.1200, 116.1586, 116.1972, 116.2358, 116.2744,
  116.3130, 116.3516, 116.3902, 116.4288, 116.4674, 116.5060, 116.5446, 116.5831, 116.6217, 116.6603,
  116.6989, 116.7375, 116.7761, 116.8147, 116.8532, 116.8918, 116.9304, 116.9690, 117.0076, 117.0461,
  117.0847, 117.1233, 117.1619, 117.2004, 117.2390, 117.2776, 117.3161, 117.3547, 117.3933, 117.4318,
  117.4704, 117.5090, 117.5475, 117.5861, 117.6247, 117.6632, 117.7018, 117.7403, 117.7789, 117.8174,
  117.8560, 117.8945, 117.9331, 117.9716, 118.0102, 118.0487, 118.0873, 118.1258, 118.1644, 118.2029,
  118.2414, 118.2800, 118.3185, 118.3571, 118.3956, 118.4341, 118.4727, 118.5112, 118.5497, 118.5883,
  118.6268, 118.6653, 118.7038, 118.7424, 118.7809, 118.8194, 118.8579, 118.8965, 118.9350, 118.9735,
  119.0120, 119.0505, 119.0890, 119.1276, 119.1661, 119.2046, 119.2431, 119.2816, 119.3201, 119.3586,
  119.3971, 119.4356, 119.4741, 119.5126, 119.5511, 119.5896, 119.6281, 119.6666, 119.7051, 119.7436,
  119.7821, 119.8206, 119.8591, 119.8976, 119.9361, 119.9746, 120.0131, 120.0516, 120.0900, 120.1285,
  120.1670, 120.2055, 120.2440, 120.2824, 120.3209, 120.3594, 120.3979, 120.4364, 120.4748, 120.5133,
  120.5518, 120.5902, 120.6287, 120.6672, 120.7056, 120.7441, 120.7826, 120.8210, 120.8595, 120.8980,
  120.9364, 120.9749, 121.0133, 121.0518, 121.0902, 121.1287, 121.1672, 121.2056, 121.2441, 121.2825,
  121.3210, 121.3594, 121.3978, 121.4363, 121.4747, 121.5132, 121.5516, 121.5901, 121.6285, 121.6669,
  121.7054, 121.7438, 121.7822, 121.8207, 121.8591, 121.8975, 121.9360, 121.9744, 122.0128, 122.0513,
  122.0897, 122.1281, 122.1665, 122.2049, 122.2434, 122.2818, 122.3202, 122.3586, 122.3970, 122.4355,
  122.4739, 122.5123, 122.5507, 122.5891, 122.6275, 122.6659, 122.7043, 122.7427, 122.7811, 122.8195,
  122.8579, 122.8963, 122.9347, 122.9731, 123.0115, 123.0499, 123.0883, 123.1267, 123.1651, 123.2035,
  123.2419, 123.2803, 123.3187, 123.3571, 123.3955, 123.4338, 123.4722, 123.5106, 123.5490, 123.5874,
  123.6257, 123.6641, 123.7025, 123.7409, 123.7792, 123.8176, 123.8560, 123.8944, 123.9327, 123.9711,
  124.0095, 124.0478, 124.0862, 124.1246, 124.1629, 124.2013, 124.2396, 124.2780, 124.3164, 124.3547,
  124.3931, 124.4314, 124.4698, 124.5081, 124.5465, 124.5848, 124.6232, 124.6615, 124.6999, 124.7382,
  124.7766, 124.8149, 124.8533, 124.8916, 124.9299, 124.9683, 125.0066, 125.0450, 125.0833, 125.1216,
  125.1600, 125.1983, 125.2366, 125.2749, 125.3133, 125.3516, 125.3899, 125.4283, 125.4666, 125.5049,
  125.5432, 125.5815, 125.6199, 125.6582, 125.6965, 125.7348, 125.7731, 125.8114, 125.8497, 125.8881,
  125.9264, 125.9647, 126.0030, 126.0413, 126.0796, 126.1179, 126.1562, 126.1945, 126.2328, 126.2711,
  126.3094, 126.3477, 126.3860, 126.4243, 126.4626, 126.5009, 126.5392, 126.5775, 126.6157, 126.6540,
  126.6923, 126.7306, 126.7689, 126.8072, 126.8455, 126.8837, 126.9220, 126.9603, 126.9986, 127.0368,
  127.0751, 127.1134, 127.1517, 127.1899, 127.2282, 127.2665, 127.3048, 127.3430, 127.3813, 127.4195,
  127.4578, 127.4961, 127.5343, 127.5726, 127.6109, 127.6491, 127.6874, 127.7256, 127.7639, 127.8021,
  127.8404, 127.8786, 127.9169, 127.9551, 127.9934, 128.0316, 128.0699, 128.1081, 128.1464, 128.1846,
  128.2228, 128.2611, 128.2993, 128.3376, 128.3758, 128.4140, 128.4523, 128.4905, 128.5287, 128.5670,
  128.6052, 128.6434, 128.6816, 128.7199, 128.7581, 128.7963, 128.8345, 128.8728, 128.9110, 128.9492,
  128.9874, 129.0256, 129.0638, 129.1021, 129.1403, 129.1785, 129.2167, 129.2549, 129.2931, 129.3313,
  129.3695, 129.4077, 129.4459, 129.4841, 129.5223, 129.5605, 129.5987, 129.6369, 129.6751, 129.7133,
  129.7515, 129.7897, 129.8279, 129.8661, 129.9043, 129.9425, 129.9807, 130.0188, 130.0570, 130.0952,
  130.1334, 130.1716, 130.2098, 130.2479, 130.2861, 130.3243, 130.3625, 130.4006, 130.4388, 130.4770,
  130.5152, 130.5533, 130.5915, 130.6297, 130.6678, 130.7060, 130.7442, 130.7823, 130.8205, 130.8586,
  130.8968, 130.9350, 130.9731, 131.0113, 131.0494, 131.0876, 131.1257, 131.1639, 131.2020, 131.2402,
  131.2783, 131.3165, 131.3546, 131.3928, 131.4309, 131.4691, 131.5072, 131.5453, 131.5835, 131.6216,
  131.6597, 131.6979, 131.7360, 131.7742, 131.8123, 131.8504, 131.8885, 131.9267, 131.9648, 132.0029,
  132.0411, 132.0792, 132.1173, 132.1554, 132.1935, 132.2317, 132.2698, 132.3079, 132.3460, 132.3841,
  132.4222, 132.4603, 132.4985, 132.5366, 132.5747, 132.6128, 132.6509, 132.6890, 132.7271, 132.7652,
  132.8033, 132.8414, 132.8795, 132.9176, 132.9557, 132.9938, 133.0319, 133.0700, 133.1081, 133.1462,
  133.1843, 133.2224, 133.2604, 133.2985, 133.3366, 133.3747, 133.4128, 133.4509, 133.4889, 133.5270,
  133.5651, 133.6032, 133.6413, 133.6793, 133.7174, 133.7555, 133.7935, 133.8316, 133.8697, 133.9078,
  133.9458, 133.9839, 134.0220, 134.0600, 134.0981, 134.1361, 134.1742, 134.2123, 134.2503, 134.2884,
  134.3264, 134.3645, 134.4025, 134.4406, 134.4786, 134.5167, 134.5547, 134.5928, 134.6308, 134.6689,
  134.7069, 134.7450, 134.7830, 134.8211, 134.8591, 134.8971, 134.9352, 134.9732, 135.0112, 135.0493,
  135.0873, 135.1253, 135.1634, 135.2014, 135.2394, 135.2774, 135.3155, 135.3535, 135.3915, 135.4295,
  135.4676, 135.5056, 135.5436, 135.5816, 135.6196, 135.6577, 135.6957, 135.7337, 135.7717, 135.8097,
  135.8477, 135.8857, 135.9237, 135.9617, 135.9997, 136.0377, 136.0757, 136.1137, 136.1517, 136.1897,
  136.2277, 136.2657, 136.3037, 136.3417, 136.3797, 136.4177, 136.4557, 136.4937, 136.5317, 136.5697,
  136.6077, 136.6456, 136.6836, 136.7216, 136.7596, 136.7976, 136.8355, 136.8735, 136.9115, 136.9495,
  136.9875, 137.0254, 137.0634, 137.1014, 137.1393, 137.1773, 137.2153, 137.2532, 137.2912, 137.3292,
  137.3671, 137.4051, 137.4431, 137.4810, 137.5190, 137.5569, 137.5949, 137.6329, 137.6708, 137.7088,
  137.7467, 137.7847, 137.8226, 137.8606, 137.8985, 137.9365, 137.9744, 138.0123, 138.0503, 138.0882,
  138.1262, 138.1641, 138.2020, 138.2400, 138.2779, 138.3158, 138.3538, 138.3917, 138.4296, 138.4676,
  138.5055
};
/*******************************************************************************
* @Function Name: ADS1248_2_Delay
* @Description  : 延时 一次循环约1us
* @param		: vu32 nCount
* @retval	   	: void
* @date		   	: 2014/10/08
*******************************************************************************/
void ADS1248_Delay(vu32 nCount)
{
  u32 i, j;
  
  for(i = 0; i < nCount; i++)
  {
    for(j = 0; j < 10; j++);
  }
}
void SPI1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化TEM2_DRDY
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化TEM1_DRDY
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化TEM1_CS
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化TEM2_CS TEM2_START
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 |GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化TEM1_RST TEM2_RST
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化TEM1_START
  
  GPIO_SetBits(GPIOB,GPIO_Pin_1);
  GPIO_SetBits(GPIOC,GPIO_Pin_4 |GPIO_Pin_5);
  GPIO_SetBits(GPIOD,GPIO_Pin_11 |GPIO_Pin_12);
  GPIO_SetBits(GPIOE,GPIO_Pin_7);
  
  
  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
  
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PB3复用为 SPI1
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PB4复用为 SPI1
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PB5复用为 SPI1
  
  
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为高电平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;		//定义波特率预分频的值:波特率预分频值为256
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
  SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
  SPI_Cmd(SPI1, ENABLE); //使能SPI外设 
}  
/*******************************************************************************
* @Function Name: SP1_ReadWriteByte
* @Description  : Sends a byte through the SPI interface and return the byte received from the SPI bus.
* @param		: uint8_t data byte to send
* @retval	   	: uint8_t The value of the received byte
* @date		   	: 2014/10/04
*******************************************************************************/
uint8_t SP1_ReadWriteByte(uint8_t data)
{
  /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  
  /* Send uint8_t through the SPI bus peripheral */
  SPI_I2S_SendData(SPI1, data);
  
  /* Wait to receive a uint8_t */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  
  /* Return the uint8_t read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}
/*******************************************************************************
* @Function Name: ADS1248_ReadByte
* @Description  : Reads a byte from the SPI Device
* @param		: void
* @retval	   	: u8 Byte Read from the SPI Device
* @date		   	: 2014/10/04
*******************************************************************************/
uint8_t ADS1248_ReadByte(void)
{
  return (SP1_ReadWriteByte(ADS_NOP));
}
/*******************************************************************************
* @Function Name: ADS1248_SetIDAC
* @Description  :
* @param		: uint8_t idac1
* @param		: uint8_t idac2
* @param		: uint8_t idacImage
* @retval	   	: void
* @date		   	: 2014/10/04
*******************************************************************************/
void ADS1248_SetIDAC(uint8_t idac1, uint8_t idac2, uint8_t idacImage)
{
  SP1_ReadWriteByte(ADS_WREG | ADS_IDAC0);
  SP1_ReadWriteByte(0x01);
  SP1_ReadWriteByte(idacImage);
  SP1_ReadWriteByte(idac1 | idac2);
}
/*******************************************************************************
* @Function Name: ADS1248_SetPGAGainAndDataRate
* @Description  : Set PGA gain and data rate (SPS)
* @param		: uint8_t pgaGain
* @param		: uint8_t dataRate
* @retval	   	: void
* @date		   	: 2014/10/04
*******************************************************************************/
void ADS1248_SetPGAGainAndDataRate(uint8_t pgaGain, uint8_t dataRate)
{
  SP1_ReadWriteByte(ADS_WREG | ADS_SYS0);
  SP1_ReadWriteByte(0x00);
  SP1_ReadWriteByte(pgaGain | dataRate);
}
/*******************************************************************************
* @Function Name: ADS1248_SetBurnoutCurrentAndInputChannel
* @Description  :
* @param		: uint8_t positiveChannel
* @param		: uint8_t negativeChannel
* @retval	   	: void
* @date		   	: 2014/10/04
*******************************************************************************/
void ADS1248_SetBurnoutCurrentAndInputChannel(uint8_t burnoutCurrent, uint8_t positiveChannel, uint8_t negativeChannel)
{
  SP1_ReadWriteByte(ADS_WREG | ADS_MUX0);
  SP1_ReadWriteByte(0x00);
  SP1_ReadWriteByte(burnoutCurrent | positiveChannel | negativeChannel);
}
/*******************************************************************************
* @Function Name: ADS1248_SetReference
* @Description  :
* @param		: uint8_t interVrefOnOff
* @param		: uint8_t refSelected
* @retval	   	: void
* @date		   	: 2014/10/04
*******************************************************************************/
void ADS1248_SetReference(uint8_t interVrefOnOff, uint8_t refSelected)
{
  SP1_ReadWriteByte(ADS_WREG | ADS_MUX1);
  SP1_ReadWriteByte(0x00);
  SP1_ReadWriteByte(interVrefOnOff | refSelected);
}

/*******************************************************************************
* @Function Name: ADS1248_ReadREG
* @Description  : 读取寄存器的第byteToRead个字节
* @param		: uint8_t regName
* @param		: uint8_t byteToRead
* @retval	   	: uint8_t
* @date		   	: 2014/10/05
*******************************************************************************/
uint8_t ADS1248_ReadREG(uint8_t regName, uint8_t byteToRead)
{
  SP1_ReadWriteByte(ADS_RREG | regName);
  
  if(byteToRead == 1)
  {
    SP1_ReadWriteByte(0x00);
  }
  else if(byteToRead == 2)
  {
    SP1_ReadWriteByte(0x01);
    ADS1248_ReadByte();
  }
  
  return ADS1248_ReadByte();
}

/*******************************************************************************
* @Function Name: ADS1248_Init
* @Description  : ADS1248初始化
* @param		: void
* @retval	   	: void
* @date		   	: 2014/10/04
*******************************************************************************/
void ADS1248_Init(void)
{
 SPI1_Init();
  TEM1_RST_L();
  TEM1_START_L();
  ADS1248_Delay(4000);
  TEM1_RST_H();
  TEM1_START_H(); 
  ADS1248_Delay(20000);
  TEM1_CS_L();
  uint8_t adcreg=0;
adcreg=adcreg;
  /********                            *************************************/
  /*    检测传感器完毕，取消Burnout Current，       设置外部REF1为参考源   */
  /************************************************************************/
  
  ADS1248_SetBurnoutCurrentAndInputChannel(CURRENT_OFF, P_AIN0, N_AIN1);	                //关闭检测电流,设置传感器通道1
  adcreg=ADS1248_ReadREG(ADS_MUX0,1);
  ADS1248_SetPGAGainAndDataRate(PGAGain_4, DataRate_2000);					// 设置PGA倍数和传输速率
  adcreg=ADS1248_ReadREG(ADS_SYS0,1);
  ADS1248_SetReference(REF_Inter_AlwaysOn, SELT_REF1);					// 设置外部REF1为参考源,内部基准打开电流源才会生效
  adcreg=ADS1248_ReadREG(ADS_MUX1,1);
  
  ADS1248_Delay(10000);													                 // 等待基准源稳定
  ADS1248_SetIDAC(IDAC1_IEXT1, IDAC2_IEXT2, IMAG_1500);				         // 设置电流源为1.5mA	
  adcreg=ADS1248_ReadREG(ADS_IDAC0,1);
  TEM1_CS_H();
  
  
  TEM2_RST_L();
  TEM2_START_L();
  ADS1248_Delay(4000);
  TEM2_RST_H();
  TEM2_START_H(); 
  ADS1248_Delay(20000);
  TEM2_CS_L();
  
  /********                            *************************************/
  /*    检测传感器完毕，取消Burnout Current，       设置外部REF1为参考源   */
  /************************************************************************/
  
 ADS1248_SetBurnoutCurrentAndInputChannel(CURRENT_OFF, P_AIN0, N_AIN1);	                //关闭检测电流,设置传感器通道1
  adcreg=ADS1248_ReadREG(ADS_MUX0,1);
  ADS1248_SetPGAGainAndDataRate(PGAGain_4, DataRate_2000);					// 设置PGA倍数和传输速率
  adcreg=ADS1248_ReadREG(ADS_SYS0,1);
  ADS1248_SetReference(REF_Inter_AlwaysOn, SELT_REF1);					// 设置外部REF1为参考源,内部基准打开电流源才会生效
  adcreg=ADS1248_ReadREG(ADS_MUX1,1);
  
  ADS1248_Delay(10000);													                 // 等待基准源稳定
  ADS1248_SetIDAC(IDAC1_IEXT1, IDAC2_IEXT2, IMAG_1500);				         // 设置电流源为1.5mA	
  adcreg=ADS1248_ReadREG(ADS_IDAC0,1);
  TEM2_CS_H();
}
/*******************************************************************************
* @Function Name: ADS1248_ReadADC
* @Description  : 返回有符号数值
* @param		: void
* @retval	   	: int32_t
* @date		   	: 2014/10/04
*******************************************************************************/
int32_t ADS1248_ReadADC(ADS1248_Type adc)
{
  if(adc==TEM1)
  {
    uint8_t temp[3]={0};
    int32_t adcVal[10] = {0},tmp=0;
    int64_t val=0;
    uint32_t wait_time = 0;
    TEM1_CS_L();
    TEM1_START_H();
    for(uint8_t i=0;i<10;i++)
    {
      
      
     
      
      wait_time =  2000;                                                 //限时等待 防死机
      
      while(TEM1_DRDY() && (wait_time--) )                     //DRDY引脚高电平表示数据没准备好
      {
	;
      }                                                        
      if(wait_time == 0)                                               //超时报错
      {
	;
      }
      
      SP1_ReadWriteByte(ADS_RDATA);
      
      //进入中断                                            /* 读取数据期间禁止任务调度     */
      temp[0] = ADS1248_ReadByte();
      temp[1] = ADS1248_ReadByte();
      temp[2] = ADS1248_ReadByte();
      
      //进入中断
      
      adcVal[i] = (int32_t)(temp[0] << 24) + (temp[1] << 16) + (temp[2] << 8); // 左移8位，放大256倍识别正负号
    }
    TEM1_CS_H();
    for(uint8_t i=0;i<10;i++)
    {
      for(uint8_t j=0;j<10-i;j++)
      {
	if(adcVal[j]>adcVal[j+1])
	{
	  tmp=adcVal[j];
	  adcVal[j]=adcVal[j+1];
	  adcVal[j+1]=tmp;
	}
      }
    }
    for(uint8_t i=0;i<6;i++)
    {
      val+=adcVal[2+i];
    }
    val/=6;
    return (val >> 8);                                             // 右移8位恢复原值
  }
  else if(adc==TEM2)
  {
    uint8_t temp[3]={0};
    int32_t adcVal[10] = {0},tmp=0;
    int64_t val=0;
    uint32_t wait_time = 0;
    TEM2_CS_L();
    TEM2_START_H();
    for(uint8_t i=0;i<10;i++)
    {
      
      
     
      
      wait_time =  2000;                                                 //限时等待 防死机
      
      while(TEM2_DRDY() && (wait_time--) )                     //DRDY引脚高电平表示数据没准备好
      {
	;
      }                                                        
      if(wait_time == 0)                                               //超时报错
      {
	;
      }
      
      SP1_ReadWriteByte(ADS_RDATA);
      
      //进入中断                                            /* 读取数据期间禁止任务调度     */
      temp[0] = ADS1248_ReadByte();
      temp[1] = ADS1248_ReadByte();
      temp[2] = ADS1248_ReadByte();
      
      //进入中断
      
      adcVal[i] = (int32_t)(temp[0] << 24) + (temp[1] << 16) + (temp[2] << 8); // 左移8位，放大256倍识别正负号
    }
    TEM2_CS_H();
    for(uint8_t i=0;i<10;i++)
    {
      for(uint8_t j=0;j<10-i;j++)
      {
	if(adcVal[j]>adcVal[j+1])
	{
	  tmp=adcVal[j];
	  adcVal[j]=adcVal[j+1];
	  adcVal[j+1]=tmp;
	}
      }
    }
    for(uint8_t i=0;i<6;i++)
    {
      val+=adcVal[2+i];
    }
    val/=6;
    return (val >> 8);                                             // 右移8位恢复原值
  }
  else return -1111;
}
/*******************************************************************************
* @Function Name: ADS1248_ResToTemp
* @Description  : 二分法查表，将PT100的电阻值转为对应温度0-100
* @param		: float resistance
* @retval	   	       : float
* @date		   	: 2014/10/06
*******************************************************************************/
float ADS1248_ResToTemp(float resistance)
{
  uint16_t low = 0;
  uint16_t mid = sizeof(pt100_table) / sizeof(pt100_table[0]) / 2;
  uint16_t high = sizeof(pt100_table) / sizeof(pt100_table[0]) - 1;
  
  if((resistance < pt100_table[0]) || (resistance > pt100_table[high]))
  {
    return -100;      // 超限，不做特殊处理  返回0度
  }
  
  while(high >= low)
  {
    mid = (high + low) / 2;
    
    if(resistance >= pt100_table[mid])
    {
      if(resistance < pt100_table[mid + 1])
      {
	return (0.1 * mid + (resistance - pt100_table[mid]) * 0.1 / ((pt100_table[mid + 1]) - pt100_table[mid]))-50;
	//return ( mid + (resistance - pt100_table[mid]) / ((pt100_table[mid + 1]) - pt100_table[mid]));
      }
      else
      {
	low = mid + 1;
      }
    }
    else if(resistance < pt100_table[mid])
    {
      high = mid - 1;
    }
  }
  
  return -1; // 查找失败，不做特殊处理
}