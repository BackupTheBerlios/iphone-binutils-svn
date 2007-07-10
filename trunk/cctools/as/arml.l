
%%

<INITIAL>{
    "add"     yylval = (0x5 << 21); return INST_ADD_LIKE;
    "bl"                            return INST_BL;
    "ldm"     yylval = (0x1 << 20); return INST_LDM_LIKE;
    "ldr"                           return INST_LDR;
    "mov"     yylval = (0xd << 21); return INST_MOV_LIKE;
    "stm"     yylval = 0;           return INST_LDM_LIKE;
    "sub"     yylval = (0x2 << 21); return INST_ADD_LIKE;
}

<cond>{
    "eq"        yylval = 0x00000000; return COND;
    "ne"        yylval = 0x10000000; return COND;
    "cs"        yylval = 0x20000000; return COND;
    "hs"        yylval = 0x20000000; return COND_CS;
    "cc"        yylval = 0x30000000; return COND_CC;
    "lo"        yylval = 0x30000000; return COND_CC;
    "mi"        yylval = 0x40000000; return COND_MI;
    "pl"        yylval = 0x50000000; return COND_PL;
    "vs"        yylval = 0x60000000; return COND_VS;
    "vc"        yylval = 0x70000000; return COND_VC;
    "hi"        yylval = 0x80000000; return COND_HI;
    "ls"        yylval = 0x90000000; return COND_LS;
    "ge"        yylval = 0xa0000000; return COND_GE;
    "lt"        yylval = 0xb0000000; return COND_LT;
    "gt"        yylval = 0xc0000000; return COND_GT;
    "le"        yylval = 0xd0000000; return COND_LE;
    "al"        yylval = 0xe0000000; return COND_AL;
    .           yyless(0); yylval = 0xe0000000; return COND_AL;
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
                    yylval = (int)strtol(yytext + 1, (char **)NULL, 10);
                    return OPRD_REG;
                }
    "a"[0-9]+   {
                    yylval = (int)strtol(yytext + 1, (char **)NULL, 10) - 1;
                    return OPRD_REG;
                }
    "wr"        yylval = 7;  return OPRD_REG;
    "sb"        yylval = 9;  return OPRD_REG;
    "sl"        yylval = 10; return OPRD_REG;
    "fp"        yylval = 11; return OPRD_REG;
    "ip"        yylval = 12; return OPRD_REG;
    "sp"        yylval = 13; return OPRD_REG;
    "lr"        yylval = 14; return OPRD_REG;
    "pc"        yylval = 15; return OPRD_REG;
    [0-9]+      {
                    yylval = (unsigned int)strtol(yytext, (char **)NULL, 10);
                    return OPRD_IMM;
                }
}

<*>"#"          return '#';
<*>"["          return '[';
<*>"]"          return ']';
<*>","          return ',';
<*>"!"          return '!';

<*>[ \t]        /* eat whitespace */

%%

