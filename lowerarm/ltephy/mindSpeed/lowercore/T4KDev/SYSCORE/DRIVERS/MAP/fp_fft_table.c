unsigned fp_fft_table_size=8192;
unsigned short fp_fft_table[]={
0x7fff, 0x0000, 0x7fff, 0xffce, 0x7fff, 0xff9b, 0x7fff, 0xff69, 
0x7fff, 0xff37, 0x7fff, 0xff05, 0x7fff, 0xfed2, 0x7ffe, 0xfea0, 
0x7ffe, 0xfe6e, 0x7ffd, 0xfe3c, 0x7ffc, 0xfe09, 0x7ffb, 0xfdd7, 
0x7ffa, 0xfda5, 0x7ff9, 0xfd73, 0x7ff8, 0xfd40, 0x7ff7, 0xfd0e, 
0x7ff6, 0xfcdc, 0x7ff5, 0xfcaa, 0x7ff4, 0xfc77, 0x7ff2, 0xfc45, 
0x7ff1, 0xfc13, 0x7fef, 0xfbe1, 0x7fed, 0xfbae, 0x7fec, 0xfb7c, 
0x7fea, 0xfb4a, 0x7fe8, 0xfb18, 0x7fe6, 0xfae5, 0x7fe4, 0xfab3, 
0x7fe2, 0xfa81, 0x7fe0, 0xfa4f, 0x7fdd, 0xfa1d, 0x7fdb, 0xf9ea, 
0x7fd9, 0xf9b8, 0x7fd6, 0xf986, 0x7fd3, 0xf954, 0x7fd1, 0xf922, 
0x7fce, 0xf8ef, 0x7fcb, 0xf8bd, 0x7fc8, 0xf88b, 0x7fc5, 0xf859, 
0x7fc2, 0xf827, 0x7fbf, 0xf7f4, 0x7fbc, 0xf7c2, 0x7fb9, 0xf790, 
0x7fb5, 0xf75e, 0x7fb2, 0xf72c, 0x7fae, 0xf6fa, 0x7fab, 0xf6c8, 
0x7fa7, 0xf695, 0x7fa3, 0xf663, 0x7fa0, 0xf631, 0x7f9c, 0xf5ff, 
0x7f98, 0xf5cd, 0x7f94, 0xf59b, 0x7f90, 0xf569, 0x7f8b, 0xf537, 
0x7f87, 0xf505, 0x7f83, 0xf4d3, 0x7f7e, 0xf4a0, 0x7f7a, 0xf46e, 
0x7f75, 0xf43c, 0x7f71, 0xf40a, 0x7f6c, 0xf3d8, 0x7f67, 0xf3a6, 
0x7f62, 0xf374, 0x7f5d, 0xf342, 0x7f58, 0xf310, 0x7f53, 0xf2de, 
0x7f4e, 0xf2ac, 0x7f49, 0xf27a, 0x7f43, 0xf248, 0x7f3e, 0xf216, 
0x7f38, 0xf1e4, 0x7f33, 0xf1b2, 0x7f2d, 0xf180, 0x7f27, 0xf14e, 
0x7f22, 0xf11c, 0x7f1c, 0xf0eb, 0x7f16, 0xf0b9, 0x7f10, 0xf087, 
0x7f0a, 0xf055, 0x7f03, 0xf023, 0x7efd, 0xeff1, 0x7ef7, 0xefbf, 
0x7ef0, 0xef8d, 0x7eea, 0xef5c, 0x7ee3, 0xef2a, 0x7edd, 0xeef8, 
0x7ed6, 0xeec6, 0x7ecf, 0xee94, 0x7ec8, 0xee62, 0x7ec1, 0xee31, 
0x7eba, 0xedff, 0x7eb3, 0xedcd, 0x7eac, 0xed9b, 0x7ea5, 0xed6a, 
0x7e9d, 0xed38, 0x7e96, 0xed06, 0x7e8e, 0xecd5, 0x7e87, 0xeca3, 
0x7e7f, 0xec71, 0x7e78, 0xec3f, 0x7e70, 0xec0e, 0x7e68, 0xebdc, 
0x7e60, 0xebab, 0x7e58, 0xeb79, 0x7e50, 0xeb47, 0x7e48, 0xeb16, 
0x7e3f, 0xeae4, 0x7e37, 0xeab3, 0x7e2f, 0xea81, 0x7e26, 0xea4f, 
0x7e1e, 0xea1e, 0x7e15, 0xe9ec, 0x7e0c, 0xe9bb, 0x7e03, 0xe989, 
0x7dfb, 0xe958, 0x7df2, 0xe926, 0x7de9, 0xe8f5, 0x7de0, 0xe8c4, 
0x7dd6, 0xe892, 0x7dcd, 0xe861, 0x7dc4, 0xe82f, 0x7dba, 0xe7fe, 
0x7db1, 0xe7cd, 0x7da7, 0xe79b, 0x7d9e, 0xe76a, 0x7d94, 0xe739, 
0x7d8a, 0xe707, 0x7d81, 0xe6d6, 0x7d77, 0xe6a5, 0x7d6d, 0xe673, 
0x7d63, 0xe642, 0x7d58, 0xe611, 0x7d4e, 0xe5e0, 0x7d44, 0xe5af, 
0x7d3a, 0xe57d, 0x7d2f, 0xe54c, 0x7d25, 0xe51b, 0x7d1a, 0xe4ea, 
0x7d0f, 0xe4b9, 0x7d05, 0xe488, 0x7cfa, 0xe457, 0x7cef, 0xe426, 
0x7ce4, 0xe3f4, 0x7cd9, 0xe3c3, 0x7cce, 0xe392, 0x7cc2, 0xe361, 
0x7cb7, 0xe330, 0x7cac, 0xe2ff, 0x7ca0, 0xe2cf, 0x7c95, 0xe29e, 
0x7c89, 0xe26d, 0x7c7e, 0xe23c, 0x7c72, 0xe20b, 0x7c66, 0xe1da, 
0x7c5a, 0xe1a9, 0x7c4e, 0xe178, 0x7c42, 0xe148, 0x7c36, 0xe117, 
0x7c2a, 0xe0e6, 0x7c1e, 0xe0b5, 0x7c11, 0xe085, 0x7c05, 0xe054, 
0x7bf9, 0xe023, 0x7bec, 0xdff2, 0x7bdf, 0xdfc2, 0x7bd3, 0xdf91, 
0x7bc6, 0xdf61, 0x7bb9, 0xdf30, 0x7bac, 0xdeff, 0x7b9f, 0xdecf, 
0x7b92, 0xde9e, 0x7b85, 0xde6e, 0x7b78, 0xde3d, 0x7b6a, 0xde0d, 
0x7b5d, 0xdddc, 0x7b50, 0xddac, 0x7b42, 0xdd7c, 0x7b34, 0xdd4b, 
0x7b27, 0xdd1b, 0x7b19, 0xdcea, 0x7b0b, 0xdcba, 0x7afd, 0xdc8a, 
0x7aef, 0xdc59, 0x7ae1, 0xdc29, 0x7ad3, 0xdbf9, 0x7ac5, 0xdbc9, 
0x7ab7, 0xdb99, 0x7aa8, 0xdb68, 0x7a9a, 0xdb38, 0x7a8c, 0xdb08, 
0x7a7d, 0xdad8, 0x7a6e, 0xdaa8, 0x7a60, 0xda78, 0x7a51, 0xda48, 
0x7a42, 0xda18, 0x7a33, 0xd9e8, 0x7a24, 0xd9b8, 0x7a15, 0xd988, 
0x7a06, 0xd958, 0x79f7, 0xd928, 0x79e7, 0xd8f8, 0x79d8, 0xd8c8, 
0x79c9, 0xd898, 0x79b9, 0xd869, 0x79aa, 0xd839, 0x799a, 0xd809, 
0x798a, 0xd7d9, 0x797a, 0xd7aa, 0x796a, 0xd77a, 0x795b, 0xd74a, 
0x794a, 0xd71b, 0x793a, 0xd6eb, 0x792a, 0xd6bb, 0x791a, 0xd68c, 
0x790a, 0xd65c, 0x78f9, 0xd62d, 0x78e9, 0xd5fd, 0x78d8, 0xd5ce, 
0x78c8, 0xd59e, 0x78b7, 0xd56f, 0x78a6, 0xd53f, 0x7895, 0xd510, 
0x7885, 0xd4e1, 0x7874, 0xd4b1, 0x7863, 0xd482, 0x7851, 0xd453, 
0x7840, 0xd424, 0x782f, 0xd3f4, 0x781e, 0xd3c5, 0x780c, 0xd396, 
0x77fb, 0xd367, 0x77e9, 0xd338, 0x77d8, 0xd309, 0x77c6, 0xd2da, 
0x77b4, 0xd2ab, 0x77a2, 0xd27c, 0x7790, 0xd24d, 0x777e, 0xd21e, 
0x776c, 0xd1ef, 0x775a, 0xd1c0, 0x7748, 0xd191, 0x7736, 0xd162, 
0x7723, 0xd134, 0x7711, 0xd105, 0x76fe, 0xd0d6, 0x76ec, 0xd0a7, 
0x76d9, 0xd079, 0x76c7, 0xd04a, 0x76b4, 0xd01b, 0x76a1, 0xcfed, 
0x768e, 0xcfbe, 0x767b, 0xcf90, 0x7668, 0xcf61, 0x7655, 0xcf33, 
0x7642, 0xcf04, 0x762e, 0xced6, 0x761b, 0xcea7, 0x7608, 0xce79, 
0x75f4, 0xce4b, 0x75e1, 0xce1c, 0x75cd, 0xcdee, 0x75b9, 0xcdc0, 
0x75a6, 0xcd92, 0x7592, 0xcd63, 0x757e, 0xcd35, 0x756a, 0xcd07, 
0x7556, 0xccd9, 0x7542, 0xccab, 0x752d, 0xcc7d, 0x7519, 0xcc4f, 
0x7505, 0xcc21, 0x74f0, 0xcbf3, 0x74dc, 0xcbc5, 0x74c7, 0xcb97, 
0x74b3, 0xcb69, 0x749e, 0xcb3c, 0x7489, 0xcb0e, 0x7475, 0xcae0, 
0x7460, 0xcab2, 0x744b, 0xca85, 0x7436, 0xca57, 0x7421, 0xca29, 
0x740b, 0xc9fc, 0x73f6, 0xc9ce, 0x73e1, 0xc9a1, 0x73cb, 0xc973, 
0x73b6, 0xc946, 0x73a0, 0xc918, 0x738b, 0xc8eb, 0x7375, 0xc8be, 
0x735f, 0xc890, 0x734a, 0xc863, 0x7334, 0xc836, 0x731e, 0xc809, 
0x7308, 0xc7db, 0x72f2, 0xc7ae, 0x72dc, 0xc781, 0x72c5, 0xc754, 
0x72af, 0xc727, 0x7299, 0xc6fa, 0x7282, 0xc6cd, 0x726c, 0xc6a0, 
0x7255, 0xc673, 0x723f, 0xc646, 0x7228, 0xc619, 0x7211, 0xc5ed, 
0x71fa, 0xc5c0, 0x71e3, 0xc593, 0x71cc, 0xc566, 0x71b5, 0xc53a, 
0x719e, 0xc50d, 0x7187, 0xc4e0, 0x7170, 0xc4b4, 0x7158, 0xc487, 
0x7141, 0xc45b, 0x712a, 0xc42e, 0x7112, 0xc402, 0x70fa, 0xc3d6, 
0x70e3, 0xc3a9, 0x70cb, 0xc37d, 0x70b3, 0xc351, 0x709b, 0xc324, 
0x7083, 0xc2f8, 0x706b, 0xc2cc, 0x7053, 0xc2a0, 0x703b, 0xc274, 
0x7023, 0xc248, 0x700b, 0xc21c, 0x6ff2, 0xc1f0, 0x6fda, 0xc1c4, 
0x6fc2, 0xc198, 0x6fa9, 0xc16c, 0x6f90, 0xc140, 0x6f78, 0xc114, 
0x6f5f, 0xc0e9, 0x6f46, 0xc0bd, 0x6f2d, 0xc091, 0x6f14, 0xc066, 
0x6efb, 0xc03a, 0x6ee2, 0xc00f, 0x6ec9, 0xbfe3, 0x6eb0, 0xbfb8, 
0x6e97, 0xbf8c, 0x6e7d, 0xbf61, 0x6e64, 0xbf35, 0x6e4a, 0xbf0a, 
0x6e31, 0xbedf, 0x6e17, 0xbeb3, 0x6dfe, 0xbe88, 0x6de4, 0xbe5d, 
0x6dca, 0xbe32, 0x6db0, 0xbe07, 0x6d96, 0xbddc, 0x6d7c, 0xbdb1, 
0x6d62, 0xbd86, 0x6d48, 0xbd5b, 0x6d2e, 0xbd30, 0x6d14, 0xbd05, 
0x6cf9, 0xbcda, 0x6cdf, 0xbcaf, 0x6cc4, 0xbc85, 0x6caa, 0xbc5a, 
0x6c8f, 0xbc2f, 0x6c75, 0xbc05, 0x6c5a, 0xbbda, 0x6c3f, 0xbbb0, 
0x6c24, 0xbb85, 0x6c09, 0xbb5b, 0x6bee, 0xbb30, 0x6bd3, 0xbb06, 
0x6bb8, 0xbadc, 0x6b9d, 0xbab1, 0x6b82, 0xba87, 0x6b66, 0xba5d, 
0x6b4b, 0xba33, 0x6b30, 0xba09, 0x6b14, 0xb9df, 0x6af8, 0xb9b5, 
0x6add, 0xb98b, 0x6ac1, 0xb961, 0x6aa5, 0xb937, 0x6a89, 0xb90d, 
0x6a6e, 0xb8e3, 0x6a52, 0xb8b9, 0x6a36, 0xb890, 0x6a1a, 0xb866, 
0x69fd, 0xb83c, 0x69e1, 0xb813, 0x69c5, 0xb7e9, 0x69a9, 0xb7c0, 
0x698c, 0xb796, 0x6970, 0xb76d, 0x6953, 0xb743, 0x6937, 0xb71a, 
0x691a, 0xb6f1, 0x68fd, 0xb6c7, 0x68e0, 0xb69e, 0x68c4, 0xb675, 
0x68a7, 0xb64c, 0x688a, 0xb623, 0x686d, 0xb5fa, 0x6850, 0xb5d1, 
0x6832, 0xb5a8, 0x6815, 0xb57f, 0x67f8, 0xb556, 0x67da, 0xb52d, 
0x67bd, 0xb505, 0x67a0, 0xb4dc, 0x6782, 0xb4b3, 0x6764, 0xb48b, 
0x6747, 0xb462, 0x6729, 0xb439, 0x670b, 0xb411, 0x66ed, 0xb3e9, 
0x66d0, 0xb3c0, 0x66b2, 0xb398, 0x6693, 0xb36f, 0x6675, 0xb347, 
0x6657, 0xb31f, 0x6639, 0xb2f7, 0x661b, 0xb2cf, 0x65fc, 0xb2a7, 
0x65de, 0xb27f, 0x65c0, 0xb257, 0x65a1, 0xb22f, 0x6582, 0xb207, 
0x6564, 0xb1df, 0x6545, 0xb1b7, 0x6526, 0xb18f, 0x6507, 0xb168, 
0x64e9, 0xb140, 0x64ca, 0xb118, 0x64ab, 0xb0f1, 0x648b, 0xb0c9, 
0x646c, 0xb0a2, 0x644d, 0xb07b, 0x642e, 0xb053, 0x640f, 0xb02c, 
0x63ef, 0xb005, 0x63d0, 0xafdd, 0x63b0, 0xafb6, 0x6391, 0xaf8f, 
0x6371, 0xaf68, 0x6351, 0xaf41, 0x6332, 0xaf1a, 0x6312, 0xaef3, 
0x62f2, 0xaecc, 0x62d2, 0xaea5, 0x62b2, 0xae7f, 0x6292, 0xae58, 
0x6272, 0xae31, 0x6252, 0xae0b, 0x6232, 0xade4, 0x6211, 0xadbd, 
0x61f1, 0xad97, 0x61d1, 0xad70, 0x61b0, 0xad4a, 0x6190, 0xad24, 
0x616f, 0xacfd, 0x614e, 0xacd7, 0x612e, 0xacb1, 0x610d, 0xac8b, 
0x60ec, 0xac65, 0x60cb, 0xac3f, 0x60aa, 0xac19, 0x6089, 0xabf3, 
0x6068, 0xabcd, 0x6047, 0xaba7, 0x6026, 0xab81, 0x6005, 0xab5c, 
0x5fe4, 0xab36, 0x5fc2, 0xab10, 0x5fa1, 0xaaeb, 0x5f80, 0xaac5, 
0x5f5e, 0xaaa0, 0x5f3c, 0xaa7a, 0x5f1b, 0xaa55, 0x5ef9, 0xaa30, 
0x5ed7, 0xaa0a, 0x5eb6, 0xa9e5, 0x5e94, 0xa9c0, 0x5e72, 0xa99b, 
0x5e50, 0xa976, 0x5e2e, 0xa951, 0x5e0c, 0xa92c, 0x5dea, 0xa907, 
0x5dc8, 0xa8e2, 0x5da5, 0xa8bd, 0x5d83, 0xa899, 0x5d61, 0xa874, 
0x5d3e, 0xa84f, 0x5d1c, 0xa82b, 0x5cf9, 0xa806, 0x5cd7, 0xa7e2, 
0x5cb4, 0xa7bd, 0x5c91, 0xa799, 0x5c6f, 0xa774, 0x5c4c, 0xa750, 
0x5c29, 0xa72c, 0x5c06, 0xa708, 0x5be3, 0xa6e4, 0x5bc0, 0xa6c0, 
0x5b9d, 0xa69c, 0x5b7a, 0xa678, 0x5b57, 0xa654, 0x5b34, 0xa630, 
0x5b10, 0xa60c, 0x5aed, 0xa5e8, 0x5ac9, 0xa5c5, 0x5aa6, 0xa5a1, 
0x5a82, 0xa57e, 0x5a5f, 0xa55a, 0x5a3b, 0xa537, 0x5a18, 0xa513, 
0x59f4, 0xa4f0, 0x59d0, 0xa4cc, 0x59ac, 0xa4a9, 0x5988, 0xa486, 
0x5964, 0xa463, 0x5940, 0xa440, 0x591c, 0xa41d, 0x58f8, 0xa3fa, 
0x58d4, 0xa3d7, 0x58b0, 0xa3b4, 0x588c, 0xa391, 0x5867, 0xa36f, 
0x5843, 0xa34c, 0x581e, 0xa329, 0x57fa, 0xa307, 0x57d5, 0xa2e4, 
0x57b1, 0xa2c2, 0x578c, 0xa29f, 0x5767, 0xa27d, 0x5743, 0xa25b, 
0x571e, 0xa238, 0x56f9, 0xa216, 0x56d4, 0xa1f4, 0x56af, 0xa1d2, 
0x568a, 0xa1b0, 0x5665, 0xa18e, 0x5640, 0xa16c, 0x561b, 0xa14a, 
0x55f6, 0xa129, 0x55d0, 0xa107, 0x55ab, 0xa0e5, 0x5586, 0xa0c4, 
0x5560, 0xa0a2, 0x553b, 0xa080, 0x5515, 0xa05f, 0x54f0, 0xa03e, 
0x54ca, 0xa01c, 0x54a4, 0x9ffb, 0x547f, 0x9fda, 0x5459, 0x9fb9, 
0x5433, 0x9f98, 0x540d, 0x9f77, 0x53e7, 0x9f56, 0x53c1, 0x9f35, 
0x539b, 0x9f14, 0x5375, 0x9ef3, 0x534f, 0x9ed2, 0x5329, 0x9eb2, 
0x5303, 0x9e91, 0x52dc, 0x9e70, 0x52b6, 0x9e50, 0x5290, 0x9e2f, 
0x5269, 0x9e0f, 0x5243, 0x9def, 0x521c, 0x9dce, 0x51f5, 0x9dae, 
0x51cf, 0x9d8e, 0x51a8, 0x9d6e, 0x5181, 0x9d4e, 0x515b, 0x9d2e, 
0x5134, 0x9d0e, 0x510d, 0x9cee, 0x50e6, 0x9cce, 0x50bf, 0x9caf, 
0x5098, 0x9c8f, 0x5071, 0x9c6f, 0x504a, 0x9c50, 0x5023, 0x9c30, 
0x4ffb, 0x9c11, 0x4fd4, 0x9bf1, 0x4fad, 0x9bd2, 0x4f85, 0x9bb3, 
0x4f5e, 0x9b94, 0x4f37, 0x9b75, 0x4f0f, 0x9b55, 0x4ee8, 0x9b36, 
0x4ec0, 0x9b17, 0x4e98, 0x9af9, 0x4e71, 0x9ada, 0x4e49, 0x9abb, 
0x4e21, 0x9a9c, 0x4df9, 0x9a7e, 0x4dd1, 0x9a5f, 0x4da9, 0x9a40, 
0x4d81, 0x9a22, 0x4d59, 0x9a04, 0x4d31, 0x99e5, 0x4d09, 0x99c7, 
0x4ce1, 0x99a9, 0x4cb9, 0x998b, 0x4c91, 0x996d, 0x4c68, 0x994e, 
0x4c40, 0x9930, 0x4c17, 0x9913, 0x4bef, 0x98f5, 0x4bc7, 0x98d7, 
0x4b9e, 0x98b9, 0x4b75, 0x989c, 0x4b4d, 0x987e, 0x4b24, 0x9860, 
0x4afb, 0x9843, 0x4ad3, 0x9826, 0x4aaa, 0x9808, 0x4a81, 0x97eb, 
0x4a58, 0x97ce, 0x4a2f, 0x97b0, 0x4a06, 0x9793, 0x49dd, 0x9776, 
0x49b4, 0x9759, 0x498b, 0x973c, 0x4962, 0x9720, 0x4939, 0x9703, 
0x490f, 0x96e6, 0x48e6, 0x96c9, 0x48bd, 0x96ad, 0x4893, 0x9690, 
0x486a, 0x9674, 0x4840, 0x9657, 0x4817, 0x963b, 0x47ed, 0x961f, 
0x47c4, 0x9603, 0x479a, 0x95e6, 0x4770, 0x95ca, 0x4747, 0x95ae, 
0x471d, 0x9592, 0x46f3, 0x9577, 0x46c9, 0x955b, 0x469f, 0x953f, 
0x4675, 0x9523, 0x464b, 0x9508, 0x4621, 0x94ec, 0x45f7, 0x94d0, 
0x45cd, 0x94b5, 0x45a3, 0x949a, 0x4579, 0x947e, 0x454f, 0x9463, 
0x4524, 0x9448, 0x44fa, 0x942d, 0x44d0, 0x9412, 0x44a5, 0x93f7, 
0x447b, 0x93dc, 0x4450, 0x93c1, 0x4426, 0x93a6, 0x43fb, 0x938b, 
0x43d1, 0x9371, 0x43a6, 0x9356, 0x437b, 0x933c, 0x4351, 0x9321, 
0x4326, 0x9307, 0x42fb, 0x92ec, 0x42d0, 0x92d2, 0x42a5, 0x92b8, 
0x427a, 0x929e, 0x424f, 0x9284, 0x4224, 0x926a, 0x41f9, 0x9250, 
0x41ce, 0x9236, 0x41a3, 0x921c, 0x4178, 0x9202, 0x414d, 0x91e9, 
0x4121, 0x91cf, 0x40f6, 0x91b6, 0x40cb, 0x919c, 0x409f, 0x9183, 
0x4074, 0x9169, 0x4048, 0x9150, 0x401d, 0x9137, 0x3ff1, 0x911e, 
0x3fc6, 0x9105, 0x3f9a, 0x90ec, 0x3f6f, 0x90d3, 0x3f43, 0x90ba, 
0x3f17, 0x90a1, 0x3eec, 0x9088, 0x3ec0, 0x9070, 0x3e94, 0x9057, 
0x3e68, 0x903e, 0x3e3c, 0x9026, 0x3e10, 0x900e, 0x3de4, 0x8ff5, 
0x3db8, 0x8fdd, 0x3d8c, 0x8fc5, 0x3d60, 0x8fad, 0x3d34, 0x8f95, 
0x3d08, 0x8f7d, 0x3cdc, 0x8f65, 0x3caf, 0x8f4d, 0x3c83, 0x8f35, 
0x3c57, 0x8f1d, 0x3c2a, 0x8f06, 0x3bfe, 0x8eee, 0x3bd2, 0x8ed6, 
0x3ba5, 0x8ebf, 0x3b79, 0x8ea8, 0x3b4c, 0x8e90, 0x3b20, 0x8e79, 
0x3af3, 0x8e62, 0x3ac6, 0x8e4b, 0x3a9a, 0x8e34, 0x3a6d, 0x8e1d, 
0x3a40, 0x8e06, 0x3a13, 0x8def, 0x39e7, 0x8dd8, 0x39ba, 0x8dc1, 
0x398d, 0x8dab, 0x3960, 0x8d94, 0x3933, 0x8d7e, 0x3906, 0x8d67, 
0x38d9, 0x8d51, 0x38ac, 0x8d3b, 0x387f, 0x8d24, 0x3852, 0x8d0e, 
0x3825, 0x8cf8, 0x37f7, 0x8ce2, 0x37ca, 0x8ccc, 0x379d, 0x8cb6, 
0x3770, 0x8ca1, 0x3742, 0x8c8b, 0x3715, 0x8c75, 0x36e8, 0x8c60, 
0x36ba, 0x8c4a, 0x368d, 0x8c35, 0x365f, 0x8c1f, 0x3632, 0x8c0a, 
0x3604, 0x8bf5, 0x35d7, 0x8bdf, 0x35a9, 0x8bca, 0x357b, 0x8bb5, 
0x354e, 0x8ba0, 0x3520, 0x8b8b, 0x34f2, 0x8b77, 0x34c4, 0x8b62, 
0x3497, 0x8b4d, 0x3469, 0x8b39, 0x343b, 0x8b24, 0x340d, 0x8b10, 
0x33df, 0x8afb, 0x33b1, 0x8ae7, 0x3383, 0x8ad3, 0x3355, 0x8abe, 
0x3327, 0x8aaa, 0x32f9, 0x8a96, 0x32cb, 0x8a82, 0x329d, 0x8a6e, 
0x326e, 0x8a5a, 0x3240, 0x8a47, 0x3212, 0x8a33, 0x31e4, 0x8a1f, 
0x31b5, 0x8a0c, 0x3187, 0x89f8, 0x3159, 0x89e5, 0x312a, 0x89d2, 
0x30fc, 0x89be, 0x30cd, 0x89ab, 0x309f, 0x8998, 0x3070, 0x8985, 
0x3042, 0x8972, 0x3013, 0x895f, 0x2fe5, 0x894c, 0x2fb6, 0x8939, 
0x2f87, 0x8927, 0x2f59, 0x8914, 0x2f2a, 0x8902, 0x2efb, 0x88ef, 
0x2ecc, 0x88dd, 0x2e9e, 0x88ca, 0x2e6f, 0x88b8, 0x2e40, 0x88a6, 
0x2e11, 0x8894, 0x2de2, 0x8882, 0x2db3, 0x8870, 0x2d84, 0x885e, 
0x2d55, 0x884c, 0x2d26, 0x883a, 0x2cf7, 0x8828, 0x2cc8, 0x8817, 
0x2c99, 0x8805, 0x2c6a, 0x87f4, 0x2c3b, 0x87e2, 0x2c0c, 0x87d1, 
0x2bdc, 0x87c0, 0x2bad, 0x87af, 0x2b7e, 0x879d, 0x2b4f, 0x878c, 
0x2b1f, 0x877b, 0x2af0, 0x876b, 0x2ac1, 0x875a, 0x2a91, 0x8749, 
0x2a62, 0x8738, 0x2a32, 0x8728, 0x2a03, 0x8717, 0x29d3, 0x8707, 
0x29a4, 0x86f6, 0x2974, 0x86e6, 0x2945, 0x86d6, 0x2915, 0x86c6, 
0x28e5, 0x86b6, 0x28b6, 0x86a5, 0x2886, 0x8696, 0x2856, 0x8686, 
0x2827, 0x8676, 0x27f7, 0x8666, 0x27c7, 0x8656, 0x2797, 0x8647, 
0x2768, 0x8637, 0x2738, 0x8628, 0x2708, 0x8619, 0x26d8, 0x8609, 
0x26a8, 0x85fa, 0x2678, 0x85eb, 0x2648, 0x85dc, 0x2618, 0x85cd, 
0x25e8, 0x85be, 0x25b8, 0x85af, 0x2588, 0x85a0, 0x2558, 0x8592, 
0x2528, 0x8583, 0x24f8, 0x8574, 0x24c8, 0x8566, 0x2498, 0x8558, 
0x2467, 0x8549, 0x2437, 0x853b, 0x2407, 0x852d, 0x23d7, 0x851f, 
0x23a7, 0x8511, 0x2376, 0x8503, 0x2346, 0x84f5, 0x2316, 0x84e7, 
0x22e5, 0x84d9, 0x22b5, 0x84cc, 0x2284, 0x84be, 0x2254, 0x84b0, 
0x2224, 0x84a3, 0x21f3, 0x8496, 0x21c3, 0x8488, 0x2192, 0x847b, 
0x2162, 0x846e, 0x2131, 0x8461, 0x2101, 0x8454, 0x20d0, 0x8447, 
0x209f, 0x843a, 0x206f, 0x842d, 0x203e, 0x8421, 0x200e, 0x8414, 
0x1fdd, 0x8407, 0x1fac, 0x83fb, 0x1f7b, 0x83ef, 0x1f4b, 0x83e2, 
0x1f1a, 0x83d6, 0x1ee9, 0x83ca, 0x1eb8, 0x83be, 0x1e88, 0x83b2, 
0x1e57, 0x83a6, 0x1e26, 0x839a, 0x1df5, 0x838e, 0x1dc4, 0x8382, 
0x1d93, 0x8377, 0x1d62, 0x836b, 0x1d31, 0x8360, 0x1d01, 0x8354, 
0x1cd0, 0x8349, 0x1c9f, 0x833e, 0x1c6e, 0x8332, 0x1c3d, 0x8327, 
0x1c0c, 0x831c, 0x1bda, 0x8311, 0x1ba9, 0x8306, 0x1b78, 0x82fb, 
0x1b47, 0x82f1, 0x1b16, 0x82e6, 0x1ae5, 0x82db, 0x1ab4, 0x82d1, 
0x1a83, 0x82c6, 0x1a51, 0x82bc, 0x1a20, 0x82b2, 0x19ef, 0x82a8, 
0x19be, 0x829d, 0x198d, 0x8293, 0x195b, 0x8289, 0x192a, 0x827f, 
0x18f9, 0x8276, 0x18c7, 0x826c, 0x1896, 0x8262, 0x1865, 0x8259, 
0x1833, 0x824f, 0x1802, 0x8246, 0x17d1, 0x823c, 0x179f, 0x8233, 
0x176e, 0x822a, 0x173c, 0x8220, 0x170b, 0x8217, 0x16da, 0x820e, 
0x16a8, 0x8205, 0x1677, 0x81fd, 0x1645, 0x81f4, 0x1614, 0x81eb, 
0x15e2, 0x81e2, 0x15b1, 0x81da, 0x157f, 0x81d1, 0x154d, 0x81c9, 
0x151c, 0x81c1, 0x14ea, 0x81b8, 0x14b9, 0x81b0, 0x1487, 0x81a8, 
0x1455, 0x81a0, 0x1424, 0x8198, 0x13f2, 0x8190, 0x13c1, 0x8188, 
0x138f, 0x8181, 0x135d, 0x8179, 0x132b, 0x8172, 0x12fa, 0x816a, 
0x12c8, 0x8163, 0x1296, 0x815b, 0x1265, 0x8154, 0x1233, 0x814d, 
0x1201, 0x8146, 0x11cf, 0x813f, 0x119e, 0x8138, 0x116c, 0x8131, 
0x113a, 0x812a, 0x1108, 0x8123, 0x10d6, 0x811d, 0x10a4, 0x8116, 
0x1073, 0x8110, 0x1041, 0x8109, 0x100f, 0x8103, 0x0fdd, 0x80fd, 
0x0fab, 0x80f6, 0x0f79, 0x80f0, 0x0f47, 0x80ea, 0x0f15, 0x80e4, 
0x0ee4, 0x80de, 0x0eb2, 0x80d9, 0x0e80, 0x80d3, 0x0e4e, 0x80cd, 
0x0e1c, 0x80c8, 0x0dea, 0x80c2, 0x0db8, 0x80bd, 0x0d86, 0x80b7, 
0x0d54, 0x80b2, 0x0d22, 0x80ad, 0x0cf0, 0x80a8, 0x0cbe, 0x80a3, 
0x0c8c, 0x809e, 0x0c5a, 0x8099, 0x0c28, 0x8094, 0x0bf6, 0x808f, 
0x0bc4, 0x808b, 0x0b92, 0x8086, 0x0b60, 0x8082, 0x0b2d, 0x807d, 
0x0afb, 0x8079, 0x0ac9, 0x8075, 0x0a97, 0x8070, 0x0a65, 0x806c, 
0x0a33, 0x8068, 0x0a01, 0x8064, 0x09cf, 0x8060, 0x099d, 0x805d, 
0x096b, 0x8059, 0x0938, 0x8055, 0x0906, 0x8052, 0x08d4, 0x804e, 
0x08a2, 0x804b, 0x0870, 0x8047, 0x083e, 0x8044, 0x080c, 0x8041, 
0x07d9, 0x803e, 0x07a7, 0x803b, 0x0775, 0x8038, 0x0743, 0x8035, 
0x0711, 0x8032, 0x06de, 0x802f, 0x06ac, 0x802d, 0x067a, 0x802a, 
0x0648, 0x8027, 0x0616, 0x8025, 0x05e3, 0x8023, 0x05b1, 0x8020, 
0x057f, 0x801e, 0x054d, 0x801c, 0x051b, 0x801a, 0x04e8, 0x8018, 
0x04b6, 0x8016, 0x0484, 0x8014, 0x0452, 0x8013, 0x041f, 0x8011, 
0x03ed, 0x800f, 0x03bb, 0x800e, 0x0389, 0x800c, 0x0356, 0x800b, 
0x0324, 0x800a, 0x02f2, 0x8009, 0x02c0, 0x8008, 0x028d, 0x8007, 
0x025b, 0x8006, 0x0229, 0x8005, 0x01f7, 0x8004, 0x01c4, 0x8003, 
0x0192, 0x8002, 0x0160, 0x8002, 0x012e, 0x8001, 0x00fb, 0x8001, 
0x00c9, 0x8001, 0x0097, 0x8000, 0x0065, 0x8000, 0x0032, 0x8000, 
0x0000, 0x8000, 0xffce, 0x8000, 0xff9b, 0x8000, 0xff69, 0x8000, 
0xff37, 0x8001, 0xff05, 0x8001, 0xfed2, 0x8001, 0xfea0, 0x8002, 
0xfe6e, 0x8002, 0xfe3c, 0x8003, 0xfe09, 0x8004, 0xfdd7, 0x8005, 
0xfda5, 0x8006, 0xfd73, 0x8007, 0xfd40, 0x8008, 0xfd0e, 0x8009, 
0xfcdc, 0x800a, 0xfcaa, 0x800b, 0xfc77, 0x800c, 0xfc45, 0x800e, 
0xfc13, 0x800f, 0xfbe1, 0x8011, 0xfbae, 0x8013, 0xfb7c, 0x8014, 
0xfb4a, 0x8016, 0xfb18, 0x8018, 0xfae5, 0x801a, 0xfab3, 0x801c, 
0xfa81, 0x801e, 0xfa4f, 0x8020, 0xfa1d, 0x8023, 0xf9ea, 0x8025, 
0xf9b8, 0x8027, 0xf986, 0x802a, 0xf954, 0x802d, 0xf922, 0x802f, 
0xf8ef, 0x8032, 0xf8bd, 0x8035, 0xf88b, 0x8038, 0xf859, 0x803b, 
0xf827, 0x803e, 0xf7f4, 0x8041, 0xf7c2, 0x8044, 0xf790, 0x8047, 
0xf75e, 0x804b, 0xf72c, 0x804e, 0xf6fa, 0x8052, 0xf6c8, 0x8055, 
0xf695, 0x8059, 0xf663, 0x805d, 0xf631, 0x8060, 0xf5ff, 0x8064, 
0xf5cd, 0x8068, 0xf59b, 0x806c, 0xf569, 0x8070, 0xf537, 0x8075, 
0xf505, 0x8079, 0xf4d3, 0x807d, 0xf4a0, 0x8082, 0xf46e, 0x8086, 
0xf43c, 0x808b, 0xf40a, 0x808f, 0xf3d8, 0x8094, 0xf3a6, 0x8099, 
0xf374, 0x809e, 0xf342, 0x80a3, 0xf310, 0x80a8, 0xf2de, 0x80ad, 
0xf2ac, 0x80b2, 0xf27a, 0x80b7, 0xf248, 0x80bd, 0xf216, 0x80c2, 
0xf1e4, 0x80c8, 0xf1b2, 0x80cd, 0xf180, 0x80d3, 0xf14e, 0x80d9, 
0xf11c, 0x80de, 0xf0eb, 0x80e4, 0xf0b9, 0x80ea, 0xf087, 0x80f0, 
0xf055, 0x80f6, 0xf023, 0x80fd, 0xeff1, 0x8103, 0xefbf, 0x8109, 
0xef8d, 0x8110, 0xef5c, 0x8116, 0xef2a, 0x811d, 0xeef8, 0x8123, 
0xeec6, 0x812a, 0xee94, 0x8131, 0xee62, 0x8138, 0xee31, 0x813f, 
0xedff, 0x8146, 0xedcd, 0x814d, 0xed9b, 0x8154, 0xed6a, 0x815b, 
0xed38, 0x8163, 0xed06, 0x816a, 0xecd5, 0x8172, 0xeca3, 0x8179, 
0xec71, 0x8181, 0xec3f, 0x8188, 0xec0e, 0x8190, 0xebdc, 0x8198, 
0xebab, 0x81a0, 0xeb79, 0x81a8, 0xeb47, 0x81b0, 0xeb16, 0x81b8, 
0xeae4, 0x81c1, 0xeab3, 0x81c9, 0xea81, 0x81d1, 0xea4f, 0x81da, 
0xea1e, 0x81e2, 0xe9ec, 0x81eb, 0xe9bb, 0x81f4, 0xe989, 0x81fd, 
0xe958, 0x8205, 0xe926, 0x820e, 0xe8f5, 0x8217, 0xe8c4, 0x8220, 
0xe892, 0x822a, 0xe861, 0x8233, 0xe82f, 0x823c, 0xe7fe, 0x8246, 
0xe7cd, 0x824f, 0xe79b, 0x8259, 0xe76a, 0x8262, 0xe739, 0x826c, 
0xe707, 0x8276, 0xe6d6, 0x827f, 0xe6a5, 0x8289, 0xe673, 0x8293, 
0xe642, 0x829d, 0xe611, 0x82a8, 0xe5e0, 0x82b2, 0xe5af, 0x82bc, 
0xe57d, 0x82c6, 0xe54c, 0x82d1, 0xe51b, 0x82db, 0xe4ea, 0x82e6, 
0xe4b9, 0x82f1, 0xe488, 0x82fb, 0xe457, 0x8306, 0xe426, 0x8311, 
0xe3f4, 0x831c, 0xe3c3, 0x8327, 0xe392, 0x8332, 0xe361, 0x833e, 
0xe330, 0x8349, 0xe2ff, 0x8354, 0xe2cf, 0x8360, 0xe29e, 0x836b, 
0xe26d, 0x8377, 0xe23c, 0x8382, 0xe20b, 0x838e, 0xe1da, 0x839a, 
0xe1a9, 0x83a6, 0xe178, 0x83b2, 0xe148, 0x83be, 0xe117, 0x83ca, 
0xe0e6, 0x83d6, 0xe0b5, 0x83e2, 0xe085, 0x83ef, 0xe054, 0x83fb, 
0xe023, 0x8407, 0xdff2, 0x8414, 0xdfc2, 0x8421, 0xdf91, 0x842d, 
0xdf61, 0x843a, 0xdf30, 0x8447, 0xdeff, 0x8454, 0xdecf, 0x8461, 
0xde9e, 0x846e, 0xde6e, 0x847b, 0xde3d, 0x8488, 0xde0d, 0x8496, 
0xdddc, 0x84a3, 0xddac, 0x84b0, 0xdd7c, 0x84be, 0xdd4b, 0x84cc, 
0xdd1b, 0x84d9, 0xdcea, 0x84e7, 0xdcba, 0x84f5, 0xdc8a, 0x8503, 
0xdc59, 0x8511, 0xdc29, 0x851f, 0xdbf9, 0x852d, 0xdbc9, 0x853b, 
0xdb99, 0x8549, 0xdb68, 0x8558, 0xdb38, 0x8566, 0xdb08, 0x8574, 
0xdad8, 0x8583, 0xdaa8, 0x8592, 0xda78, 0x85a0, 0xda48, 0x85af, 
0xda18, 0x85be, 0xd9e8, 0x85cd, 0xd9b8, 0x85dc, 0xd988, 0x85eb, 
0xd958, 0x85fa, 0xd928, 0x8609, 0xd8f8, 0x8619, 0xd8c8, 0x8628, 
0xd898, 0x8637, 0xd869, 0x8647, 0xd839, 0x8656, 0xd809, 0x8666, 
0xd7d9, 0x8676, 0xd7aa, 0x8686, 0xd77a, 0x8696, 0xd74a, 0x86a5, 
0xd71b, 0x86b6, 0xd6eb, 0x86c6, 0xd6bb, 0x86d6, 0xd68c, 0x86e6, 
0xd65c, 0x86f6, 0xd62d, 0x8707, 0xd5fd, 0x8717, 0xd5ce, 0x8728, 
0xd59e, 0x8738, 0xd56f, 0x8749, 0xd53f, 0x875a, 0xd510, 0x876b, 
0xd4e1, 0x877b, 0xd4b1, 0x878c, 0xd482, 0x879d, 0xd453, 0x87af, 
0xd424, 0x87c0, 0xd3f4, 0x87d1, 0xd3c5, 0x87e2, 0xd396, 0x87f4, 
0xd367, 0x8805, 0xd338, 0x8817, 0xd309, 0x8828, 0xd2da, 0x883a, 
0xd2ab, 0x884c, 0xd27c, 0x885e, 0xd24d, 0x8870, 0xd21e, 0x8882, 
0xd1ef, 0x8894, 0xd1c0, 0x88a6, 0xd191, 0x88b8, 0xd162, 0x88ca, 
0xd134, 0x88dd, 0xd105, 0x88ef, 0xd0d6, 0x8902, 0xd0a7, 0x8914, 
0xd079, 0x8927, 0xd04a, 0x8939, 0xd01b, 0x894c, 0xcfed, 0x895f, 
0xcfbe, 0x8972, 0xcf90, 0x8985, 0xcf61, 0x8998, 0xcf33, 0x89ab, 
0xcf04, 0x89be, 0xced6, 0x89d2, 0xcea7, 0x89e5, 0xce79, 0x89f8, 
0xce4b, 0x8a0c, 0xce1c, 0x8a1f, 0xcdee, 0x8a33, 0xcdc0, 0x8a47, 
0xcd92, 0x8a5a, 0xcd63, 0x8a6e, 0xcd35, 0x8a82, 0xcd07, 0x8a96, 
0xccd9, 0x8aaa, 0xccab, 0x8abe, 0xcc7d, 0x8ad3, 0xcc4f, 0x8ae7, 
0xcc21, 0x8afb, 0xcbf3, 0x8b10, 0xcbc5, 0x8b24, 0xcb97, 0x8b39, 
0xcb69, 0x8b4d, 0xcb3c, 0x8b62, 0xcb0e, 0x8b77, 0xcae0, 0x8b8b, 
0xcab2, 0x8ba0, 0xca85, 0x8bb5, 0xca57, 0x8bca, 0xca29, 0x8bdf, 
0xc9fc, 0x8bf5, 0xc9ce, 0x8c0a, 0xc9a1, 0x8c1f, 0xc973, 0x8c35, 
0xc946, 0x8c4a, 0xc918, 0x8c60, 0xc8eb, 0x8c75, 0xc8be, 0x8c8b, 
0xc890, 0x8ca1, 0xc863, 0x8cb6, 0xc836, 0x8ccc, 0xc809, 0x8ce2, 
0xc7db, 0x8cf8, 0xc7ae, 0x8d0e, 0xc781, 0x8d24, 0xc754, 0x8d3b, 
0xc727, 0x8d51, 0xc6fa, 0x8d67, 0xc6cd, 0x8d7e, 0xc6a0, 0x8d94, 
0xc673, 0x8dab, 0xc646, 0x8dc1, 0xc619, 0x8dd8, 0xc5ed, 0x8def, 
0xc5c0, 0x8e06, 0xc593, 0x8e1d, 0xc566, 0x8e34, 0xc53a, 0x8e4b, 
0xc50d, 0x8e62, 0xc4e0, 0x8e79, 0xc4b4, 0x8e90, 0xc487, 0x8ea8, 
0xc45b, 0x8ebf, 0xc42e, 0x8ed6, 0xc402, 0x8eee, 0xc3d6, 0x8f06, 
0xc3a9, 0x8f1d, 0xc37d, 0x8f35, 0xc351, 0x8f4d, 0xc324, 0x8f65, 
0xc2f8, 0x8f7d, 0xc2cc, 0x8f95, 0xc2a0, 0x8fad, 0xc274, 0x8fc5, 
0xc248, 0x8fdd, 0xc21c, 0x8ff5, 0xc1f0, 0x900e, 0xc1c4, 0x9026, 
0xc198, 0x903e, 0xc16c, 0x9057, 0xc140, 0x9070, 0xc114, 0x9088, 
0xc0e9, 0x90a1, 0xc0bd, 0x90ba, 0xc091, 0x90d3, 0xc066, 0x90ec, 
0xc03a, 0x9105, 0xc00f, 0x911e, 0xbfe3, 0x9137, 0xbfb8, 0x9150, 
0xbf8c, 0x9169, 0xbf61, 0x9183, 0xbf35, 0x919c, 0xbf0a, 0x91b6, 
0xbedf, 0x91cf, 0xbeb3, 0x91e9, 0xbe88, 0x9202, 0xbe5d, 0x921c, 
0xbe32, 0x9236, 0xbe07, 0x9250, 0xbddc, 0x926a, 0xbdb1, 0x9284, 
0xbd86, 0x929e, 0xbd5b, 0x92b8, 0xbd30, 0x92d2, 0xbd05, 0x92ec, 
0xbcda, 0x9307, 0xbcaf, 0x9321, 0xbc85, 0x933c, 0xbc5a, 0x9356, 
0xbc2f, 0x9371, 0xbc05, 0x938b, 0xbbda, 0x93a6, 0xbbb0, 0x93c1, 
0xbb85, 0x93dc, 0xbb5b, 0x93f7, 0xbb30, 0x9412, 0xbb06, 0x942d, 
0xbadc, 0x9448, 0xbab1, 0x9463, 0xba87, 0x947e, 0xba5d, 0x949a, 
0xba33, 0x94b5, 0xba09, 0x94d0, 0xb9df, 0x94ec, 0xb9b5, 0x9508, 
0xb98b, 0x9523, 0xb961, 0x953f, 0xb937, 0x955b, 0xb90d, 0x9577, 
0xb8e3, 0x9592, 0xb8b9, 0x95ae, 0xb890, 0x95ca, 0xb866, 0x95e6, 
0xb83c, 0x9603, 0xb813, 0x961f, 0xb7e9, 0x963b, 0xb7c0, 0x9657, 
0xb796, 0x9674, 0xb76d, 0x9690, 0xb743, 0x96ad, 0xb71a, 0x96c9, 
0xb6f1, 0x96e6, 0xb6c7, 0x9703, 0xb69e, 0x9720, 0xb675, 0x973c, 
0xb64c, 0x9759, 0xb623, 0x9776, 0xb5fa, 0x9793, 0xb5d1, 0x97b0, 
0xb5a8, 0x97ce, 0xb57f, 0x97eb, 0xb556, 0x9808, 0xb52d, 0x9826, 
0xb505, 0x9843, 0xb4dc, 0x9860, 0xb4b3, 0x987e, 0xb48b, 0x989c, 
0xb462, 0x98b9, 0xb439, 0x98d7, 0xb411, 0x98f5, 0xb3e9, 0x9913, 
0xb3c0, 0x9930, 0xb398, 0x994e, 0xb36f, 0x996d, 0xb347, 0x998b, 
0xb31f, 0x99a9, 0xb2f7, 0x99c7, 0xb2cf, 0x99e5, 0xb2a7, 0x9a04, 
0xb27f, 0x9a22, 0xb257, 0x9a40, 0xb22f, 0x9a5f, 0xb207, 0x9a7e, 
0xb1df, 0x9a9c, 0xb1b7, 0x9abb, 0xb18f, 0x9ada, 0xb168, 0x9af9, 
0xb140, 0x9b17, 0xb118, 0x9b36, 0xb0f1, 0x9b55, 0xb0c9, 0x9b75, 
0xb0a2, 0x9b94, 0xb07b, 0x9bb3, 0xb053, 0x9bd2, 0xb02c, 0x9bf1, 
0xb005, 0x9c11, 0xafdd, 0x9c30, 0xafb6, 0x9c50, 0xaf8f, 0x9c6f, 
0xaf68, 0x9c8f, 0xaf41, 0x9caf, 0xaf1a, 0x9cce, 0xaef3, 0x9cee, 
0xaecc, 0x9d0e, 0xaea5, 0x9d2e, 0xae7f, 0x9d4e, 0xae58, 0x9d6e, 
0xae31, 0x9d8e, 0xae0b, 0x9dae, 0xade4, 0x9dce, 0xadbd, 0x9def, 
0xad97, 0x9e0f, 0xad70, 0x9e2f, 0xad4a, 0x9e50, 0xad24, 0x9e70, 
0xacfd, 0x9e91, 0xacd7, 0x9eb2, 0xacb1, 0x9ed2, 0xac8b, 0x9ef3, 
0xac65, 0x9f14, 0xac3f, 0x9f35, 0xac19, 0x9f56, 0xabf3, 0x9f77, 
0xabcd, 0x9f98, 0xaba7, 0x9fb9, 0xab81, 0x9fda, 0xab5c, 0x9ffb, 
0xab36, 0xa01c, 0xab10, 0xa03e, 0xaaeb, 0xa05f, 0xaac5, 0xa080, 
0xaaa0, 0xa0a2, 0xaa7a, 0xa0c4, 0xaa55, 0xa0e5, 0xaa30, 0xa107, 
0xaa0a, 0xa129, 0xa9e5, 0xa14a, 0xa9c0, 0xa16c, 0xa99b, 0xa18e, 
0xa976, 0xa1b0, 0xa951, 0xa1d2, 0xa92c, 0xa1f4, 0xa907, 0xa216, 
0xa8e2, 0xa238, 0xa8bd, 0xa25b, 0xa899, 0xa27d, 0xa874, 0xa29f, 
0xa84f, 0xa2c2, 0xa82b, 0xa2e4, 0xa806, 0xa307, 0xa7e2, 0xa329, 
0xa7bd, 0xa34c, 0xa799, 0xa36f, 0xa774, 0xa391, 0xa750, 0xa3b4, 
0xa72c, 0xa3d7, 0xa708, 0xa3fa, 0xa6e4, 0xa41d, 0xa6c0, 0xa440, 
0xa69c, 0xa463, 0xa678, 0xa486, 0xa654, 0xa4a9, 0xa630, 0xa4cc, 
0xa60c, 0xa4f0, 0xa5e8, 0xa513, 0xa5c5, 0xa537, 0xa5a1, 0xa55a, 
0xa57e, 0xa57e, 0xa55a, 0xa5a1, 0xa537, 0xa5c5, 0xa513, 0xa5e8, 
0xa4f0, 0xa60c, 0xa4cc, 0xa630, 0xa4a9, 0xa654, 0xa486, 0xa678, 
0xa463, 0xa69c, 0xa440, 0xa6c0, 0xa41d, 0xa6e4, 0xa3fa, 0xa708, 
0xa3d7, 0xa72c, 0xa3b4, 0xa750, 0xa391, 0xa774, 0xa36f, 0xa799, 
0xa34c, 0xa7bd, 0xa329, 0xa7e2, 0xa307, 0xa806, 0xa2e4, 0xa82b, 
0xa2c2, 0xa84f, 0xa29f, 0xa874, 0xa27d, 0xa899, 0xa25b, 0xa8bd, 
0xa238, 0xa8e2, 0xa216, 0xa907, 0xa1f4, 0xa92c, 0xa1d2, 0xa951, 
0xa1b0, 0xa976, 0xa18e, 0xa99b, 0xa16c, 0xa9c0, 0xa14a, 0xa9e5, 
0xa129, 0xaa0a, 0xa107, 0xaa30, 0xa0e5, 0xaa55, 0xa0c4, 0xaa7a, 
0xa0a2, 0xaaa0, 0xa080, 0xaac5, 0xa05f, 0xaaeb, 0xa03e, 0xab10, 
0xa01c, 0xab36, 0x9ffb, 0xab5c, 0x9fda, 0xab81, 0x9fb9, 0xaba7, 
0x9f98, 0xabcd, 0x9f77, 0xabf3, 0x9f56, 0xac19, 0x9f35, 0xac3f, 
0x9f14, 0xac65, 0x9ef3, 0xac8b, 0x9ed2, 0xacb1, 0x9eb2, 0xacd7, 
0x9e91, 0xacfd, 0x9e70, 0xad24, 0x9e50, 0xad4a, 0x9e2f, 0xad70, 
0x9e0f, 0xad97, 0x9def, 0xadbd, 0x9dce, 0xade4, 0x9dae, 0xae0b, 
0x9d8e, 0xae31, 0x9d6e, 0xae58, 0x9d4e, 0xae7f, 0x9d2e, 0xaea5, 
0x9d0e, 0xaecc, 0x9cee, 0xaef3, 0x9cce, 0xaf1a, 0x9caf, 0xaf41, 
0x9c8f, 0xaf68, 0x9c6f, 0xaf8f, 0x9c50, 0xafb6, 0x9c30, 0xafdd, 
0x9c11, 0xb005, 0x9bf1, 0xb02c, 0x9bd2, 0xb053, 0x9bb3, 0xb07b, 
0x9b94, 0xb0a2, 0x9b75, 0xb0c9, 0x9b55, 0xb0f1, 0x9b36, 0xb118, 
0x9b17, 0xb140, 0x9af9, 0xb168, 0x9ada, 0xb18f, 0x9abb, 0xb1b7, 
0x9a9c, 0xb1df, 0x9a7e, 0xb207, 0x9a5f, 0xb22f, 0x9a40, 0xb257, 
0x9a22, 0xb27f, 0x9a04, 0xb2a7, 0x99e5, 0xb2cf, 0x99c7, 0xb2f7, 
0x99a9, 0xb31f, 0x998b, 0xb347, 0x996d, 0xb36f, 0x994e, 0xb398, 
0x9930, 0xb3c0, 0x9913, 0xb3e9, 0x98f5, 0xb411, 0x98d7, 0xb439, 
0x98b9, 0xb462, 0x989c, 0xb48b, 0x987e, 0xb4b3, 0x9860, 0xb4dc, 
0x9843, 0xb505, 0x9826, 0xb52d, 0x9808, 0xb556, 0x97eb, 0xb57f, 
0x97ce, 0xb5a8, 0x97b0, 0xb5d1, 0x9793, 0xb5fa, 0x9776, 0xb623, 
0x9759, 0xb64c, 0x973c, 0xb675, 0x9720, 0xb69e, 0x9703, 0xb6c7, 
0x96e6, 0xb6f1, 0x96c9, 0xb71a, 0x96ad, 0xb743, 0x9690, 0xb76d, 
0x9674, 0xb796, 0x9657, 0xb7c0, 0x963b, 0xb7e9, 0x961f, 0xb813, 
0x9603, 0xb83c, 0x95e6, 0xb866, 0x95ca, 0xb890, 0x95ae, 0xb8b9, 
0x9592, 0xb8e3, 0x9577, 0xb90d, 0x955b, 0xb937, 0x953f, 0xb961, 
0x9523, 0xb98b, 0x9508, 0xb9b5, 0x94ec, 0xb9df, 0x94d0, 0xba09, 
0x94b5, 0xba33, 0x949a, 0xba5d, 0x947e, 0xba87, 0x9463, 0xbab1, 
0x9448, 0xbadc, 0x942d, 0xbb06, 0x9412, 0xbb30, 0x93f7, 0xbb5b, 
0x93dc, 0xbb85, 0x93c1, 0xbbb0, 0x93a6, 0xbbda, 0x938b, 0xbc05, 
0x9371, 0xbc2f, 0x9356, 0xbc5a, 0x933c, 0xbc85, 0x9321, 0xbcaf, 
0x9307, 0xbcda, 0x92ec, 0xbd05, 0x92d2, 0xbd30, 0x92b8, 0xbd5b, 
0x929e, 0xbd86, 0x9284, 0xbdb1, 0x926a, 0xbddc, 0x9250, 0xbe07, 
0x9236, 0xbe32, 0x921c, 0xbe5d, 0x9202, 0xbe88, 0x91e9, 0xbeb3, 
0x91cf, 0xbedf, 0x91b6, 0xbf0a, 0x919c, 0xbf35, 0x9183, 0xbf61, 
0x9169, 0xbf8c, 0x9150, 0xbfb8, 0x9137, 0xbfe3, 0x911e, 0xc00f, 
0x9105, 0xc03a, 0x90ec, 0xc066, 0x90d3, 0xc091, 0x90ba, 0xc0bd, 
0x90a1, 0xc0e9, 0x9088, 0xc114, 0x9070, 0xc140, 0x9057, 0xc16c, 
0x903e, 0xc198, 0x9026, 0xc1c4, 0x900e, 0xc1f0, 0x8ff5, 0xc21c, 
0x8fdd, 0xc248, 0x8fc5, 0xc274, 0x8fad, 0xc2a0, 0x8f95, 0xc2cc, 
0x8f7d, 0xc2f8, 0x8f65, 0xc324, 0x8f4d, 0xc351, 0x8f35, 0xc37d, 
0x8f1d, 0xc3a9, 0x8f06, 0xc3d6, 0x8eee, 0xc402, 0x8ed6, 0xc42e, 
0x8ebf, 0xc45b, 0x8ea8, 0xc487, 0x8e90, 0xc4b4, 0x8e79, 0xc4e0, 
0x8e62, 0xc50d, 0x8e4b, 0xc53a, 0x8e34, 0xc566, 0x8e1d, 0xc593, 
0x8e06, 0xc5c0, 0x8def, 0xc5ed, 0x8dd8, 0xc619, 0x8dc1, 0xc646, 
0x8dab, 0xc673, 0x8d94, 0xc6a0, 0x8d7e, 0xc6cd, 0x8d67, 0xc6fa, 
0x8d51, 0xc727, 0x8d3b, 0xc754, 0x8d24, 0xc781, 0x8d0e, 0xc7ae, 
0x8cf8, 0xc7db, 0x8ce2, 0xc809, 0x8ccc, 0xc836, 0x8cb6, 0xc863, 
0x8ca1, 0xc890, 0x8c8b, 0xc8be, 0x8c75, 0xc8eb, 0x8c60, 0xc918, 
0x8c4a, 0xc946, 0x8c35, 0xc973, 0x8c1f, 0xc9a1, 0x8c0a, 0xc9ce, 
0x8bf5, 0xc9fc, 0x8bdf, 0xca29, 0x8bca, 0xca57, 0x8bb5, 0xca85, 
0x8ba0, 0xcab2, 0x8b8b, 0xcae0, 0x8b77, 0xcb0e, 0x8b62, 0xcb3c, 
0x8b4d, 0xcb69, 0x8b39, 0xcb97, 0x8b24, 0xcbc5, 0x8b10, 0xcbf3, 
0x8afb, 0xcc21, 0x8ae7, 0xcc4f, 0x8ad3, 0xcc7d, 0x8abe, 0xccab, 
0x8aaa, 0xccd9, 0x8a96, 0xcd07, 0x8a82, 0xcd35, 0x8a6e, 0xcd63, 
0x8a5a, 0xcd92, 0x8a47, 0xcdc0, 0x8a33, 0xcdee, 0x8a1f, 0xce1c, 
0x8a0c, 0xce4b, 0x89f8, 0xce79, 0x89e5, 0xcea7, 0x89d2, 0xced6, 
0x89be, 0xcf04, 0x89ab, 0xcf33, 0x8998, 0xcf61, 0x8985, 0xcf90, 
0x8972, 0xcfbe, 0x895f, 0xcfed, 0x894c, 0xd01b, 0x8939, 0xd04a, 
0x8927, 0xd079, 0x8914, 0xd0a7, 0x8902, 0xd0d6, 0x88ef, 0xd105, 
0x88dd, 0xd134, 0x88ca, 0xd162, 0x88b8, 0xd191, 0x88a6, 0xd1c0, 
0x8894, 0xd1ef, 0x8882, 0xd21e, 0x8870, 0xd24d, 0x885e, 0xd27c, 
0x884c, 0xd2ab, 0x883a, 0xd2da, 0x8828, 0xd309, 0x8817, 0xd338, 
0x8805, 0xd367, 0x87f4, 0xd396, 0x87e2, 0xd3c5, 0x87d1, 0xd3f4, 
0x87c0, 0xd424, 0x87af, 0xd453, 0x879d, 0xd482, 0x878c, 0xd4b1, 
0x877b, 0xd4e1, 0x876b, 0xd510, 0x875a, 0xd53f, 0x8749, 0xd56f, 
0x8738, 0xd59e, 0x8728, 0xd5ce, 0x8717, 0xd5fd, 0x8707, 0xd62d, 
0x86f6, 0xd65c, 0x86e6, 0xd68c, 0x86d6, 0xd6bb, 0x86c6, 0xd6eb, 
0x86b6, 0xd71b, 0x86a5, 0xd74a, 0x8696, 0xd77a, 0x8686, 0xd7aa, 
0x8676, 0xd7d9, 0x8666, 0xd809, 0x8656, 0xd839, 0x8647, 0xd869, 
0x8637, 0xd898, 0x8628, 0xd8c8, 0x8619, 0xd8f8, 0x8609, 0xd928, 
0x85fa, 0xd958, 0x85eb, 0xd988, 0x85dc, 0xd9b8, 0x85cd, 0xd9e8, 
0x85be, 0xda18, 0x85af, 0xda48, 0x85a0, 0xda78, 0x8592, 0xdaa8, 
0x8583, 0xdad8, 0x8574, 0xdb08, 0x8566, 0xdb38, 0x8558, 0xdb68, 
0x8549, 0xdb99, 0x853b, 0xdbc9, 0x852d, 0xdbf9, 0x851f, 0xdc29, 
0x8511, 0xdc59, 0x8503, 0xdc8a, 0x84f5, 0xdcba, 0x84e7, 0xdcea, 
0x84d9, 0xdd1b, 0x84cc, 0xdd4b, 0x84be, 0xdd7c, 0x84b0, 0xddac, 
0x84a3, 0xdddc, 0x8496, 0xde0d, 0x8488, 0xde3d, 0x847b, 0xde6e, 
0x846e, 0xde9e, 0x8461, 0xdecf, 0x8454, 0xdeff, 0x8447, 0xdf30, 
0x843a, 0xdf61, 0x842d, 0xdf91, 0x8421, 0xdfc2, 0x8414, 0xdff2, 
0x8407, 0xe023, 0x83fb, 0xe054, 0x83ef, 0xe085, 0x83e2, 0xe0b5, 
0x83d6, 0xe0e6, 0x83ca, 0xe117, 0x83be, 0xe148, 0x83b2, 0xe178, 
0x83a6, 0xe1a9, 0x839a, 0xe1da, 0x838e, 0xe20b, 0x8382, 0xe23c, 
0x8377, 0xe26d, 0x836b, 0xe29e, 0x8360, 0xe2cf, 0x8354, 0xe2ff, 
0x8349, 0xe330, 0x833e, 0xe361, 0x8332, 0xe392, 0x8327, 0xe3c3, 
0x831c, 0xe3f4, 0x8311, 0xe426, 0x8306, 0xe457, 0x82fb, 0xe488, 
0x82f1, 0xe4b9, 0x82e6, 0xe4ea, 0x82db, 0xe51b, 0x82d1, 0xe54c, 
0x82c6, 0xe57d, 0x82bc, 0xe5af, 0x82b2, 0xe5e0, 0x82a8, 0xe611, 
0x829d, 0xe642, 0x8293, 0xe673, 0x8289, 0xe6a5, 0x827f, 0xe6d6, 
0x8276, 0xe707, 0x826c, 0xe739, 0x8262, 0xe76a, 0x8259, 0xe79b, 
0x824f, 0xe7cd, 0x8246, 0xe7fe, 0x823c, 0xe82f, 0x8233, 0xe861, 
0x822a, 0xe892, 0x8220, 0xe8c4, 0x8217, 0xe8f5, 0x820e, 0xe926, 
0x8205, 0xe958, 0x81fd, 0xe989, 0x81f4, 0xe9bb, 0x81eb, 0xe9ec, 
0x81e2, 0xea1e, 0x81da, 0xea4f, 0x81d1, 0xea81, 0x81c9, 0xeab3, 
0x81c1, 0xeae4, 0x81b8, 0xeb16, 0x81b0, 0xeb47, 0x81a8, 0xeb79, 
0x81a0, 0xebab, 0x8198, 0xebdc, 0x8190, 0xec0e, 0x8188, 0xec3f, 
0x8181, 0xec71, 0x8179, 0xeca3, 0x8172, 0xecd5, 0x816a, 0xed06, 
0x8163, 0xed38, 0x815b, 0xed6a, 0x8154, 0xed9b, 0x814d, 0xedcd, 
0x8146, 0xedff, 0x813f, 0xee31, 0x8138, 0xee62, 0x8131, 0xee94, 
0x812a, 0xeec6, 0x8123, 0xeef8, 0x811d, 0xef2a, 0x8116, 0xef5c, 
0x8110, 0xef8d, 0x8109, 0xefbf, 0x8103, 0xeff1, 0x80fd, 0xf023, 
0x80f6, 0xf055, 0x80f0, 0xf087, 0x80ea, 0xf0b9, 0x80e4, 0xf0eb, 
0x80de, 0xf11c, 0x80d9, 0xf14e, 0x80d3, 0xf180, 0x80cd, 0xf1b2, 
0x80c8, 0xf1e4, 0x80c2, 0xf216, 0x80bd, 0xf248, 0x80b7, 0xf27a, 
0x80b2, 0xf2ac, 0x80ad, 0xf2de, 0x80a8, 0xf310, 0x80a3, 0xf342, 
0x809e, 0xf374, 0x8099, 0xf3a6, 0x8094, 0xf3d8, 0x808f, 0xf40a, 
0x808b, 0xf43c, 0x8086, 0xf46e, 0x8082, 0xf4a0, 0x807d, 0xf4d3, 
0x8079, 0xf505, 0x8075, 0xf537, 0x8070, 0xf569, 0x806c, 0xf59b, 
0x8068, 0xf5cd, 0x8064, 0xf5ff, 0x8060, 0xf631, 0x805d, 0xf663, 
0x8059, 0xf695, 0x8055, 0xf6c8, 0x8052, 0xf6fa, 0x804e, 0xf72c, 
0x804b, 0xf75e, 0x8047, 0xf790, 0x8044, 0xf7c2, 0x8041, 0xf7f4, 
0x803e, 0xf827, 0x803b, 0xf859, 0x8038, 0xf88b, 0x8035, 0xf8bd, 
0x8032, 0xf8ef, 0x802f, 0xf922, 0x802d, 0xf954, 0x802a, 0xf986, 
0x8027, 0xf9b8, 0x8025, 0xf9ea, 0x8023, 0xfa1d, 0x8020, 0xfa4f, 
0x801e, 0xfa81, 0x801c, 0xfab3, 0x801a, 0xfae5, 0x8018, 0xfb18, 
0x8016, 0xfb4a, 0x8014, 0xfb7c, 0x8013, 0xfbae, 0x8011, 0xfbe1, 
0x800f, 0xfc13, 0x800e, 0xfc45, 0x800c, 0xfc77, 0x800b, 0xfcaa, 
0x800a, 0xfcdc, 0x8009, 0xfd0e, 0x8008, 0xfd40, 0x8007, 0xfd73, 
0x8006, 0xfda5, 0x8005, 0xfdd7, 0x8004, 0xfe09, 0x8003, 0xfe3c, 
0x8002, 0xfe6e, 0x8002, 0xfea0, 0x8001, 0xfed2, 0x8001, 0xff05, 
0x8001, 0xff37, 0x8000, 0xff69, 0x8000, 0xff9b, 0x8000, 0xffce, 

};
