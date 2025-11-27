#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ======================= Config DLL ======================= */
static HMODULE g_hDll = NULL;

/* Convenção de chamada (Windows): __stdcall */
#ifndef CALLCONV
#  define CALLCONV WINAPI
#endif

/* ======================= Assinaturas da DLL ======================= */
typedef int (CALLCONV *AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int (CALLCONV *FechaConexaoImpressora_t)(void);
typedef int (CALLCONV *ImpressaoTexto_t)(const char *, int, int, int);
typedef int (CALLCONV *Corte_t)(int);
typedef int (CALLCONV *ImpressaoQRCode_t)(const char *, int, int);
typedef int (CALLCONV *ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int (CALLCONV *AvancaPapel_t)(int);
typedef int (CALLCONV *AbreGavetaElgin_t)(int, int, int);
typedef int (CALLCONV *AbreGaveta_t)(int, int, int);
typedef int (CALLCONV *SinalSonoro_t)(int, int, int);
typedef int (CALLCONV *ImprimeXMLSAT_t)(const char *, int);
typedef int (CALLCONV *ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int (CALLCONV *InicializaImpressora_t)(void);

/* ======================= Ponteiros ======================= */
static AbreConexaoImpressora_t        AbreConexaoImpressora        = NULL;
static FechaConexaoImpressora_t       FechaConexaoImpressora       = NULL;
static ImpressaoTexto_t               ImpressaoTexto               = NULL;
static Corte_t                        Corte                        = NULL;
static ImpressaoQRCode_t              ImpressaoQRCode              = NULL;
static ImpressaoCodigoBarras_t        ImpressaoCodigoBarras        = NULL;
static AvancaPapel_t                  AvancaPapel                  = NULL;
static AbreGavetaElgin_t              AbreGavetaElgin              = NULL;
static AbreGaveta_t                   AbreGaveta                   = NULL;
static SinalSonoro_t                  SinalSonoro                  = NULL;
static ImprimeXMLSAT_t                ImprimeXMLSAT                = NULL;
static ImprimeXMLCancelamentoSAT_t    ImprimeXMLCancelamentoSAT    = NULL;
static InicializaImpressora_t         InicializaImpressora         = NULL;

/* ======================= Configuração ======================= */
static int   g_tipo      = 1;
static char  g_modelo[64] = "i9";
static char  g_conexao[128] = "USB";
static int   g_parametro = 0;
static int   g_conectada = 0;

/* ======================= Utilidades ======================= */
#define LOAD_FN(h, name)                                                         \
    do {                                                                         \
        name = (name##_t)GetProcAddress((HMODULE)(h), #name);                    \
        if (!(name)) {                                                           \
            fprintf(stderr, "Falha ao resolver símbolo %s (erro=%lu)\n",         \
                    #name, GetLastError());                                      \
            return 0;                                                            \
        }                                                                        \
    } while (0)

static void flush_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ======================= Funções para manipular a DLL ======================= */
static int carregarFuncoes(void)
{
    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    if (!g_hDll) {
        fprintf(stderr, "Erro ao carregar E1_Impressora01.dll (erro=%lu)\n", GetLastError());
        return 0;
    }

    LOAD_FN(g_hDll, AbreConexaoImpressora);
    LOAD_FN(g_hDll, FechaConexaoImpressora);
    LOAD_FN(g_hDll, ImpressaoTexto);
    LOAD_FN(g_hDll, Corte);
    LOAD_FN(g_hDll, ImpressaoQRCode);
    LOAD_FN(g_hDll, ImpressaoCodigoBarras);
    LOAD_FN(g_hDll, AvancaPapel);
    LOAD_FN(g_hDll, AbreGavetaElgin);
    LOAD_FN(g_hDll, AbreGaveta);
    LOAD_FN(g_hDll, SinalSonoro);
    LOAD_FN(g_hDll, ImprimeXMLSAT);
    LOAD_FN(g_hDll, ImprimeXMLCancelamentoSAT);
    LOAD_FN(g_hDll, InicializaImpressora);

    return 1;
}

static void liberarBiblioteca(void)
{
    if (g_hDll) {
        FreeLibrary(g_hDll);
        g_hDll = NULL;
    }
}

/* ======================= Funções a serem implementadas pelos alunos ======================= */

static void exibirMenu(void)
{
    // TODO: implementar exibição do menu principal com as opções de impressão
    printf("1 - Configurar Conexao\n");
    printf("2 - Abrir Conexao\n");
    printf("3 - Impressao Texto\n");
    printf("4 - Impressao QRCode\n");
    printf("5 - Impressao Cod Barras\n");
    printf("6 - Impressao XML SAT\n");
    printf("7 - Impressao XML Canc SAT\n");
    printf("8 - Abrir Gaveta Elgin\n");
    printf("9 - Abrir Gaveta\n");
    printf("10 - Sinal Sonoro\n");
    printf("0 - Fechar Conexao e Sair\n");
}

static void configurarConexao(void)
{
    printf("Digite o tipo de conexao\n");
    scanf("%d", &g_tipo);
    
    printf("Digite o modelo de conexao\n");
    scanf("%s", &g_modelo);
    
    printf("Digite a conexao\n");
    scanf("%s", &g_conexao);
    
    printf("Digite o parametro de conexao\n");
    scanf("%d", &g_parametro);
    
    printf("Dados salvos com sucesso\n");
    
    
}

static void abrirConexao(void)
{
    if(g_conectada == 0){
    	
    	int retorno = AbreConexaoImpressora(g_tipo, g_modelo, g_conexao, g_parametro);
    	
    	if(retorno == 0){
    		printf("Conexao aberta com sucesso!!!\n");
    		g_conectada = 1;
		}else{
			printf("Deu erro. Retorno %d\n", retorno);
		}
    	
	}else{
		printf("Ja tem uma conexao em aberto\n");
	}
}

static void fecharConexao(void)
{ 
    if(g_conectada == 1){
    	int retorno = FechaConexaoImpressora();
    	
    	if(retorno ==0){
        	printf("Conexao fechada\n");
        	g_conectada = 0;
		}else{
			printf("Deu erro. Retorno %d\n", retorno);
		}
	}else{
		printf("A Conexao ja esta fechada\n");
	}
    // TODO: chamar FechaConexaoImpressora e tratar retorno
}

static void imprimirTexto(void)
{   
char texto[512];
    int ret;
   
    if (g_conectada ==0){
    printf("Erro: Conexao nao esta aberta");
    return;
}

    printf("Digite o texto (uma linha): ");
    flush_entrada();
    if (fgets(texto, sizeof(texto), stdin) == NULL) {
printf("entrada vazia.\n");
return;
}

{
size_t n = strcspn(texto, "\r\n");
texto[n];
}

ret = ImpressaoTexto(texto, 0, 0,0);
if (ret < 0){
printf("Erro ImpressaoTexto, Codigo=%d\n", ret);
return;
}

AvancaPapel(3);
Corte(2);
printf("Texto impresso com sucesso.\n");

    // TODO: solicitar texto do usuário e chamar ImpressaoTexto
    // incluir AvancaPapel e Corte no final
}

static void imprimirQRCode(void)
{
    // TODO: solicitar conteúdo do QRCode e chamar ImpressaoQRCode(texto, 6, 4)
    // incluir AvancaPapel e Corte no final
    char dados[512];
    int ret;
    
    if(g_conectada == 0) {
    	printf("Erro: Conexao nao esta aberta. \n");
    	return;
	}

	
	printf("Conteudo do QRCode: ");
	flush_entrada();
	if (fgets(dados, sizeof(dados), stdin) == NULL) {
		printf("Entrada vazia. \n");
		return;
	}
	{
		size_t n = strcspn(dados, "\r\n");
		dados[n] = '\0';
	}
	
	ret = ImpressaoQRCode(dados, 6, 4);
	if(ret != 0) {
		printf("Erro ImpressaoQRCode. Codigo=%d\n", ret);
		return;
	}
	
	AvancaPapel(3);
	Corte(2);
	printf("QRCode impresso com sucesso");
}

static void imprimirCodigoBarras(void)
{  
int ret;
 if (g_conectada == 0) {
   printf("Erro: Conexao nao aberta.\n");
   return;
   }


 ret = ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3);
 if (ret !=0) {
    printf("Erro ImpressãoCodigoBarras. Codigo=%d\n");
    return;
}
 AvancaPapel(3);
 Corte(2);
 printf("Codigo de barras imprimido com sucesso.\n");
    // TODO: usar ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3)
    // incluir AvancaPapel e Corte no final
}

static void imprimirXMLSAT(void)
{
      if(g_conectada == 1){
    	char dados[200] = "path=C:\\Users\\anjos_gomes\\Downloads\\C Aluno\\C Aluno\\XMLSAT.xml";
    	int retorno = ImprimeXMLSAT(dados, 0);
    	
    	if(retorno == 0){
    		printf("XML SAT impresso com sucesso!!!\n");
    		g_conectada = 1;
		}else{
			printf("Deu erro. Retorno %d\n", retorno);
		}
    	
	}else{
		printf("Precisa abrir conexao primeiro\n");
	}
}
	

static void imprimirXMLCancelamentoSAT(void)
{
    // TODO: ler o arquivo ./CANC_SAT.xml e chamar ImprimeXMLCancelamentoSAT
    // incluir AvancaPapel e Corte no final
    
	/*usar assinatura abaixo:
        "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==";
        */
    if (g_conectada == 0) {
        printf("Erro: Conexao nao esta aberta.\n");
        return;
    }
    // Caminho padrão do arquivo (ajuste se precisar)
    const char *xmlPath = "C:\\Users\\anjos_gomes\\Downloads\\C Aluno\\C Aluno\\CANC_SAT.xml";

    // Assinatura fixa informada no comentário
    const char *assinatura =
        "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==";

    int retorno = ImprimeXMLCancelamentoSAT(xmlPath, assinatura, 0);

    if (retorno != 0) {
        printf("Erro ImprimeXMLCancelamentoSAT. Codigo=%d\n", retorno);
        return;
    }

    AvancaPapel(3);
    Corte(2);

    printf("XML de cancelamento SAT impresso com sucesso!\n");

}

static void abrirGavetaElgin(void)
{  
   if(g_conectada == 1){
   
    int retorno = AbreGavetaElgin(1,50,50);
   
    if(retorno == 0){
    printf("Abrir gaveta elgin OK");
   
}
   else{
   printf("Erro ao abrir gaveta elgin. Retorno %d", retorno);
}
}
   else{
   printf("Abra a conexao primeiro");
}
    // TODO: chamar AbreGavetaElgin(1, 50, 50)
}

static void abrirGaveta(void)
{
	if(g_conectada == 1){
   
    int retorno = AbreGaveta(1, 5, 10);
   
    if(retorno == 0){
    printf("Abrir gaveta OK");
   
}
    else{
    printf("Erro ao abrir gaveta. Retorno %d", retorno);
}
}
    else{
    printf("Abra a conexao primeiro");
}
    // TODO: chamar AbreGaveta(1, 5, 10)
}

static void emitirSinalSonoro(void)
{
   int qtd = 4;
   int tempoInicio = 50;
   int tempoFim = 5;
   int retorno = SinalSonoro(qtd, tempoInicio, tempoFim);
}

/* ======================= Função principal ======================= */
int main(void)
{
     if (!carregarFuncoes()) {
        return 1;
    }

    int opcao = -1;

    while (1) {
        printf("\n================ MENU IMPRESSORA ================\n");
        exibirMenu();
        printf("\nEscolha uma opo: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1:
                configurarConexao();
                break;

            case 2:
                abrirConexao();
                break;

            case 3:
                imprimirTexto();
                break;

            case 4:
                imprimirQRCode();
                break;

            case 5:
                imprimirCodigoBarras();
                break;

            case 6:
                imprimirXMLSAT();
                break;

            case 7:
                imprimirXMLCancelamentoSAT();
                break;

            case 8:
                abrirGavetaElgin();
                break;

            case 9:
                abrirGaveta();
                break;

            case 10:
                emitirSinalSonoro();
                break;

            case 0:
                fecharConexao();
                liberarBiblioteca();
                printf("Saindo...\n");
                return 0;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

        printf("\nPressione ENTER para continuar...");
        flush_entrada();
        getchar();
    }

    return 0;
}

