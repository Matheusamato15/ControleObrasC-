//Trabalho Final
//Matheus Amato
//Rodrigo Matheus
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

//CONSTANTES
#define NUMERO_SERVICOS        100
#define NUMERO_OBRAS           5
#define NUMERO_FUNCIONARIOS    10
#define NUMERO_DIAS            7
#define TAM_NOME               30
#define NUMERO_TOTAL_OBRAS     5
#define NUMERO_MAXIMO_HORAS    8  							 //NÚMERO MÁXIMO DE HORAS TRABALHADAS
#define NUMERO_MINIMO_CADASTRO 1                 //ESTA CONSTANTE É USADA EM VÁIAS PARTES ONDE NA CONDIÇÃO ALGO NÃO PODE SER MENOR QUE 1
#define NUMERO_MULTIPLICADOR   4
#define NUMERO_CATEGORIA       4
//FIM CONSTANTES

using namespace std;

//-------------------------------------------STRUCT DAS OBRAS
struct Funcoes
{
	int Funcionario, Dia, Obra;
	float Horas;
};
Funcoes VetorServicos[NUMERO_SERVICOS];
Funcoes Cadastro;
Funcoes Deletar;
//------------------------------------------- FIM STRUCT DA OBRAS

//-------------------------------------------VARIÁVEIS GLOBAIS DOS ARQUIVOS DE CUSTO E PERCENTUAL
float CustoBase;
float Multiplicador[NUMERO_MULTIPLICADOR];
float Acrescimo[NUMERO_DIAS];
int Categoria[NUMERO_CATEGORIA], Dia[NUMERO_DIAS];
//-------------------------------------------FIM VARIÁVEIS GLOBAIS DOS ARQUIVOS DE CUSTO E PERCENTUAL

//-------------------------------------------VARIÁVEIS GLOBAIS DA FUNÇÃO 4
float Matriz           [NUMERO_OBRAS][NUMERO_DIAS+1];					  	//MATRIZ DE HORAS
float MatrizHoras      [NUMERO_OBRAS][NUMERO_DIAS+1];
float SomaTotalPorDia  [NUMERO_DIAS+1]={0,0,0,0,0,0,0,0};
//-------------------------------------------FIM VARIÁVEIS GLOBAIS DA FUNÇÃO 4



//-------------------------------------------FUNÇÃO DO MENU
int menu()
{
	int Opcao;
	
	//Menu de opções
	cout << "### GERÊNCIA DE SERVIÇOS ###" << endl;
	cout << " ---- MENU PRINCIPAL ----" << endl;
	cout << "(1) Cadastrar serviço" << endl;
	cout << "(2) Excluir serviço" << endl;
	cout << "(3) Mostrar serviços" << endl;
	cout << "(4) Consultar horas trabalhadas por obra e dia de serviço" << endl;
	cout << "(5) Consultar horas trabalhadas por funcionário e por obra" << endl;
	cout << "(6) Consultar o custo total de uma obra" << endl;
	cout << "(7) Consulta o custo total da semana, de todas as obras" << endl;
	cout << "(8) Fim" << endl;
	cout << endl << "Opção desejada: ";
		
	cin >> Opcao; fflush(stdin);
	system("cls");
	
	//VALIDA OPÇÃO DIGITADA
	if(Opcao < 1 || Opcao > 8)
	{
		system ("cls");
		cout << "Opção incorreta, tente novamente!" << endl;
		system ("pause>>null");
		system ("cls");
	}
	
	return Opcao;
}

//-------------------------------------------- CARREGAR DADOS NECESSÁRIOS PARA NÃO FICAR PRESO EM UMA FUNÇÃO
void carregardia()
{
	int I, J;
	
	for(I=0; I<= NUMERO_OBRAS-1; I++)
	{
		for(J=0; J<= NUMERO_FUNCIONARIOS; J++)
		{
			MatrizHoras [I][J] = Matriz[I][J];
			SomaTotalPorDia[J]+=Matriz[I][J];
		}
	}
}

void carregarmatriz (int *RetornoQtdServicos)
{
	int I, J, K;
	
	for ( I=0; I <= *RetornoQtdServicos-1; I++)
	{	
		for( J=1; J<= NUMERO_OBRAS; J++)
		{
			for (K=1; K <= NUMERO_DIAS; K++)
			{
				if ( VetorServicos[I].Obra == J && VetorServicos[I].Dia == K)
				{
					MatrizHoras[J-1][K-1] += VetorServicos[I].Horas;             //ESSA LINHA JOGA NA MATRIZ AS HORAS DE ACORDO COM O DIA E OBRA
				}			
			}
		}
	}
}

//--------------------------------------------- FUNÇÃO DE ATUALIZAR O ARQUIVO
void atualizararquivo ( char NomeArquivo[], int *RetornoQtdServicos)
{
	setlocale(LC_ALL, "USA");
	
	FILE *Arquivo;
	int I;
	
	Arquivo = fopen (NomeArquivo, "w");
	
	//ESCRVE NO ARQUIVO O NOVO REGISTRO
	fprintf(Arquivo, " #	FUNCIONARIO	DIA	OBRA	HORAS\n");
	for ( I=0; I<= *RetornoQtdServicos-1; I++)
	{
		if ( VetorServicos[I].Funcionario != 0)
		{
			fprintf (Arquivo, "\t%d", VetorServicos[I].Funcionario);
			fprintf (Arquivo, "\t\t");
			fprintf (Arquivo, "%d",	  VetorServicos[I].Dia);
			fprintf (Arquivo, "\t");
			fprintf (Arquivo, "%d",	  VetorServicos[I].Obra);
			fprintf (Arquivo, "\t");
			fprintf (Arquivo, "%.1f", VetorServicos[I].Horas);
			fprintf (Arquivo, "\n");
		}
	}
	
	fclose (Arquivo);
}

//-------------------------------------------- CARREGA OS DADOS DO ARQUIVO PARA O REGISTRO
int carregarservicos (char NomeArquivo[], FILE *Arquivo)
{
	setlocale(LC_ALL, "USA");
	int	 I=0;
	char C[35];
	
	Arquivo=fopen(NomeArquivo, "r");
	if(Arquivo == NULL)
	{
		cout << "Não foi possivel abrir o arquivo!" << endl;
		exit(1);
	}
	
	while( fscanf (Arquivo, "%d %d %d %f", &VetorServicos[I].Funcionario, &VetorServicos[I].Dia, 
				 &VetorServicos[I].Obra, &VetorServicos[I].Horas) != EOF)	 
	{
		fgets(C, 99, Arquivo);
		I=I+1;;	  														 					  	
  }

	fclose(Arquivo);
	return I;
}

//------------------------------------------- FUNÇÃO DO ÍTEM 1 DO MENU
void cadastrarservico ( char NomeArquivo [], int *RetornoQtdServicos)
{
	setlocale (LC_ALL, "Portuguese");
	FILE *Arquivo;
	char Controle;                          //VARIÁVEL QUE CONTROLA UM NOVO CADASTRO
	int  Flag=0;														//ESTE FLAG DIZ SE O CADASTRO É VÁLIDO OU NÃO
	
	Arquivo = fopen (NomeArquivo, "a");
	
	do
	{
		//----------------------- LEITURA E VALIDAÇÃO DO FUNCIONÁRIO
		cout << "Digite o número do funcionário (Válidos 1-10): ";
		cin  >> Cadastro.Funcionario;
		if (Cadastro.Funcionario >= NUMERO_MINIMO_CADASTRO && Cadastro.Funcionario <= NUMERO_FUNCIONARIOS)
		{
			Flag++;
		}
		else
		{
			cout << "\n\nErro no cadastro do funcionário!\n";  
			system ("pause"); break;
		}
		//----------------------------------------- FIM DA VALIDAÇÃO
		
		//-------------------- LEITURA E VALIDAÇÃO DO DIA DO SERVIÇO	
		cout << "Digite o dia do serviço (Válidos 1-7): ";
		cin  >> Cadastro.Dia;
		
		if (Cadastro.Dia >= NUMERO_MINIMO_CADASTRO && Cadastro.Dia <= NUMERO_DIAS)
		{
			Flag++;
		}
		else
		{
			cout << "\n\nErro no cadastro do dia!\n";  
			system ("pause"); break;
		}
		//----------------------------------------- FIM DA VALIDAÇÃO
		
		//-------------------- LEITURA E VALIDAÇÃO DO NÚMERO DA OBRA
		cout << "Digite o número da obra (Válido 1-5): ";
		cin  >> Cadastro.Obra;
		
		if (Cadastro.Obra >= NUMERO_MINIMO_CADASTRO && Cadastro.Obra <= NUMERO_TOTAL_OBRAS)
		{
			Flag++;
		}
		else
		{
			cout << "\n\nErro no cadastro da obra!\n";  
			system ("pause"); break;
		}
		//----------------------------------------- FIM DA VALIDAÇÃO
		
		//---------------------------- LEITURA E VALIDAÇÃO DAS HORAS
		cout << "Digite a quantidade de horas (Válido 1-8): ";
		cin  >> Cadastro.Horas;
		
		if (Cadastro.Horas >= NUMERO_MINIMO_CADASTRO && Cadastro.Horas <= NUMERO_MAXIMO_HORAS)
		{
			Flag++;
		}
		else
		{
			cout << "\n\nErro no cadastro da obra!\n";  
			system ("pause"); break;
		}
		//----------------------------------------- FIM DA VALIDAÇÃO


		//---------- REGISTRANDO O CADASTRO NO ARQUIVO E NO REGISTRO
		if ( Flag >= 4)
		{
			fprintf(Arquivo, "\n\t%d	", Cadastro.Funcionario);
			fprintf(Arquivo, "\t%d	",   Cadastro.Dia);
			fprintf(Arquivo, "%d	",   Cadastro.Obra);
			fprintf(Arquivo, "%.1f",   Cadastro.Horas);
			
			VetorServicos[*RetornoQtdServicos].Funcionario = Cadastro.Funcionario;
			VetorServicos[*RetornoQtdServicos].Dia         = Cadastro.Dia;
			VetorServicos[*RetornoQtdServicos].Obra        = Cadastro.Obra;
			VetorServicos[*RetornoQtdServicos].Horas       = Cadastro.Horas;
			
			*RetornoQtdServicos = *RetornoQtdServicos+1;   //AUMENTA 1 NA VARIÁVEL QUE CONTROLA O NÚMERO ATUAL DE OBRAS CADASTRADAS
		}
	
		cout << endl << "\nDeseja cadastrar outro serviço? [S]-Sim, [N]-Não\n>>: ";
		cin  >> Controle; fflush (stdin);
		
		Controle=toupper(Controle);                      //COLOCA A VARIÁVEL EM CAIXA ALTA PARA EVITAR PROBLEMAS COM O LAÇO       
		
		
		system("pause");
		system("cls");
	}
	while(Controle != 'N');
	
	system("cls");
	
	cout << endl << "Você retornará ao menu principal..." << endl;
	system("Pause>>null");
	system("cls");
	
	fclose (Arquivo);
}

//------------------------------------------- FUNÇÃO DO ÍTEM 2 DO MENU
void excluirservico ( char NomeArquivo[], int *RetornoQtdServicos)
{
	setlocale (LC_ALL, "USA");
	FILE *Arquivo;
	int I, J, Flag=0;
	char Controle;
	
	do
	{	
		Arquivo=fopen(NomeArquivo, "w");
		//PRINTA NA TELA A TABELA COM OS SERVIÇOS PARA FACILITAR NA EXCLUSÃO
		cout << "# FUNCIONÁRIO\tDIA\tOBRA\n";
		for (I=0; I<=*RetornoQtdServicos; I++)
		{
			if (VetorServicos[I].Funcionario != 0)
				cout << "       "            << VetorServicos[I].Funcionario   << "\t " 
					   << VetorServicos[I].Dia << "\t " << VetorServicos[I].Obra << "\t " << endl;
		}
		
		//----------------------- LEITURA E VALIDAÇÃO DO FUNCIONÁRIO
		cout << "\nDigite o número do funcionário que você deseja excluir o serviço: ";
		cin  >> Deletar.Funcionario;
		if (Deletar.Funcionario >= NUMERO_MINIMO_CADASTRO && Deletar.Funcionario <= NUMERO_FUNCIONARIOS)
		{
			Flag++;
		}
		else
		{
			cout << "\n\nErro na exclusão do funcionário!\n";  
			system ("pause"); break;
		}
		//----------------------------------------- FIM DA VALIDAÇÃO
		
		//------------------ LEITURA E VALIDAÇÃO DO DIA PARA EXCLUIR
		cout << "Digite o dia que você deseja excluir o serviço: ";
		cin  >> Deletar.Dia;
		if (Deletar.Dia >= NUMERO_MINIMO_CADASTRO && Deletar.Dia <= NUMERO_DIAS)
		{
			Flag++;
		}
		else
		{
			cout << "\n\nErro na exclusão do dia!\n";  
			system ("pause"); break;
		}
		//----------------------------------------- FIM DA VALIDAÇÃO
		
		//------------------------------ LEITURA E VALIDAÇÃO DA OBRA
		cout << "Digite a obra que você deseja excluir o serviço: ";
		cin  >> Deletar.Obra;
		if (Deletar.Obra >= NUMERO_MINIMO_CADASTRO && Deletar.Obra <= NUMERO_TOTAL_OBRAS)
		{
			Flag++;
		}
		else
		{
			cout << "\n\nErro na exclusão da obra!\n";  
			system ("pause"); break;
		}
		//----------------------------------------- FIM DA VALIDAÇÃO
		
		system ("cls");

		cout << "# FUNCIONÁRIO\tDIA\tOBRA\n";
		
		for (I=0; I<= *RetornoQtdServicos-1; I++)
		{
			if (VetorServicos[I].Dia         == Deletar.Dia			 		 && 
		      VetorServicos[I].Funcionario == Deletar.Funcionario  &&
					VetorServicos[I].Obra        == Deletar.Obra)
			{	
				for (J=I ; J<= NUMERO_SERVICOS-1; J++)
				{
					VetorServicos[J].Dia 				 = VetorServicos[J+1].Dia;
					VetorServicos[J].Funcionario = VetorServicos[J+1].Funcionario;
					VetorServicos[J].Obra				 = VetorServicos[J+1].Obra;
					VetorServicos[J].Horas 			 = VetorServicos[J+1].Horas;	
					
				}
				
				*RetornoQtdServicos = *RetornoQtdServicos-1;
			}
		}
	
		system ("cls");
		cout << "Deseja excluir outro serviço? [S]-Sim, [N]-Não\n>>: ";
		cin  >> Controle;
		Controle = toupper (Controle);
		
		if (Controle == 'S')
			Flag=0;

		system ("cls");
		
		fclose(Arquivo);
	}
	while(Controle != 'N');

	system("cls");
	
	cout << endl << "Você retornará ao menu principal..." << endl;
	system("Pause>>null");
	system("cls");
}

//------------------------------------------- FUNÇÃO DO ÍTEM 3 DO MENU
void mostrarservicos (int *RetornoQtdServicos)
{
	int I=0;
	setlocale(LC_ALL, "USA");
	
	printf("nº Serv. #  FUNCIONÁRIO\t\tDIA\tOBRA\tHORAS\n");
	for(I=0; I<*RetornoQtdServicos; I++)
	{
		if ( VetorServicos[I].Obra != 0)
		{
			cout << "-" << I;
			printf("	\t%d \t\t %d\t %d\t %.1lf\n",VetorServicos[I].Funcionario, VetorServicos[I].Dia, VetorServicos[I].Obra, VetorServicos[I].Horas);
		}
	}
	
	system("Pause");
	system("cls");
	
	cout << endl << "Você retornará ao menu principal." << endl;
	system("Pause>>null");
	system("cls");
}

//------------------------------------------- FUNÇÃO DO ÍTEM 4 DO MENU
void obradia (char NomeArquivo[], int *RetornoQtdServicos)
{
	setlocale(LC_ALL, "USA");
	FILE *Arquivo;
	int I, J=0, K=0;
	float SomaTotalPorObra [NUMERO_TOTAL_OBRAS]={0,0,0,0,0};
	
	Arquivo=fopen(NomeArquivo, "r");
	
	//ZERANDO OS VALORES DO VETOR PARA EVITAR SOMAR COM VALORES ANTIGOS
	SomaTotalPorDia[0] = 0;
	SomaTotalPorDia[1] = 0;
	SomaTotalPorDia[2] = 0;
	SomaTotalPorDia[3] = 0;
	SomaTotalPorDia[4] = 0;
	SomaTotalPorDia[5] = 0;
	SomaTotalPorDia[6] = 0;
	
	//----------------------------------------------------------------
	//------------------------------------------- CABEÇALHO
	//----------------------------------------------------------------
	cout << "*** HORAS POR OBRA E POR DIA ***" << endl;
	cout << endl << "OBRA #|  Seg     Ter     Qua     Qui     Sex     Sab     Dom  |  TOTAL" << endl;
	cout <<"-------------------------------------------------------------------------------" << endl;

	
	//----------------------------------------------------------------
	//---- ZERA A MATRIZ DAS HORAS
	//----------------------------------------------------------------
	for(I=0; I <= NUMERO_OBRAS-1; I++)
	{
		for (J=0; J<=NUMERO_DIAS-1; J++)
		{
			Matriz[I][J]=0;
		}
	}
	
	//----------------------------------------------------------------
	//SOMATÓRIO DAS HORAS DE TODOS OS DIAS DA SEMANA DE TODAS AS OBRAS
	//----------------------------------------------------------------
	for ( I=0; I <= *RetornoQtdServicos-1; I++)
	{	
		for( J=1; J<= NUMERO_OBRAS; J++)
		{
			for (K=1; K <= NUMERO_DIAS; K++)
			{
				if ( VetorServicos[I].Obra == J && VetorServicos[I].Dia == K)
				{
					SomaTotalPorObra[J-1] += VetorServicos[I].Horas;        //ESSA LINHA SOMA TODAS AS HORAS DE ACORDO COM AS OBRAS E DIAS
					Matriz[J-1][K-1] += VetorServicos[I].Horas;             //ESSA LINHA JOGA NA MATRIZ AS HORAS DE ACORDO COM O DIA E OBRA
				}			
			}
		}
	}
	
	//--------------------------------------------------------------------------
	//COLOCA NA ÚLTIMA COLUNA DA MATRIZ O RESULTADO DA SOMA DOS DIAS DE UMA OBRA
	//--------------------------------------------------------------------------
	for (I=0; I<= NUMERO_OBRAS-1; I++)
	{
		Matriz[I][NUMERO_DIAS] = SomaTotalPorObra[I];
	}
	
	//---------------------------------------------------------------------
	//MATRIZ QUE IMPRIME A QUANTIDADE DE HORAS DE ACORDO COM A OBRA E O DIA
	//---------------------------------------------------------------------
	for (I=0; I <= NUMERO_OBRAS-1; I++)
	{
		cout << " " << I+1;              // ESTA LINHA IMPRIME O NÚMERO DAS OBRAS
		for (J=0; J<= NUMERO_DIAS; J++)
		{
			cout << "\t " << Matriz[I][J];
			
			if ( J==6)
				cout << " ";
		}
		cout <<endl;
	}
	cout << "-------------------------------------------------------------------------------" << endl;
	
	for(I=0; I<= NUMERO_OBRAS-1; I++)
	{
		for(J=0; J<= NUMERO_DIAS; J++)
		{
			MatrizHoras [I][J] =  Matriz[I][J];
			SomaTotalPorDia[J] += Matriz[I][J];
		}
	}
	
	//-------------------------------------------------------------------
	//---------------IMPRIME A SOMA DAS HORAS POR DIA E DE TODAS AS OBRAS
	//-------------------------------------------------------------------
	cout << "TOTAL|  ";
	for(J=0; J<= NUMERO_DIAS; J++)
	{
		cout << SomaTotalPorDia[J] << "\t" << " ";
	}
	cout << endl;
	system("pause");
	system("cls");
	
	cout << endl << "Você retornará ao menu principal..." << endl;
	
	system("Pause>>null");
	system("cls");
	fclose(Arquivo);
}

//------------------------------------------- FUNÇÃO DO ÍTEM 5 DO MENU
void funcobras (char NomeArquivoObras[], int *RetornoQtdServicos)
{
	setlocale(LC_ALL, "Portuguese");
	FILE *Arquivo;
	int I, J=0, K=0;
	float SomaTotalPorObra				[NUMERO_TOTAL_OBRAS] ={0,0,0,0,0};
	float SomaTotalPorFuncionario [NUMERO_FUNCIONARIOS]={0,0,0,0,0,0,0,0,0,0};
	float Matriz [NUMERO_FUNCIONARIOS][NUMERO_OBRAS+1];
	
	Arquivo=fopen(NomeArquivoObras, "r+");
	
	//----------------------------------------------------------------
	//-------------------------------------------- CABEÇALHO
	//----------------------------------------------------------------
	cout << "*** HORAS POR FUNCIONARIO E POR OBRA ***" << endl;
	cout << endl << "FUNC #| 1       2       3       4       5   |  TOTAL" << endl;
	cout <<"-------------------------------------------------------------------------------" << endl;

	for(I=0; I < 10; I++)
	{
		for (J=0; J < 6; J++)
		{
			Matriz[I][J]=0;
		}
	}
	
	//----------------------------------------------------------------
	//SOMATÓRIO DAS HORAS DE TODOS OS DIAS DA SEMANA DE TODAS AS OBRAS
	//----------------------------------------------------------------
	for ( I=0; I < *RetornoQtdServicos-1; I++)
	{	
		for( J=1; J <= NUMERO_FUNCIONARIOS; J++)
		{
			for (K=1; K <= NUMERO_OBRAS; K++)
			{
				if ( VetorServicos[I].Funcionario == J && VetorServicos[I].Obra == K)
				{
					SomaTotalPorFuncionario[J-1] += VetorServicos[I].Horas;         //ESSA LINHA SOMA TODAS AS HORAS DO FUNCIONÁRIO
					Matriz[J-1][K-1] += VetorServicos[I].Horas;                     //ESSA LINHA JOGA NA MATRIZ AS HORAS DE ACORDO COM O DIA E OBRA
				}			
			}
		}
	}
	
	//--------------------------------------------------------------------------
	//COLOCA NA ÚLTIMA COLUNA DA MATRIZ O RESULTADO DA SOMA DOS DIAS DE UMA OBRA
	//--------------------------------------------------------------------------
	for (I=0; I<= NUMERO_FUNCIONARIOS-1; I++)
	{
		Matriz[I][NUMERO_OBRAS] = SomaTotalPorFuncionario[I];
	}
	
	//-----------------------------------------------------------------------------
	//MATRIZ QUE IMPRIME A QUANTIDADE DE HORAS DE ACORDO COM A OBRA E O FUNCIONARIO
	//-----------------------------------------------------------------------------
	for (I=0; I<10; I++)
	{
		cout << " " << I+1;                          // ESTA LINHA IMPRIME O NÚMERO DOS FUNCIONÁRIOS
		for (J=0; J<6; J++)
		{
			cout << "\t" << Matriz[I][J];
			
			if ( J==6)
				cout << " ";
		}
		cout << endl;
	}
	cout <<"-------------------------------------------------------------------------------" << endl;
	
	for(I=0; I<10; I++)
	{
		for(J=0; J<6; J++)
		{
			SomaTotalPorObra[J]+=Matriz[I][J];
		}
	}
	
	cout << "TOTAL|  ";
	for(J=0; J<6; J++)
	{
		cout << SomaTotalPorObra[J] << "\t";
	}

	cout << endl;
	
	system("pause");
	system("cls");
	
	cout << endl << "Você retornará ao menu principal..." << endl;
	
	system("Pause>>null");
	system("cls");
	fclose(Arquivo);
}

//------------------------------------------- TAREFA 2
float totalhoras ( int *RetornoQtdServicos, int *NumeroObra) 
{
	setlocale (LC_ALL, "USA");
	float *HorasCalculadas;
	
	int I;
	float TotalHoras=0;
	
	cout << "Digite o número da obra: ";
	cin  >> *NumeroObra;
	
	if ( *NumeroObra < NUMERO_MINIMO_CADASTRO || *NumeroObra > NUMERO_OBRAS)
	{
		cout << "Você informou uma obra inválida!";
	}
	else
	{
		for ( I=0; I<= *RetornoQtdServicos-1; I++)
		{
			if ( VetorServicos[I].Obra == *NumeroObra)
			{
				TotalHoras += VetorServicos[I].Horas;
			}
		}	
	}

	system ("cls");
	
	return TotalHoras;
}

//------------------------------------------- TAREFA 1
void custotalobra (FILE *ArqCustoBase, FILE *ArquivoPercentual)
{
	setlocale (LC_ALL, "USA");
	
	int I;

	// LER DADOS DO ARQUIVO DE CUSTOS
	fseek (ArqCustoBase, 33, SEEK_CUR);
	fscanf(ArqCustoBase, "%f", &CustoBase);

	fseek (ArqCustoBase, 26, SEEK_CUR);
	while ( fgetc( ArqCustoBase) != EOF)
	{
		fscanf (ArqCustoBase, "%d", &Categoria[0]);	fscanf (ArqCustoBase, "%f", &Multiplicador[0]);
		fscanf (ArqCustoBase, "%d", &Categoria[1]);	fscanf (ArqCustoBase, "%f", &Multiplicador[1]);
		fscanf (ArqCustoBase, "%d", &Categoria[2]);	fscanf (ArqCustoBase, "%f", &Multiplicador[2]);
		fscanf (ArqCustoBase, "%d", &Categoria[3]);	fscanf (ArqCustoBase, "%f", &Multiplicador[3]);
	}
	
	//LER DADOS DO ARQUIVO DE PERCENTUAL
	fseek (ArquivoPercentual, 37, SEEK_CUR);
	while ( fgetc( ArquivoPercentual) != EOF)
	{
		fscanf (ArquivoPercentual, "%d", &Dia[0]);	fscanf (ArquivoPercentual, "%f", &Acrescimo[0]);
		fscanf (ArquivoPercentual, "%d", &Dia[1]);	fscanf (ArquivoPercentual, "%f", &Acrescimo[1]);
		fscanf (ArquivoPercentual, "%d", &Dia[2]);	fscanf (ArquivoPercentual, "%f", &Acrescimo[2]);
		fscanf (ArquivoPercentual, "%d", &Dia[3]);	fscanf (ArquivoPercentual, "%f", &Acrescimo[3]);
		fscanf (ArquivoPercentual, "%d", &Dia[4]);	fscanf (ArquivoPercentual, "%f", &Acrescimo[4]);
		fscanf (ArquivoPercentual, "%d", &Dia[5]);	fscanf (ArquivoPercentual, "%f", &Acrescimo[5]);
		fscanf (ArquivoPercentual, "%d", &Dia[6]);	fscanf (ArquivoPercentual, "%f", &Acrescimo[6]);
	}
	
	fclose (ArqCustoBase);
	fclose(ArquivoPercentual);
}

//------------------------------------------- TAREFA 3
int classobra ( float *HorasObra)
{
	if ( *HorasObra < 50.0)
	{
		return 1;
	}	
	else
	if ( *HorasObra >= 50.0 && *HorasObra < 100.0)
	{
		return 2;
	}
	else
	if ( *HorasObra >= 100.0 && *HorasObra < 250.0)
	{
		return 3;
	}
	else
	if ( *HorasObra >= 250.0)
	{
		return 4;
	}
}

//------------------------------------------- FUNÇÃO DO ÍTEM 6 DO MENU, TAREFA 4
float custobratotal ( int *RetornoQtdServicos, int *NumeroObra, FILE *ArqCustoBase, FILE *ArquivoPercentual)
{
	float PorcentSabado, PorcentDomingo;
	float TotalDeHoras, HorasSab=0, HorasDoming=0;
	int ClassificacaoObra;
	float CustoTotalObra;
	int I;
	
	custotalobra(ArqCustoBase, ArquivoPercentual);
	PorcentSabado  = Acrescimo[5]/100;
	PorcentDomingo = Acrescimo[6]/100;
	
	TotalDeHoras = totalhoras ( *&RetornoQtdServicos, *&NumeroObra);
	ClassificacaoObra = classobra ( &TotalDeHoras);
	
	
	//CÁCULO DO CUSTO TOTAL DA OBRA	
	for ( I=0; I <= *RetornoQtdServicos; I++)
	{
		if ( VetorServicos[I].Obra == *NumeroObra)
			if ( VetorServicos[I].Dia == 6)
			{
				TotalDeHoras -= VetorServicos[I].Horas;
				HorasSab += VetorServicos[I].Horas;
			}
			else 
			if ( VetorServicos[I].Dia == 7)
			{
				TotalDeHoras -= VetorServicos[I].Horas;
				HorasDoming += VetorServicos[I].Horas;
			}
	}
	
	if ( TotalDeHoras == 0)
		CustoTotalObra = 0;
	else
		CustoTotalObra = ( TotalDeHoras * CustoBase ) + (( HorasSab * (CustoBase + (PorcentSabado * CustoBase))) + ( HorasDoming * CustoBase + (PorcentDomingo * CustoBase))  );
	
	if ( ClassificacaoObra == Categoria[0])
	{
		CustoTotalObra *= Multiplicador[0];
	}
	else
	if ( ClassificacaoObra == Categoria[1])
	{
		CustoTotalObra *= Multiplicador[1];
	}
	else
	if ( ClassificacaoObra == Categoria[2])
	{
		CustoTotalObra *= Multiplicador[2];
	}
	else
	if ( ClassificacaoObra == Categoria[3])
	{
		CustoTotalObra *= Multiplicador [3];
	}
	//FIM DO CÁLCULO
	
	cout << "-----------------------------------------------" << endl;
	cout << "------------ Custo total Obra " << *NumeroObra << " ---------------" << endl;
	cout << "-----------------------------------------------" << endl << endl;
	
	cout << "Classificação: " << ClassificacaoObra << endl;
	cout << "Total de horas trabalhadas: " << TotalDeHoras + HorasDoming + HorasSab << endl;
	cout << "Custo total: " << "R$ " << CustoTotalObra << "\n\n";
	
	system ("pause >> null");
	system ("cls");
	cout << endl << "Você retornará ao menu principal..." << endl;
	system ("pause >> null");
	system ("cls");
}

//------------------------------------------- FUNÇÃO DO ÍTEM 7 DO MENU, TAREFA 5
void custototalsemana( int *RetornoQtdServicos, int *NumeroObra, FILE *ArqCustoBase, FILE *ArquivoPercentual)
{
	setlocale(LC_ALL, "USA");
	int  I, J;
	float  Total=0, TotalGeral, NovoCustoBase;
	
	custotalobra(ArqCustoBase, ArquivoPercentual);
	carregarmatriz( RetornoQtdServicos);
	
	//NOVO CUSTO BASE PARA NÃO ALTERAR O CUSTO ORIGINAL
	NovoCustoBase=CustoBase;
	for(I=0; I<5; I++)
	{
		if(MatrizHoras[I][5] != 0)
		{
			NovoCustoBase=NovoCustoBase + 20.0/100.0 * NovoCustoBase;
		}
		
		if(MatrizHoras[I][6] != 0)
		{
			NovoCustoBase=NovoCustoBase + 50.0/100.0 * NovoCustoBase;
		}
		
		for(J=0; J<7; J++)
		{	
			Total=Total+MatrizHoras[I][J];
		}
	}
	
	TotalGeral=Total*NovoCustoBase;
	
	printf( "O total de todas as obras é de: R$ %.2f\n", TotalGeral);
	system ("pause >> null");
	system ("cls");
	cout << endl << "Você retornará ao menu principal..." << endl;
	system ("pause >> null");
	system ("cls");
}

int main ()
{
	FILE *Arquivo, *ArqCustoBase, *ArquivoPercentual;
	char NomeArquivo[TAM_NOME], NomeArquivoCustos[TAM_NOME], NomeArquivoPercentual[TAM_NOME];
	int RetornoMenu, RetornoQtdServicos;
	int NumeroObra;
	float HorasCalculadas;
	
	system("title Trabalho Final");
	setlocale(LC_ALL, "Portuguese");
	
	//CARREGAR SERVIÇOS DO ARQUIVO

	cout << "Digite o nome do arquivo de serviços que será usado no programa (Arquivo Obras): ";
	gets(NomeArquivo); fflush(stdin);
	strcat(NomeArquivo, ".txt");
	RetornoQtdServicos=carregarservicos(NomeArquivo, Arquivo);
	system("cls");
	

	cout << "Digite o nome do arquivo de Custos (Arquivo Custos): ";
	gets (NomeArquivoCustos); fflush(stdin);
	
	strcat (NomeArquivoCustos, ".txt");
	ArqCustoBase = fopen (NomeArquivoCustos, "r");
	
	if (ArqCustoBase == NULL)
	{
		cout << "Erro ao abrir o arquivo!\n";
		exit (1);
	}
	
	system("cls");
	
	cout << "Digite o nome do arquivo de Percentual (Arquivo Percentual): ";
	gets (NomeArquivoPercentual); fflush(stdin);
	
	strcat (NomeArquivoPercentual, ".txt");
	ArquivoPercentual = fopen (NomeArquivoPercentual, "r");
	
	if (ArquivoPercentual == NULL)
	{
		cout << "Erro ao abrir o arquivo!\n";
		exit (1);
	}
	
	
	
	system("cls");
	
	carregarmatriz( &RetornoQtdServicos);
	carregardia();
				
	// FIM CARREGAR SERVIÇOS DO ARQUIVO
	do
	{
		RetornoMenu=menu();
		switch (RetornoMenu)
		{
			case 1 :
				cout << "--------------------------------------------" << endl;
				cout << "	Ambiente 1: Cadastrar serviços." << endl;
				cout << "--------------------------------------------" << endl << endl;
				cadastrarservico(NomeArquivo, &RetornoQtdServicos);
			break;
			
			case 2 :
				cout << "----------------------------------------------" << endl;
				cout << "Ambiente 2: Exclusão de serviços." << endl;
				cout << "----------------------------------------------" << endl << endl;
				excluirservico(NomeArquivo, &RetornoQtdServicos);
			break;
			
			case 3 :
				cout << "----------------------------------------------" << endl;
				cout << "	Ambimente 3: Amostra de serviços." << endl;
				cout << "----------------------------------------------" << endl << endl;
				mostrarservicos(&RetornoQtdServicos);
				break;
				
			case 4 :
				cout << "--------------------------------------------------------------------" << endl;
				cout << "Ambiente 4: Consulta de horas trabalhadas por obra e dia de serviço." << endl;
				cout << "--------------------------------------------------------------------" << endl << endl;	
				obradia(NomeArquivo, &RetornoQtdServicos);
				break;
				
			case 5 : 
				cout << "-----------------------------------------------------------------" << endl;
				cout << "Ambiente 5: Consulta de horas trabalhadas por funcionário e obra." << endl;
				cout << "-----------------------------------------------------------------" << endl << endl;
				funcobras(NomeArquivo, &RetornoQtdServicos);
				break;
				
			case 6 :
				cout << "-----------------------------------------------" << endl;
				cout << "Ambiente 6: Consulta o custo total de uma obra." << endl;
				cout << "-----------------------------------------------" << endl << endl;
				custobratotal ( &RetornoQtdServicos, &NumeroObra, ArqCustoBase, ArquivoPercentual);
				break;
				
			case 7 :
				cout << "----------------------------------------------------------------" << endl;
				cout << "Ambiente 7: Consulta o custo total da semana, de todas as obras." << endl;
				cout << "----------------------------------------------------------------" << endl << endl;
				custototalsemana( &RetornoQtdServicos, &NumeroObra, ArqCustoBase, ArquivoPercentual);
				break;
			case 8 :
				atualizararquivo(NomeArquivo, &RetornoQtdServicos);
				break;
		}
	}
	while(RetornoMenu!= 8);
	system("Pause");
	return 0;
}
