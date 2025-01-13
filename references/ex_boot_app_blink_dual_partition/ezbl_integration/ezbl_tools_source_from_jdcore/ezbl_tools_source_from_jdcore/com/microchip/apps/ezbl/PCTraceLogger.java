package com.microchip.apps.ezbl;

import java.io.File;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.PrintStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;


















public class PCTraceLogger
{
  private int[] PCs = new int['䀀'];
  private byte[] bl2Bytes;
  ELFDumpParser elf = new ELFDumpParser();
  List<String> outLines;
  String outputPath;
  Bl2b code;
  Map<Integer, Integer> instr;
  String syncString = "HELO";
  long idleTimeout = 60000L;
  int[] instanceCounts = new int['Ā'];
  int[] staticCounts = new int['Ā'];
  int staticCount = 0;
  int netBranchesTaken = 0;
  int retfies = 0;
  int returns = 0;
  int total2Cycles = 0;
  int skips = 0;
  int skipsNotTaken = 0;
  int obviousBranches = 0;
  int repeats = 0;
  int calls = 0;
  int gotoBras = 0;
  int conditionalBraches = 0;
  int conditionalBrachesTaken = 0;
  int conditionalBrachesNotTaken = 0;
  int conditionalBranchesTakenForward = 0;
  int conditionalBranchesTakenBackward = 0;
  int conditionalBranchesNotTakenForward = 0;
  int conditionalBranchesNotTakenBackward = 0;
  DatagramSocket udp = null;
  byte[] pktData = new byte['؀'];
  DatagramPacket pkt = new DatagramPacket(pktData, pktData.length);
  int pktLen;
  private File file;
  private FileWriter fileOut;
  private int fileSequence = 0;
  int traceBytesProcessed = 0;
  int netInstructions = 0;
  int logLinesThisFile;
  long fileStartTime = 0L;
  float instrPerSecond;
  private static final Map<Integer, String> OpcodeToMnemonic;
  
  public PCTraceLogger PCTraceLogger() {
    return this;
  }
  


  static
  {
    Map<Integer, String> mnemonic = new HashMap(512);
    mnemonic.put(Integer.valueOf(0), "nop");
    mnemonic.put(Integer.valueOf(65536), "bra/call/goto/rcall Wn");
    mnemonic.put(Integer.valueOf(131072), "call        #lit23");
    mnemonic.put(Integer.valueOf(196608), "-");
    mnemonic.put(Integer.valueOf(262144), "goto        #lit23");
    mnemonic.put(Integer.valueOf(327680), "retlw");
    mnemonic.put(Integer.valueOf(393216), "retfie/return");
    

    mnemonic.put(Integer.valueOf(458752), "rcall (PC rel)");
    mnemonic.put(Integer.valueOf(524288), "do");
    mnemonic.put(Integer.valueOf(589824), "repeat");
    mnemonic.put(Integer.valueOf(655360), "-");
    mnemonic.put(Integer.valueOf(720896), "-");
    mnemonic.put(Integer.valueOf(786432), "bra OA");
    mnemonic.put(Integer.valueOf(851968), "bra OB");
    mnemonic.put(Integer.valueOf(917504), "bra SA");
    mnemonic.put(Integer.valueOf(983040), "bra SB");
    mnemonic.put(Integer.valueOf(1048576), "subr.{b}      w1/w0, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1114112), "subr.{b}      w3/w2, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1179648), "subr.{b}      w5/w4, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1245184), "subr.{b}      w7/w6, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1310720), "subr.{b}      w9/w8, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1376256), "subr.{b}    w11/w10, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1441792), "subr.{b}    w13/w12, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1507328), "subr.{b}    w15/w14, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1572864), "subbr.{b}     w1/w0, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1638400), "subbr.{b}     w3/w2, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1703936), "subbr.{b}     w5/w4, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1769472), "subbr.{b}     w7/w6, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1835008), "subbr.{b}     w9/w8, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1900544), "subbr.{b}   w11/w10, #lit5, Wd");
    mnemonic.put(Integer.valueOf(1966080), "subbr.{b}   w13/w12, #lit5, Wd");
    mnemonic.put(Integer.valueOf(2031616), "subbr.{b}   w15/w14, #lit5, Wd");
    mnemonic.put(Integer.valueOf(2097152), "mov         #lit16, Wnd      (0000..0FFF)");
    mnemonic.put(Integer.valueOf(2162688), "mov         #lit16, Wnd      (1000..1FFF)");
    mnemonic.put(Integer.valueOf(2228224), "mov         #lit16, Wnd      (2000..2FFF)");
    mnemonic.put(Integer.valueOf(2293760), "mov         #lit16, Wnd      (3000..3FFF)");
    mnemonic.put(Integer.valueOf(2359296), "mov         #lit16, Wnd      (4000..4FFF)");
    mnemonic.put(Integer.valueOf(2424832), "mov         #lit16, Wnd      (5000..5FFF)");
    mnemonic.put(Integer.valueOf(2490368), "mov         #lit16, Wnd      (6000..6FFF)");
    mnemonic.put(Integer.valueOf(2555904), "mov         #lit16, Wnd      (7000..7FFF)");
    mnemonic.put(Integer.valueOf(2621440), "mov         #lit16, Wnd      (8000..8FFF)");
    mnemonic.put(Integer.valueOf(2686976), "mov         #lit16, Wnd      (9000..9FFF)");
    mnemonic.put(Integer.valueOf(2752512), "mov         #lit16, Wnd      (A000..AFFF)");
    mnemonic.put(Integer.valueOf(2818048), "mov         #lit16, Wnd      (B000..BFFF)");
    mnemonic.put(Integer.valueOf(2883584), "mov         #lit16, Wnd      (C000..CFFF)");
    mnemonic.put(Integer.valueOf(2949120), "mov         #lit16, Wnd      (D000..DFFF)");
    mnemonic.put(Integer.valueOf(3014656), "mov         #lit16, Wnd      (E000..EFFF)");
    mnemonic.put(Integer.valueOf(3080192), "mov         #lit16, Wnd      (F000..FFFF)");
    mnemonic.put(Integer.valueOf(3145728), "bra OV");
    mnemonic.put(Integer.valueOf(3211264), "bra C");
    mnemonic.put(Integer.valueOf(3276800), "bra Z");
    mnemonic.put(Integer.valueOf(3342336), "bra N");
    mnemonic.put(Integer.valueOf(3407872), "bra LE");
    mnemonic.put(Integer.valueOf(3473408), "bra LT");
    mnemonic.put(Integer.valueOf(3538944), "bra LEU");
    mnemonic.put(Integer.valueOf(3604480), "bra (PC rel)");
    mnemonic.put(Integer.valueOf(3670016), "bra NOV");
    mnemonic.put(Integer.valueOf(3735552), "bra NC");
    mnemonic.put(Integer.valueOf(3801088), "bra NZ");
    mnemonic.put(Integer.valueOf(3866624), "bra NN");
    mnemonic.put(Integer.valueOf(3932160), "bra GT");
    mnemonic.put(Integer.valueOf(3997696), "bra GE");
    mnemonic.put(Integer.valueOf(4063232), "bra GTU");
    mnemonic.put(Integer.valueOf(4128768), "-");
    mnemonic.put(Integer.valueOf(4194304), "add.{b}       w1/w0, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4259840), "add.{b}       w3/w2, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4325376), "add.{b}       w5/w4, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4390912), "add.{b}       w7/w6, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4456448), "add.{b}       w9/w8, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4521984), "add.{b}     w11/w10, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4587520), "add.{b}     w13/w12, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4653056), "add.{b}     w15/w14, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4718592), "addc.{b}      w1/w0, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4784128), "addc.{b}      w3/w2, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4849664), "addc.{b}      w5/w4, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4915200), "addc.{b}      w7/w6, #lit5, Wd");
    mnemonic.put(Integer.valueOf(4980736), "addc.{b}      w9/w8, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5046272), "addc.{b}    w11/w10, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5111808), "addc.{b}    w13/w12, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5177344), "addc.{b}    w15/w14, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5242880), "sub.{b}       w1/w0, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5308416), "sub.{b}       w3/w2, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5373952), "sub.{b}       w5/w4, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5439488), "sub.{b}       w7/w6, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5505024), "sub.{b}       w9/w8, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5570560), "sub.{b}     w11/w10, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5636096), "sub.{b}     w13/w12, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5701632), "sub.{b}     w15/w14, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5767168), "subb.{b}      w1/w0, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5832704), "subb.{b}      w3/w2, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5898240), "subb.{b}      w5/w4, #lit5, Wd");
    mnemonic.put(Integer.valueOf(5963776), "subb.{b}      w7/w6, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6029312), "subb.{b}      w9/w8, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6094848), "subb.{b}    w11/w10, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6160384), "subb.{b}    w13/w12, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6225920), "subb.{b}    w15/w14, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6291456), "and.{b}       w1/w0, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6356992), "and.{b}       w3/w2, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6422528), "and.{b}       w5/w4, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6488064), "and.{b}       w7/w6, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6553600), "and.{b}       w9/w8, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6619136), "and.{b}     w11/w10, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6684672), "and.{b}     w13/w12, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6750208), "and.{b}     w15/w14, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6815744), "xor.{b}       w1/w0, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6881280), "xor.{b}       w3/w2, #lit5, Wd");
    mnemonic.put(Integer.valueOf(6946816), "xor.{b}       w5/w4, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7012352), "xor.{b}       w7/w6, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7077888), "xor.{b}       w9/w8, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7143424), "xor.{b}     w11/w10, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7208960), "xor.{b}     w13/w12, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7274496), "xor.{b}     w15/w14, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7340032), "ior.{b}       w1/w0, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7405568), "ior.{b}       w3/w2, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7471104), "ior.{b}       w5/w4, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7536640), "ior.{b}       w7/w6, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7602176), "ior.{b}       w9/w8, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7667712), "ior.{b}     w11/w10, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7733248), "ior.{b}     w13/w12, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7798784), "ior.{b}     w15/w14, #lit5, Wd");
    mnemonic.put(Integer.valueOf(7864320), "mov{.b}     Ws, Wd");
    mnemonic.put(Integer.valueOf(7929856), "mov{.b}     Ws, Wd");
    mnemonic.put(Integer.valueOf(7995392), "mov{.b}     Ws, Wd");
    mnemonic.put(Integer.valueOf(8060928), "mov{.b}     Ws, Wd");
    mnemonic.put(Integer.valueOf(8126464), "mov{.b}     Ws, Wd");
    mnemonic.put(Integer.valueOf(8192000), "mov{.b}     Ws, Wd");
    mnemonic.put(Integer.valueOf(8257536), "mov{.b}     Ws, Wd");
    mnemonic.put(Integer.valueOf(8323072), "mov{.b}     Ws, Wd");
    mnemonic.put(Integer.valueOf(8388608), "mov         f, Wnd           (0000..1FFF)");
    mnemonic.put(Integer.valueOf(8454144), "mov         f, Wnd           (2000..3FFF)");
    mnemonic.put(Integer.valueOf(8519680), "mov         f, Wnd           (4000..5FFF)");
    mnemonic.put(Integer.valueOf(8585216), "mov         f, Wnd           (6000..7FFF)");
    mnemonic.put(Integer.valueOf(8650752), "mov         f, Wnd           (8000..9FFF)");
    mnemonic.put(Integer.valueOf(8716288), "mov         f, Wnd           (A000..BFFF)");
    mnemonic.put(Integer.valueOf(8781824), "mov         f, Wnd           (C000..DFFF)");
    mnemonic.put(Integer.valueOf(8847360), "mov         f, Wnd           (E000..FFFF)");
    mnemonic.put(Integer.valueOf(8912896), "mov         Wns, f           (0000..1FFF)");
    mnemonic.put(Integer.valueOf(8978432), "mov         Wns, f           (2000..3FFF)");
    mnemonic.put(Integer.valueOf(9043968), "mov         Wns, f           (4000..5FFF)");
    mnemonic.put(Integer.valueOf(9109504), "mov         Wns, f           (6000..7FFF)");
    mnemonic.put(Integer.valueOf(9175040), "mov         Wns, f           (8000..9FFF)");
    mnemonic.put(Integer.valueOf(9240576), "mov         Wns, f           (A000..BFFF)");
    mnemonic.put(Integer.valueOf(9306112), "mov         Wns, f           (C000..DFFF)");
    mnemonic.put(Integer.valueOf(9371648), "mov         Wns, f           (E000..FFFF)");
    mnemonic.put(Integer.valueOf(9437184), "mov{.b}     [Ws+Slit10], Wnd (   0.. 127)");
    mnemonic.put(Integer.valueOf(9502720), "mov{.b}     [Ws+Slit10], Wnd ( 128.. 255)");
    mnemonic.put(Integer.valueOf(9568256), "mov{.b}     [Ws+Slit10], Wnd ( 256.. 383)");
    mnemonic.put(Integer.valueOf(9633792), "mov{.b}     [Ws+Slit10], Wnd ( 384.. 511)");
    mnemonic.put(Integer.valueOf(9699328), "mov{.b}     [Ws+Slit10], Wnd (-512..-385)");
    mnemonic.put(Integer.valueOf(9764864), "mov{.b}     [Ws+Slit10], Wnd (-384..-257)");
    mnemonic.put(Integer.valueOf(9830400), "mov{.b}     [Ws+Slit10], Wnd (-256..-129)");
    mnemonic.put(Integer.valueOf(9895936), "mov{.b}     [Ws+Slit10], Wnd (-128..  -1)");
    mnemonic.put(Integer.valueOf(9961472), "mov{.b}     Wns, [Wd+Slit10] (   0.. 127)");
    mnemonic.put(Integer.valueOf(10027008), "mov{.b}     Wns, [Wd+Slit10] ( 128.. 255)");
    mnemonic.put(Integer.valueOf(10092544), "mov{.b}     Wns, [Wd+Slit10] ( 256.. 383)");
    mnemonic.put(Integer.valueOf(10158080), "mov{.b}     Wns, [Wd+Slit10] ( 384.. 511)");
    mnemonic.put(Integer.valueOf(10223616), "mov{.b}     Wns, [Wd+Slit10] (-512..-385)");
    mnemonic.put(Integer.valueOf(10289152), "mov{.b}     Wns, [Wd+Slit10] (-384..-257)");
    mnemonic.put(Integer.valueOf(10354688), "mov{.b}     Wns, [Wd+Slit10] (-256..-129)");
    mnemonic.put(Integer.valueOf(10420224), "mov{.b}     Wns, [Wd+Slit10] (-128..  -1)");
    mnemonic.put(Integer.valueOf(10485760), "bset{.b}    Ws, #bit4");
    mnemonic.put(Integer.valueOf(10551296), "bclr{.b}    Ws, #bit4");
    mnemonic.put(Integer.valueOf(10616832), "btg{.b}     Ws, #bit4");
    mnemonic.put(Integer.valueOf(10682368), "btst.[cz]   Ws, #bit4");
    mnemonic.put(Integer.valueOf(10747904), "btsts.[cz]  Ws, #bit4");
    mnemonic.put(Integer.valueOf(10813440), "btst.[cz]   Ws, Wb");
    mnemonic.put(Integer.valueOf(10878976), "btss        Ws, #lit4");
    mnemonic.put(Integer.valueOf(10944512), "btsc        Ws, #lit4");
    mnemonic.put(Integer.valueOf(11010048), "bset{.b}    f,  #bit4");
    mnemonic.put(Integer.valueOf(11075584), "bclr{.b}    f,  #bit4");
    mnemonic.put(Integer.valueOf(11141120), "btg{.b}     f,  #bit4");
    mnemonic.put(Integer.valueOf(11206656), "btst{.b}    f,  #bit4");
    mnemonic.put(Integer.valueOf(11272192), "btsts{.b}   f,  #bit4");
    mnemonic.put(Integer.valueOf(11337728), "bsw.[cz]    Ws, Wb");
    mnemonic.put(Integer.valueOf(11403264), "btss        f,  #lit4");
    mnemonic.put(Integer.valueOf(11468800), "btsc        f,  #lit4");
    mnemonic.put(Integer.valueOf(11534336), "add/addc");
    mnemonic.put(Integer.valueOf(11599872), "sub/subb");
    mnemonic.put(Integer.valueOf(11665408), "and/xor");
    mnemonic.put(Integer.valueOf(11730944), "ior/mov");
    mnemonic.put(Integer.valueOf(11796480), "add.{b} f/addc.{b} f");
    mnemonic.put(Integer.valueOf(11862016), "sub.{b} f/subb.{b} f");
    mnemonic.put(Integer.valueOf(11927552), "and.{b} f/xor.{b} f");
    mnemonic.put(Integer.valueOf(11993088), "ior.{b} f/mov.{b} WREG, f");
    mnemonic.put(Integer.valueOf(12058624), "mul.us/mul.uu");
    mnemonic.put(Integer.valueOf(12124160), "mul.ss/mul.su");
    mnemonic.put(Integer.valueOf(12189696), "tblrdh/tblrdl");
    mnemonic.put(Integer.valueOf(12255232), "tblwth/tblwtl");
    mnemonic.put(Integer.valueOf(12320768), "mul.{b}     f");
    mnemonic.put(Integer.valueOf(12386304), "sub/subb");
    mnemonic.put(Integer.valueOf(12451840), "mov.d       Ws, Wnd/mov.d Wns, Wd");
    mnemonic.put(Integer.valueOf(12517376), "mov.{b}     f");
    mnemonic.put(Integer.valueOf(12582912), "mac/mpy/mpy.n/msc");
    mnemonic.put(Integer.valueOf(12648448), "mac/mpy/mpy.n/msc");
    mnemonic.put(Integer.valueOf(12713984), "mac/mpy/mpy.n/msc");
    mnemonic.put(Integer.valueOf(12779520), "clrac");
    mnemonic.put(Integer.valueOf(12845056), "mac/mpy/mpy.n/msc");
    mnemonic.put(Integer.valueOf(12910592), "mac/mpy/mpy.n/msc");
    mnemonic.put(Integer.valueOf(12976128), "mac/mpy/mpy.n/msc");
    mnemonic.put(Integer.valueOf(13041664), "movsac");
    mnemonic.put(Integer.valueOf(13107200), "sftac");
    mnemonic.put(Integer.valueOf(13172736), "add         Ws, #Slit4, Acc");
    mnemonic.put(Integer.valueOf(13238272), "lac");
    mnemonic.put(Integer.valueOf(13303808), "add/neg/sub");
    mnemonic.put(Integer.valueOf(13369344), "sac");
    mnemonic.put(Integer.valueOf(13434880), "sac.r");
    mnemonic.put(Integer.valueOf(13500416), "-");
    mnemonic.put(Integer.valueOf(13565952), "ff1l/ff1r");
    mnemonic.put(Integer.valueOf(13631488), "sl.{b}      Ws, Wd");
    mnemonic.put(Integer.valueOf(13697024), "asr/lsr");
    mnemonic.put(Integer.valueOf(13762560), "rlc/rlnc");
    mnemonic.put(Integer.valueOf(13828096), "rrc/rrnc");
    mnemonic.put(Integer.valueOf(13893632), "sl.{b}      f");
    mnemonic.put(Integer.valueOf(13959168), "asr/lsr");
    mnemonic.put(Integer.valueOf(14024704), "rlc/rlnc");
    mnemonic.put(Integer.valueOf(14090240), "rrc/rrnc");
    mnemonic.put(Integer.valueOf(14155776), "div.s/div.u");
    mnemonic.put(Integer.valueOf(14221312), "divf");
    mnemonic.put(Integer.valueOf(14286848), "-");
    mnemonic.put(Integer.valueOf(14352384), "-");
    mnemonic.put(Integer.valueOf(14417920), "-");
    mnemonic.put(Integer.valueOf(14483456), "sl          Wb, #lit4, Wnd");
    mnemonic.put(Integer.valueOf(14548992), "asr/lsr");
    mnemonic.put(Integer.valueOf(14614528), "fbcl");
    mnemonic.put(Integer.valueOf(14680064), "cp0         Ws");
    mnemonic.put(Integer.valueOf(14745600), "cp/cpb      Wb, #lit/Ws");
    mnemonic.put(Integer.valueOf(14811136), "cp0         f");
    mnemonic.put(Integer.valueOf(14876672), "cp/cpb      f");
    mnemonic.put(Integer.valueOf(14942208), "-");
    mnemonic.put(Integer.valueOf(15007744), "-");
    mnemonic.put(Integer.valueOf(15073280), "cpbgt/cpblt/cpsgt/cpslt");
    mnemonic.put(Integer.valueOf(15138816), "cpbeq/cpbne/cpseq/cpsne");
    mnemonic.put(Integer.valueOf(15204352), "inc/inc2    Ws, Wd");
    mnemonic.put(Integer.valueOf(15269888), "dec/dec2    Ws, Wd");
    mnemonic.put(Integer.valueOf(15335424), "com/neg     Ws, Wd");
    mnemonic.put(Integer.valueOf(15400960), "clr/setm    Wd");
    mnemonic.put(Integer.valueOf(15466496), "inc/inc2    f");
    mnemonic.put(Integer.valueOf(15532032), "dec/dec2    f");
    mnemonic.put(Integer.valueOf(15597568), "com/neg     f");
    mnemonic.put(Integer.valueOf(15663104), "clr/setm    f");
    mnemonic.put(Integer.valueOf(15728640), "ed/edac/mac/mpy");
    mnemonic.put(Integer.valueOf(15794176), "ed/edac/mac/mpy");
    mnemonic.put(Integer.valueOf(15859712), "ed/edac/mac/mpy");
    mnemonic.put(Integer.valueOf(15925248), "ed/edac/mac/mpy");
    mnemonic.put(Integer.valueOf(15990784), "-");
    mnemonic.put(Integer.valueOf(16056320), "-");
    mnemonic.put(Integer.valueOf(16121856), "-");
    mnemonic.put(Integer.valueOf(16187392), "-");
    mnemonic.put(Integer.valueOf(16252928), "push        f");
    mnemonic.put(Integer.valueOf(16318464), "pop         f");
    mnemonic.put(Integer.valueOf(16384000), "lnk/ulnk");
    mnemonic.put(Integer.valueOf(16449536), "se/ze");
    mnemonic.put(Integer.valueOf(16515072), "disi");
    mnemonic.put(Integer.valueOf(16580608), "daw/exch/swap");
    mnemonic.put(Integer.valueOf(16646144), "clrwdt/movpag/pwrsav/pop.s/push.s/reset");
    mnemonic.put(Integer.valueOf(16711680), "nopr");
    OpcodeToMnemonic = Collections.unmodifiableMap(mnemonic);
  }
  
  static String GetMnemonic(Integer Opcode)
  {
    int mask = 16711680;
    if (Opcode == null)
    {
      return "no_code";
    }
    String ret = null;
    
    for (int i = 0; i < 1; i++)
    {
      ret = (String)OpcodeToMnemonic.get(Integer.valueOf(Opcode.intValue() & mask));
      if (ret != null) {
        break;
      }
      
      mask <<= 1;
      mask &= 0xFFFFFF;
    }
    if (ret == null)
    {
      return "unimpl";
    }
    return ret;
  }
  









  static String StaticAnalysis(Bl2b bl2file)
  {
    List<String> outLines = new ArrayList();
    
    List<PairWithText> instanceCountList = new ArrayList();
    int[] instrCounts = new int['Ā'];
    





    if (bl2file == null)
    {
      return null;
    }
    
    Map<Integer, Integer> flashMap = bl2file.GetData();
    Map<Integer, Integer> opCountMap = new HashMap(4096);
    
    int instrCountTotal = 0;
    int effectiveInstrCountTotal = 0;
    int movdIndirectDirectCount = 0;
    for (Iterator i$ = flashMap.values().iterator(); i$.hasNext();) { int opcode = ((Integer)i$.next()).intValue();
      
      int opclass = (opcode & 0xFF0000) >> 16;
      Integer opCount = (Integer)opCountMap.get(Integer.valueOf(opcode));
      if (opCount == null)
      {
        opCount = Integer.valueOf(0);
      }
      opCountMap.put(Integer.valueOf(opcode), Integer.valueOf(opCount.intValue() + 1));
      instrCounts[opclass] += 1;
      instrCountTotal++;
      effectiveInstrCountTotal++;
      if (opclass == 190)
      {
        if (((opcode & 0x70) != 0) && ((opcode & 0xF) != 15))
        {
          movdIndirectDirectCount++;
        }
      }
      else if ((opclass == 0) || (opclass == 255))
      {
        effectiveInstrCountTotal--;
      }
    }
    
    int line = 0;
    outLines.add(line++, String.format("\n\nStatic instruction counters for .elf/.hex file contents (%1$d total instruction words, %2$d instruction words not NOP/NOPR) %3$,d bytes/%4$,d bytes:", new Object[] { Integer.valueOf(instrCountTotal), Integer.valueOf(effectiveInstrCountTotal), Integer.valueOf(instrCountTotal * 3), Integer.valueOf(effectiveInstrCountTotal * 3) }));
    

    for (int i = 0; i < 256; i++)
    {
      instanceCountList.add(new PairWithText(instrCounts[i], i, GetMnemonic(Integer.valueOf(i << 16))));
    }
    Collections.sort(instanceCountList);
    for (int i = 255; i >= 0; i--)
    {
      PairWithText p = (PairWithText)instanceCountList.get(i);
      if (first != 0L)
      {
        outLines.add(line++, String.format("\n  0x%1$02Xxxxx  %2$-48s: %3$,8d (%4$5.2f%% of total flash, %5$5.2f%% of effective code) %6$,d bytes", new Object[] { Long.valueOf(second), text, Long.valueOf(first), Double.valueOf(100.0D * first / instrCountTotal), Double.valueOf(100.0D * first / effectiveInstrCountTotal), Long.valueOf(first * 3L) }));
        if (second == 0L)
        {
          outLines.add(line++, "  NOTE: includes IVT, PSV constants, Config words, and second instruction word in goto #lit23/call #lit23 instructions");
        }
        if (second == 190L)
        {
          outLines.add(line++, String.format("\n%1$-12smov.d [indirect, not w15], Wnd                  : %2$,8d (%3$5.2f%% of total flash, %4$5.2f%% of effective code) %5$,d bytes", new Object[] { "", Integer.valueOf(movdIndirectDirectCount), Double.valueOf(100.0D * movdIndirectDirectCount / instrCountTotal), Double.valueOf(100.0D * movdIndirectDirectCount / effectiveInstrCountTotal), Integer.valueOf(movdIndirectDirectCount * 3) }));
        }
      }
    }
    
    int uniqueOpcodeCount = opCountMap.size();
    instanceCountList = new ArrayList(uniqueOpcodeCount);
    double minBitsPerDictionaryWord = Math.log10(uniqueOpcodeCount) / Math.log10(2.0D);
    int compressedSize = uniqueOpcodeCount * 3 + (instrCountTotal * (int)(minBitsPerDictionaryWord + 0.99999D) + 7) / 8;
    outLines.add(line++, String.format("\n\n\n\nCompression analysis and unique opcode counts\n---------------------------------------------\nUnique Opcodes: %1$,d\nDictionary data size: %2$,d bytes\nDictionary min bits/opcode: %3$6.3f\nTotal compressed if contiguous: %4$,d bytes (%5$,d bytes uncompressed, %6$5.2f%%)", new Object[] { Integer.valueOf(uniqueOpcodeCount), Integer.valueOf(uniqueOpcodeCount * 3), Double.valueOf(minBitsPerDictionaryWord), Integer.valueOf(compressedSize), Integer.valueOf(instrCountTotal * 3), Double.valueOf(compressedSize / (instrCountTotal * 3) * 100.0D) }));
    












    for (Iterator i$ = opCountMap.keySet().iterator(); i$.hasNext();) { int opcode = ((Integer)i$.next()).intValue();
      
      instanceCountList.add(new PairWithText(((Integer)opCountMap.get(Integer.valueOf(opcode))).intValue(), opcode, GetMnemonic(Integer.valueOf(opcode & 0xFF0000))));
    }
    Collections.sort(instanceCountList);
    for (int i = uniqueOpcodeCount - 1; i >= 0; i--)
    {
      PairWithText p = (PairWithText)instanceCountList.get(i);
      outLines.add(line++, String.format("\n  %1$06X %2$-48s: %3$,8d (%4$5.2f%% of total flash, %5$5.2f%% of effective code) %6$,d bytes compressed (%7$,d uncompressed)", new Object[] { Long.valueOf(second), text, Long.valueOf(first), Double.valueOf(100.0D * first / instrCountTotal), Double.valueOf(100.0D * first / effectiveInstrCountTotal), Long.valueOf(3L + first * (((int)(minBitsPerDictionaryWord + 0.99999D) + 7) / 8)), Long.valueOf(first * 3L) }));
    }
    

    return Multifunction.CatStringList(outLines);
  }
  




  void StartTrace(String inputElfPath, String inputBL2Path, String outputFilePath)
  {
    boolean repeatStarted = false;
    boolean repeated = false;
    int lastPC = 0;
    


    boolean lastWasSkip = false;
    boolean lastWasMultiCycle = false;
    boolean lastWas2InstrWord = false;
    boolean lastWasRetfie = false;
    boolean lastWasReturn = false;
    boolean lastWasRepeat = false;
    boolean lastWasObviousBranch = false;
    boolean lastWasCall = false;
    boolean lastWasGotoBra = false;
    boolean lastWasConditionalBra = false;
    boolean lastConditionalBraTaken = false;
    int lastBranchSource = 0;
    int lastBranchTarget = 0;
    
    outputPath = Multifunction.FixSlashes(Multifunction.TrimQuotes(outputFilePath));
    

    try
    {
      byte[] wildAddr = new byte[4];
      udp = new DatagramSocket(42460, InetAddress.getByAddress(wildAddr));
      udp.setReceiveBufferSize(262144);
      udp.setSoTimeout(100);
      udp.setBroadcast(true);
    }
    catch (SocketException ex)
    {
      System.err.printf("Error opening and configuring UDP RX socket: %s\n", new Object[] { ex.getMessage() });
      System.exit(-1);
    }
    catch (UnknownHostException ex)
    {
      System.err.printf("Error opening wildcard address: %s\n", new Object[] { ex.getMessage() });
      System.exit(-1);
    }
    

    if (elf.startObjDump(new String[] { "C:/Program Files (x86)/Microchip/xc16/vLatestPublic/bin/xc16-objdump", "--syms", "--section-headers", inputElfPath }) < 0)
    {
      System.err.printf("Can't get symbols from '%1$s'", new Object[] { inputElfPath });
    }
    if (elf.startObjDump(new String[] { "C:/Program Files (x86)/Microchip/xc16/vLatestPublic/bin/xc16-objdump", "--disassemble", inputElfPath }) < 0)
    {
      System.err.printf("Can't get disassembly contents from '%1$s'", new Object[] { inputElfPath });
    }
    elf.parseAllObjOutput(0);
    elf.parseAllObjOutput(0);
    
    for (Section sec : elf.romSectionMapByName.values())
    {
      if (symbols == null)
      {
        sec.LoadSymbols(elf.symbols);
      }
    }
    bl2Bytes = Multifunction.ReadFileBinary(inputBL2Path);
    code = new Bl2b(bl2Bytes, null);
    instr = code.GetData();
    for (Iterator i$ = instr.values().iterator(); i$.hasNext();) { int instruction = ((Integer)i$.next()).intValue();
      
      staticCounts[(instruction >> 16)] += 1;
    }
    staticCount = instr.size();
    

    file = new File(String.format("%1$s", new Object[] { outputPath }));
    
    File folder = file.getParentFile();
    File[] existingFiles = folder.listFiles(new FilenameFilter()
    {

      public boolean accept(File dir, String name)
      {
        return name.matches(String.format("%1$s\\.[0-9]{4}\\.txt", new Object[] { file.getName() }));
      }
    });
    if (existingFiles != null)
    {
      for (File f : existingFiles)
      {
        f.delete();
      }
    }
    

    file = new File(String.format("%1$s.%2$04d.txt", new Object[] { outputPath, Integer.valueOf(fileSequence++) }));
    logLinesThisFile = 0;
    try
    {
      fileOut = new FileWriter(file);
    }
    catch (IOException ex)
    {
      System.err.printf("Could not create %s: %s\n", new Object[] { file.getName(), ex.getMessage() });
      System.exit(-2);
    }
    outLines = new ArrayList(8192);
    
    long startTime = System.currentTimeMillis();
    long timeout = idleTimeout;
    for (;;)
    {
      if (logLinesThisFile >= 2097152)
      {
        FlushLogData(true);
      }
      
      try
      {
        udp.receive(pkt);
        if (fileStartTime == 0L)
        {
          fileStartTime = System.nanoTime();
        }
      }
      catch (IOException ex)
      {
        if (System.currentTimeMillis() - startTime > timeout)
        {
          System.out.printf("No trace data received for %1$d seconds. Terminating.\n", new Object[] { Long.valueOf(timeout / 1000L) });
          fileStartTime += timeout * 1000000L;
          FlushLogData(false);
          return;
        } }
      continue;
      
      startTime = System.currentTimeMillis();
      timeout = 2000L;
      
      pktLen = pkt.getLength();
      if (pktLen == 4)
      {
        if (logLinesThisFile != 0)
        {
          FlushLogData(true);
        }
        
      }
      else
      {
        int instrInPkt = 0;
        for (int i = 0; i < pktLen; i++)
        {
          if ((pktData[i] & 0xFFFFFF80) == 0)
          {
            instrInPkt += pktData[i];
          }
          else
          {
            instrInPkt++;
            i += 2;
          }
        }
        netInstructions += instrInPkt;
        logLinesThisFile += instrInPkt;
        

        int PCindex = 0;
        int PC = lastPC;
        for (int i = 0; i < pktLen; i++)
        {
          byte b = pktData[i];
          if ((b & 0xFFFFFF80) == 0) {
            for (;;) {
              b = (byte)(b - 1); if (b == 0)
                break;
              PC += 2;
              PCs[(PCindex++)] = PC;
            }
          }
          

          PC = (b & 0x7F) << 16 | (pktData[(i + 1)] & 0xFF) << 8 | pktData[(i + 2)] & 0xFF;
          PCs[(PCindex++)] = PC;
          i += 2;
        }
        

        for (PCindex = 0; PCindex < instrInPkt; PCindex++)
        {
          PC = PCs[PCindex];
          boolean branched = lastPC + 2 != PC;
          
          boolean skipped = false;
          repeated = false;
          if (lastWasSkip)
          {
            if (lastPC + 4 == PC)
            {
              skips += 1;
              skipped = true;
              branched = false;
            }
            else
            {
              skipsNotTaken += 1;
            }
          }
          else if ((lastWasRepeat) || (repeatStarted))
          {
            if (lastPC == PC)
            {
              branched = false;
              repeated = true;
              repeats += 1;
            }
            else
            {
              repeatStarted = lastWasRepeat;
            }
          }
          else if (lastWasConditionalBra)
          {
            if (PC == lastBranchTarget)
            {
              branched = true;
              lastConditionalBraTaken = true;
              conditionalBrachesTaken += 1;
              if (lastBranchTarget >= lastBranchSource)
              {
                conditionalBranchesTakenForward += 1;
              }
              else
              {
                conditionalBranchesTakenBackward += 1;
              }
            }
            else
            {
              lastConditionalBraTaken = false;
              conditionalBrachesNotTaken += 1;
              if (lastBranchTarget >= lastBranchSource)
              {
                conditionalBranchesNotTakenForward += 1;
              }
              else
              {
                conditionalBranchesNotTakenBackward += 1;
              }
            }
          }
          
          if (branched)
          {
            netBranchesTaken += 1;
          }
          
          if (!instr.containsKey(Integer.valueOf(PC)))
          {
            String lineBasic = String.format("\n%1$06X  ??????  (unknown)", new Object[] { Integer.valueOf(PC) });
            outLines.add(lineBasic);
            if ((branched) || (skipped) || (repeated))
            {
              int lenSoFar = lineBasic.length();
              int padLen = 54 - lenSoFar;
              if (padLen < 2)
              {
                padLen = 2;
              }
              outLines.add(String.format("%1$" + String.valueOf(padLen) + "s%2$s", new Object[] { "", repeated ? "(repeated)" : skipped ? "(skipped1)" : branched ? "(branched)" : "" }));
            }
            

            lastPC = PC;
            lastWasSkip = false;
            lastWasRepeat = false;
            lastWasConditionalBra = false;
          }
          else
          {
            int opcode = ((Integer)instr.get(Integer.valueOf(PC))).intValue();
            int ophigh = opcode >> 16;
            if ((!lastWas2InstrWord) || (ophigh != 0))
            {


              String label = "";
              Section sec = (Section)elf.romSectionMapByAddr.get(Integer.valueOf(PC));
              if ((sec != null) && (symbolsByAddr.containsKey(Integer.valueOf(PC))))
              {
                label = symbolsByAddr.get(Integer.valueOf(PC))).name;
              }
              
              instanceCounts[(opcode >> 16)] += 1;
              String mnemonic = (String)elf.disassemblyMap.get(Integer.valueOf(PC));
              if (mnemonic == null)
              {
                mnemonic = GetMnemonic(Integer.valueOf(opcode));
              }
              

              String lineBasic = String.format("\n%1$06X  %2$06X  %3$s", new Object[] { Integer.valueOf(PC), Integer.valueOf(opcode), mnemonic });
              outLines.add(lineBasic);
              if ((branched) || (skipped) || (repeated) || (!label.isEmpty()))
              {
                int lenSoFar = lineBasic.length();
                int padLen = 54 - lenSoFar;
                if (padLen < 2)
                {
                  padLen = 2;
                }
                if ((branched) || (skipped) || (repeated))
                {
                  outLines.add(String.format("%1$" + String.valueOf(padLen) + "s%2$s", new Object[] { "", repeated ? "(repeated)" : skipped ? "(skipped1)" : branched ? "(branched)" : "" }));
                  

                  padLen = 0;
                }
                else
                {
                  padLen += 10;
                }
                if (!label.isEmpty())
                {
                  outLines.add(String.format("%1$" + (padLen != 0 ? String.valueOf(padLen) : "") + "s\t\t%2$s", new Object[] { "", label }));
                }
              }
              


              lastWasSkip = (ophigh == 166) || (ophigh == 167) || (ophigh == 174) || (ophigh == 175) || ((opcode & 0xFF83E0) == 15073280) || ((opcode & 0xFF83E0) == 15106048) || ((opcode & 0xFF83E0) == 15138816) || ((opcode & 0xFF83E0) == 15171584);
              









              lastWasMultiCycle = (ophigh == 1) || (ophigh == 2) || (ophigh == 4) || (ophigh == 5) || (ophigh == 6) || (ophigh == 7) || (ophigh == 186) || (ophigh == 187) || (ophigh == 190);
              















              lastWas2InstrWord = (ophigh == 2) || (ophigh == 4) || (ophigh == 190);
              



              lastWasRetfie = (ophigh == 6) && ((opcode & 0xC000) == 16384);
              lastWasReturn = (ophigh == 5) || ((ophigh == 6) && ((opcode & 0xC000) != 16384));
              

              lastWasObviousBranch = (ophigh == 1) || (ophigh == 2) || (ophigh == 4) || (ophigh == 5) || (ophigh == 6) || (ophigh == 7) || (ophigh == 55);
              











              lastWasCall = ((opcode & 0xFFFFF0) == 73728) || ((opcode & 0xFFFFF0) == 66048) || ((opcode & 0xFF87F0) == 98304) || ((opcode & 0xFFFFF0) == 65536) || (ophigh == 2) || (ophigh == 7);
              









              lastWasGotoBra = ((ophigh == 1) && ((opcode & 0xFFFFF0) != 73728) && ((opcode & 0xFFFFF0) != 66048) && ((opcode & 0xFF87F0) != 98304) && ((opcode & 0xFFFFF0) != 65536)) || (ophigh == 4) || (ophigh == 55);
              












              lastWasConditionalBra = (ophigh == 12) || (ophigh == 13) || (ophigh == 14) || (ophigh == 15) || (ophigh == 48) || (ophigh == 49) || (ophigh == 50) || (ophigh == 51) || (ophigh == 52) || (ophigh == 53) || (ophigh == 54) || (ophigh == 56) || (ophigh == 57) || (ophigh == 58) || (ophigh == 59) || (ophigh == 60) || (ophigh == 61) || (ophigh == 62) || (((ophigh == 230) || (ophigh == 231)) && (!lastWasSkip));
              





































              lastWasRepeat = ophigh == 9;
              if (lastWasReturn)
              {
                lastBranchSource = PC;
                returns += 1;
              }
              if (lastWasRetfie)
              {
                lastBranchSource = PC;
                retfies += 1;
              }
              if (lastWas2InstrWord)
              {
                total2Cycles += 1;
              }
              if (lastWasObviousBranch)
              {
                lastBranchSource = PC;
                obviousBranches += 1;
              }
              if (lastWasCall)
              {
                lastBranchSource = PC;
                calls += 1;
              }
              if (lastWasGotoBra)
              {
                lastBranchSource = PC;
                gotoBras += 1;
              }
              if (lastWasConditionalBra)
              {
                lastBranchSource = PC;
                conditionalBraches += 1;
                
                int offset;
                if ((ophigh == 230) || (ophigh == 231))
                {
                  int offset = (opcode & 0x3F0) >> 4;
                  if ((opcode & 0x3F) > 31)
                  {
                    offset |= 0xFFFFFFC0;
                  }
                }
                else
                {
                  offset = opcode & 0xFFFF;
                  if ((opcode & 0xFFFF) > 32767)
                  {
                    offset |= 0xFFFF0000;
                  }
                }
                lastBranchTarget = PC + 2 + 2 * offset;
              }
              lastPC = PC;
            }
          } }
        traceBytesProcessed += pktLen;
      }
    }
  }
  
  boolean FlushLogData(boolean startAnother) {
    String outLog = null;
    int line = 0;
    List<PairWithText> instanceCountList = new ArrayList(256);
    
    if (outLines != null)
    {
      instrPerSecond = ((float)(logLinesThisFile / ((System.nanoTime() - fileStartTime) / 1.0E9D)));
      netInstructions -= total2Cycles;
      outLines.add(line++, String.format("Total instructions logged: %1$,10d (%2$,7d instructions/second)", new Object[] { Integer.valueOf(netInstructions), Integer.valueOf((int)instrPerSecond) }));
      outLines.add(line++, String.format("\nTwo instr word instrs:     %1$,10d", new Object[] { Integer.valueOf(total2Cycles) }));
      outLines.add(line++, String.format("\nTotal times PC branched:   %1$,10d (%2$5.2f%% of total instrs)", new Object[] { Integer.valueOf(netBranchesTaken), Double.valueOf(100.0D * netBranchesTaken / netInstructions) }));
      outLines.add(line++, String.format("\n  Obvious branches:        %1$,10d (%2$5.2f%% of total PC branches); call, goto, rcall, bra, retfie, return, retlw", new Object[] { Integer.valueOf(obviousBranches), Double.valueOf(100.0D * obviousBranches / netBranchesTaken) }));
      outLines.add(line++, String.format("\n    call:                  %1$,10d (%2$5.2f%% of obvious branches); includes rcall", new Object[] { Integer.valueOf(calls), Double.valueOf(100.0D * calls / obviousBranches) }));
      outLines.add(line++, String.format("\n    return:                %1$,10d (%2$5.2f%% of obvious branches); includes retlw", new Object[] { Integer.valueOf(returns), Double.valueOf(100.0D * returns / obviousBranches) }));
      outLines.add(line++, String.format("\n    retfie:                %1$,10d (%2$5.2f%% of obvious branches)", new Object[] { Integer.valueOf(retfies), Double.valueOf(100.0D * retfies / obviousBranches) }));
      outLines.add(line++, String.format("\n    branch always:         %1$,10d (%2$5.2f%% of obvious branches); includes goto, goto.l, bra", new Object[] { Integer.valueOf(gotoBras), Double.valueOf(100.0D * gotoBras / obviousBranches) }));
      outLines.add(line++, String.format("\n  Cond branches taken:     %1$,10d (%2$5.2f%% of total PC branches; %3$5.2f%% of conditional branches total)", new Object[] { Integer.valueOf(conditionalBrachesTaken), Double.valueOf(100.0D * conditionalBrachesTaken / netBranchesTaken), Double.valueOf(100.0D * conditionalBrachesTaken / (conditionalBrachesTaken + conditionalBrachesNotTaken)) }));
      outLines.add(line++, String.format("\n    Taken forwards:        %1$,10d (%2$5.2f%% of conditionals taken; %3$5.2f%% of conditionals pointing forwards)", new Object[] { Integer.valueOf(conditionalBranchesTakenForward), Double.valueOf(100.0D * conditionalBranchesTakenForward / conditionalBrachesTaken), Double.valueOf(100.0D * conditionalBranchesTakenForward / (conditionalBranchesTakenForward + conditionalBranchesNotTakenForward)) }));
      outLines.add(line++, String.format("\n    Taken backwards:       %1$,10d (%2$5.2f%% of conditionals taken; %3$5.2f%% of conditionals pointing backwards)", new Object[] { Integer.valueOf(conditionalBranchesTakenBackward), Double.valueOf(100.0D * conditionalBranchesTakenBackward / conditionalBrachesTaken), Double.valueOf(100.0D * conditionalBranchesTakenBackward / (conditionalBranchesTakenBackward + conditionalBranchesNotTakenBackward)) }));
      outLines.add(line++, String.format("\n  Surprise branches:       %1$,10d (%2$5.2f%% of total PC branches); presumably interrupts", new Object[] { Integer.valueOf(netBranchesTaken - obviousBranches - conditionalBrachesTaken), Double.valueOf(100.0D * (netBranchesTaken - obviousBranches - conditionalBrachesTaken) / netBranchesTaken) }));
      outLines.add(line++, String.format("\nBranches not taken:        %1$,10d (%2$5.2f%% of conditional branches total)", new Object[] { Integer.valueOf(conditionalBrachesNotTaken), Double.valueOf(100.0D * conditionalBrachesNotTaken / (conditionalBrachesTaken + conditionalBrachesNotTaken)) }));
      outLines.add(line++, String.format("\n    Not taken forwards:    %1$,10d (%2$5.2f%% of conditionals not taken; %3$5.2f%% of conditionals pointing forwards)", new Object[] { Integer.valueOf(conditionalBranchesNotTakenForward), Double.valueOf(100.0D * conditionalBranchesNotTakenForward / conditionalBrachesNotTaken), Double.valueOf(100.0D * conditionalBranchesNotTakenForward / (conditionalBranchesTakenForward + conditionalBranchesNotTakenForward)) }));
      outLines.add(line++, String.format("\n    Not taken backwards:   %1$,10d (%2$5.2f%% of conditionals not taken; %3$5.2f%% of conditionals pointing backwards)", new Object[] { Integer.valueOf(conditionalBranchesNotTakenBackward), Double.valueOf(100.0D * conditionalBranchesNotTakenBackward / conditionalBrachesNotTaken), Double.valueOf(100.0D * conditionalBranchesNotTakenBackward / (conditionalBranchesTakenForward + conditionalBranchesNotTakenBackward)) }));
      outLines.add(line++, String.format("\nSkipped instructions:      %1$,10d (%2$5.2f%% of total instrs; %3$5.2f%% of skip instrs)", new Object[] { Integer.valueOf(skips), Double.valueOf(100.0D * skips / netInstructions), Double.valueOf(100.0D * skips / (skips + skipsNotTaken)) }));
      outLines.add(line++, String.format("\nNot skipped instructions:  %1$,10d (%2$5.2f%% of total instrs; %3$5.2f%% of skip instrs)", new Object[] { Integer.valueOf(skipsNotTaken), Double.valueOf(100.0D * skipsNotTaken / netInstructions), Double.valueOf(100.0D * skipsNotTaken / (skips + skipsNotTaken)) }));
      outLines.add(line++, String.format("\nRepeated instructions:     %1$,10d (%2$5.2f%% of total instrs)", new Object[] { Integer.valueOf(repeats), Double.valueOf(100.0D * repeats / netInstructions) }));
      
      outLines.add(line++, "\n\nExecution counters:");
      
      for (int i = 0; i < 256; i++)
      {
        instanceCountList.add(new PairWithText(instanceCounts[i], i, GetMnemonic(Integer.valueOf(i << 16))));
      }
      Collections.sort(instanceCountList);
      for (int i = 255; i >= 0; i--)
      {
        PairWithText p = (PairWithText)instanceCountList.get(i);
        outLines.add(line++, String.format("\n  0x%1$02Xxxxx  %2$-32s: %3$,8d (%4$5.2f%% of total instrs)", new Object[] { Long.valueOf(second), text, Long.valueOf(first), Double.valueOf(100.0D * first / netInstructions) }));
      }
      instanceCountList.clear();
      
      outLines.add(line++, String.format("\n\nStatic instruction counters for .elf/.hex file contents (%1$d total instruction words):", new Object[] { Integer.valueOf(staticCount) }));
      

      for (int i = 0; i < 256; i++)
      {
        instanceCountList.add(new PairWithText(staticCounts[i], i, GetMnemonic(Integer.valueOf(i << 16))));
      }
      Collections.sort(instanceCountList);
      for (int i = 255; i >= 0; i--)
      {
        PairWithText p = (PairWithText)instanceCountList.get(i);
        outLines.add(line++, String.format("\n  0x%1$02Xxxxx  %2$-32s: %3$,8d (%4$5.2f%% of total instrs)", new Object[] { Long.valueOf(second), text, Long.valueOf(first), Double.valueOf(100.0D * first / staticCount) }));
        if (second == 0L)
        {
          outLines.add(line++, "  NOTE: includes IVT, PSV constants, Config words, and second instruction word in goto #lit23/call #lit23 instructions");
        }
      }
      
      outLines.add(line++, "\n\nPC      Opcode  Instruction                           Notes    \t\tLabel");
      
      outLog = Multifunction.CatStringList(outLines);
      outLines = new ArrayList(8192);
      instanceCountList = null;
    }
    
    try
    {
      if (outLog != null)
      {
        fileOut.write(outLog);
      }
      fileOut.close();
    }
    catch (IOException ex)
    {
      System.err.printf("Could not write to %s: %s\n", new Object[] { file.getName(), ex.getMessage() });
      return false;
    }
    

    if (startAnother)
    {
      file = new File(String.format("%1$s.%2$04d.txt", new Object[] { outputPath, Integer.valueOf(fileSequence++) }));
      if (file.exists())
      {
        file.delete();
      }
      try
      {
        fileOut = new FileWriter(file);
      }
      catch (IOException ex)
      {
        System.err.printf("Could not create %s: %s\n", new Object[] { file.getName(), ex.getMessage() });
        return false;
      }
      logLinesThisFile = 0;
      fileStartTime = 0L;
    }
    
    return true;
  }
  
  public PCTraceLogger() {}
}
