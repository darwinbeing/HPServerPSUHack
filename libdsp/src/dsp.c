/****************************************************************************
*
* $Source: /cvs/c30_dsp/src/dsp.c,v $
* $Revision: 1.2 $
*
* Copyright 2002, Microchip, Inc.  All rights reserved.
*
* Main controller to test DSP Libary operations.
*
* $Log: dsp.c,v $
* Revision 1.2  2003/07/23 18:27:27  curtiss
*
* : Modified Files:
* : 	Readme.html build.txt makefile bin/delog bin/gendata
* : 	bin/release bin/testops data/factors.txt
* : 	lib/PIC30v1_10_beta_04/libdsp.a src/dsp.c src/frct2flt.c
* : 	src/initbart.c src/initblck.c src/initcosf.c src/inithamm.c
* : 	src/inithann.c src/initkais.c src/inittwid.c src/minv.c
* : 	tst/devv05.cmd tst/devv06.cmd tst/devv07.cmd tst/devv1_00.cmd
* : 	tst/devv1_00_05.cmd tst/devv1_10_beta_01.cmd
* : 	tst/devv1_10_beta_04.cmd tst/ds.exe tst/dspv05.cmd
* : 	tst/dspv06.cmd tst/dspv07.cmd tst/dspv1_00.cmd
* : 	tst/dspv1_00_05.cmd tst/dspv1_10_beta_01.cmd
* : 	tst/dspv1_10_beta_04.cmd tst/tstreprt.txt tst/tstreprt_hw.txt
* : Added Files:
* : 	data/1sw1_10_1_2ulnhrd0_256.dat
* : 	data/2sw1_10_1_2ulnhrd0_128.dat
* : 	data/2sw1_10_1_2ulnhrd0_256.dat
* : 	data/2sw1_10_1_2ulnhrpod30_256.dat
* : 	data/4sw1_10_1_2hrd30_512.dat
* : 	data/bp1_10_1_5_1_3_1_25iirlatt10.dat data/bpiirlatt8.dat
* : 	data/c1_3frd0_256.dat data/c1_3frd0_32.dat
* : 	data/c1_3hrd0_256.dat data/c1_3hrd0_256_ci.dat
* : 	data/c1_3hrd0_256_cr.dat data/c1_3hrd0_256_frq.dat
* : 	data/c1_3hrd0_32.dat data/c1_3hrd0_32_ci.dat
* : 	data/c1_3hrd0_32_cr.dat data/c1_3ps1_3fr_32.dat
* : 	data/c1_3ps1_9pfr_32.dat data/c1_3ps1_9phr_32.dat
* : 	data/c1_3ps1_9pulnhr_128.dat data/c1_3ps1_9pulnhr_256.dat
* : 	data/c1_3ps1_9pulnhr_256_frq.dat data/c1_3ps1_9pulnhr_32.dat
* : 	data/c1_3ulnfrd0_256.dat data/c1_3ulnfrd0_32.dat
* : 	data/c1_3ulnhrd0_128.dat data/c1_3ulnhrd0_256.dat
* : 	data/c1_3ulnhrd0_256_ci.dat data/c1_3ulnhrd0_256_cr.dat
* : 	data/c1_3ulnhrd0_32.dat data/c1_3ulnhrd0_32_ci.dat
* : 	data/c1_3ulnhrd0_32_cr.dat data/c1_3ulnhrd0_64.dat
* : 	data/c1_3ulnhrd0_64_cr.dat data/c1_3ulnhrd0_64_frq.dat
* : 	data/c1_3ulnqrd0_256.dat data/c1_3ulnqrd0_64.dat
* : 	data/c1_3ulnqrd0_64_frq.dat data/c1_9pfrd0_256.dat
* : 	data/c1_9pfrd0_32.dat data/c1_9phrd0_256.dat
* : 	data/c1_9phrd0_256_ci.dat data/c1_9phrd0_256_cr.dat
* : 	data/c1_9phrd0_32.dat data/c1_9phrd0_32_ci.dat
* : 	data/c1_9phrd0_32_cr.dat data/c1_9pulnfrd0_256.dat
* : 	data/c1_9pulnfrd0_32.dat data/c1_9pulnhrd0_18x18.dat
* : 	data/c1_9pulnhrd0_256.dat data/c1_9pulnhrd0_256_ci.dat
* : 	data/c1_9pulnhrd0_256_cr.dat data/c1_9pulnhrd0_256_frq.dat
* : 	data/c1_9pulnhrd0_32.dat data/c1_9pulnhrd0_32_ci.dat
* : 	data/c1_9pulnhrd0_32_cr.dat data/c1_9pulnqrd0_256.dat
* : 	data/cos_128_cc.dat data/cos_16_cc.dat data/cos_32_cc.dat
* : 	data/cos_64_cc.dat data/cos_8_cc.dat data/e1_3hrd0_256_cc.dat
* : 	data/e1_3hrd0_32_cc.dat data/e1_3hrpod0_256_cc.dat
* : 	data/e1_3hrpod0_32_cc.dat data/e1_3ulnhrd0_128_cc.dat
* : 	data/e1_3ulnhrd0_32_cc.dat data/e1_3ulnhrd0_64_cc.dat
* : 	data/e1_9phrd0_256_cc.dat data/e1_9phrd0_32_cc.dat
* : 	data/e1_9phrpod0_256_cc.dat data/e1_9phrpod0_32_cc.dat
* : 	data/e1_9pulnhrd0_32_cc.dat data/flatt3.dat
* : 	data/high1_4_1_5iirc9.dat data/high1_4_1_5iirt9.dat
* : 	data/high1_5_1_3iirlatt6.dat data/ifrd0_100.dat
* : 	data/ifrd0_128.dat data/ifrd0_256.dat data/ifrd0_64.dat
* : 	data/ifrd22_128.dat data/ihrd0_256.dat data/ihrd12_128.dat
* : 	data/indata.dat data/iqrd0_256.dat data/iuhnfrd0_100.dat
* : 	data/iuhnhrd0_256.dat data/iulnfrd0_128.dat
* : 	data/iulnfrd22_128.dat data/iulnfrpod22_128.dat
* : 	data/iulnhrd0_128.dat data/iulnhrpod22_128.dat
* : 	data/iulnqrpod22_300.dat data/iuqnhrd0_256.dat
* : 	data/low1_4_1_2ham27t.dat data/low1_5_1_3iirlatt6.dat
* : 	data/low1_6_1_2eblk35t.dat data/low1_6_1_3iirc6.dat
* : 	data/low1_6_1_3iirt6.dat data/low1_6_1_4han75t.dat
* : 	data/low1_8_1_3iirlatt5.dat data/low1_8_1_5iirc5_40dB.dat
* : 	data/low1_8_1_5iirc7.dat data/low1_8_1_5iirt5_40dB.dat
* : 	data/low1_8_1_5iirt7.dat data/ref_0.dat data/ref_1.dat
* : 	data/ref_10.dat data/ref_100.dat data/ref_101.dat
* : 	data/ref_102.dat data/ref_103.dat data/ref_104.dat
* : 	data/ref_105.dat data/ref_106.dat data/ref_107.dat
* : 	data/ref_108.dat data/ref_109.dat data/ref_11.dat
* : 	data/ref_110.dat data/ref_111.dat data/ref_112.dat
* : 	data/ref_113.dat data/ref_114.dat data/ref_115.dat
* : 	data/ref_116.dat data/ref_117.dat data/ref_118.dat
* : 	data/ref_119.dat data/ref_12.dat data/ref_120.dat
* : 	data/ref_121.dat data/ref_122.dat data/ref_123.dat
* : 	data/ref_124.dat data/ref_125.dat data/ref_126.dat
* : 	data/ref_127.dat data/ref_128.dat data/ref_129.dat
* : 	data/ref_13.dat data/ref_130.dat data/ref_131.dat
* : 	data/ref_132.dat data/ref_133.dat data/ref_134.dat
* : 	data/ref_135.dat data/ref_136.dat data/ref_137.dat
* : 	data/ref_138.dat data/ref_139.dat data/ref_14.dat
* : 	data/ref_140.dat data/ref_141.dat data/ref_142.dat
* : 	data/ref_143.dat data/ref_144.dat data/ref_145.dat
* : 	data/ref_146.dat data/ref_147.dat data/ref_148.dat
* : 	data/ref_149.dat data/ref_15.dat data/ref_150.dat
* : 	data/ref_151.dat data/ref_152.dat data/ref_153.dat
* : 	data/ref_154.dat data/ref_155.dat data/ref_156.dat
* : 	data/ref_157.dat data/ref_158.dat data/ref_159.dat
* : 	data/ref_16.dat data/ref_160.dat data/ref_161.dat
* : 	data/ref_162.dat data/ref_163.dat data/ref_164.dat
* : 	data/ref_165.dat data/ref_166.dat data/ref_167.dat
* : 	data/ref_168.dat data/ref_169.dat data/ref_17.dat
* : 	data/ref_170.dat data/ref_171.dat data/ref_172.dat
* : 	data/ref_173.dat data/ref_174.dat data/ref_175.dat
* : 	data/ref_176.dat data/ref_177.dat data/ref_178.dat
* : 	data/ref_179.dat data/ref_18.dat data/ref_180.dat
* : 	data/ref_181.dat data/ref_182.dat data/ref_183.dat
* : 	data/ref_184.dat data/ref_185.dat data/ref_186.dat
* : 	data/ref_187.dat data/ref_188.dat data/ref_189.dat
* : 	data/ref_19.dat data/ref_190.dat data/ref_191.dat
* : 	data/ref_192.dat data/ref_193.dat data/ref_194.dat
* : 	data/ref_195.dat data/ref_196.dat data/ref_197.dat
* : 	data/ref_198.dat data/ref_199.dat data/ref_2.dat
* : 	data/ref_20.dat data/ref_200.dat data/ref_201.dat
* : 	data/ref_202.dat data/ref_203.dat data/ref_204.dat
* : 	data/ref_205.dat data/ref_206.dat data/ref_207.dat
* : 	data/ref_208.dat data/ref_209.dat data/ref_21.dat
* : 	data/ref_210.dat data/ref_211.dat data/ref_212.dat
* : 	data/ref_213.dat data/ref_214.dat data/ref_215.dat
* : 	data/ref_216.dat data/ref_217.dat data/ref_218.dat
* : 	data/ref_219.dat data/ref_22.dat data/ref_220.dat
* : 	data/ref_221.dat data/ref_222.dat data/ref_223.dat
* : 	data/ref_224.dat data/ref_225.dat data/ref_226.dat
* : 	data/ref_227.dat data/ref_228.dat data/ref_229.dat
* : 	data/ref_23.dat data/ref_230.dat data/ref_231.dat
* : 	data/ref_232.dat data/ref_233.dat data/ref_234.dat
* : 	data/ref_235.dat data/ref_236.dat data/ref_237.dat
* : 	data/ref_238.dat data/ref_239.dat data/ref_24.dat
* : 	data/ref_240.dat data/ref_241.dat data/ref_242.dat
* : 	data/ref_243.dat data/ref_244.dat data/ref_245.dat
* : 	data/ref_246.dat data/ref_247.dat data/ref_248.dat
* : 	data/ref_249.dat data/ref_25.dat data/ref_250.dat
* : 	data/ref_251.dat data/ref_252.dat data/ref_253.dat
* : 	data/ref_254.dat data/ref_255.dat data/ref_256.dat
* : 	data/ref_257.dat data/ref_258.dat data/ref_259.dat
* : 	data/ref_26.dat data/ref_260.dat data/ref_261.dat
* : 	data/ref_262.dat data/ref_263.dat data/ref_264.dat
* : 	data/ref_265.dat data/ref_266.dat data/ref_267.dat
* : 	data/ref_27.dat data/ref_28.dat data/ref_29.dat data/ref_3.dat
* : 	data/ref_30.dat data/ref_31.dat data/ref_32.dat
* : 	data/ref_33.dat data/ref_34.dat data/ref_35.dat
* : 	data/ref_36.dat data/ref_37.dat data/ref_38.dat
* : 	data/ref_39.dat data/ref_4.dat data/ref_40.dat data/ref_41.dat
* : 	data/ref_42.dat data/ref_43.dat data/ref_44.dat
* : 	data/ref_45.dat data/ref_46.dat data/ref_47.dat
* : 	data/ref_48.dat data/ref_49.dat data/ref_5.dat data/ref_50.dat
* : 	data/ref_51.dat data/ref_52.dat data/ref_53.dat
* : 	data/ref_54.dat data/ref_55.dat data/ref_56.dat
* : 	data/ref_57.dat data/ref_58.dat data/ref_59.dat data/ref_6.dat
* : 	data/ref_60.dat data/ref_61.dat data/ref_62.dat
* : 	data/ref_63.dat data/ref_64.dat data/ref_65.dat
* : 	data/ref_66.dat data/ref_67.dat data/ref_68.dat
* : 	data/ref_69.dat data/ref_7.dat data/ref_70.dat data/ref_71.dat
* : 	data/ref_72.dat data/ref_73.dat data/ref_74.dat
* : 	data/ref_75.dat data/ref_76.dat data/ref_77.dat
* : 	data/ref_78.dat data/ref_79.dat data/ref_8.dat data/ref_80.dat
* : 	data/ref_81.dat data/ref_82.dat data/ref_83.dat
* : 	data/ref_84.dat data/ref_85.dat data/ref_86.dat
* : 	data/ref_87.dat data/ref_88.dat data/ref_89.dat data/ref_9.dat
* : 	data/ref_90.dat data/ref_91.dat data/ref_92.dat
* : 	data/ref_93.dat data/ref_94.dat data/ref_95.dat
* : 	data/ref_96.dat data/ref_97.dat data/ref_98.dat
* : 	data/ref_99.dat data/s1_3frd0_256.dat data/s1_3frd0_32.dat
* : 	data/s1_3frd30_256.dat data/s1_3frd30_32.dat
* : 	data/s1_3hrd0_256.dat data/s1_3hrd0_256_ci.dat
* : 	data/s1_3hrd0_32.dat data/s1_3hrd0_32_ci.dat
* : 	data/s1_3hrd30_256.dat data/s1_3hrd30_32.dat
* : 	data/s1_3ulnfrd0_256.dat data/s1_3ulnfrd0_32.dat
* : 	data/s1_3ulnfrd30_256.dat data/s1_3ulnfrd30_32.dat
* : 	data/s1_3ulnhrd0_256.dat data/s1_3ulnhrd0_256_ci.dat
* : 	data/s1_3ulnhrd0_256_cr.dat data/s1_3ulnhrd0_32.dat
* : 	data/s1_3ulnhrd0_32_ci.dat data/s1_3ulnhrd0_32_cr.dat
* : 	data/s1_3ulnhrd0_64.dat data/s1_3ulnhrd0_64_ci.dat
* : 	data/s1_3ulnhrd0_64_frq.dat data/s1_3ulnhrd30_256.dat
* : 	data/s1_3ulnhrd30_256_frq.dat data/s1_3ulnhrd30_32.dat
* : 	data/s1_9pfrd0_256.dat data/s1_9pfrd0_32.dat
* : 	data/s1_9pfrd30_256.dat data/s1_9pfrd30_32.dat
* : 	data/s1_9phrd0_256.dat data/s1_9phrd0_256_ci.dat
* : 	data/s1_9phrd0_32.dat data/s1_9phrd0_32_ci.dat
* : 	data/s1_9phrd0_64.dat data/s1_9phrd0_64_frq.dat
* : 	data/s1_9phrd30_256.dat data/s1_9phrd30_256_frq.dat
* : 	data/s1_9phrd30_32.dat data/s1_9phrd30_64.dat
* : 	data/s1_9pulnfrd0_256.dat data/s1_9pulnfrd0_32.dat
* : 	data/s1_9pulnfrd30_256.dat data/s1_9pulnfrd30_32.dat
* : 	data/s1_9pulnhrd0_128.dat data/s1_9pulnhrd0_256.dat
* : 	data/s1_9pulnhrd0_256_ci.dat data/s1_9pulnhrd0_256_cr.dat
* : 	data/s1_9pulnhrd0_32.dat data/s1_9pulnhrd0_32_ci.dat
* : 	data/s1_9pulnhrd0_32_cr.dat data/s1_9pulnhrd30_128.dat
* : 	data/s1_9pulnhrd30_256.dat data/s1_9pulnhrd30_32.dat
* : 	data/s1_9pulnhrd30_64.dat data/s1_9pulnhrpod0_256.dat
* : 	data/sq1_7frd0_256.dat data/sq1_7hrd60_256.dat
* : 	data/sq1_7ulnhrd60_256.dat data/sq1_7ulnhrd60_256_frq.dat
* : 	data/sq1_7ulnhrd60_64.dat data/sq1_7ulnhrd60_64_frq.dat
* : 	data/sq1_7ulnhrpod60_256.dat data/sq1_7ulnqrd60_64.dat
* : 	data/step600.dat data/t1_3hrd20_256.dat data/t1_3hrd20_64.dat
* : 	data/t1_3hrd20_64_frq.dat data/t1_3qrd30_64.dat
* : 	data/t1_3qrd30_64_frq.dat data/t1_3ulnhrpod60_128.dat
* : 	data/t1_3ulnqrd20_64.dat data/t1_3ulnqrd20_64_frq.dat
* : 	data/t1_9pulnhrpod60_128.dat data/t1_9pulnhrpod60_128_frq.dat
* : 	data/t1_9qrd30_256.dat data/t1_9ulnqrd30_256.dat
* : 	data/t1_9ulntrd30_128.dat data/tst_0.dat data/tst_1.dat
* : 	data/tst_10.dat data/tst_100.dat data/tst_101.dat
* : 	data/tst_102.dat data/tst_103.dat data/tst_104.dat
* : 	data/tst_105.dat data/tst_106.dat data/tst_107.dat
* : 	data/tst_108.dat data/tst_109.dat data/tst_11.dat
* : 	data/tst_110.dat data/tst_111.dat data/tst_112.dat
* : 	data/tst_113.dat data/tst_114.dat data/tst_115.dat
* : 	data/tst_116.dat data/tst_117.dat data/tst_118.dat
* : 	data/tst_119.dat data/tst_12.dat data/tst_120.dat
* : 	data/tst_121.dat data/tst_122.dat data/tst_123.dat
* : 	data/tst_124.dat data/tst_125.dat data/tst_126.dat
* : 	data/tst_127.dat data/tst_128.dat data/tst_129.dat
* : 	data/tst_13.dat data/tst_130.dat data/tst_131.dat
* : 	data/tst_132.dat data/tst_133.dat data/tst_134.dat
* : 	data/tst_135.dat data/tst_136.dat data/tst_137.dat
* : 	data/tst_138.dat data/tst_139.dat data/tst_14.dat
* : 	data/tst_140.dat data/tst_141.dat data/tst_142.dat
* : 	data/tst_143.dat data/tst_144.dat data/tst_145.dat
* : 	data/tst_146.dat data/tst_147.dat data/tst_148.dat
* : 	data/tst_149.dat data/tst_15.dat data/tst_150.dat
* : 	data/tst_151.dat data/tst_152.dat data/tst_153.dat
* : 	data/tst_154.dat data/tst_155.dat data/tst_156.dat
* : 	data/tst_157.dat data/tst_158.dat data/tst_159.dat
* : 	data/tst_16.dat data/tst_160.dat data/tst_161.dat
* : 	data/tst_162.dat data/tst_163.dat data/tst_164.dat
* : 	data/tst_165.dat data/tst_166.dat data/tst_167.dat
* : 	data/tst_168.dat data/tst_169.dat data/tst_17.dat
* : 	data/tst_170.dat data/tst_171.dat data/tst_172.dat
* : 	data/tst_173.dat data/tst_174.dat data/tst_175.dat
* : 	data/tst_176.dat data/tst_177.dat data/tst_178.dat
* : 	data/tst_179.dat data/tst_18.dat data/tst_180.dat
* : 	data/tst_181.dat data/tst_182.dat data/tst_183.dat
* : 	data/tst_184.dat data/tst_185.dat data/tst_186.dat
* : 	data/tst_187.dat data/tst_188.dat data/tst_189.dat
* : 	data/tst_19.dat data/tst_190.dat data/tst_191.dat
* : 	data/tst_192.dat data/tst_193.dat data/tst_194.dat
* : 	data/tst_195.dat data/tst_196.dat data/tst_197.dat
* : 	data/tst_198.dat data/tst_199.dat data/tst_2.dat
* : 	data/tst_20.dat data/tst_200.dat data/tst_201.dat
* : 	data/tst_202.dat data/tst_203.dat data/tst_204.dat
* : 	data/tst_205.dat data/tst_206.dat data/tst_207.dat
* : 	data/tst_208.dat data/tst_209.dat data/tst_21.dat
* : 	data/tst_210.dat data/tst_211.dat data/tst_212.dat
* : 	data/tst_213.dat data/tst_214.dat data/tst_215.dat
* : 	data/tst_216.dat data/tst_217.dat data/tst_218.dat
* : 	data/tst_219.dat data/tst_22.dat data/tst_220.dat
* : 	data/tst_221.dat data/tst_222.dat data/tst_223.dat
* : 	data/tst_224.dat data/tst_225.dat data/tst_226.dat
* : 	data/tst_227.dat data/tst_228.dat data/tst_229.dat
* : 	data/tst_23.dat data/tst_230.dat data/tst_231.dat
* : 	data/tst_232.dat data/tst_233.dat data/tst_234.dat
* : 	data/tst_235.dat data/tst_236.dat data/tst_237.dat
* : 	data/tst_238.dat data/tst_239.dat data/tst_24.dat
* : 	data/tst_240.dat data/tst_241.dat data/tst_242.dat
* : 	data/tst_243.dat data/tst_244.dat data/tst_245.dat
* : 	data/tst_246.dat data/tst_247.dat data/tst_248.dat
* : 	data/tst_249.dat data/tst_25.dat data/tst_250.dat
* : 	data/tst_251.dat data/tst_252.dat data/tst_253.dat
* : 	data/tst_254.dat data/tst_255.dat data/tst_256.dat
* : 	data/tst_257.dat data/tst_258.dat data/tst_259.dat
* : 	data/tst_26.dat data/tst_260.dat data/tst_261.dat
* : 	data/tst_262.dat data/tst_263.dat data/tst_264.dat
* : 	data/tst_265.dat data/tst_266.dat data/tst_267.dat
* : 	data/tst_27.dat data/tst_28.dat data/tst_29.dat data/tst_3.dat
* : 	data/tst_30.dat data/tst_31.dat data/tst_32.dat
* : 	data/tst_33.dat data/tst_34.dat data/tst_35.dat
* : 	data/tst_36.dat data/tst_37.dat data/tst_38.dat
* : 	data/tst_39.dat data/tst_4.dat data/tst_40.dat data/tst_41.dat
* : 	data/tst_42.dat data/tst_43.dat data/tst_44.dat
* : 	data/tst_45.dat data/tst_46.dat data/tst_47.dat
* : 	data/tst_48.dat data/tst_49.dat data/tst_5.dat data/tst_50.dat
* : 	data/tst_51.dat data/tst_52.dat data/tst_53.dat
* : 	data/tst_54.dat data/tst_55.dat data/tst_56.dat
* : 	data/tst_57.dat data/tst_58.dat data/tst_59.dat data/tst_6.dat
* : 	data/tst_60.dat data/tst_61.dat data/tst_62.dat
* : 	data/tst_63.dat data/tst_64.dat data/tst_65.dat
* : 	data/tst_66.dat data/tst_67.dat data/tst_68.dat
* : 	data/tst_69.dat data/tst_7.dat data/tst_70.dat data/tst_71.dat
* : 	data/tst_72.dat data/tst_73.dat data/tst_74.dat
* : 	data/tst_75.dat data/tst_76.dat data/tst_77.dat
* : 	data/tst_78.dat data/tst_79.dat data/tst_8.dat data/tst_80.dat
* : 	data/tst_81.dat data/tst_82.dat data/tst_83.dat
* : 	data/tst_84.dat data/tst_85.dat data/tst_86.dat
* : 	data/tst_87.dat data/tst_88.dat data/tst_89.dat data/tst_9.dat
* : 	data/tst_90.dat data/tst_91.dat data/tst_92.dat
* : 	data/tst_93.dat data/tst_94.dat data/tst_95.dat
* : 	data/tst_96.dat data/tst_97.dat data/tst_98.dat
* : 	data/tst_99.dat data/tstreprt.dat data/twid_128_cc.dat
* : 	data/twid_16_cc.dat data/twid_256_cc.dat data/twid_32_cc.dat
* : 	data/twid_64_cc.dat data/twid_8_cc.dat data/twid_cc_128_cc.dat
* : 	data/twid_cc_16_cc.dat data/twid_cc_256_cc.dat
* : 	data/twid_cc_32_cc.dat data/twid_cc_64_cc.dat
* : 	data/twid_cc_8_cc.dat data/unfr11x15.dat data/unfr128_cc.dat
* : 	data/unfr15x11.dat data/unfr256.dat data/unfr256_ci.dat
* : 	data/unfr256_cr.dat data/unfr32_ci.dat data/unfr32_cr.dat
* : 	data/unhr11x15.dat data/unhr128_cc.dat data/unhr15x11.dat
* : 	data/unhr256.dat data/unhr256_ci.dat data/unhr256_cr.dat
* : 	data/unhr32_ci.dat data/unhr32_cr.dat data/unhrpo16x16.dat
* : 	data/unhrpo256.dat data/unor21x21.dat data/unqr11x15.dat
* : 	data/unqr12x12.dat data/unqr15x11.dat data/unqr20x5.dat
* : 	data/unqr21x21.dat data/unqr256.dat data/unqr5x20.dat
* : 	data/unqrno20x5.dat data/unqrno256.dat data/unqrno5x20.dat
* : 	data/unsr11x15.dat data/unsr128.dat data/unsr128_cc.dat
* : 	data/unsr15x11.dat data/unsr15x15.dat data/unsr256.dat
* : 	data/unsr256_ci.dat data/unsr256_cr.dat data/unsr256_frq.dat
* : 	data/unsr32_ci.dat data/unsr32_cr.dat data/unsr64.dat
* : ----------------------------------------------------------------------
*
*  Update to version 6; add the built testing files to ease future testing.
*  The data/*.dat files should be deleted and re-generated with each UPDATE!
*
****************************************************************************/

/* Global headers. */
#include <stdio.h>		/* printf,FILE,f* */
#include <stdlib.h>		/* malloc, NULL */
#include <string.h>		/* memcpy */

/*...........................................................................*/

/* Local headers. */
#include "dsp.h"				/* DSP Library interface */
#include "testing.h"				/* testing interface */
#if	DATA_TYPE==FRACTIONAL		/* [ */
#ifndef	IAR_TOOLS			/* [ */
#include "p30f6014.h"				/* dsPIC specifics */
#else	/* ][ */	/* IAR */
#include "io30f6014.h"				/* dsPIC specifics */
#endif	/* ]  */
#endif	/* ]  */

/* Local defines. */
#ifdef	IN_SPACE			/* [ */
#ifndef PSVPAGE				/* [ */
#define PSVPAGE(fn,var)							\
	   asm  ("mov #psvpage(" #fn "),w7;\n\t"			\
                 "mov w7, %0" : "=g"(var) : /* inputs */ : "w7" );
#endif	/* ]  */
#ifndef PSVOFFSET			/* [ */
#define PSVOFFSET(fn,var)						\
	   asm  ("mov #psvoffset(" #fn "),w7;\n\t"			\
                 "mov w7, %0" : "=g"(var) : /* inputs */ : "w7" )
#endif	/* ]  */
#endif	/* ]  */

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */
#define	IN_FILE_NAME		"data/indata.dat" /* fixed input data file */
#else	/* ][ */
#define	IN_FILE_NAME		"indata.txt"	/* fixed input data file */
						/* path from -I./data */
#endif	/* ]  */

#if	VALIDATION==MPLAB_VAL		/* [ MPLAB C30 validation */
#define	SCALE			32768.0		/* transforms double results */
						/* into "equivalent" decimal */
#define ENABLE_TMR_ONE_TO_ONE	0x8000		/* for cycle count */
#define MAX_PERIOD		0xFFFF		/* for cycle count */
#endif	/* ]  */

#define	FACTORS_FILE		"factors.txt"	/* fixed factors data file */
						/* path from -I./data */

#define	MAX_WORDS_YMEM		2048		/* 2 KWords of Y available */
						/* for allocation of data */
						/* samples and/or delays */
						/* Note: not all available */
						/* memory may be used for */
						/* data samples and/or delays */
#define	MAX_WORDS_XMEM		2048		/* 2 KWords of X available */
						/* for allocation of filter */
						/* coefficients or transform */
						/* factors */
						/* Note: not all available */
						/* memory may be used for */
						/* coefficients or factors */
#define	MAX_WORDS_PMEM		MAX_WORDS_XMEM	/* 2 KWords of P available */
						/* if coefficients or factors */
						/* are allocated in P */
#define	MAX_SAMPLE_SETS		3		/* for vector and matrix ops */
						/* max number of sample sets */
						/* is 3: dst, src1, src2 when */
						/* out of place; for FIR ops */
						/* is also 3: dst, src, ref; */
						/* for IIR filters is 2: dst, */
						/* src; and for transforms */
						/* is 2: dst, src when out */
						/* of place (complex) */
#define MAX_WORDS_SAMPLES	1536		/* max number sample words */
						/* 3/4 of 2 KWords assigned */
						/* to data samples */
#define	MAX_WORDS_SAMPLE_SET	MAX_WORDS_SAMPLES/MAX_SAMPLE_SETS
						/* max words per sample set */
						/* is 512; for real data this */
						/* allows 512 samples; for */
						/* complex, only 256 samples */
#define MAX_WORDS_FACTORS	MAX_WORDS_SAMPLE_SET
						/* max number factor words */
						/* used in transforms; since */
						/* factors are complex, max */
						/* number of factors is */
						/* MAX_WORDS_FACTORS/2 = 256 */
						/* Note: for DCT there are */
						/* cosine and twiddle factors */
						/* so, no more than 128 each */
						/* enough for a 256-point DCT */
#define MAX_WORDS_COEFFS	128		/* max number words for */
						/* filter coefficients; use */
						/* same memory as factors */
						/* (chosen conservatively) */
#define MAX_WORDS_DELAYS	MAX_WORDS_COEFFS /* max number delay words */

#define Buf_X __attribute__((__section__(".xbss")))
#define Buf_Y __attribute__((__section__(".ybss")))
#define ModBuf_X(k) __attribute__((__section__(".xbss"), aligned(2*k)))
#define ModBuf_Y(k) __attribute__((__section__(".ybss"), aligned(2*k)))
#define ModBuf_P(k) __attribute__((__section__(".const, r"), aligned(2*k)))

/* Local variables. */
#if	VALIDATION==MPLAB_VAL		/* [ MPLAB C30 validation */
#ifndef	IAR_TOOLS			/* [ */
static const double indata[] = {
#else	/* ][ */
#pragma location = "CONST_DATA_IN"
const __constptr double indata[] = {
#endif	/* ]  */
#include IN_FILE_NAME				/* values from data file */
0};
unsigned int test_time = 0;			/* reports cycle count */
#endif	/* ]  */

/* Global memory allocation. */

#ifdef	IN_SPACE			/* [ */
#ifndef	IAR_TOOLS			/* [ */
fractional BufferYMEM[MAX_WORDS_SAMPLES] ModBuf_Y(MAX_WORDS_DELAYS);
						/* usable Y memory for data */
fractional ModBufferYMEM[MAX_WORDS_DELAYS] ModBuf_Y(MAX_WORDS_DELAYS);
						/* usable Y memory for dels */
fractional BufferXMEM[MAX_WORDS_FACTORS] Buf_X;	/* usable X memory for data */
fractional ModBufferXMEM[MAX_WORDS_FACTORS] ModBuf_X(MAX_WORDS_FACTORS);
						/* usable X memory for trans */
						/* factors or filter coeffs */
const fractional ModBufferPMEM[MAX_WORDS_FACTORS] ModBuf_P(MAX_WORDS_FACTORS) = {
						/* usable P memory for trans */
						/* factors or filter coeffs */
#include FACTORS_FILE
0
};
#else	/* ][ */
__no_init fractional __ymem ModBufferYMEM[MAX_WORDS_DELAYS] @ 0x2400;
						/* usable Y memory */
__no_init fractional __ymem BufferYMEM[MAX_WORDS_SAMPLES] @ 0x1800;	
						/* usable Y memory */
__no_init fractional __xmem ModBufferXMEM[MAX_WORDS_FACTORS] @ 0x1000;
						/* usable X memory */
__no_init fractional __xmem BufferXMEM[MAX_WORDS_FACTORS] @ 0x0C00;
						/* usable X memory */
extern const fractional* ModBufferPMEM;		/* usable P memory for trans */
						/* factors or filter coeffs */
#endif	/* ]  */
#else	/* ][ */
fractional ModBufferYMEM[MAX_WORDS_DELAYS];	/* usable Y memory */
fractional BufferYMEM[MAX_WORDS_SAMPLES];	/* usable Y memory */
fractional ModBufferXMEM[MAX_WORDS_FACTORS];	/* usable X memory */
fractional BufferXMEM[MAX_WORDS_FACTORS];	/* usable X memory */
fractional ModBufferPMEM[MAX_WORDS_FACTORS];
#endif	/* ]  */

/* Global memory assignment and partition. */
fractional* BufferDelays = ModBufferYMEM;	/* Y memory for delays */
fractional* BufferData = BufferYMEM;		/* Y memory for data samples */
fractional* BufferAuxData = BufferXMEM;		/* X memory for aux. data */
fractcomplex* BufferFacts = (fractcomplex*) ModBufferXMEM;
						/* X memory for trns factors */
fractional* BufferCoeffs = ModBufferXMEM;	/* X memory for coefficients */

/* Local function prototypes. */

void TestOperations ( void );

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

fractional* LoadVector (
   fractional* aVector,
   int numElems,
   FILE* aFile
);

fractcomplex* LoadVectorComplex (
   fractcomplex* aVectorComplex,
   int numElems,
   FILE* aFile
);

void LoadValues (
   fractional* valsPtr,
   int numElems,
   FILE* aFile
);

void LoadValuesFloat (
   float* valsPtr,
   int numElems,
   FILE* aFile
);

#else	/* ][ */

void CopyValues (
   int numElems,
   fractional* dstPtr,
#ifndef	IAR_TOOLS			/* [ */
   const double* srcPtr
#else	/* ][ */
   const double __constptr * srcPtr
#endif	/* ] */
);

void CopyValuesFloat (
   int numElems,
   float* dstPtr,
#ifndef	IAR_TOOLS			/* [ */
   const double* srcPtr
#else	/* ][ */
   const double __constptr * srcPtr
#endif	/* ] */
);

#endif	/* ] */

/*...........................................................................*/

/* MAIN PROGRAM */
int main (
	int argc,
	char** argv
) {

   TestOperations ( );

   /* That is all. */
   return (0);					/* fixed address to set BP */

}; /* end of main */

/***************************************************************************/

/* Local function implementations. */

void TestOperations (
   void
) {

   /* Local declarations. */
   fractional* srcVectorOne = NULL;
   fractional* srcVectorTwo = NULL;
   fractional* theWindow = NULL;
   fractional* dstVector = NULL;
   fractional* srcMatrixOne = NULL;
   fractional* srcMatrixTwo = NULL;
   fractional* dstMatrix = NULL;
   fractional* srcSamples = 0;
   fractional* refSamples = 0;
   fractional* filtCoeffs = 0;
   fractional* kappaVals = 0;
   fractional* gammaVals = 0;
   fractional* dstSamples = 0;
   fractional* filtDelays = 0;
   fractcomplex* srcVectorComplexOne = 0;
   fractcomplex* dstVectorComplex = 0;
   fractcomplex* twidFactors = 0;
   fractcomplex* cosFactors = 0;
   float* srcMatrixFloat = NULL;		/* the black sheep... */
   float* pivotFlag = NULL;
   int* swappedRows = NULL;
   int* swappedCols = NULL;
   FIRStruct* FIRFilter = \
      (FIRStruct*) malloc (sizeof(FIRStruct));
   IIRCanonicStruct* iirCanonic = \
      (IIRCanonicStruct*) malloc (sizeof(IIRCanonicStruct));
   IIRTransposedStruct* iirTransposed = \
      (IIRTransposedStruct*) malloc (sizeof(IIRTransposedStruct));
   IIRLatticeStruct* iirLattice = \
      (IIRLatticeStruct*) malloc (sizeof(IIRLatticeStruct));
   float floatVal = 0;
   fractional fractVal = 0;
   fractional energyEstimate = 0;
   fractional initGain = 0;
   int intVal = 0;
   OPER_CODE operCode = NOT_A_CODE;
   OPER_MODE operMode = NOT_A_MODE;
   int coeffsPage = (int) COEFFS_IN_DATA;
   int numRows = 0;
   int numCols = 0;
   int numElems = 0;
   int numSamps = 0;
   int numTaps = 0;
   int numSects = 0;
   int finShift = 0;
   int filtOrder = 0;
   int sampRate = 0;
   int index = 0;
   int log2N = 0;
   int tmpElems = 0;
   int tmpRows = 0;
   int tmpCols = 0;
   int cntr = 0;
#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */
   double doubleVal = 0;
   FILE* fid = NULL;
#endif	/* ]  */
#if	VALIDATION==MPLAB_VAL		/* [ MPLAB C30 validation */
   int offset = 0;
#endif	/* ]  */

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

   /* Open data file. */
   if ((fid = fopen (IN_FILE_NAME, "r")) == NULL) {	/* for now text input */
      exit (EFOPEN);
   }

   /* Read operation code from data file. */
   fscanf (fid, "%d\n", (int*) &operCode); 

   /* Read operation mode from data file. */
   fscanf (fid, "%d\n", (int*) &operMode); 

#else	/* ][ */

   operCode = (int) indata[offset];
   offset++;
   operMode = (int) indata[offset];
   offset++;

#if	DATA_TYPE==FRACTIONAL		/* [ */

   /* Initialize timer 1 as a 1:1 stopwatch */
   PR1 = MAX_PERIOD;
   T1CON = ENABLE_TMR_ONE_TO_ONE;
    
#endif	/* ]  */

#endif	/* ]  */

   switch (operCode) {
/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case VMAX:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 fractVal = VectorMax (numElems, srcVectorOne, &index);

	 /* Report results. */
         printf ("%2.20f\n", Fract2Float (fractVal));
         printf ("%d\n", index);

#else	/* ][ */

#if	TEST_OPER==OPER_VMAX		/* [ VectorMax validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;
	 
         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 fractVal = VectorMax (numElems, srcVectorOne, &index);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 fractVal = VectorMax (numElems, srcVectorOne, &index);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 fractVal = VectorMax (numElems, srcVectorOne, &index);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
         printf ("%2.20f\n", Fract2Float (fractVal));
         printf ("%2.20f\n", Fract2Float (((double) index)/SCALE));

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = 2;			/* num results */
	 BufferAuxData[cntr++] = fractVal;		/* max value */
	 BufferAuxData[cntr++] = (fractional) index;	/* index of max val */

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case VMIN:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 fractVal = VectorMin (numElems, srcVectorOne, &index);

	 /* Report results. */
         printf ("%2.20f\n", Fract2Float (fractVal));
         printf ("%d\n", index);

#else	/* ][ */

#if	TEST_OPER==OPER_VMIN		/* [ VectorMin validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 fractVal = VectorMin (numElems, srcVectorOne, &index);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 fractVal = VectorMin (numElems, srcVectorOne, &index);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 fractVal = VectorMin (numElems, srcVectorOne, &index);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
         printf ("%2.20f\n", Fract2Float (fractVal));
         printf ("%2.20f\n", Fract2Float (((double) index)/SCALE));

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = 2;			/* num results */
	 BufferAuxData[cntr++] = fractVal;		/* min value */
	 BufferAuxData[cntr++] = (fractional) index;	/* index of min val */

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case VNEG:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for destination vector. */
	 dstVector = srcVectorOne + numElems;

         /* Apply operation. */
	 dstVector = VectorNegate (numElems, dstVector, srcVectorOne);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_VNEG		/* [ VectorNegate validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Assign memory for destination vector. */
	 dstVector = srcVectorOne + numElems;

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstVector = VectorNegate (numElems, dstVector, srcVectorOne);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstVector = VectorNegate (numElems, dstVector, srcVectorOne);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 dstVector = VectorNegate (numElems, dstVector, srcVectorOne);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case VSCL:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

	 /* Get scale value. */
	 fscanf (fid, "%lf\n", &doubleVal);
	 fractVal = Float2Fract ((float) doubleVal);

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for destination vector. */
	 dstVector = srcVectorOne + numElems;

         /* Apply operation. */
	 dstVector = VectorScale (numElems, dstVector, srcVectorOne, fractVal);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_VSCL		/* [ VectorScale validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Get scale value. */
	 fractVal = Float2Fract ((float) indata[offset]);
	 offset++;

	 /* Assign memory for destination vector. */
	 dstVector = srcVectorOne + numElems;

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstVector = VectorScale (numElems, dstVector, srcVectorOne, fractVal);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstVector = VectorScale (numElems, dstVector, srcVectorOne, fractVal);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w1" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstVector = VectorScale (numElems, dstVector, srcVectorOne, fractVal);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case VADD:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    fscanf (fid, "%d\n", &tmpElems);
	    if (numElems != tmpElems) {
	       exit (EVADD);
	    } else {
	       /* Load data samples (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       LoadValues (srcVectorTwo, numElems, fid);
	       /* Assign memory for destination vector. */
	       dstVector = srcVectorTwo + numElems;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
	    /* Assign memory for destination vector. */
	    dstVector = srcVectorOne + numElems;
	 }

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 dstVector = VectorAdd (numElems, dstVector, srcVectorOne, srcVectorTwo);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_VADD		/* [ VectorAdd validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    tmpElems = (int) indata[offset];
	    offset++;
	    if (numElems != tmpElems) {
	       exit (EVADD);
	    } else {
	       /* Copy data samples from static input array (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       CopyValues (numElems, srcVectorTwo, &indata[offset]);
	       offset += numElems;
	       /* Assign memory for destination vector. */
	       dstVector = srcVectorTwo + numElems;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
	    /* Assign memory for destination vector. */
	    dstVector = srcVectorOne + numElems;
	 }

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstVector = VectorAdd (numElems, dstVector, srcVectorOne, srcVectorTwo);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstVector = VectorAdd (numElems, dstVector, srcVectorOne, srcVectorTwo);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w1" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstVector = VectorAdd (numElems, dstVector, srcVectorOne, srcVectorTwo);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case VSUB:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    fscanf (fid, "%d\n", &tmpElems);
	    if (numElems != tmpElems) {
	       exit (EVSUB);
	    } else {
	       /* Load data samples (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       LoadValues (srcVectorTwo, numElems, fid);
	       /* Assign memory for destination vector. */
	       dstVector = srcVectorTwo + numElems;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
	    /* Assign memory for destination vector. */
	    dstVector = srcVectorOne + numElems;
	 }

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 dstVector = VectorSubtract (numElems, dstVector, srcVectorOne, srcVectorTwo);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_VSUB		/* [ VectorSubtract validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    tmpElems = (int) indata[offset];
	    offset++;
	    if (numElems != tmpElems) {
	       exit (EVSUB);
	    } else {
	       /* Copy data samples from static input array (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       CopyValues (numElems, srcVectorTwo, &indata[offset]);
	       offset += numElems;
	       /* Assign memory for destination vector. */
	       dstVector = srcVectorTwo + numElems;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
	    /* Assign memory for destination vector. */
	    dstVector = srcVectorOne + numElems;
	 }

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstVector = VectorSubtract (numElems, dstVector, srcVectorOne, srcVectorTwo);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstVector = VectorSubtract (numElems, dstVector, srcVectorOne, srcVectorTwo);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w1" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstVector = VectorSubtract (numElems, dstVector, srcVectorOne, srcVectorTwo);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case VMUL:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    fscanf (fid, "%d\n", &tmpElems);
	    if (numElems != tmpElems) {
	       exit (EVMUL);
	    } else {
	       /* Load data samples (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       LoadValues (srcVectorTwo, numElems, fid);
	       /* Assign memory for destination vector. */
	       dstVector = srcVectorTwo + numElems;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
	    /* Assign memory for destination vector. */
	    dstVector = srcVectorOne + numElems;
	 }

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 dstVector = VectorMultiply (numElems, dstVector, srcVectorOne, srcVectorTwo);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_VMUL		/* [ VectorMultiply validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    tmpElems = (int) indata[offset];
	    offset++;
	    if (numElems != tmpElems) {
	       exit (EVMUL);
	    } else {
	       /* Copy data samples from static input array (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       CopyValues (numElems, srcVectorTwo, &indata[offset]);
	       offset += numElems;
	       /* Assign memory for destination vector. */
	       dstVector = srcVectorTwo + numElems;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
	    /* Assign memory for destination vector. */
	    dstVector = srcVectorOne + numElems;
	 }

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstVector = VectorMultiply (numElems, dstVector, srcVectorOne, srcVectorTwo);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstVector = VectorMultiply (numElems, dstVector, srcVectorOne, srcVectorTwo);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w1" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstVector = VectorMultiply (numElems, dstVector, srcVectorOne, srcVectorTwo);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case VDOT:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    fscanf (fid, "%d\n", &tmpElems);
	    if (numElems != tmpElems) {
	       exit (EVDOT);
	    } else {
	       /* Load data samples (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       LoadValues (srcVectorTwo, numElems, fid);
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
	 }

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 fractVal = VectorDotProduct (numElems, srcVectorOne, srcVectorTwo);

	 /* Report results. */
         printf ("%2.20f\n", Fract2Float (fractVal));

#else	/* ][ */

#if	TEST_OPER==OPER_VDOT		/* [ VectorDotProduct validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    tmpElems = (int) indata[offset];
	    offset++;
	    if (numElems != tmpElems) {
	       exit (EVDOT);
	    } else {
	       /* Copy data samples from static input array (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       CopyValues (numElems, srcVectorTwo, &indata[offset]);
	       offset += numElems;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
	 }

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 fractVal = VectorDotProduct (numElems, srcVectorOne, srcVectorTwo);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 fractVal = VectorDotProduct (numElems, srcVectorOne, srcVectorTwo);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 fractVal = VectorDotProduct (numElems, srcVectorOne, srcVectorTwo);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
         printf ("%2.20f\n", Fract2Float (fractVal));

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = 1;			/* num results */
	 BufferAuxData[cntr] = fractVal;

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case VPOW:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 fractVal = VectorPower (numElems, srcVectorOne);

	 /* Report results. */
         printf ("%2.20f\n", Fract2Float (fractVal));

#else	/* ][ */

#if	TEST_OPER==OPER_VPOW		/* [ VectorPower validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 fractVal = VectorPower (numElems, srcVectorOne);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 fractVal = VectorPower (numElems, srcVectorOne);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 fractVal = VectorPower (numElems, srcVectorOne);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
         printf ("%2.20f\n", Fract2Float (fractVal));

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = 1;			/* num results */
	 BufferAuxData[cntr] = fractVal;

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case VCON:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    fscanf (fid, "%d\n", &tmpElems);
	    if (numElems < tmpElems) {
	       exit (EVCON);
	    } else {
	       /* Load data samples (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       LoadValues (srcVectorTwo, tmpElems, fid);
	       /* Assign memory for destination vector. */
	       dstVector = srcVectorTwo + tmpElems;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
            tmpElems = numElems;
	    /* Assign memory for destination vector. */
	    dstVector = srcVectorOne + numElems;
	 }

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 dstVector = VectorConvolve (numElems, tmpElems, dstVector, srcVectorOne, srcVectorTwo);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems+tmpElems-1; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_VCON		/* [ VectorConvolve validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    tmpElems = (int) indata[offset];
	    offset++;
	    if (numElems < tmpElems) {
	       exit (EVCON);
	    } else {
	       /* Copy data samples from static input array (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       CopyValues (tmpElems, srcVectorTwo, &indata[offset]);
	       offset += tmpElems;
	       /* Assign memory for destination vector. */
	       dstVector = srcVectorTwo + tmpElems;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
            tmpElems = numElems;
	    /* Assign memory for destination vector. */
	    dstVector = srcVectorOne + numElems;
	 }

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstVector = VectorConvolve (numElems, tmpElems, dstVector, srcVectorOne, srcVectorTwo);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstVector = VectorConvolve (numElems, tmpElems, dstVector, srcVectorOne, srcVectorTwo);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w2" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstVector = VectorConvolve (numElems, tmpElems, dstVector, srcVectorOne, srcVectorTwo);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems+tmpElems-1; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems+tmpElems-1;	/* num results */
	 for ( ; cntr <= numElems+tmpElems-1; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case VCOR:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    fscanf (fid, "%d\n", &tmpElems);
	    if (numElems < tmpElems) {
	       exit (EVCOR);
	    } else {
	       /* Load data samples (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       LoadValues (srcVectorTwo, tmpElems, fid);
	       /* Assign memory for destination vector. */
	       dstVector = srcVectorTwo + tmpElems;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
            tmpElems = numElems;
	    /* Assign memory for destination vector. */
	    dstVector = srcVectorOne + numElems;
	 }

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 dstVector = VectorCorrelate (numElems, tmpElems, dstVector, srcVectorOne, srcVectorTwo);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems+tmpElems-1; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_VCOR		/* [ VectorCorrelate validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of input samples (second operand). */
	    tmpElems = (int) indata[offset];
	    offset++;
	    if (numElems < tmpElems) {
	       exit (EVCOR);
	    } else {
	       /* Copy data samples from static input array (second operand). */
	       srcVectorTwo = BufferData + numElems;
	       CopyValues (tmpElems, srcVectorTwo, &indata[offset]);
	       offset += tmpElems;
	       /* Assign memory for destination vector. */
	       dstVector = srcVectorTwo + tmpElems;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcVectorTwo = srcVectorOne;
            tmpElems = numElems;
	    /* Assign memory for destination vector. */
	    dstVector = srcVectorOne + numElems;
	 }

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstVector = VectorCorrelate (numElems, tmpElems, dstVector, srcVectorOne, srcVectorTwo);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstVector = VectorCorrelate (numElems, tmpElems, dstVector, srcVectorOne, srcVectorTwo);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w2" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstVector = VectorCorrelate (numElems, tmpElems, dstVector, srcVectorOne, srcVectorTwo);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems+tmpElems-1; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems+tmpElems-1;	/* num results */
	 for ( ; cntr <= numElems+tmpElems-1; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case MSCL:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out dimensions of input matrix. */
	 fscanf (fid, "%d\n", &numRows);
	 fscanf (fid, "%d\n", &numCols);
   
         /* Load data samples. */
	 srcMatrixOne = BufferData;
         LoadValues (srcMatrixOne, numRows*numCols, fid); 

	 /* Get scale value. */
	 fscanf (fid, "%lf\n", &doubleVal);
	 fractVal = Float2Fract ((float) doubleVal);

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for destination vector. */
	 dstMatrix = srcMatrixOne + numRows*numCols;

         /* Apply operation. */
	 dstMatrix = MatrixScale (numRows, numCols, dstMatrix, srcMatrixOne, fractVal);

	 /* Report results. */
	 numElems = numRows*numCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstMatrix[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_MSCL		/* [ MatrixScale validation */

         /* Find out dimensions of input matrix. */
	 numRows = (int) indata[offset];
	 offset++;
	 numCols = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcMatrixOne = BufferData;
         CopyValues (numRows*numCols, srcMatrixOne, &indata[offset]); 
	 offset += numRows*numCols;

	 /* Get scale value. */
	 fractVal = Float2Fract ((float) indata[offset]);
	 offset++;

	 /* Assign memory for destination vector. */
	 dstMatrix = srcMatrixOne + numRows*numCols;

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstMatrix = MatrixScale (numRows, numCols, dstMatrix, srcMatrixOne, fractVal);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstMatrix = MatrixScale (numRows, numCols, dstMatrix, srcMatrixOne, fractVal);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time++;					/* compiler clears */
	 						/* TMR1 after w2 was */
							/* loaded... */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w11" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstMatrix = MatrixScale (numRows, numCols, dstMatrix, srcMatrixOne, fractVal);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 numElems = numRows*numCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstMatrix[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 numElems = numRows*numCols;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstMatrix++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case MTRP:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out dimensions of input matrix. */
	 fscanf (fid, "%d\n", &numRows);
	 fscanf (fid, "%d\n", &numCols);
   
         /* Load data samples. */
	 srcMatrixOne = BufferData;
         LoadValues (srcMatrixOne, numRows*numCols, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for destination vector. */
	 dstMatrix = srcMatrixOne + numRows*numCols;

         /* Apply operation. */
	 dstMatrix = MatrixTranspose (numRows, numCols, dstMatrix, srcMatrixOne);

	 /* Report results. */
	 numElems = numRows*numCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstMatrix[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_MTRP		/* [ MatrixTranspose validation */

         /* Find out dimensions of input matrix. */
	 numRows = (int) indata[offset];
	 offset++;
	 numCols = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcMatrixOne = BufferData;
         CopyValues (numRows*numCols, srcMatrixOne, &indata[offset]); 
	 offset += numRows*numCols;

	 /* Assign memory for destination vector. */
	 dstMatrix = srcMatrixOne + numRows*numCols;

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstMatrix = MatrixTranspose (numRows, numCols, dstMatrix, srcMatrixOne);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstMatrix = MatrixTranspose (numRows, numCols, dstMatrix, srcMatrixOne);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time++;					/* compiler clears */
	 						/* TMR1 after w2 was */
							/* loaded... */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w2" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstMatrix = MatrixTranspose (numRows, numCols, dstMatrix, srcMatrixOne);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 numElems = numRows*numCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstMatrix[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 numElems = numRows*numCols;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstMatrix++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case MINV:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out dimensions of input matrix. */
	 fscanf (fid, "%d\n", &numRows);
	 fscanf (fid, "%d\n", &numCols);
   
	 /* Check that matrix is square. */
	 if (numRows != numCols) {
	    exit (EMINV);
	 }

         /* Load data samples. */
	 srcMatrixFloat = (float*) BufferData;
         LoadValuesFloat (srcMatrixFloat, numRows*numCols, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign vectors for internal use. */
	 pivotFlag = (float*) BufferAuxData;
	 swappedRows = (int*) BufferCoeffs;
	 swappedCols = (int*) BufferDelays;

	 /* Perform operation in place, */
	 /* so that no float destination matrix needs to be generated. */

         /* Apply operation. */
	 srcMatrixFloat = MatrixInvert (numRows, srcMatrixFloat, srcMatrixFloat, pivotFlag, swappedRows, swappedCols);

	 /* Report results. */
	 numElems = numRows*numCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", srcMatrixFloat[cntr]);
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_MINV		/* [ MatrixInvert validation */

         /* Find out dimensions of input matrix. */
	 numRows = (int) indata[offset];
	 offset++;
	 numCols = (int) indata[offset];
	 offset++;
   
	 /* Check that matrix is square. */
	 if (numRows != numCols) {
	    exit (EMINV);
	 }

	 /* Copy data samples from static input array. */
	 srcMatrixFloat = (float*) BufferData;
         CopyValuesFloat (numRows*numCols, srcMatrixFloat, &indata[offset]); 
	 offset += numRows*numCols;

	 /* Assign vectors for internal use. */
	 pivotFlag = (float*) BufferAuxData;
	 swappedRows = (int*) BufferCoeffs;
	 swappedCols = (int*) BufferDelays;

	 /* Perform operation in place, */
	 /* so that no float destination matrix needs to be generated. */

         /* Apply operation. */
	 srcMatrixFloat = MatrixInvert (numRows, srcMatrixFloat, srcMatrixFloat, pivotFlag, swappedRows, swappedCols);

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 numElems = numRows*numCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", srcMatrixFloat[cntr]);
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 numElems = numRows*numCols;
	 *(BufferAuxData++) = numElems;			/* num results */
	 memcpy ((void*) BufferAuxData, (void*) srcMatrixFloat, numElems*(sizeof(float)));
	 /* NOTE: the values placed in BufferAuxData must be interpreted */
	 /* as floating point numbers in little endian arrangement. */

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case MADD:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out dimensions of input matrix. */
	 fscanf (fid, "%d\n", &numRows);
	 fscanf (fid, "%d\n", &numCols);
   
         /* Load data samples. */
	 srcMatrixOne = BufferData;
         LoadValues (srcMatrixOne, numRows*numCols, fid); 

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of rows and columns (second operand). */
	    fscanf (fid, "%d\n", &tmpRows);
	    fscanf (fid, "%d\n", &tmpCols);
	    if ((numRows != tmpRows) | (numCols != tmpCols)) {
	       exit (EMADD);
	    } else {
	       /* Load data samples (second operand). */
	       srcMatrixTwo = BufferData + numRows*numCols;
	       LoadValues (srcMatrixTwo, numRows*numCols, fid);
	       /* Assign memory for destination vector. */
	       dstMatrix = srcMatrixTwo + numRows*numCols;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcMatrixTwo = srcMatrixOne;
	    /* Assign memory for destination vector. */
	    dstMatrix = srcMatrixOne + numRows*numCols;
	 }

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 dstMatrix = MatrixAdd (numRows, numCols, dstMatrix, srcMatrixOne, srcMatrixTwo);

	 /* Report results. */
	 numElems = numRows*numCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstMatrix[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_MADD		/* [ MatrixAdd validation */

         /* Find out dimensions of input matrix. */
	 numRows = (int) indata[offset];
	 offset++;
	 numCols = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcMatrixOne = BufferData;
         CopyValues (numRows*numCols, srcMatrixOne, &indata[offset]); 
	 offset += numRows*numCols;

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of rows and columns (second operand). */
	    tmpRows = (int) indata[offset];
	    offset++;
	    tmpCols = (int) indata[offset];
	    offset++;
	    if ((numRows != tmpRows) | (numCols != tmpCols)) {
	       exit (EMADD);
	    } else {
	       /* Load data samples (second operand). */
	       srcMatrixTwo = BufferData + numRows*numCols;
	       CopyValues (numRows*numCols, srcMatrixTwo, &indata[offset]);
	       offset += numRows*numCols;
	       /* Assign memory for destination vector. */
	       dstMatrix = srcMatrixTwo + numRows*numCols;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcMatrixTwo = srcMatrixOne;
	    /* Assign memory for destination vector. */
	    dstMatrix = srcMatrixOne + numRows*numCols;
	 }

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstMatrix = MatrixAdd (numRows, numCols, dstMatrix, srcMatrixOne, srcMatrixTwo);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstMatrix = MatrixAdd (numRows, numCols, dstMatrix, srcMatrixOne, srcMatrixTwo);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time++;					/* compiler clears */
	 						/* TMR1 after w2 was */
							/* loaded... */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w2" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstMatrix = MatrixAdd (numRows, numCols, dstMatrix, srcMatrixOne, srcMatrixTwo);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 numElems = numRows*numCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstMatrix[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 numElems = numRows*numCols;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstMatrix++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case MSUB:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out dimensions of input matrix. */
	 fscanf (fid, "%d\n", &numRows);
	 fscanf (fid, "%d\n", &numCols);
   
         /* Load data samples. */
	 srcMatrixOne = BufferData;
         LoadValues (srcMatrixOne, numRows*numCols, fid); 

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of rows and columns (second operand). */
	    fscanf (fid, "%d\n", &tmpRows);
	    fscanf (fid, "%d\n", &tmpCols);
	    if ((numRows != tmpRows) | (numCols != tmpCols)) {
	       exit (EMSUB);
	    } else {
	       /* Load data samples (second operand). */
	       srcMatrixTwo = BufferData + numRows*numCols;
	       LoadValues (srcMatrixTwo, numRows*numCols, fid);
	       /* Assign memory for destination vector. */
	       dstMatrix = srcMatrixTwo + numRows*numCols;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcMatrixTwo = srcMatrixOne;
	    /* Assign memory for destination vector. */
	    dstMatrix = srcMatrixOne + numRows*numCols;
	 }

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 dstMatrix = MatrixSubtract (numRows, numCols, dstMatrix, srcMatrixOne, srcMatrixTwo);

	 /* Report results. */
	 numElems = numRows*numCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstMatrix[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_MSUB		/* [ MatrixSubtract validation */

         /* Find out dimensions of input matrix. */
	 numRows = (int) indata[offset];
	 offset++;
	 numCols = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcMatrixOne = BufferData;
         CopyValues (numRows*numCols, srcMatrixOne, &indata[offset]); 
	 offset += numRows*numCols;

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of rows and columns (second operand). */
	    tmpRows = (int) indata[offset];
	    offset++;
	    tmpCols = (int) indata[offset];
	    offset++;
	    if ((numRows != tmpRows) | (numCols != tmpCols)) {
	       exit (EMSUB);
	    } else {
	       /* Load data samples (second operand). */
	       srcMatrixTwo = BufferData + numRows*numCols;
	       CopyValues (numRows*numCols, srcMatrixTwo, &indata[offset]);
	       offset += numRows*numCols;
	       /* Assign memory for destination vector. */
	       dstMatrix = srcMatrixTwo + numRows*numCols;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcMatrixTwo = srcMatrixOne;
	    /* Assign memory for destination vector. */
	    dstMatrix = srcMatrixOne + numRows*numCols;
	 }

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstMatrix = MatrixSubtract (numRows, numCols, dstMatrix, srcMatrixOne, srcMatrixTwo);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstMatrix = MatrixSubtract (numRows, numCols, dstMatrix, srcMatrixOne, srcMatrixTwo);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time++;					/* compiler clears */
	 						/* TMR1 after w2 was */
							/* loaded... */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w2" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstMatrix = MatrixSubtract (numRows, numCols, dstMatrix, srcMatrixOne, srcMatrixTwo);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 numElems = numRows*numCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstMatrix[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 numElems = numRows*numCols;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstMatrix++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case MMUL:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out dimensions of input matrix. */
	 fscanf (fid, "%d\n", &numRows);
	 fscanf (fid, "%d\n", &numCols);
   
         /* Load data samples. */
	 srcMatrixOne = BufferData;
         LoadValues (srcMatrixOne, numRows*numCols, fid); 

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of rows and columns (second operand). */
	    fscanf (fid, "%d\n", &tmpRows);
	    fscanf (fid, "%d\n", &tmpCols);
	    if (numCols != tmpRows) {
	       exit (EMMUL);
	    } else {
	       /* Load data samples (second operand). */
	       srcMatrixTwo = BufferData + numRows*numCols;
	       LoadValues (srcMatrixTwo, tmpRows*tmpCols, fid);
	       /* Assign memory for destination vector. */
	       dstMatrix = srcMatrixTwo + tmpRows*tmpCols;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcMatrixTwo = srcMatrixOne;
            tmpCols = numCols;
	    /* Assign memory for destination vector. */
	    dstMatrix = srcMatrixOne + numRows*numCols;
	 }

         /* Close file (before I forget). */
         fclose (fid);

         /* Apply operation. */
	 dstMatrix = MatrixMultiply (numRows, numCols, tmpCols, dstMatrix, srcMatrixOne, srcMatrixTwo);

	 /* Report results. */
	 numElems = numRows*tmpCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstMatrix[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_MMUL		/* [ MatrixMultiply validation */

         /* Find out dimensions of input matrix. */
	 numRows = (int) indata[offset];
	 offset++;
	 numCols = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcMatrixOne = BufferData;
         CopyValues (numRows*numCols, srcMatrixOne, &indata[offset]); 
	 offset += numRows*numCols;

	 /* Regarding second operand... */
	 if (operMode == BINARY) {
	    /* Find out number of rows and columns (second operand). */
	    tmpRows = (int) indata[offset];
	    offset++;
	    tmpCols = (int) indata[offset];
	    offset++;
	    if (numCols != tmpRows) {
	       exit (EMMUL);
	    } else {
	       /* Load data samples (second operand). */
	       srcMatrixTwo = BufferData + numRows*numCols;
	       CopyValues (tmpRows*tmpCols, srcMatrixTwo, &indata[offset]);
	       offset += tmpRows*tmpCols;
	       /* Assign memory for destination vector. */
	       dstMatrix = srcMatrixTwo + tmpRows*tmpCols;
	    }
	 } else {
	    /* Assign memory for second (unary) operand. */
            srcMatrixTwo = srcMatrixOne;
            tmpCols = numCols;
	    /* Assign memory for destination vector. */
	    dstMatrix = srcMatrixOne + numRows*numCols;
	 }

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstMatrix = MatrixMultiply (numRows, numCols, tmpCols, dstMatrix, srcMatrixOne, srcMatrixTwo);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstMatrix = MatrixMultiply (numRows, numCols, tmpCols, dstMatrix, srcMatrixOne, srcMatrixTwo);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time++;					/* compiler clears */
	 						/* TMR1 after w2 was */
							/* loaded... */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w3" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstMatrix = MatrixMultiply (numRows, numCols, tmpCols, dstMatrix, srcMatrixOne, srcMatrixTwo);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 numElems = numRows*tmpCols;
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstMatrix[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 numElems = numRows*tmpCols;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstMatrix++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case WBAR:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for destination vector and window. */
	 dstVector = srcVectorOne + numElems;
	 theWindow = dstVector + numElems;

         /* Apply operation. */
	 /* With implicit window initialization. */
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, BartlettInit (numElems, theWindow));

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_WBAR		/* [ Barlett windowing validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Assign memory for destination vector and window. */
	 dstVector = srcVectorOne + numElems;
	 theWindow = dstVector + numElems;

         /* Apply operation. */
	 theWindow = BartlettInit (numElems, theWindow);
#if	DATA_TYPE==FLOATING		/* [ */
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, theWindow);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, theWindow);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w11" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, theWindow);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case WBLK:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for destination vector and window. */
	 dstVector = srcVectorOne + numElems;
	 theWindow = dstVector + numElems;

         /* Apply operation. */
	 /* With implicit window initialization. */
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, BlackmanInit (numElems, theWindow));

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_WBLK		/* [ Blackman windowing validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Assign memory for destination vector and window. */
	 dstVector = srcVectorOne + numElems;
	 theWindow = dstVector + numElems;

         /* Apply operation. */
	 theWindow = BlackmanInit (numElems, theWindow);
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, theWindow);

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case WHAM:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for destination vector and window. */
	 dstVector = srcVectorOne + numElems;
	 theWindow = dstVector + numElems;

         /* Apply operation. */
	 /* With implicit window initialization. */
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, HammingInit (numElems, theWindow));

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_WHAM		/* [ Hamming windowing validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Assign memory for destination vector and window. */
	 dstVector = srcVectorOne + numElems;
	 theWindow = dstVector + numElems;

         /* Apply operation. */
	 theWindow = HammingInit (numElems, theWindow);
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, theWindow);

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case WHAN:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for destination vector and window. */
	 dstVector = srcVectorOne + numElems;
	 theWindow = dstVector + numElems;

         /* Apply operation. */
	 /* With implicit window initialization. */
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, HanningInit (numElems, theWindow));

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_WHAN		/* [ Hanning windowing validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Assign memory for destination vector and window. */
	 dstVector = srcVectorOne + numElems;
	 theWindow = dstVector + numElems;

         /* Apply operation. */
	 theWindow = HanningInit (numElems, theWindow);
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, theWindow);

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case WKSR:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 srcVectorOne = BufferData;
         LoadValues (srcVectorOne, numElems, fid); 

	 /* Get shape value. */
	 fscanf (fid, "%lf\n", &doubleVal);
	 floatVal = (float) doubleVal;

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for destination vector and window. */
	 dstVector = srcVectorOne + numElems;
	 theWindow = dstVector + numElems;

         /* Apply operation. */
	 /* With implicit window initialization. */
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, KaiserInit (numElems, theWindow, floatVal));

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_WKSR		/* [ Kaiser windowing validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Get scale value. */
	 floatVal = (float) indata[offset];
	 offset++;

	 /* Assign memory for destination vector and window. */
	 dstVector = srcVectorOne + numElems;
	 theWindow = dstVector + numElems;

         /* Apply operation. */
	 theWindow = KaiserInit (numElems, theWindow,floatVal);
	 dstVector = VectorWindow (numElems, dstVector, srcVectorOne, theWindow);

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case FIRF:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numSamps);
   
         /* Load data samples. */
	 srcSamples = BufferData;
         LoadValues (srcSamples, numSamps, fid); 

         /* Find out number of filter coefficients. */
	 fscanf (fid, "%d\n", &numTaps);
   
         /* Load filter coefficients. */
	 filtCoeffs = BufferCoeffs;
         LoadValues (filtCoeffs, numTaps, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

         /* Apply operation. */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
	 FIRDelayInit (FIRFilter);
	 dstSamples = FIR (numSamps, dstSamples, srcSamples, FIRFilter);

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
#if	DATA_TYPE==FLOATING		/* [ */
	 /* Delay values are ordered increasingly and linearly */
	 /* from base address. */
	 for (cntr = 0; cntr < numTaps; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }
#else	/* ][ */
	 /* Delay values are ordered decreasingly and modularly */
	 /* from (FIRFilter->delay)-1 address. */
	 /* Find out location of fist delay. */
	 dstSamples = FIRFilter->delay;			/* reuse dstSamples */
	 tmpElems = dstSamples - filtDelays;		/* offset next delay */
	 for (cntr = tmpElems; cntr >= 0; cntr--) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }
	 for (cntr = numTaps-1; cntr > tmpElems; cntr--) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }
#endif	/* ] */

#else	/* ][ */

#if	TEST_OPER==OPER_FIRF		/* [ FIR filter validation */

         /* Find out number of input samples. */
	 numSamps = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcSamples = BufferData;
         CopyValues (numSamps, srcSamples, &indata[offset]); 
	 offset += numSamps;

         /* Find out number of filter coefficients. */
	 numTaps = (int) indata[offset];
	 offset++;
   
	 /* Copy filter coefficients from static input array. */
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 filtCoeffs = BufferCoeffs;
         CopyValues (numTaps, filtCoeffs, &indata[offset]); 
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, filtCoeffs);
#else	/* ][ */
	 coeffsPage = 0;				/* forced from linker */
	 						/* control file */
	 filtCoeffs = (fractional*) ModBufferPMEM;	/* makes dsPIC reset */
	 filtCoeffs = (fractional*) 0x0B000;		/* forced from linker */
	 						/* control file */
#endif	/* ]  */
#endif	/* ]  */
	 offset += numTaps;

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time++;					/* compiler clears */
	 						/* TMR1 after w2 was */
							/* loaded... */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
#endif	/* ]  */
#endif	/* ]  */
#if	DATA_TYPE==FLOATING		/* [ */
	 FIRDelayInit (FIRFilter);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 FIRDelayInit (FIRFilter);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 FIRDelayInit (FIRFilter);
#endif	/* ]  */
#endif	/* ]  */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstSamples = FIR (numSamps, dstSamples, srcSamples, FIRFilter);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstSamples = FIR (numSamps, dstSamples, srcSamples, FIRFilter);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w12" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstSamples = FIR (numSamps, dstSamples, srcSamples, FIRFilter);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }

	 /* Delay values are ordered increasingly and linearly */
	 /* from base address. */
	 for (cntr = 0; cntr < numTaps; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numSamps+numTaps;	/* num results */
	 for ( ; cntr <= numSamps; cntr++) {
	    BufferAuxData[cntr] = *(dstSamples++);
	 }

	 /* Delay values are ordered decreasingly and modularly */
	 /* from (FIRFilter->delay)-1 address. */
	 /* Find out location of fist delay. */
	 dstSamples = FIRFilter->delay;			/* reuse dstSamples */
	 tmpElems = dstSamples - filtDelays;		/* offset next delay */
	 for (offset = tmpElems; offset >= 0; offset--) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }
	 for (offset = numTaps-1; offset > tmpElems; offset--) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case FDEC:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numSamps);
   
         /* Load data samples. */
	 srcSamples = BufferData;
         LoadValues (srcSamples, numSamps, fid); 

         /* Find out number of filter coefficients. */
	 fscanf (fid, "%d\n", &numTaps);
   
         /* Load filter coefficients. */
	 filtCoeffs = BufferCoeffs;
         LoadValues (filtCoeffs, numTaps, fid); 

	 /* Get downsampling rate value. */
	 fscanf (fid, "%lf\n", &doubleVal);
	 sampRate = (int) doubleVal;

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

	 /* Readjust the (output) number of samples. */
	 numSamps /= sampRate;

         /* Apply operation. */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
	 FIRDelayInit (FIRFilter);
	 dstSamples = FIRDecimate (numSamps, dstSamples, srcSamples, FIRFilter, sampRate);

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr < numTaps; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_FDEC		/* [ FIR decimator validation */

         /* Find out number of input samples. */
	 numSamps = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcSamples = BufferData;
         CopyValues (numSamps, srcSamples, &indata[offset]); 
	 offset += numSamps;

         /* Find out number of filter coefficients. */
	 numTaps = (int) indata[offset];
	 offset++;
   
	 /* Copy filter coefficients from static input array. */
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 filtCoeffs = BufferCoeffs;
         CopyValues (numTaps, filtCoeffs, &indata[offset]); 
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, filtCoeffs);
#else	/* ][ */
	 coeffsPage = 0;				/* forced from linker */
	 						/* control file */
	 filtCoeffs = (fractional*) ModBufferPMEM;	/* makes dsPIC reset */
	 filtCoeffs = (fractional*) 0x0B000;		/* forced from linker */
	 						/* control file */
#endif	/* ]  */
#endif	/* ]  */
	 offset += numTaps;

	 /* Get downsampling rate value. */
	 sampRate = (int) indata[offset];
	 offset++;

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

	 /* Readjust the (output) number of samples. */
	 numSamps /= sampRate;

         /* Apply operation. */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
	 FIRDelayInit (FIRFilter);
#if	DATA_TYPE==FLOATING		/* [ */
	 dstSamples = FIRDecimate (numSamps, dstSamples, srcSamples, FIRFilter, sampRate);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstSamples = FIRDecimate (numSamps, dstSamples, srcSamples, FIRFilter, sampRate);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w14" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstSamples = FIRDecimate (numSamps, dstSamples, srcSamples, FIRFilter, sampRate);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr < numTaps; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numSamps+numTaps;	/* num results */
	 for ( ; cntr <= numSamps; cntr++) {
	    BufferAuxData[cntr] = *(dstSamples++);
	 }

	 /* Delay values are ordered increasingly from base address. */
	 for (offset = 0; offset < numTaps; offset++) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case FINT:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numSamps);
   
         /* Load data samples. */
	 srcSamples = BufferData;
         LoadValues (srcSamples, numSamps, fid); 

         /* Find out number of filter coefficients. */
	 fscanf (fid, "%d\n", &numTaps);
   
         /* Load filter coefficients. */
	 filtCoeffs = BufferCoeffs;
         LoadValues (filtCoeffs, numTaps, fid); 

	 /* Get upsampling rate value. */
	 fscanf (fid, "%lf\n", &doubleVal);
	 sampRate = (int) doubleVal;

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

         /* Apply operation. */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
	 FIRInterpDelayInit (FIRFilter, sampRate);
	 dstSamples = FIRInterpolate (numSamps, dstSamples, srcSamples, FIRFilter, sampRate);

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps*sampRate; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr < numTaps/sampRate; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_FINT		/* [ FIR interpolator validation */

         /* Find out number of input samples. */
	 numSamps = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcSamples = BufferData;
         CopyValues (numSamps, srcSamples, &indata[offset]); 
	 offset += numSamps;

         /* Find out number of filter coefficients. */
	 numTaps = (int) indata[offset];
	 offset++;
   
	 /* Copy filter coefficients from static input array. */
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 filtCoeffs = BufferCoeffs;
         CopyValues (numTaps, filtCoeffs, &indata[offset]); 
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, filtCoeffs);
#else	/* ][ */
	 coeffsPage = 0;				/* forced from linker */
	 						/* control file */
	 filtCoeffs = (fractional*) ModBufferPMEM;	/* makes dsPIC reset */
	 filtCoeffs = (fractional*) 0x0B000;		/* forced from linker */
	 						/* control file */
#endif	/* ]  */
#endif	/* ]  */
	 offset += numTaps;

	 /* Get upsampling rate value. */
	 sampRate = (int) indata[offset];
	 offset++;

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

         /* Apply operation. */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
#if	DATA_TYPE==FLOATING		/* [ */
	 FIRInterpDelayInit (FIRFilter, sampRate);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 FIRInterpDelayInit (FIRFilter, sampRate);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 FIRInterpDelayInit (FIRFilter, sampRate);
#endif	/* ]  */
#endif	/* ]  */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstSamples = FIRInterpolate (numSamps, dstSamples, srcSamples, FIRFilter, sampRate);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstSamples = FIRInterpolate (numSamps, dstSamples, srcSamples, FIRFilter, sampRate);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w14" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstSamples = FIRInterpolate (numSamps, dstSamples, srcSamples, FIRFilter, sampRate);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps*sampRate; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr < numTaps/sampRate; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numSamps*sampRate+numTaps/sampRate;
	 						/* num results */
	 for ( ; cntr <= numSamps*sampRate; cntr++) {
	    BufferAuxData[cntr] = *(dstSamples++);
	 }

	 /* Delay values are ordered increasingly from base address. */
	 for (offset = 0; offset < numTaps/sampRate; offset++) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case FLMS:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numSamps);
   
         /* Load data samples. */
	 srcSamples = BufferData;
         LoadValues (srcSamples, numSamps, fid); 

         /* Find out number of filter coefficients. */
	 fscanf (fid, "%d\n", &numTaps);
   
         /* Load filter coefficients. */
	 filtCoeffs = BufferCoeffs;
         LoadValues (filtCoeffs, numTaps, fid); 

         /* Find out number of reference samples. */
	 fscanf (fid, "%d\n", &tmpElems);
	 if (numSamps != tmpElems) {
	    exit (EFLMS);
	 } else {
	    /* Load reference samples. */
	    refSamples = srcSamples + numSamps;
	    LoadValues (refSamples, numSamps, fid); 
	 }

	 /* Get mu value. */
	 fscanf (fid, "%lf\n", &doubleVal);
	 fractVal = Float2Fract ((float) doubleVal);

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = refSamples + numSamps;

         /* Apply operation. */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
	 FIRDelayInit (FIRFilter);
	 dstSamples = FIRLMS (numSamps, dstSamples, srcSamples, FIRFilter, refSamples, fractVal);

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
#if	DATA_TYPE==FLOATING		/* [ */
	 /* Delay values are ordered increasingly and linearly */
	 /* from base address. */
	 for (cntr = 0; cntr < numTaps; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }
#else	/* ][ */
	 /* Delay values are ordered decreasingly and modularly */
	 /* from (FIRFilter->delay)-1 address. */
	 /* Find out location of fist delay. */
	 dstSamples = FIRFilter->delay;			/* reuse dstSamples */
	 tmpElems = dstSamples - filtDelays;		/* offset next delay */
	 for (cntr = tmpElems; cntr >= 0; cntr--) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }
	 for (cntr = numTaps-1; cntr > tmpElems; cntr--) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }
#endif	/* ] */

#else	/* ][ */

#if	TEST_OPER==OPER_FLMS		/* [ FIR LMS validation */

         /* Find out number of input samples. */
	 numSamps = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcSamples = BufferData;
         CopyValues (numSamps, srcSamples, &indata[offset]); 
	 offset += numSamps;

         /* Find out number of filter coefficients. */
	 numTaps = (int) indata[offset];
	 offset++;
   
	 /* Copy filter coefficients from static input array. */
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 filtCoeffs = BufferCoeffs;
         CopyValues (numTaps, filtCoeffs, &indata[offset]); 
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, filtCoeffs);
	 /* NOTE: this is an error condition, since filter coefficients */
	 /* could not be adapted at run time if in program memory. In this */
	 /* case, the function is to return a NULL pointer. */
#endif	/* ]  */
	 offset += numTaps;

         /* Find out number of reference samples. */
	 tmpElems = (int) indata[offset];
	 offset++;
	 if (numSamps != tmpElems) {
	    exit (EFLMS);
	 } else {
	    /* Copy reference samples from static input array. */
	    refSamples = srcSamples + numSamps;
	    CopyValues (numSamps, refSamples, &indata[offset]);
	    offset += numSamps;
	 }

	 /* Get mu value. */
	 fractVal = Float2Fract ((float) indata[offset]);
	 offset++;

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = refSamples + numSamps;

         /* Apply operation. */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
	 FIRDelayInit (FIRFilter);
#if	DATA_TYPE==FLOATING		/* [ */
	 dstSamples = FIRLMS (numSamps, dstSamples, srcSamples, FIRFilter, refSamples, fractVal);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstSamples = FIRLMS (numSamps, dstSamples, srcSamples, FIRFilter, refSamples, fractVal);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w9" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstSamples = FIRLMS (numSamps, dstSamples, srcSamples, FIRFilter, refSamples, fractVal);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }

	 /* Delay values are ordered increasingly and linearly */
	 /* from base address. */
	 for (cntr = 0; cntr < numTaps; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numSamps+numTaps;	/* num results */
	 for ( ; cntr <= numSamps; cntr++) {
	    BufferAuxData[cntr] = *(dstSamples++);
	 }

	 /* Delay values are ordered decreasingly and modularly */
	 /* from (FIRFilter->delay)-1 address. */
	 /* Find out location of fist delay. */
	 dstSamples = FIRFilter->delay;			/* reuse dstSamples */
	 tmpElems = dstSamples - filtDelays;		/* offset next delay */
	 for (offset = tmpElems; offset >= 0; offset--) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }
	 for (offset = numTaps-1; offset > tmpElems; offset--) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case FLMSN:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numSamps);
   
         /* Load data samples. */
	 srcSamples = BufferData;
         LoadValues (srcSamples, numSamps, fid); 

         /* Find out number of filter coefficients. */
	 fscanf (fid, "%d\n", &numTaps);
   
         /* Load filter coefficients. */
	 filtCoeffs = BufferCoeffs;
         LoadValues (filtCoeffs, numTaps, fid); 

         /* Find out number of reference samples. */
	 fscanf (fid, "%d\n", &tmpElems);
	 if (numSamps != tmpElems) {
	    exit (EFLMSN);
	 } else {
	    /* Load reference samples. */
	    refSamples = srcSamples + numSamps;
	    LoadValues (refSamples, numSamps, fid); 
	 }

	 /* Get mu value. */
	 fscanf (fid, "%lf\n", &doubleVal);
	 fractVal = Float2Fract ((float) doubleVal);

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = refSamples + numSamps;

         /* Apply operation. */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
	 FIRDelayInit (FIRFilter);
	 dstSamples = FIRLMSNorm (numSamps, dstSamples, srcSamples, FIRFilter, refSamples, fractVal, &energyEstimate);

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
#if	DATA_TYPE==FLOATING		/* [ */
	 /* Delay values are ordered increasingly and linearly */
	 /* from base address. */
	 for (cntr = 0; cntr < numTaps; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }
#else	/* ][ */
	 /* Delay values are ordered decreasingly and modularly */
	 /* from (FIRFilter->delay)-1 address. */
	 /* Find out location of fist delay. */
	 dstSamples = FIRFilter->delay;			/* reuse dstSamples */
	 tmpElems = dstSamples - filtDelays;		/* offset next delay */
	 for (cntr = tmpElems; cntr >= 0; cntr--) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }
	 for (cntr = numTaps-1; cntr > tmpElems; cntr--) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }
#endif	/* ] */
	 /* Report energy estimate for last iteration. */
         printf ("%2.20f\n", Fract2Float (energyEstimate));

#else	/* ][ */

#if	TEST_OPER==OPER_FLMSN		/* [ FIR LMS normalized validation */

         /* Find out number of input samples. */
	 numSamps = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcSamples = BufferData;
         CopyValues (numSamps, srcSamples, &indata[offset]); 
	 offset += numSamps;

         /* Find out number of filter coefficients. */
	 numTaps = (int) indata[offset];
	 offset++;
   
	 /* Copy filter coefficients from static input array. */
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 filtCoeffs = BufferCoeffs;
         CopyValues (numTaps, filtCoeffs, &indata[offset]); 
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, filtCoeffs);
	 /* NOTE: this is an error condition, since filter coefficients */
	 /* could not be adapted at run time if in program memory. In this */
	 /* case, the function is to return a NULL pointer. */
#endif	/* ]  */
	 offset += numTaps;

         /* Find out number of reference samples. */
	 tmpElems = (int) indata[offset];
	 offset++;
	 if (numSamps != tmpElems) {
	    exit (EFLMSN);
	 } else {
	    /* Copy reference samples from static input array. */
	    refSamples = srcSamples + numSamps;
	    CopyValues (numSamps, refSamples, &indata[offset]);
	    offset += numSamps;
	 }

	 /* Get mu value. */
	 fractVal = Float2Fract ((float) indata[offset]);
	 offset++;

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = refSamples + numSamps;

         /* Apply operation. */
	 FIRStructInit (FIRFilter, numTaps, filtCoeffs, coeffsPage, filtDelays);
	 FIRDelayInit (FIRFilter);
#if	DATA_TYPE==FLOATING		/* [ */
	 dstSamples = FIRLMSNorm (numSamps, dstSamples, srcSamples, FIRFilter, refSamples, fractVal, &energyEstimate);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstSamples = FIRLMSNorm (numSamps, dstSamples, srcSamples, FIRFilter, refSamples, fractVal, &energyEstimate);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w9" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstSamples = FIRLMSNorm (numSamps, dstSamples, srcSamples, FIRFilter, refSamples, fractVal, &energyEstimate);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }

	 /* Delay values are ordered increasingly and linearly */
	 /* from base address. */
	 for (cntr = 0; cntr < numTaps; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

	 /* Report energy estimate for last iteration. */
         printf ("%2.20f\n", Fract2Float (energyEstimate));

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numSamps+numTaps+1;	/* num results */
	 for ( ; cntr <= numSamps; cntr++) {
	    BufferAuxData[cntr] = *(dstSamples++);
	 }

	 /* Delay values are ordered decreasingly and modularly */
	 /* from (FIRFilter->delay)-1 address. */
	 /* Find out location of fist delay. */
	 dstSamples = FIRFilter->delay;			/* reuse dstSamples */
	 tmpElems = dstSamples - filtDelays;		/* offset next delay */
	 for (offset = tmpElems; offset >= 0; offset--) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }
	 for (offset = numTaps-1; offset > tmpElems; offset--) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }

	 /* Report energy estimate for last iteration. */
         BufferAuxData[cntr++] = energyEstimate;

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case FLAT:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numSamps);
   
         /* Load data samples. */
	 srcSamples = BufferData;
         LoadValues (srcSamples, numSamps, fid); 

         /* Find out filter order. */
	 fscanf (fid, "%d\n", &filtOrder);
   
         /* Load filter coefficients. */
	 kappaVals = BufferCoeffs;
         LoadValues (kappaVals, filtOrder, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

         /* Apply operation. */
	 FIRStructInit (FIRFilter, filtOrder, kappaVals, coeffsPage, filtDelays);
	 FIRDelayInit (FIRFilter);
	 dstSamples = FIRLattice (numSamps, dstSamples, srcSamples, FIRFilter);

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr < filtOrder; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_FLAT		/* [ FIR lattice validation */

         /* Find out number of input samples. */
	 numSamps = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcSamples = BufferData;
         CopyValues (numSamps, srcSamples, &indata[offset]); 
	 offset += numSamps;

         /* Find out number of filter coefficients. */
	 filtOrder = (int) indata[offset];
	 offset++;
   
	 /* Copy filter coefficients from static input array. */
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 kappaVals = BufferCoeffs;
         CopyValues (filtOrder, kappaVals, &indata[offset]); 
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, kappaVals);
#else	/* ][ */
	 coeffsPage = 0;				/* forced from linker */
	 						/* control file */
	 kappaVals = (fractional*) ModBufferPMEM;	/* makes dsPIC reset */
	 kappaVals = (fractional*) 0x0B000;		/* forced from linker */
	 						/* control file */
#endif	/* ]  */
#endif	/* ]  */
	 offset += filtOrder;

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

         /* Apply operation. */
	 FIRStructInit (FIRFilter, filtOrder, kappaVals, coeffsPage, filtDelays);
	 FIRDelayInit (FIRFilter);
#if	DATA_TYPE==FLOATING		/* [ */
	 dstSamples = FIRLattice (numSamps, dstSamples, srcSamples, FIRFilter);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstSamples = FIRLattice (numSamps, dstSamples, srcSamples, FIRFilter);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w12" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstSamples = FIRLattice (numSamps, dstSamples, srcSamples, FIRFilter);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr < filtOrder; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numSamps+filtOrder;	/* num results */
	 for ( ; cntr <= numSamps; cntr++) {
	    BufferAuxData[cntr] = *(dstSamples++);
	 }

	 /* Delay values are ordered increasingly from base address. */
	 for (offset = 0; offset < filtOrder; offset++) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case IIRC:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numSamps);
   
         /* Load data samples. */
	 srcSamples = BufferData;
         LoadValues (srcSamples, numSamps, fid); 

         /* Find out number of biquadratic sections. */
	 fscanf (fid, "%d\n", &numSects);
   
         /* Find out initial gain. */
	 fscanf (fid, "%lf\n", &doubleVal);
	 initGain = Float2Fract ((float) doubleVal);
   
         /* Find out final shift. */
	 fscanf (fid, "%d\n", &finShift);
   
         /* Load filter coefficients. */
	 filtCoeffs = BufferCoeffs;
         LoadValues (filtCoeffs, numSects*5, fid);	/* {a2,a1,b2,b1,b0} */
	 						/* per section */

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

	 /* Initialize filter structure. */
	 iirCanonic->numSectionsLess1 = numSects-1;
	 iirCanonic->coeffsBase = filtCoeffs;
	 iirCanonic->delayBase = filtDelays;
	 iirCanonic->coeffsPage = coeffsPage;
	 iirCanonic->initialGain = initGain;
	 iirCanonic->finalShift = finShift;

         /* Apply operation. */
	 IIRCanonicInit (iirCanonic);
	 dstSamples = IIRCanonic (numSamps, dstSamples, srcSamples, iirCanonic);

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr < numSects*2; cntr++) {	/* 2 delays/section */
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_IIRC		/* [ IIR Canonic validation */

         /* Find out number of input samples. */
	 numSamps = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcSamples = BufferData;
         CopyValues (numSamps, srcSamples, &indata[offset]); 
	 offset += numSamps;

         /* Find out number of biquadratic sections. */
	 numSects = (int) indata[offset];
	 offset++;
   
         /* Find out initial gain. */
	 initGain = Float2Fract ((float) indata[offset]);
	 offset++;
   
         /* Find out final shift. */
	 finShift = (int) indata[offset];
	 offset++;
   
	 /* Copy filter coefficients from static input array. */
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 filtCoeffs = BufferCoeffs;
         CopyValues (numSects*5, filtCoeffs, &indata[offset]); 
	 						/* {a2,a1,b2,b1,b0} */
	 						/* per section */
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, filtCoeffs);
#else	/* ][ */
	 coeffsPage = 0;				/* forced from linker */
	 						/* control file */
	 filtCoeffs = (fractional*) ModBufferPMEM;	/* makes dsPIC reset */
#endif	/* ]  */
#endif	/* ]  */
	 offset += numSects*5;

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

	 /* Initialize filter structure. */
	 iirCanonic->numSectionsLess1 = numSects-1;
#ifndef	IAR_TOOLS			/* [ */
	 iirCanonic->coeffsBase = filtCoeffs;
#else	/* ][ */
	 iirCanonic->coeffsBase = (fractional*) 0x0B000;/* forced from linker */
	 						/* control file */
#endif	/* ]  */
	 iirCanonic->delayBase = filtDelays;
	 iirCanonic->coeffsPage = coeffsPage;
	 iirCanonic->initialGain = initGain;
	 iirCanonic->finalShift = finShift;

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 IIRCanonicInit (iirCanonic);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 IIRCanonicInit (iirCanonic);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 IIRCanonicInit (iirCanonic);
#endif	/* ]  */
#endif	/* ]  */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstSamples = IIRCanonic (numSamps, dstSamples, srcSamples, iirCanonic);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstSamples = IIRCanonic (numSamps, dstSamples, srcSamples, iirCanonic);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w14" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstSamples = IIRCanonic (numSamps, dstSamples, srcSamples, iirCanonic);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr < numSects*2; cntr++) {	/* 2 delays/section */
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numSamps+numSects*2;	/* num results */
	 for ( ; cntr <= numSamps; cntr++) {
	    BufferAuxData[cntr] = *(dstSamples++);
	 }

	 /* Delay values are ordered increasingly from base address. */
	 for (offset = 0; offset < numSects*2; offset++) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case IIRT:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numSamps);
   
         /* Load data samples. */
	 srcSamples = BufferData;
         LoadValues (srcSamples, numSamps, fid); 

         /* Find out number of biquadratic sections. */
	 fscanf (fid, "%d\n", &numSects);
   
         /* Find out initial gain. */
	 /* Not used in transposed implementations!!! */
	 /* Yet, in data file for homogeneity with canonic from. */
	 fscanf (fid, "%lf\n", &doubleVal);
	 initGain = Float2Fract ((float) doubleVal);
   
         /* Find out final shift. */
	 fscanf (fid, "%d\n", &finShift);
   
         /* Load filter coefficients. */
	 filtCoeffs = BufferCoeffs;
         LoadValues (filtCoeffs, numSects*5, fid);	/* {b0,b1,a1,b2,a2} */
	 						/* per section */

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

	 /* Initialize filter structure. */
	 iirTransposed->numSectionsLess1 = numSects-1;
	 iirTransposed->coeffsBase = filtCoeffs;
	 iirTransposed->delayBase1 = filtDelays;
	 iirTransposed->delayBase2 = filtDelays+numSects;
	 iirTransposed->coeffsPage = coeffsPage;
	 iirTransposed->finalShift = finShift;

         /* Apply operation. */
	 IIRTransposedInit (iirTransposed);
	 dstSamples = IIRTransposed (numSamps, dstSamples, srcSamples, iirTransposed);

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr < numSects*2; cntr++) {	/* 2 delays/section */
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_IIRT		/* [ IIR Transposed validation */

         /* Find out number of input samples. */
	 numSamps = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcSamples = BufferData;
         CopyValues (numSamps, srcSamples, &indata[offset]); 
	 offset += numSamps;

         /* Find out number of biquadratic sections. */
	 numSects = (int) indata[offset];
	 offset++;
   
         /* Find out initial gain. */
	 /* Not used in transposed implementations!!! */
	 /* Yet, in data array for homogeneity with canonic from. */
	 initGain = Float2Fract ((float) indata[offset]);
	 offset++;
   
         /* Find out final shift. */
	 finShift = (int) indata[offset];
	 offset++;
   
	 /* Copy filter coefficients from static input array. */
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 filtCoeffs = BufferCoeffs;
         CopyValues (numSects*5, filtCoeffs, &indata[offset]); 
	 						/* {b0,b1,a1,b2,a2} */
	 						/* per section */
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, filtCoeffs);
#else	/* ][ */
	 coeffsPage = 0;				/* forced from linker */
	 						/* control file */
	 filtCoeffs = (fractional*) ModBufferPMEM;	/* makes dsPIC reset */
#endif	/* ]  */
#endif	/* ]  */
	 offset += numSects*5;

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

	 /* Initialize filter structure. */
	 iirTransposed->numSectionsLess1 = numSects-1;
#ifndef	IAR_TOOLS			/* [ */
	 iirTransposed->coeffsBase = filtCoeffs;
#else	/* ][ */
	 iirTransposed->coeffsBase = (fractional*) 0x0B000;
	 						/* forced from linker */
	 						/* control file */
#endif	/* ]  */
	 iirTransposed->delayBase1 = filtDelays;
	 iirTransposed->delayBase2 = filtDelays+numSects;
	 iirTransposed->coeffsPage = coeffsPage;
	 iirTransposed->finalShift = finShift;

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 IIRTransposedInit (iirTransposed);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 IIRTransposedInit (iirTransposed);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 IIRTransposedInit (iirTransposed);
#endif	/* ]  */
#endif	/* ]  */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstSamples = IIRTransposed (numSamps, dstSamples, srcSamples, iirTransposed);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstSamples = IIRTransposed (numSamps, dstSamples, srcSamples, iirTransposed);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w14" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstSamples = IIRTransposed (numSamps, dstSamples, srcSamples, iirTransposed);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr < numSects*2; cntr++) {	/* 2 delays/section */
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numSamps+numSects*2;	/* num results */
	 for ( ; cntr <= numSamps; cntr++) {
	    BufferAuxData[cntr] = *(dstSamples++);
	 }

	 /* Delay values are ordered increasingly from base address. */
	 for (offset = 0; offset < numSects*2; offset++) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case ILAT:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numSamps);
   
         /* Load data samples. */
	 srcSamples = BufferData;
         LoadValues (srcSamples, numSamps, fid); 

         /* Find out filter order. */
	 fscanf (fid, "%d\n", &filtOrder);
   
         /* Load filter coefficients (kappa values). */
	 kappaVals = BufferCoeffs;
         LoadValues (kappaVals, filtOrder+1, fid); 

	 /* Load filter coefficients (gamma values). */
	 gammaVals = kappaVals + filtOrder+1;
	 LoadValues (gammaVals, filtOrder+1, fid);

	 /* Try avoiding saturation. */
	 /* Get scale value. */
	 fscanf (fid, "%lf\n", &doubleVal);
	 fractVal = Float2Fract ((float) doubleVal);
	 srcSamples = VectorScale (numSamps, srcSamples, srcSamples, fractVal);

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

	 /* Initialize filter structure. */
	 iirLattice->order = filtOrder;
	 iirLattice->kappaVals = kappaVals;
	 iirLattice->gammaVals = gammaVals;
	 iirLattice->coeffsPage = coeffsPage;
	 iirLattice->delay = filtDelays;

         /* Apply operation. */
	 IIRLatticeInit (iirLattice);
	 dstSamples = IIRLattice (numSamps, dstSamples, srcSamples, iirLattice);

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr <= filtOrder; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_ILAT		/* [ IIR Lattice validation */

         /* Find out number of input samples. */
	 numSamps = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcSamples = BufferData;
         CopyValues (numSamps, srcSamples, &indata[offset]); 
	 offset += numSamps;

         /* Find out filter order. */
	 filtOrder = (int) indata[offset];
	 offset++;
   
	 /* Copy filter coefficients (kappa values) from static input array. */
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 kappaVals = BufferCoeffs;
         CopyValues (filtOrder+1, kappaVals, &indata[offset]); 
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, kappaVals);
#else	/* ][ */
	 coeffsPage = 0;				/* forced from linker */
	 						/* control file */
	 kappaVals = (fractional*) ModBufferPMEM;	/* makes dsPIC reset */
	 kappaVals = (fractional*) 0x0B000;		/* forced from linker */
	 						/* control file */
#endif	/* ]  */
#endif	/* ]  */
	 offset += filtOrder+1;


	 /* Copy filter coefficients (gamma values) from static input array. */
	 gammaVals = kappaVals + filtOrder+1;
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
         CopyValues (filtOrder+1, gammaVals, &indata[offset]); 
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
#endif	/* ]  */
	 offset += filtOrder+1;

	 /* Try avoiding saturation. */
	 /* Get scale value. */
	 fractVal = Float2Fract ((float) indata[offset]);
	 offset++;
	 srcSamples = VectorScale (numSamps, srcSamples, srcSamples, fractVal);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

	 /* Initialize filter structure. */
	 iirLattice->order = filtOrder;
	 iirLattice->kappaVals = kappaVals;
	 iirLattice->gammaVals = gammaVals;
	 iirLattice->coeffsPage = coeffsPage;
	 iirLattice->delay = filtDelays;

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 IIRLatticeInit (iirLattice);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 IIRLatticeInit (iirLattice);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 IIRLatticeInit (iirLattice);
#endif	/* ]  */
#endif	/* ]  */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstSamples = IIRLattice (numSamps, dstSamples, srcSamples, iirLattice);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstSamples = IIRLattice (numSamps, dstSamples, srcSamples, iirLattice);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w14" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstSamples = IIRLattice (numSamps, dstSamples, srcSamples, iirLattice);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr <= filtOrder; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numSamps+filtOrder+1;	/* num results */
	 for ( ; cntr <= numSamps; cntr++) {
	    BufferAuxData[cntr] = *(dstSamples++);
	 }

	 /* Delay values are ordered increasingly from base address. */
	 for (offset = 0; offset <= filtOrder; offset++) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case ILAT_AP:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numSamps);
   
         /* Load data samples. */
	 srcSamples = BufferData;
         LoadValues (srcSamples, numSamps, fid); 

         /* Find out filter order. */
	 fscanf (fid, "%d\n", &filtOrder);
   
         /* Load filter coefficients (kappa values). */
	 kappaVals = BufferCoeffs;
         LoadValues (kappaVals, filtOrder+1, fid); 

	 /* ATTENTION!!! IIR Lattice All Pole filter will be */
	 /* tested (for the time being) using the same set of */
	 /* zero-pole filter coefficients as for the regular */
	 /* IIR Lattice filter test. The only difference being */
	 /* that the gamma values are discarded... */
	 /* NOTE that we still have to read all the gamma values */
	 /* so that we can read the scaling to avoid saturation. */
	 /* Load filter coefficients (gamma values). */
	 gammaVals = kappaVals + filtOrder+1;
	 LoadValues (gammaVals, filtOrder+1, fid);

	 /* Try avoiding saturation. */
	 /* Get scale value. */
	 fscanf (fid, "%lf\n", &doubleVal);
	 fractVal = Float2Fract ((float) doubleVal);
	 srcSamples = VectorScale (numSamps, srcSamples, srcSamples, fractVal);

         /* Close file (before I forget). */
         fclose (fid);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

	 /* Initialize filter structure. */
	 iirLattice->order = filtOrder;
	 iirLattice->kappaVals = kappaVals;
	 iirLattice->gammaVals = NULL;
	 iirLattice->coeffsPage = coeffsPage;
	 iirLattice->delay = filtDelays;

         /* Apply operation. */
	 IIRLatticeInit (iirLattice);
	 dstSamples = IIRLattice (numSamps, dstSamples, srcSamples, iirLattice);

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr <= filtOrder; cntr++) {
            printf ("%2.20f\n", Fract2Float (filtDelays[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_ILAT_AP		/* [ IIR Lattice (all-pole) valid. */

         /* Find out number of input samples. */
	 numSamps = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 srcSamples = BufferData;
         CopyValues (numSamps, srcSamples, &indata[offset]); 
	 offset += numSamps;

         /* Find out number of filter coefficients. */
	 filtOrder = (int) indata[offset];
	 offset++;
   
	 /* Copy filter coefficients from static input array. */
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 kappaVals = BufferCoeffs;
         CopyValues (filtOrder+1, kappaVals, &indata[offset]); 
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, kappaVals);
#else	/* ][ */
	 coeffsPage = 0;				/* forced from linker */
	 						/* control file */
	 kappaVals = (fractional*) ModBufferPMEM;	/* makes dsPIC reset */
	 kappaVals = (fractional*) 0x0B000;		/* forced from linker */
	 						/* control file */
#endif	/* ]  */
#endif	/* ]  */
	 offset += filtOrder+1;

	 /* ATTENTION!!! IIR Lattice All Pole filter will be */
	 /* tested (for the time being) using the same set of */
	 /* zero-pole filter coefficients as for the regular */
	 /* IIR Lattice filter test. The only difference being */
	 /* that the gamma values are discarded... */
	 /* NOTE that we still have to read all the gamma values */
	 /* so that we can read the scaling to avoid saturation. */
	 /* Copy filter coefficients (gamma values) from static input array. */
	 gammaVals = kappaVals + filtOrder+1;
#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
         CopyValues (filtOrder+1, gammaVals, &indata[offset]); 
#else	/* ][ */
	 /* Filter coefficients already in P memory at 'ModBufferPMEM' */
#endif	/* ]  */
	 offset += filtOrder+1;

	 /* Try avoiding saturation. */
	 /* Get scale value. */
	 fractVal = Float2Fract ((float) indata[offset]);
	 offset++;
	 srcSamples = VectorScale (numSamps, srcSamples, srcSamples, fractVal);

	 /* Assign memory for delay and destination samples. */
	 filtDelays = BufferDelays;
	 dstSamples = srcSamples + numSamps;

	 /* Initialize filter structure. */
	 iirLattice->order = filtOrder;
	 iirLattice->kappaVals = kappaVals;
	 iirLattice->gammaVals = NULL;
	 iirLattice->coeffsPage = coeffsPage;
	 iirLattice->delay = filtDelays;

         /* Apply operation. */
	 IIRLatticeInit (iirLattice);
#if	DATA_TYPE==FLOATING		/* [ */
	 dstSamples = IIRLattice (numSamps, dstSamples, srcSamples, iirLattice);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstSamples = IIRLattice (numSamps, dstSamples, srcSamples, iirLattice);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w14" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstSamples = IIRLattice (numSamps, dstSamples, srcSamples, iirLattice);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numSamps; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstSamples[cntr]));
	 }
	 for (cntr = 0; cntr <= filtOrder; cntr++) {
            printf ("%2.20f\n", (Fract2Float (filtDelays[cntr])));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numSamps+filtOrder+1;	/* num results */
	 for ( ; cntr <= numSamps; cntr++) {
	    BufferAuxData[cntr] = *(dstSamples++);
	 }

	 /* Delay values are ordered increasingly from base address. */
	 for (offset = 0; offset <= filtOrder; offset++) {
	    BufferAuxData[cntr++] = filtDelays[offset];
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case FFT_OOP:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input complex samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load input complex samples. */
	 srcVectorComplexOne = (fractcomplex*) BufferData;
         LoadVectorComplex (srcVectorComplexOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Figure out FFT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EFFT_OOP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

	 /* Assign memory for destination vector. */
	 dstVectorComplex = srcVectorComplexOne+numElems;

	 /* Generate twiddle factors. */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 0);

         /* Apply operation. */
	 FFTComplex (log2N, dstVectorComplex, srcVectorComplexOne, twidFactors, coeffsPage);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVectorComplex[cntr].real));
            printf ("%2.20f\n", Fract2Float (dstVectorComplex[cntr].imag));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_FFT_OOP		/* [ FFT (out of place) validation */

         /* Find out number of input complex samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data complex samples from static input array. */
	 srcVectorComplexOne = (fractcomplex*) BufferData;
         CopyValues (numElems*2, (fractional*) srcVectorComplexOne, &indata[offset]); 
	 offset += numElems*2;

	 /* Figure out FFT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EFFT_OOP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

	 /* Assign memory for destination vector. */
	 dstVectorComplex = srcVectorComplexOne+numElems;

#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 /* Generate twiddle factors. */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 0);
#else	/* ][ */
	 /* Twiddle factors already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, twidFactors);
#else	/* ][ */
	 coeffsPage = 0;				/* forced from linker */
	 						/* control file */
	 cntr = (int) *ModBufferPMEM;			/* makes dsPIC reset */
	 twidFactors = (fractcomplex*) 0x0B000;		/* forced from linker */
	 						/* control file */
#endif	/* ]  */
#endif	/* ]  */

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 FFTComplex (log2N, dstVectorComplex, srcVectorComplexOne, twidFactors, coeffsPage);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 FFTComplex (log2N, dstVectorComplex, srcVectorComplexOne, twidFactors, coeffsPage);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 FFTComplex (log2N, dstVectorComplex, srcVectorComplexOne, twidFactors, coeffsPage);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVectorComplex[cntr].real));
            printf ("%2.20f\n", Fract2Float (dstVectorComplex[cntr].imag));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems*2;		/* num results */
	 dstVector = (fractional*) dstVectorComplex;	/* reuse dstVector */
	 for ( ; cntr <= numElems*2; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case FFT_IP:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input complex samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load input complex samples. */
	 srcVectorComplexOne = (fractcomplex*) BufferData;
         LoadVectorComplex (srcVectorComplexOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Figure out FFT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EFFT_IP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

	 /* Generate twiddle factors. */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 0);

         /* Apply operation. */
	 FFTComplexIP (log2N, srcVectorComplexOne, twidFactors, (int) COEFFS_IN_DATA);
	 BitReverseComplex (log2N, srcVectorComplexOne);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (srcVectorComplexOne[cntr].real));
            printf ("%2.20f\n", Fract2Float (srcVectorComplexOne[cntr].imag));
	 }
	 for (cntr = 0; cntr < numElems/2; cntr++) {
            printf ("%2.20f\n", Fract2Float (twidFactors[cntr].real));
            printf ("%2.20f\n", Fract2Float (twidFactors[cntr].imag));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_FFT_IP		/* [ FFT (in place) validation */

         /* Find out number of input complex samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data complex samples from static input array. */
	 srcVectorComplexOne = (fractcomplex*) BufferData;
         CopyValues (numElems*2, (fractional*) srcVectorComplexOne, &indata[offset]); 
	 offset += numElems*2;

	 /* Figure out FFT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EFFT_IP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 /* Generate twiddle factors. */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 0);
#else	/* ][ */
	 /* Twiddle factors already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, twidFactors);
#else	/* ][ */
	 /* Not tested for IAR tools... */
	 /* Test relies on 'FFTComplex' */
#endif	/* ]  */
#endif	/* ]  */

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 FFTComplexIP (log2N, srcVectorComplexOne, twidFactors, coeffsPage);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 FFTComplexIP (log2N, srcVectorComplexOne, twidFactors, coeffsPage);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 FFTComplexIP (log2N, srcVectorComplexOne, twidFactors, coeffsPage);
#endif	/* ]  */
#endif	/* ]  */
#if	DATA_TYPE==FLOATING		/* [ */
	 BitReverseComplex (log2N, srcVectorComplexOne);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 BitReverseComplex (log2N, srcVectorComplexOne);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 BitReverseComplex (log2N, srcVectorComplexOne);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (srcVectorComplexOne[cntr].real));
            printf ("%2.20f\n", Fract2Float (srcVectorComplexOne[cntr].imag));
	 }
	 for (cntr = 0; cntr < numElems/2; cntr++) {
            printf ("%2.20f\n", Fract2Float (twidFactors[cntr].real));
            printf ("%2.20f\n", Fract2Float (twidFactors[cntr].imag));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems*3;		/* num results */
	 dstVector = (fractional*) srcVectorComplexOne;	/* reuse dstVector */
	 for (offset = 0; offset < numElems*2; offset++) {
	    BufferAuxData[cntr++] = *(dstVector++);
	 }
	 dstVector = (fractional*) twidFactors;		/* reuse dstVector */
	 for (offset = 0; offset < numElems; offset++) {
	    BufferAuxData[cntr++] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case IFFT_OOP:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input complex samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load input complex samples. */
	 srcVectorComplexOne = (fractcomplex*) BufferData;
         LoadVectorComplex (srcVectorComplexOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Figure out IFFT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EIFFT_OOP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

	 /* Assign memory for destination vector. */
	 dstVectorComplex = srcVectorComplexOne+numElems;

	 /* Generate twiddle factors (complex conjugates). */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 1);

         /* Apply operation. */
	 IFFTComplex (log2N, dstVectorComplex, srcVectorComplexOne, twidFactors, (int) COEFFS_IN_DATA);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVectorComplex[cntr].real));
            printf ("%2.20f\n", Fract2Float (dstVectorComplex[cntr].imag));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_IFFT_OOP	/* [ IFFT (out of place) validation */

         /* Find out number of input complex samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data complex samples from static input array. */
	 srcVectorComplexOne = (fractcomplex*) BufferData;
         CopyValues (numElems*2, (fractional*) srcVectorComplexOne, &indata[offset]); 
	 offset += numElems*2;

	 /* Figure out IFFT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EIFFT_OOP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

	 /* Assign memory for destination vector. */
	 dstVectorComplex = srcVectorComplexOne+numElems;

#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 /* Generate twiddle factors (complex conjugates). */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 1);
#else	/* ][ */
	 /* Twiddle factors already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, twidFactors);
#else	/* ][ */
	 coeffsPage = 0;				/* forced from linker */
	 						/* control file */
	 cntr = (int) *ModBufferPMEM;			/* makes dsPIC reset */
	 twidFactors = (fractcomplex*) 0x0B000;		/* forced from linker */
	 						/* control file */
#endif	/* ]  */
#endif	/* ]  */

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 IFFTComplex (log2N, dstVectorComplex, srcVectorComplexOne, twidFactors, coeffsPage);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 IFFTComplex (log2N, dstVectorComplex, srcVectorComplexOne, twidFactors, coeffsPage);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 IFFTComplex (log2N, dstVectorComplex, srcVectorComplexOne, twidFactors, coeffsPage);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVectorComplex[cntr].real));
            printf ("%2.20f\n", Fract2Float (dstVectorComplex[cntr].imag));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems*2;		/* num results */
	 dstVector = (fractional*) dstVectorComplex;	/* reuse dstVector */
	 for ( ; cntr <= numElems*2; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case IFFT_IP:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input complex samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load input complex samples. */
	 srcVectorComplexOne = (fractcomplex*) BufferData;
         LoadVectorComplex (srcVectorComplexOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Figure out IFFT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EIFFT_IP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

	 /* Generate twiddle factors (complex conjugates). */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 1);

         /* Apply operation. */
	 IFFTComplexIP (log2N, srcVectorComplexOne, twidFactors, (int) COEFFS_IN_DATA);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems/2; cntr++) {
            printf ("%2.20f\n", Fract2Float (twidFactors[cntr].real));
            printf ("%2.20f\n", Fract2Float (twidFactors[cntr].imag));
	 }
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (srcVectorComplexOne[cntr].real));
            printf ("%2.20f\n", Fract2Float (srcVectorComplexOne[cntr].imag));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_IFFT_IP		/* [ IFFT (in place) validation */

         /* Find out number of input complex samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data complex samples from static input array. */
	 srcVectorComplexOne = (fractcomplex*) BufferData;
         CopyValues (numElems*2, (fractional*) srcVectorComplexOne, &indata[offset]); 
	 offset += numElems*2;

	 /* Figure out IFFT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EIFFT_IP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 /* Generate twiddle factors (complex conjugates). */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 1);
#else	/* ][ */
	 /* Twiddle factors already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, twidFactors);
#else	/* ][ */
	 /* Not tested for IAR tools... */
	 /* Test relies on 'IFFTComplex' */
#endif	/* ]  */
#endif	/* ]  */

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 IFFTComplexIP (log2N, srcVectorComplexOne, twidFactors, coeffsPage);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 IFFTComplexIP (log2N, srcVectorComplexOne, twidFactors, coeffsPage);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 IFFTComplexIP (log2N, srcVectorComplexOne, twidFactors, coeffsPage);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (srcVectorComplexOne[cntr].real));
            printf ("%2.20f\n", Fract2Float (srcVectorComplexOne[cntr].imag));
	 }
	 for (cntr = 0; cntr < numElems/2; cntr++) {
            printf ("%2.20f\n", Fract2Float (twidFactors[cntr].real));
            printf ("%2.20f\n", Fract2Float (twidFactors[cntr].imag));
	 }

#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems*3;		/* num results */
	 dstVector = (fractional*) srcVectorComplexOne;	/* reuse dstVector */
	 for (offset = 0; offset < numElems*2; offset++) {
	    BufferAuxData[cntr++] = *(dstVector++);
	 }
	 dstVector = (fractional*) twidFactors;		/* reuse dstVector */
	 for (offset = 0; offset < numElems; offset++) {
	    BufferAuxData[cntr++] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case DCT_OOP:

	 /* NOTE: contrary to all the other tests, the destination vector */
	 /* is here allocated prior to the source vector; then the source */
	 /* vector is allocated behind it (leaving enough room to zero pad */
	 /* destination vector. The reason for this change is that the out */
	 /* of place DCT routine applies a bit reverse operation to the */
	 /* first N elements of the destination vector. If destination is */
	 /* allocated in the customary fashion (numElems after the source */
	 /* vector, the latter allocated in BufferData) then the address */
	 /* of destination vector might not be modulo aligned. And for some */
	 /* reason, still under Microchip's investigation, it seems that */
	 /* for a buffer to be bit reversed, it is MANDATORY that the buffer */
	 /* be modulo aligned during its allocation... */
	 /* NOTE: this situation is not encountered with the other two */
	 /* out of place operations, FFT oop, and IFFT oop, because in */
	 /* those two cases the destination vector is allocated N*2 */
	 /* elements after the source vector (since source is complex). */
	 /* It seems then that the destination vector is always allocated */
	 /* with modulo alignment... Since the modulo alignment has not */
	 /* yet been identified as a requirement of the system, but rather */
	 /* it seems like a bug, the FFT oop and IFFT oop tests have not */
	 /* been modified. If the modulo alignment becomes a requirement, */
	 /* both tests may have to be reviewed... */

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
	 /* Assign memory for destination vector. */
	 /* dstVector must have enough room for numElems zero padding. */
	 dstVector = BufferData;

         /* Load data samples. */
	 srcVectorOne = dstVector + numElems*2;
         LoadVector (srcVectorOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Figure out DCT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EDCT_OOP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

	 /* Generate twiddle factors (complex conjugates). */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 1);

	 /* Generate cosine factors. */
	 cosFactors = twidFactors + numElems/2;
	 CosFactorInit (log2N, cosFactors);

         /* Apply operation. */
	 dstVector = DCT (log2N, dstVector, srcVectorOne, cosFactors, twidFactors, coeffsPage);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_DCT_OOP		/* [ DCT (out of place) validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Assign memory for destination vector. */
	 /* dstVector must have enough room for numElems zero padding. */
	 dstVector = BufferData;

	 /* Copy data samples from static input array. */
	 srcVectorOne = dstVector + numElems*2;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Figure out DCT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EDCT_OOP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 /* Generate twiddle factors (complex conjugates). */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 1);

	 /* Generate cosine factors. */
	 cosFactors = twidFactors + numElems/2;
	 CosFactorInit (log2N, cosFactors);
#else	/* ][ */
	 /* Twiddle factors already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, twidFactors);
#else	/* ][ */
	 coeffsPage = 0;				/* forced from linker */
	 						/* control file */
	 cntr = (int) *ModBufferPMEM;			/* makes dsPIC reset */
	 twidFactors = (fractcomplex*) 0x0B000;		/* forced from linker */
	 						/* control file */
#endif	/* ]  */

	 /* Cosine factors already in P memory at 'ModBufferPMEM' */
	 cosFactors = twidFactors + numElems/2;
#endif	/* ]  */

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 dstVector = DCT (log2N, dstVector, srcVectorOne, cosFactors, twidFactors, coeffsPage);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 dstVector = DCT (log2N, dstVector, srcVectorOne, cosFactors, twidFactors, coeffsPage);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w10" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 dstVector = DCT (log2N, dstVector, srcVectorOne, cosFactors, twidFactors, coeffsPage);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (dstVector[cntr]));
	 }


#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      case DCT_IP:

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

         /* Find out number of input samples. */
	 fscanf (fid, "%d\n", &numElems);
   
         /* Load data samples. */
	 /* srcVectorOne must have enough room for numElems zero padding. */
	 srcVectorOne = BufferData;
         LoadVector (srcVectorOne, numElems, fid); 

         /* Close file (before I forget). */
         fclose (fid);

	 /* Figure out DCT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EDCT_IP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

	 /* Generate twiddle factors (complex conjugates). */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 1);

	 /* Generate cosine factors. */
	 cosFactors = twidFactors + numElems/2;
	 CosFactorInit (log2N, cosFactors);

	 /* Zero padd input data vector */
	 srcVectorOne = VectorZeroPad (numElems, numElems, srcVectorOne, srcVectorOne);

         /* Apply operation. */
	 DCTIP (log2N, srcVectorOne, cosFactors, twidFactors, coeffsPage);

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (srcVectorOne[cntr]));
	 }
	 for (cntr = 0; cntr < numElems/2; cntr++) {
            printf ("%2.20f\n", Fract2Float (cosFactors[cntr].real));
            printf ("%2.20f\n", Fract2Float (cosFactors[cntr].imag));
	 }

#else	/* ][ */

#if	TEST_OPER==OPER_DCT_IP		/* [ DCT (in place) validation */

         /* Find out number of input samples. */
	 numElems = (int) indata[offset];
	 offset++;
   
	 /* Copy data samples from static input array. */
	 /* srcVectorOne must have enough room for numElems zero padding. */
	 srcVectorOne = BufferData;
         CopyValues (numElems, srcVectorOne, &indata[offset]); 
	 offset += numElems;

	 /* Figure out DCT order. */
	 intVal = numElems%2;
	 if (intVal) {
	    exit (EDCT_IP);
	 }
	 intVal = numElems>>1;
	 log2N = 0;
	 while (intVal) {
	    log2N += 1;
	    intVal = intVal>>1;
	 }

#if	IN_SPACE==IN_X_SPACE		/* [ X-Data memory space */
	 /* Generate twiddle factors (complex conjugates). */
	 twidFactors = (fractcomplex*) BufferFacts;
	 TwidFactorInit (log2N, twidFactors, 1);

	 /* Generate cosine factors. */
	 cosFactors = twidFactors + numElems/2;
	 CosFactorInit (log2N, cosFactors);
#else	/* ][ */
	 /* Twiddle factors already in P memory at 'ModBufferPMEM' */
#ifndef	IAR_TOOLS			/* [ */
	 PSVPAGE(_ModBufferPMEM, coeffsPage);
	 PSVOFFSET(_ModBufferPMEM, twidFactors);
#else	/* ][ */
	 /* Not tested for IAR tools... */
	 /* Test relies on 'DCT' */
#endif	/* ]  */

	 /* Cosine factors already in P memory at 'ModBufferPMEM' */
	 cosFactors = twidFactors + numElems/2;
#endif	/* ]  */

	 /* Zero padd input data vector */
#if	DATA_TYPE==FLOATING		/* [ */
	 srcVectorOne = VectorZeroPad (numElems, numElems, srcVectorOne, srcVectorOne);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 srcVectorOne = VectorZeroPad (numElems, numElems, srcVectorOne, srcVectorOne);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
	 test_time--;					/* compiler adds: */
	 						/* "mov w0,w12" after */
							/* function call... */
#else	/* ][ */	/* IAR */
	 srcVectorOne = VectorZeroPad (numElems, numElems, srcVectorOne, srcVectorOne);
#endif	/* ]  */
#endif	/* ]  */

         /* Apply operation. */
#if	DATA_TYPE==FLOATING		/* [ */
	 DCTIP (log2N, srcVectorOne, cosFactors, twidFactors, coeffsPage);
#else	/* ][ */
#ifndef	IAR_TOOLS			/* [ */
	 asm volatile ("clr	TMR1");			/* start timing */
	 DCTIP (log2N, srcVectorOne, cosFactors, twidFactors, coeffsPage);
	 asm volatile ("push	w0");			/* save return val */
	 asm volatile ("mov	TMR1, w0");		/* end timing */
	 asm volatile ("mov	w0, _test_time");	/* record cycle count */
	 asm volatile ("pop	w0");			/* restore return val */
	 test_time--;					/* adjust count for */
	 						/* "push w0" */
#else	/* ][ */	/* IAR */
	 DCTIP (log2N, srcVectorOne, cosFactors, twidFactors, coeffsPage);
#endif	/* ]  */
#endif	/* ]  */

#if	DATA_TYPE==FLOATING		/* [ */

	 /* Report results. */
	 for (cntr = 0; cntr < numElems; cntr++) {
            printf ("%2.20f\n", Fract2Float (srcVectorOne[cntr]));
	 }
	 for (cntr = 0; cntr < numElems/2; cntr++) {
            printf ("%2.20f\n", Fract2Float (cosFactors[cntr].real));
            printf ("%2.20f\n", Fract2Float (cosFactors[cntr].imag));
	 }


#else	/* ][ */

	 /* Place results in fixed location (BufferAuxData). */
	 cntr = 0;
	 BufferAuxData[cntr++] = numElems*2;		/* num results */
	 for ( ; cntr <= numElems; cntr++) {
	    BufferAuxData[cntr] = *(srcVectorOne++);
	 }
	 dstVector = (fractional*) cosFactors;		/* reuse dstVector */
	 for (offset = 0; offset < numElems; offset++) {
	    BufferAuxData[cntr++] = *(dstVector++);
	 }

#endif	/* ]  */

#endif	/* ]  */

#endif	/* ]  */

	 break;			/* That's it... */

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

      default:

         /* Report error, and get out... */
	 exit (ENOT_A_CODE);

/* .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . */

   } /* end switch (operCode) */

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */

} /* end of TestOperations */

/*...........................................................................*/

#if	VALIDATION==SIM_VAL		/* [ SIMPIC30 validation */

fractional* LoadVector (
   fractional* aVector,
   int numElems,
   FILE* aFile
) {

   /* Read vector values from data file. */
   LoadValues (aVector, numElems, aFile);

   /* Return loaded vector. */
   return (aVector);

   
} /* end of LoadVector */

/*...........................................................................*/

fractcomplex* LoadVectorComplex (
   fractcomplex* aVectorComplex,
   int numElems,
   FILE* aFile
) {

   /* Read vector values from data file. */
   LoadValues ((fractional*) aVectorComplex, 2*numElems, aFile);

   /* Return loaded vector. */
   return (aVectorComplex);

   
} /* end of LoadVectorComplex */

/*...........................................................................*/

void LoadValues (
   fractional* valsPtr,
   int numElems,
   FILE* aFile
) {

   /* Local declarations. */
   fractional* aVal = valsPtr;
   double value = 0;
   int cntr = 0;
   
   /* Read values from data file. */
   for (cntr=0; cntr < numElems; cntr++) {
      fscanf (aFile, "%lf\n", &value);
      *(aVal++) = Float2Fract ((float) value);
   }

} /* end of LoadValues */

/*...........................................................................*/

void LoadValuesFloat (
   float* valsPtr,
   int numElems,
   FILE* aFile
) {

   /* Local declarations. */
   float* aVal = valsPtr;
   double value = 0;
   int cntr = 0;
   
   /* Read values from data file. */
   for (cntr=0; cntr < numElems; cntr++) {
      fscanf (aFile, "%lf\n", &value);
      *(aVal++) = (float) value;
   }

} /* end of LoadValuesFloat */

/*...........................................................................*/

#else	/* ][ */

/*...........................................................................*/

void CopyValues (
   int numElems,
   fractional* dstPtr,
#ifndef	IAR_TOOLS			/* [ */
   const double* srcPtr
#else	/* ][ */
   const double __constptr * srcPtr
#endif	/* ] */
) {

   /* Local declarations. */
   fractional* dPtr = dstPtr;
#ifndef	IAR_TOOLS			/* [ */
   const double* sPtr = srcPtr;
#else	/* ][ */
   const double __constptr * sPtr = srcPtr;
#endif	/* ] */
   int cntr = 0;
   
   /* Read values from data file. */
   for (cntr=0; cntr < numElems; cntr++) {
      *(dPtr++) = Float2Fract ((float) *(sPtr++));
   }

} /* end of CopyValues */

/*...........................................................................*/

void CopyValuesFloat (
   int numElems,
   float* dstPtr,
#ifndef	IAR_TOOLS			/* [ */
   const double* srcPtr
#else	/* ][ */
   const double __constptr * srcPtr
#endif	/* ] */
) {

   /* Local declarations. */
   float* dPtr = dstPtr;
#ifndef	IAR_TOOLS			/* [ */
   const double* sPtr = srcPtr;
#else	/* ][ */
   const double __constptr * sPtr = srcPtr;
#endif	/* ] */
   int cntr = 0;
   
   /* Read values from data file. */
   for (cntr=0; cntr < numElems; cntr++) {
      *(dPtr++) = (float) *(sPtr++);
   }

} /* end of CopyValuesFloat */

#endif	/* ]  */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
