/*
 * Autor: Thiago A. M. Monteiro
 * Data: 19/08/07
 * OBS: Pode fazer o que quiser desde que mantenha a autoria. 
 */

#include <stdio.h>
#include <stdlib.h>

#define TAM 20

/* Prototipos de sub-rotinas. */
void ORDENA_CRES (int vet[], int faixa);

/* Prototipos de funcoes */
float MEDIANA (int vet[], int faixa);
int *MODA (int vet[], int faixa);
int CONTA_ELEM (int vet[], int faixa);

int main (void)
{
  int vetor[TAM]; // 80 bytes alocados.
  float mediana;
  register int i; // indexador.
  int qtd, n;
  int *v_moda = NULL;

  
  do
  {
    system ("clear");
    printf ("\nInforme a quantidade de elementos: ");
    scanf  ("%d", &qtd);
    
    if (qtd > TAM)
      printf ("Tamanho maximo permitido = 20.\n");
  } while (qtd > TAM);
  
  
  
  printf ("\nInforme o valor correspondente p\\ cada elemento do vetor: \n\n");
  
  for (i = 0; i < qtd; i++)
  {
    printf ("%d elemento, posicao [%d] = ", i + 1, i);
    scanf  ("%d", &vetor[i]);
  }  

  mediana =  MEDIANA (vetor, qtd);
  
  printf ("\n\n\n\n\nM E D I A N A"); 
  printf ("\n\n\n\nO vetor tem mediana = %.1f\n\n", mediana); 
  
  printf ("\n\nM O D A\n\n");
  
  n = CONTA_ELEM (vetor, qtd);  

  v_moda = MODA (vetor, qtd);
  
  printf ("\n");
  
  for (i = 0; i < n; i++)
    printf ("\t%d", *(v_moda + i) );
    
  printf ("\n\nQuantidade de elementos repetidos = %d\n\n", n);
      
  printf ("\n\n");
  
  return (0);
}


/* Sub-rotina para ordenar elementos de um 
  vetor de forma crescente */
void ORDENA_CRES (int vet[], int faixa)
{
  register int i, j; // indexadores.
  int aux;  // variavel auxiliar.
  
  for (i = 0; i < (faixa - 1); i++)
    for (j = i + 1; j < faixa; j++)
      if (vet[i] > vet[j])
      {
        aux = vet[i];
        vet[i] = vet[j];
        vet[j] = aux;
      }    
}

/* Funcao que retorna o valor da mediana de um
  vetor de numeros de ponto flutuante */

float MEDIANA (int vet[], int faixa)
{
  register int i; /* indexadores com tipo register
                    dao mais ganho de performance no processamento
                    pois sao armazenados em registradores. */
  float m1, m2;

  
  ORDENA_CRES (vet, faixa); // Ordenando conjunto numerico.

  puts ("");  
  for (i = 0; i < faixa; i++)
    printf ("%5d", vet[i]);
  
  
  switch (faixa % 2) // Seletor para calculo da mediana.
  {
    case 0: // Faixa de valores (qtd de elem do vetor) e PAR.
      m1 = vet[faixa / 2 - 1];
      m2 = vet[faixa / 2];     
      m1 += m2;
      return  (m1 / 2);    
    
    case 1: // Faixa de valores do vetor e IMPAR.
      m1 = vet[ (faixa - 1) / 2 ];
      return m1;   
  }  
}

int CONTA_ELEM (int vet[], int faixa)
{
  int x;
  register int i, j;
  int flag;
  
  x = 0;      
  flag = 0;  
  
  for (i = 0, j = i + 1; i < (faixa - 1); i ++, j++)
  {
    if (vet[i] == vet[j])
    {
      if (flag == 0)
      {
        x++;
        flag = 1;
      }
    }
    else    
      flag = 0;
    
  }
  return (x);
} 

int *MODA (int vet[], int faixa)
{
  /*int moda[faixa];  Este vetor usando alocacao estatica e visto como variavel local 
                        pelo compilador e nao pode ser retornado pela funcao pois ao terminar
                        a execucao da mesma ele e destruido. */

  int n, flag;
  int *moda;
  register int i, j, k;
  
  n = CONTA_ELEM (vet, faixa); // Retorna a quantidade de elementos repetidos no vetor.
  
  moda = (int *) malloc (n * sizeof (int) );  /* Este vetor com alocacao dinamica pode
                                                       ser retornado pois ele so e desalocado
                                                       quando se usa o comando free(); */
  
  
  k = flag = 0;
  
  
  for (i = 0, j = i + 1; i < (faixa - 1); i++, j++)
  {
    if (vet[i] == vet[j])
    {
      if (flag == 0)
      {
        moda[k++] = vet[j];
        flag = 1;
      }      
    }
    else
      flag = 0;
  }    
        
  return moda;
}














## reference: https://www.vivaolinux.com.br/script/Estatistica-Moda-e-mediana-de-um-vetor
