%{
#define YYSTYPE unsigned int
%}

%token  OPRD_REG OPRD_IMM;
%token  INST_BL INST_ADD_LIKE INST_LDM_LIKE;
%type   inst;

%%

inst:
      branch_inst
    | data_inst
    | load_inst
    | load_mult_inst 
    ;

branch_inst:
      INST_BL { BEGIN(cond) } COND { BEGIN(oprd) } OPRD_IMM;
        {   $$ = (0x0b000000 | $3 | $5); }
    ;

data_inst:
      INST_MOV_LIKE { BEGIN(cond) } COND { BEGIN(ccup) } CCUP { BEGIN(oprd) }
        dest_reg ',' shifter_operand
        {   $$ = ($1 | $3 | $5 | $7 | $9); }
    ;

load_inst:
      INST_LDR { BEGIN(cond) } COND { BEGIN(oprd) } dest_reg ',' load_am
        {   $$ = ($1 | $3 | $5 | $7);   }
    ; 

load_mult_inst:
      INST_LDM_LIKE { BEGIN(cond) } COND { BEGIN(lmam) } LMAM { BEGIN(oprd) }
        src_reg maybe_bang ',' '{' reg_list '}'
        {   $$ = ($1 | $3 | $5 | $7 | $8 | $11) } 
    ;

maybe_bang:
    /* empty */     { $$ = 0; }
    | '!'           { $$ = (0x1 << 21); }
    ;

reg_list:
    /* empty */             { $$ = 0;                   }
    | OPRD_REG              { $$ = $1;                  }
    | reg_list ',' OPRD_REG { $$ = ($1 | (1 << $3));    }
    ;

dest_reg:
      OPRD_REG      { $$ = ($1 << 12); }
    ;

shifter_operand:
      '#' OPRD_IMM
        {
            /* TODO: support expressions here */

            if ($2 > 0xff)
                abort();    /* FIXME: try to rotate to fit it in */
            $$ = ((0x1 << 25) | $2);
        }
    | OPRD_REG      { $$ = $1; }
    ;

load_am:
      expr
        {
            /* assumes PC-relative addressing */
        }
    ;

%%

