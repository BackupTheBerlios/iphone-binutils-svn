
%%

<INITIAL>{
    "add"     yylval.nval = (0x5 << 21); return INST_ADD_LIKE;
    "bl"                                 return INST_BL;
    "ldm"     yylval.nval = (0x1 << 20); return INST_LDM_LIKE;
    "ldr"     yylval.nval = 0x04100000;  return INST_LDR_LIKE;
    "mov"     yylval.nval = (0xd << 21); return INST_MOV_LIKE;
    "stm"     yylval.nval = 0;           return INST_LDM_LIKE;
    "str"     yylval.nval = 0x04000000;  return INST_LDR_LIKE;
    "sub"     yylval.nval = (0x2 << 21); return INST_ADD_LIKE;
}

<cond>{
    "eq"        yylval.nval = 0x00000000; return COND;
    "ne"        yylval.nval = 0x10000000; return COND;
    "cs"        yylval.nval = 0x20000000; return COND;
    "hs"        yylval.nval = 0x20000000; return COND_CS;
    "cc"        yylval.nval = 0x30000000; return COND_CC;
    "lo"        yylval.nval = 0x30000000; return COND_CC;
    "mi"        yylval.nval = 0x40000000; return COND_MI;
    "pl"        yylval.nval = 0x50000000; return COND_PL;
    "vs"        yylval.nval = 0x60000000; return COND_VS;
    "vc"        yylval.nval = 0x70000000; return COND_VC;
    "hi"        yylval.nval = 0x80000000; return COND_HI;
    "ls"        yylval.nval = 0x90000000; return COND_LS;
    "ge"        yylval.nval = 0xa0000000; return COND_GE;
    "lt"        yylval.nval = 0xb0000000; return COND_LT;
    "gt"        yylval.nval = 0xc0000000; return COND_GT;
    "le"        yylval.nval = 0xd0000000; return COND_LE;
    "al"        yylval.nval = 0xe0000000; return COND_AL;
    .           yyless(0); yylval.nval = 0xe0000000; return COND_AL;
}

<ccup>{
    "s"                     yylval = (1 << 20); return CCUP;
    .           yyless(0);  yylval = 0;         return CCUP;
}

<lmam>{
    "ia"        return LMAM_IA;
    "ib"        return LMAM_IB;
    "da"        return LMAM_DA;
    "db"        return LMAM_DB;
    "fd"        return LMAM_FD;
    "fa"        return LMAM_FA;
    "ed"        return LMAM_ED;
    "ea"        return LMAM_EA;
}

<oprd>{
    "r"[0-9]+   {
                    yylval.nval = (unsigned int)strtol(yytext + 1,
                        (char **)NULL, 10);
                    return OPRD_REG;
                }
    "a"[0-9]+   {
                    yylval.nval = (unsigned int)strtol(yytext + 1,
                        (char **)NULL, 10) - 1;
                    return OPRD_REG;
                }
    "wr"        yylval.nval = 7;  return OPRD_REG;
    "sb"        yylval.nval = 9;  return OPRD_REG;
    "sl"        yylval.nval = 10; return OPRD_REG;
    "fp"        yylval.nval = 11; return OPRD_REG;
    "ip"        yylval.nval = 12; return OPRD_REG;
    "sp"        yylval.nval = 13; return OPRD_REG;
    "lr"        yylval.nval = 14; return OPRD_REG;
    "pc"        yylval.nval = 15; return OPRD_REG;
    [-+]?[0-9]+ {
                    yylval.ival = (int)strtol(yytext, (char **)NULL, 10);
                    return OPRD_IMM;
                }
}

<*>"#"          return '#';
<*>"["          return '[';
<*>"]"          return ']';
<*>"{"          return '{';
<*>"}"          return '}';
<*>","          return ',';
<*>"!"          return '!';
<*>"+"          return '+';
<*>"-"          return '-';

<*>[ \t]        /* eat whitespace */

%%

