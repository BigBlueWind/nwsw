//-------------------------------------------------------------------------------------------
/** @file bs_rx_prach_table.c
 *
 * @brief PRACH Tables
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "typedef.h"

#ifndef _WIN32
#if defined (__CC_ARM)
__align(8) S16 PRACH_Tables[6720] = {
#elif defined (__GNUC__)
__attribute__((aligned(8))) S16 PRACH_Tables[6720] = {
#endif
#else
S16 PRACH_Tables[6720] = {
#endif
// S16 ZC839Base[3360] = {
(short)0x7FFF,   //    0
(short)0x0000,
(short)0x7FFF,   //    1
(short)0xFF85,
(short)0x7FFF,   //    2
(short)0xFF0B,
(short)0x7FFE,   //    3
(short)0xFE90,
(short)0x7FFC,   //    4
(short)0xFE15,
(short)0x7FFA,   //    5
(short)0xFD9B,
(short)0x7FF8,   //    6
(short)0xFD20,
(short)0x7FF5,   //    7
(short)0xFCA5,
(short)0x7FF1,   //    8
(short)0xFC2B,
(short)0x7FED,   //    9
(short)0xFBB0,
(short)0x7FE9,   //   10
(short)0xFB35,
(short)0x7FE4,   //   11
(short)0xFABB,
(short)0x7FDF,   //   12
(short)0xFA40,
(short)0x7FD9,   //   13
(short)0xF9C6,
(short)0x7FD3,   //   14
(short)0xF94B,
(short)0x7FCC,   //   15
(short)0xF8D0,
(short)0x7FC5,   //   16
(short)0xF856,
(short)0x7FBE,   //   17
(short)0xF7DC,
(short)0x7FB6,   //   18
(short)0xF761,
(short)0x7FAD,   //   19
(short)0xF6E7,
(short)0x7FA4,   //   20
(short)0xF66C,
(short)0x7F9B,   //   21
(short)0xF5F2,
(short)0x7F91,   //   22
(short)0xF578,
(short)0x7F87,   //   23
(short)0xF4FD,
(short)0x7F7C,   //   24
(short)0xF483,
(short)0x7F71,   //   25
(short)0xF409,
(short)0x7F65,   //   26
(short)0xF38F,
(short)0x7F59,   //   27
(short)0xF315,
(short)0x7F4C,   //   28
(short)0xF29B,
(short)0x7F3F,   //   29
(short)0xF221,
(short)0x7F31,   //   30
(short)0xF1A7,
(short)0x7F23,   //   31
(short)0xF12D,
(short)0x7F15,   //   32
(short)0xF0B3,
(short)0x7F06,   //   33
(short)0xF039,
(short)0x7EF7,   //   34
(short)0xEFC0,
(short)0x7EE7,   //   35
(short)0xEF46,
(short)0x7ED7,   //   36
(short)0xEECC,
(short)0x7EC6,   //   37
(short)0xEE53,
(short)0x7EB5,   //   38
(short)0xEDD9,
(short)0x7EA3,   //   39
(short)0xED60,
(short)0x7E91,   //   40
(short)0xECE6,
(short)0x7E7F,   //   41
(short)0xEC6D,
(short)0x7E6C,   //   42
(short)0xEBF4,
(short)0x7E58,   //   43
(short)0xEB7B,
(short)0x7E44,   //   44
(short)0xEB02,
(short)0x7E30,   //   45
(short)0xEA89,
(short)0x7E1B,   //   46
(short)0xEA10,
(short)0x7E06,   //   47
(short)0xE997,
(short)0x7DF0,   //   48
(short)0xE91E,
(short)0x7DDA,   //   49
(short)0xE8A5,
(short)0x7DC3,   //   50
(short)0xE82D,
(short)0x7DAC,   //   51
(short)0xE7B4,
(short)0x7D95,   //   52
(short)0xE73C,
(short)0x7D7D,   //   53
(short)0xE6C4,
(short)0x7D64,   //   54
(short)0xE64B,
(short)0x7D4C,   //   55
(short)0xE5D3,
(short)0x7D32,   //   56
(short)0xE55B,
(short)0x7D18,   //   57
(short)0xE4E3,
(short)0x7CFE,   //   58
(short)0xE46B,
(short)0x7CE4,   //   59
(short)0xE3F4,
(short)0x7CC8,   //   60
(short)0xE37C,
(short)0x7CAD,   //   61
(short)0xE304,
(short)0x7C91,   //   62
(short)0xE28D,
(short)0x7C74,   //   63
(short)0xE216,
(short)0x7C58,   //   64
(short)0xE19E,
(short)0x7C3A,   //   65
(short)0xE127,
(short)0x7C1C,   //   66
(short)0xE0B0,
(short)0x7BFE,   //   67
(short)0xE039,
(short)0x7BE0,   //   68
(short)0xDFC2,
(short)0x7BC0,   //   69
(short)0xDF4C,
(short)0x7BA1,   //   70
(short)0xDED5,
(short)0x7B81,   //   71
(short)0xDE5F,
(short)0x7B60,   //   72
(short)0xDDE8,
(short)0x7B3F,   //   73
(short)0xDD72,
(short)0x7B1E,   //   74
(short)0xDCFC,
(short)0x7AFC,   //   75
(short)0xDC86,
(short)0x7ADA,   //   76
(short)0xDC10,
(short)0x7AB7,   //   77
(short)0xDB9B,
(short)0x7A94,   //   78
(short)0xDB25,
(short)0x7A71,   //   79
(short)0xDAB0,
(short)0x7A4D,   //   80
(short)0xDA3A,
(short)0x7A28,   //   81
(short)0xD9C5,
(short)0x7A03,   //   82
(short)0xD950,
(short)0x79DE,   //   83
(short)0xD8DB,
(short)0x79B8,   //   84
(short)0xD866,
(short)0x7992,   //   85
(short)0xD7F2,
(short)0x796C,   //   86
(short)0xD77D,
(short)0x7945,   //   87
(short)0xD709,
(short)0x791D,   //   88
(short)0xD695,
(short)0x78F5,   //   89
(short)0xD621,
(short)0x78CD,   //   90
(short)0xD5AD,
(short)0x78A4,   //   91
(short)0xD539,
(short)0x787B,   //   92
(short)0xD4C6,
(short)0x7851,   //   93
(short)0xD452,
(short)0x7827,   //   94
(short)0xD3DF,
(short)0x77FD,   //   95
(short)0xD36C,
(short)0x77D2,   //   96
(short)0xD2F9,
(short)0x77A6,   //   97
(short)0xD286,
(short)0x777A,   //   98
(short)0xD214,
(short)0x774E,   //   99
(short)0xD1A1,
(short)0x7722,   //  100
(short)0xD12F,
(short)0x76F4,   //  101
(short)0xD0BD,
(short)0x76C7,   //  102
(short)0xD04B,
(short)0x7699,   //  103
(short)0xCFD9,
(short)0x766B,   //  104
(short)0xCF67,
(short)0x763C,   //  105
(short)0xCEF6,
(short)0x760D,   //  106
(short)0xCE85,
(short)0x75DD,   //  107
(short)0xCE14,
(short)0x75AD,   //  108
(short)0xCDA3,
(short)0x757C,   //  109
(short)0xCD32,
(short)0x754B,   //  110
(short)0xCCC2,
(short)0x751A,   //  111
(short)0xCC51,
(short)0x74E8,   //  112
(short)0xCBE1,
(short)0x74B6,   //  113
(short)0xCB71,
(short)0x7484,   //  114
(short)0xCB01,
(short)0x7451,   //  115
(short)0xCA92,
(short)0x741D,   //  116
(short)0xCA22,
(short)0x73E9,   //  117
(short)0xC9B3,
(short)0x73B5,   //  118
(short)0xC944,
(short)0x7380,   //  119
(short)0xC8D5,
(short)0x734B,   //  120
(short)0xC867,
(short)0x7316,   //  121
(short)0xC7F8,
(short)0x72E0,   //  122
(short)0xC78A,
(short)0x72AA,   //  123
(short)0xC71C,
(short)0x7273,   //  124
(short)0xC6AE,
(short)0x723C,   //  125
(short)0xC641,
(short)0x7204,   //  126
(short)0xC5D3,
(short)0x71CC,   //  127
(short)0xC566,
(short)0x7194,   //  128
(short)0xC4F9,
(short)0x715B,   //  129
(short)0xC48C,
(short)0x7122,   //  130
(short)0xC420,
(short)0x70E8,   //  131
(short)0xC3B3,
(short)0x70AE,   //  132
(short)0xC347,
(short)0x7074,   //  133
(short)0xC2DB,
(short)0x7039,   //  134
(short)0xC270,
(short)0x6FFE,   //  135
(short)0xC204,
(short)0x6FC2,   //  136
(short)0xC199,
(short)0x6F86,   //  137
(short)0xC12E,
(short)0x6F4A,   //  138
(short)0xC0C3,
(short)0x6F0D,   //  139
(short)0xC059,
(short)0x6ED0,   //  140
(short)0xBFEE,
(short)0x6E92,   //  141
(short)0xBF84,
(short)0x6E54,   //  142
(short)0xBF1A,
(short)0x6E16,   //  143
(short)0xBEB1,
(short)0x6DD7,   //  144
(short)0xBE47,
(short)0x6D98,   //  145
(short)0xBDDE,
(short)0x6D58,   //  146
(short)0xBD75,
(short)0x6D18,   //  147
(short)0xBD0C,
(short)0x6CD8,   //  148
(short)0xBCA4,
(short)0x6C97,   //  149
(short)0xBC3C,
(short)0x6C56,   //  150
(short)0xBBD4,
(short)0x6C14,   //  151
(short)0xBB6C,
(short)0x6BD2,   //  152
(short)0xBB05,
(short)0x6B90,   //  153
(short)0xBA9D,
(short)0x6B4D,   //  154
(short)0xBA36,
(short)0x6B0A,   //  155
(short)0xB9D0,
(short)0x6AC7,   //  156
(short)0xB969,
(short)0x6A83,   //  157
(short)0xB903,
(short)0x6A3F,   //  158
(short)0xB89D,
(short)0x69FA,   //  159
(short)0xB837,
(short)0x69B5,   //  160
(short)0xB7D2,
(short)0x6970,   //  161
(short)0xB76D,
(short)0x692A,   //  162
(short)0xB708,
(short)0x68E4,   //  163
(short)0xB6A3,
(short)0x689D,   //  164
(short)0xB63F,
(short)0x6856,   //  165
(short)0xB5DA,
(short)0x680F,   //  166
(short)0xB577,
(short)0x67C7,   //  167
(short)0xB513,
(short)0x677F,   //  168
(short)0xB4B0,
(short)0x6737,   //  169
(short)0xB44D,
(short)0x66EE,   //  170
(short)0xB3EA,
(short)0x66A5,   //  171
(short)0xB387,
(short)0x665C,   //  172
(short)0xB325,
(short)0x6612,   //  173
(short)0xB2C3,
(short)0x65C8,   //  174
(short)0xB261,
(short)0x657D,   //  175
(short)0xB200,
(short)0x6532,   //  176
(short)0xB19F,
(short)0x64E7,   //  177
(short)0xB13E,
(short)0x649B,   //  178
(short)0xB0DD,
(short)0x644F,   //  179
(short)0xB07D,
(short)0x6403,   //  180
(short)0xB01D,
(short)0x63B6,   //  181
(short)0xAFBD,
(short)0x6369,   //  182
(short)0xAF5E,
(short)0x631B,   //  183
(short)0xAEFF,
(short)0x62CD,   //  184
(short)0xAEA0,
(short)0x627F,   //  185
(short)0xAE41,
(short)0x6231,   //  186
(short)0xADE3,
(short)0x61E2,   //  187
(short)0xAD85,
(short)0x6193,   //  188
(short)0xAD27,
(short)0x6143,   //  189
(short)0xACCA,
(short)0x60F3,   //  190
(short)0xAC6D,
(short)0x60A3,   //  191
(short)0xAC10,
(short)0x6052,   //  192
(short)0xABB4,
(short)0x6001,   //  193
(short)0xAB57,
(short)0x5FB0,   //  194
(short)0xAAFB,
(short)0x5F5E,   //  195
(short)0xAAA0,
(short)0x5F0C,   //  196
(short)0xAA45,
(short)0x5EBA,   //  197
(short)0xA9EA,
(short)0x5E67,   //  198
(short)0xA98F,
(short)0x5E14,   //  199
(short)0xA935,
(short)0x5DC1,   //  200
(short)0xA8DB,
(short)0x5D6D,   //  201
(short)0xA881,
(short)0x5D19,   //  202
(short)0xA828,
(short)0x5CC5,   //  203
(short)0xA7CE,
(short)0x5C70,   //  204
(short)0xA776,
(short)0x5C1B,   //  205
(short)0xA71D,
(short)0x5BC6,   //  206
(short)0xA6C5,
(short)0x5B70,   //  207
(short)0xA66D,
(short)0x5B1A,   //  208
(short)0xA616,
(short)0x5AC3,   //  209
(short)0xA5BF,
(short)0x5A6D,   //  210
(short)0xA568,
(short)0x5A16,   //  211
(short)0xA511,
(short)0x59BE,   //  212
(short)0xA4BB,
(short)0x5967,   //  213
(short)0xA465,
(short)0x590F,   //  214
(short)0xA410,
(short)0x58B7,   //  215
(short)0xA3BB,
(short)0x585E,   //  216
(short)0xA366,
(short)0x5805,   //  217
(short)0xA311,
(short)0x57AC,   //  218
(short)0xA2BD,
(short)0x5752,   //  219
(short)0xA269,
(short)0x56F8,   //  220
(short)0xA216,
(short)0x569E,   //  221
(short)0xA1C2,
(short)0x5644,   //  222
(short)0xA16F,
(short)0x55E9,   //  223
(short)0xA11D,
(short)0x558E,   //  224
(short)0xA0CB,
(short)0x5532,   //  225
(short)0xA079,
(short)0x54D7,   //  226
(short)0xA027,
(short)0x547B,   //  227
(short)0x9FD6,
(short)0x541E,   //  228
(short)0x9F85,
(short)0x53C2,   //  229
(short)0x9F35,
(short)0x5365,   //  230
(short)0x9EE5,
(short)0x5307,   //  231
(short)0x9E95,
(short)0x52AA,   //  232
(short)0x9E46,
(short)0x524C,   //  233
(short)0x9DF7,
(short)0x51EE,   //  234
(short)0x9DA8,
(short)0x5190,   //  235
(short)0x9D5A,
(short)0x5131,   //  236
(short)0x9D0C,
(short)0x50D2,   //  237
(short)0x9CBE,
(short)0x5073,   //  238
(short)0x9C71,
(short)0x5013,   //  239
(short)0x9C24,
(short)0x4FB3,   //  240
(short)0x9BD7,
(short)0x4F53,   //  241
(short)0x9B8B,
(short)0x4EF3,   //  242
(short)0x9B3F,
(short)0x4E92,   //  243
(short)0x9AF4,
(short)0x4E31,   //  244
(short)0x9AA8,
(short)0x4DCF,   //  245
(short)0x9A5E,
(short)0x4D6E,   //  246
(short)0x9A13,
(short)0x4D0C,   //  247
(short)0x99C9,
(short)0x4CAA,   //  248
(short)0x9980,
(short)0x4C48,   //  249
(short)0x9936,
(short)0x4BE5,   //  250
(short)0x98ED,
(short)0x4B82,   //  251
(short)0x98A5,
(short)0x4B1F,   //  252
(short)0x985D,
(short)0x4ABB,   //  253
(short)0x9815,
(short)0x4A58,   //  254
(short)0x97CD,
(short)0x49F4,   //  255
(short)0x9786,
(short)0x498F,   //  256
(short)0x973F,
(short)0x492B,   //  257
(short)0x96F9,
(short)0x48C6,   //  258
(short)0x96B3,
(short)0x4861,   //  259
(short)0x966E,
(short)0x47FB,   //  260
(short)0x9628,
(short)0x4796,   //  261
(short)0x95E4,
(short)0x4730,   //  262
(short)0x959F,
(short)0x46CA,   //  263
(short)0x955B,
(short)0x4664,   //  264
(short)0x9517,
(short)0x45FD,   //  265
(short)0x94D4,
(short)0x4596,   //  266
(short)0x9491,
(short)0x452F,   //  267
(short)0x944F,
(short)0x44C8,   //  268
(short)0x940D,
(short)0x4460,   //  269
(short)0x93CB,
(short)0x43F8,   //  270
(short)0x938A,
(short)0x4390,   //  271
(short)0x9349,
(short)0x4328,   //  272
(short)0x9308,
(short)0x42BF,   //  273
(short)0x92C8,
(short)0x4256,   //  274
(short)0x9288,
(short)0x41ED,   //  275
(short)0x9249,
(short)0x4184,   //  276
(short)0x920A,
(short)0x411B,   //  277
(short)0x91CB,
(short)0x40B1,   //  278
(short)0x918D,
(short)0x4047,   //  279
(short)0x914F,
(short)0x3FDD,   //  280
(short)0x9112,
(short)0x3F72,   //  281
(short)0x90D5,
(short)0x3F07,   //  282
(short)0x9098,
(short)0x3E9D,   //  283
(short)0x905C,
(short)0x3E31,   //  284
(short)0x9020,
(short)0x3DC6,   //  285
(short)0x8FE5,
(short)0x3D5A,   //  286
(short)0x8FAA,
(short)0x3CEF,   //  287
(short)0x8F6F,
(short)0x3C83,   //  288
(short)0x8F35,
(short)0x3C16,   //  289
(short)0x8EFB,
(short)0x3BAA,   //  290
(short)0x8EC2,
(short)0x3B3D,   //  291
(short)0x8E89,
(short)0x3AD0,   //  292
(short)0x8E50,
(short)0x3A63,   //  293
(short)0x8E18,
(short)0x39F6,   //  294
(short)0x8DE0,
(short)0x3989,   //  295
(short)0x8DA9,
(short)0x391B,   //  296
(short)0x8D72,
(short)0x38AD,   //  297
(short)0x8D3B,
(short)0x383F,   //  298
(short)0x8D05,
(short)0x37D1,   //  299
(short)0x8CCF,
(short)0x3762,   //  300
(short)0x8C9A,
(short)0x36F3,   //  301
(short)0x8C65,
(short)0x3684,   //  302
(short)0x8C31,
(short)0x3615,   //  303
(short)0x8BFD,
(short)0x35A6,   //  304
(short)0x8BC9,
(short)0x3536,   //  305
(short)0x8B96,
(short)0x34C7,   //  306
(short)0x8B63,
(short)0x3457,   //  307
(short)0x8B31,
(short)0x33E7,   //  308
(short)0x8AFF,
(short)0x3377,   //  309
(short)0x8ACD,
(short)0x3306,   //  310
(short)0x8A9C,
(short)0x3296,   //  311
(short)0x8A6B,
(short)0x3225,   //  312
(short)0x8A3B,
(short)0x31B4,   //  313
(short)0x8A0B,
(short)0x3143,   //  314
(short)0x89DC,
(short)0x30D1,   //  315
(short)0x89AD,
(short)0x3060,   //  316
(short)0x897E,
(short)0x2FEE,   //  317
(short)0x8950,
(short)0x2F7C,   //  318
(short)0x8922,
(short)0x2F0A,   //  319
(short)0x88F5,
(short)0x2E98,   //  320
(short)0x88C8,
(short)0x2E26,   //  321
(short)0x889C,
(short)0x2DB3,   //  322
(short)0x8870,
(short)0x2D40,   //  323
(short)0x8844,
(short)0x2CCE,   //  324
(short)0x8819,
(short)0x2C5B,   //  325
(short)0x87EE,
(short)0x2BE7,   //  326
(short)0x87C4,
(short)0x2B74,   //  327
(short)0x879A,
(short)0x2B01,   //  328
(short)0x8771,
(short)0x2A8D,   //  329
(short)0x8748,
(short)0x2A19,   //  330
(short)0x871F,
(short)0x29A5,   //  331
(short)0x86F7,
(short)0x2931,   //  332
(short)0x86CF,
(short)0x28BD,   //  333
(short)0x86A8,
(short)0x2848,   //  334
(short)0x8681,
(short)0x27D4,   //  335
(short)0x865B,
(short)0x275F,   //  336
(short)0x8635,
(short)0x26EA,   //  337
(short)0x860F,
(short)0x2675,   //  338
(short)0x85EA,
(short)0x2600,   //  339
(short)0x85C5,
(short)0x258B,   //  340
(short)0x85A1,
(short)0x2516,   //  341
(short)0x857D,
(short)0x24A0,   //  342
(short)0x855A,
(short)0x242B,   //  343
(short)0x8537,
(short)0x23B5,   //  344
(short)0x8515,
(short)0x233F,   //  345
(short)0x84F3,
(short)0x22C9,   //  346
(short)0x84D1,
(short)0x2253,   //  347
(short)0x84B0,
(short)0x21DC,   //  348
(short)0x848F,
(short)0x2166,   //  349
(short)0x846F,
(short)0x20F0,   //  350
(short)0x844F,
(short)0x2079,   //  351
(short)0x8430,
(short)0x2002,   //  352
(short)0x8411,
(short)0x1F8B,   //  353
(short)0x83F3,
(short)0x1F14,   //  354
(short)0x83D5,
(short)0x1E9D,   //  355
(short)0x83B7,
(short)0x1E26,   //  356
(short)0x839A,
(short)0x1DAF,   //  357
(short)0x837D,
(short)0x1D37,   //  358
(short)0x8361,
(short)0x1CC0,   //  359
(short)0x8345,
(short)0x1C48,   //  360
(short)0x832A,
(short)0x1BD1,   //  361
(short)0x830F,
(short)0x1B59,   //  362
(short)0x82F5,
(short)0x1AE1,   //  363
(short)0x82DB,
(short)0x1A69,   //  364
(short)0x82C1,
(short)0x19F1,   //  365
(short)0x82A8,
(short)0x1979,   //  366
(short)0x828F,
(short)0x1900,   //  367
(short)0x8277,
(short)0x1888,   //  368
(short)0x825F,
(short)0x180F,   //  369
(short)0x8248,
(short)0x1797,   //  370
(short)0x8231,
(short)0x171E,   //  371
(short)0x821B,
(short)0x16A5,   //  372
(short)0x8205,
(short)0x162D,   //  373
(short)0x81EF,
(short)0x15B4,   //  374
(short)0x81DA,
(short)0x153B,   //  375
(short)0x81C6,
(short)0x14C2,   //  376
(short)0x81B2,
(short)0x1449,   //  377
(short)0x819E,
(short)0x13D0,   //  378
(short)0x818B,
(short)0x1356,   //  379
(short)0x8178,
(short)0x12DD,   //  380
(short)0x8166,
(short)0x1264,   //  381
(short)0x8154,
(short)0x11EA,   //  382
(short)0x8143,
(short)0x1171,   //  383
(short)0x8132,
(short)0x10F7,   //  384
(short)0x8121,
(short)0x107D,   //  385
(short)0x8111,
(short)0x1004,   //  386
(short)0x8101,
(short)0x0F8A,   //  387
(short)0x80F2,
(short)0x0F10,   //  388
(short)0x80E4,
(short)0x0E96,   //  389
(short)0x80D5,
(short)0x0E1C,   //  390
(short)0x80C8,
(short)0x0DA2,   //  391
(short)0x80BA,
(short)0x0D28,   //  392
(short)0x80AE,
(short)0x0CAE,   //  393
(short)0x80A1,
(short)0x0C34,   //  394
(short)0x8095,
(short)0x0BBA,   //  395
(short)0x808A,
(short)0x0B40,   //  396
(short)0x807F,
(short)0x0AC5,   //  397
(short)0x8074,
(short)0x0A4B,   //  398
(short)0x806A,
(short)0x09D1,   //  399
(short)0x8060,
(short)0x0956,   //  400
(short)0x8057,
(short)0x08DC,   //  401
(short)0x804F,
(short)0x0862,   //  402
(short)0x8046,
(short)0x07E7,   //  403
(short)0x803F,
(short)0x076D,   //  404
(short)0x8037,
(short)0x06F2,   //  405
(short)0x8030,
(short)0x0678,   //  406
(short)0x802A,
(short)0x05FD,   //  407
(short)0x8024,
(short)0x0583,   //  408
(short)0x801E,
(short)0x0508,   //  409
(short)0x8019,
(short)0x048D,   //  410
(short)0x8015,
(short)0x0413,   //  411
(short)0x8011,
(short)0x0398,   //  412
(short)0x800D,
(short)0x031D,   //  413
(short)0x800A,
(short)0x02A3,   //  414
(short)0x8007,
(short)0x0228,   //  415
(short)0x8005,
(short)0x01AD,   //  416
(short)0x8003,
(short)0x0133,   //  417
(short)0x8001,
(short)0x00B8,   //  418
(short)0x8001,
(short)0x003D,   //  419
(short)0x8000,
(short)0xFFC3,   //  420
(short)0x8000,
(short)0xFF48,   //  421
(short)0x8001,
(short)0xFECD,   //  422
(short)0x8001,
(short)0xFE53,   //  423
(short)0x8003,
(short)0xFDD8,   //  424
(short)0x8005,
(short)0xFD5D,   //  425
(short)0x8007,
(short)0xFCE3,   //  426
(short)0x800A,
(short)0xFC68,   //  427
(short)0x800D,
(short)0xFBED,   //  428
(short)0x8011,
(short)0xFB73,   //  429
(short)0x8015,
(short)0xFAF8,   //  430
(short)0x8019,
(short)0xFA7D,   //  431
(short)0x801E,
(short)0xFA03,   //  432
(short)0x8024,
(short)0xF988,   //  433
(short)0x802A,
(short)0xF90E,   //  434
(short)0x8030,
(short)0xF893,   //  435
(short)0x8037,
(short)0xF819,   //  436
(short)0x803F,
(short)0xF79E,   //  437
(short)0x8046,
(short)0xF724,   //  438
(short)0x804F,
(short)0xF6AA,   //  439
(short)0x8057,
(short)0xF62F,   //  440
(short)0x8060,
(short)0xF5B5,   //  441
(short)0x806A,
(short)0xF53B,   //  442
(short)0x8074,
(short)0xF4C0,   //  443
(short)0x807F,
(short)0xF446,   //  444
(short)0x808A,
(short)0xF3CC,   //  445
(short)0x8095,
(short)0xF352,   //  446
(short)0x80A1,
(short)0xF2D8,   //  447
(short)0x80AE,
(short)0xF25E,   //  448
(short)0x80BA,
(short)0xF1E4,   //  449
(short)0x80C8,
(short)0xF16A,   //  450
(short)0x80D5,
(short)0xF0F0,   //  451
(short)0x80E4,
(short)0xF076,   //  452
(short)0x80F2,
(short)0xEFFC,   //  453
(short)0x8101,
(short)0xEF83,   //  454
(short)0x8111,
(short)0xEF09,   //  455
(short)0x8121,
(short)0xEE8F,   //  456
(short)0x8132,
(short)0xEE16,   //  457
(short)0x8143,
(short)0xED9C,   //  458
(short)0x8154,
(short)0xED23,   //  459
(short)0x8166,
(short)0xECAA,   //  460
(short)0x8178,
(short)0xEC30,   //  461
(short)0x818B,
(short)0xEBB7,   //  462
(short)0x819E,
(short)0xEB3E,   //  463
(short)0x81B2,
(short)0xEAC5,   //  464
(short)0x81C6,
(short)0xEA4C,   //  465
(short)0x81DA,
(short)0xE9D3,   //  466
(short)0x81EF,
(short)0xE95B,   //  467
(short)0x8205,
(short)0xE8E2,   //  468
(short)0x821B,
(short)0xE869,   //  469
(short)0x8231,
(short)0xE7F1,   //  470
(short)0x8248,
(short)0xE778,   //  471
(short)0x825F,
(short)0xE700,   //  472
(short)0x8277,
(short)0xE687,   //  473
(short)0x828F,
(short)0xE60F,   //  474
(short)0x82A8,
(short)0xE597,   //  475
(short)0x82C1,
(short)0xE51F,   //  476
(short)0x82DB,
(short)0xE4A7,   //  477
(short)0x82F5,
(short)0xE42F,   //  478
(short)0x830F,
(short)0xE3B8,   //  479
(short)0x832A,
(short)0xE340,   //  480
(short)0x8345,
(short)0xE2C9,   //  481
(short)0x8361,
(short)0xE251,   //  482
(short)0x837D,
(short)0xE1DA,   //  483
(short)0x839A,
(short)0xE163,   //  484
(short)0x83B7,
(short)0xE0EC,   //  485
(short)0x83D5,
(short)0xE075,   //  486
(short)0x83F3,
(short)0xDFFE,   //  487
(short)0x8411,
(short)0xDF87,   //  488
(short)0x8430,
(short)0xDF10,   //  489
(short)0x844F,
(short)0xDE9A,   //  490
(short)0x846F,
(short)0xDE24,   //  491
(short)0x848F,
(short)0xDDAD,   //  492
(short)0x84B0,
(short)0xDD37,   //  493
(short)0x84D1,
(short)0xDCC1,   //  494
(short)0x84F3,
(short)0xDC4B,   //  495
(short)0x8515,
(short)0xDBD5,   //  496
(short)0x8537,
(short)0xDB60,   //  497
(short)0x855A,
(short)0xDAEA,   //  498
(short)0x857D,
(short)0xDA75,   //  499
(short)0x85A1,
(short)0xDA00,   //  500
(short)0x85C5,
(short)0xD98B,   //  501
(short)0x85EA,
(short)0xD916,   //  502
(short)0x860F,
(short)0xD8A1,   //  503
(short)0x8635,
(short)0xD82C,   //  504
(short)0x865B,
(short)0xD7B8,   //  505
(short)0x8681,
(short)0xD743,   //  506
(short)0x86A8,
(short)0xD6CF,   //  507
(short)0x86CF,
(short)0xD65B,   //  508
(short)0x86F7,
(short)0xD5E7,   //  509
(short)0x871F,
(short)0xD573,   //  510
(short)0x8748,
(short)0xD4FF,   //  511
(short)0x8771,
(short)0xD48C,   //  512
(short)0x879A,
(short)0xD419,   //  513
(short)0x87C4,
(short)0xD3A5,   //  514
(short)0x87EE,
(short)0xD332,   //  515
(short)0x8819,
(short)0xD2C0,   //  516
(short)0x8844,
(short)0xD24D,   //  517
(short)0x8870,
(short)0xD1DA,   //  518
(short)0x889C,
(short)0xD168,   //  519
(short)0x88C8,
(short)0xD0F6,   //  520
(short)0x88F5,
(short)0xD084,   //  521
(short)0x8922,
(short)0xD012,   //  522
(short)0x8950,
(short)0xCFA0,   //  523
(short)0x897E,
(short)0xCF2F,   //  524
(short)0x89AD,
(short)0xCEBD,   //  525
(short)0x89DC,
(short)0xCE4C,   //  526
(short)0x8A0B,
(short)0xCDDB,   //  527
(short)0x8A3B,
(short)0xCD6A,   //  528
(short)0x8A6B,
(short)0xCCFA,   //  529
(short)0x8A9C,
(short)0xCC89,   //  530
(short)0x8ACD,
(short)0xCC19,   //  531
(short)0x8AFF,
(short)0xCBA9,   //  532
(short)0x8B31,
(short)0xCB39,   //  533
(short)0x8B63,
(short)0xCACA,   //  534
(short)0x8B96,
(short)0xCA5A,   //  535
(short)0x8BC9,
(short)0xC9EB,   //  536
(short)0x8BFD,
(short)0xC97C,   //  537
(short)0x8C31,
(short)0xC90D,   //  538
(short)0x8C65,
(short)0xC89E,   //  539
(short)0x8C9A,
(short)0xC82F,   //  540
(short)0x8CCF,
(short)0xC7C1,   //  541
(short)0x8D05,
(short)0xC753,   //  542
(short)0x8D3B,
(short)0xC6E5,   //  543
(short)0x8D72,
(short)0xC677,   //  544
(short)0x8DA9,
(short)0xC60A,   //  545
(short)0x8DE0,
(short)0xC59D,   //  546
(short)0x8E18,
(short)0xC530,   //  547
(short)0x8E50,
(short)0xC4C3,   //  548
(short)0x8E89,
(short)0xC456,   //  549
(short)0x8EC2,
(short)0xC3EA,   //  550
(short)0x8EFB,
(short)0xC37D,   //  551
(short)0x8F35,
(short)0xC311,   //  552
(short)0x8F6F,
(short)0xC2A6,   //  553
(short)0x8FAA,
(short)0xC23A,   //  554
(short)0x8FE5,
(short)0xC1CF,   //  555
(short)0x9020,
(short)0xC163,   //  556
(short)0x905C,
(short)0xC0F9,   //  557
(short)0x9098,
(short)0xC08E,   //  558
(short)0x90D5,
(short)0xC023,   //  559
(short)0x9112,
(short)0xBFB9,   //  560
(short)0x914F,
(short)0xBF4F,   //  561
(short)0x918D,
(short)0xBEE5,   //  562
(short)0x91CB,
(short)0xBE7C,   //  563
(short)0x920A,
(short)0xBE13,   //  564
(short)0x9249,
(short)0xBDAA,   //  565
(short)0x9288,
(short)0xBD41,   //  566
(short)0x92C8,
(short)0xBCD8,   //  567
(short)0x9308,
(short)0xBC70,   //  568
(short)0x9349,
(short)0xBC08,   //  569
(short)0x938A,
(short)0xBBA0,   //  570
(short)0x93CB,
(short)0xBB38,   //  571
(short)0x940D,
(short)0xBAD1,   //  572
(short)0x944F,
(short)0xBA6A,   //  573
(short)0x9491,
(short)0xBA03,   //  574
(short)0x94D4,
(short)0xB99C,   //  575
(short)0x9517,
(short)0xB936,   //  576
(short)0x955B,
(short)0xB8D0,   //  577
(short)0x959F,
(short)0xB86A,   //  578
(short)0x95E4,
(short)0xB805,   //  579
(short)0x9628,
(short)0xB79F,   //  580
(short)0x966E,
(short)0xB73A,   //  581
(short)0x96B3,
(short)0xB6D5,   //  582
(short)0x96F9,
(short)0xB671,   //  583
(short)0x973F,
(short)0xB60C,   //  584
(short)0x9786,
(short)0xB5A8,   //  585
(short)0x97CD,
(short)0xB545,   //  586
(short)0x9815,
(short)0xB4E1,   //  587
(short)0x985D,
(short)0xB47E,   //  588
(short)0x98A5,
(short)0xB41B,   //  589
(short)0x98ED,
(short)0xB3B8,   //  590
(short)0x9936,
(short)0xB356,   //  591
(short)0x9980,
(short)0xB2F4,   //  592
(short)0x99C9,
(short)0xB292,   //  593
(short)0x9A13,
(short)0xB231,   //  594
(short)0x9A5E,
(short)0xB1CF,   //  595
(short)0x9AA8,
(short)0xB16E,   //  596
(short)0x9AF4,
(short)0xB10D,   //  597
(short)0x9B3F,
(short)0xB0AD,   //  598
(short)0x9B8B,
(short)0xB04D,   //  599
(short)0x9BD7,
(short)0xAFED,   //  600
(short)0x9C24,
(short)0xAF8D,   //  601
(short)0x9C71,
(short)0xAF2E,   //  602
(short)0x9CBE,
(short)0xAECF,   //  603
(short)0x9D0C,
(short)0xAE70,   //  604
(short)0x9D5A,
(short)0xAE12,   //  605
(short)0x9DA8,
(short)0xADB4,   //  606
(short)0x9DF7,
(short)0xAD56,   //  607
(short)0x9E46,
(short)0xACF9,   //  608
(short)0x9E95,
(short)0xAC9B,   //  609
(short)0x9EE5,
(short)0xAC3E,   //  610
(short)0x9F35,
(short)0xABE2,   //  611
(short)0x9F85,
(short)0xAB85,   //  612
(short)0x9FD6,
(short)0xAB29,   //  613
(short)0xA027,
(short)0xAACE,   //  614
(short)0xA079,
(short)0xAA72,   //  615
(short)0xA0CB,
(short)0xAA17,   //  616
(short)0xA11D,
(short)0xA9BC,   //  617
(short)0xA16F,
(short)0xA962,   //  618
(short)0xA1C2,
(short)0xA908,   //  619
(short)0xA216,
(short)0xA8AE,   //  620
(short)0xA269,
(short)0xA854,   //  621
(short)0xA2BD,
(short)0xA7FB,   //  622
(short)0xA311,
(short)0xA7A2,   //  623
(short)0xA366,
(short)0xA749,   //  624
(short)0xA3BB,
(short)0xA6F1,   //  625
(short)0xA410,
(short)0xA699,   //  626
(short)0xA465,
(short)0xA642,   //  627
(short)0xA4BB,
(short)0xA5EA,   //  628
(short)0xA511,
(short)0xA593,   //  629
(short)0xA568,
(short)0xA53D,   //  630
(short)0xA5BF,
(short)0xA4E6,   //  631
(short)0xA616,
(short)0xA490,   //  632
(short)0xA66D,
(short)0xA43A,   //  633
(short)0xA6C5,
(short)0xA3E5,   //  634
(short)0xA71D,
(short)0xA390,   //  635
(short)0xA776,
(short)0xA33B,   //  636
(short)0xA7CE,
(short)0xA2E7,   //  637
(short)0xA828,
(short)0xA293,   //  638
(short)0xA881,
(short)0xA23F,   //  639
(short)0xA8DB,
(short)0xA1EC,   //  640
(short)0xA935,
(short)0xA199,   //  641
(short)0xA98F,
(short)0xA146,   //  642
(short)0xA9EA,
(short)0xA0F4,   //  643
(short)0xAA45,
(short)0xA0A2,   //  644
(short)0xAAA0,
(short)0xA050,   //  645
(short)0xAAFB,
(short)0x9FFF,   //  646
(short)0xAB57,
(short)0x9FAE,   //  647
(short)0xABB4,
(short)0x9F5D,   //  648
(short)0xAC10,
(short)0x9F0D,   //  649
(short)0xAC6D,
(short)0x9EBD,   //  650
(short)0xACCA,
(short)0x9E6D,   //  651
(short)0xAD27,
(short)0x9E1E,   //  652
(short)0xAD85,
(short)0x9DCF,   //  653
(short)0xADE3,
(short)0x9D81,   //  654
(short)0xAE41,
(short)0x9D33,   //  655
(short)0xAEA0,
(short)0x9CE5,   //  656
(short)0xAEFF,
(short)0x9C97,   //  657
(short)0xAF5E,
(short)0x9C4A,   //  658
(short)0xAFBD,
(short)0x9BFD,   //  659
(short)0xB01D,
(short)0x9BB1,   //  660
(short)0xB07D,
(short)0x9B65,   //  661
(short)0xB0DD,
(short)0x9B19,   //  662
(short)0xB13E,
(short)0x9ACE,   //  663
(short)0xB19F,
(short)0x9A83,   //  664
(short)0xB200,
(short)0x9A38,   //  665
(short)0xB261,
(short)0x99EE,   //  666
(short)0xB2C3,
(short)0x99A4,   //  667
(short)0xB325,
(short)0x995B,   //  668
(short)0xB387,
(short)0x9912,   //  669
(short)0xB3EA,
(short)0x98C9,   //  670
(short)0xB44D,
(short)0x9881,   //  671
(short)0xB4B0,
(short)0x9839,   //  672
(short)0xB513,
(short)0x97F1,   //  673
(short)0xB577,
(short)0x97AA,   //  674
(short)0xB5DA,
(short)0x9763,   //  675
(short)0xB63F,
(short)0x971C,   //  676
(short)0xB6A3,
(short)0x96D6,   //  677
(short)0xB708,
(short)0x9690,   //  678
(short)0xB76D,
(short)0x964B,   //  679
(short)0xB7D2,
(short)0x9606,   //  680
(short)0xB837,
(short)0x95C1,   //  681
(short)0xB89D,
(short)0x957D,   //  682
(short)0xB903,
(short)0x9539,   //  683
(short)0xB969,
(short)0x94F6,   //  684
(short)0xB9D0,
(short)0x94B3,   //  685
(short)0xBA36,
(short)0x9470,   //  686
(short)0xBA9D,
(short)0x942E,   //  687
(short)0xBB05,
(short)0x93EC,   //  688
(short)0xBB6C,
(short)0x93AA,   //  689
(short)0xBBD4,
(short)0x9369,   //  690
(short)0xBC3C,
(short)0x9328,   //  691
(short)0xBCA4,
(short)0x92E8,   //  692
(short)0xBD0C,
(short)0x92A8,   //  693
(short)0xBD75,
(short)0x9268,   //  694
(short)0xBDDE,
(short)0x9229,   //  695
(short)0xBE47,
(short)0x91EA,   //  696
(short)0xBEB1,
(short)0x91AC,   //  697
(short)0xBF1A,
(short)0x916E,   //  698
(short)0xBF84,
(short)0x9130,   //  699
(short)0xBFEE,
(short)0x90F3,   //  700
(short)0xC059,
(short)0x90B6,   //  701
(short)0xC0C3,
(short)0x907A,   //  702
(short)0xC12E,
(short)0x903E,   //  703
(short)0xC199,
(short)0x9002,   //  704
(short)0xC204,
(short)0x8FC7,   //  705
(short)0xC270,
(short)0x8F8C,   //  706
(short)0xC2DB,
(short)0x8F52,   //  707
(short)0xC347,
(short)0x8F18,   //  708
(short)0xC3B3,
(short)0x8EDE,   //  709
(short)0xC420,
(short)0x8EA5,   //  710
(short)0xC48C,
(short)0x8E6C,   //  711
(short)0xC4F9,
(short)0x8E34,   //  712
(short)0xC566,
(short)0x8DFC,   //  713
(short)0xC5D3,
(short)0x8DC4,   //  714
(short)0xC641,
(short)0x8D8D,   //  715
(short)0xC6AE,
(short)0x8D56,   //  716
(short)0xC71C,
(short)0x8D20,   //  717
(short)0xC78A,
(short)0x8CEA,   //  718
(short)0xC7F8,
(short)0x8CB5,   //  719
(short)0xC867,
(short)0x8C80,   //  720
(short)0xC8D5,
(short)0x8C4B,   //  721
(short)0xC944,
(short)0x8C17,   //  722
(short)0xC9B3,
(short)0x8BE3,   //  723
(short)0xCA22,
(short)0x8BAF,   //  724
(short)0xCA92,
(short)0x8B7C,   //  725
(short)0xCB01,
(short)0x8B4A,   //  726
(short)0xCB71,
(short)0x8B18,   //  727
(short)0xCBE1,
(short)0x8AE6,   //  728
(short)0xCC51,
(short)0x8AB5,   //  729
(short)0xCCC2,
(short)0x8A84,   //  730
(short)0xCD32,
(short)0x8A53,   //  731
(short)0xCDA3,
(short)0x8A23,   //  732
(short)0xCE14,
(short)0x89F3,   //  733
(short)0xCE85,
(short)0x89C4,   //  734
(short)0xCEF6,
(short)0x8995,   //  735
(short)0xCF67,
(short)0x8967,   //  736
(short)0xCFD9,
(short)0x8939,   //  737
(short)0xD04B,
(short)0x890C,   //  738
(short)0xD0BD,
(short)0x88DE,   //  739
(short)0xD12F,
(short)0x88B2,   //  740
(short)0xD1A1,
(short)0x8886,   //  741
(short)0xD214,
(short)0x885A,   //  742
(short)0xD286,
(short)0x882E,   //  743
(short)0xD2F9,
(short)0x8803,   //  744
(short)0xD36C,
(short)0x87D9,   //  745
(short)0xD3DF,
(short)0x87AF,   //  746
(short)0xD452,
(short)0x8785,   //  747
(short)0xD4C6,
(short)0x875C,   //  748
(short)0xD539,
(short)0x8733,   //  749
(short)0xD5AD,
(short)0x870B,   //  750
(short)0xD621,
(short)0x86E3,   //  751
(short)0xD695,
(short)0x86BB,   //  752
(short)0xD709,
(short)0x8694,   //  753
(short)0xD77D,
(short)0x866E,   //  754
(short)0xD7F2,
(short)0x8648,   //  755
(short)0xD866,
(short)0x8622,   //  756
(short)0xD8DB,
(short)0x85FD,   //  757
(short)0xD950,
(short)0x85D8,   //  758
(short)0xD9C5,
(short)0x85B3,   //  759
(short)0xDA3A,
(short)0x858F,   //  760
(short)0xDAB0,
(short)0x856C,   //  761
(short)0xDB25,
(short)0x8549,   //  762
(short)0xDB9B,
(short)0x8526,   //  763
(short)0xDC10,
(short)0x8504,   //  764
(short)0xDC86,
(short)0x84E2,   //  765
(short)0xDCFC,
(short)0x84C1,   //  766
(short)0xDD72,
(short)0x84A0,   //  767
(short)0xDDE8,
(short)0x847F,   //  768
(short)0xDE5F,
(short)0x845F,   //  769
(short)0xDED5,
(short)0x8440,   //  770
(short)0xDF4C,
(short)0x8420,   //  771
(short)0xDFC2,
(short)0x8402,   //  772
(short)0xE039,
(short)0x83E4,   //  773
(short)0xE0B0,
(short)0x83C6,   //  774
(short)0xE127,
(short)0x83A8,   //  775
(short)0xE19E,
(short)0x838C,   //  776
(short)0xE216,
(short)0x836F,   //  777
(short)0xE28D,
(short)0x8353,   //  778
(short)0xE304,
(short)0x8338,   //  779
(short)0xE37C,
(short)0x831C,   //  780
(short)0xE3F4,
(short)0x8302,   //  781
(short)0xE46B,
(short)0x82E8,   //  782
(short)0xE4E3,
(short)0x82CE,   //  783
(short)0xE55B,
(short)0x82B4,   //  784
(short)0xE5D3,
(short)0x829C,   //  785
(short)0xE64B,
(short)0x8283,   //  786
(short)0xE6C4,
(short)0x826B,   //  787
(short)0xE73C,
(short)0x8254,   //  788
(short)0xE7B4,
(short)0x823D,   //  789
(short)0xE82D,
(short)0x8226,   //  790
(short)0xE8A5,
(short)0x8210,   //  791
(short)0xE91E,
(short)0x81FA,   //  792
(short)0xE997,
(short)0x81E5,   //  793
(short)0xEA10,
(short)0x81D0,   //  794
(short)0xEA89,
(short)0x81BC,   //  795
(short)0xEB02,
(short)0x81A8,   //  796
(short)0xEB7B,
(short)0x8194,   //  797
(short)0xEBF4,
(short)0x8181,   //  798
(short)0xEC6D,
(short)0x816F,   //  799
(short)0xECE6,
(short)0x815D,   //  800
(short)0xED60,
(short)0x814B,   //  801
(short)0xEDD9,
(short)0x813A,   //  802
(short)0xEE53,
(short)0x8129,   //  803
(short)0xEECC,
(short)0x8119,   //  804
(short)0xEF46,
(short)0x8109,   //  805
(short)0xEFC0,
(short)0x80FA,   //  806
(short)0xF039,
(short)0x80EB,   //  807
(short)0xF0B3,
(short)0x80DD,   //  808
(short)0xF12D,
(short)0x80CF,   //  809
(short)0xF1A7,
(short)0x80C1,   //  810
(short)0xF221,
(short)0x80B4,   //  811
(short)0xF29B,
(short)0x80A7,   //  812
(short)0xF315,
(short)0x809B,   //  813
(short)0xF38F,
(short)0x808F,   //  814
(short)0xF409,
(short)0x8084,   //  815
(short)0xF483,
(short)0x8079,   //  816
(short)0xF4FD,
(short)0x806F,   //  817
(short)0xF578,
(short)0x8065,   //  818
(short)0xF5F2,
(short)0x805C,   //  819
(short)0xF66C,
(short)0x8053,   //  820
(short)0xF6E7,
(short)0x804A,   //  821
(short)0xF761,
(short)0x8042,   //  822
(short)0xF7DC,
(short)0x803B,   //  823
(short)0xF856,
(short)0x8034,   //  824
(short)0xF8D0,
(short)0x802D,   //  825
(short)0xF94B,
(short)0x8027,   //  826
(short)0xF9C6,
(short)0x8021,   //  827
(short)0xFA40,
(short)0x801C,   //  828
(short)0xFABB,
(short)0x8017,   //  829
(short)0xFB35,
(short)0x8013,   //  830
(short)0xFBB0,
(short)0x800F,   //  831
(short)0xFC2B,
(short)0x800B,   //  832
(short)0xFCA5,
(short)0x8008,   //  833
(short)0xFD20,
(short)0x8006,   //  834
(short)0xFD9B,
(short)0x8004,   //  835
(short)0xFE15,
(short)0x8002,   //  836
(short)0xFE90,
(short)0x8001,   //  837
(short)0xFF0B,
(short)0x8000,   //  838
(short)0xFF85,
(short)0x8000,   //  839
(short)0x0000,
(short)0x8000,   //  840
(short)0x007B,
(short)0x8001,   //  841
(short)0x00F5,
(short)0x8002,   //  842
(short)0x0170,
(short)0x8004,   //  843
(short)0x01EB,
(short)0x8006,   //  844
(short)0x0265,
(short)0x8008,   //  845
(short)0x02E0,
(short)0x800B,   //  846
(short)0x035B,
(short)0x800F,   //  847
(short)0x03D5,
(short)0x8013,   //  848
(short)0x0450,
(short)0x8017,   //  849
(short)0x04CB,
(short)0x801C,   //  850
(short)0x0545,
(short)0x8021,   //  851
(short)0x05C0,
(short)0x8027,   //  852
(short)0x063A,
(short)0x802D,   //  853
(short)0x06B5,
(short)0x8034,   //  854
(short)0x0730,
(short)0x803B,   //  855
(short)0x07AA,
(short)0x8042,   //  856
(short)0x0824,
(short)0x804A,   //  857
(short)0x089F,
(short)0x8053,   //  858
(short)0x0919,
(short)0x805C,   //  859
(short)0x0994,
(short)0x8065,   //  860
(short)0x0A0E,
(short)0x806F,   //  861
(short)0x0A88,
(short)0x8079,   //  862
(short)0x0B03,
(short)0x8084,   //  863
(short)0x0B7D,
(short)0x808F,   //  864
(short)0x0BF7,
(short)0x809B,   //  865
(short)0x0C71,
(short)0x80A7,   //  866
(short)0x0CEB,
(short)0x80B4,   //  867
(short)0x0D65,
(short)0x80C1,   //  868
(short)0x0DDF,
(short)0x80CF,   //  869
(short)0x0E59,
(short)0x80DD,   //  870
(short)0x0ED3,
(short)0x80EB,   //  871
(short)0x0F4D,
(short)0x80FA,   //  872
(short)0x0FC7,
(short)0x8109,   //  873
(short)0x1040,
(short)0x8119,   //  874
(short)0x10BA,
(short)0x8129,   //  875
(short)0x1134,
(short)0x813A,   //  876
(short)0x11AD,
(short)0x814B,   //  877
(short)0x1227,
(short)0x815D,   //  878
(short)0x12A0,
(short)0x816F,   //  879
(short)0x131A,
(short)0x8181,   //  880
(short)0x1393,
(short)0x8194,   //  881
(short)0x140C,
(short)0x81A8,   //  882
(short)0x1485,
(short)0x81BC,   //  883
(short)0x14FE,
(short)0x81D0,   //  884
(short)0x1577,
(short)0x81E5,   //  885
(short)0x15F0,
(short)0x81FA,   //  886
(short)0x1669,
(short)0x8210,   //  887
(short)0x16E2,
(short)0x8226,   //  888
(short)0x175B,
(short)0x823D,   //  889
(short)0x17D3,
(short)0x8254,   //  890
(short)0x184C,
(short)0x826B,   //  891
(short)0x18C4,
(short)0x8283,   //  892
(short)0x193C,
(short)0x829C,   //  893
(short)0x19B5,
(short)0x82B4,   //  894
(short)0x1A2D,
(short)0x82CE,   //  895
(short)0x1AA5,
(short)0x82E8,   //  896
(short)0x1B1D,
(short)0x8302,   //  897
(short)0x1B95,
(short)0x831C,   //  898
(short)0x1C0C,
(short)0x8338,   //  899
(short)0x1C84,
(short)0x8353,   //  900
(short)0x1CFC,
(short)0x836F,   //  901
(short)0x1D73,
(short)0x838C,   //  902
(short)0x1DEA,
(short)0x83A8,   //  903
(short)0x1E62,
(short)0x83C6,   //  904
(short)0x1ED9,
(short)0x83E4,   //  905
(short)0x1F50,
(short)0x8402,   //  906
(short)0x1FC7,
(short)0x8420,   //  907
(short)0x203E,
(short)0x8440,   //  908
(short)0x20B4,
(short)0x845F,   //  909
(short)0x212B,
(short)0x847F,   //  910
(short)0x21A1,
(short)0x84A0,   //  911
(short)0x2218,
(short)0x84C1,   //  912
(short)0x228E,
(short)0x84E2,   //  913
(short)0x2304,
(short)0x8504,   //  914
(short)0x237A,
(short)0x8526,   //  915
(short)0x23F0,
(short)0x8549,   //  916
(short)0x2465,
(short)0x856C,   //  917
(short)0x24DB,
(short)0x858F,   //  918
(short)0x2550,
(short)0x85B3,   //  919
(short)0x25C6,
(short)0x85D8,   //  920
(short)0x263B,
(short)0x85FD,   //  921
(short)0x26B0,
(short)0x8622,   //  922
(short)0x2725,
(short)0x8648,   //  923
(short)0x279A,
(short)0x866E,   //  924
(short)0x280E,
(short)0x8694,   //  925
(short)0x2883,
(short)0x86BB,   //  926
(short)0x28F7,
(short)0x86E3,   //  927
(short)0x296B,
(short)0x870B,   //  928
(short)0x29DF,
(short)0x8733,   //  929
(short)0x2A53,
(short)0x875C,   //  930
(short)0x2AC7,
(short)0x8785,   //  931
(short)0x2B3A,
(short)0x87AF,   //  932
(short)0x2BAE,
(short)0x87D9,   //  933
(short)0x2C21,
(short)0x8803,   //  934
(short)0x2C94,
(short)0x882E,   //  935
(short)0x2D07,
(short)0x885A,   //  936
(short)0x2D7A,
(short)0x8886,   //  937
(short)0x2DEC,
(short)0x88B2,   //  938
(short)0x2E5F,
(short)0x88DE,   //  939
(short)0x2ED1,
(short)0x890C,   //  940
(short)0x2F43,
(short)0x8939,   //  941
(short)0x2FB5,
(short)0x8967,   //  942
(short)0x3027,
(short)0x8995,   //  943
(short)0x3099,
(short)0x89C4,   //  944
(short)0x310A,
(short)0x89F3,   //  945
(short)0x317B,
(short)0x8A23,   //  946
(short)0x31EC,
(short)0x8A53,   //  947
(short)0x325D,
(short)0x8A84,   //  948
(short)0x32CE,
(short)0x8AB5,   //  949
(short)0x333E,
(short)0x8AE6,   //  950
(short)0x33AF,
(short)0x8B18,   //  951
(short)0x341F,
(short)0x8B4A,   //  952
(short)0x348F,
(short)0x8B7C,   //  953
(short)0x34FF,
(short)0x8BAF,   //  954
(short)0x356E,
(short)0x8BE3,   //  955
(short)0x35DE,
(short)0x8C17,   //  956
(short)0x364D,
(short)0x8C4B,   //  957
(short)0x36BC,
(short)0x8C80,   //  958
(short)0x372B,
(short)0x8CB5,   //  959
(short)0x3799,
(short)0x8CEA,   //  960
(short)0x3808,
(short)0x8D20,   //  961
(short)0x3876,
(short)0x8D56,   //  962
(short)0x38E4,
(short)0x8D8D,   //  963
(short)0x3952,
(short)0x8DC4,   //  964
(short)0x39BF,
(short)0x8DFC,   //  965
(short)0x3A2D,
(short)0x8E34,   //  966
(short)0x3A9A,
(short)0x8E6C,   //  967
(short)0x3B07,
(short)0x8EA5,   //  968
(short)0x3B74,
(short)0x8EDE,   //  969
(short)0x3BE0,
(short)0x8F18,   //  970
(short)0x3C4D,
(short)0x8F52,   //  971
(short)0x3CB9,
(short)0x8F8C,   //  972
(short)0x3D25,
(short)0x8FC7,   //  973
(short)0x3D90,
(short)0x9002,   //  974
(short)0x3DFC,
(short)0x903E,   //  975
(short)0x3E67,
(short)0x907A,   //  976
(short)0x3ED2,
(short)0x90B6,   //  977
(short)0x3F3D,
(short)0x90F3,   //  978
(short)0x3FA7,
(short)0x9130,   //  979
(short)0x4012,
(short)0x916E,   //  980
(short)0x407C,
(short)0x91AC,   //  981
(short)0x40E6,
(short)0x91EA,   //  982
(short)0x414F,
(short)0x9229,   //  983
(short)0x41B9,
(short)0x9268,   //  984
(short)0x4222,
(short)0x92A8,   //  985
(short)0x428B,
(short)0x92E8,   //  986
(short)0x42F4,
(short)0x9328,   //  987
(short)0x435C,
(short)0x9369,   //  988
(short)0x43C4,
(short)0x93AA,   //  989
(short)0x442C,
(short)0x93EC,   //  990
(short)0x4494,
(short)0x942E,   //  991
(short)0x44FB,
(short)0x9470,   //  992
(short)0x4563,
(short)0x94B3,   //  993
(short)0x45CA,
(short)0x94F6,   //  994
(short)0x4630,
(short)0x9539,   //  995
(short)0x4697,
(short)0x957D,   //  996
(short)0x46FD,
(short)0x95C1,   //  997
(short)0x4763,
(short)0x9606,   //  998
(short)0x47C9,
(short)0x964B,   //  999
(short)0x482E,
(short)0x9690,   // 1000
(short)0x4893,
(short)0x96D6,   // 1001
(short)0x48F8,
(short)0x971C,   // 1002
(short)0x495D,
(short)0x9763,   // 1003
(short)0x49C1,
(short)0x97AA,   // 1004
(short)0x4A26,
(short)0x97F1,   // 1005
(short)0x4A89,
(short)0x9839,   // 1006
(short)0x4AED,
(short)0x9881,   // 1007
(short)0x4B50,
(short)0x98C9,   // 1008
(short)0x4BB3,
(short)0x9912,   // 1009
(short)0x4C16,
(short)0x995B,   // 1010
(short)0x4C79,
(short)0x99A4,   // 1011
(short)0x4CDB,
(short)0x99EE,   // 1012
(short)0x4D3D,
(short)0x9A38,   // 1013
(short)0x4D9F,
(short)0x9A83,   // 1014
(short)0x4E00,
(short)0x9ACE,   // 1015
(short)0x4E61,
(short)0x9B19,   // 1016
(short)0x4EC2,
(short)0x9B65,   // 1017
(short)0x4F23,
(short)0x9BB1,   // 1018
(short)0x4F83,
(short)0x9BFD,   // 1019
(short)0x4FE3,
(short)0x9C4A,   // 1020
(short)0x5043,
(short)0x9C97,   // 1021
(short)0x50A2,
(short)0x9CE5,   // 1022
(short)0x5101,
(short)0x9D33,   // 1023
(short)0x5160,
(short)0x9D81,   // 1024
(short)0x51BF,
(short)0x9DCF,   // 1025
(short)0x521D,
(short)0x9E1E,   // 1026
(short)0x527B,
(short)0x9E6D,   // 1027
(short)0x52D9,
(short)0x9EBD,   // 1028
(short)0x5336,
(short)0x9F0D,   // 1029
(short)0x5393,
(short)0x9F5D,   // 1030
(short)0x53F0,
(short)0x9FAE,   // 1031
(short)0x544C,
(short)0x9FFF,   // 1032
(short)0x54A9,
(short)0xA050,   // 1033
(short)0x5505,
(short)0xA0A2,   // 1034
(short)0x5560,
(short)0xA0F4,   // 1035
(short)0x55BB,
(short)0xA146,   // 1036
(short)0x5616,
(short)0xA199,   // 1037
(short)0x5671,
(short)0xA1EC,   // 1038
(short)0x56CB,
(short)0xA23F,   // 1039
(short)0x5725,
(short)0xA293,   // 1040
(short)0x577F,
(short)0xA2E7,   // 1041
(short)0x57D8,
(short)0xA33B,   // 1042
(short)0x5832,
(short)0xA390,   // 1043
(short)0x588A,
(short)0xA3E5,   // 1044
(short)0x58E3,
(short)0xA43A,   // 1045
(short)0x593B,
(short)0xA490,   // 1046
(short)0x5993,
(short)0xA4E6,   // 1047
(short)0x59EA,
(short)0xA53D,   // 1048
(short)0x5A41,
(short)0xA593,   // 1049
(short)0x5A98,
(short)0xA5EA,   // 1050
(short)0x5AEF,
(short)0xA642,   // 1051
(short)0x5B45,
(short)0xA699,   // 1052
(short)0x5B9B,
(short)0xA6F1,   // 1053
(short)0x5BF0,
(short)0xA749,   // 1054
(short)0x5C45,
(short)0xA7A2,   // 1055
(short)0x5C9A,
(short)0xA7FB,   // 1056
(short)0x5CEF,
(short)0xA854,   // 1057
(short)0x5D43,
(short)0xA8AE,   // 1058
(short)0x5D97,
(short)0xA908,   // 1059
(short)0x5DEA,
(short)0xA962,   // 1060
(short)0x5E3E,
(short)0xA9BC,   // 1061
(short)0x5E91,
(short)0xAA17,   // 1062
(short)0x5EE3,
(short)0xAA72,   // 1063
(short)0x5F35,
(short)0xAACE,   // 1064
(short)0x5F87,
(short)0xAB29,   // 1065
(short)0x5FD9,
(short)0xAB85,   // 1066
(short)0x602A,
(short)0xABE2,   // 1067
(short)0x607B,
(short)0xAC3E,   // 1068
(short)0x60CB,
(short)0xAC9B,   // 1069
(short)0x611B,
(short)0xACF9,   // 1070
(short)0x616B,
(short)0xAD56,   // 1071
(short)0x61BA,
(short)0xADB4,   // 1072
(short)0x6209,
(short)0xAE12,   // 1073
(short)0x6258,
(short)0xAE70,   // 1074
(short)0x62A6,
(short)0xAECF,   // 1075
(short)0x62F4,
(short)0xAF2E,   // 1076
(short)0x6342,
(short)0xAF8D,   // 1077
(short)0x638F,
(short)0xAFED,   // 1078
(short)0x63DC,
(short)0xB04D,   // 1079
(short)0x6429,
(short)0xB0AD,   // 1080
(short)0x6475,
(short)0xB10D,   // 1081
(short)0x64C1,
(short)0xB16E,   // 1082
(short)0x650C,
(short)0xB1CF,   // 1083
(short)0x6558,
(short)0xB231,   // 1084
(short)0x65A2,
(short)0xB292,   // 1085
(short)0x65ED,
(short)0xB2F4,   // 1086
(short)0x6637,
(short)0xB356,   // 1087
(short)0x6680,
(short)0xB3B8,   // 1088
(short)0x66CA,
(short)0xB41B,   // 1089
(short)0x6713,
(short)0xB47E,   // 1090
(short)0x675B,
(short)0xB4E1,   // 1091
(short)0x67A3,
(short)0xB545,   // 1092
(short)0x67EB,
(short)0xB5A8,   // 1093
(short)0x6833,
(short)0xB60C,   // 1094
(short)0x687A,
(short)0xB671,   // 1095
(short)0x68C1,
(short)0xB6D5,   // 1096
(short)0x6907,
(short)0xB73A,   // 1097
(short)0x694D,
(short)0xB79F,   // 1098
(short)0x6992,
(short)0xB805,   // 1099
(short)0x69D8,
(short)0xB86A,   // 1100
(short)0x6A1C,
(short)0xB8D0,   // 1101
(short)0x6A61,
(short)0xB936,   // 1102
(short)0x6AA5,
(short)0xB99C,   // 1103
(short)0x6AE9,
(short)0xBA03,   // 1104
(short)0x6B2C,
(short)0xBA6A,   // 1105
(short)0x6B6F,
(short)0xBAD1,   // 1106
(short)0x6BB1,
(short)0xBB38,   // 1107
(short)0x6BF3,
(short)0xBBA0,   // 1108
(short)0x6C35,
(short)0xBC08,   // 1109
(short)0x6C76,
(short)0xBC70,   // 1110
(short)0x6CB7,
(short)0xBCD8,   // 1111
(short)0x6CF8,
(short)0xBD41,   // 1112
(short)0x6D38,
(short)0xBDAA,   // 1113
(short)0x6D78,
(short)0xBE13,   // 1114
(short)0x6DB7,
(short)0xBE7C,   // 1115
(short)0x6DF6,
(short)0xBEE5,   // 1116
(short)0x6E35,
(short)0xBF4F,   // 1117
(short)0x6E73,
(short)0xBFB9,   // 1118
(short)0x6EB1,
(short)0xC023,   // 1119
(short)0x6EEE,
(short)0xC08E,   // 1120
(short)0x6F2B,
(short)0xC0F9,   // 1121
(short)0x6F68,
(short)0xC163,   // 1122
(short)0x6FA4,
(short)0xC1CF,   // 1123
(short)0x6FE0,
(short)0xC23A,   // 1124
(short)0x701B,
(short)0xC2A6,   // 1125
(short)0x7056,
(short)0xC311,   // 1126
(short)0x7091,
(short)0xC37D,   // 1127
(short)0x70CB,
(short)0xC3EA,   // 1128
(short)0x7105,
(short)0xC456,   // 1129
(short)0x713E,
(short)0xC4C3,   // 1130
(short)0x7177,
(short)0xC530,   // 1131
(short)0x71B0,
(short)0xC59D,   // 1132
(short)0x71E8,
(short)0xC60A,   // 1133
(short)0x7220,
(short)0xC677,   // 1134
(short)0x7257,
(short)0xC6E5,   // 1135
(short)0x728E,
(short)0xC753,   // 1136
(short)0x72C5,
(short)0xC7C1,   // 1137
(short)0x72FB,
(short)0xC82F,   // 1138
(short)0x7331,
(short)0xC89E,   // 1139
(short)0x7366,
(short)0xC90D,   // 1140
(short)0x739B,
(short)0xC97C,   // 1141
(short)0x73CF,
(short)0xC9EB,   // 1142
(short)0x7403,
(short)0xCA5A,   // 1143
(short)0x7437,
(short)0xCACA,   // 1144
(short)0x746A,
(short)0xCB39,   // 1145
(short)0x749D,
(short)0xCBA9,   // 1146
(short)0x74CF,
(short)0xCC19,   // 1147
(short)0x7501,
(short)0xCC89,   // 1148
(short)0x7533,
(short)0xCCFA,   // 1149
(short)0x7564,
(short)0xCD6A,   // 1150
(short)0x7595,
(short)0xCDDB,   // 1151
(short)0x75C5,
(short)0xCE4C,   // 1152
(short)0x75F5,
(short)0xCEBD,   // 1153
(short)0x7624,
(short)0xCF2F,   // 1154
(short)0x7653,
(short)0xCFA0,   // 1155
(short)0x7682,
(short)0xD012,   // 1156
(short)0x76B0,
(short)0xD084,   // 1157
(short)0x76DE,
(short)0xD0F6,   // 1158
(short)0x770B,
(short)0xD168,   // 1159
(short)0x7738,
(short)0xD1DA,   // 1160
(short)0x7764,
(short)0xD24D,   // 1161
(short)0x7790,
(short)0xD2C0,   // 1162
(short)0x77BC,
(short)0xD332,   // 1163
(short)0x77E7,
(short)0xD3A5,   // 1164
(short)0x7812,
(short)0xD419,   // 1165
(short)0x783C,
(short)0xD48C,   // 1166
(short)0x7866,
(short)0xD4FF,   // 1167
(short)0x788F,
(short)0xD573,   // 1168
(short)0x78B8,
(short)0xD5E7,   // 1169
(short)0x78E1,
(short)0xD65B,   // 1170
(short)0x7909,
(short)0xD6CF,   // 1171
(short)0x7931,
(short)0xD743,   // 1172
(short)0x7958,
(short)0xD7B8,   // 1173
(short)0x797F,
(short)0xD82C,   // 1174
(short)0x79A5,
(short)0xD8A1,   // 1175
(short)0x79CB,
(short)0xD916,   // 1176
(short)0x79F1,
(short)0xD98B,   // 1177
(short)0x7A16,
(short)0xDA00,   // 1178
(short)0x7A3B,
(short)0xDA75,   // 1179
(short)0x7A5F,
(short)0xDAEA,   // 1180
(short)0x7A83,
(short)0xDB60,   // 1181
(short)0x7AA6,
(short)0xDBD5,   // 1182
(short)0x7AC9,
(short)0xDC4B,   // 1183
(short)0x7AEB,
(short)0xDCC1,   // 1184
(short)0x7B0D,
(short)0xDD37,   // 1185
(short)0x7B2F,
(short)0xDDAD,   // 1186
(short)0x7B50,
(short)0xDE24,   // 1187
(short)0x7B71,
(short)0xDE9A,   // 1188
(short)0x7B91,
(short)0xDF10,   // 1189
(short)0x7BB1,
(short)0xDF87,   // 1190
(short)0x7BD0,
(short)0xDFFE,   // 1191
(short)0x7BEF,
(short)0xE075,   // 1192
(short)0x7C0D,
(short)0xE0EC,   // 1193
(short)0x7C2B,
(short)0xE163,   // 1194
(short)0x7C49,
(short)0xE1DA,   // 1195
(short)0x7C66,
(short)0xE251,   // 1196
(short)0x7C83,
(short)0xE2C9,   // 1197
(short)0x7C9F,
(short)0xE340,   // 1198
(short)0x7CBB,
(short)0xE3B8,   // 1199
(short)0x7CD6,
(short)0xE42F,   // 1200
(short)0x7CF1,
(short)0xE4A7,   // 1201
(short)0x7D0B,
(short)0xE51F,   // 1202
(short)0x7D25,
(short)0xE597,   // 1203
(short)0x7D3F,
(short)0xE60F,   // 1204
(short)0x7D58,
(short)0xE687,   // 1205
(short)0x7D71,
(short)0xE700,   // 1206
(short)0x7D89,
(short)0xE778,   // 1207
(short)0x7DA1,
(short)0xE7F1,   // 1208
(short)0x7DB8,
(short)0xE869,   // 1209
(short)0x7DCF,
(short)0xE8E2,   // 1210
(short)0x7DE5,
(short)0xE95B,   // 1211
(short)0x7DFB,
(short)0xE9D3,   // 1212
(short)0x7E11,
(short)0xEA4C,   // 1213
(short)0x7E26,
(short)0xEAC5,   // 1214
(short)0x7E3A,
(short)0xEB3E,   // 1215
(short)0x7E4E,
(short)0xEBB7,   // 1216
(short)0x7E62,
(short)0xEC30,   // 1217
(short)0x7E75,
(short)0xECAA,   // 1218
(short)0x7E88,
(short)0xED23,   // 1219
(short)0x7E9A,
(short)0xED9C,   // 1220
(short)0x7EAC,
(short)0xEE16,   // 1221
(short)0x7EBD,
(short)0xEE8F,   // 1222
(short)0x7ECE,
(short)0xEF09,   // 1223
(short)0x7EDF,
(short)0xEF83,   // 1224
(short)0x7EEF,
(short)0xEFFC,   // 1225
(short)0x7EFF,
(short)0xF076,   // 1226
(short)0x7F0E,
(short)0xF0F0,   // 1227
(short)0x7F1C,
(short)0xF16A,   // 1228
(short)0x7F2B,
(short)0xF1E4,   // 1229
(short)0x7F38,
(short)0xF25E,   // 1230
(short)0x7F46,
(short)0xF2D8,   // 1231
(short)0x7F52,
(short)0xF352,   // 1232
(short)0x7F5F,
(short)0xF3CC,   // 1233
(short)0x7F6B,
(short)0xF446,   // 1234
(short)0x7F76,
(short)0xF4C0,   // 1235
(short)0x7F81,
(short)0xF53B,   // 1236
(short)0x7F8C,
(short)0xF5B5,   // 1237
(short)0x7F96,
(short)0xF62F,   // 1238
(short)0x7FA0,
(short)0xF6AA,   // 1239
(short)0x7FA9,
(short)0xF724,   // 1240
(short)0x7FB1,
(short)0xF79E,   // 1241
(short)0x7FBA,
(short)0xF819,   // 1242
(short)0x7FC1,
(short)0xF893,   // 1243
(short)0x7FC9,
(short)0xF90E,   // 1244
(short)0x7FD0,
(short)0xF988,   // 1245
(short)0x7FD6,
(short)0xFA03,   // 1246
(short)0x7FDC,
(short)0xFA7D,   // 1247
(short)0x7FE2,
(short)0xFAF8,   // 1248
(short)0x7FE7,
(short)0xFB73,   // 1249
(short)0x7FEB,
(short)0xFBED,   // 1250
(short)0x7FEF,
(short)0xFC68,   // 1251
(short)0x7FF3,
(short)0xFCE3,   // 1252
(short)0x7FF6,
(short)0xFD5D,   // 1253
(short)0x7FF9,
(short)0xFDD8,   // 1254
(short)0x7FFB,
(short)0xFE53,   // 1255
(short)0x7FFD,
(short)0xFECD,   // 1256
(short)0x7FFF,
(short)0xFF48,   // 1257
(short)0x7FFF,
(short)0xFFC3,   // 1258
(short)0x7FFF,
(short)0x003D,   // 1259
(short)0x7FFF,
(short)0x00B8,   // 1260
(short)0x7FFF,
(short)0x0133,   // 1261
(short)0x7FFF,
(short)0x01AD,   // 1262
(short)0x7FFD,
(short)0x0228,   // 1263
(short)0x7FFB,
(short)0x02A3,   // 1264
(short)0x7FF9,
(short)0x031D,   // 1265
(short)0x7FF6,
(short)0x0398,   // 1266
(short)0x7FF3,
(short)0x0413,   // 1267
(short)0x7FEF,
(short)0x048D,   // 1268
(short)0x7FEB,
(short)0x0508,   // 1269
(short)0x7FE7,
(short)0x0583,   // 1270
(short)0x7FE2,
(short)0x05FD,   // 1271
(short)0x7FDC,
(short)0x0678,   // 1272
(short)0x7FD6,
(short)0x06F2,   // 1273
(short)0x7FD0,
(short)0x076D,   // 1274
(short)0x7FC9,
(short)0x07E7,   // 1275
(short)0x7FC1,
(short)0x0862,   // 1276
(short)0x7FBA,
(short)0x08DC,   // 1277
(short)0x7FB1,
(short)0x0956,   // 1278
(short)0x7FA9,
(short)0x09D1,   // 1279
(short)0x7FA0,
(short)0x0A4B,   // 1280
(short)0x7F96,
(short)0x0AC5,   // 1281
(short)0x7F8C,
(short)0x0B40,   // 1282
(short)0x7F81,
(short)0x0BBA,   // 1283
(short)0x7F76,
(short)0x0C34,   // 1284
(short)0x7F6B,
(short)0x0CAE,   // 1285
(short)0x7F5F,
(short)0x0D28,   // 1286
(short)0x7F52,
(short)0x0DA2,   // 1287
(short)0x7F46,
(short)0x0E1C,   // 1288
(short)0x7F38,
(short)0x0E96,   // 1289
(short)0x7F2B,
(short)0x0F10,   // 1290
(short)0x7F1C,
(short)0x0F8A,   // 1291
(short)0x7F0E,
(short)0x1004,   // 1292
(short)0x7EFF,
(short)0x107D,   // 1293
(short)0x7EEF,
(short)0x10F7,   // 1294
(short)0x7EDF,
(short)0x1171,   // 1295
(short)0x7ECE,
(short)0x11EA,   // 1296
(short)0x7EBD,
(short)0x1264,   // 1297
(short)0x7EAC,
(short)0x12DD,   // 1298
(short)0x7E9A,
(short)0x1356,   // 1299
(short)0x7E88,
(short)0x13D0,   // 1300
(short)0x7E75,
(short)0x1449,   // 1301
(short)0x7E62,
(short)0x14C2,   // 1302
(short)0x7E4E,
(short)0x153B,   // 1303
(short)0x7E3A,
(short)0x15B4,   // 1304
(short)0x7E26,
(short)0x162D,   // 1305
(short)0x7E11,
(short)0x16A5,   // 1306
(short)0x7DFB,
(short)0x171E,   // 1307
(short)0x7DE5,
(short)0x1797,   // 1308
(short)0x7DCF,
(short)0x180F,   // 1309
(short)0x7DB8,
(short)0x1888,   // 1310
(short)0x7DA1,
(short)0x1900,   // 1311
(short)0x7D89,
(short)0x1979,   // 1312
(short)0x7D71,
(short)0x19F1,   // 1313
(short)0x7D58,
(short)0x1A69,   // 1314
(short)0x7D3F,
(short)0x1AE1,   // 1315
(short)0x7D25,
(short)0x1B59,   // 1316
(short)0x7D0B,
(short)0x1BD1,   // 1317
(short)0x7CF1,
(short)0x1C48,   // 1318
(short)0x7CD6,
(short)0x1CC0,   // 1319
(short)0x7CBB,
(short)0x1D37,   // 1320
(short)0x7C9F,
(short)0x1DAF,   // 1321
(short)0x7C83,
(short)0x1E26,   // 1322
(short)0x7C66,
(short)0x1E9D,   // 1323
(short)0x7C49,
(short)0x1F14,   // 1324
(short)0x7C2B,
(short)0x1F8B,   // 1325
(short)0x7C0D,
(short)0x2002,   // 1326
(short)0x7BEF,
(short)0x2079,   // 1327
(short)0x7BD0,
(short)0x20F0,   // 1328
(short)0x7BB1,
(short)0x2166,   // 1329
(short)0x7B91,
(short)0x21DC,   // 1330
(short)0x7B71,
(short)0x2253,   // 1331
(short)0x7B50,
(short)0x22C9,   // 1332
(short)0x7B2F,
(short)0x233F,   // 1333
(short)0x7B0D,
(short)0x23B5,   // 1334
(short)0x7AEB,
(short)0x242B,   // 1335
(short)0x7AC9,
(short)0x24A0,   // 1336
(short)0x7AA6,
(short)0x2516,   // 1337
(short)0x7A83,
(short)0x258B,   // 1338
(short)0x7A5F,
(short)0x2600,   // 1339
(short)0x7A3B,
(short)0x2675,   // 1340
(short)0x7A16,
(short)0x26EA,   // 1341
(short)0x79F1,
(short)0x275F,   // 1342
(short)0x79CB,
(short)0x27D4,   // 1343
(short)0x79A5,
(short)0x2848,   // 1344
(short)0x797F,
(short)0x28BD,   // 1345
(short)0x7958,
(short)0x2931,   // 1346
(short)0x7931,
(short)0x29A5,   // 1347
(short)0x7909,
(short)0x2A19,   // 1348
(short)0x78E1,
(short)0x2A8D,   // 1349
(short)0x78B8,
(short)0x2B01,   // 1350
(short)0x788F,
(short)0x2B74,   // 1351
(short)0x7866,
(short)0x2BE7,   // 1352
(short)0x783C,
(short)0x2C5B,   // 1353
(short)0x7812,
(short)0x2CCE,   // 1354
(short)0x77E7,
(short)0x2D40,   // 1355
(short)0x77BC,
(short)0x2DB3,   // 1356
(short)0x7790,
(short)0x2E26,   // 1357
(short)0x7764,
(short)0x2E98,   // 1358
(short)0x7738,
(short)0x2F0A,   // 1359
(short)0x770B,
(short)0x2F7C,   // 1360
(short)0x76DE,
(short)0x2FEE,   // 1361
(short)0x76B0,
(short)0x3060,   // 1362
(short)0x7682,
(short)0x30D1,   // 1363
(short)0x7653,
(short)0x3143,   // 1364
(short)0x7624,
(short)0x31B4,   // 1365
(short)0x75F5,
(short)0x3225,   // 1366
(short)0x75C5,
(short)0x3296,   // 1367
(short)0x7595,
(short)0x3306,   // 1368
(short)0x7564,
(short)0x3377,   // 1369
(short)0x7533,
(short)0x33E7,   // 1370
(short)0x7501,
(short)0x3457,   // 1371
(short)0x74CF,
(short)0x34C7,   // 1372
(short)0x749D,
(short)0x3536,   // 1373
(short)0x746A,
(short)0x35A6,   // 1374
(short)0x7437,
(short)0x3615,   // 1375
(short)0x7403,
(short)0x3684,   // 1376
(short)0x73CF,
(short)0x36F3,   // 1377
(short)0x739B,
(short)0x3762,   // 1378
(short)0x7366,
(short)0x37D1,   // 1379
(short)0x7331,
(short)0x383F,   // 1380
(short)0x72FB,
(short)0x38AD,   // 1381
(short)0x72C5,
(short)0x391B,   // 1382
(short)0x728E,
(short)0x3989,   // 1383
(short)0x7257,
(short)0x39F6,   // 1384
(short)0x7220,
(short)0x3A63,   // 1385
(short)0x71E8,
(short)0x3AD0,   // 1386
(short)0x71B0,
(short)0x3B3D,   // 1387
(short)0x7177,
(short)0x3BAA,   // 1388
(short)0x713E,
(short)0x3C16,   // 1389
(short)0x7105,
(short)0x3C83,   // 1390
(short)0x70CB,
(short)0x3CEF,   // 1391
(short)0x7091,
(short)0x3D5A,   // 1392
(short)0x7056,
(short)0x3DC6,   // 1393
(short)0x701B,
(short)0x3E31,   // 1394
(short)0x6FE0,
(short)0x3E9D,   // 1395
(short)0x6FA4,
(short)0x3F07,   // 1396
(short)0x6F68,
(short)0x3F72,   // 1397
(short)0x6F2B,
(short)0x3FDD,   // 1398
(short)0x6EEE,
(short)0x4047,   // 1399
(short)0x6EB1,
(short)0x40B1,   // 1400
(short)0x6E73,
(short)0x411B,   // 1401
(short)0x6E35,
(short)0x4184,   // 1402
(short)0x6DF6,
(short)0x41ED,   // 1403
(short)0x6DB7,
(short)0x4256,   // 1404
(short)0x6D78,
(short)0x42BF,   // 1405
(short)0x6D38,
(short)0x4328,   // 1406
(short)0x6CF8,
(short)0x4390,   // 1407
(short)0x6CB7,
(short)0x43F8,   // 1408
(short)0x6C76,
(short)0x4460,   // 1409
(short)0x6C35,
(short)0x44C8,   // 1410
(short)0x6BF3,
(short)0x452F,   // 1411
(short)0x6BB1,
(short)0x4596,   // 1412
(short)0x6B6F,
(short)0x45FD,   // 1413
(short)0x6B2C,
(short)0x4664,   // 1414
(short)0x6AE9,
(short)0x46CA,   // 1415
(short)0x6AA5,
(short)0x4730,   // 1416
(short)0x6A61,
(short)0x4796,   // 1417
(short)0x6A1C,
(short)0x47FB,   // 1418
(short)0x69D8,
(short)0x4861,   // 1419
(short)0x6992,
(short)0x48C6,   // 1420
(short)0x694D,
(short)0x492B,   // 1421
(short)0x6907,
(short)0x498F,   // 1422
(short)0x68C1,
(short)0x49F4,   // 1423
(short)0x687A,
(short)0x4A58,   // 1424
(short)0x6833,
(short)0x4ABB,   // 1425
(short)0x67EB,
(short)0x4B1F,   // 1426
(short)0x67A3,
(short)0x4B82,   // 1427
(short)0x675B,
(short)0x4BE5,   // 1428
(short)0x6713,
(short)0x4C48,   // 1429
(short)0x66CA,
(short)0x4CAA,   // 1430
(short)0x6680,
(short)0x4D0C,   // 1431
(short)0x6637,
(short)0x4D6E,   // 1432
(short)0x65ED,
(short)0x4DCF,   // 1433
(short)0x65A2,
(short)0x4E31,   // 1434
(short)0x6558,
(short)0x4E92,   // 1435
(short)0x650C,
(short)0x4EF3,   // 1436
(short)0x64C1,
(short)0x4F53,   // 1437
(short)0x6475,
(short)0x4FB3,   // 1438
(short)0x6429,
(short)0x5013,   // 1439
(short)0x63DC,
(short)0x5073,   // 1440
(short)0x638F,
(short)0x50D2,   // 1441
(short)0x6342,
(short)0x5131,   // 1442
(short)0x62F4,
(short)0x5190,   // 1443
(short)0x62A6,
(short)0x51EE,   // 1444
(short)0x6258,
(short)0x524C,   // 1445
(short)0x6209,
(short)0x52AA,   // 1446
(short)0x61BA,
(short)0x5307,   // 1447
(short)0x616B,
(short)0x5365,   // 1448
(short)0x611B,
(short)0x53C2,   // 1449
(short)0x60CB,
(short)0x541E,   // 1450
(short)0x607B,
(short)0x547B,   // 1451
(short)0x602A,
(short)0x54D7,   // 1452
(short)0x5FD9,
(short)0x5532,   // 1453
(short)0x5F87,
(short)0x558E,   // 1454
(short)0x5F35,
(short)0x55E9,   // 1455
(short)0x5EE3,
(short)0x5644,   // 1456
(short)0x5E91,
(short)0x569E,   // 1457
(short)0x5E3E,
(short)0x56F8,   // 1458
(short)0x5DEA,
(short)0x5752,   // 1459
(short)0x5D97,
(short)0x57AC,   // 1460
(short)0x5D43,
(short)0x5805,   // 1461
(short)0x5CEF,
(short)0x585E,   // 1462
(short)0x5C9A,
(short)0x58B7,   // 1463
(short)0x5C45,
(short)0x590F,   // 1464
(short)0x5BF0,
(short)0x5967,   // 1465
(short)0x5B9B,
(short)0x59BE,   // 1466
(short)0x5B45,
(short)0x5A16,   // 1467
(short)0x5AEF,
(short)0x5A6D,   // 1468
(short)0x5A98,
(short)0x5AC3,   // 1469
(short)0x5A41,
(short)0x5B1A,   // 1470
(short)0x59EA,
(short)0x5B70,   // 1471
(short)0x5993,
(short)0x5BC6,   // 1472
(short)0x593B,
(short)0x5C1B,   // 1473
(short)0x58E3,
(short)0x5C70,   // 1474
(short)0x588A,
(short)0x5CC5,   // 1475
(short)0x5832,
(short)0x5D19,   // 1476
(short)0x57D8,
(short)0x5D6D,   // 1477
(short)0x577F,
(short)0x5DC1,   // 1478
(short)0x5725,
(short)0x5E14,   // 1479
(short)0x56CB,
(short)0x5E67,   // 1480
(short)0x5671,
(short)0x5EBA,   // 1481
(short)0x5616,
(short)0x5F0C,   // 1482
(short)0x55BB,
(short)0x5F5E,   // 1483
(short)0x5560,
(short)0x5FB0,   // 1484
(short)0x5505,
(short)0x6001,   // 1485
(short)0x54A9,
(short)0x6052,   // 1486
(short)0x544C,
(short)0x60A3,   // 1487
(short)0x53F0,
(short)0x60F3,   // 1488
(short)0x5393,
(short)0x6143,   // 1489
(short)0x5336,
(short)0x6193,   // 1490
(short)0x52D9,
(short)0x61E2,   // 1491
(short)0x527B,
(short)0x6231,   // 1492
(short)0x521D,
(short)0x627F,   // 1493
(short)0x51BF,
(short)0x62CD,   // 1494
(short)0x5160,
(short)0x631B,   // 1495
(short)0x5101,
(short)0x6369,   // 1496
(short)0x50A2,
(short)0x63B6,   // 1497
(short)0x5043,
(short)0x6403,   // 1498
(short)0x4FE3,
(short)0x644F,   // 1499
(short)0x4F83,
(short)0x649B,   // 1500
(short)0x4F23,
(short)0x64E7,   // 1501
(short)0x4EC2,
(short)0x6532,   // 1502
(short)0x4E61,
(short)0x657D,   // 1503
(short)0x4E00,
(short)0x65C8,   // 1504
(short)0x4D9F,
(short)0x6612,   // 1505
(short)0x4D3D,
(short)0x665C,   // 1506
(short)0x4CDB,
(short)0x66A5,   // 1507
(short)0x4C79,
(short)0x66EE,   // 1508
(short)0x4C16,
(short)0x6737,   // 1509
(short)0x4BB3,
(short)0x677F,   // 1510
(short)0x4B50,
(short)0x67C7,   // 1511
(short)0x4AED,
(short)0x680F,   // 1512
(short)0x4A89,
(short)0x6856,   // 1513
(short)0x4A26,
(short)0x689D,   // 1514
(short)0x49C1,
(short)0x68E4,   // 1515
(short)0x495D,
(short)0x692A,   // 1516
(short)0x48F8,
(short)0x6970,   // 1517
(short)0x4893,
(short)0x69B5,   // 1518
(short)0x482E,
(short)0x69FA,   // 1519
(short)0x47C9,
(short)0x6A3F,   // 1520
(short)0x4763,
(short)0x6A83,   // 1521
(short)0x46FD,
(short)0x6AC7,   // 1522
(short)0x4697,
(short)0x6B0A,   // 1523
(short)0x4630,
(short)0x6B4D,   // 1524
(short)0x45CA,
(short)0x6B90,   // 1525
(short)0x4563,
(short)0x6BD2,   // 1526
(short)0x44FB,
(short)0x6C14,   // 1527
(short)0x4494,
(short)0x6C56,   // 1528
(short)0x442C,
(short)0x6C97,   // 1529
(short)0x43C4,
(short)0x6CD8,   // 1530
(short)0x435C,
(short)0x6D18,   // 1531
(short)0x42F4,
(short)0x6D58,   // 1532
(short)0x428B,
(short)0x6D98,   // 1533
(short)0x4222,
(short)0x6DD7,   // 1534
(short)0x41B9,
(short)0x6E16,   // 1535
(short)0x414F,
(short)0x6E54,   // 1536
(short)0x40E6,
(short)0x6E92,   // 1537
(short)0x407C,
(short)0x6ED0,   // 1538
(short)0x4012,
(short)0x6F0D,   // 1539
(short)0x3FA7,
(short)0x6F4A,   // 1540
(short)0x3F3D,
(short)0x6F86,   // 1541
(short)0x3ED2,
(short)0x6FC2,   // 1542
(short)0x3E67,
(short)0x6FFE,   // 1543
(short)0x3DFC,
(short)0x7039,   // 1544
(short)0x3D90,
(short)0x7074,   // 1545
(short)0x3D25,
(short)0x70AE,   // 1546
(short)0x3CB9,
(short)0x70E8,   // 1547
(short)0x3C4D,
(short)0x7122,   // 1548
(short)0x3BE0,
(short)0x715B,   // 1549
(short)0x3B74,
(short)0x7194,   // 1550
(short)0x3B07,
(short)0x71CC,   // 1551
(short)0x3A9A,
(short)0x7204,   // 1552
(short)0x3A2D,
(short)0x723C,   // 1553
(short)0x39BF,
(short)0x7273,   // 1554
(short)0x3952,
(short)0x72AA,   // 1555
(short)0x38E4,
(short)0x72E0,   // 1556
(short)0x3876,
(short)0x7316,   // 1557
(short)0x3808,
(short)0x734B,   // 1558
(short)0x3799,
(short)0x7380,   // 1559
(short)0x372B,
(short)0x73B5,   // 1560
(short)0x36BC,
(short)0x73E9,   // 1561
(short)0x364D,
(short)0x741D,   // 1562
(short)0x35DE,
(short)0x7451,   // 1563
(short)0x356E,
(short)0x7484,   // 1564
(short)0x34FF,
(short)0x74B6,   // 1565
(short)0x348F,
(short)0x74E8,   // 1566
(short)0x341F,
(short)0x751A,   // 1567
(short)0x33AF,
(short)0x754B,   // 1568
(short)0x333E,
(short)0x757C,   // 1569
(short)0x32CE,
(short)0x75AD,   // 1570
(short)0x325D,
(short)0x75DD,   // 1571
(short)0x31EC,
(short)0x760D,   // 1572
(short)0x317B,
(short)0x763C,   // 1573
(short)0x310A,
(short)0x766B,   // 1574
(short)0x3099,
(short)0x7699,   // 1575
(short)0x3027,
(short)0x76C7,   // 1576
(short)0x2FB5,
(short)0x76F4,   // 1577
(short)0x2F43,
(short)0x7722,   // 1578
(short)0x2ED1,
(short)0x774E,   // 1579
(short)0x2E5F,
(short)0x777A,   // 1580
(short)0x2DEC,
(short)0x77A6,   // 1581
(short)0x2D7A,
(short)0x77D2,   // 1582
(short)0x2D07,
(short)0x77FD,   // 1583
(short)0x2C94,
(short)0x7827,   // 1584
(short)0x2C21,
(short)0x7851,   // 1585
(short)0x2BAE,
(short)0x787B,   // 1586
(short)0x2B3A,
(short)0x78A4,   // 1587
(short)0x2AC7,
(short)0x78CD,   // 1588
(short)0x2A53,
(short)0x78F5,   // 1589
(short)0x29DF,
(short)0x791D,   // 1590
(short)0x296B,
(short)0x7945,   // 1591
(short)0x28F7,
(short)0x796C,   // 1592
(short)0x2883,
(short)0x7992,   // 1593
(short)0x280E,
(short)0x79B8,   // 1594
(short)0x279A,
(short)0x79DE,   // 1595
(short)0x2725,
(short)0x7A03,   // 1596
(short)0x26B0,
(short)0x7A28,   // 1597
(short)0x263B,
(short)0x7A4D,   // 1598
(short)0x25C6,
(short)0x7A71,   // 1599
(short)0x2550,
(short)0x7A94,   // 1600
(short)0x24DB,
(short)0x7AB7,   // 1601
(short)0x2465,
(short)0x7ADA,   // 1602
(short)0x23F0,
(short)0x7AFC,   // 1603
(short)0x237A,
(short)0x7B1E,   // 1604
(short)0x2304,
(short)0x7B3F,   // 1605
(short)0x228E,
(short)0x7B60,   // 1606
(short)0x2218,
(short)0x7B81,   // 1607
(short)0x21A1,
(short)0x7BA1,   // 1608
(short)0x212B,
(short)0x7BC0,   // 1609
(short)0x20B4,
(short)0x7BE0,   // 1610
(short)0x203E,
(short)0x7BFE,   // 1611
(short)0x1FC7,
(short)0x7C1C,   // 1612
(short)0x1F50,
(short)0x7C3A,   // 1613
(short)0x1ED9,
(short)0x7C58,   // 1614
(short)0x1E62,
(short)0x7C74,   // 1615
(short)0x1DEA,
(short)0x7C91,   // 1616
(short)0x1D73,
(short)0x7CAD,   // 1617
(short)0x1CFC,
(short)0x7CC8,   // 1618
(short)0x1C84,
(short)0x7CE4,   // 1619
(short)0x1C0C,
(short)0x7CFE,   // 1620
(short)0x1B95,
(short)0x7D18,   // 1621
(short)0x1B1D,
(short)0x7D32,   // 1622
(short)0x1AA5,
(short)0x7D4C,   // 1623
(short)0x1A2D,
(short)0x7D64,   // 1624
(short)0x19B5,
(short)0x7D7D,   // 1625
(short)0x193C,
(short)0x7D95,   // 1626
(short)0x18C4,
(short)0x7DAC,   // 1627
(short)0x184C,
(short)0x7DC3,   // 1628
(short)0x17D3,
(short)0x7DDA,   // 1629
(short)0x175B,
(short)0x7DF0,   // 1630
(short)0x16E2,
(short)0x7E06,   // 1631
(short)0x1669,
(short)0x7E1B,   // 1632
(short)0x15F0,
(short)0x7E30,   // 1633
(short)0x1577,
(short)0x7E44,   // 1634
(short)0x14FE,
(short)0x7E58,   // 1635
(short)0x1485,
(short)0x7E6C,   // 1636
(short)0x140C,
(short)0x7E7F,   // 1637
(short)0x1393,
(short)0x7E91,   // 1638
(short)0x131A,
(short)0x7EA3,   // 1639
(short)0x12A0,
(short)0x7EB5,   // 1640
(short)0x1227,
(short)0x7EC6,   // 1641
(short)0x11AD,
(short)0x7ED7,   // 1642
(short)0x1134,
(short)0x7EE7,   // 1643
(short)0x10BA,
(short)0x7EF7,   // 1644
(short)0x1040,
(short)0x7F06,   // 1645
(short)0x0FC7,
(short)0x7F15,   // 1646
(short)0x0F4D,
(short)0x7F23,   // 1647
(short)0x0ED3,
(short)0x7F31,   // 1648
(short)0x0E59,
(short)0x7F3F,   // 1649
(short)0x0DDF,
(short)0x7F4C,   // 1650
(short)0x0D65,
(short)0x7F59,   // 1651
(short)0x0CEB,
(short)0x7F65,   // 1652
(short)0x0C71,
(short)0x7F71,   // 1653
(short)0x0BF7,
(short)0x7F7C,   // 1654
(short)0x0B7D,
(short)0x7F87,   // 1655
(short)0x0B03,
(short)0x7F91,   // 1656
(short)0x0A88,
(short)0x7F9B,   // 1657
(short)0x0A0E,
(short)0x7FA4,   // 1658
(short)0x0994,
(short)0x7FAD,   // 1659
(short)0x0919,
(short)0x7FB6,   // 1660
(short)0x089F,
(short)0x7FBE,   // 1661
(short)0x0824,
(short)0x7FC5,   // 1662
(short)0x07AA,
(short)0x7FCC,   // 1663
(short)0x0730,
(short)0x7FD3,   // 1664
(short)0x06B5,
(short)0x7FD9,   // 1665
(short)0x063A,
(short)0x7FDF,   // 1666
(short)0x05C0,
(short)0x7FE4,   // 1667
(short)0x0545,
(short)0x7FE9,   // 1668
(short)0x04CB,
(short)0x7FED,   // 1669
(short)0x0450,
(short)0x7FF1,   // 1670
(short)0x03D5,
(short)0x7FF5,   // 1671
(short)0x035B,
(short)0x7FF8,   // 1672
(short)0x02E0,
(short)0x7FFA,   // 1673
(short)0x0265,
(short)0x7FFC,   // 1674
(short)0x01EB,
(short)0x7FFE,   // 1675
(short)0x0170,
(short)0x7FFF,   // 1676
(short)0x00F5,
(short)0x7FFF,   // 1677
(short)0x007B,
(short)0x0000,
(short)0x0000,
(short)0x0000,
(short)0x0000,
//};
//
//S16 ZC839DFTk0[1680] = {   // x32
(short)0x0000,   //   0
(short)0x0000,
(short)0x5201,   //   1
(short)0xAE26,
(short)0x73DD,   //   2
(short)0x0038,
(short)0x51B2,   //   3
(short)0x5228,
(short)0xFF91,   //   4
(short)0x73DC,
(short)0xADB1,   //   5
(short)0x518B,
(short)0x8C24,   //   6
(short)0xFF59,
(short)0xAE9D,   //   7
(short)0xAD8A,
(short)0x00DE,   //   8
(short)0x8C24,
(short)0x529D,   //   9
(short)0xAEC4,
(short)0x73DB,   //  10
(short)0x0116,
(short)0xAEEC,   //  11
(short)0xAD3C,
(short)0xFEB3,   //  12
(short)0x73DB,
(short)0x52EB,   //  13
(short)0xAF14,
(short)0x8C26,   //  14
(short)0xFE7B,
(short)0xAF3B,   //  15
(short)0xACEE,
(short)0x01BC,   //  16
(short)0x8C27,
(short)0xACC8,   //  17
(short)0x509D,
(short)0x73D8,   //  18
(short)0x01F4,
(short)0x5075,   //  19
(short)0x535F,
(short)0xFDD5,   //  20
(short)0x73D7,
(short)0xAC7B,   //  21
(short)0x504D,
(short)0x73D6,   //  22
(short)0x0263,
(short)0xAFDB,   //  23
(short)0xAC54,
(short)0x029A,   //  24
(short)0x8C2B,
(short)0x53D2,   //  25
(short)0xB003,
(short)0x8C2C,   //  26
(short)0xFD2E,
(short)0x4FD5,   //  27
(short)0x53F9,
(short)0xFCF7,   //  28
(short)0x73D2,
(short)0x541F,   //  29
(short)0xB054,
(short)0x8C2F,   //  30
(short)0xFCBF,
(short)0x4F84,   //  31
(short)0x5445,
(short)0x0378,   //  32
(short)0x8C31,
(short)0xAB95,   //  33
(short)0x4F5B,
(short)0x8C32,   //  34
(short)0xFC50,
(short)0x4F33,   //  35
(short)0x5491,
(short)0xFC19,   //  36
(short)0x73CC,
(short)0xAB49,   //  37
(short)0x4F0A,
(short)0x8C36,   //  38
(short)0xFBE1,
(short)0x4EE2,   //  39
(short)0x54DD,
(short)0x0456,   //  40
(short)0x8C38,
(short)0xAAFD,   //  41
(short)0x4EB9,
(short)0x73C6,   //  42
(short)0x048E,
(short)0x4E90,   //  43
(short)0x5528,
(short)0x04C5,   //  44
(short)0x8C3C,
(short)0xAAB2,   //  45
(short)0x4E67,
(short)0x8C3F,   //  46
(short)0xFB03,
(short)0xB1C2,   //  47
(short)0xAA8D,
(short)0x0534,   //  48
(short)0x8C41,
(short)0x5599,   //  49
(short)0xB1EA,
(short)0x73BC,   //  50
(short)0x056C,
(short)0xB214,   //  51
(short)0xAA42,
(short)0x05A3,   //  52
(short)0x8C46,
(short)0xAA1C,   //  53
(short)0x4DC3,
(short)0x8C49,   //  54
(short)0xFA25,
(short)0x4D9A,   //  55
(short)0x5609,
(short)0x0612,   //  56
(short)0x8C4C,
(short)0x562E,   //  57
(short)0xB28F,
(short)0x8C4F,   //  58
(short)0xF9B6,
(short)0x4D48,   //  59
(short)0x5653,
(short)0xF97F,   //  60
(short)0x73AE,
(short)0xA988,   //  61
(short)0x4D1E,
(short)0x73AB,   //  62
(short)0x06B9,
(short)0xB30B,   //  63
(short)0xA963,
(short)0x06F0,   //  64
(short)0x8C59,
(short)0xA93E,   //  65
(short)0x4CCB,
(short)0x8C5C,   //  66
(short)0xF8D9,
(short)0xB35F,   //  67
(short)0xA919,
(short)0x075F,   //  68
(short)0x8C5F,
(short)0xA8F5,   //  69
(short)0x4C78,
(short)0x8C63,   //  70
(short)0xF86A,
(short)0x4C4E,   //  71
(short)0x5730,
(short)0x07CE,   //  72
(short)0x8C67,
(short)0x5754,   //  73
(short)0xB3DC,
(short)0x7396,   //  74
(short)0x0805,
(short)0x4BFA,   //  75
(short)0x5779,
(short)0xF7C4,   //  76
(short)0x7392,
(short)0x579D,   //  77
(short)0xB430,
(short)0x738E,   //  78
(short)0x0874,
(short)0x4BA6,   //  79
(short)0x57C2,
(short)0x08AB,   //  80
(short)0x8C76,
(short)0x57E6,   //  81
(short)0xB484,
(short)0x8C7B,   //  82
(short)0xF71D,
(short)0xB4AE,   //  83
(short)0xA7F6,
(short)0xF6E6,   //  84
(short)0x7381,
(short)0x582E,   //  85
(short)0xB4D8,
(short)0x8C83,   //  86
(short)0xF6AF,
(short)0x4AFE,   //  87
(short)0x5852,
(short)0xF677,   //  88
(short)0x7378,
(short)0xA78A,   //  89
(short)0x4AD3,
(short)0x7373,   //  90
(short)0x09C0,
(short)0xB557,   //  91
(short)0xA766,
(short)0xF609,   //  92
(short)0x736F,
(short)0x58BD,   //  93
(short)0xB582,
(short)0x8C96,   //  94
(short)0xF5D1,
(short)0xB5AC,   //  95
(short)0xA71F,
(short)0x0A66,   //  96
(short)0x8C9B,
(short)0xA6FB,   //  97
(short)0x4A29,
(short)0x7360,   //  98
(short)0x0A9D,
(short)0xB602,   //  99
(short)0xA6D8,
(short)0xF52B,   // 100
(short)0x735B,
(short)0xA6B4,   // 101
(short)0x49D4,
(short)0x7356,   // 102
(short)0x0B0C,
(short)0x49A9,   // 103
(short)0x596F,
(short)0xF4BD,   // 104
(short)0x7350,
(short)0x5992,   // 105
(short)0xB682,
(short)0x734B,   // 106
(short)0x0B7A,
(short)0xB6AD,   // 107
(short)0xA64B,
(short)0xF44E,   // 108
(short)0x7345,
(short)0x59D8,   // 109
(short)0xB6D8,
(short)0x7340,   // 110
(short)0x0BE9,
(short)0xB703,   // 111
(short)0xA604,
(short)0x0C20,   // 112
(short)0x8CC6,
(short)0xA5E2,   // 113
(short)0x48D2,
(short)0x7334,   // 114
(short)0x0C57,
(short)0x48A6,   // 115
(short)0x5A41,
(short)0x0C8E,   // 116
(short)0x8CD2,
(short)0x5A64,   // 117
(short)0xB785,
(short)0x8CD8,   // 118
(short)0xF33A,
(short)0x4850,   // 119
(short)0x5A87,
(short)0x0CFD,   // 120
(short)0x8CDE,
(short)0x5AA9,   // 121
(short)0xB7DC,
(short)0x731B,   // 122
(short)0x0D34,
(short)0xB807,   // 123
(short)0xA534,
(short)0x0D6B,   // 124
(short)0x8CEB,
(short)0xA512,   // 125
(short)0x47CD,
(short)0x8CF1,   // 126
(short)0xF25E,
(short)0xB85E,   // 127
(short)0xA4EF,
(short)0x0DDA,   // 128
(short)0x8CF8,
(short)0x5B33,   // 129
(short)0xB88A,
(short)0x8CFF,   // 130
(short)0xF1EF,
(short)0xB8B6,   // 131
(short)0xA4AB,
(short)0x0E48,   // 132
(short)0x8D05,
(short)0xA489,   // 133
(short)0x471E,
(short)0x72F4,   // 134
(short)0x0E7F,
(short)0xB90D,   // 135
(short)0xA467,
(short)0xF14A,   // 136
(short)0x72ED,
(short)0x5BBB,   // 137
(short)0xB939,
(short)0x72E6,   // 138
(short)0x0EED,
(short)0xB965,   // 139
(short)0xA423,
(short)0xF0DC,   // 140
(short)0x72DE,
(short)0xA401,   // 141
(short)0x466F,
(short)0x72D7,   // 142
(short)0x0F5B,
(short)0xB9BE,   // 143
(short)0xA3DF,
(short)0x0F92,   // 144
(short)0x8D30,
(short)0xA3BE,   // 145
(short)0x4616,
(short)0x72C8,   // 146
(short)0x0FC9,
(short)0x45EA,   // 147
(short)0x5C64,
(short)0xF000,   // 148
(short)0x72C0,
(short)0x5C85,   // 149
(short)0xBA42,
(short)0x8D47,   // 150
(short)0xEFC9,
(short)0x4591,   // 151
(short)0x5CA7,
(short)0x106E,   // 152
(short)0x8D4F,
(short)0xA338,   // 153
(short)0x4565,
(short)0x8D57,   // 154
(short)0xEF5B,
(short)0xBAC8,   // 155
(short)0xA317,
(short)0x10DC,   // 156
(short)0x8D5F,
(short)0x5D0B,   // 157
(short)0xBAF4,
(short)0x7299,   // 158
(short)0x1113,
(short)0xBB21,   // 159
(short)0xA2D4,
(short)0x114A,   // 160
(short)0x8D6F,
(short)0x5D4D,   // 161
(short)0xBB4E,
(short)0x7288,   // 162
(short)0x1181,
(short)0xBB7A,   // 163
(short)0xA293,
(short)0x11B8,   // 164
(short)0x8D80,
(short)0x5D8E,   // 165
(short)0xBBA7,
(short)0x7277,   // 166
(short)0x11EF,
(short)0x442C,   // 167
(short)0x5DAF,
(short)0x1225,   // 168
(short)0x8D91,
(short)0x5DD0,   // 169
(short)0xBC01,
(short)0x8D9A,   // 170
(short)0xEDA4,
(short)0x43D2,   // 171
(short)0x5DF0,
(short)0xED6D,   // 172
(short)0x725D,
(short)0x5E11,   // 173
(short)0xBC5B,
(short)0x7254,   // 174
(short)0x12CA,
(short)0xBC88,   // 175
(short)0xA1CF,
(short)0xECFF,   // 176
(short)0x724B,
(short)0x5E51,   // 177
(short)0xBCB5,
(short)0x8DBE,   // 178
(short)0xECC9,
(short)0x431E,   // 179
(short)0x5E71,
(short)0xEC92,   // 180
(short)0x7239,
(short)0x5E92,   // 181
(short)0xBD10,
(short)0x722F,   // 182
(short)0x13A5,
(short)0xBD3D,   // 183
(short)0xA14E,
(short)0x13DC,   // 184
(short)0x8DDA,
(short)0x5ED2,   // 185
(short)0xBD6B,
(short)0x8DE4,   // 186
(short)0xEBEE,
(short)0x4268,   // 187
(short)0x5EF1,
(short)0xEBB7,   // 188
(short)0x7213,
(short)0xA0EF,   // 189
(short)0x423A,
(short)0x8DF7,   // 190
(short)0xEB80,
(short)0x420D,   // 191
(short)0x5F31,
(short)0x14B6,   // 192
(short)0x8E01,
(short)0x5F51,   // 193
(short)0xBE21,
(short)0x8E0B,   // 194
(short)0xEB13,
(short)0xBE4E,   // 195
(short)0xA090,
(short)0xEADC,   // 196
(short)0x71EB,
(short)0xA070,   // 197
(short)0x4184,
(short)0x71E1,   // 198
(short)0x155A,
(short)0x4156,   // 199
(short)0x5FAF,
(short)0x1591,   // 200
(short)0x8E2A,
(short)0xA032,   // 201
(short)0x4128,
(short)0x71CC,   // 202
(short)0x15C7,
(short)0xBF06,   // 203
(short)0xA013,
(short)0x15FE,   // 204
(short)0x8E3F,
(short)0x600C,   // 205
(short)0xBF34,
(short)0x71B7,   // 206
(short)0x1634,
(short)0xBF62,   // 207
(short)0x9FD5,
(short)0xE995,   // 208
(short)0x71AC,
(short)0x9FB6,   // 209
(short)0x4070,
(short)0x71A1,   // 210
(short)0x16A1,
(short)0x4042,   // 211
(short)0x6069,
(short)0xE928,   // 212
(short)0x7196,
(short)0x6088,   // 213
(short)0xBFED,
(short)0x718B,   // 214
(short)0x170E,
(short)0xC01B,   // 215
(short)0x9F59,
(short)0x1745,   // 216
(short)0x8E80,
(short)0x9F3B,   // 217
(short)0x3FB7,
(short)0x8E8B,   // 218
(short)0xE885,
(short)0x3F88,   // 219
(short)0x60E4,
(short)0x17B1,   // 220
(short)0x8E96,
(short)0x9EFE,   // 221
(short)0x3F5A,
(short)0x8EA2,   // 222
(short)0xE818,
(short)0xC0D5,   // 223
(short)0x9EE0,
(short)0x181E,   // 224
(short)0x8EAD,
(short)0x613F,   // 225
(short)0xC103,
(short)0x8EB9,   // 226
(short)0xE7AC,
(short)0x3ECE,   // 227
(short)0x615D,
(short)0xE775,   // 228
(short)0x713C,
(short)0x617B,   // 229
(short)0xC160,
(short)0x8ED0,   // 230
(short)0xE73F,
(short)0x3E71,   // 231
(short)0x6199,
(short)0xE709,   // 232
(short)0x7124,
(short)0x9E49,   // 233
(short)0x3E42,
(short)0x8EE8,   // 234
(short)0xE6D3,
(short)0x3E13,   // 235
(short)0x61D5,
(short)0xE69C,   // 236
(short)0x710C,
(short)0x61F2,   // 237
(short)0xC21C,
(short)0x7100,   // 238
(short)0x199A,
(short)0x3DB5,   // 239
(short)0x6210,
(short)0x19D0,   // 240
(short)0x8F0D,
(short)0x622D,   // 241
(short)0xC27A,
(short)0x70E7,   // 242
(short)0x1A06,
(short)0x3D57,   // 243
(short)0x624B,
(short)0xE5C4,   // 244
(short)0x70DA,
(short)0x9D98,   // 245
(short)0x3D28,
(short)0x70CE,   // 246
(short)0x1A72,
(short)0x3CF9,   // 247
(short)0x6285,
(short)0xE558,   // 248
(short)0x70C1,
(short)0x9D5D,   // 249
(short)0x3CCA,
(short)0x70B4,   // 250
(short)0x1ADE,
(short)0xC366,   // 251
(short)0x9D40,
(short)0xE4EC,   // 252
(short)0x70A7,
(short)0x62DD,   // 253
(short)0xC395,
(short)0x8F66,   // 254
(short)0xE4B6,
(short)0x3C3B,   // 255
(short)0x62FA,
(short)0x1B80,   // 256
(short)0x8F73,
(short)0x6316,   // 257
(short)0xC3F4,
(short)0x7080,   // 258
(short)0x1BB6,
(short)0x3BDC,   // 259
(short)0x6333,
(short)0x1BEC,   // 260
(short)0x8F8E,
(short)0x6350,   // 261
(short)0xC453,
(short)0x7065,   // 262
(short)0x1C22,
(short)0x3B7D,   // 263
(short)0x636C,
(short)0xE3A8,   // 264
(short)0x7058,
(short)0x6389,   // 265
(short)0xC4B2,
(short)0x704A,   // 266
(short)0x1C8E,
(short)0xC4E2,   // 267
(short)0x9C5B,
(short)0x1CC3,   // 268
(short)0x8FC4,
(short)0x63C2,   // 269
(short)0xC512,
(short)0x8FD2,   // 270
(short)0xE307,
(short)0xC542,   // 271
(short)0x9C22,
(short)0xE2D1,   // 272
(short)0x7020,
(short)0x9C06,   // 273
(short)0x3A8E,
(short)0x7012,   // 274
(short)0x1D65,
(short)0xC5A2,   // 275
(short)0x9BEA,
(short)0xE266,   // 276
(short)0x7004,
(short)0x6432,   // 277
(short)0xC5D2,
(short)0x6FF6,   // 278
(short)0x1DD0,
(short)0x39FE,   // 279
(short)0x644E,
(short)0x1E06,   // 280
(short)0x9018,
(short)0x6469,   // 281
(short)0xC632,
(short)0x6FD9,   // 282
(short)0x1E3B,
(short)0x399E,   // 283
(short)0x6485,
(short)0x1E71,   // 284
(short)0x9035,
(short)0x9B5F,   // 285
(short)0x396E,
(short)0x9044,   // 286
(short)0xE15A,
(short)0x393E,   // 287
(short)0x64BC,
(short)0x1EDC,   // 288
(short)0x9053,
(short)0x64D7,   // 289
(short)0xC6F3,
(short)0x9062,   // 290
(short)0xE0EE,
(short)0xC723,   // 291
(short)0x9B0D,
(short)0xE0B9,   // 292
(short)0x6F8F,
(short)0x9AF2,   // 293
(short)0x38AD,
(short)0x9080,   // 294
(short)0xE084,
(short)0xC784,   // 295
(short)0x9AD7,
(short)0x1FB2,   // 296
(short)0x908F,
(short)0x9ABC,   // 297
(short)0x384C,
(short)0x909E,   // 298
(short)0xE019,
(short)0xC7E5,   // 299
(short)0x9AA1,
(short)0xDFE3,   // 300
(short)0x6F53,
(short)0x9A86,   // 301
(short)0x37EA,
(short)0x6F43,   // 302
(short)0x2052,
(short)0xC846,   // 303
(short)0x9A6B,
(short)0x2087,   // 304
(short)0x90CC,
(short)0x65AF,   // 305
(short)0xC877,
(short)0x90DC,   // 306
(short)0xDF43,
(short)0xC8A8,   // 307
(short)0x9A36,
(short)0x20F2,   // 308
(short)0x90EC,
(short)0x65E4,   // 309
(short)0xC8D9,
(short)0x6F05,   // 310
(short)0x2127,
(short)0xC909,   // 311
(short)0x9A01,
(short)0xDEA4,   // 312
(short)0x6EF5,
(short)0x6619,   // 313
(short)0xC93A,
(short)0x911B,   // 314
(short)0xDE6F,
(short)0x3695,   // 315
(short)0x6633,
(short)0x21C7,   // 316
(short)0x912C,
(short)0x99B3,   // 317
(short)0x3664,
(short)0x913C,   // 318
(short)0xDE04,
(short)0xC9CD,   // 319
(short)0x9999,
(short)0x2231,   // 320
(short)0x914C,
(short)0x997F,   // 321
(short)0x3602,
(short)0x6EA3,   // 322
(short)0x2266,
(short)0xCA30,   // 323
(short)0x9965,
(short)0xDD65,   // 324
(short)0x6E93,
(short)0x66B5,   // 325
(short)0xCA61,
(short)0x6E82,   // 326
(short)0x22D0,
(short)0x356E,   // 327
(short)0x66CF,
(short)0xDCFB,   // 328
(short)0x6E71,
(short)0x66E8,   // 329
(short)0xCAC3,
(short)0x919F,   // 330
(short)0xDCC6,
(short)0xCAF5,   // 331
(short)0x98FE,
(short)0x236F,   // 332
(short)0x91B0,
(short)0x98E5,   // 333
(short)0x34DA,
(short)0x6E3F,   // 334
(short)0x23A3,
(short)0x34A9,   // 335
(short)0x6734,
(short)0x23D8,   // 336
(short)0x91D2,
(short)0x674D,   // 337
(short)0xCB89,
(short)0x6E1C,   // 338
(short)0x240D,
(short)0xCBBB,   // 339
(short)0x9899,
(short)0x2442,   // 340
(short)0x91F5,
(short)0x9880,   // 341
(short)0x3414,
(short)0x9206,   // 342
(short)0xDB8A,
(short)0xCC1E,   // 343
(short)0x9868,
(short)0x24AB,   // 344
(short)0x9218,
(short)0x67B1,   // 345
(short)0xCC4F,
(short)0x922A,   // 346
(short)0xDB20,
(short)0x337F,   // 347
(short)0x67CA,
(short)0x2514,   // 348
(short)0x923B,
(short)0x67E3,   // 349
(short)0xCCB3,
(short)0x924D,   // 350
(short)0xDAB7,
(short)0x331B,   // 351
(short)0x67FB,
(short)0xDA82,   // 352
(short)0x6DA1,
(short)0x97EC,   // 353
(short)0x32E9,
(short)0x6D8F,   // 354
(short)0x25B2,
(short)0xCD48,   // 355
(short)0x97D4,
(short)0x25E7,   // 356
(short)0x9283,
(short)0x6844,   // 357
(short)0xCD7A,
(short)0x9295,   // 358
(short)0xD9E5,
(short)0x3254,   // 359
(short)0x685C,
(short)0x2650,   // 360
(short)0x92A8,
(short)0x6875,   // 361
(short)0xCDDE,
(short)0x92BA,   // 362
(short)0xD97C,
(short)0x31EF,   // 363
(short)0x688D,
(short)0x26B8,   // 364
(short)0x92CD,
(short)0x975C,   // 365
(short)0x31BD,
(short)0x92DF,   // 366
(short)0xD913,
(short)0x318B,   // 367
(short)0x68BC,
(short)0x2721,   // 368
(short)0x92F2,
(short)0x972C,   // 369
(short)0x3159,
(short)0x6CFB,   // 370
(short)0x2755,
(short)0x3127,   // 371
(short)0x68EB,
(short)0x2789,   // 372
(short)0x9318,
(short)0x96FD,   // 373
(short)0x30F4,
(short)0x932B,   // 374
(short)0xD842,
(short)0xCF3E,   // 375
(short)0x96E6,
(short)0x27F2,   // 376
(short)0x933E,
(short)0x6932,   // 377
(short)0xCF70,
(short)0x6CAF,   // 378
(short)0x2826,
(short)0xCFA3,   // 379
(short)0x96B7,
(short)0xD7A6,   // 380
(short)0x6C9C,
(short)0x96A0,   // 381
(short)0x302B,
(short)0x6C88,   // 382
(short)0x288E,
(short)0x2FF8,   // 383
(short)0x6977,
(short)0x28C2,   // 384
(short)0x938B,
(short)0x9672,   // 385
(short)0x2FC6,
(short)0x6C61,   // 386
(short)0x28F6,
(short)0x2F93,   // 387
(short)0x69A5,
(short)0x292A,   // 388
(short)0x93B2,
(short)0x69BC,   // 389
(short)0xD0A0,
(short)0x6C3A,   // 390
(short)0x295E,
(short)0x2F2E,   // 391
(short)0x69D2,
(short)0x2992,   // 392
(short)0x93DA,
(short)0x9617,   // 393
(short)0x2EFB,
(short)0x6C12,   // 394
(short)0x29C5,
(short)0xD138,   // 395
(short)0x9601,
(short)0x29F9,   // 396
(short)0x9402,
(short)0x6A16,   // 397
(short)0xD16B,
(short)0x6BEA,   // 398
(short)0x2A2D,
(short)0xD19E,   // 399
(short)0x95D4,
(short)0x2A61,   // 400
(short)0x942A,
(short)0x95BE,   // 401
(short)0x2E2F,
(short)0x943F,   // 402
(short)0xD56C,
(short)0x2DFD,   // 403
(short)0x6A58,
(short)0xD538,   // 404
(short)0x6BAD,
(short)0x9592,   // 405
(short)0x2DCA,
(short)0x6B98,   // 406
(short)0x2AFB,
(short)0x2D97,   // 407
(short)0x6A84,
(short)0xD4D1,   // 408
(short)0x6B83,
(short)0x9566,   // 409
(short)0x2D63,
(short)0x9491,   // 410
(short)0xD49D,
(short)0x2D30,   // 411
(short)0x6AB0,
(short)0x2B96,   // 412
(short)0x94A6,
(short)0x953B,   // 413
(short)0x2CFD,
(short)0x94BB,   // 414
(short)0xD437,
(short)0x2CCA,   // 415
(short)0x6ADB,
(short)0xD403,   // 416
(short)0x6B30,
(short)0x9510,   // 417
(short)0x2C97,
(short)0x94E5,   // 418
(short)0xD3D0,
(short)0xD39D,   // 419
(short)0x94FA,
(short)0xD39D,   // 420
(short)0x6B06,
(short)0x94E5,   // 421
(short)0x2C30,
(short)0x9510,   // 422
(short)0xD369,
(short)0xD403,   // 423
(short)0x94D0,
(short)0x2CCA,   // 424
(short)0x9525,
(short)0x94BB,   // 425
(short)0x2BC9,
(short)0x953B,   // 426
(short)0xD303,
(short)0x2B96,   // 427
(short)0x6B5A,
(short)0x2D30,   // 428
(short)0x9550,
(short)0x9491,   // 429
(short)0x2B63,
(short)0x9566,   // 430
(short)0xD29D,
(short)0xD4D1,   // 431
(short)0x947D,
(short)0x2D97,   // 432
(short)0x957C,
(short)0x6B98,   // 433
(short)0xD505,
(short)0x9592,   // 434
(short)0xD236,
(short)0xD538,   // 435
(short)0x9453,
(short)0x2DFD,   // 436
(short)0x95A8,
(short)0x943F,   // 437
(short)0x2A94,
(short)0x95BE,   // 438
(short)0xD1D1,
(short)0x2A61,   // 439
(short)0x6BD6,
(short)0xD19E,   // 440
(short)0x6A2C,
(short)0x6BEA,   // 441
(short)0xD5D3,
(short)0x6A16,   // 442
(short)0x2E95,
(short)0x29F9,   // 443
(short)0x6BFE,
(short)0xD138,   // 444
(short)0x69FF,
(short)0x6C12,   // 445
(short)0xD63B,
(short)0x9617,   // 446
(short)0xD105,
(short)0x2992,   // 447
(short)0x6C26,
(short)0x2F2E,   // 448
(short)0x962E,
(short)0x6C3A,   // 449
(short)0xD6A2,
(short)0x69BC,   // 450
(short)0x2F60,
(short)0x292A,   // 451
(short)0x6C4E,
(short)0x2F93,   // 452
(short)0x965B,
(short)0x6C61,   // 453
(short)0xD70A,
(short)0x9672,   // 454
(short)0xD03A,
(short)0x28C2,   // 455
(short)0x6C75,
(short)0x2FF8,   // 456
(short)0x9689,
(short)0x6C88,   // 457
(short)0xD772,
(short)0x96A0,   // 458
(short)0xCFD5,
(short)0xD7A6,   // 459
(short)0x9364,
(short)0xCFA3,   // 460
(short)0x6949,
(short)0x6CAF,   // 461
(short)0xD7DA,
(short)0x6932,   // 462
(short)0x3090,
(short)0x27F2,   // 463
(short)0x6CC2,
(short)0xCF3E,   // 464
(short)0x691A,
(short)0x932B,   // 465
(short)0x27BE,
(short)0x96FD,   // 466
(short)0xCF0C,
(short)0x2789,   // 467
(short)0x6CE8,
(short)0x3127,   // 468
(short)0x9715,
(short)0x6CFB,   // 469
(short)0xD8AB,
(short)0x972C,   // 470
(short)0xCEA7,
(short)0x2721,   // 471
(short)0x6D0E,
(short)0x318B,   // 472
(short)0x9744,
(short)0x92DF,   // 473
(short)0x26ED,
(short)0x975C,   // 474
(short)0xCE43,
(short)0x26B8,   // 475
(short)0x6D33,
(short)0x31EF,   // 476
(short)0x9773,
(short)0x92BA,   // 477
(short)0x2684,
(short)0x6875,   // 478
(short)0x3222,
(short)0x2650,   // 479
(short)0x6D58,
(short)0x3254,   // 480
(short)0x97A4,
(short)0x9295,   // 481
(short)0x261B,
(short)0x6844,   // 482
(short)0x3286,
(short)0x25E7,   // 483
(short)0x6D7D,
(short)0xCD48,   // 484
(short)0x682C,
(short)0x6D8F,   // 485
(short)0xDA4E,
(short)0x97EC,   // 486
(short)0xCD17,
(short)0xDA82,   // 487
(short)0x925F,
(short)0x331B,   // 488
(short)0x9805,
(short)0x924D,   // 489
(short)0x2549,
(short)0x67E3,   // 490
(short)0x334D,
(short)0x2514,   // 491
(short)0x6DC5,
(short)0x337F,   // 492
(short)0x9836,
(short)0x922A,   // 493
(short)0x24E0,
(short)0x67B1,   // 494
(short)0x33B1,
(short)0x24AB,   // 495
(short)0x6DE8,
(short)0xCC1E,   // 496
(short)0x6798,
(short)0x9206,   // 497
(short)0x2476,
(short)0x9880,   // 498
(short)0xCBEC,
(short)0x2442,   // 499
(short)0x6E0B,
(short)0xCBBB,   // 500
(short)0x6767,
(short)0x6E1C,   // 501
(short)0xDBF3,
(short)0x674D,   // 502
(short)0x3477,
(short)0x23D8,   // 503
(short)0x6E2E,
(short)0x34A9,   // 504
(short)0x98CC,
(short)0x6E3F,   // 505
(short)0xDC5D,
(short)0x98E5,   // 506
(short)0xCB26,
(short)0x236F,   // 507
(short)0x6E50,
(short)0xCAF5,   // 508
(short)0x6702,
(short)0x919F,   // 509
(short)0x233A,
(short)0x66E8,   // 510
(short)0x353D,
(short)0xDCFB,   // 511
(short)0x918F,
(short)0x356E,   // 512
(short)0x9931,
(short)0x6E82,   // 513
(short)0xDD30,
(short)0x66B5,   // 514
(short)0x359F,
(short)0xDD65,   // 515
(short)0x916D,
(short)0xCA30,   // 516
(short)0x669B,
(short)0x6EA3,   // 517
(short)0xDD9A,
(short)0x997F,   // 518
(short)0xC9FE,
(short)0x2231,   // 519
(short)0x6EB4,
(short)0xC9CD,   // 520
(short)0x6667,
(short)0x913C,   // 521
(short)0x21FC,
(short)0x99B3,   // 522
(short)0xC99C,
(short)0x21C7,   // 523
(short)0x6ED4,
(short)0x3695,   // 524
(short)0x99CD,
(short)0x911B,   // 525
(short)0x2191,
(short)0x6619,   // 526
(short)0x36C6,
(short)0xDEA4,   // 527
(short)0x910B,
(short)0xC909,   // 528
(short)0x65FF,
(short)0x6F05,   // 529
(short)0xDED9,
(short)0x65E4,   // 530
(short)0x3727,
(short)0x20F2,   // 531
(short)0x6F14,
(short)0xC8A8,   // 532
(short)0x65CA,
(short)0x90DC,   // 533
(short)0x20BD,
(short)0x65AF,   // 534
(short)0x3789,
(short)0x2087,   // 535
(short)0x6F34,
(short)0xC846,   // 536
(short)0x6595,
(short)0x6F43,   // 537
(short)0xDFAE,
(short)0x9A86,   // 538
(short)0xC816,
(short)0xDFE3,   // 539
(short)0x90AD,
(short)0xC7E5,   // 540
(short)0x655F,
(short)0x909E,   // 541
(short)0x1FE7,
(short)0x9ABC,   // 542
(short)0xC7B4,
(short)0x1FB2,   // 543
(short)0x6F71,
(short)0xC784,   // 544
(short)0x6529,
(short)0x9080,   // 545
(short)0x1F7C,
(short)0x9AF2,   // 546
(short)0xC753,
(short)0xE0B9,   // 547
(short)0x9071,
(short)0xC723,   // 548
(short)0x64F3,
(short)0x9062,   // 549
(short)0x1F12,
(short)0x64D7,   // 550
(short)0x390D,
(short)0x1EDC,   // 551
(short)0x6FAD,
(short)0x393E,   // 552
(short)0x9B44,
(short)0x9044,   // 553
(short)0x1EA6,
(short)0x9B5F,   // 554
(short)0xC692,
(short)0x1E71,   // 555
(short)0x6FCB,
(short)0x399E,   // 556
(short)0x9B7B,
(short)0x6FD9,   // 557
(short)0xE1C5,
(short)0x6469,   // 558
(short)0x39CE,
(short)0x1E06,   // 559
(short)0x6FE8,
(short)0x39FE,   // 560
(short)0x9BB2,
(short)0x6FF6,   // 561
(short)0xE230,
(short)0x6432,   // 562
(short)0x3A2E,
(short)0xE266,   // 563
(short)0x8FFC,
(short)0xC5A2,   // 564
(short)0x6416,
(short)0x7012,   // 565
(short)0xE29B,
(short)0x9C06,   // 566
(short)0xC572,
(short)0xE2D1,   // 567
(short)0x8FE0,
(short)0xC542,   // 568
(short)0x63DE,
(short)0x8FD2,   // 569
(short)0x1CF9,
(short)0x63C2,   // 570
(short)0x3AEE,
(short)0x1CC3,   // 571
(short)0x703C,
(short)0xC4E2,   // 572
(short)0x63A5,
(short)0x704A,   // 573
(short)0xE372,
(short)0x6389,   // 574
(short)0x3B4E,
(short)0xE3A8,   // 575
(short)0x8FA8,
(short)0x3B7D,   // 576
(short)0x9C94,
(short)0x7065,   // 577
(short)0xE3DE,
(short)0x6350,   // 578
(short)0x3BAD,
(short)0x1BEC,   // 579
(short)0x7072,
(short)0x3BDC,   // 580
(short)0x9CCD,
(short)0x7080,   // 581
(short)0xE44A,
(short)0x6316,   // 582
(short)0x3C0C,
(short)0x1B80,   // 583
(short)0x708D,
(short)0x3C3B,   // 584
(short)0x9D06,
(short)0x8F66,   // 585
(short)0x1B4A,
(short)0x62DD,   // 586
(short)0x3C6B,
(short)0xE4EC,   // 587
(short)0x8F59,
(short)0xC366,   // 588
(short)0x62C0,
(short)0x70B4,   // 589
(short)0xE522,
(short)0x9D5D,   // 590
(short)0xC336,
(short)0xE558,   // 591
(short)0x8F3F,
(short)0x3CF9,   // 592
(short)0x9D7B,
(short)0x70CE,   // 593
(short)0xE58E,
(short)0x9D98,   // 594
(short)0xC2D8,
(short)0xE5C4,   // 595
(short)0x8F26,
(short)0x3D57,   // 596
(short)0x9DB5,
(short)0x70E7,   // 597
(short)0xE5FA,
(short)0x622D,   // 598
(short)0x3D86,
(short)0x19D0,   // 599
(short)0x70F3,
(short)0x3DB5,   // 600
(short)0x9DF0,
(short)0x7100,   // 601
(short)0xE666,
(short)0x61F2,   // 602
(short)0x3DE4,
(short)0xE69C,   // 603
(short)0x8EF4,
(short)0x3E13,   // 604
(short)0x9E2B,
(short)0x8EE8,   // 605
(short)0x192D,
(short)0x9E49,   // 606
(short)0xC1BE,
(short)0xE709,   // 607
(short)0x8EDC,
(short)0x3E71,   // 608
(short)0x9E67,
(short)0x8ED0,   // 609
(short)0x18C1,
(short)0x617B,   // 610
(short)0x3EA0,
(short)0xE775,   // 611
(short)0x8EC4,
(short)0x3ECE,   // 612
(short)0x9EA3,
(short)0x8EB9,   // 613
(short)0x1854,
(short)0x613F,   // 614
(short)0x3EFD,
(short)0x181E,   // 615
(short)0x7153,
(short)0xC0D5,   // 616
(short)0x6120,
(short)0x8EA2,   // 617
(short)0x17E8,
(short)0x9EFE,   // 618
(short)0xC0A6,
(short)0x17B1,   // 619
(short)0x716A,
(short)0x3F88,   // 620
(short)0x9F1C,
(short)0x8E8B,   // 621
(short)0x177B,
(short)0x9F3B,   // 622
(short)0xC049,
(short)0x1745,   // 623
(short)0x7180,
(short)0xC01B,   // 624
(short)0x60A7,
(short)0x718B,   // 625
(short)0xE8F2,
(short)0x6088,   // 626
(short)0x4013,
(short)0xE928,   // 627
(short)0x8E6A,
(short)0x4042,   // 628
(short)0x9F97,
(short)0x71A1,   // 629
(short)0xE95F,
(short)0x9FB6,   // 630
(short)0xBF90,
(short)0xE995,   // 631
(short)0x8E54,
(short)0xBF62,   // 632
(short)0x602B,
(short)0x71B7,   // 633
(short)0xE9CC,
(short)0x600C,   // 634
(short)0x40CC,
(short)0x15FE,   // 635
(short)0x71C1,
(short)0xBF06,   // 636
(short)0x5FED,
(short)0x71CC,   // 637
(short)0xEA39,
(short)0xA032,   // 638
(short)0xBED8,
(short)0x1591,   // 639
(short)0x71D6,
(short)0x4156,   // 640
(short)0xA051,
(short)0x71E1,   // 641
(short)0xEAA6,
(short)0xA070,   // 642
(short)0xBE7C,
(short)0xEADC,   // 643
(short)0x8E15,
(short)0xBE4E,   // 644
(short)0x5F70,
(short)0x8E0B,   // 645
(short)0x14ED,
(short)0x5F51,   // 646
(short)0x41DF,
(short)0x14B6,   // 647
(short)0x71FF,
(short)0x420D,   // 648
(short)0xA0CF,
(short)0x8DF7,   // 649
(short)0x1480,
(short)0xA0EF,   // 650
(short)0xBDC6,
(short)0xEBB7,   // 651
(short)0x8DED,
(short)0x4268,   // 652
(short)0xA10F,
(short)0x8DE4,   // 653
(short)0x1412,
(short)0x5ED2,   // 654
(short)0x4295,
(short)0x13DC,   // 655
(short)0x7226,
(short)0xBD3D,   // 656
(short)0x5EB2,
(short)0x722F,   // 657
(short)0xEC5B,
(short)0x5E92,   // 658
(short)0x42F0,
(short)0xEC92,   // 659
(short)0x8DC7,
(short)0x431E,   // 660
(short)0xA18F,
(short)0x8DBE,   // 661
(short)0x1337,
(short)0x5E51,   // 662
(short)0x434B,
(short)0xECFF,   // 663
(short)0x8DB5,
(short)0xBC88,   // 664
(short)0x5E31,
(short)0x7254,   // 665
(short)0xED36,
(short)0x5E11,   // 666
(short)0x43A5,
(short)0xED6D,   // 667
(short)0x8DA3,
(short)0x43D2,   // 668
(short)0xA210,
(short)0x8D9A,   // 669
(short)0x125C,
(short)0x5DD0,   // 670
(short)0x43FF,
(short)0x1225,   // 671
(short)0x726F,
(short)0x442C,   // 672
(short)0xA251,
(short)0x7277,   // 673
(short)0xEE11,
(short)0x5D8E,   // 674
(short)0x4459,
(short)0x11B8,   // 675
(short)0x7280,
(short)0xBB7A,   // 676
(short)0x5D6D,
(short)0x7288,   // 677
(short)0xEE7F,
(short)0x5D4D,   // 678
(short)0x44B2,
(short)0x114A,   // 679
(short)0x7291,
(short)0xBB21,   // 680
(short)0x5D2C,
(short)0x7299,   // 681
(short)0xEEED,
(short)0x5D0B,   // 682
(short)0x450C,
(short)0x10DC,   // 683
(short)0x72A1,
(short)0xBAC8,   // 684
(short)0x5CE9,
(short)0x8D57,   // 685
(short)0x10A5,
(short)0xA338,   // 686
(short)0xBA9B,
(short)0x106E,   // 687
(short)0x72B1,
(short)0x4591,   // 688
(short)0xA359,
(short)0x8D47,   // 689
(short)0x1037,
(short)0x5C85,   // 690
(short)0x45BE,
(short)0xF000,   // 691
(short)0x8D40,
(short)0x45EA,   // 692
(short)0xA39C,
(short)0x72C8,   // 693
(short)0xF037,
(short)0xA3BE,   // 694
(short)0xB9EA,
(short)0x0F92,   // 695
(short)0x72D0,
(short)0xB9BE,   // 696
(short)0x5C21,
(short)0x72D7,   // 697
(short)0xF0A5,
(short)0xA401,   // 698
(short)0xB991,
(short)0xF0DC,   // 699
(short)0x8D22,
(short)0xB965,   // 700
(short)0x5BDD,
(short)0x72E6,   // 701
(short)0xF113,
(short)0x5BBB,   // 702
(short)0x46C7,
(short)0xF14A,   // 703
(short)0x8D13,
(short)0xB90D,   // 704
(short)0x5B99,
(short)0x72F4,   // 705
(short)0xF181,
(short)0xA489,   // 706
(short)0xB8E2,
(short)0x0E48,   // 707
(short)0x72FB,
(short)0xB8B6,   // 708
(short)0x5B55,
(short)0x8CFF,   // 709
(short)0x0E11,
(short)0x5B33,   // 710
(short)0x4776,
(short)0x0DDA,   // 711
(short)0x7308,
(short)0xB85E,   // 712
(short)0x5B11,
(short)0x8CF1,   // 713
(short)0x0DA2,
(short)0xA512,   // 714
(short)0xB833,
(short)0x0D6B,   // 715
(short)0x7315,
(short)0xB807,   // 716
(short)0x5ACC,
(short)0x731B,   // 717
(short)0xF2CC,
(short)0x5AA9,   // 718
(short)0x4824,
(short)0x0CFD,   // 719
(short)0x7322,
(short)0x4850,   // 720
(short)0xA579,
(short)0x8CD8,   // 721
(short)0x0CC6,
(short)0x5A64,   // 722
(short)0x487B,
(short)0x0C8E,   // 723
(short)0x732E,
(short)0x48A6,   // 724
(short)0xA5BF,
(short)0x7334,   // 725
(short)0xF3A9,
(short)0xA5E2,   // 726
(short)0xB72E,
(short)0x0C20,   // 727
(short)0x733A,
(short)0xB703,   // 728
(short)0x59FC,
(short)0x7340,   // 729
(short)0xF417,
(short)0x59D8,   // 730
(short)0x4928,
(short)0xF44E,   // 731
(short)0x8CBB,
(short)0xB6AD,   // 732
(short)0x59B5,
(short)0x734B,   // 733
(short)0xF486,
(short)0x5992,   // 734
(short)0x497E,
(short)0xF4BD,   // 735
(short)0x8CB0,
(short)0x49A9,   // 736
(short)0xA691,
(short)0x7356,   // 737
(short)0xF4F4,
(short)0xA6B4,   // 738
(short)0xB62C,
(short)0xF52B,   // 739
(short)0x8CA5,
(short)0xB602,   // 740
(short)0x5928,
(short)0x7360,   // 741
(short)0xF563,
(short)0xA6FB,   // 742
(short)0xB5D7,
(short)0x0A66,   // 743
(short)0x7365,
(short)0xB5AC,   // 744
(short)0x58E1,
(short)0x8C96,   // 745
(short)0x0A2F,
(short)0x58BD,   // 746
(short)0x4A7E,
(short)0xF609,   // 747
(short)0x8C91,
(short)0xB557,   // 748
(short)0x589A,
(short)0x7373,   // 749
(short)0xF640,
(short)0xA78A,   // 750
(short)0xB52D,
(short)0xF677,   // 751
(short)0x8C88,
(short)0x4AFE,   // 752
(short)0xA7AE,
(short)0x8C83,   // 753
(short)0x0951,
(short)0x582E,   // 754
(short)0x4B28,
(short)0xF6E6,   // 755
(short)0x8C7F,
(short)0xB4AE,   // 756
(short)0x580A,
(short)0x8C7B,   // 757
(short)0x08E3,
(short)0x57E6,   // 758
(short)0x4B7C,
(short)0x08AB,   // 759
(short)0x738A,
(short)0x4BA6,   // 760
(short)0xA83E,
(short)0x738E,   // 761
(short)0xF78C,
(short)0x579D,   // 762
(short)0x4BD0,
(short)0xF7C4,   // 763
(short)0x8C6E,
(short)0x4BFA,   // 764
(short)0xA887,
(short)0x7396,   // 765
(short)0xF7FB,
(short)0x5754,   // 766
(short)0x4C24,
(short)0x07CE,   // 767
(short)0x7399,
(short)0x4C4E,   // 768
(short)0xA8D0,
(short)0x8C63,   // 769
(short)0x0796,
(short)0xA8F5,   // 770
(short)0xB388,
(short)0x075F,   // 771
(short)0x73A1,
(short)0xB35F,   // 772
(short)0x56E7,
(short)0x8C5C,   // 773
(short)0x0727,
(short)0xA93E,   // 774
(short)0xB335,
(short)0x06F0,   // 775
(short)0x73A7,
(short)0xB30B,   // 776
(short)0x569D,
(short)0x73AB,   // 777
(short)0xF947,
(short)0xA988,   // 778
(short)0xB2E2,
(short)0xF97F,   // 779
(short)0x8C52,
(short)0x4D48,   // 780
(short)0xA9AD,
(short)0x8C4F,   // 781
(short)0x064A,
(short)0x562E,   // 782
(short)0x4D71,
(short)0x0612,   // 783
(short)0x73B4,
(short)0x4D9A,   // 784
(short)0xA9F7,
(short)0x8C49,   // 785
(short)0x05DB,
(short)0xAA1C,   // 786
(short)0xB23D,
(short)0x05A3,   // 787
(short)0x73BA,
(short)0xB214,   // 788
(short)0x55BE,
(short)0x73BC,   // 789
(short)0xFA94,
(short)0x5599,   // 790
(short)0x4E16,
(short)0x0534,   // 791
(short)0x73BF,
(short)0xB1C2,   // 792
(short)0x5573,
(short)0x8C3F,   // 793
(short)0x04FD,
(short)0xAAB2,   // 794
(short)0xB199,
(short)0x04C5,   // 795
(short)0x73C4,
(short)0x4E90,   // 796
(short)0xAAD8,
(short)0x73C6,   // 797
(short)0xFB72,
(short)0xAAFD,   // 798
(short)0xB147,
(short)0x0456,   // 799
(short)0x73C8,
(short)0x4EE2,   // 800
(short)0xAB23,
(short)0x8C36,   // 801
(short)0x041F,
(short)0xAB49,   // 802
(short)0xB0F6,
(short)0xFC19,   // 803
(short)0x8C34,
(short)0x4F33,   // 804
(short)0xAB6F,
(short)0x8C32,   // 805
(short)0x03B0,
(short)0xAB95,   // 806
(short)0xB0A5,
(short)0x0378,   // 807
(short)0x73CF,
(short)0x4F84,   // 808
(short)0xABBB,
(short)0x8C2F,   // 809
(short)0x0341,
(short)0x541F,   // 810
(short)0x4FAC,
(short)0xFCF7,   // 811
(short)0x8C2E,
(short)0x4FD5,   // 812
(short)0xAC07,
(short)0x8C2C,   // 813
(short)0x02D2,
(short)0x53D2,   // 814
(short)0x4FFD,
(short)0x029A,   // 815
(short)0x73D5,
(short)0xAFDB,   // 816
(short)0x53AC,
(short)0x73D6,   // 817
(short)0xFD9D,
(short)0xAC7B,   // 818
(short)0xAFB3,
(short)0xFDD5,   // 819
(short)0x8C29,
(short)0x5075,   // 820
(short)0xACA1,
(short)0x73D8,   // 821
(short)0xFE0C,
(short)0xACC8,   // 822
(short)0xAF63,
(short)0x01BC,   // 823
(short)0x73D9,
(short)0xAF3B,   // 824
(short)0x5312,
(short)0x8C26,   // 825
(short)0x0185,
(short)0x52EB,   // 826
(short)0x50EC,
(short)0xFEB3,   // 827
(short)0x8C25,
(short)0xAEEC,   // 828
(short)0x52C4,
(short)0x73DB,   // 829
(short)0xFEEA,
(short)0x529D,   // 830
(short)0x513C,
(short)0x00DE,   // 831
(short)0x73DC,
(short)0xAE9D,   // 832
(short)0x5276,
(short)0x8C24,   // 833
(short)0x00A7,
(short)0xADB1,   // 834
(short)0xAE75,
(short)0xFF91,   // 835
(short)0x8C24,
(short)0x51B2,   // 836
(short)0xADD8,
(short)0x73DD,   // 837
(short)0xFFC8,
(short)0x5201,   // 838
(short)0x51DA,
(short)0x0000,
(short)0x0000,
//};
//
//
//S16 ZC839MultInv[840] = {
(short)   0,   //   0
(short)   1,   //   1
(short) 420,   //   2
(short) 280,   //   3
(short) 210,   //   4
(short) 168,   //   5
(short) 140,   //   6
(short) 120,   //   7
(short) 105,   //   8
(short) 373,   //   9
(short)  84,   //  10
(short) 534,   //  11
(short)  70,   //  12
(short) 710,   //  13
(short)  60,   //  14
(short)  56,   //  15
(short) 472,   //  16
(short) 691,   //  17
(short) 606,   //  18
(short) 265,   //  19
(short)  42,   //  20
(short)  40,   //  21
(short) 267,   //  22
(short)  73,   //  23
(short)  35,   //  24
(short) 537,   //  25
(short) 355,   //  26
(short) 404,   //  27
(short)  30,   //  28
(short) 434,   //  29
(short)  28,   //  30
(short) 406,   //  31
(short) 236,   //  32
(short) 178,   //  33
(short) 765,   //  34
(short)  24,   //  35
(short) 303,   //  36
(short) 771,   //  37
(short) 552,   //  38
(short) 796,   //  39
(short)  21,   //  40
(short) 573,   //  41
(short)  20,   //  42
(short) 800,   //  43
(short) 553,   //  44
(short) 578,   //  45
(short) 456,   //  46
(short) 482,   //  47
(short) 437,   //  48
(short) 137,   //  49
(short) 688,   //  50
(short) 510,   //  51
(short) 597,   //  52
(short)  95,   //  53
(short) 202,   //  54
(short) 778,   //  55
(short)  15,   //  56
(short) 368,   //  57
(short) 217,   //  58
(short) 128,   //  59
(short)  14,   //  60
(short) 784,   //  61
(short) 203,   //  62
(short) 293,   //  63
(short) 118,   //  64
(short) 142,   //  65
(short)  89,   //  66
(short) 551,   //  67
(short) 802,   //  68
(short) 304,   //  69
(short)  12,   //  70
(short) 130,   //  71
(short) 571,   //  72
(short)  23,   //  73
(short) 805,   //  74
(short) 179,   //  75
(short) 276,   //  76
(short) 316,   //  77
(short) 398,   //  78
(short) 308,   //  79
(short) 430,   //  80
(short) 694,   //  81
(short) 706,   //  82
(short) 465,   //  83
(short)  10,   //  84
(short) 306,   //  85
(short) 400,   //  86
(short) 704,   //  87
(short) 696,   //  88
(short)  66,   //  89
(short) 289,   //  90
(short) 461,   //  91
(short) 228,   //  92
(short) 415,   //  93
(short) 241,   //  94
(short)  53,   //  95
(short) 638,   //  96
(short) 173,   //  97
(short) 488,   //  98
(short) 339,   //  99
(short) 344,   // 100
(short) 108,   // 101
(short) 255,   // 102
(short) 391,   // 103
(short) 718,   // 104
(short)   8,   // 105
(short) 467,   // 106
(short) 494,   // 107
(short) 101,   // 108
(short) 585,   // 109
(short) 389,   // 110
(short) 257,   // 111
(short) 427,   // 112
(short) 297,   // 113
(short) 184,   // 114
(short) 518,   // 115
(short) 528,   // 116
(short) 545,   // 117
(short)  64,   // 118
(short) 698,   // 119
(short)   7,   // 120
(short) 735,   // 121
(short) 392,   // 122
(short) 191,   // 123
(short) 521,   // 124
(short) 443,   // 125
(short) 566,   // 126
(short) 621,   // 127
(short)  59,   // 128
(short) 826,   // 129
(short)  71,   // 130
(short) 269,   // 131
(short) 464,   // 132
(short) 757,   // 133
(short) 695,   // 134
(short) 752,   // 135
(short) 401,   // 136
(short)  49,   // 137
(short) 152,   // 138
(short) 670,   // 139
(short)   6,   // 140
(short) 720,   // 141
(short)  65,   // 142
(short) 751,   // 143
(short) 705,   // 144
(short) 758,   // 145
(short) 431,   // 146
(short) 605,   // 147
(short) 822,   // 148
(short) 473,   // 149
(short) 509,   // 150
(short) 789,   // 151
(short) 138,   // 152
(short) 170,   // 153
(short) 158,   // 154
(short) 249,   // 155
(short) 199,   // 156
(short) 668,   // 157
(short) 154,   // 158
(short) 591,   // 159
(short) 215,   // 160
(short) 370,   // 161
(short) 347,   // 162
(short) 664,   // 163
(short) 353,   // 164
(short) 539,   // 165
(short) 652,   // 166
(short) 628,   // 167
(short)   5,   // 168
(short) 700,   // 169
(short) 153,   // 170
(short) 682,   // 171
(short) 200,   // 172
(short)  97,   // 173
(short) 352,   // 174
(short) 676,   // 175
(short) 348,   // 176
(short) 602,   // 177
(short)  33,   // 178
(short)  75,   // 179
(short) 564,   // 180
(short) 445,   // 181
(short) 650,   // 182
(short) 541,   // 183
(short) 114,   // 184
(short) 322,   // 185
(short) 627,   // 186
(short) 673,   // 187
(short) 540,   // 188
(short) 657,   // 189
(short) 446,   // 190
(short) 123,   // 191
(short) 319,   // 192
(short) 313,   // 193
(short) 506,   // 194
(short) 327,   // 195
(short) 244,   // 196
(short) 477,   // 197
(short) 589,   // 198
(short) 156,   // 199
(short) 172,   // 200
(short) 743,   // 201
(short)  54,   // 202
(short)  62,   // 203
(short) 547,   // 204
(short) 618,   // 205
(short) 615,   // 206
(short) 381,   // 207
(short) 359,   // 208
(short) 558,   // 209
(short)   4,   // 210
(short) 672,   // 211
(short) 653,   // 212
(short) 323,   // 213
(short) 247,   // 214
(short) 160,   // 215
(short) 470,   // 216
(short)  58,   // 217
(short) 712,   // 218
(short) 567,   // 219
(short) 614,   // 220
(short) 634,   // 221
(short) 548,   // 222
(short) 380,   // 223
(short) 633,   // 224
(short) 619,   // 225
(short) 568,   // 226
(short) 377,   // 227
(short)  92,   // 228
(short) 425,   // 229
(short) 259,   // 230
(short) 385,   // 231
(short) 264,   // 232
(short) 821,   // 233
(short) 692,   // 234
(short) 432,   // 235
(short)  32,   // 236
(short) 662,   // 237
(short) 349,   // 238
(short) 502,   // 239
(short) 423,   // 240
(short)  94,   // 241
(short) 787,   // 242
(short) 511,   // 243
(short) 196,   // 244
(short) 363,   // 245
(short) 515,   // 246
(short) 214,   // 247
(short) 680,   // 248
(short) 155,   // 249
(short) 641,   // 250
(short) 478,   // 251
(short) 283,   // 252
(short) 388,   // 253
(short) 730,   // 254
(short) 102,   // 255
(short) 449,   // 256
(short) 111,   // 257
(short) 413,   // 258
(short) 230,   // 259
(short) 455,   // 260
(short) 794,   // 261
(short) 554,   // 262
(short) 453,   // 263
(short) 232,   // 264
(short)  19,   // 265
(short) 798,   // 266
(short)  22,   // 267
(short) 767,   // 268
(short) 131,   // 269
(short) 376,   // 270
(short) 613,   // 271
(short) 620,   // 272
(short) 713,   // 273
(short) 444,   // 274
(short) 659,   // 275
(short)  76,   // 276
(short) 524,   // 277
(short) 335,   // 278
(short) 418,   // 279
(short)   3,   // 280
(short) 630,   // 281
(short) 360,   // 282
(short) 252,   // 283
(short) 452,   // 284
(short) 577,   // 285
(short) 795,   // 286
(short) 801,   // 287
(short) 772,   // 288
(short)  90,   // 289
(short) 379,   // 290
(short) 617,   // 291
(short) 635,   // 292
(short)  63,   // 293
(short) 722,   // 294
(short) 529,   // 295
(short) 411,   // 296
(short) 113,   // 297
(short) 656,   // 298
(short) 651,   // 299
(short) 674,   // 300
(short) 354,   // 301
(short) 814,   // 302
(short)  36,   // 303
(short)  69,   // 304
(short) 828,   // 305
(short)  85,   // 306
(short) 440,   // 307
(short)  79,   // 308
(short) 410,   // 309
(short) 544,   // 310
(short) 723,   // 311
(short) 519,   // 312
(short) 193,   // 313
(short) 334,   // 314
(short) 562,   // 315
(short)  77,   // 316
(short) 442,   // 317
(short) 715,   // 318
(short) 192,   // 319
(short) 527,   // 320
(short) 724,   // 321
(short) 185,   // 322
(short) 213,   // 323
(short) 593,   // 324
(short) 364,   // 325
(short) 332,   // 326
(short) 195,   // 327
(short) 596,   // 328
(short) 788,   // 329
(short) 689,   // 330
(short) 474,   // 331
(short) 326,   // 332
(short) 645,   // 333
(short) 314,   // 334
(short) 278,   // 335
(short) 422,   // 336
(short) 600,   // 337
(short) 350,   // 338
(short)  99,   // 339
(short) 496,   // 340
(short) 342,   // 341
(short) 341,   // 342
(short) 499,   // 343
(short) 100,   // 344
(short) 732,   // 345
(short) 468,   // 346
(short) 162,   // 347
(short) 176,   // 348
(short) 238,   // 349
(short) 338,   // 350
(short) 741,   // 351
(short) 174,   // 352
(short) 164,   // 353
(short) 301,   // 354
(short)  26,   // 355
(short) 436,   // 356
(short) 792,   // 357
(short) 457,   // 358
(short) 208,   // 359
(short) 282,   // 360
(short) 588,   // 361
(short) 642,   // 362
(short) 245,   // 363
(short) 325,   // 364
(short) 508,   // 365
(short) 690,   // 366
(short) 823,   // 367
(short)  57,   // 368
(short) 623,   // 369
(short) 161,   // 370
(short) 493,   // 371
(short) 733,   // 372
(short)   9,   // 373
(short) 756,   // 374
(short) 707,   // 375
(short) 270,   // 376
(short) 227,   // 377
(short) 748,   // 378
(short) 290,   // 379
(short) 223,   // 380
(short) 207,   // 381
(short) 481,   // 382
(short) 793,   // 383
(short) 579,   // 384
(short) 231,   // 385
(short) 576,   // 386
(short) 555,   // 387
(short) 253,   // 388
(short) 110,   // 389
(short) 583,   // 390
(short) 103,   // 391
(short) 122,   // 392
(short) 649,   // 393
(short) 658,   // 394
(short) 565,   // 395
(short) 714,   // 396
(short) 522,   // 397
(short)  78,   // 398
(short) 532,   // 399
(short)  86,   // 400
(short) 136,   // 401
(short) 791,   // 402
(short) 483,   // 403
(short)  27,   // 404
(short) 810,   // 405
(short)  31,   // 406
(short) 604,   // 407
(short) 693,   // 408
(short) 759,   // 409
(short) 309,   // 410
(short) 296,   // 411
(short) 727,   // 412
(short) 258,   // 413
(short) 610,   // 414
(short)  93,   // 415
(short) 599,   // 416
(short) 503,   // 417
(short) 279,   // 418
(short) 837,   // 419
(short)   2,   // 420
(short) 560,   // 421
(short) 336,   // 422
(short) 240,   // 423
(short) 746,   // 424
(short) 229,   // 425
(short) 581,   // 426
(short) 112,   // 427
(short) 543,   // 428
(short) 530,   // 429
(short)  80,   // 430
(short) 146,   // 431
(short) 235,   // 432
(short) 808,   // 433
(short)  29,   // 434
(short) 812,   // 435
(short) 356,   // 436
(short)  48,   // 437
(short) 703,   // 438
(short) 753,   // 439
(short) 307,   // 440
(short) 761,   // 441
(short) 317,   // 442
(short) 125,   // 443
(short) 274,   // 444
(short) 181,   // 445
(short) 190,   // 446
(short) 717,   // 447
(short) 736,   // 448
(short) 256,   // 449
(short) 729,   // 450
(short) 586,   // 451
(short) 284,   // 452
(short) 263,   // 453
(short) 608,   // 454
(short) 260,   // 455
(short)  46,   // 456
(short) 358,   // 457
(short) 632,   // 458
(short) 616,   // 459
(short) 549,   // 460
(short)  91,   // 461
(short) 612,   // 462
(short) 569,   // 463
(short) 132,   // 464
(short)  83,   // 465
(short) 830,   // 466
(short) 106,   // 467
(short) 346,   // 468
(short) 678,   // 469
(short) 216,   // 470
(short) 782,   // 471
(short)  16,   // 472
(short) 149,   // 473
(short) 331,   // 474
(short) 514,   // 475
(short) 594,   // 476
(short) 197,   // 477
(short) 251,   // 478
(short) 557,   // 479
(short) 631,   // 480
(short) 382,   // 481
(short)  47,   // 482
(short) 403,   // 483
(short) 813,   // 484
(short) 538,   // 485
(short) 675,   // 486
(short) 665,   // 487
(short)  98,   // 488
(short) 501,   // 489
(short) 601,   // 490
(short) 663,   // 491
(short) 677,   // 492
(short) 371,   // 493
(short) 107,   // 494
(short) 739,   // 495
(short) 340,   // 496
(short) 498,   // 497
(short) 497,   // 498
(short) 343,   // 499
(short) 740,   // 500
(short) 489,   // 501
(short) 239,   // 502
(short) 417,   // 503
(short) 561,   // 504
(short) 525,   // 505
(short) 194,   // 506
(short) 513,   // 507
(short) 365,   // 508
(short) 150,   // 509
(short)  51,   // 510
(short) 243,   // 511
(short) 644,   // 512
(short) 507,   // 513
(short) 475,   // 514
(short) 246,   // 515
(short) 626,   // 516
(short) 654,   // 517
(short) 115,   // 518
(short) 312,   // 519
(short) 647,   // 520
(short) 124,   // 521
(short) 397,   // 522
(short) 762,   // 523
(short) 277,   // 524
(short) 505,   // 525
(short) 646,   // 526
(short) 320,   // 527
(short) 116,   // 528
(short) 295,   // 529
(short) 429,   // 530
(short) 760,   // 531
(short) 399,   // 532
(short) 754,   // 533
(short)  11,   // 534
(short) 770,   // 535
(short) 803,   // 536
(short)  25,   // 537
(short) 485,   // 538
(short) 165,   // 539
(short) 188,   // 540
(short) 183,   // 541
(short) 726,   // 542
(short) 428,   // 543
(short) 310,   // 544
(short) 117,   // 545
(short) 776,   // 546
(short) 204,   // 547
(short) 222,   // 548
(short) 460,   // 549
(short) 749,   // 550
(short)  67,   // 551
(short)  38,   // 552
(short)  44,   // 553
(short) 262,   // 554
(short) 387,   // 555
(short) 587,   // 556
(short) 479,   // 557
(short) 209,   // 558
(short) 836,   // 559
(short) 421,   // 560
(short) 504,   // 561
(short) 315,   // 562
(short) 763,   // 563
(short) 180,   // 564
(short) 395,   // 565
(short) 126,   // 566
(short) 219,   // 567
(short) 226,   // 568
(short) 463,   // 569
(short) 708,   // 570
(short)  72,   // 571
(short) 817,   // 572
(short)  41,   // 573
(short) 820,   // 574
(short) 607,   // 575
(short) 386,   // 576
(short) 285,   // 577
(short)  45,   // 578
(short) 384,   // 579
(short) 609,   // 580
(short) 426,   // 581
(short) 728,   // 582
(short) 390,   // 583
(short) 737,   // 584
(short) 109,   // 585
(short) 451,   // 586
(short) 556,   // 587
(short) 361,   // 588
(short) 198,   // 589
(short) 684,   // 590
(short) 159,   // 591
(short) 625,   // 592
(short) 324,   // 593
(short) 476,   // 594
(short) 643,   // 595
(short) 328,   // 596
(short)  52,   // 597
(short) 745,   // 598
(short) 416,   // 599
(short) 337,   // 600
(short) 490,   // 601
(short) 177,   // 602
(short) 807,   // 603
(short) 407,   // 604
(short) 147,   // 605
(short)  18,   // 606
(short) 575,   // 607
(short) 454,   // 608
(short) 580,   // 609
(short) 414,   // 610
(short) 747,   // 611
(short) 462,   // 612
(short) 271,   // 613
(short) 220,   // 614
(short) 206,   // 615
(short) 459,   // 616
(short) 291,   // 617
(short) 205,   // 618
(short) 225,   // 619
(short) 272,   // 620
(short) 127,   // 621
(short) 781,   // 622
(short) 369,   // 623
(short) 679,   // 624
(short) 592,   // 625
(short) 516,   // 626
(short) 186,   // 627
(short) 167,   // 628
(short) 835,   // 629
(short) 281,   // 630
(short) 480,   // 631
(short) 458,   // 632
(short) 224,   // 633
(short) 221,   // 634
(short) 292,   // 635
(short) 777,   // 636
(short) 785,   // 637
(short)  96,   // 638
(short) 667,   // 639
(short) 683,   // 640
(short) 250,   // 641
(short) 362,   // 642
(short) 595,   // 643
(short) 512,   // 644
(short) 333,   // 645
(short) 526,   // 646
(short) 520,   // 647
(short) 716,   // 648
(short) 393,   // 649
(short) 182,   // 650
(short) 299,   // 651
(short) 166,   // 652
(short) 212,   // 653
(short) 517,   // 654
(short) 725,   // 655
(short) 298,   // 656
(short) 189,   // 657
(short) 394,   // 658
(short) 275,   // 659
(short) 764,   // 660
(short) 806,   // 661
(short) 237,   // 662
(short) 491,   // 663
(short) 163,   // 664
(short) 487,   // 665
(short) 742,   // 666
(short) 639,   // 667
(short) 157,   // 668
(short) 686,   // 669
(short) 139,   // 670
(short) 834,   // 671
(short) 211,   // 672
(short) 187,   // 673
(short) 300,   // 674
(short) 486,   // 675
(short) 175,   // 676
(short) 492,   // 677
(short) 469,   // 678
(short) 624,   // 679
(short) 248,   // 680
(short) 685,   // 681
(short) 171,   // 682
(short) 640,   // 683
(short) 590,   // 684
(short) 681,   // 685
(short) 669,   // 686
(short) 701,   // 687
(short)  50,   // 688
(short) 330,   // 689
(short) 366,   // 690
(short)  17,   // 691
(short) 234,   // 692
(short) 408,   // 693
(short)  81,   // 694
(short) 134,   // 695
(short)  88,   // 696
(short) 774,   // 697
(short) 119,   // 698
(short) 833,   // 699
(short) 169,   // 700
(short) 687,   // 701
(short) 790,   // 702
(short) 438,   // 703
(short)  87,   // 704
(short) 144,   // 705
(short)  82,   // 706
(short) 375,   // 707
(short) 570,   // 708
(short) 768,   // 709
(short)  13,   // 710
(short) 780,   // 711
(short) 218,   // 712
(short) 273,   // 713
(short) 396,   // 714
(short) 318,   // 715
(short) 648,   // 716
(short) 447,   // 717
(short) 104,   // 718
(short) 832,   // 719
(short) 141,   // 720
(short) 775,   // 721
(short) 294,   // 722
(short) 311,   // 723
(short) 321,   // 724
(short) 655,   // 725
(short) 542,   // 726
(short) 412,   // 727
(short) 582,   // 728
(short) 450,   // 729
(short) 254,   // 730
(short) 738,   // 731
(short) 345,   // 732
(short) 372,   // 733
(short) 831,   // 734
(short) 121,   // 735
(short) 448,   // 736
(short) 584,   // 737
(short) 731,   // 738
(short) 495,   // 739
(short) 500,   // 740
(short) 351,   // 741
(short) 666,   // 742
(short) 201,   // 743
(short) 786,   // 744
(short) 598,   // 745
(short) 424,   // 746
(short) 611,   // 747
(short) 378,   // 748
(short) 550,   // 749
(short) 773,   // 750
(short) 143,   // 751
(short) 135,   // 752
(short) 439,   // 753
(short) 533,   // 754
(short) 829,   // 755
(short) 374,   // 756
(short) 133,   // 757
(short) 145,   // 758
(short) 409,   // 759
(short) 531,   // 760
(short) 441,   // 761
(short) 523,   // 762
(short) 563,   // 763
(short) 660,   // 764
(short)  34,   // 765
(short) 816,   // 766
(short) 268,   // 767
(short) 709,   // 768
(short) 827,   // 769
(short) 535,   // 770
(short)  37,   // 771
(short) 288,   // 772
(short) 750,   // 773
(short) 697,   // 774
(short) 721,   // 775
(short) 546,   // 776
(short) 636,   // 777
(short)  55,   // 778
(short) 825,   // 779
(short) 711,   // 780
(short) 622,   // 781
(short) 471,   // 782
(short) 824,   // 783
(short)  61,   // 784
(short) 637,   // 785
(short) 744,   // 786
(short) 242,   // 787
(short) 329,   // 788
(short) 151,   // 789
(short) 702,   // 790
(short) 402,   // 791
(short) 357,   // 792
(short) 383,   // 793
(short) 261,   // 794
(short) 286,   // 795
(short)  39,   // 796
(short) 819,   // 797
(short) 266,   // 798
(short) 818,   // 799
(short)  43,   // 800
(short) 287,   // 801
(short)  68,   // 802
(short) 536,   // 803
(short) 815,   // 804
(short)  74,   // 805
(short) 661,   // 806
(short) 603,   // 807
(short) 433,   // 808
(short) 811,   // 809
(short) 405,   // 810
(short) 809,   // 811
(short) 435,   // 812
(short) 484,   // 813
(short) 302,   // 814
(short) 804,   // 815
(short) 766,   // 816
(short) 572,   // 817
(short) 799,   // 818
(short) 797,   // 819
(short) 574,   // 820
(short) 233,   // 821
(short) 148,   // 822
(short) 367,   // 823
(short) 783,   // 824
(short) 779,   // 825
(short) 129,   // 826
(short) 769,   // 827
(short) 305,   // 828
(short) 755,   // 829
(short) 466,   // 830
(short) 734,   // 831
(short) 719,   // 832
(short) 699,   // 833
(short) 671,   // 834
(short) 629,   // 835
(short) 559,   // 836
(short) 419,   // 837
(short) 838,   // 838
(short)   0,
//};
//
//S16 ZC839a1[840] = {
(short)   0,   //   0
(short)   2,   //   1
(short)1260,   //   2
(short)1120,   //   3
(short)1050,   //   4
(short)1008,   //   5
(short) 980,   //   6
(short) 960,   //   7
(short) 106,   //   8
(short) 374,   //   9
(short) 924,   //  10
(short)1374,   //  11
(short) 910,   //  12
(short)1550,   //  13
(short) 900,   //  14
(short) 896,   //  15
(short)1312,   //  16
(short) 692,   //  17
(short)1446,   //  18
(short) 266,   //  19
(short) 882,   //  20
(short) 880,   //  21
(short) 268,   //  22
(short)  74,   //  23
(short)  36,   //  24
(short) 538,   //  25
(short) 356,   //  26
(short)1244,   //  27
(short) 870,   //  28
(short)1274,   //  29
(short) 868,   //  30
(short)1246,   //  31
(short)1076,   //  32
(short)1018,   //  33
(short) 766,   //  34
(short) 864,   //  35
(short) 304,   //  36
(short) 772,   //  37
(short)1392,   //  38
(short)1636,   //  39
(short)  22,   //  40
(short) 574,   //  41
(short) 860,   //  42
(short)1640,   //  43
(short) 554,   //  44
(short)1418,   //  45
(short)1296,   //  46
(short)1322,   //  47
(short) 438,   //  48
(short) 138,   //  49
(short)1528,   //  50
(short)1350,   //  51
(short) 598,   //  52
(short)  96,   //  53
(short)1042,   //  54
(short)1618,   //  55
(short)  16,   //  56
(short)1208,   //  57
(short) 218,   //  58
(short) 968,   //  59
(short) 854,   //  60
(short)1624,   //  61
(short) 204,   //  62
(short) 294,   //  63
(short) 958,   //  64
(short) 982,   //  65
(short)  90,   //  66
(short) 552,   //  67
(short)1642,   //  68
(short)1144,   //  69
(short) 852,   //  70
(short) 970,   //  71
(short) 572,   //  72
(short)  24,   //  73
(short) 806,   //  74
(short) 180,   //  75
(short)1116,   //  76
(short)1156,   //  77
(short)1238,   //  78
(short)1148,   //  79
(short)1270,   //  80
(short)1534,   //  81
(short)1546,   //  82
(short) 466,   //  83
(short) 850,   //  84
(short)1146,   //  85
(short)1240,   //  86
(short)1544,   //  87
(short)1536,   //  88
(short) 906,   //  89
(short) 290,   //  90
(short) 462,   //  91
(short)1068,   //  92
(short) 416,   //  93
(short) 242,   //  94
(short)  54,   //  95
(short)1478,   //  96
(short) 174,   //  97
(short)1328,   //  98
(short) 340,   //  99
(short)1184,   // 100
(short) 948,   // 101
(short) 256,   // 102
(short) 392,   // 103
(short)1558,   // 104
(short) 848,   // 105
(short) 468,   // 106
(short)1334,   // 107
(short) 102,   // 108
(short) 586,   // 109
(short) 390,   // 110
(short) 258,   // 111
(short) 428,   // 112
(short) 298,   // 113
(short)1024,   // 114
(short)1358,   // 115
(short)1368,   // 116
(short) 546,   // 117
(short) 904,   // 118
(short)1538,   // 119
(short)   8,   // 120
(short) 736,   // 121
(short)1232,   // 122
(short) 192,   // 123
(short) 522,   // 124
(short) 444,   // 125
(short)1406,   // 126
(short) 622,   // 127
(short)  60,   // 128
(short)1666,   // 129
(short)  72,   // 130
(short) 270,   // 131
(short)1304,   // 132
(short) 758,   // 133
(short) 696,   // 134
(short)1592,   // 135
(short) 402,   // 136
(short)  50,   // 137
(short) 992,   // 138
(short)1510,   // 139
(short) 846,   // 140
(short)1560,   // 141
(short)  66,   // 142
(short) 752,   // 143
(short) 706,   // 144
(short)1598,   // 145
(short) 432,   // 146
(short) 606,   // 147
(short)1662,   // 148
(short) 474,   // 149
(short) 510,   // 150
(short) 790,   // 151
(short) 978,   // 152
(short)1010,   // 153
(short) 998,   // 154
(short) 250,   // 155
(short) 200,   // 156
(short)1508,   // 157
(short) 994,   // 158
(short) 592,   // 159
(short) 216,   // 160
(short)1210,   // 161
(short) 348,   // 162
(short)1504,   // 163
(short) 354,   // 164
(short) 540,   // 165
(short)1492,   // 166
(short)1468,   // 167
(short)   6,   // 168
(short)1540,   // 169
(short) 154,   // 170
(short)1522,   // 171
(short)1040,   // 172
(short)  98,   // 173
(short)1192,   // 174
(short)1516,   // 175
(short)1188,   // 176
(short)1442,   // 177
(short)  34,   // 178
(short)  76,   // 179
(short)1404,   // 180
(short) 446,   // 181
(short)1490,   // 182
(short) 542,   // 183
(short) 954,   // 184
(short)1162,   // 185
(short) 628,   // 186
(short) 674,   // 187
(short)1380,   // 188
(short) 658,   // 189
(short)1286,   // 190
(short) 124,   // 191
(short) 320,   // 192
(short) 314,   // 193
(short)1346,   // 194
(short) 328,   // 195
(short)1084,   // 196
(short) 478,   // 197
(short) 590,   // 198
(short) 996,   // 199
(short)1012,   // 200
(short) 744,   // 201
(short) 894,   // 202
(short) 902,   // 203
(short) 548,   // 204
(short)1458,   // 205
(short) 616,   // 206
(short) 382,   // 207
(short) 360,   // 208
(short)1398,   // 209
(short) 844,   // 210
(short)1512,   // 211
(short) 654,   // 212
(short) 324,   // 213
(short) 248,   // 214
(short)1000,   // 215
(short)1310,   // 216
(short) 898,   // 217
(short)1552,   // 218
(short) 568,   // 219
(short)1454,   // 220
(short)1474,   // 221
(short)1388,   // 222
(short)1220,   // 223
(short) 634,   // 224
(short) 620,   // 225
(short)1408,   // 226
(short) 378,   // 227
(short) 932,   // 228
(short) 426,   // 229
(short) 260,   // 230
(short) 386,   // 231
(short)1104,   // 232
(short) 822,   // 233
(short)1532,   // 234
(short)1272,   // 235
(short) 872,   // 236
(short)1502,   // 237
(short) 350,   // 238
(short)1342,   // 239
(short) 424,   // 240
(short) 934,   // 241
(short) 788,   // 242
(short) 512,   // 243
(short)1036,   // 244
(short) 364,   // 245
(short) 516,   // 246
(short)1054,   // 247
(short)1520,   // 248
(short) 156,   // 249
(short) 642,   // 250
(short)1318,   // 251
(short) 284,   // 252
(short)1228,   // 253
(short)1570,   // 254
(short) 942,   // 255
(short) 450,   // 256
(short) 112,   // 257
(short) 414,   // 258
(short)1070,   // 259
(short) 456,   // 260
(short)1634,   // 261
(short)1394,   // 262
(short) 454,   // 263
(short)1072,   // 264
(short)  20,   // 265
(short)1638,   // 266
(short) 862,   // 267
(short) 768,   // 268
(short) 132,   // 269
(short)1216,   // 270
(short) 614,   // 271
(short)1460,   // 272
(short) 714,   // 273
(short)1284,   // 274
(short) 660,   // 275
(short) 916,   // 276
(short)1364,   // 277
(short) 336,   // 278
(short)1258,   // 279
(short)   4,   // 280
(short)1470,   // 281
(short)1200,   // 282
(short)1092,   // 283
(short)1292,   // 284
(short) 578,   // 285
(short) 796,   // 286
(short) 802,   // 287
(short)1612,   // 288
(short) 930,   // 289
(short) 380,   // 290
(short) 618,   // 291
(short) 636,   // 292
(short)  64,   // 293
(short)1562,   // 294
(short) 530,   // 295
(short) 412,   // 296
(short) 114,   // 297
(short)1496,   // 298
(short) 652,   // 299
(short)1514,   // 300
(short)1194,   // 301
(short)1654,   // 302
(short) 876,   // 303
(short)  70,   // 304
(short)1668,   // 305
(short)  86,   // 306
(short)1280,   // 307
(short)  80,   // 308
(short)1250,   // 309
(short)1384,   // 310
(short) 724,   // 311
(short) 520,   // 312
(short) 194,   // 313
(short)1174,   // 314
(short)1402,   // 315
(short)  78,   // 316
(short)1282,   // 317
(short) 716,   // 318
(short)1032,   // 319
(short) 528,   // 320
(short)1564,   // 321
(short) 186,   // 322
(short) 214,   // 323
(short) 594,   // 324
(short)1204,   // 325
(short)1172,   // 326
(short) 196,   // 327
(short)1436,   // 328
(short)1628,   // 329
(short) 690,   // 330
(short)1314,   // 331
(short)1166,   // 332
(short) 646,   // 333
(short)1154,   // 334
(short)1118,   // 335
(short)1262,   // 336
(short)1440,   // 337
(short)1190,   // 338
(short) 100,   // 339
(short)1336,   // 340
(short)1182,   // 341
(short) 342,   // 342
(short) 500,   // 343
(short) 940,   // 344
(short)1572,   // 345
(short)1308,   // 346
(short)1002,   // 347
(short)1016,   // 348
(short)1078,   // 349
(short)1178,   // 350
(short) 742,   // 351
(short)1014,   // 352
(short)1004,   // 353
(short) 302,   // 354
(short) 866,   // 355
(short)1276,   // 356
(short)1632,   // 357
(short) 458,   // 358
(short)1048,   // 359
(short)1122,   // 360
(short)1428,   // 361
(short)1482,   // 362
(short) 246,   // 363
(short) 326,   // 364
(short)1348,   // 365
(short)1530,   // 366
(short) 824,   // 367
(short)  58,   // 368
(short) 624,   // 369
(short) 162,   // 370
(short) 494,   // 371
(short) 734,   // 372
(short)  10,   // 373
(short)1596,   // 374
(short) 708,   // 375
(short)1110,   // 376
(short) 228,   // 377
(short)1588,   // 378
(short)1130,   // 379
(short) 224,   // 380
(short) 208,   // 381
(short) 482,   // 382
(short) 794,   // 383
(short) 580,   // 384
(short) 232,   // 385
(short)1416,   // 386
(short) 556,   // 387
(short) 254,   // 388
(short) 950,   // 389
(short) 584,   // 390
(short) 104,   // 391
(short) 962,   // 392
(short) 650,   // 393
(short)1498,   // 394
(short) 566,   // 395
(short)1554,   // 396
(short)1362,   // 397
(short) 918,   // 398
(short)1372,   // 399
(short) 926,   // 400
(short) 976,   // 401
(short) 792,   // 402
(short) 484,   // 403
(short)  28,   // 404
(short)1650,   // 405
(short)  32,   // 406
(short)1444,   // 407
(short) 694,   // 408
(short) 760,   // 409
(short) 310,   // 410
(short)1136,   // 411
(short) 728,   // 412
(short)1098,   // 413
(short)1450,   // 414
(short)  94,   // 415
(short) 600,   // 416
(short) 504,   // 417
(short) 280,   // 418
(short) 838,   // 419
(short) 842,   // 420
(short)1400,   // 421
(short)1176,   // 422
(short)1080,   // 423
(short)1586,   // 424
(short) 230,   // 425
(short) 582,   // 426
(short) 952,   // 427
(short) 544,   // 428
(short)1370,   // 429
(short) 920,   // 430
(short) 986,   // 431
(short) 236,   // 432
(short)1648,   // 433
(short)  30,   // 434
(short)1652,   // 435
(short)1196,   // 436
(short) 888,   // 437
(short) 704,   // 438
(short) 754,   // 439
(short) 308,   // 440
(short) 762,   // 441
(short) 318,   // 442
(short) 126,   // 443
(short)1114,   // 444
(short) 182,   // 445
(short)1030,   // 446
(short) 718,   // 447
(short)1576,   // 448
(short)1096,   // 449
(short) 730,   // 450
(short)1426,   // 451
(short)1124,   // 452
(short) 264,   // 453
(short)1448,   // 454
(short)1100,   // 455
(short) 886,   // 456
(short)1198,   // 457
(short)1472,   // 458
(short)1456,   // 459
(short) 550,   // 460
(short)  92,   // 461
(short)1452,   // 462
(short) 570,   // 463
(short) 972,   // 464
(short)  84,   // 465
(short)1670,   // 466
(short) 946,   // 467
(short)1186,   // 468
(short)1518,   // 469
(short)1056,   // 470
(short)1622,   // 471
(short) 856,   // 472
(short) 150,   // 473
(short) 332,   // 474
(short)1354,   // 475
(short)1434,   // 476
(short) 198,   // 477
(short) 252,   // 478
(short) 558,   // 479
(short) 632,   // 480
(short)1222,   // 481
(short)  48,   // 482
(short) 404,   // 483
(short) 814,   // 484
(short)1378,   // 485
(short) 676,   // 486
(short) 666,   // 487
(short) 938,   // 488
(short) 502,   // 489
(short) 602,   // 490
(short) 664,   // 491
(short) 678,   // 492
(short) 372,   // 493
(short) 108,   // 494
(short) 740,   // 495
(short)1180,   // 496
(short)1338,   // 497
(short) 498,   // 498
(short) 344,   // 499
(short)1580,   // 500
(short) 490,   // 501
(short) 240,   // 502
(short) 418,   // 503
(short) 562,   // 504
(short) 526,   // 505
(short)1034,   // 506
(short) 514,   // 507
(short) 366,   // 508
(short) 990,   // 509
(short)  52,   // 510
(short) 244,   // 511
(short)1484,   // 512
(short) 508,   // 513
(short) 476,   // 514
(short)1086,   // 515
(short)1466,   // 516
(short)1494,   // 517
(short) 116,   // 518
(short)1152,   // 519
(short) 648,   // 520
(short) 964,   // 521
(short) 398,   // 522
(short)1602,   // 523
(short) 278,   // 524
(short) 506,   // 525
(short)1486,   // 526
(short)1160,   // 527
(short) 956,   // 528
(short) 296,   // 529
(short) 430,   // 530
(short)1600,   // 531
(short) 400,   // 532
(short)1594,   // 533
(short)  12,   // 534
(short)1610,   // 535
(short) 804,   // 536
(short)  26,   // 537
(short) 486,   // 538
(short) 166,   // 539
(short)1028,   // 540
(short) 184,   // 541
(short)1566,   // 542
(short)1268,   // 543
(short)1150,   // 544
(short) 118,   // 545
(short)1616,   // 546
(short)1044,   // 547
(short)1062,   // 548
(short)1300,   // 549
(short) 750,   // 550
(short)  68,   // 551
(short) 878,   // 552
(short) 884,   // 553
(short)1102,   // 554
(short) 388,   // 555
(short) 588,   // 556
(short) 480,   // 557
(short) 210,   // 558
(short)1676,   // 559
(short) 422,   // 560
(short)1344,   // 561
(short) 316,   // 562
(short) 764,   // 563
(short)1020,   // 564
(short) 396,   // 565
(short) 966,   // 566
(short) 220,   // 567
(short)1066,   // 568
(short) 464,   // 569
(short)1548,   // 570
(short) 912,   // 571
(short) 818,   // 572
(short)  42,   // 573
(short)1660,   // 574
(short) 608,   // 575
(short)1226,   // 576
(short) 286,   // 577
(short)  46,   // 578
(short)1224,   // 579
(short) 610,   // 580
(short)1266,   // 581
(short)1568,   // 582
(short)1230,   // 583
(short) 738,   // 584
(short) 110,   // 585
(short) 452,   // 586
(short)1396,   // 587
(short) 362,   // 588
(short)1038,   // 589
(short)1524,   // 590
(short) 160,   // 591
(short) 626,   // 592
(short)1164,   // 593
(short)1316,   // 594
(short) 644,   // 595
(short)1168,   // 596
(short) 892,   // 597
(short) 746,   // 598
(short)1256,   // 599
(short) 338,   // 600
(short)1330,   // 601
(short) 178,   // 602
(short) 808,   // 603
(short) 408,   // 604
(short) 148,   // 605
(short) 858,   // 606
(short) 576,   // 607
(short)1294,   // 608
(short)1420,   // 609
(short)1254,   // 610
(short) 748,   // 611
(short)1302,   // 612
(short) 272,   // 613
(short)1060,   // 614
(short)1046,   // 615
(short) 460,   // 616
(short) 292,   // 617
(short) 206,   // 618
(short) 226,   // 619
(short)1112,   // 620
(short) 128,   // 621
(short) 782,   // 622
(short) 370,   // 623
(short) 680,   // 624
(short)1432,   // 625
(short)1356,   // 626
(short)1026,   // 627
(short) 168,   // 628
(short) 836,   // 629
(short) 282,   // 630
(short)1320,   // 631
(short)1298,   // 632
(short)1064,   // 633
(short) 222,   // 634
(short)1132,   // 635
(short) 778,   // 636
(short) 786,   // 637
(short) 936,   // 638
(short) 668,   // 639
(short) 684,   // 640
(short)1090,   // 641
(short)1202,   // 642
(short) 596,   // 643
(short)1352,   // 644
(short) 334,   // 645
(short)1366,   // 646
(short)1360,   // 647
(short)1556,   // 648
(short) 394,   // 649
(short)1022,   // 650
(short) 300,   // 651
(short)1006,   // 652
(short)1052,   // 653
(short) 518,   // 654
(short) 726,   // 655
(short)1138,   // 656
(short) 190,   // 657
(short)1234,   // 658
(short) 276,   // 659
(short)1604,   // 660
(short)1646,   // 661
(short) 238,   // 662
(short) 492,   // 663
(short) 164,   // 664
(short) 488,   // 665
(short)1582,   // 666
(short) 640,   // 667
(short) 158,   // 668
(short)1526,   // 669
(short) 140,   // 670
(short)1674,   // 671
(short) 212,   // 672
(short) 188,   // 673
(short)1140,   // 674
(short)1326,   // 675
(short) 176,   // 676
(short)1332,   // 677
(short) 470,   // 678
(short)1464,   // 679
(short)1088,   // 680
(short) 686,   // 681
(short) 172,   // 682
(short)1480,   // 683
(short)1430,   // 684
(short) 682,   // 685
(short) 670,   // 686
(short) 702,   // 687
(short) 890,   // 688
(short)1170,   // 689
(short)1206,   // 690
(short)  18,   // 691
(short)1074,   // 692
(short)1248,   // 693
(short)  82,   // 694
(short) 974,   // 695
(short) 928,   // 696
(short)1614,   // 697
(short) 120,   // 698
(short) 834,   // 699
(short) 170,   // 700
(short) 688,   // 701
(short)1630,   // 702
(short)1278,   // 703
(short)  88,   // 704
(short) 984,   // 705
(short) 922,   // 706
(short) 376,   // 707
(short)1410,   // 708
(short)1608,   // 709
(short)  14,   // 710
(short)1620,   // 711
(short)1058,   // 712
(short) 274,   // 713
(short)1236,   // 714
(short)1158,   // 715
(short)1488,   // 716
(short) 448,   // 717
(short) 944,   // 718
(short)1672,   // 719
(short) 142,   // 720
(short) 776,   // 721
(short)1134,   // 722
(short) 312,   // 723
(short) 322,   // 724
(short) 656,   // 725
(short)1382,   // 726
(short)1252,   // 727
(short)1422,   // 728
(short)1290,   // 729
(short)1094,   // 730
(short)1578,   // 731
(short) 346,   // 732
(short)1212,   // 733
(short) 832,   // 734
(short) 122,   // 735
(short)1288,   // 736
(short)1424,   // 737
(short) 732,   // 738
(short) 496,   // 739
(short)1340,   // 740
(short) 352,   // 741
(short)1506,   // 742
(short) 202,   // 743
(short)1626,   // 744
(short)1438,   // 745
(short)1264,   // 746
(short) 612,   // 747
(short)1218,   // 748
(short)1390,   // 749
(short) 774,   // 750
(short) 144,   // 751
(short) 136,   // 752
(short) 440,   // 753
(short) 534,   // 754
(short) 830,   // 755
(short)1214,   // 756
(short) 134,   // 757
(short) 146,   // 758
(short) 410,   // 759
(short) 532,   // 760
(short) 442,   // 761
(short) 524,   // 762
(short) 564,   // 763
(short)1500,   // 764
(short) 874,   // 765
(short)1656,   // 766
(short)1108,   // 767
(short) 710,   // 768
(short) 828,   // 769
(short) 536,   // 770
(short)  38,   // 771
(short)1128,   // 772
(short)1590,   // 773
(short) 698,   // 774
(short) 722,   // 775
(short)1386,   // 776
(short)1476,   // 777
(short)  56,   // 778
(short) 826,   // 779
(short) 712,   // 780
(short)1462,   // 781
(short) 472,   // 782
(short)1664,   // 783
(short)  62,   // 784
(short) 638,   // 785
(short)1584,   // 786
(short)1082,   // 787
(short) 330,   // 788
(short) 152,   // 789
(short)1542,   // 790
(short)1242,   // 791
(short) 358,   // 792
(short) 384,   // 793
(short) 262,   // 794
(short)1126,   // 795
(short)  40,   // 796
(short) 820,   // 797
(short)1106,   // 798
(short)1658,   // 799
(short)  44,   // 800
(short) 288,   // 801
(short) 908,   // 802
(short)1376,   // 803
(short) 816,   // 804
(short) 914,   // 805
(short) 662,   // 806
(short) 604,   // 807
(short) 434,   // 808
(short) 812,   // 809
(short) 406,   // 810
(short) 810,   // 811
(short) 436,   // 812
(short)1324,   // 813
(short)1142,   // 814
(short)1644,   // 815
(short)1606,   // 816
(short)1412,   // 817
(short) 800,   // 818
(short) 798,   // 819
(short)1414,   // 820
(short) 234,   // 821
(short) 988,   // 822
(short) 368,   // 823
(short) 784,   // 824
(short) 780,   // 825
(short) 130,   // 826
(short) 770,   // 827
(short) 306,   // 828
(short) 756,   // 829
(short)1306,   // 830
(short)1574,   // 831
(short) 720,   // 832
(short) 700,   // 833
(short) 672,   // 834
(short) 630,   // 835
(short) 560,   // 836
(short) 420,   // 837
(short)   0,   // 838
(short)   0
};
