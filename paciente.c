#include <stdio.h>  // Biblioteca padrão para entrada e saída
#include <stdlib.h> // Biblioteca padrão para alocação de memória, controle de processos, etc.
#include <string.h> // Biblioteca para manipulação de strings
#include <ctype.h>  // Biblioteca para funções de classificação e conversão de caracteres

// Definição da estrutura para armazenar informações do paciente
typedef struct paciente
{
    char nome[50];           // Nome do paciente
    char cpf[15];            // CPF do paciente
    char dataNascimento[11]; // Data de nascimento do paciente
    char genero;             // Gênero do paciente (M/F)
    char telefone[15];       // Telefone do paciente
    char planoSaude[20];     // Plano de saúde do paciente
    char endereco[100];      // Endereço do paciente
} Paciente;

// Função para limpar o buffer de entrada
void limparBuffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) // Limpa o buffer até encontrar uma nova linha ou o final do arquivo
        ;
}

// Função para ler uma string com uma máscara de formatação
void lerStringComMascara(char *campo, int tamanho, const char *mascara)
{
    int i = 0, j = 0;
    char c;
    while (i < tamanho - 1)
    {
        c = getchar();
        if (c == '\n')
        {
            break;
        }
        if (mascara[j] == 'X')
        {
            if (isdigit(c))
            {
                campo[i++] = c;
            }
        }
        else if (mascara[j] == c)
        {
            campo[i++] = c;
        }
        else if (mascara[j] != 'X' && isdigit(c))
        {
            campo[i++] = mascara[j++];
            campo[i++] = c;
        }
        else
        {
            continue;
        }
        j++;
    }
    campo[i] = '\0';
    limparBuffer();
}

// Função para validar o CPF
int validarCPF(char *cpf)
{
    int i, j, num, soma, digito1, digito2;
    char numeros[12] = {0};

    // Remove caracteres não numéricos do CPF
    for (i = 0, j = 0; i < 14; i++)
    {
        if (isdigit(cpf[i]))
        {
            numeros[j++] = cpf[i];
        }
    }

    // Verifica se o CPF tem 11 dígitos
    if (strlen(numeros) != 11)
        return 0;

    // Cálculo do primeiro dígito verificador
    soma = 0;
    for (i = 0, j = 10; i < 9; i++, j--)
        soma += (numeros[i] - '0') * j;
    num = soma % 11;
    digito1 = (num < 2) ? 0 : 11 - num;

    // Cálculo do segundo dígito verificador
    soma = 0;
    for (i = 0, j = 11; i < 10; i++, j--)
        soma += (numeros[i] - '0') * j;
    num = soma % 11;
    digito2 = (num < 2) ? 0 : 11 - num;

    // Verifica se os dígitos calculados são iguais aos informados
    return (digito1 == (numeros[9] - '0') && digito2 == (numeros[10] - '0'));
}

// Função para validar a data de nascimento
int validarDataNascimento(char *data)
{
    int dia, mes, ano;
    if (strlen(data) != 10 || data[2] != '/' || data[5] != '/')
        return 0;
    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 1900 || ano > 2100)
        return 0;
    // Verificação simplificada para dias do mês e anos bissextos
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30)
        return 0;
    if (mes == 2)
    {
        int bissexto = (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0));
        if (dia > (bissexto ? 29 : 28))
            return 0;
    }
    return 1;
}

// Função para validar o gênero
int validarGenero(char genero)
{
    return (genero == 'M' || genero == 'F' || genero == 'm' || genero == 'f');
}

// Função para validar o telefone
int validarTelefone(char *telefone)
{
    int i;
    int digitos = 0;
    for (i = 0; i < strlen(telefone); i++)
    {
        if (isdigit(telefone[i]))
        {
            digitos++;
        }
    }
    return (digitos == 10 || digitos == 11);
}

// Função para cadastrar um novo paciente
void cadastrarPaciente(Paciente *paciente)
{
    FILE *arquivo;
    arquivo = fopen("pacientes.txt", "a"); // Abre o arquivo em modo de adição

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        exit(1);
    }

    printf("\n\nCadastro de Paciente\n");

    // Lê e valida o nome do paciente
    do
    {
        printf("Nome: ");
        fgets(paciente->nome, 50, stdin);
        paciente->nome[strcspn(paciente->nome, "\n")] = 0;
        if (strlen(paciente->nome) == 0)
        {
            printf("Nome não pode ser vazio.\n");
        }
    } while (strlen(paciente->nome) == 0);

    // Lê e valida o CPF do paciente
    do
    {
        printf("CPF (XXX.XXX.XXX-XX): ");
        lerStringComMascara(paciente->cpf, 15, "XXX.XXX.XXX-XX");
        if (!validarCPF(paciente->cpf))
        {
            printf("CPF inválido. Deve conter 11 dígitos numéricos.\n");
        }
    } while (!validarCPF(paciente->cpf));

    // Lê e valida a data de nascimento do paciente
    do
    {
        printf("Data de Nascimento (DD/MM/AAAA): ");
        lerStringComMascara(paciente->dataNascimento, 11, "XX/XX/XXXX");
        if (!validarDataNascimento(paciente->dataNascimento))
        {
            printf("Data de nascimento inválida.\n");
        }
    } while (!validarDataNascimento(paciente->dataNascimento));

    // Lê e valida o gênero do paciente
    do
    {
        printf("Genero (M/F): ");
        scanf(" %c", &paciente->genero);
        limparBuffer();
        if (!validarGenero(paciente->genero))
        {
            printf("Gênero inválido. Use M ou F.\n");
        }
    } while (!validarGenero(paciente->genero));

    // Lê e valida o telefone do paciente
    do
    {
        printf("Telefone (XXXXXXXXXXX ou (XX)XXXXXXXXX): ");
        lerStringComMascara(paciente->telefone, 15, "(XX)XXXXXXXXX");
        if (!validarTelefone(paciente->telefone))
        {
            printf("Telefone inválido. Deve conter 10 ou 11 dígitos numéricos.\n");
        }
    } while (!validarTelefone(paciente->telefone));

    // Lê e valida o plano de saúde do paciente
    do
    {
        printf("Plano de Saude: ");
        fgets(paciente->planoSaude, 20, stdin);
        paciente->planoSaude[strcspn(paciente->planoSaude, "\n")] = 0;
        if (strlen(paciente->planoSaude) == 0)
        {
            printf("Plano de saúde não pode ser vazio.\n");
        }
    } while (strlen(paciente->planoSaude) == 0);

    // Lê e valida o endereço do paciente
    do
    {
        printf("Endereco: ");
        fgets(paciente->endereco, 100, stdin);
        paciente->endereco[strcspn(paciente->endereco, "\n")] = 0;
        if (strlen(paciente->endereco) == 0)
        {
            printf("Endereço não pode ser vazio.\n");
        }
    } while (strlen(paciente->endereco) == 0);

    // Escrevendo os dados em uma linha única, separados por vírgulas
    fprintf(arquivo, "%s,%s,%s,%c,%s,%s,%s\n",
            paciente->nome, paciente->cpf, paciente->dataNascimento,
            paciente->genero, paciente->telefone, paciente->planoSaude,
            paciente->endereco);

    fclose(arquivo); // Fecha o arquivo
    printf("\nPaciente cadastrado com sucesso!\n");
}

// Função para exibir todos os pacientes cadastrados
void exibirPacientes()
{
    FILE *arquivo;
    arquivo = fopen("pacientes.txt", "r"); // Abre o arquivo em modo de leitura

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        exit(1);
    }

    printf("\n\nLista de Pacientes\n");
    printf("--------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-50s | %-15s | %-12s | %-7s | %-13s | %-20s | %-100s |\n", "Nome", "CPF", "Data Nasc.", "Genero", "Telefone", "Plano de Saude", "Endereco");
    printf("--------------------------------------------------------------------------------------------------------------------\n");

    char linha[350];
    while (fgets(linha, sizeof(linha), arquivo)) // Lê cada linha do arquivo
    {
        Paciente paciente;
        sscanf(linha, "%49[^,],%14[^,],%10[^,],%c,%14[^,],%19[^,],%99[^\n]",
               paciente.nome, paciente.cpf, paciente.dataNascimento,
               &paciente.genero, paciente.telefone, paciente.planoSaude,
               paciente.endereco);
        printf("| %-50s | %-15s | %-12s | %-7c | %-13s | %-20s | %-100s |\n",
               paciente.nome, paciente.cpf, paciente.dataNascimento,
               paciente.genero, paciente.telefone, paciente.planoSaude,
               paciente.endereco);
    }

    printf("--------------------------------------------------------------------------------------------------------------------\n");
    fclose(arquivo); // Fecha o arquivo
}

// Função principal
int main()
{
    int opcao;

    // Loop do menu principal
    do
    {
        printf("\n\nMenu Principal\n");
        printf("1. Cadastrar Paciente\n");
        printf("2. Exibir Pacientes\n");
        printf("3. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao); // Lê a opção do usuário
        limparBuffer();      // Limpa o buffer de entrada

        // Executa a função correspondente à opção escolhida
        switch (opcao)
        {
        case 1:
            cadastrarPaciente((Paciente *)malloc(sizeof(Paciente))); // Cadastra um novo paciente
            break;
        case 2:
            exibirPacientes(); // Exibe todos os pacientes cadastrados
            break;
        case 3:
            printf("\nSaindo do programa...\n");
            break;
        default:
            printf("\nOpcao invalida!\n");
        }
    } while (opcao != 3); // Continua até que a opção seja sair (3)

    return 0; // Finaliza o programa
}
