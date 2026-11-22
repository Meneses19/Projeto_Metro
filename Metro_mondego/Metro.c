#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAME 50
#define CODE 10

typedef struct Paragem {
    char nome[NAME];
    char codigo[CODE];
    struct Paragem* prox;
    struct Paragem* ant;
} Paragem;

typedef struct Linha {
    char nome[NAME];
    struct Paragem* inicio;
    struct Paragem* fim;
    struct Linha* prox;
} Linha;

Paragem* array = NULL;
int tamanho = 0;
Linha* listaLinhas = NULL;

void lerTexto(char* destino, int maxTamanho) {
    fflush(stdout);
    if (fgets(destino, maxTamanho, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[--len] = '\0';
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        if (len > 0 && destino[len - 1] == '\r') {
            destino[--len] = '\0';
        }
    } else {
        destino[0] = '\0';
    }
}

void paraMaiusculas(char* texto) {
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = (char)toupper((unsigned char)texto[i]);
    }
}

int lerInteiro() {
    char buffer[32];
    int valor;
    lerTexto(buffer, sizeof(buffer));
    if (sscanf(buffer, "%d", &valor) == 1) {
        return valor;
    }
    return -1;
}

void pausar() {
    char dummy[8];
    printf("\nPressione Enter para voltar ao menu...");
    lerTexto(dummy, sizeof(dummy));
}

void registarParagem(Paragem** array, int* tamanho) {
    int i, j, flag;
    char nomeTemp[NAME];

    printf("Digite o nome da paragem %d: ", *tamanho + 1);
    lerTexto(nomeTemp, NAME);

    if (strlen(nomeTemp) == 0) {
        printf("Nome invalido.\n");
        return;
    }

    (*tamanho)++;
    Paragem* temp = (Paragem*)realloc(*array, (*tamanho) * sizeof(Paragem));
    if (temp == NULL) {
        printf("Erro ao alocar memoria.\n");
        (*tamanho)--;
        return;
    }
    *array = temp;

    strcpy((*array)[*tamanho - 1].nome, nomeTemp);
    (*array)[*tamanho - 1].prox = NULL;
    (*array)[*tamanho - 1].ant = NULL;

    do {
        for (i = 0; i < 4; i++) {
            j = rand() % 36;
            if (j < 10) {
                (*array)[*tamanho - 1].codigo[i] = '0' + j;
            } else {
                (*array)[*tamanho - 1].codigo[i] = 'A' + j - 10;
            }
        }
        (*array)[*tamanho - 1].codigo[4] = '\0';

        flag = 1;
        for (i = 0; i < *tamanho - 1; i++) {
            if (strcmp((*array)[i].codigo, (*array)[*tamanho - 1].codigo) == 0) {
                flag = 0;
                break;
            }
        }
    } while (!flag);

    printf("Paragem registada com o codigo: %s\n", (*array)[*tamanho - 1].codigo);
}

void visualizarParagem(Paragem* array, int tamanho) {
    if (tamanho == 0) {
        printf("Nao ha paragens registadas.\n");
        pausar();
        return;
    }

    printf("\nLista de Paragens:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Nome: %s | Codigo: %s\n", array[i].nome, array[i].codigo);
    }
    pausar();
}

void eliminarParagem() {
    if (tamanho == 0) {
        printf("Nao ha paragens registadas.\n");
        pausar();
        return;
    }

    printf("\nLista de Paragens:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Nome: %s | Codigo: %s\n", array[i].nome, array[i].codigo);
    }

    char codigo[CODE];
    printf("\nDigite o codigo da paragem que deseja eliminar: ");
    lerTexto(codigo, CODE);
    paraMaiusculas(codigo);

    int i;
    for (i = 0; i < tamanho; i++) {
        if (strcmp(array[i].codigo, codigo) == 0) {
            break;
        }
    }

    if (i == tamanho) {
        printf("Nao foi encontrada uma paragem com o codigo fornecido.\n");
    } else {
        for (; i < tamanho - 1; i++) {
            array[i] = array[i + 1];
        }
        tamanho--;
        if (tamanho == 0) {
            free(array);
            array = NULL;
        } else {
            Paragem* temp = (Paragem*)realloc(array, tamanho * sizeof(Paragem));
            if (temp != NULL) {
                array = temp;
            }
        }
        printf("Paragem eliminada com sucesso.\n");
    }

    pausar();
}

void criarLinha(Linha** apontaInicioPtr) {
    char resposta[16];

    if (tamanho == 0) {
        printf("Nao ha paragens registadas.\n");
        pausar();
        return;
    }

    Linha* novaLinha = (Linha*)malloc(sizeof(Linha));
    if (novaLinha == NULL) {
        printf("Erro ao alocar memoria.\n");
        pausar();
        return;
    }

    printf("Digite o nome da linha: ");
    lerTexto(novaLinha->nome, NAME);
    if (strlen(novaLinha->nome) == 0) {
        printf("Nome de linha invalido.\n");
        free(novaLinha);
        pausar();
        return;
    }

    novaLinha->inicio = NULL;
    novaLinha->fim = NULL;
    novaLinha->prox = NULL;

    do {
        printf("\nLista de Paragens:\n");
        for (int i = 0; i < tamanho; i++) {
            printf("Nome: %s | Codigo: %s\n", array[i].nome, array[i].codigo);
        }

        char codigo[CODE];
        printf("\nDigite o codigo da paragem que deseja adicionar a linha '%s': ", novaLinha->nome);
        lerTexto(codigo, CODE);
        paraMaiusculas(codigo);

        int i;
        for (i = 0; i < tamanho; i++) {
            if (strcmp(array[i].codigo, codigo) == 0) {
                break;
            }
        }

        if (i == tamanho) {
            printf("Nao foi encontrada uma paragem com o codigo fornecido.\n");
        } else {
            int jaExiste = 0;
            Paragem* verificar = novaLinha->inicio;
            while (verificar != NULL) {
                if (strcmp(verificar->codigo, codigo) == 0) {
                    jaExiste = 1;
                    break;
                }
                verificar = verificar->prox;
            }

            if (jaExiste) {
                printf("Essa paragem ja foi adicionada a esta linha.\n");
            } else {
                Paragem* novaParagem = (Paragem*)malloc(sizeof(Paragem));
                if (novaParagem != NULL) {
                    *novaParagem = array[i];
                    novaParagem->prox = NULL;
                    novaParagem->ant = NULL;

                    if (novaLinha->inicio == NULL) {
                        novaLinha->inicio = novaParagem;
                        novaLinha->fim = novaParagem;
                    } else {
                        novaLinha->fim->prox = novaParagem;
                        novaParagem->ant = novaLinha->fim;
                        novaLinha->fim = novaParagem;
                    }

                    printf("\nParagens na linha '%s':\n", novaLinha->nome);
                    Paragem* atual = novaLinha->inicio;
                    while (atual != NULL) {
                        printf("Nome: %s | Codigo: %s\n", atual->nome, atual->codigo);
                        atual = atual->prox;
                    }
                }
            }
        }

        printf("\nDeseja adicionar mais alguma paragem (S/N)? ");
        lerTexto(resposta, sizeof(resposta));
    } while (resposta[0] == 'S' || resposta[0] == 's');

    if (novaLinha->inicio == NULL) {
        free(novaLinha);
        printf("\nNenhuma paragem adicionada. Linha descartada.\n");
    } else {
        if (*apontaInicioPtr == NULL) {
            *apontaInicioPtr = novaLinha;
        } else {
            Linha* atual = *apontaInicioPtr;
            while (atual->prox != NULL) {
                atual = atual->prox;
            }
            atual->prox = novaLinha;
        }
        printf("\nLinha criada com sucesso.\n");
    }

    pausar();
}

void visualizarLinhas(Linha* inicio) {
    if (inicio == NULL) {
        printf("A lista de linhas esta vazia.\n");
        pausar();
        return;
    }

    Linha* atual = inicio;
    while (atual != NULL) {
        printf("\nNome da linha: %s\n", atual->nome);
        printf("Paragens na linha:\n");

        Paragem* paragemAtual = atual->inicio;
        while (paragemAtual != NULL) {
            printf("Nome: %s | Codigo: %s\n", paragemAtual->nome, paragemAtual->codigo);
            paragemAtual = paragemAtual->prox;
        }

        atual = atual->prox;
    }

    pausar();
}

void atualizarLinha(Linha* inicio) {
    if (inicio == NULL) {
        printf("A lista de linhas esta vazia.\n");
        pausar();
        return;
    }

    printf("\nLinhas existentes:\n");
    Linha* temp = inicio;
    while (temp != NULL) {
        printf("- %s\n", temp->nome);
        temp = temp->prox;
    }

    char nomeLinha[NAME];
    printf("\nDigite o nome da linha que deseja atualizar: ");
    lerTexto(nomeLinha, NAME);

    Linha* atual = inicio;
    while (atual != NULL) {
        if (strcmp(atual->nome, nomeLinha) == 0) {
            char novoNome[NAME];
            printf("Digite o novo nome da linha '%s': ", atual->nome);
            lerTexto(novoNome, NAME);
            if (strlen(novoNome) == 0) {
                printf("Nome invalido.\n");
            } else {
                strcpy(atual->nome, novoNome);
                printf("Nome da linha atualizado com sucesso.\n");
            }
            pausar();
            return;
        }
        atual = atual->prox;
    }

    printf("Nao foi encontrada uma linha com o nome fornecido.\n");
    pausar();
}

void calcularPercurso(Paragem* array, int tamanho, Linha* linhas) {
    if (tamanho == 0 || linhas == NULL) {
        printf("Nao existem paragens ou linhas suficientes registadas.\n");
        pausar();
        return;
    }

    printf("\nLista de Paragens:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Nome: %s | Codigo: %s\n", array[i].nome, array[i].codigo);
    }

    char codigoOrigem[CODE];
    char codigoDestino[CODE];
    printf("\nDigite o codigo da paragem de origem: ");
    lerTexto(codigoOrigem, CODE);
    paraMaiusculas(codigoOrigem);

    printf("Digite o codigo da paragem de destino: ");
    lerTexto(codigoDestino, CODE);
    paraMaiusculas(codigoDestino);

    if (strcmp(codigoOrigem, codigoDestino) == 0) {
        printf("A paragem de origem e de destino sao iguais.\n");
        pausar();
        return;
    }

    int existeOrigem = 0;
    int existeDestino = 0;

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(array[i].codigo, codigoOrigem) == 0) {
            existeOrigem = 1;
        }
        if (strcmp(array[i].codigo, codigoDestino) == 0) {
            existeDestino = 1;
        }
    }

    if (!existeOrigem || !existeDestino) {
        printf("Nao foram encontradas paragens com os codigos fornecidos.\n");
        pausar();
        return;
    }

    Linha* linhaAtual = linhas;
    int encontrouLinha = 0;

    printf("\nLinhas disponiveis para o percurso:\n");
    while (linhaAtual != NULL) {
        Paragem* paragemAtual = linhaAtual->inicio;
        Paragem* ptrOrigem = NULL;
        Paragem* ptrDestino = NULL;
        int pos = 0, posOrigem = -1, posDestino = -1;

        while (paragemAtual != NULL) {
            if (strcmp(paragemAtual->codigo, codigoOrigem) == 0) {
                ptrOrigem = paragemAtual;
                posOrigem = pos;
            }
            if (strcmp(paragemAtual->codigo, codigoDestino) == 0) {
                ptrDestino = paragemAtual;
                posDestino = pos;
            }
            paragemAtual = paragemAtual->prox;
            pos++;
        }

        if (ptrOrigem != NULL && ptrDestino != NULL) {
            printf("Linha: %s\n", linhaAtual->nome);
            printf("Percurso: ");
            Paragem* p = ptrOrigem;
            while (p != NULL) {
                printf("%s (%s)", p->nome, p->codigo);
                if (p == ptrDestino) {
                    break;
                }
                printf(" -> ");
                p = (posOrigem <= posDestino) ? p->prox : p->ant;
            }
            printf("\n\n");
            encontrouLinha = 1;
        }

        linhaAtual = linhaAtual->prox;
    }

    if (!encontrouLinha) {
        printf("Nenhuma linha direta encontrada entre essas paragens.\n");
    }

    pausar();
}

void libertarMemoria() {
    free(array);
    array = NULL;

    Linha* linhaAtual = listaLinhas;
    while (linhaAtual != NULL) {
        Paragem* paragemAtual = linhaAtual->inicio;
        while (paragemAtual != NULL) {
            Paragem* tempParagem = paragemAtual;
            paragemAtual = paragemAtual->prox;
            free(tempParagem);
        }
        Linha* tempLinha = linhaAtual;
        linhaAtual = linhaAtual->prox;
        free(tempLinha);
    }
    listaLinhas = NULL;
}

void menu() {
    int opcao, op;

    while (1) {
        if (feof(stdin)) {
            return;
        }

        printf("\n==============================\n");
        printf("Metro Mondego\n");
        printf("==============================\n");
        printf("1- Paragens\n2- Linhas\n3- Calcular Percurso\n4- Sair\n");
        printf("Opcao: ");
        opcao = lerInteiro();

        if (opcao == 1) {
            printf("\n--- Menu Paragens ---\n");
            printf("1- Registar Paragens\n2- Eliminar Paragens\n3- Visualizar Paragens\n4- Voltar Atras\n5- Sair\n");
            printf("Opcao: ");
            op = lerInteiro();

            switch (op) {
                case 1: {
                    char resposta[16];
                    do {
                        printf("\n");
                        registarParagem(&array, &tamanho);
                        printf("Deseja adicionar mais alguma paragem (S/N)? ");
                        lerTexto(resposta, sizeof(resposta));
                    } while (resposta[0] == 'S' || resposta[0] == 's');
                    break;
                }
                case 2:
                    eliminarParagem();
                    break;
                case 3:
                    visualizarParagem(array, tamanho);
                    break;
                case 4:
                    break;
                case 5:
                    return;
                default:
                    printf("Opcao invalida!\n");
                    pausar();
            }
        } else if (opcao == 2) {
            printf("\n--- Menu Linhas ---\n");
            printf("1- Adicionar Linhas\n2- Atualizar Linhas\n3- Visualizar Linhas\n4- Voltar atras\n5- Sair\n");
            printf("Opcao: ");
            op = lerInteiro();

            switch (op) {
                case 1:
                    criarLinha(&listaLinhas);
                    break;
                case 2:
                    atualizarLinha(listaLinhas);
                    break;
                case 3:
                    visualizarLinhas(listaLinhas);
                    break;
                case 4:
                    break;
                case 5:
                    return;
                default:
                    printf("Opcao invalida!\n");
                    pausar();
            }
        } else if (opcao == 3) {
            calcularPercurso(array, tamanho, listaLinhas);
        } else if (opcao == 4) {
            return;
        } else {
            printf("Opcao invalida!\n");
            pausar();
        }
    }
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    srand((unsigned int)time(NULL));
    menu();
    libertarMemoria();
    return 0;
}