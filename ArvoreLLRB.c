#include <stdio.h>
#include <stdlib.h>
#include "ArvoreLLRB.h" //inclui os Protótipos

#define RED 1 //define as cores
#define BLACK 0

struct NO{
  int info;
  struct NO *esq;
  struct NO *dir;
  int cor; 
};

//programa principal
//ArvLLRB* raiz; //ponteiro para ponteiro
//int insere_ArvLLRB(ArvLLRB* raiz, int valor);
//int x = insere_ArvLLRB(raiz,valor);

//int remove_ArvLLRB(ArvLLRB *raiz, int valor);
//int x = remove_ArvLLRB(raiz,valor);
typedef struct NO* ArvLLRB;

//Protótipos das Funções Auxiliares (Para resolver 'implicit declaration')
int cor(struct NO* H);
void trocaCor(struct NO* H);
struct NO* rotacionaEsquerda(struct NO* A);
struct NO* rotacionaDireita(struct NO* A);
struct NO* move2EsqRED(struct NO* H);
struct NO* move2DirRED(struct NO* H);
struct NO* balancear(struct NO* H);
struct NO* procuraMenor(struct NO* atual);
struct NO* removerMenor(struct NO* H);
struct NO* remove_NO(struct NO* H, int valor);

//Protótipo INSERÇÃO
struct NO* insereNO(struct NO* H, int valor, int *resp);


//Funções auxiliares
//Acessando a cor de um nó
int cor (struct NO* H){
  if(H == NULL)
  return BLACK;
  else 
  return H->cor;
}
//Inverte a cor do pai e de seus filhos
void trocaCor(struct NO* H){
H->cor = !H->cor;
if (H->esq != NULL)
H->esq->cor = !H->esq->cor;
if (H->dir != NULL)
H->dir->cor = !H->dir->cor;
}

//Rotação à Esquerda
struct NO* rotacionaEsquerda(struct NO* A){
  struct NO* B = A->dir;
  A->dir = B-> esq;
  B->esq = A;
  B->cor = A->cor;
  A->cor = RED;
  return B;
}
//Rotação à Direita
struct NO* rotacionaDireita(struct NO* A){
  struct NO* B = A->esq;
  A->esq = B->dir;
  B->dir = A;
  B->cor = A->cor;
  A->cor = RED;
  return B;
}

//Mover um nó vermelho para a esquerda
struct NO* move2EsqRED(struct NO* H){
  trocaCor(H);
  if(cor(H->dir->esq) == RED){
    H->dir = rotacionaDireita(H->dir);
    H = rotacionaEsquerda(H);
    trocaCor(H);
  }
  return H;
}
//Mover um nó vermelho para a direita
struct NO* move2DirRED(struct NO* H){
  trocaCor(H);
  if(cor(H->esq->esq) == RED){
    H = rotacionaDireita(H);
    trocaCor(H);
  }
  return H;
}

//Arrumar o balanceamento
struct NO* balancear(struct NO* H){
  //nó Vermelho é sempre filho à esquerda
  if (cor(H->dir) == RED){
    H = rotacionaEsquerda(H);
  }
  //Filho da direita e neto da esquerda são vermelho:
  if (H->esq != NULL && cor(H->dir) == RED && cor(H->esq->esq) == RED){
    H = rotacionaDireita(H);
  }
  //2 filhos vermelhos: troca cor!
  if(cor(H->esq)==RED && cor(H->dir)==RED){
    trocaCor(H);
  }
  return H;
}


//busca o menor nó
struct NO* procuraMenor(struct NO* atual){
  struct NO *no1 = atual;
  struct NO *no2 = atual->esq;
  while(no2 != NULL){
    no1 = no2;
    no2 = no2->esq;
  }
  return no1;
}
//remove o nó com menor valor
struct NO* removerMenor(struct NO* H){
  if(H->esq == NULL){
    free(H);
    return NULL;
  }
  if(cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK){
    H = move2EsqRED(H);
  }
  H->esq = removerMenor(H->esq);
  return balancear(H);
}

//Função INSERIR
struct NO* insereNO(struct NO* H, int valor, int *resp){
  if(H == NULL){
    struct NO *novo;
    novo = (struct NO*)malloc(sizeof(struct NO));
    if(novo == NULL){
      *resp = 0;
      return NULL;
    }
    novo->info = valor;
    novo->cor = RED;
    novo->dir = NULL;
    novo->esq = NULL;
    *resp = 1;
    return novo;
  }
  if(valor == H->info){
    *resp = 0; //valor duplicado
  }
  else{
    if(valor < H->info){
      H->esq = insereNO(H->esq,valor,resp);
    }
    else {
      H->dir = insereNO(H->dir,valor,resp);
    }
  }
  
  if(cor(H->dir) == RED && cor(H->esq) == BLACK){
    H = rotacionaEsquerda(H);
  }
  if(cor(H->esq) == RED && cor(H->esq->esq) == RED){
    H = rotacionaDireita(H);
  }
  if(cor(H->esq) == RED && cor(H->dir) == RED){
    trocaCor(H);
  }
  return H;
}


//Funções Principais

//Função INSERIR
int insere_ArvLLRB(ArvLLRB* raiz, int valor){
  int resp;
  *raiz = insereNO(*raiz,valor,&resp);
  if((*raiz) != NULL){
    (*raiz)->cor = BLACK;
  }
  return resp;
}

//Função REMOVER NO
struct NO* remove_NO(struct NO* H, int valor){
  if(valor < H->info){
    if(cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK){
      H = move2EsqRED(H);
    }
    H->esq = remove_NO(H->esq, valor);
  } else {
    if(cor(H->esq) == RED){
      H = rotacionaDireita(H);
    }
    if(valor == H->info && (H->dir == NULL)){
      free(H);
      return NULL;
    }
    if(cor(H->dir) == BLACK && cor(H->dir->esq) == BLACK){
      H = move2DirRED(H);
    }
  }
  if(valor == H->info){
    struct NO* x = procuraMenor(H->dir);
    H->info = x->info;
    H->dir = removerMenor(H->dir);
  } else {
    H->dir = remove_NO(H->dir, valor);
  }
  return balancear(H);
}

// Consulta 
int consulta_ArvLLRB(ArvLLRB *raiz, int valor){
    if (raiz == NULL || *raiz == NULL) return 0;
    struct NO* atual = *raiz;
    while(atual != NULL){
        if(valor == atual->info) return 1;
        if(valor < atual->info) atual = atual->esq;
        else atual = atual->dir;
    }
    return 0;
}

//Função REMOVER Principal
int remove_ArvLLRB(ArvLLRB *raiz, int valor){
  if(consulta_ArvLLRB(raiz,valor)){
    struct NO* h = *raiz;
    *raiz = remove_NO(h,valor);
    if(*raiz != NULL){
      (*raiz)->cor = BLACK;
    }
    return 1;
  } else {
    return 0;
  }
}
