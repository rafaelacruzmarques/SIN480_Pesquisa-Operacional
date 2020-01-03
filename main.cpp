#include <iostream>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <ilcplex/ilocplex.h>

 
typedef IloArray<IloNumVarArray> IloNumVarMatrix;
typedef IloArray<IloNumVarMatrix> IloNumVar3Matrix;
 
typedef IloArray<IloNumArray> IloNumMatrix;
typedef IloArray<IloNumMatrix> IloNum3Matrix;
 
using namespace std;
 
vector< int > A;
vector< int > L;
vector< int > H;
 
vector< int > B;
vector< int > C;

vector< vector<int> > Bk;
vector< vector<int> > Ck;

vector< int > S;
vector< int > I;

vector< vector<int> > Si;
vector< vector<int> > Ii;

vector< vector<int> > Hi;

vector< int > cj;
vector< int > mj;
vector< int > qi;


void imprimiDados() {
	printf("\n\n------\n\n");
	printf("\nA - Conjunto das turmas:\n");
	for( auto a : A )
		printf("%d ", a);
	printf("\n");

	printf("\nL - Conjunto dos locais:\n");
	for ( auto l : L ) 
		printf("%d ", l);
	printf("\n");


	printf("\nH - Conjunto dos horarios:\n");
	for( auto h : H) 
		printf("%d ", h);
	printf("\n");

	// B
	printf("\nB c A - Conjunto de turmas que requerem apenas salas:\n");
	for( auto b : B)
		printf("%d ", b);
	printf("\n");

	// C
	printf("\nC c A - Conjunto de turmas que requerem apenas lab:\n");
	for( auto c : C)
		printf("%d ", c);
	printf("\n");

	// Bk
	printf("\nBk c B - Conjuto turmas SALAS de aula horario k existente H: \n");
	for(int k = 0; k < Bk.size(); k++) {
		printf("\tHorario %d: ", k);
	    for ( auto turma : Bk[k] ) {
	       printf( "%d ", turma );
	    }
	    printf( "\n" );
	}
	printf("\n");

	printf("\nCk c C - COnjunto turmas LABORATORIOS horario k existente H: \n");
	for(int k = 0; k < Ck.size(); k++) {
		printf("\tHorario %d: ", k);
	    for ( auto turma : Ck[k] ) {
	       printf( "%d ", turma );
	    }
	    printf( "\n" );
	}
	printf("\n");


	// S
	printf("\nS c L- Conjunto SALAS:\n");
	for( auto s : S)
		printf("%d ", s);
	printf("\n");

	// I
	printf("\nI c L- Conjunto LABORATORIOS:\n");
	for( auto i : I)
		printf("%d ", i);
	printf("\n");


	printf("\nSi c S - Conjunto SALAS suportam alunos tumra i existente B: \n");
	for(int k = 0; k < Si.size(); k++) {
		printf("\tTurma (Sala) %d: ", k);
	    for ( auto sala : Si[k] ) {
	       printf( "%d ", sala );
	    }
	    printf( "\n" );
	}
	printf("\n");

	printf("\nIi c I - Conjunto LABORATORIOS suportam alunos turma i existente C: \n");
	for(int k = 0; k < Ii.size(); k++) {
		printf("\tTurma (Lab) %d: ", k);
	    for ( auto lab : Ii[k] ) {
	       printf( "%d ", lab );
	    }
	    printf( "\n" );
	}
	printf("\n");

	printf("\nHi c H - Horario aulas turma i existente A:\n");
	for(int k = 0; k < Hi.size(); k++) {
		printf("\tTurma %d: ", k);
	    for ( auto horario : Hi[k] ) {
	       printf( "%d ", horario );
	    }
	    printf( "\n" );
	}
	printf("\n");

	printf("\ncj - custo aula local aula j existente L:\n");
	for(int custo : cj)
		printf("%d ", custo);
	printf("\n");

	printf("\nmj - custo utilizacao LABORATORIO j existente I:\n");
	for(int custopc : mj)
		printf("%d ", custopc);
	printf("\n");

	printf("\nqi - quantidade alinos i existente C:\n");
	for(int qtd : qi)
		printf("%d ", qtd);
	printf("\n");	


	printf("\n\n------\n\n");
}

int main(int argc, char *argv[])
{	
	IloEnv env;
	IloModel modelo;
	modelo = IloModel(env);
	
	int qntTurmas;
	int qntHorarios;
	int qntLocais;
	int qntTurmasSala;
	int qntTurmasLab;
	int qntSala;
	int qntLab;
	
	// Leitura do Arquivo
    FILE* fp;
    fp = fopen(argv[1],"r");

	if( fp == NULL)
	{
		printf("Erro ao abrir o arquivo!\n");
		exit(1);    
	}
	

	////////////////LEITURA DOS DADOS DE ENTRADA//////////////////
	/////////////////////////////////////////////////////////////

	//leitura tamanho de A
	fscanf(fp, "%d", &qntTurmas);
	//leitura tamanho de B
	fscanf(fp, "%d", &qntLocais);
	//leitura tamanho de C
	fscanf(fp, "%d", &qntHorarios);
	
	//Conjunto de Turmas A
	for(int i=0;i<qntTurmas;i++)
	{
		int indiceA;
		fscanf(fp, "%d", &indiceA);
		A.push_back(indiceA);
	}
	//Conjunto de Locais L
	//leitura tamanho de L
	for(int i=0;i<qntLocais;i++)
	{
		int indiceL;
		fscanf(fp, "%d", &indiceL);
		L.push_back(indiceL);
	}
	//Conjunto de Horários H
	//leitura tamanho de H
	for(int i=0;i<qntHorarios;i++)
	{
		int indiceH;
		fscanf(fp, "%d", &indiceH);
		H.push_back(indiceH);
	}



	//Conjunto de Turmas B
	//leitura tamanho de B
	fscanf(fp, "%d", &qntTurmasSala);
	for(int i=0;i<qntTurmasSala;i++)
	{
		int indiceB;
		fscanf(fp, "%d", &indiceB);
		B.push_back(indiceB);
	}
	//Conjunto de Turmas C
	//leitura tamanho de C
	fscanf(fp, "%d", &qntTurmasLab);
	for(int i=0;i<qntTurmasLab;i++)
	{
		int indiceC;
		fscanf(fp, "%d", &indiceC);
		C.push_back(indiceC);
	}

	//Conjunto Bk (Turmas que requerem salas no mesmo horário)
	//Conjunto Ck (Turmas que requerem lab no mesmo horário)
	int quantasTurmasSalasEsteHorario;
	int quantasTurmasLaboratorioEsteHorario;	
	for(int i = 0; i < H.size(); i++) 
	{
		vector<int> pushBkHorario;
		vector<int> pushCkHorario;
		//lendo do arquivo
		fscanf(fp, "%d", &quantasTurmasSalasEsteHorario);
    	fscanf(fp, "%d", &quantasTurmasLaboratorioEsteHorario);
		
		for( int j = 0; j < quantasTurmasSalasEsteHorario ; j++) 
		{
			int indiceBk;
			fscanf(fp, "%d", &indiceBk);
			pushBkHorario.push_back(indiceBk);	
		}
		Bk.push_back(pushBkHorario);

		for( int j = 0; j < quantasTurmasLaboratorioEsteHorario ; j++) 
		{
			int indiceCk;
			fscanf(fp, "%d", &indiceCk);
			pushCkHorario.push_back(indiceCk);
		}
		Ck.push_back(pushCkHorario);
	}
	//Conjunto S
	//Leitura tamanho de S
	fscanf(fp, "%d", &qntSala);
	for(int i=0;i<qntSala;i++)
	{
		int indiceS;
		fscanf(fp, "%d", &indiceS);
		S.push_back(indiceS);

	}
	//Conjunto I
	//Leitura tamanho de I
	fscanf(fp, "%d", &qntLab);
	for(int i=0;i<qntLab;i++)
	{
		
		int indiceL;
		fscanf(fp, "%d", &indiceL);
		I.push_back(indiceL);

	}

	//Conjunto Si
	int quantasSalaSuporta;
	for(int i=0; i < B.size(); i++)
	{
		fscanf(fp, "%d", &quantasSalaSuporta);
		

		vector<int> pushSiTurma;
		//lendo do arquivo
		
		for( int j = 0; j < quantasSalaSuporta; j++) 
		{
			int indiceSi;
			fscanf(fp, "%d", &indiceSi);
			pushSiTurma.push_back(indiceSi);
				
		}
		Si.push_back(pushSiTurma);
	}

	//conjunto Ii
	int quantosLabSuporta;
	for(int i = 0; i < C.size(); i++) 
	{	
		fscanf(fp, "%d", &quantosLabSuporta);
		

		vector<int> pushIiTurma;
		
		for( int j = 0; j < quantosLabSuporta ; j++) 
		{
			int indiceIi;
			fscanf(fp, "%d", &indiceIi);
			pushIiTurma.push_back(indiceIi);
			
		}
		Ii.push_back(pushIiTurma);
	}	

	//conjunto Hi
	int quantosHorariosSuporta;
	for (int i = 0; i < A.size(); i++ )
	{
		fscanf(fp, "%d", &quantosHorariosSuporta);
		

		vector<int> pushHiTurma;
		for( int j = 0; j < quantosHorariosSuporta; j++) 
		{
			int indiceHi;
			fscanf(fp, "%d", &indiceHi);
			pushHiTurma.push_back(indiceHi);
			
		}
		Hi.push_back(pushHiTurma);
	}
	
	//conjunto cj
	for(int i=0;i < L.size();i++)
	{
		int indiceCj;
		fscanf(fp, "%d", &indiceCj);
		cj.push_back(indiceCj);
	}
	//conjunto mj
	for(int i=0;i < I.size();i++)
	{
		int indiceMj;
		fscanf(fp, "%d", &indiceMj);
		mj.push_back(indiceMj);
	}
	//conjunto qi
	for(int i=0;i < C.size();i++)
	{
		int indiceQi;
		fscanf(fp, "%d", &indiceQi);
		qi.push_back(indiceQi);
	}

	
	
	//imprimindo os dados
	//matriz Bk e Ck

	imprimiDados();

	///////////DECLARAÇÃO DAS VARIAVEIS DE DECISÃO///////////////
	/////////////////////////////////////////////////////////////
	
	// VARIÁVEL Y
	IloNumVarMatrix y(env, B.size() );
	for ( int i = 0; i < B.size(); i++ ) {
		y[i] = IloNumVarArray(env, Si[i].size(), 0, 1, ILOBOOL);
		
		for( int j = 0; j < Si[i].size(); j++ ) {
			stringstream var;
			var << "y["<<i<<"]["<<j<<"]: ";
			y[i][j].setName(var.str().c_str());
			modelo.add(y[i][j]); 
		}
	}
	
	// VARIÁVEL X
	//int sizeX = B.size();
	IloNumVar3Matrix x(env, B.size());
	for ( int i = 0; i < B.size(); i++ ) {
		//int sizeXi = Hi[i].size();
		x[i] = IloNumVarMatrix(env, Hi[ B[i] ].size());
		
		for ( int k = 0; k < Hi[ B[i] ].size(); k++ ) {
			//int sizeXij = Si[i].size();
			
			x[i][k] = IloNumVarArray(env, Si[i].size(), 0, 1, ILOBOOL);
			
			for( int j = 0; j < Si[i].size(); j++ ) {
			
				stringstream var;
				var << "x["<<i<<"]["<<k<<"]["<<j<<"]: ";
				x[i][k][j].setName(var.str().c_str());
				modelo.add(x[i][k][j]); 
				
			}
		}
	}

	// VARIÁVEL U
	IloNumVarMatrix u(env, C.size() );
	for ( int i = 0; i < C.size(); i++ ) {
		u[i] = IloNumVarArray(env, Ii[i].size(), 0, 1, ILOBOOL);
		
		for( int j = 0; j < Ii[i].size(); j++ ) {
			stringstream var;
			var << "u["<<i<<"]["<<j<<"]: ";
			u[i][j].setName(var.str().c_str());
			modelo.add(u[i][j]); 
		}
	}
	
	// VARIÁVEL T
	IloNumVar3Matrix t(env, C.size());
	for ( int i = 0; i < C.size(); i++ ) {
		//printf("T[%d] = %d\n", i, Hi[ C[i] ].size());
		t[i] = IloNumVarMatrix(env, Hi[ C[i] ].size());
		
		for ( int k = 0; k < Hi[ C[i] ].size(); k++ ) {
			//printf("T[%d][%d] = %d\n", i, k, Ii[i].size());
			t[i][k] = IloNumVarArray(env, Ii[i].size(), 0, 1, ILOBOOL);
		
			for( int j = 0; j < Ii[i].size(); j++ ) {
				stringstream var;
				var << "t["<<i<<"]["<<k<<"]["<<j<<"]: ";
				t[i][k][j].setName(var.str().c_str());
				modelo.add(t[i][k][j]); 
			}
		}
	}
	
	

	////////////////DECLARAÇÃO FUNÇÃO OBJETIVO//////////////////////
	///////////////////////////////////////////////////////////////
	IloExpr fo(env);
	for ( int i = 0; i < B.size(); i++ ) {
		//printf("\n[%d %d %d]\n", B.size(), Hi[ B[i] ].size(), Si[i].size());
		for ( int k = 0; k < Hi[ B[i] ].size(); k++ ) {
			//int horario = Hi[i];
			for( int j = 0; j < Si[i].size(); j++ ) {
				//printf("\n%d %d %d - %d\n", i, k, j, cj[i]);
				 fo += cj[i] * x[i][k][j];
				//fo += x[i][k][j];
			}
		}
	}
	for ( int i = 0; i < C.size(); i++ ) {
		for ( int k = 0; k < Hi[ C[i] ].size(); k++ ) {
			for( int j = 0; j < Ii[i].size(); j++ ) {
				fo += (cj[i] + (qi[i] * mj[j])) * t[i][k][j];
			}
		}
	}
	modelo.add(IloMinimize(env, fo));
	

	
	/////////////DECLARAÇÃO DAS RESTRIÇÕES///////////////////////
	////////////////////////////////////////////////////////////
	// Restrição 2
	for ( int i = 0; i < B.size(); i++ ) {
		for ( int k = 0; k < Hi[ B[i] ].size(); k++ ) {
			IloExpr soma(env);
			for( int j = 0; j < Si[i].size(); j++ ) {
				soma += x[i][k][j];
			}
			IloRange rest(env, 1, soma, 1);
			modelo.add(rest);
		}
	}
	
	
	// Restrição 3
	for ( int i = 0; i < C.size(); i++ ) {
		for ( int k = 0; k < Hi[ C[i] ].size(); k++ ) {
			IloExpr soma(env);
			for( int j = 0; j < Ii[i].size(); j++ ) {
				soma += t[i][k][j];
			}
			IloRange rest(env, 1, soma, 1);
			modelo.add(rest);
		}
	}

	//Restrição 4
	for(int j = 0; j < L.size(); j++) {

		int Bj = -1;
		int Cj = -1;
		// Pega um J valido em B
		for ( int tmp = 0; tmp < B.size(); tmp++) {
			//printf("(%d - %d)", tmp, B[tmp]);
			if ( B[tmp] == j ) {
				Bj = tmp;
				break;
			}
		}

		// Pega um J valido em C
		for( int tmp = 0; tmp < C.size(); tmp++) {
			//printf("(%d - %d)", tmp, C[tmp]);
			if ( C[tmp] == j) {
				Cj = tmp;
				break;
			}
		}

		for(int k = 0; k < H.size(); k++) {
			IloExpr soma(env);

			int BindiceK = -1;
			int CindiceK = -1;


			if ( Bj != -1 ) {
				//printf("\nBusca K [%d] para B em Hi", k);
				for ( int tmpK = 0; tmpK < Hi[Bj].size(); tmpK++ ) {
					if ( Hi[Bj][tmpK] == k ) {
						BindiceK = k;
						break;
					}
				}
			}

			if ( Cj != -1 ) {
				//printf("\nBusca K [%d] para C em Hi", k);
				for ( int tmpK = 0; tmpK < Hi[Cj].size(); tmpK++ ) {
					if ( Hi[Cj][tmpK] == k ) {
						CindiceK = k;
						break;
					}
				}
			}

			if ( Bj != -1 && BindiceK != -1 ) {
				for( int i = 0; i < Bk[ BindiceK ].size(); i++ ) {
				
					soma += x[ i ][ BindiceK ][ Bj ];
				}
			}
			if( Cj != -1 && CindiceK != -1) {
				for( int i = 0; i < Ck[ CindiceK ].size(); i++ ) {
					
					soma += t[ i ][ CindiceK ][ Cj ];
				}
			}
			
			IloRange rest(env, -IloInfinity, soma, 1);
			modelo.add(rest);
		}
	}
	//Restrição 5
	for ( int i = 0; i < B.size(); i++ ) {
		for ( int k = 0; k < Hi[ B[i] ].size(); k++ ) {
			for( int j = 0; j < Si[i].size(); j++ ) {
				IloExpr soma(env);
				soma += x[i][k][j];
				soma -= y[i][j];
				IloRange rest(env, 0, soma, 0);
				modelo.add(rest);
			}
		}
	}
	
	// Restrição 6
	for ( int i = 0; i < C.size(); i++ ) {
		for ( int k = 0; k < Hi[ B[i] ].size(); k++ ) {			
			for( int j = 0; j < Ii[i].size(); j++ ) {
				IloExpr soma(env);
				soma += t[i][k][j];
				soma -= u[i][j];
				IloRange rest(env, 0, soma, 0);
				modelo.add(rest);
			}
		}
	}
	
	
	IloCplex cplex(modelo);
	cplex.exportModel("alocacao.lp");
	cplex.solve();
	
	
	printf("\n\n\n\n\n");
	printf("VARIÁVEL X: \n");
	printf("Turma \tHorário \tLocal \n");
	for ( int i = 0; i < B.size(); i++ ) {
		
		for ( int k = 0; k < Hi[ B[i] ].size(); k++ ) {
			
			for( int j = 0; j < Si[i].size(); j++ ) {
				
				int indiceTurma = B[i];
				int indiceHorario = Hi[ indiceTurma ][k];
				int indiceLocal = S[ Si[i][j] ];

				bool valor = cplex.getValue( x[ i ][ k ][ j ] );
				if ( valor ) {
					printf("%d \t%d \t\t%d\n", indiceTurma, indiceHorario, indiceLocal);
				}
			}
		}
	}
	printf("----------------------------------------------\n");
	printf("VARIÁVEL Y: \n");
	printf("Turma \tLocal \n");
	for ( int i = 0; i < B.size(); i++ ) {
		for( int j = 0; j < Si[i].size(); j++ ) 
		{
			int indiceTurma = B[i];
			int indiceLocal = S[ Si[i][j] ];

			bool valor = cplex.getValue( y[i][j] );
			if ( valor ) {
				printf("%d \t%d\n", indiceTurma, indiceLocal);
			}
			
		}
	}
	printf("----------------------------------------------\n");
	printf("VARIÁVEL T: \n");
	printf("Turma \tHorário \tLocal \n");
	for ( int i = 0; i < C.size(); i++ ) {
		for ( int k = 0; k < Hi[ C[i] ].size(); k++ ) {
			for( int j = 0; j < Ii[i].size(); j++ ) {
				int indiceTurma = C[i];
				int indiceHorario = Hi[ indiceTurma ][k];
				int indiceLocal = I[ Ii[i][j] ];

				bool valor = cplex.getValue( t[i][k][j] );
				if ( valor ) {
					printf("%d \t%d \t\t%d\n", indiceTurma, indiceHorario, indiceLocal);
				}
			}
		}
	}
	printf("----------------------------------------------\n");
	printf("VARIÁVEL U: \n");
	printf("Turma \tLocal \n");
	for ( int i = 0; i < C.size(); i++ ) {
		for( int j = 0; j < Ii[i].size(); j++ ) 
		{
			int indiceTurma = C[i];
			int indiceLocal = I[ Ii[i][j] ];

			bool valor = cplex.getValue( u[i][j] );
			if ( valor ) {
				printf("%d \t%d\n", indiceTurma, indiceLocal);
			}
			
		}
	}
	
}
