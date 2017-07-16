/*
 * small complier program implemented in C
 *
 * The program has been tested on Visual Studio 2010
 *
 * ʹ�÷�����
 * ���к�����smallԴ�����ļ���
 * �ش��Ƿ�������������
 * �ش��Ƿ�������ű�
 * fcode.txt������������
 * foutput.txt���Դ�ļ�������ʾ�⣨���д��͸��ж�Ӧ�����ɴ����׵�ַ�����޴�
 * fresult.txt������н��
 * ftable.txt������ű�
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define bool int
#define true 1
#define false 0

#define norw 29       /* �����ָ��� */
#define txmax 100     /* ���ű����� */
#define nmax 14       /* ���ֵ����λ�� */
#define al 10         /* ��ʶ������󳤶� */
#define maxerr 30     /* ������������� */
#define amax 2048     /* ��ַ�Ͻ�*/
#define levmax 3      /* ����������Ƕ����������*/
#define cxmax 200     /* ��������������� */
#define stacksize 500 /* ����ʱ����ջԪ�����Ϊ500�� */
#define breakmax 10

/* ���� */
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

/* ���ű��е����� */
enum object
{
    constant,
    variable,
    proc,
};

/* ���������ָ�� */
enum fct
{
    lit,     opr,     lod,
    sto,     cal,     ini,
    jmp,     jpc,
};
#define fctnum 8

/* ���������ṹ */
struct instruction
{
    enum fct f; /* ���������ָ�� */
    int l;      /* ���ò���������Ĳ�β� */
    int a;      /* ����f�Ĳ�ͬ����ͬ */
};

char cnstnm[al+1];
char strtmp[al+1];
int lastcase;           /*��һ��case���ɵ�������תָ���ַ���������ָ�*/
int iniexit[breakmax];
int inicont[breakmax];
int inibreak[breakmax];/*��ʼ���ڵ�break��Ӧ������ָ������ַ*/
bool listswitch ;   /* ��ʾ������������ */
bool tableswitch ;  /* ��ʾ���ű���� */
char ch;            /* ��ŵ�ǰ��ȡ���ַ���getch ʹ�� */
enum symbol sym;    /* ��ǰ�ķ��� */
char id[al+1];      /* ��ǰident�������һ���ֽ����ڴ��0 */
int num;            /* ��ǰnumber */
int cur;           /*��ǰboolֵ*/
int cc, ll;         /* getchʹ�õļ�������cc��ʾ��ǰ�ַ�(ch)��λ�� */
int cx;             /* ���������ָ��, ȡֵ��Χ[0, cxmax-1]*/
char line[81];      /* ��ȡ�л����� */
char a[al+1];       /* ��ʱ���ţ������һ���ֽ����ڴ��0 */
struct instruction code[cxmax]; /* ����������������� */
char word[norw][al];        /* ������ */
enum symbol wsym[norw];     /* �����ֶ�Ӧ�ķ���ֵ */
enum symbol ssym[256];      /* ���ַ��ķ���ֵ */
char mnemonic[fctnum][5];   /* ���������ָ������ */
bool declbegsys[symnum];    /* ��ʾ������ʼ�ķ��ż��� */
bool statbegsys[symnum];    /* ��ʾ��俪ʼ�ķ��ż��� */
bool facbegsys[symnum];     /* ��ʾ���ӿ�ʼ�ķ��ż��� */

/* ���ű�ṹ */
struct tablestruct
{
    char name[al];	    /* ���� */
    enum object kind;	/* ���ͣ�const��var��proc */
    int type;           /*�������ͣ�1Ϊint��2Ϊboolean����proc��ʹ��*/
    int val;            /* ��ֵ����constʹ�� */
    bool bl;            /*bool����ֵ����constʹ��*/
    int level;          /* �����㣬��const��ʹ�� */
    int adr;            /* ��ַ����const��ʹ�� */
    int size;           /* ��Ҫ������������ռ�, ��procʹ�� */
};

struct tablestruct table[txmax]; /* ���ű� */

FILE* fin;      /* ����Դ�ļ� */
FILE* ftable;	/* ������ű� */
FILE* fcode;    /* ������������ */
FILE* foutput;  /* ����ļ�������ʾ�⣨���д������ж�Ӧ�����ɴ����׵�ַ�����޴� */
FILE* fresult;  /* ���ִ�н�� */
char fname[81];
int err;        /* ��������� */

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


/* ������ʼ */
int main()
{
    bool nxtlev[symnum];

    printf("Input small file?   ");
    scanf("%s", fname);		/* �����ļ��� */

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

    //printf("List object codes?(Y/N)");	/* �Ƿ������������� */
    //scanf("%s", fname);
    listswitch = 1;//(fname[0]=='y' || fname[0]=='Y');

    //printf("List symbol table?(Y/N)");	/* �Ƿ�������ű� */
    //scanf("%s", fname);
    tableswitch = 1;//(fname[0]=='y' || fname[0]=='Y');

    init();		/* ��ʼ�� */
    err = 0;
    cc = ll = cx = 0;
    ch = ' ';

    getsym();

    addset(nxtlev, declbegsys, statbegsys, symnum);
    nxtlev[period] = true;

    block(0, 0, nxtlev);	/* ����ֳ��� */
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

        listall();	 /* ������д��� */
        fclose(fcode);

        interpret();	/* ���ý���ִ�г��� */
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
 * ��ʼ��
 */
void init()
{
    int i;
    memset(inibreak,-1,sizeof(inibreak));
    memset(inicont,-1,sizeof(inicont));
    memset(iniexit,-1,sizeof(inicont));
    iniexit[0] = 0;
    /* ���õ��ַ����� */
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

    /* ���ñ���������,������ĸ˳�򣬱��ڶ��ֲ��� */
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

    /* ���ñ����ַ��� */
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

    /* ����ָ������ */
    strcpy(&(mnemonic[lit][0]), "lit");
    strcpy(&(mnemonic[opr][0]), "opr");
    strcpy(&(mnemonic[lod][0]), "lod");
    strcpy(&(mnemonic[sto][0]), "sto");
    strcpy(&(mnemonic[cal][0]), "cal");
    strcpy(&(mnemonic[ini][0]), "int");
    strcpy(&(mnemonic[jmp][0]), "jmp");
    strcpy(&(mnemonic[jpc][0]), "jpc");

    /* ���÷��ż� */
    for (i=0; i<symnum; i++)
    {
        declbegsys[i] = false;
        statbegsys[i] = false;
        facbegsys[i] = false;
    }

    /* ����������ʼ���ż� */
    declbegsys[constsym] = true;
    declbegsys[intsym] = true;
    declbegsys[boolsym]  = true;
    declbegsys[procsym] = true;

    /* ������俪ʼ���ż� */
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



    /* �������ӿ�ʼ���ż� */
    facbegsys[ident] = true;
    facbegsys[number] = true;
    facbegsys[lparen] = true;
    facbegsys[truesym] = true;
    facbegsys[falsesym] = true;
}

/*
 * ������ʵ�ּ��ϵļ�������
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
 *	��������ӡ����λ�úʹ������
 */
void error(int n)
{
    char space[81];
    memset(space,32,81);

    space[cc-1]=0; /* ����ʱ��ǰ�����Ѿ����꣬����cc-1 */

    printf("**%s^%d\n", space, n);
    fprintf(foutput,"**%s^%d\n", space, n);

    err = err + 1;
    if (err > maxerr)
    {
        exit(1);
    }
}

/*
 * ���˿ո񣬶�ȡһ���ַ�
 * ÿ�ζ�һ�У�����line��������line��getsymȡ�պ��ٶ�һ��
 * ������getsym����
 */
void getch()
{
    if (cc == ll) /* �жϻ��������Ƿ����ַ��������ַ����������һ���ַ����������� */
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
 * �ʷ���������ȡһ������
 */
void getsym()
{
    int i,j,k;

    while (ch == ' ' || ch == 10 || ch == 9)	/* ���˿ո񡢻��к��Ʊ�� */
    {
        getch();
    }
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) /* ��ǰ�ĵ����Ǳ�ʶ�����Ǳ����� */
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
        do      /* ������ǰ�����Ƿ�Ϊ�����֣�ʹ�ö��ַ����� */
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
        if (i-1 > j) /* ��ǰ�ĵ����Ǳ����� */
        {
            sym = wsym[k];
            if(sym == truesym)
                cur = 1;
            else if (sym == falsesym)
                cur = 0;
        }
        else /* ��ǰ�ĵ����Ǳ�ʶ�� */
        {
            sym = ident;
        }
    }
    else if (ch >= '0' && ch <= '9') /* ��ǰ�ĵ��������� */
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
        while (ch >= '0' && ch <= '9');   /* ��ȡ���ֵ�ֵ */
        k--;
        if (k > nmax) /* ����λ��̫�� */
        {
            error(30);
        }
    }
    else if (ch == ':')		/* ��⸳ֵ���� */
    {
        getch();
        if (ch == '=')
        {
            sym = becomes;
            getch();
        }
        else
        {
            sym = colon;	/* ð�� */
        }
    }
    else if (ch == '!') /* ��ⲻ���ڷ��� */
    {
        getch();
        if (ch == '=')
        {
            sym = neq;
            getch();
        }
        else
        {
            sym = not;	/* ȡ�Ƿ��� */
        }
    }
    else if (ch == '<')		/* ���С�ڻ�С�ڵ��ڷ��� */
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
    else if (ch == '>')		/* �����ڻ���ڵ��ڷ��� */
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
    else if (ch == '=')  /*����ж���ȷ���*/
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
    else if (ch == '+')  /*������������*/
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
    else if (ch == '-')  /*����Լ������*/
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
    else if (ch == '/')/*Ԥ����ע��*/
    {
        getch();
        if (ch == '/')//��ע��
        {   getch();
            while (ch != 10){
                getch();
            }
            getch();
            getsym();
        }
        else if (ch == '*')/*��ע��*/
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
        sym = ssym[(int)ch];		/* �����Ų�������������ʱ��ȫ�����յ��ַ����Ŵ��� */
        if (sym != period)
        {
            getch();
        }
    }
}

/*
 * �������������
 *
 * x: instruction.f;
 * y: instruction.l;
 * z: instruction.a;
 */
void gen(enum fct x, int y, int z )
{
    if (cx >= cxmax)
    {
        printf("Program is too long!\n");	/* ���ɵ���������������� */
        exit(1);
    }
    if ( z >= amax)
    {
        printf("Displacement address is too big!\n");	/* ��ַƫ��Խ�� */
        exit(1);
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
}


/*
 * ���Ե�ǰ�����Ƿ�Ϸ�
 *
 * ���﷨�����������ںͳ��ڴ����ò��Ժ���test��
 * ��鵱ǰ���ʽ�����˳����﷨��λ�ĺϷ���
 *
 * s1:	��Ҫ�ĵ��ʼ���
 * s2:	���������Ҫ�ĵ��ʣ���ĳһ����״̬ʱ��
 *      �ɻָ��﷨�����������������Ĳ��䵥�ʷ��ż���
 * n:  	�����
 */
void test(bool* s1, bool* s2, int n)
{
    if (!inset(sym, s1))
    {
        error(n);
        /* ����ⲻͨ��ʱ����ͣ��ȡ���ţ�ֱ����������Ҫ�ļ��ϻ򲹾ȵļ��� */
        while ((!inset(sym,s1)) && (!inset(sym,s2)))
        {
            getsym();
        }
    }
}

/*
 * �����������
 *
 * lev:    ��ǰ�ֳ������ڲ�
 * tx:     ���ű�ǰβָ��
 * fsys:   ��ǰģ���̷��ż���
 */
void block(int lev, int tx, bool* fsys)
{
    int i;

    int dx;                 /* ��¼���ݷ������Ե�ַ */
    int tx0;                /* ������ʼtx */
    int cx0;                /* ������ʼcx */
    bool nxtlev[symnum];    /* ���¼������Ĳ����У����ż��Ͼ�Ϊֵ�Σ�������ʹ������ʵ�֣�
	                           ���ݽ�������ָ�룬Ϊ��ֹ�¼������ı��ϼ������ļ��ϣ������µĿռ�
	                           ���ݸ��¼�����*/
    int nxtbreak[breakmax];   /**/
    int nxtcont[breakmax];
    dx = 3;                 /* �����ռ����ڴ�ž�̬��SL����̬��DL�ͷ��ص�ַRA  */
    tx0 = tx;		        /* ��¼�����ʶ���ĳ�ʼλ�� */
    table[tx].adr = cx;	    /* ��¼��ǰ�����Ŀ�ʼλ�� */
    gen(jmp, 0, 0);         /* ������תָ���תλ��δ֪��ʱ��0 */
    if (lev > levmax)		/* Ƕ�ײ������� */
    {
        error(32);
    }
    do
    {
        while (sym == constsym)	/* ���������������ţ���ʼ���������� */
        {
            getsym();
            if (sym == intsym)  /*������type=1*/
            {
                getsym();
                do
                {
                    constdeclaration(&tx, lev, &dx,1);	/* dx��ֵ�ᱻconstdeclaration�ı䣬ʹ��ָ�� */
                    while (sym == comma)  /* �������ż������峣�� */
                    {
                        getsym();
                        constdeclaration(&tx, lev, &dx,1);
                    }
                    if (sym == semicolon) /* �����ֺŽ������峣�� */
                    {
                        getsym();
                    }
                    else
                    {
                        error(5);   /* ©���˷ֺ� */
                    }
                }
                while (sym == ident);
            }
            else if (sym == boolsym)  /*bool�ͣ�type = 2*/
            {
                getsym();
                do
                {
                    constdeclaration(&tx, lev, &dx,2);	/* dx��ֵ�ᱻconstdeclaration�ı䣬ʹ��ָ�� */
                    while (sym == comma)  /* �������ż������峣�� */
                    {
                        getsym();
                        constdeclaration(&tx, lev, &dx,2);
                    }
                    if (sym == semicolon) /* �����ֺŽ������峣�� */
                    {
                        getsym();
                    }
                    else
                    {
                        error(5);   /* ©���˷ֺ� */
                    }
                }
                while (sym == ident);
            }
            else
            {
                error(34); /*δ������������*/
            }
        }
        if (sym == intsym)  /* ���������������ţ���ʼ����������� */
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
                    error(5); /* ©���˷ֺ� */
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
                    error(5); /* ©���˷ֺ� */
                }
            }
            while (sym == ident);
        }
        while (sym == procsym) /* ���������������ţ���ʼ����������� */
        {
            getsym();
            if (sym == ident)
            {
                enter(proc, &tx, lev, &dx,0);	/* ��д���ű� */
                getsym();
            }
            else
            {
                error(4);	/* proc��ӦΪ��ʶ�� */
            }
            if (sym == semicolon)
            {
                getsym();
            }
            else
            {
                error(5);	/* ©���˷ֺ� */
            }
            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[semicolon] = true;
            block(lev + 1, tx, nxtlev); /* �ݹ���� */
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
                error(5);	/* ©���˷ֺ� */
            }
        }
        memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        test(nxtlev, declbegsys, 7);
    }while (inset(sym, declbegsys));	/* ֱ��û���������� */
    code[table[tx0].adr].a = cx;	/* ��ǰ�����ɵ���ת������תλ�øĳɵ�ǰλ�� */
    table[tx0].adr = cx;	        /* ��¼��ǰ���̴����ַ */
    table[tx0].size = dx;	        /* ����������ÿ����һ�����������dx����1�����������Ѿ�������dx���ǵ�ǰ�������ݵ�size */
    cx0 = cx;
    gen(ini, 0, dx);	            /* ����ָ���ָ��ִ��ʱ������ջ��Ϊ�����õĹ��̿���dx����Ԫ�������� */
    if (tableswitch)		/* ������ű� */
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
    /* ����̷���Ϊ�ֺŻ�end */
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);	/* ÿ����̷��ż��϶������ϲ��̷��ż��ϣ��Ա㲹�� */
    memcpy(nxtbreak,inibreak,sizeof(int) * breakmax);
    memcpy(nxtcont,inicont,sizeof(int) * breakmax);
    nxtlev[semicolon] = true;
    nxtlev[rbra] = true;
    statement(nxtlev, &tx, lev, nxtbreak, nxtcont);
    gen(opr, 0, 0);	                    /* ÿ�����̳��ڶ�Ҫʹ�õ��ͷ����ݶ�ָ�� */
    memset(nxtlev, 0, sizeof(bool) * symnum);	/* �ֳ���û�в��ȼ��� */
    test(fsys, nxtlev, 8);            	/* ����̷�����ȷ�� */
    listcode(cx0);                      /* ������ֳ������ɵĴ��� */
}

/*
 * �ڷ��ű��м���һ��
 *
 * k:      ��ʶ��������Ϊconst��var��proc
 * ptx:    ���ű�βָ���ָ�룬Ϊ�˿��Ըı���ű�βָ���ֵ
 * lev:    ��ʶ�����ڵĲ��
 * pdx:    dxΪ��ǰӦ����ı�������Ե�ַ�������Ҫ����1
 *
 */
void enter(enum object k, int* ptx,	int lev, int* pdx,int type)
{
    (*ptx)++;
    strcpy(table[(*ptx)].name, id); /* ���ű��name���¼��ʶ�������� */
    table[(*ptx)].kind = k;
    switch (k)
    {
    case constant:	/* ���� */
        if (type == 1)
        {
            if (num > amax)
            {
                error(31);	/* ����Խ�� */
                num = 0;
            }
            table[(*ptx)].type = type;
            table[(*ptx)].val = num; /* �Ǽǳ�����ֵ */
        }
        else if (type == 2)
        {
            table[(*ptx)].val = num; /* �Ǽ�bool������ֵ,תΪ1��0 */
            table[(*ptx)].type = type;
        }
        break;
    case variable:	/* ���� */
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*pdx);
        table[(*ptx)].type = type;
        (*pdx)++;
        break;
    case proc:	/* ���� */
        table[(*ptx)].level = lev;
        break;
    }
}

/*
 * ���ұ�ʶ���ڷ��ű��е�λ�ã���tx��ʼ������ұ�ʶ��
 * �ҵ��򷵻��ڷ��ű��е�λ�ã����򷵻�0
 *
 * id:    Ҫ���ҵ�����
 * tx:    ��ǰ���ű�βָ��
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
 * ������������
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
                error(1);	/* ��=д����:= */
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
            //    error(2);	/* ���������е�=������Ͳ�ƥ�� */
            //}
        }
        else
        {
            error(3);	/* ���������еı�ʶ����Ӧ��= */
        }
    }
    else
    {
        error(4);	/* const��Ӧ�Ǳ�ʶ�� */
    }
}

/*
 * ������������
 */
void vardeclaration(int* ptx,int lev,int* pdx, int type)
{
    if (sym == ident)
    {
        enter(variable, ptx, lev, pdx, type);	// ��д���ű�
        getsym();
    }
    else
    {
        error(4);	/* ����Ӧ�Ǳ�ʶ�� */
    }
}

/*
 * ���Ŀ������嵥
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
 * �������Ŀ�����
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
 * ��䴦��
 */
 /*fbreak* : ����һ�㴫���ģ���ǰѭ����������break��Ӧ������ת�Ƶ�ַ��fbreak[0]Ϊ��ַ��������ǰ�㲻�ܴ���breakʱfbreak[0]Ϊ-1*/
 /*fcont:(��һ�㴫����)��ǰѭ����������continue��Ӧ��������ת�ƴ����ַ����ǰ�㲻�ܴ���continueʱfcont[0] = -1*/
void statement(bool* fsys, int* ptx, int lev, int *fbreak, int *fcont)
{
    int i, cx1, cx2,cx3,cx4;
    bool nxtlev[symnum];
    int nxtbreak[breakmax];
    int nxtcont[breakmax];
    if (sym == ident)	/* ׼�����ո�ֵ���������Լ���䴦�� */
    {
        i = position(id, *ptx);/* ���ұ�ʶ���ڷ��ű��е�λ�� */
        if (i == 0)
        {
            error(11);	/* ��ʶ��δ���� */
        }
        else if(table[i].kind != variable)
        {
            error(12);	/* ��ֵ����У���ֵ���󲿱�ʶ��Ӧ���Ǳ��� */
            i = 0;
        }
        else
        {
            getsym();
            if(sym == becomes)
            {
                getsym();
                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                expression(nxtlev, ptx, lev);	/* ����ֵ�����Ҳ���ʽ */
                if(i != 0)
                {
                    /* expression��ִ��һϵ��ָ������ս�����ᱣ����ջ����ִ��sto������ɸ�ֵ */
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
                error(13);	/* û�м�⵽��ֵ�������Լ����� */
            }
        }
    }
    else if (sym == readsym)	/* ׼������read��䴦�� */
    {
         /* һ��read���ɶ�һ������ */
         getsym();
         if (sym == ident)
         {
             i = position(id, *ptx);	/* ����Ҫ���ı��� */
         }
         else
         {
             i = 0;
         }
         if (i == 0)
         {
             error(35);	/* read��������еı�ʶ��Ӧ�����������ı��� */
         }
         else
         {
             if (table[i].type == 1)
             gen(opr, 0, 16);	/* ��������ָ���ȡֵ��ջ�� */
             else {
                gen(opr,1,16);
             }
             gen(sto, lev-table[i].level, table[i].adr);	/* ��ջ���������������Ԫ�� */
         }
         getsym();
    }
    else if (sym == writesym)	/* ׼������write��䴦�� */
    {
        /* һ��write�����һ��������ֵ */
        isbool = 1;
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        expression(nxtlev, ptx, lev);	/* ���ñ��ʽ���� */
        if (isbool == 0)
            gen(opr, 0, 14);	/* �������ָ����ջ����ֵ */
        else gen(opr,1,14);
        gen(opr, 0, 15);	/* ���ɻ���ָ�� */
        //getsym();
    }
    else if (sym == callsym)	/* ׼������call��䴦�� */
    {
        getsym();
        if (sym != ident)
        {
            error(14);	/* call��ӦΪ��ʶ�� */
        }
        else
        {
            i = position(id, *ptx);
            if (i == 0)
            {
                error(11);	/* ������δ�ҵ� */
            }
            else
            {
                if (table[i].kind == proc)
                {
                    gen(cal, lev-table[i].level, table[i].adr);	/* ����callָ�� */
                }
                else
                {
                    error(15);	/* call���ʶ������ӦΪ���� */
                }
            }
            getsym();
        }
    }
    else if (sym == ifsym)	/* ׼������if��䴦�� */
    {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[thensym] = true;/* ��̷���Ϊthen */
        condition(nxtlev, ptx, lev); /* ������������ */
        if (sym == thensym)
        {
            getsym();
        }
        else
        {
            error(16);	/* ȱ��then */
        }
        cx1 = cx;	/* ���浱ǰָ���ַ */
        gen(jpc, 0, 0);	/* ����������תָ���ת��ַδ֪����ʱд0 */
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[semicolon] = true;
        nxtlev[endsym] = true;
        nxtlev[elsesym] = true;/* ��̷���Ϊend��else��ֺ� */
        statement(nxtlev, ptx, lev, fbreak, fcont);	/* ����then��else��end֮������ */
        code[cx1].a = cx;	/* ��statement�����cxΪthen�����ִ�����λ�ã�������ǰ��δ������ת��ַ����ʱ���л��� */
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
            statement(nxtlev, ptx, lev, fbreak, fcont);	/* ����else��end֮������ */
            getsym();
            if (sym == semicolon) {
                getsym();
            }
            if (sym == endsym) {
                getsym();
            }
            else {
                error(17);  /*ȱ��end*/
            }
        }
    }
    else if (sym == lbra)	/* ׼�����ո�����䴦�� */
    {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[semicolon] = true;
        nxtlev[rbra] = true;	/* ��̷���Ϊ�ֺŻ�end */

        statement(nxtlev, ptx, lev,fbreak,fcont);  /* ��begin��end֮��������з������� */
        /* ���������һ���������俪ʼ����ֺţ���ѭ��������һ����� */
        while (inset(sym, statbegsys) || sym == semicolon)
        {
            if (sym == semicolon)
            {
                getsym();
            }
            else
            {
                error(10);	/* ȱ�ٷֺ� */
            }
            statement(nxtlev, ptx, lev,fbreak,fcont);
        }
        if(sym == rbra)
        {
            getsym();
        }
        else
        {
            error(17);	/* ȱ��end */
        }
    }
    else if (sym == whilesym)	/* ׼������while��䴦�� */
    {
        cx1 = cx;	/* �����ж�����������λ�� */
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        memcpy(nxtbreak,fbreak,sizeof(int) * breakmax);
        memcpy(nxtcont,fcont ,sizeof(int) * breakmax);
        nxtbreak[0] = 0;
        nxtcont[0] = 0;
		nxtlev[continuesym] = true;
		nxtlev[breaksym] = true;
        nxtlev[dosym] = true;	/* ��̷���Ϊdo */
        condition(nxtlev, ptx, lev);	/* ������������ */
        cx2 = cx;	/* ����ѭ����Ľ�������һ��λ�� */
        gen(jpc, 0, 0);	/* ����������ת��������ѭ���ĵ�ַδ֪�����Ϊ0�ȴ����� */
        if (sym == dosym)
        {
            getsym();
        }
        else
        {
            error(18);	/* ȱ��do */
        }
        statement(nxtlev, ptx, lev, nxtbreak, nxtcont);	/* ѭ���� */
        gen(jmp, 0, cx1);	/* ������������תָ���ת��ǰ���ж�����������λ�� */
        code[cx2].a = cx;	/* ��������ѭ���ĵ�ַ */
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
            error(34);/*��ʽ����Ӧ����������*/
        }
        else  {
            getsym();
        }
        if (sym != ident) {
            error(14);  /*for�ĵ�һ����Ӧ���Ǹ�ֵ��䣬��ͷ�Ǳ�ʶ��*/
        }
        else {
            i = position(id,*ptx);
            if (i == 0)
                error(11);/*��ʶ��δ����*/
            else if (table[i].kind != variable) {
                error(12);/*��ֵ����У���ֵ���󲿱�ʶ��Ӧ���Ǳ���*/
                i = 0;
            }
            getsym();
        }
        if (sym == becomes)
            getsym();
        else error(13);/* û�м�⵽��ֵ���� */
        memcpy(nxtlev,fsys,sizeof(bool)*symnum);
        nxtlev[semicolon] = true;
        expression(nxtlev, ptx, lev);	/* ����ֵ�����Ҳ���ʽ */
        if(i != 0)
        {
            /* expression��ִ��һϵ��ָ������ս�����ᱣ����ջ����ִ��sto������ɸ�ֵ */
            gen(sto, lev-table[i].level, table[i].adr);
        }
        //getsym();///***��֪���費��Ҫgetsym����Ҫ����
        if (sym != semicolon) {
            error(10); /*ȱ�ٷֺ�*/
        }///for(i=0;
        else {
            getsym();
        }
        cx1 = cx; /*�����ж���������λ��*/
        memcpy(nxtlev,fsys,sizeof(bool)*symnum);
        memcpy(nxtbreak,fbreak,sizeof(int) * breakmax);
        memcpy(nxtcont,fcont ,sizeof(int) * breakmax);
        nxtbreak[0] = 0;
        nxtcont[0] = 0;
        nxtlev[semicolon] = true;/*��̷���Ϊ�ֺ�*/
        condition(nxtlev,ptx,lev);/**������������*/
        cx2 = cx;   /*������ת��ѭ�����������һ��λ��*/
        gen(jpc,0,0);
        cx3 = cx;   /*��������ת��begin����ĵ�һ�����(���������Լ���ֵ)*/
        gen(jmp,0,0);
        if (sym != semicolon) {
            error(10);/*ȱ�ٷֺ�*/
        }
        else {///for(i = 0; i < n;
            getsym();
        }
        if (sym != ident) {
            error(45);/*for��������Ӧ���������Լ���ֵ���Ա�ʶ����ͷ*/
        }
        else {
            i = position(id, *ptx);
            if (i == 0)
            {
                error(11);/*��ʶ��δ����*/
            }
            else if (table[i].kind != variable) {
                error(12);  /*��ʶ�������Ǳ���*/
                i = 0;
            }
            getsym();
        }
        if (sym != inc && sym != dec && sym != becomes) {
            error(46);/*�����������Լ���ֵ�����*/
        }
        else {
            if (sym == inc || sym == dec) {/*���������Լ�����*/
                if (i != 0) {
                    cx4  = cx;/*�����������ʼλ��*/
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
                    cx4 = cx;/*��ֵ��俪ʼλ��*/
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
            error(33);  /*��ʽ����ȱ��������*/
        }
        else {///for(i = 0; i < n; i++)
            getsym();
        }
        code[cx3].a = cx;/*ѭ���忪ʼλ��*/
        //cx6 = cx;
        memcpy(nxtlev,fsys,sizeof(bool)*symnum);
        nxtlev[continuesym] = true;
        nxtlev[breaksym] = true;
        statement(nxtlev, ptx, lev, nxtbreak, nxtcont);	/* ѭ���� */
        gen(jmp,0,cx4);/*for�������֣���ֵ���������Լ�*/
        code[cx2].a = cx;/*for�����֮��������ʼλ��*/
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
        gen(jmp,0,0);/*��һ��ִ��ʱҪ����ת�������ж���䣬������*/
        getsym();
        cx2 = cx;   /*repeat�������ʼָ���ַ*/
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        memcpy(nxtbreak,fbreak,sizeof(int) * breakmax);
        memcpy(nxtcont,fcont ,sizeof(int) * breakmax);
        nxtbreak[0] = 0;
        nxtcont[0] = 0;
		nxtlev[continuesym] = true;
		nxtlev[breaksym] = true;
        nxtlev[untilsym] = true;	/* ��̷���Ϊuntil */
        statement(nxtlev,ptx,lev,nxtbreak,nxtcont);
        if (sym == semicolon) {
            getsym();
        }
        if (sym != untilsym) {
            error(49); /*ȱ��until*/
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
            error(56);/*ȱ��������*/
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
        //gen(opr,0,23);  /*���switch��Ƶ�ָ������ʽ��ֵ����table[0].val*/
        if (sym != rparen) {
            error(57); /*ȱ��������*/
        }
        else {
            getsym();
        }
        nxtlev[casesym] = true;
        nxtlev[defaultsym] = true;
		//nxtlev[breaksym] = true;
        nxtlev[rbra] = true;	/* ��̷���Ϊcase,default,�Ҵ����� */
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
            error(48);/*case��Ӧ���Ǳ�ʶ��������*/
        }
        else {
            if (lastcase != -1) {
                code[lastcase].a = cx;
            }
            if (sym == ident) {
                i = position(id,*ptx);
                if (i == 0) {
                    error(35);/*��ʶ��δ����*/
                }
                else {
                    if (table[i].kind == variable)
                        gen(lod, lev-table[i].level, table[i].adr);
                    else gen(lit,0,table[i].val);
                    gen(opr,0,23);
                    lastcase = cx;
                    gen(jpc,0,0);/*��ת����һ��case��default��������*/
                    getsym();
                }
            }
            else if (sym == number) {
                if (num > amax)
                {
                    error(31); /* ��Խ�� */
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
            error(54);/*ȱ��ð��*/
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
            error(54);/*ȱ��ð��*/
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
        cx1 = cx;   /*do�������ʼָ���ַ*/
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        memcpy(nxtbreak,fbreak,sizeof(int) * breakmax);
        memcpy(nxtcont,fcont ,sizeof(int) * breakmax);
        nxtbreak[0] = 0;
        nxtcont[0] = 0;
		nxtlev[continuesym] = true;
		nxtlev[breaksym] = true;
        nxtlev[whilesym] = true;	/* ��̷���Ϊwhile */
        statement(nxtlev,ptx,lev,nxtbreak,nxtcont);
        if (sym == semicolon) {
            getsym();
        }
        if (sym != whilesym) {
            error(50); /*ȱ��while*/
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
            error(41);/*��ǰ����鲻Ӧ����break*/
            getsym();
        }
        else if (fbreak[0] >= breakmax-1) {
            error(46);/*��ǰѭ����switch��break�������̫��*/
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
            error(42); /*��ǰ����鲻Ӧ����continue*/
            getsym();
        }
        else if (fcont[0] >= breakmax-1) {
            error(47);/*��ǰѭ��continue�������̫��*/
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
            error(60);/*exit���̫��*/
        }
        iniexit[0]+=1;
        iniexit[iniexit[0]] = cx;
        gen(jmp,0,0);
        getsym();
    }
    memset(nxtlev, 0, sizeof(bool) * symnum);	/* �������޲��ȼ��� */
    test(fsys, nxtlev, 19);	/* �������������ȷ�� */
}

/*
 * ���ʽ����
 */
void expression(bool* fsys, int* ptx, int lev)
{
    enum symbol addop;	/* ���ڱ��������� */
    bool nxtlev[symnum];

    if(sym == plus || sym == minus)	/* ���ʽ��ͷ�������ţ���ʱ��ǰ���ʽ������һ�����Ļ򸺵��� */
    {
        isbool = 0;
        addop = sym;	/* ���濪ͷ�������� */
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termmul(nxtlev, ptx, lev);	/* ����˳�ȡĤ���� */
        if (addop == minus)
        {
            gen(opr,0,1);	/* �����ͷΪ��������ȡ��ָ�� */
        }
    }
    else	/* ��ʱ���ʽ��������İ�λ�� */
    {
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[or] = true;
        termxor(nxtlev, ptx, lev);	/* ���������� */
    }
    while (sym == or)
    {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[or] = true;
        termxor(nxtlev, ptx, lev);	/* ��������� */
        gen(opr, 0, 17);	/* ���ɰ�λ��ָ�� */
    }
}

/*
 * ���
 */

void termxor(bool* fsys, int* ptx, int lev)
{
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[xor] = true;
    termand(nxtlev, ptx, lev);	/* ����λ����� */
    while(sym == xor)
    {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[xor] = true;
        termand(nxtlev, ptx, lev);
        gen(opr, 0, 18);	/* �������ָ�� */
    }
}

void termand(bool* fsys, int* ptx, int lev)
{
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[and] = true;
    termadd(nxtlev, ptx, lev);	/* ����Ӽ������� */
    while(sym == and)
    {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[and] = true;
        termadd(nxtlev, ptx, lev);
        gen(opr, 0, 19);	/* ���ɰ�λ��ָ�� */
    }
}

void termadd(bool* fsys, int* ptx, int lev)
{
    enum symbol addop;	/* ���ڱ��������� */
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[plus] = true;
    nxtlev[minus] = true;
    termmul(nxtlev, ptx, lev);	/* ����˳�����ȡĤ�� */
    while (sym == plus || sym == minus)
    {
        isbool = 0;
        addop = sym;
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termmul(nxtlev, ptx, lev);	/* ����˳�����ȡĤ�� */
        if (addop == plus)
        {
            gen(opr, 0, 2);	/* ���ɼӷ�ָ�� */
        }
        else
        {
            gen(opr, 0, 3);	/* ���ɼ���ָ�� */
        }
    }
}


void termmul(bool* fsys, int* ptx, int lev)
{
    enum symbol mulop;	/* ���ڱ���˳�����ȡĤ���� */
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[times] = true;
    nxtlev[slash] = true;
    nxtlev[mod] = true;
    termnot(nxtlev, ptx, lev);	/* �������� */
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
            gen(opr, 0, 4);	/* ���ɳ˷�ָ�� */
        }
        else if (mulop == slash)
        {
            gen(opr, 0, 5);	/* ���ɳ���ָ�� */
        }
        else
        {
            gen(opr,0,7);   /*����ȡĤָ��*/
        }
    }
}

void termnot(bool* fsys, int* ptx, int lev)
{
    //enum symbol incop;	/* ���ڱ��������Լ��Ͱ�λȡ�Ƿ��� */
    bool nxtlev[symnum];
    if (sym == not) {
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        nxtlev[number] = true;
        nxtlev[lparen] = true;
        nxtlev[truesym] = true;
        nxtlev[falsesym] = true;
        factor(nxtlev, ptx, lev);	/* �������� */
        gen(opr,0,22);
    }
    else {
        factor(fsys,ptx,lev);/*������*/
    }
    /*while(sym == inc || sym == dec|| sym == not)
    {
        incop = sym;
        //getsym();
        //factor(nxtlev, ptx, lev);
        if(incop == inc)
        {
            gen(opr, 0, 20);	��������ָ��
        }
        else if (incop == dec)
        {
            gen(opr, 0, 21);	 �����Լ�ָ��
        }
        else
        {
            gen(opr,0,22);   ���ɰ�λȡ��ָ��
        }
    }*/
}

/*
 * ���Ӵ���
 */
void factor(bool* fsys, int* ptx, int lev)
{
    int i;
    bool nxtlev[symnum];
    test(facbegsys, fsys, 24);	/* ������ӵĿ�ʼ���� */
    while(inset(sym, facbegsys)) 	/* ѭ���������� */
    {
        if(sym == ident)	/* ����Ϊ��������� */
        {
            i = position(id, *ptx);	/* ���ұ�ʶ���ڷ��ű��е�λ�� */
            if (i == 0)
            {
                error(11);	/* ��ʶ��δ���� */
            }
            else
            {
                switch (table[i].kind)
                {
                case constant:	/* ��ʶ��Ϊ���� */
                    gen(lit, 0, table[i].val);	/* ֱ�Ӱѳ�����ֵ��ջ */
                    if (table[i].type == 1)
                        isbool = 0;
                    break;
                case variable:	/* ��ʶ��Ϊ���� */
                    gen(lod, lev-table[i].level, table[i].adr);	/* �ҵ�������ַ������ֵ��ջ */
                    if (table[i].type == 1)
                        isbool = 0;
                    break;
                case proc:	/* ��ʶ��Ϊ���� */
                    error(21);	/* ����Ϊ���� */
                    break;
                }
            }
            getsym();
        }
        else if(sym == number)	/* ����Ϊ�� */
        {
            if (num > amax)
            {
                error(31); /* ��Խ�� */
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
        else if (sym == lparen)	/* ����Ϊ���ʽ */
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
                error(22);	/* ȱ�������� */
            }
        }
        memset(nxtlev, 0, sizeof(bool) * symnum);
        nxtlev[lparen] = true;
        test(fsys, nxtlev, 23); /* һ�����Ӵ�����ϣ������ĵ���Ӧ��fsys������ */
        /* ������ǣ������ҵ���һ�����ӵĿ�ʼ��ʹ�﷨�������Լ���������ȥ */
    }
}

/*
 * ��������
 */
void condition(bool* fsys, int* ptx, int lev)
{
    enum symbol relop;
    bool nxtlev[symnum];

    if(sym == oddsym)	/* ׼������odd���㴦�� */
    {
        getsym();
        expression(fsys, ptx, lev);
        gen(opr, 0, 6);	/* ����oddָ�� */
    }
    else
    {
        /* �߼����ʽ���� */
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
            error(20); /* Ӧ��Ϊ��ϵ����� */
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
 * ���ͳ���
 */
void interpret()
{
    int p = 0; /* ָ��ָ�� */
    int b = 1; /* ָ���ַ */
    int t = 0; /* ջ��ָ�� */
    struct instruction i;	/* ��ŵ�ǰָ�� */
    int s[stacksize];	/* ջ */

    printf("Start small\n");
    fprintf(fresult,"Start small\n");
    s[0] = 0; /* s[0]���� */
    s[1] = 0; /* �������������ϵ��Ԫ����Ϊ0 */
    s[2] = 0;
    s[3] = 0;
    do
    {
        i = code[p];	/* ����ǰָ�� */
        p = p + 1;
        switch (i.f)
        {
        case lit:	/* ������a��ֵȡ��ջ�� */
            t = t + 1;
            s[t] = i.a;
            break;
        case opr:	/* ��ѧ���߼����� */
            switch (i.a)
            {
            case 0:  /* �������ý����󷵻� */
                t = b - 1;
                p = s[t + 3];
                b = s[t + 2];
                break;
            case 1: /* ջ��Ԫ��ȡ�� */
                s[t] = - s[t];
                break;
            case 2: /* ��ջ�������ջ���������ջԪ�أ����ֵ��ջ */
                t = t - 1;
                s[t] = s[t] + s[t + 1];
                break;
            case 3:/* ��ջ�����ȥջ���� */
                t = t - 1;
                s[t] = s[t] - s[t + 1];
                break;
            case 4:/* ��ջ�������ջ���� */
                t = t - 1;
                s[t] = s[t] * s[t + 1];
                break;
            case 5:/* ��ջ�������ջ���� */
                t = t - 1;
                s[t] = s[t] / s[t + 1];
                break;
            case 6:/* ջ��Ԫ�ص���ż�ж� */
                s[t] = s[t] % 2;
                break;
            case 7:/*��ջ����Ĥջ����*/
                t = t-1;
                s[t] = s[t] % s[t+1];
                break;
            case 8:/* ��ջ������ջ�����Ƿ���� */
                t = t - 1;
                s[t] = (s[t] == s[t + 1]);
                break;
            case 9:/* ��ջ������ջ�����Ƿ񲻵� */
                t = t - 1;
                s[t] = (s[t] != s[t + 1]);
                break;
            case 10:/* ��ջ�����Ƿ�С��ջ���� */
                t = t - 1;
                s[t] = (s[t] < s[t + 1]);
                break;
            case 11:/* ��ջ�����Ƿ���ڵ���ջ���� */
                t = t - 1;
                s[t] = (s[t] >= s[t + 1]);
                break;
            case 12:/* ��ջ�����Ƿ����ջ���� */
                t = t - 1;
                s[t] = (s[t] > s[t + 1]);
                break;
            case 13: /* ��ջ�����Ƿ�С�ڵ���ջ���� */
                t = t - 1;
                s[t] = (s[t] <= s[t + 1]);
                break;
            case 14:/* ջ��ֵ��� */
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
            case 15:/* ������з� */
                printf("\n");
                fprintf(fresult,"\n");
                break;
            case 16:/* ����һ����������ջ�� */
                t = t + 1;
                if (i.l == 0) {
                    printf("������һ������:");
                    fprintf(fresult, "������һ������:");
                    scanf("%d", &(s[t]));
                    fprintf(fresult, "%d\n", s[t]);
                }
                else {
                    printf("������true��false(�������밴false������");
                    fprintf(fresult,"������true��false(�������밴false������");
                    scanf("%s",strtmp);
                    fprintf(fresult, "%s\n", strtmp);
                    if (strcmp(strtmp,"true") == 0) {
                        s[t] = 1;
                    }
                    else s[t] = 0;
                }
                break;
            case 17:/* ��ջ���λ��ջ���� */
                t = t - 1;
                s[t] = s[t] | s[t + 1];
                break;
            case 18:/* ��ջ���λ���ջ���� */
                t = t - 1;
                s[t] = s[t] ^ s[t + 1];
                break;
            case 19: /* ��ջ���λ��ջ���� */
                t = t - 1;
                s[t] = s[t] & s[t + 1];
                break;
            case 20:/* ջ�������� */
                s[t] = s[t]+1;
                break;
            case 21:/* ջ�����Լ� */
                s[t] = s[t]-1;
                break;
            case 22:/* ջ����ȡ�� */
                s[t] = !s[t];
                break;
            case 23:/*���switchʹ�õ�(opr,0,8)����ջ���ʹ�ջ����ȣ����������У����򣬽���ջ��������ջ*/
                t = t-1;
                if (s[t] != s[t+1]) {/*�����*/
                    t = t+1;
                    s[t] = 0;
                }
                else s[t] = 1;
                break;
            }
            break;
        case lod:	/* ȡ��Ե�ǰ���̵����ݻ���ַΪa���ڴ��ֵ��ջ�� */
            t = t + 1;
            s[t] = s[base(i.l,s,b) + i.a];
            break;
        case sto:	/* ջ����ֵ�浽��Ե�ǰ���̵����ݻ���ַΪa���ڴ� */
            s[base(i.l, s, b) + i.a] = s[t];
            t = t - 1;
            break;
        case cal:	/* �����ӹ��� */
            s[t + 1] = base(i.l, s, b);	/* �������̻���ַ��ջ����������̬�� */
            s[t + 2] = b;	/* �������̻���ַ��ջ����������̬�� */
            s[t + 3] = p;	/* ����ǰָ��ָ����ջ�������淵�ص�ַ */
            b = t + 1;	/* �ı����ַָ��ֵΪ�¹��̵Ļ���ַ */
            p = i.a;	/* ��ת */
            break;
        case ini:	/* ������ջ��Ϊ�����õĹ��̿���a����Ԫ�������� */
            t = t + i.a;
            break;
        case jmp:	/* ֱ����ת */
            p = i.a;
            break;
        case jpc:	/* ������ת */
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

/* ͨ�����̻�ַ����l����̵Ļ�ַ */
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

