/*
 * small complier program implemented in C
 *
 * The program has been tested on Visual Studio 2010
 *
 * 使用方法：
 * 运行后输入small源程序文件名
 * 回答是否输出虚拟机代码
 * 回答是否输出符号表
 * fcode.txt输出虚拟机代码
 * foutput.txt输出源文件、出错示意（如有错）和各行对应的生成代码首地址（如无错）
 * fresult.txt输出运行结果
 * ftable.txt输出符号表
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define bool int
#define true 1
#define false 0

#define norw 29       /* 保留字个数 */
#define txmax 100     /* 符号表容量 */
#define nmax 14       /* 数字的最大位数 */
#define al 10         /* 标识符的最大长度 */
#define maxerr 30     /* 允许的最多错误数 */
#define amax 2048     /* 地址上界*/
#define levmax 3      /* 最大允许过程嵌套声明层数*/
#define cxmax 200     /* 最多的虚拟机代码数 */
#define stacksize 500 /* 运行时数据栈元素最多为500个 */
#define breakmax 10

/* 符号 */
enum symbol
{
    nul,         ident,     number,     boolean,   plus,      minus,
    times,       slash,     mod,        xor,       and,       or,
    not,         note,      lnote,      rnote,     inc,       dec,
    eql,         neq,       lss,        leq,       gtr,       geq,
    lparen,      rparen,    comma,      semicolon, period,    becomes,
    constder,    startsym,  lbra,       rbra,
    beginsym,    breaksym,  callsym,    casesym,   constsym,  continuesym,
    defaultsym,  dosym,     elsesym,    endsym,    exitsym,   falsesym,
    finishsym,   forsym,    ifsym,      oddsym,    procsym,   readsym,
    repeatsym,   switchsym, thensym,    tosym,     truesym,   untilsym,
    whilesym,    writesym,  intsym,     boolsym,   colon,
};

#define symnum 63

/* 符号表中的类型 */
enum object
{
    constant,
    variable,
    proc,
};

/* 虚拟机代码指令 */
enum fct
{
    lit,     opr,     lod,
    sto,     cal,     ini,
    jmp,     jpc,
};
#define fctnum 8

/* 虚拟机代码结构 */
struct instruction
{
    enum fct f; /* 虚拟机代码指令 */
    int l;      /* 引用层与声明层的层次差 */
    int a;      /* 根据f的不同而不同 */
};

char cnstnm[al+1];
char strtmp[al+1];
int lastcase;           /*上一个case生成的条件跳转指令地址（待回填的指令）*/
int iniexit[breakmax];
int inicont[breakmax];
int inibreak[breakmax];/*初始存在的break对应无条件指令代码地址*/
bool listswitch ;   /* 显示虚拟机代码与否 */
bool tableswitch ;  /* 显示符号表与否 */
char ch;            /* 存放当前读取的字符，getch 使用 */
enum symbol sym;    /* 当前的符号 */
char id[al+1];      /* 当前ident，多出的一个字节用于存放0 */
int num;            /* 当前number */
int cur;           /*当前bool值*/
int cc, ll;         /* getch使用的计数器，cc表示当前字符(ch)的位置 */
int cx;             /* 虚拟机代码指针, 取值范围[0, cxmax-1]*/
char line[81];      /* 读取行缓冲区 */
char a[al+1];       /* 临时符号，多出的一个字节用于存放0 */
struct instruction code[cxmax]; /* 存放虚拟机代码的数组 */
char word[norw][al];        /* 保留字 */
enum symbol wsym[norw];     /* 保留字对应的符号值 */
enum symbol ssym[256];      /* 单字符的符号值 */
char mnemonic[fctnum][5];   /* 虚拟机代码指令名称 */
bool declbegsys[symnum];    /* 表示声明开始的符号集合 */
bool statbegsys[symnum];    /* 表示语句开始的符号集合 */
bool facbegsys[symnum];     /* 表示因子开始的符号集合 */

/* 符号表结构 */
struct tablestruct
{
    char name[al];	    /* 名字 */
    enum object kind;	/* 类型：const，var或proc */
    int type;           /*数据类型，1为int，2为boolean，仅proc不使用*/
    int val;            /* 数值，仅const使用 */
    bool bl;            /*bool型数值，仅const使用*/
    int level;          /* 所处层，仅const不使用 */
    int adr;            /* 地址，仅const不使用 */
    int size;           /* 需要分配的数据区空间, 仅proc使用 */
};

struct tablestruct table[txmax]; /* 符号表 */

FILE* fin;      /* 输入源文件 */
FILE* ftable;	/* 输出符号表 */
FILE* fcode;    /* 输出虚拟机代码 */
FILE* foutput;  /* 输出文件及出错示意（如有错）、各行对应的生成代码首地址（如无错） */
FILE* fresult;  /* 输出执行结果 */
char fname[81];
int err;        /* 错误计数器 */

int isbool;
void error(int n);
void getsym();
void getch();
void init();
void gen(enum fct x, int y, int z);
void test(bool* s1, bool* s2, int n);
int inset(int e, bool* s);
int addset(bool* sr, bool* s1, bool* s2, int n);
int subset(bool* sr, bool* s1, bool* s2, int n);
int mulset(bool* sr, bool* s1, bool* s2, int n);
void block(int lev, int tx, bool* fsys);
void interpret();
void factor(bool* fsys, int* ptx, int lev);
void termnot(bool* fsys, int* ptx, int lev);
void termmul(bool* fsys, int* ptx, int lev);
void termadd(bool* fsys, int* ptx, int lev);
void termand(bool* fsys, int* ptx, int lev);
void termxor(bool* fsys, int* ptx, int lev);
void condition(bool* fsys, int* ptx, int lev);
void expression(bool* fsys, int* ptx, int lev);
void statement(bool* fsys, int* ptx, int lev, int *fbreak, int* fcont);
void listcode(int cx0);
void listall();
void vardeclaration(int* ptx, int lev, int* pdx,int type);
void constdeclaration(int* ptx, int lev, int* pdx, int type);
int position(char* idt, int tx);
void enter(enum object k, int* ptx, int lev, int* pdx,int type);
int base(int l, int* s, int b);


/* 主程序开始 */
int main()
{
    bool nxtlev[symnum];

    printf("Input small file?   ");
    scanf("%s", fname);		/* 输入文件名 */

    if ((fin = fopen(fname, "r")) == NULL)
    {
        printf("Can't open the input file!\n");
        exit(1);
    }

    ch = fgetc(fin);
    if (ch == EOF)
    {
        printf("The input file is empty!\n");
        fclose(fin);
        exit(1);
    }
    rewind(fin);

    if ((foutput = fopen("foutput.txt", "w")) == NULL)
    {
        printf("Can't open the output file!\n");
        exit(1);
    }

    if ((ftable = fopen("ftable.txt", "w")) == NULL)
    {
        printf("Can't open ftable.txt file!\n");
        exit(1);
    }

    //printf("List object codes?(Y/N)");	/* 是否输出虚拟机代码 */
    //scanf("%s", fname);
    listswitch = 1;//(fname[0]=='y' || fname[0]=='Y');

    //printf("List symbol table?(Y/N)");	/* 是否输出符号表 */
    //scanf("%s", fname);
    tableswitch = 1;//(fname[0]=='y' || fname[0]=='Y');

    init();		/* 初始化 */
    err = 0;
    cc = ll = cx = 0;
    ch = ' ';

    getsym();

    addset(nxtlev, declbegsys, statbegsys, symnum);
    nxtlev[period] = true;

    block(0, 0, nxtlev);	/* 处理分程序 */
    int j;
    if (iniexit[0] >  0) {
        for (j = 1; j <= iniexit[0]; ++j) {
            code[iniexit[j]].a = cx-1;
        }
    }
    if (sym != period)
    {
    	error(9);
    }

    if (err == 0)
    {
        printf("\n===Parsing success!===\n");
        fprintf(foutput,"\n===Parsing success!===\n");

        if ((fcode = fopen("fcode.txt", "w")) == NULL)
        {
            printf("Can't open fcode.txt file!\n");
            exit(1);
        }

        if ((fresult = fopen("fresult.txt", "w")) == NULL)
        {
            printf("Can't open fresult.txt file!\n");
            exit(1);
        }

        listall();	 /* 输出所有代码 */
        fclose(fcode);

        interpret();	/* 调用解释执行程序 */
        fclose(fresult);
    }
    else
    {
        printf("\n%d errors in small program!\n",err);
        fprintf(foutput,"\n%d errors in small program!\n",err);
    }

    fclose(ftable);
    fclose(foutput);
    fclose(fin);
    //getchar();
    system("pause");
    return 0;
}

/*
 * 初始化
 */
void init()
{
    int i;
    memset(inibreak,-1,sizeof(inibreak));
    memset(inicont,-1,sizeof(inicont));
    memset(iniexit,-1,sizeof(inicont));
    iniexit[0] = 0;
    /* 设置单字符符号 */
    for (i=0; i<=255; i++)
    {
        ssym[i] = nul;
    }
    ssym['+'] = plus;
    ssym['-'] = minus;
    ssym['*'] = times;
    ssym['/'] = slash;
    ssym['('] = lparen;
    ssym[')'] = rparen;
    ssym['='] = constder;
    ssym[','] = comma;
    ssym['.'] = period;
    ssym['#'] = neq;
    ssym[';'] = semicolon;
    ssym['^'] = xor;
    ssym['%'] = mod;
    ssym['&'] = and;
    ssym['|'] = or;
    ssym['!'] = not;
    ssym['{'] = lbra;
    ssym['}'] = rbra;
    ssym[':'] = colon;

    /* 设置保留字名字,按照字母顺序，便于二分查找 */
    strcpy(&(word[0][0]), "begin");
    strcpy(&(word[1][0]), "bool");
    strcpy(&(word[2][0]), "break");
    strcpy(&(word[3][0]), "call");
    strcpy(&(word[4][0]), "case");
    strcpy(&(word[5][0]), "const");
    strcpy(&(word[6][0]), "continue");
    strcpy(&(word[7][0]), "default");
    strcpy(&(word[8][0]), "do");
    strcpy(&(word[9][0]), "else");
    strcpy(&(word[10][0]), "end");
    strcpy(&(word[11][0]), "exit");
    strcpy(&(word[12][0]), "false");
    strcpy(&(word[13][0]), "finish");
    strcpy(&(word[14][0]), "for");
    strcpy(&(word[15][0]), "if");
    strcpy(&(word[16][0]), "int");
    strcpy(&(word[17][0]), "odd");
    strcpy(&(word[18][0]), "proc");
    strcpy(&(word[19][0]), "read");
    strcpy(&(word[20][0]), "repeat");
    strcpy(&(word[21][0]), "start");
    strcpy(&(word[22][0]), "switch");
    strcpy(&(word[23][0]), "then");
    strcpy(&(word[24][0]), "to");
    strcpy(&(word[25][0]), "true");
    strcpy(&(word[26][0]), "until");
    strcpy(&(word[27][0]), "while");
    strcpy(&(word[28][0]), "write");

    /* 设置保留字符号 */
    wsym[0] = beginsym;
    wsym[1] = boolsym;
    wsym[2] = breaksym;
    wsym[3] = callsym;
    wsym[4] = casesym;
    wsym[5] = constsym;
    wsym[6] = continuesym;
    wsym[7] = defaultsym;
    wsym[8] = dosym;
    wsym[9] = elsesym;
    wsym[10] = endsym;
    wsym[11] = exitsym;
    wsym[12] = falsesym;
    wsym[13] = finishsym;
    wsym[14] = forsym;
    wsym[15] = ifsym;
    wsym[16] = intsym;
    wsym[17] = oddsym;
    wsym[18] = procsym;
    wsym[19] = readsym;
    wsym[20] = repeatsym;
    wsym[21] = startsym;
    wsym[22] = switchsym;
    wsym[23] = thensym;
    wsym[24] = tosym;
    wsym[25] = truesym;
    wsym[26] = untilsym;
    wsym[27] = whilesym;
    wsym[28] = writesym;

    /* 设置指令名称 */
    strcpy(&(mnemonic[lit][0]), "lit");
    strcpy(&(mnemonic[opr][0]), "opr");
    strcpy(&(mnemonic[lod][0]), "lod");
    strcpy(&(mnemonic[sto][0]), "sto");
    strcpy(&(mnemonic[cal][0]), "cal");
    strcpy(&(mnemonic[ini][0]), "int");
    strcpy(&(mnemonic[jmp][0]), "jmp");
    strcpy(&(mnemonic[jpc][0]), "jpc");

    /* 设置符号集 */
    for (i=0; i<symnum; i++)
    {
        declbegsys[i] = false;
        statbegsys[i] = false;
        facbegsys[i] = false;
    }

    /* 设置声明开始符号集 */
    declbegsys[constsym] = true;
    declbegsys[intsym] = true;
    declbegsys[boolsym]  = true;
    declbegsys[procsym] = true;

    /* 设置语句开始符号集 */
    //statbegsys[elsesym] = true;
    //statbegsys[endsym] = true;
    //statbegsys[ident] = true;
    //statbegsys[writesym] = true;
    //statbegsys[readsym] = true;
    statbegsys[lbra] = true;
    statbegsys[callsym] = true;
    statbegsys[ifsym] = true;
    statbegsys[whilesym] = true;
    statbegsys[breaksym] = true;
    statbegsys[forsym] = true;
    statbegsys[switchsym] = true;
    //statbegsys[casesym] = true;
    //statbegsys[defaultsym] = true;
   // statbegsys[continuesym] = true;
    statbegsys[exitsym] = true;
    statbegsys[repeatsym] = true;



    /* 设置因子开始符号集 */
    facbegsys[ident] = true;
    facbegsys[number] = true;
    facbegsys[lparen] = true;
    facbegsys[truesym] = true;
    facbegsys[falsesym] = true;
}

/*
 * 用数组实现集合的集合运算
 */
int inset(int e, bool* s)
{
    return s[e];
}

int addset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        sr[i] = s1[i]||s2[i];
    }
    return 0;
}

int subset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        sr[i] = s1[i]&&(!s2[i]);
    }
    return 0;
}

int mulset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        sr[i] = s1[i]&&s2[i];
    }
    return 0;
}

/*
 *	出错处理，打印出错位置和错误编码
 */
void error(int n)
{
    char space[81];
    memset(space,32,81);

    space[cc-1]=0; /* 出错时当前符号已经读完，所以cc-1 */

    printf("**%s^%d\n", space, n);
    fprintf(foutput,"**%s^%d\n", space, n);

    err = err + 1;
    if (err > maxerr)
    {
        exit(1);
    }
}

/*
 * 过滤空格，读取一个字符
 * 每次读一行，存入line缓冲区，line被getsym取空后再读一行
 * 被函数getsym调用
 */
void getch()
{
    if (cc == ll) /* 判断缓冲区中是否有字符，若无字符，则读入下一行字符到缓冲区中 */
    {
        if (feof(fin))
        {
            printf("Program is incomplete!\n");
            fprintf(foutput,"Program is incomplete!\n");
            exit(1);
        }
        ll = 0;
        cc = 0;
        printf("%d ", cx);
        fprintf(foutput,"%d ", cx);
        ch = ' ';
        while (ch != 10)
        {
            if (EOF == fscanf(fin,"%c", &ch))
            {
                line[ll] = 0;
                break;
            }
            printf("%c", ch);
            fprintf(foutput, "%c", ch);
            line[ll] = ch;
            ll++;
        }
    }
    ch = line[cc];
    cc++;
}

/*
 * 词法分析，获取一个符号
 */
void getsym()
{
    int i,j,k;

    while (ch == ' ' || ch == 10 || ch == 9)	/* 过滤空格、换行和制表符 */
    {
        getch();
    }
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) /* 当前的单词是标识符或是保留字 */
    {
        k = 0;
        do
        {
            if(k < al)
            {
                a[k] = ch;
                k++;
            }
            getch();
        }
        while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));
        a[k] = 0;
        strcpy(id, a);
        i = 0;
        j = norw - 1;
        do      /* 搜索当前单词是否为保留字，使用二分法查找 */
        {
            k = (i + j) / 2;
            if (strcmp(id,word[k]) <= 0)
            {
                j = k - 1;
            }
            if (strcmp(id,word[k]) >= 0)
            {
                i = k + 1;
            }
        }
        while (i <= j);
        if (i-1 > j) /* 当前的单词是保留字 */
        {
            sym = wsym[k];
            if(sym == truesym)
                cur = 1;
            else if (sym == falsesym)
                cur = 0;
        }
        else /* 当前的单词是标识符 */
        {
            sym = ident;
        }
    }
    else if (ch >= '0' && ch <= '9') /* 当前的单词是数字 */
    {
        k = 0;
        num = 0;
        sym = number;
        do
        {
            num = 10 * num + ch - '0';
            k++;
            getch();;
        }
        while (ch >= '0' && ch <= '9');   /* 获取数字的值 */
        k--;
        if (k > nmax) /* 数字位数太多 */
        {
            error(30);
        }
    }
    else if (ch == ':')		/* 检测赋值符号 */
    {
        getch();
        if (ch == '=')
        {
            sym = becomes;
            getch();
        }
        else
        {
            sym = colon;	/* 冒号 */
        }
    }
    else if (ch == '!') /* 检测不等于符号 */
    {
        getch();
        if (ch == '=')
        {
            sym = neq;
            getch();
        }
        else
        {
            sym = not;	/* 取非符号 */
        }
    }
    else if (ch == '<')		/* 检测小于或小于等于符号 */
    {
        getch();
        if (ch == '=')
        {
            sym = leq;
            getch();
        }
        else
        {
            sym = lss;
        }
    }
    else if (ch == '>')		/* 检测大于或大于等于符号 */
    {
        getch();
        if (ch == '=')
        {
            sym = geq;
            getch();
        }
        else
        {
            sym = gtr;
        }
    }
    else if (ch == '=')  /*检测判断相等符号*/
    {
        getch();
        if (ch == '=')
        {
            sym = eql;
            getch();
        }
        else
        {
            sym = constder;
        }
    }
    else if (ch == '+')  /*检测自增运算符*/
    {
        getch();
        if (ch == '+')
        {
            sym = inc;
            getch();
        }
        else
        {
            sym = plus;
        }
    }
    else if (ch == '-')  /*检测自减运算符*/
    {
        getch();
        if (ch == '-')
        {
            sym = dec;
            getch();
        }
        else
        {
            sym = minus;
        }
    }
    else if (ch == '/')/*预处理注释*/
    {
        getch();
        if (ch == '/')//行注释
        {   getch();
            while (ch != 10){
                getch();
            }
            getch();
            getsym();
        }
        else if (ch == '*')/*块注释*/
        {
            getch();
            while (1) {
                if (ch == '*')
                {
                    getch();
                    if (ch == '/'){
                        getch();
                        //getsym();
                        break;
                    }
                    else continue;
                }
                else getch();
            }
            getsym();
        }
        else
        {
            sym = slash;
        }
    }
    else if (ch == '*')
    {
        getch();
        if (ch == '/')
        {
            sym = rnote;
            getch();
        }
        else
        {
            sym = times;
        }
    }
    else
    {
        sym = ssym[(int)ch];		/* 当符号不满足上述条件时，全部按照单字符符号处理 */
        if (sym != period)
        {
            getch();
        }
    }
}

/*
 * 生成虚拟机代码
 *
 * x: instruction.f;
 * y: instruction.l;
 * z: instruction.a;
 */
void gen(enum fct x, int y, int z )
{
    if (cx >= cxmax)
    {
        printf("Program is too long!\n");	/* 生成的虚拟机代码程序过长 */
        exit(1);
    }
    if ( z >= amax)
    {
        printf("Displacement address is too big!\n");	/* 地址偏移越界 */
        exit(1);
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
}


/*
 * 测试当前符号是否合法
 *
 * 在语法分析程序的入口和出口处调用测试函数test，
 * 检查当前单词进入和退出该语法单位的合法性
 *
 * s1:	需要的单词集合
 * s2:	如果不是需要的单词，在某一出错状态时，
 *      可恢复语法分析继续正常工作的补充单词符号集合
 * n:  	错误号
 */
void test(bool* s1, bool* s2, int n)
{
    if (!inset(sym, s1))
    {
        error(n);
        /* 当检测不通过时，不停获取符号，直到它属于需要的集合或补救的集合 */
        while ((!inset(sym,s1)) && (!inset(sym,s2)))
        {
            getsym();
        }
    }
}

/*
 * 编译程序主体
 *
 * lev:    当前分程序所在层
 * tx:     符号表当前尾指针
 * fsys:   当前模块后继符号集合
 */
void block(int lev, int tx, bool* fsys)
{
    int i;

    int dx;                 /* 记录数据分配的相对地址 */
    int tx0;                /* 保留初始tx */
    int cx0;                /* 保留初始cx */
    bool nxtlev[symnum];    /* 在下级函数的参数中，符号集合均为值参，但由于使用数组实现，
	                           传递进来的是指针，为防止下级函数改变上级函数的集合，开辟新的空间
	                           传递给下级函数*/
    int nxtbreak[breakmax];   /**/
    int nxtcont[breakmax];
    dx = 3;                 /* 三个空间用于存放静态链SL、动态链DL和返回地址RA  */
    tx0 = tx;		        /* 记录本层标识符的初始位置 */
    table[tx].adr = cx;	    /* 记录当前层代码的开始位置 */
    gen(jmp, 0, 0);         /* 产生跳转指令，跳转位置未知暂时填0 */
    if (lev > levmax)		/* 嵌套层数过多 */
    {
        error(32);
    }
    do
    {
        while (sym == constsym)	/* 遇到常量声明符号，开始处理常量声明 */
        {
            getsym();
            if (sym == intsym)  /*整数，type=1*/
            {
                getsym();
                do
                {
                    constdeclaration(&tx, lev, &dx,1);	/* dx的值会被constdeclaration改变，使用指针 */
                    while (sym == comma)  /* 遇到逗号继续定义常量 */
                    {
                        getsym();
                        constdeclaration(&tx, lev, &dx,1);
                    }
                    if (sym == semicolon) /* 遇到分号结束定义常量 */
                    {
                        getsym();
                    }
                    else
                    {
                        error(5);   /* 漏掉了分号 */
                    }
                }
                while (sym == ident);
            }
            else if (sym == boolsym)  /*bool型，type = 2*/
            {
                getsym();
                do
                {
                    constdeclaration(&tx, lev, &dx,2);	/* dx的值会被constdeclaration改变，使用指针 */
                    while (sym == comma)  /* 遇到逗号继续定义常量 */
                    {
                        getsym();
                        constdeclaration(&tx, lev, &dx,2);
                    }
                    if (sym == semicolon) /* 遇到分号结束定义常量 */
                    {
                        getsym();
                    }
                    else
                    {
                        error(5);   /* 漏掉了分号 */
                    }
                }
                while (sym == ident);
            }
            else
            {
                error(34); /*未定义数据类型*/
            }
        }
        if (sym == intsym)  /* 遇到变量声明符号，开始处理变量声明 */
        {
            getsym();
            do
            {
                vardeclaration(&tx, lev, &dx,1);
                while (sym == comma)
                {
                    getsym();
                    vardeclaration(&tx, lev, &dx,1);
                }
                if (sym == semicolon)
                {
                    getsym();
                }
                else
                {
                    error(5); /* 漏掉了分号 */
                }
            }
            while (sym == ident);
        }
        while (sym == boolsym)
        {
            getsym();
            do
            {
                vardeclaration(&tx, lev, &dx,2);
                while (sym == comma)
                {
                    getsym();
                    vardeclaration(&tx, lev, &dx,2);
                }
                if (sym == semicolon)
                {
                    getsym();
                }
                else
                {
                    error(5); /* 漏掉了分号 */
                }
            }
            while (sym == ident);
        }
        while (sym == procsym) /* 遇到过程声明符号，开始处理过程声明 */
        {
            getsym();
            if (sym == ident)
            {
                enter(proc, &tx, lev, &dx,0);	/* 填写符号表 */
                getsym();
            }
            else
            {
                error(4);	/* proc后应为标识符 */
            }
            if (sym == semicolon)
            {
                getsym();
            }
            else
            {
                error(5);	/* 漏掉了分号 */
            }
            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[semicolon] = true;
            block(lev + 1, tx, nxtlev); /* 递归调用 */
            if(sym == semicolon)
            {
                getsym();
                memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
                nxtlev[ident] = true;
                nxtlev[procsym] = true;
                test(nxtlev, fsys, 6);
            }
            else
            {
                error(5);	/* 漏掉了分号 */
            }
        }
        memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        test(nxtlev, declbegsys, 7);
    }while (inset(sym, declbegsys));	/* 直到没有声明符号 */
    code[table[tx0].adr].a = cx;	/* 把前面生成的跳转语句的跳转位置改成当前位置 */
    table[tx0].adr = cx;	        /* 记录当前过程代码地址 */
    table[tx0].size = dx;	        /* 声明部分中每增加一条声明都会给dx增加1，声明部分已经结束，dx就是当前过程数据的size */
    cx0 = cx;
    gen(ini, 0, dx);	            /* 生成指令，此指令执行时在数据栈中为被调用的过程开辟dx个单元的数据区 */
    if (tableswitch)		/* 输出符号表 */
    {
        for (i = 1; i <= tx; i++)
        {
            switch (table[i].kind)
            {
            case constant:
                printf("    %d const %s ", i, table[i].name);
                printf("type=%s ", table[i].type == 1?"int":"boolean");
                if (table[i].type == 1)
                    printf("val=%d\n", table[i].val);
                else printf("bool=%s\n", table[i].bl?"true":"false");
                fprintf(ftable, "    %d const %s ", i, table[i].name);
                fprintf(ftable, "type=%s ", table[i].type == 1?"int":"boolean");
                if (table[i].type == 1)
                    fprintf(ftable, "val=%d\n", table[i].val);
                else fprintf(ftable, "bool=%s\n", table[i].bl?"true":"false");
                break;
            case variable:
                printf("    %d var   %s ", i, table[i].name);
                printf("lev=%d addr=%d\n", table[i].level, table[i].adr);
                fprintf(ftable, "    %d var   %s ", i, table[i].name);
                fprintf(ftable, "lev=%d addr=%d\n", table[i].level, table[i].adr);
                break;
            case proc:
                printf("    %d proc  %s ", i, table[i].name);
                printf("lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
                fprintf(ftable,"    %d proc  %s ", i, table[i].name);
                fprintf(ftable,"lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
                break;
            }
        }
        printf("\n");
        fprintf(ftable,"\n");
    }
    /* 语句后继符号为分号或end */
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);	/* 每个后继符号集合都包含上层后继符号集合，以便补救 */
    memcpy(nxtbreak,inibreak,sizeof(int) * breakmax);
    memcpy(nxtcont,inicont,sizeof(int) * breakmax);
    nxtlev[semicolon] = true;
    nxtlev[rbra] = true;
    statement(nxtlev, &tx, lev, nxtbreak, nxtcont);
    gen(opr, 0, 0);	                    /* 每个过程出口都要使用的释放数据段指令 */
    memset(nxtlev, 0, sizeof(bool) * symnum);	/* 分程序没有补救集合 */
    test(fsys, nxtlev, 8);            	/* 检测后继符号正确性 */
    listcode(cx0);                      /* 输出本分程序生成的代码 */
}

/*
 * 在符号表中加入一项
 *
 * k:      标识符的种类为const，var或proc
 * ptx:    符号表尾指针的指针，为了可以改变符号表尾指针的值
 * lev:    标识符所在的层次
 * pdx:    dx为当前应分配的变量的相对地址，分配后要增加1
 *
 */
void enter(enum object k, int* ptx,	int lev, int* pdx,int type)
{
    (*ptx)++;
    strcpy(table[(*ptx)].name, id); /* 符号表的name域记录标识符的名字 */
    table[(*ptx)].kind = k;
    switch (k)
    {
    case constant:	/* 常量 */
        if (type == 1)
        {
            if (num > amax)
            {
                error(31);	/* 常数越界 */
                num = 0;
            }
            table[(*ptx)].type = type;
            table[(*ptx)].val = num; /* 登记常数的值 */
        }
        else if (type == 2)
        {
            table[(*ptx)].val = num; /* 登记bool常数的值,转为1或0 */
            table[(*ptx)].type = type;
        }
        break;
    case variable:	/* 变量 */
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*pdx);
        table[(*ptx)].type = type;
        (*pdx)++;
        break;
    case proc:	/* 过程 */
        table[(*ptx)].level = lev;
        break;
    }
}

/*
 * 查找标识符在符号表中的位置，从tx开始倒序查找标识符
 * 找到则返回在符号表中的位置，否则返回0
 *
 * id:    要查找的名字
 * tx:    当前符号表尾指针
 */
int position(char* id, int tx)
{
    int i;
    strcpy(table[0].name, id);
    i = tx;
    while (strcmp(table[i].name, id) != 0)
    {
        i--;
    }
    return i;
}

/*
 * 常量声明处理
 */
void constdeclaration(int* ptx, int lev, int* pdx, int type)
{
    if (sym == ident)
    {
        getsym();
        if (sym == constder || sym == becomes)
        {
            if (sym == becomes)
            {
                error(1);	/* 把=写成了:= */
            }
            getsym();
            if (type == 1)
            {
                enter(constant, ptx, lev, pdx,1);
                getsym();
            }
            else if (type == 2)
            {
                enter(constant, ptx, lev, pdx,2);
                getsym();
            }
            //else
            //{
            //    error(2);	/* 常量声明中的=后和类型不匹配 */
            //}
        }
        else
        {
            error(3);	/* 常量声明中的标识符后应是= */
        }
    }
    else
    {
        error(4);	/* const后应是标识符 */
    }
}

/*
 * 变量声明处理
 */
void vardeclaration(int* ptx,int lev,int* pdx, int type)
{
    if (sym == ident)
    {
        enter(variable, ptx, lev, pdx, type);	// 填写符号表
        getsym();
    }
    else
    {
        error(4);	/* 后面应是标识符 */
    }
}

/*
 * 输出目标代码清单
 */
void listcode(int cx0)
{
    int i;
    if (listswitch)
    {
        printf("\n");
        for (i = cx0; i < cx; i++)
        {
            printf("%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
        }
    }
}

/*
 * 输出所有目标代码
 */
void listall()
{
    int i;
    if (listswitch)
    {
        for (i = 0; i < cx; i++)
        {
            printf("%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
            fprintf(fcode,"%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
        }
    }
}

/*
 * 语句处理
 */
 /*fbreak* : （上一层传来的）当前循环层所含的break对应无条件转移地址，fbreak[0]为地址个数，当前层不能存在break时fbreak[0]为-1*/
 /*fcont:(上一层传来的)当前循环层所含的continue对应的无条件转移代码地址，当前层不能存在continue时fcont[0] = -1*/
void statement(bool* fsys, int* ptx, int lev, int *fbreak, int *fcont)
{
    int i, cx1, cx2,cx3,cx4;
    bool nxtlev[symnum];
    int nxtbreak[breakmax];
    int nxtcont[breakmax];
    if (sym == ident)	/* 准备按照赋值语句或自增自减语句处理 */
    {
        i = position(id, *ptx);/* 查找标识符在符号表中的位置 */
        if (i == 0)
        {
            error(11);	/* 标识符未声明 */
        }
        else if(table[i].kind != variable)
        {
            error(12);	/* 赋值语句中，赋值号左部标识符应该是变量 */
            i = 0;
        }
        else
        {
            getsym();
            if(sym == becomes)
            {
                getsym();
                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                expression(nxtlev, ptx, lev);	/* 处理赋值符号右侧表达式 */
                if(i != 0)
                {
                    /* expression将执行一系列指令，但最终结果将会保存在栈顶，执行sto命令完成赋值 */
                    gen(sto, lev-table[i].level, table[i].adr);
                }
            }
            else if (sym == inc || sym == dec) {
                    if (i != 0) {
                        gen(lod, lev-table[i].level, table[i].adr);
                    gen(lit,0,1);
                    if (sym == inc)
                        gen(opr,0,2);
                    else gen(opr,0,3);
                    gen(sto, lev-table[i].level, table[i].adr);
                }

            }
            else
            {
                error(13);	/* 没有检测到赋值或自增自减符号 */
            }
        }
    }
    else if (sym == readsym)	/* 准备按照read语句处理 */
    {
         /* 一条read语句可读一个变量 */
         getsym();
         if (sym == ident)
         {
             i = position(id, *ptx);	/* 查找要读的变量 */
         }
         else
         {
             i = 0;
         }
         if (i == 0)
         {
             error(35);	/* read语句括号中的标识符应该是声明过的变量 */
         }
         else
         {
             if (table[i].type == 1)
             gen(opr, 0, 16);	/* 生成输入指令，读取值到栈顶 */
             else {
                gen(opr,1,16);
             }
             gen(sto, lev-table[i].level, table[i].adr);	/* 将栈顶内容送入变量单元中 */
         }
         getsym();
    }
    else if (sym == writesym)	/* 准备按照write语句处理 */
    {
        /* 一条write可输出一个变量的值 */
        isbool = 1;
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        expression(nxtlev, ptx, lev);	/* 调用表达式处理 */
        if (isbool == 0)
            gen(opr, 0, 14);	/* 生成输出指令，输出栈顶的值 */
        else gen(opr,1,14);
        gen(opr, 0, 15);	/* 生成换行指令 */
        //getsym();
    }
    else if (sym == callsym)	/* 准备按照call语句处理 */
    {
        getsym();
        if (sym != ident)
        {
            error(14);	/* call后应为标识符 */
        }
        else
        {
            i = position(id, *ptx);
            if (i == 0)
            {
                error(11);	/* 过程名未找到 */
            }
            else
            {
                if (table[i].kind == proc)
                {
                    gen(cal, lev-table[i].level, table[i].adr);	/* 生成call指令 */
                }
                else
                {
                    error(15);	/* call后标识符类型应为过程 */
                }
            }
            getsym();
        }
    }
    else if (sym == ifsym)	/* 准备按照if语句处理 */
    {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[thensym] = true;/* 后继符号为then */
        condition(nxtlev, ptx, lev); /* 调用条件处理 */
        if (sym == thensym)
        {
            getsym();
        }
        else
        {
            error(16);	/* 缺少then */
        }
        cx1 = cx;	/* 保存当前指令地址 */
        gen(jpc, 0, 0);	/* 生成条件跳转指令，跳转地址未知，暂时写0 */
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[semicolon] = true;
        nxtlev[endsym] = true;
        nxtlev[elsesym] = true;/* 后继符号为end或else或分号 */
        statement(nxtlev, ptx, lev, fbreak, fcont);	/* 处理then和else或end之间的语句 */
        code[cx1].a = cx;	/* 经statement处理后，cx为then后语句执行完的位置，它正是前面未定的跳转地址，此时进行回填 */
        getsym();
        if (sym == semicolon) {
            getsym();
        }
        if (sym == endsym) {
            getsym();
        }
        else if (sym == elsesym) {
            getsym();
            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[semicolon] = true;
            nxtlev[endsym] = true;
            statement(nxtlev, ptx, lev, fbreak, fcont);	/* 处理else和end之间的语句 */
            getsym();
            if (sym == semicolon) {
                getsym();
            }
            if (sym == endsym) {
                getsym();
            }
            else {
                error(17);  /*缺少end*/
            }
        }
    }
    else if (sym == lbra)	/* 准备按照复合语句处理 */
    {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[semicolon] = true;
        nxtlev[rbra] = true;	/* 后继符号为分号或end */

        statement(nxtlev, ptx, lev,fbreak,fcont);  /* 对begin与end之间的语句进行分析处理 */
        /* 如果分析完一句后遇到语句开始符或分号，则循环分析下一句语句 */
        while (inset(sym, statbegsys) || sym == semicolon)
        {
            if (sym == semicolon)
            {
                getsym();
            }
            else
            {
                error(10);	/* 缺少分号 */
            }
            statement(nxtlev, ptx, lev,fbreak,fcont);
        }
        if(sym == rbra)
        {
            getsym();
        }
        else
        {
            error(17);	/* 缺少end */
        }
    }
    else if (sym == whilesym)	/* 准备按照while语句处理 */
    {
        cx1 = cx;	/* 保存判断条件操作的位置 */
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        memcpy(nxtbreak,fbreak,sizeof(int) * breakmax);
        memcpy(nxtcont,fcont ,sizeof(int) * breakmax);
        nxtbreak[0] = 0;
        nxtcont[0] = 0;
		nxtlev[continuesym] = true;
		nxtlev[breaksym] = true;
        nxtlev[dosym] = true;	/* 后继符号为do */
        condition(nxtlev, ptx, lev);	/* 调用条件处理 */
        cx2 = cx;	/* 保存循环体的结束的下一个位置 */
        gen(jpc, 0, 0);	/* 生成条件跳转，但跳出循环的地址未知，标记为0等待回填 */
        if (sym == dosym)
        {
            getsym();
        }
        else
        {
            error(18);	/* 缺少do */
        }
        statement(nxtlev, ptx, lev, nxtbreak, nxtcont);	/* 循环体 */
        gen(jmp, 0, cx1);	/* 生成无条件跳转指令，跳转到前面判断条件操作的位置 */
        code[cx2].a = cx;	/* 回填跳出循环的地址 */
        if (nxtbreak[0] != 0) {
            int j;
            for (j = 1; j <= nxtbreak[0]; ++j) {
                code[nxtbreak[j]].a = cx;
            }
        }
        if (nxtcont[0] != 0) {
            int j;
            for (j = 1; j <= nxtcont[0]; ++j) {
                code[nxtcont[j]].a = cx1;
            }
        }
    }
    else if (sym == forsym) {
        int cx3,cx4;//,cx5,cx6;
        getsym();
        if (sym != lparen) {
            error(34);/*格式错误，应该是左括号*/
        }
        else  {
            getsym();
        }
        if (sym != ident) {
            error(14);  /*for的第一部分应该是赋值语句，开头是标识符*/
        }
        else {
            i = position(id,*ptx);
            if (i == 0)
                error(11);/*标识符未声明*/
            else if (table[i].kind != variable) {
                error(12);/*赋值语句中，赋值号左部标识符应该是变量*/
                i = 0;
            }
            getsym();
        }
        if (sym == becomes)
            getsym();
        else error(13);/* 没有检测到赋值符号 */
        memcpy(nxtlev,fsys,sizeof(bool)*symnum);
        nxtlev[semicolon] = true;
        expression(nxtlev, ptx, lev);	/* 处理赋值符号右侧表达式 */
        if(i != 0)
        {
            /* expression将执行一系列指令，但最终结果将会保存在栈顶，执行sto命令完成赋值 */
            gen(sto, lev-table[i].level, table[i].adr);
        }
        //getsym();///***不知道需不需要getsym，需要测试
        if (sym != semicolon) {
            error(10); /*缺少分号*/
        }///for(i=0;
        else {
            getsym();
        }
        cx1 = cx; /*保存判断条件操作位置*/
        memcpy(nxtlev,fsys,sizeof(bool)*symnum);
        memcpy(nxtbreak,fbreak,sizeof(int) * breakmax);
        memcpy(nxtcont,fcont ,sizeof(int) * breakmax);
        nxtbreak[0] = 0;
        nxtcont[0] = 0;
        nxtlev[semicolon] = true;/*后继符号为分号*/
        condition(nxtlev,ptx,lev);/**调用条件处理*/
        cx2 = cx;   /*条件跳转，循环体结束的下一个位置*/
        gen(jpc,0,0);
        cx3 = cx;   /*无条件跳转，begin后面的第一条语句(跳过自增自减赋值)*/
        gen(jmp,0,0);
        if (sym != semicolon) {
            error(10);/*缺少分号*/
        }
        else {///for(i = 0; i < n;
            getsym();
        }
        if (sym != ident) {
            error(45);/*for第三部分应是自增或自减或赋值，以标识符开头*/
        }
        else {
            i = position(id, *ptx);
            if (i == 0)
            {
                error(11);/*标识符未声明*/
            }
            else if (table[i].kind != variable) {
                error(12);  /*标识符必须是变量*/
                i = 0;
            }
            getsym();
        }
        if (sym != inc && sym != dec && sym != becomes) {
            error(46);/*必须是自增自减或赋值运算符*/
        }
        else {
            if (sym == inc || sym == dec) {/*处理自增自减运算*/
                if (i != 0) {
                    cx4  = cx;/*自增运算符开始位置*/
                    gen(lod, lev-table[i].level, table[i].adr);
                    gen(lit,0,1);
                    if (sym == inc)
                        gen(opr,0,2);
                    else gen(opr,0,3);
                    gen(sto, lev-table[i].level, table[i].adr);
                    //cx5 = cx;
                    gen(jmp,0,cx1);
                    getsym();
                }
                else {
                    error(47);
                    getsym();
                }
            }
            else if (sym == becomes) {
                if (i != 0) {
                    cx4 = cx;/*赋值语句开始位置*/
                    getsym();
                    memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                    nxtlev[rparen] = true;
                    expression(nxtlev,ptx,lev);
                    gen(sto, lev-table[i].level, table[i].adr);
                    gen(jmp,0,cx1);
                }
                else {
                    error(47);
                    getsym();
                }
            }
        }///i = 0; i < n; i++
        if (sym != rparen) {
            error(33);  /*格式错误，缺少右括号*/
        }
        else {///for(i = 0; i < n; i++)
            getsym();
        }
        code[cx3].a = cx;/*循环体开始位置*/
        //cx6 = cx;
        memcpy(nxtlev,fsys,sizeof(bool)*symnum);
        nxtlev[continuesym] = true;
        nxtlev[breaksym] = true;
        statement(nxtlev, ptx, lev, nxtbreak, nxtcont);	/* 循环体 */
        gen(jmp,0,cx4);/*for第三部分，赋值语句或自增自减*/
        code[cx2].a = cx;/*for代码块之后的语句起始位置*/
        if (nxtbreak[0] > 0) {
            int j;
            for (j = 1; j <= nxtbreak[0]; ++j) {
                code[nxtbreak[j]].a = cx;
            }
        }
        if (nxtcont[0] > 0) {
            int j;
            for (j = 1; j <= nxtcont[0]; ++j) {
                code[nxtcont[j]].a = cx4;
            }
        }
    }
    else if (sym == repeatsym) {
        cx1 = cx;
        gen(jmp,0,0);/*第一次执行时要先跳转到条件判断语句，待回填*/
        getsym();
        cx2 = cx;   /*repeat代码块起始指令地址*/
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        memcpy(nxtbreak,fbreak,sizeof(int) * breakmax);
        memcpy(nxtcont,fcont ,sizeof(int) * breakmax);
        nxtbreak[0] = 0;
        nxtcont[0] = 0;
		nxtlev[continuesym] = true;
		nxtlev[breaksym] = true;
        nxtlev[untilsym] = true;	/* 后继符号为until */
        statement(nxtlev,ptx,lev,nxtbreak,nxtcont);
        if (sym == semicolon) {
            getsym();
        }
        if (sym != untilsym) {
            error(49); /*缺少until*/
        }
        else {
            getsym();
        }
        cx3 = cx;
        code[cx1].a = cx3;
        condition(nxtlev,ptx,lev);
        gen(jpc,0,cx2);
        cx4 = cx;
        gen(jmp,0,0);
        code[cx4].a = cx;
        if (nxtbreak[0] != 0) {
            int j = 0;
            for (j = 1; j <= nxtbreak[0] ; ++j) {
                code[nxtbreak[j]].a = cx;
            }
        }
        if (nxtcont[0] != 0) {
            int j = 0;
            for (j = 1; j <= nxtcont[0]; ++j) {
                code[nxtcont[j]].a = cx3;
            }
        }
    }
    else if (sym == switchsym) {
        getsym();
        if (sym != lparen) {
            error(56);/*缺少左括号*/
        }
        else {
            getsym();
        }
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        memcpy(nxtbreak,fbreak,sizeof(int) * breakmax);
        memcpy(nxtcont,fcont ,sizeof(int) * breakmax);
        nxtbreak[0] = 0;
		//nxtlev[continuesym] = true;
		nxtlev[rparen] = true;
        expression(nxtlev,ptx,lev);
        //gen(opr,0,23);  /*针对switch设计的指令，将表达式的值存入table[0].val*/
        if (sym != rparen) {
            error(57); /*缺少右括号*/
        }
        else {
            getsym();
        }
        nxtlev[casesym] = true;
        nxtlev[defaultsym] = true;
		//nxtlev[breaksym] = true;
        nxtlev[rbra] = true;	/* 后继符号为case,default,右大括号 */
        lastcase = -1;
        statement(nxtlev, ptx, lev, nxtbreak, nxtcont);
        code[lastcase].a = cx;
        if(nxtbreak[0] != 0) {
            int j;
            for (j = 1; j <= nxtbreak[0]; ++j) {
                code[nxtbreak[j]].a = cx;
            }
        }
    }
    else if (sym == casesym) {
        getsym();
        if (sym != ident && sym != number) {
            error(48);/*case后应该是标识符或数字*/
        }
        else {
            if (lastcase != -1) {
                code[lastcase].a = cx;
            }
            if (sym == ident) {
                i = position(id,*ptx);
                if (i == 0) {
                    error(35);/*标识符未声明*/
                }
                else {
                    if (table[i].kind == variable)
                        gen(lod, lev-table[i].level, table[i].adr);
                    else gen(lit,0,table[i].val);
                    gen(opr,0,23);
                    lastcase = cx;
                    gen(jpc,0,0);/*跳转到下一个case或default，待回填*/
                    getsym();
                }
            }
            else if (sym == number) {
                if (num > amax)
                {
                    error(31); /* 数越界 */
                    num = 0;
                }
                gen(lit, 0, num);
                gen(opr,0,23);
                lastcase = cx;
                gen(jpc,0,0);
                getsym();
            }
        }
        if (sym != colon) {
            error(54);/*缺少冒号*/
        }
        else {
            getsym();
        }
        memcpy(nxtlev,fsys,sizeof(bool) * symnum);
        nxtlev[breaksym] = true;
        nxtlev[semicolon] = true;
        statement(nxtlev,ptx,lev,fbreak,fcont);
    }
    else if (sym == defaultsym) {
        if (lastcase != -1) {
            code[lastcase].a = cx;
        }
        lastcase = -1;
        getsym();
        if (sym != colon) {
            error(54);/*缺少冒号*/
        }
        else {
            getsym();
        }
        memcpy(nxtlev,fsys,sizeof(bool) * symnum);
        nxtlev[breaksym] = true;
        nxtlev[semicolon] = true;
        statement(nxtlev,ptx,lev,fbreak,fcont);
    }
    else if (sym == dosym) {
        getsym();
        cx1 = cx;   /*do代码块起始指令地址*/
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        memcpy(nxtbreak,fbreak,sizeof(int) * breakmax);
        memcpy(nxtcont,fcont ,sizeof(int) * breakmax);
        nxtbreak[0] = 0;
        nxtcont[0] = 0;
		nxtlev[continuesym] = true;
		nxtlev[breaksym] = true;
        nxtlev[whilesym] = true;	/* 后继符号为while */
        statement(nxtlev,ptx,lev,nxtbreak,nxtcont);
        if (sym == semicolon) {
            getsym();
        }
        if (sym != whilesym) {
            error(50); /*缺少while*/
        }
        else {
            getsym();
        }
        cx3 = cx;
        condition(nxtlev,ptx,lev);
        cx2 = cx;
        gen(jpc,0,0);
        //cx4 = cx;
        gen(jmp,0,cx1);
        code[cx2].a = cx;
        if (nxtbreak[0] != 0) {
            int j = 0;
            for (j = 1; j <= nxtbreak[0] ; ++j) {
                code[nxtbreak[j]].a = cx;
            }
        }
        if (nxtcont[0] != 0) {
            int j = 0;
            for (j = 1; j <= nxtcont[0]; ++j) {
                code[nxtcont[j]].a = cx3;
            }
        }
    }
    else if (sym == breaksym) {
        if (fbreak[0] == -1) {
            error(41);/*当前代码块不应出现break*/
            getsym();
        }
        else if (fbreak[0] >= breakmax-1) {
            error(46);/*当前循环或switch层break语句数量太多*/
            getsym();
        }
        else {
            cx1 = cx;
            gen(jmp,0,0);
            fbreak[0] ++;
            fbreak[fbreak[0]] = cx1;
            getsym();
        }
    }
    else if (sym == continuesym) {
        if (fcont[0] == -1) {
            error(42); /*当前代码块不应出现continue*/
            getsym();
        }
        else if (fcont[0] >= breakmax-1) {
            error(47);/*当前循环continue语句数量太多*/
            getsym();
        }
        else {
            cx1 = cx;
            gen(jmp,0,0);
            fcont[0]++;
            fcont[fcont[0]] = cx1;
            getsym();
        }
    }
    else if (sym == exitsym) {
        if (iniexit[0] >= breakmax-1){
            error(60);/*exit语句太多*/
        }
        iniexit[0]+=1;
        iniexit[iniexit[0]] = cx;
        gen(jmp,0,0);
        getsym();
    }
    memset(nxtlev, 0, sizeof(bool) * symnum);	/* 语句结束无补救集合 */
    test(fsys, nxtlev, 19);	/* 检测语句结束的正确性 */
}

/*
 * 表达式处理
 */
void expression(bool* fsys, int* ptx, int lev)
{
    enum symbol addop;	/* 用于保存正负号 */
    bool nxtlev[symnum];

    if(sym == plus || sym == minus)	/* 表达式开头有正负号，此时当前表达式被看作一个正的或负的项 */
    {
        isbool = 0;
        addop = sym;	/* 保存开头的正负号 */
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termmul(nxtlev, ptx, lev);	/* 处理乘除取膜的项 */
        if (addop == minus)
        {
            gen(opr,0,1);	/* 如果开头为负号生成取负指令 */
        }
    }
    else	/* 此时表达式被看作项的按位或 */
    {
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[or] = true;
        termxor(nxtlev, ptx, lev);	/* 处理异或的项 */
    }
    while (sym == or)
    {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[or] = true;
        termxor(nxtlev, ptx, lev);	/* 处理异或项 */
        gen(opr, 0, 17);	/* 生成按位或指令 */
    }
}

/*
 * 项处理
 */

void termxor(bool* fsys, int* ptx, int lev)
{
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[xor] = true;
    termand(nxtlev, ptx, lev);	/* 处理按位与的项 */
    while(sym == xor)
    {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[xor] = true;
        termand(nxtlev, ptx, lev);
        gen(opr, 0, 18);	/* 生成异或指令 */
    }
}

void termand(bool* fsys, int* ptx, int lev)
{
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[and] = true;
    termadd(nxtlev, ptx, lev);	/* 处理加减法的项 */
    while(sym == and)
    {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[and] = true;
        termadd(nxtlev, ptx, lev);
        gen(opr, 0, 19);	/* 生成按位与指令 */
    }
}

void termadd(bool* fsys, int* ptx, int lev)
{
    enum symbol addop;	/* 用于保存正负号 */
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[plus] = true;
    nxtlev[minus] = true;
    termmul(nxtlev, ptx, lev);	/* 处理乘除法和取膜项 */
    while (sym == plus || sym == minus)
    {
        isbool = 0;
        addop = sym;
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termmul(nxtlev, ptx, lev);	/* 处理乘除法和取膜项 */
        if (addop == plus)
        {
            gen(opr, 0, 2);	/* 生成加法指令 */
        }
        else
        {
            gen(opr, 0, 3);	/* 生成减法指令 */
        }
    }
}


void termmul(bool* fsys, int* ptx, int lev)
{
    enum symbol mulop;	/* 用于保存乘除法和取膜符号 */
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[times] = true;
    nxtlev[slash] = true;
    nxtlev[mod] = true;
    termnot(nxtlev, ptx, lev);	/* 处理因子 */
    while(sym == times || sym == slash || sym == mod)
    {
        mulop = sym;
        isbool = 0;
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[times] = true;
        nxtlev[slash] = true;
        nxtlev[mod] = true;
        termnot(nxtlev, ptx, lev);
        if(mulop == times)
        {
            gen(opr, 0, 4);	/* 生成乘法指令 */
        }
        else if (mulop == slash)
        {
            gen(opr, 0, 5);	/* 生成除法指令 */
        }
        else
        {
            gen(opr,0,7);   /*生成取膜指令*/
        }
    }
}

void termnot(bool* fsys, int* ptx, int lev)
{
    //enum symbol incop;	/* 用于保存自增自减和按位取非符号 */
    bool nxtlev[symnum];
    if (sym == not) {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        nxtlev[number] = true;
        nxtlev[lparen] = true;
        nxtlev[truesym] = true;
        nxtlev[falsesym] = true;
        factor(nxtlev, ptx, lev);	/* 处理因子 */
        gen(opr,0,22);
    }
    else {
        factor(fsys,ptx,lev);/*处理项*/
    }
    /*while(sym == inc || sym == dec|| sym == not)
    {
        incop = sym;
        //getsym();
        //factor(nxtlev, ptx, lev);
        if(incop == inc)
        {
            gen(opr, 0, 20);	生成自增指令
        }
        else if (incop == dec)
        {
            gen(opr, 0, 21);	 生成自减指令
        }
        else
        {
            gen(opr,0,22);   生成按位取非指令
        }
    }*/
}

/*
 * 因子处理
 */
void factor(bool* fsys, int* ptx, int lev)
{
    int i;
    bool nxtlev[symnum];
    test(facbegsys, fsys, 24);	/* 检测因子的开始符号 */
    while(inset(sym, facbegsys)) 	/* 循环处理因子 */
    {
        if(sym == ident)	/* 因子为常量或变量 */
        {
            i = position(id, *ptx);	/* 查找标识符在符号表中的位置 */
            if (i == 0)
            {
                error(11);	/* 标识符未声明 */
            }
            else
            {
                switch (table[i].kind)
                {
                case constant:	/* 标识符为常量 */
                    gen(lit, 0, table[i].val);	/* 直接把常量的值入栈 */
                    if (table[i].type == 1)
                        isbool = 0;
                    break;
                case variable:	/* 标识符为变量 */
                    gen(lod, lev-table[i].level, table[i].adr);	/* 找到变量地址并将其值入栈 */
                    if (table[i].type == 1)
                        isbool = 0;
                    break;
                case proc:	/* 标识符为过程 */
                    error(21);	/* 不能为过程 */
                    break;
                }
            }
            getsym();
        }
        else if(sym == number)	/* 因子为数 */
        {
            if (num > amax)
            {
                error(31); /* 数越界 */
                num = 0;
            }
            isbool = 0;
            gen(lit, 0, num);
            getsym();
        }
        else if (sym == truesym || sym == falsesym) {
            gen(lit,0,cur);
            getsym();
        }
        else if (sym == lparen)	/* 因子为表达式 */
        {
            getsym();
            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[rparen] = true;
            expression(nxtlev, ptx, lev);
            if (sym == rparen)
            {
                getsym();
            }
            else
            {
                error(22);	/* 缺少右括号 */
            }
        }
        memset(nxtlev, 0, sizeof(bool) * symnum);
        nxtlev[lparen] = true;
        test(fsys, nxtlev, 23); /* 一个因子处理完毕，遇到的单词应在fsys集合中 */
        /* 如果不是，报错并找到下一个因子的开始，使语法分析可以继续运行下去 */
    }
}

/*
 * 条件处理
 */
void condition(bool* fsys, int* ptx, int lev)
{
    enum symbol relop;
    bool nxtlev[symnum];

    if(sym == oddsym)	/* 准备按照odd运算处理 */
    {
        getsym();
        expression(fsys, ptx, lev);
        gen(opr, 0, 6);	/* 生成odd指令 */
    }
    else
    {
        /* 逻辑表达式处理 */
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[eql] = true;
        nxtlev[neq] = true;
        nxtlev[lss] = true;
        nxtlev[leq] = true;
        nxtlev[gtr] = true;
        nxtlev[geq] = true;
        expression(nxtlev, ptx, lev);
        if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq)
        {
            error(20); /* 应该为关系运算符 */
        }
        else
        {
            relop = sym;
            getsym();
            expression(fsys, ptx, lev);
            switch (relop)
            {
            case eql:
                gen(opr, 0, 8);
                break;
            case neq:
                gen(opr, 0, 9);
                break;
            case lss:
                gen(opr, 0, 10);
                break;
            case geq:
                gen(opr, 0, 11);
                break;
            case gtr:
                gen(opr, 0, 12);
                break;
            case leq:
                gen(opr, 0, 13);
                break;
            default:
                break;
            }
        }
    }
}

/*
 * 解释程序
 */
void interpret()
{
    int p = 0; /* 指令指针 */
    int b = 1; /* 指令基址 */
    int t = 0; /* 栈顶指针 */
    struct instruction i;	/* 存放当前指令 */
    int s[stacksize];	/* 栈 */

    printf("Start small\n");
    fprintf(fresult,"Start small\n");
    s[0] = 0; /* s[0]不用 */
    s[1] = 0; /* 主程序的三个联系单元均置为0 */
    s[2] = 0;
    s[3] = 0;
    do
    {
        i = code[p];	/* 读当前指令 */
        p = p + 1;
        switch (i.f)
        {
        case lit:	/* 将常量a的值取到栈顶 */
            t = t + 1;
            s[t] = i.a;
            break;
        case opr:	/* 数学、逻辑运算 */
            switch (i.a)
            {
            case 0:  /* 函数调用结束后返回 */
                t = b - 1;
                p = s[t + 3];
                b = s[t + 2];
                break;
            case 1: /* 栈顶元素取反 */
                s[t] = - s[t];
                break;
            case 2: /* 次栈顶项加上栈顶项，退两个栈元素，相加值进栈 */
                t = t - 1;
                s[t] = s[t] + s[t + 1];
                break;
            case 3:/* 次栈顶项减去栈顶项 */
                t = t - 1;
                s[t] = s[t] - s[t + 1];
                break;
            case 4:/* 次栈顶项乘以栈顶项 */
                t = t - 1;
                s[t] = s[t] * s[t + 1];
                break;
            case 5:/* 次栈顶项除以栈顶项 */
                t = t - 1;
                s[t] = s[t] / s[t + 1];
                break;
            case 6:/* 栈顶元素的奇偶判断 */
                s[t] = s[t] % 2;
                break;
            case 7:/*次栈顶项膜栈顶项*/
                t = t-1;
                s[t] = s[t] % s[t+1];
                break;
            case 8:/* 次栈顶项与栈顶项是否相等 */
                t = t - 1;
                s[t] = (s[t] == s[t + 1]);
                break;
            case 9:/* 次栈顶项与栈顶项是否不等 */
                t = t - 1;
                s[t] = (s[t] != s[t + 1]);
                break;
            case 10:/* 次栈顶项是否小于栈顶项 */
                t = t - 1;
                s[t] = (s[t] < s[t + 1]);
                break;
            case 11:/* 次栈顶项是否大于等于栈顶项 */
                t = t - 1;
                s[t] = (s[t] >= s[t + 1]);
                break;
            case 12:/* 次栈顶项是否大于栈顶项 */
                t = t - 1;
                s[t] = (s[t] > s[t + 1]);
                break;
            case 13: /* 次栈顶项是否小于等于栈顶项 */
                t = t - 1;
                s[t] = (s[t] <= s[t + 1]);
                break;
            case 14:/* 栈顶值输出 */
                if (i.l == 0) {
                    printf("%d", s[t]);
                    fprintf(fresult, "%d", s[t]);
                }
                else {
                    if (s[t] == 0){
                        printf("false");
                        fprintf(fresult,"false");
                    }
                    else{
                        printf("true");
                        fprintf(fresult,"true");
                    }
                }
                t = t - 1;
                break;
            case 15:/* 输出换行符 */
                printf("\n");
                fprintf(fresult,"\n");
                break;
            case 16:/* 读入一个输入置于栈顶 */
                t = t + 1;
                if (i.l == 0) {
                    printf("请输入一个数字:");
                    fprintf(fresult, "请输入一个数字:");
                    scanf("%d", &(s[t]));
                    fprintf(fresult, "%d\n", s[t]);
                }
                else {
                    printf("请输入true或false(其他输入按false处理）：");
                    fprintf(fresult,"请输入true或false(其他输入按false处理）：");
                    scanf("%s",strtmp);
                    fprintf(fresult, "%s\n", strtmp);
                    if (strcmp(strtmp,"true") == 0) {
                        s[t] = 1;
                    }
                    else s[t] = 0;
                }
                break;
            case 17:/* 次栈顶项按位或栈顶项 */
                t = t - 1;
                s[t] = s[t] | s[t + 1];
                break;
            case 18:/* 次栈顶项按位异或栈顶项 */
                t = t - 1;
                s[t] = s[t] ^ s[t + 1];
                break;
            case 19: /* 次栈顶项按位与栈顶项 */
                t = t - 1;
                s[t] = s[t] & s[t + 1];
                break;
            case 20:/* 栈顶项自增 */
                s[t] = s[t]+1;
                break;
            case 21:/* 栈顶项自减 */
                s[t] = s[t]-1;
                break;
            case 22:/* 栈顶项取非 */
                s[t] = !s[t];
                break;
            case 23:/*针对switch使用的(opr,0,8)，当栈顶和次栈顶相等，则正常运行，否则，将次栈顶重新入栈*/
                t = t-1;
                if (s[t] != s[t+1]) {/*不相等*/
                    t = t+1;
                    s[t] = 0;
                }
                else s[t] = 1;
                break;
            }
            break;
        case lod:	/* 取相对当前过程的数据基地址为a的内存的值到栈顶 */
            t = t + 1;
            s[t] = s[base(i.l,s,b) + i.a];
            break;
        case sto:	/* 栈顶的值存到相对当前过程的数据基地址为a的内存 */
            s[base(i.l, s, b) + i.a] = s[t];
            t = t - 1;
            break;
        case cal:	/* 调用子过程 */
            s[t + 1] = base(i.l, s, b);	/* 将父过程基地址入栈，即建立静态链 */
            s[t + 2] = b;	/* 将本过程基地址入栈，即建立动态链 */
            s[t + 3] = p;	/* 将当前指令指针入栈，即保存返回地址 */
            b = t + 1;	/* 改变基地址指针值为新过程的基地址 */
            p = i.a;	/* 跳转 */
            break;
        case ini:	/* 在数据栈中为被调用的过程开辟a个单元的数据区 */
            t = t + i.a;
            break;
        case jmp:	/* 直接跳转 */
            p = i.a;
            break;
        case jpc:	/* 条件跳转 */
            if (s[t] == 0)
                p = i.a;
            t = t - 1;
            break;
        }
    }
    while (p != 0);
    printf("End small\n");
    fprintf(fresult,"End small\n");
}

/* 通过过程基址求上l层过程的基址 */
int base(int l, int* s, int b)
{
    int b1;
    b1 = b;
    while (l > 0)
    {
        b1 = s[b1];
        l--;
    }
    return b1;
}

