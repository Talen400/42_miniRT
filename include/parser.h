/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/02/02 00:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "miniRt.h"
# include "scene.h"

/*
** ============================================================================
** CONCEITO: Sistema de Parsing em Camadas
** ============================================================================
**
** O parser do miniRT é construído em três camadas distintas, cada uma com
** uma responsabilidade específica:
**
** CAMADA 1 - Leitura de Arquivo:
** Abre o arquivo, lê linha por linha, remove linhas vazias e comentários.
** Esta camada não tenta entender o conteúdo, apenas fornece linhas limpas
** para a próxima camada processar.
**
** CAMADA 2 - Identificação e Dispatch:
** Pega cada linha, identifica o tipo de elemento pela primeira palavra
** (A, C, L, sp, pl, cy), e chama a função de parsing específica apropriada.
** Usa uma tabela de lookup para mapear identificadores para funções.
**
** CAMADA 3 - Parsing Específico e Validação:
** Cada função de parsing específica processa sua linha, extrai os números,
** cria vetores, valida os dados, e adiciona o elemento na cena.
** Também valida regras globais (uma câmera, uma luz ambiente, etc).
**
** Esta separação em camadas torna o código modular, testável e fácil de
** estender com novos tipos de elementos.
** ============================================================================
*/

/*
** ============================================================================
** TYPEDEF: t_parse_function
** ============================================================================
**
** Este é o tipo para ponteiros de funções de parsing. Todas as funções que
** processam um tipo específico de elemento (luz ambiente, câmera, esfera, etc)
** seguem esta assinatura.
**
** Por que usar typedef para ponteiro de função? Porque ponteiros de função
** em C têm uma sintaxe notoriamente confusa. Compare:
**
** SEM typedef (confuso):
**    bool (*func)(const char *, t_scene *, int)
**
** COM typedef (claro):
**    t_parse_function func
**
** Ambos representam exatamente a mesma coisa, mas o segundo é infinitamente
** mais legível. Quando você precisa declarar arrays de ponteiros de função
** ou passar ponteiros de função como parâmetros, o typedef se torna essencial
** para manter a sanidade.
**
** PARÂMETROS:
** - line: A linha completa do arquivo (após o identificador)
** - scene: Ponteiro para a cena onde o elemento será adicionado
** - line_num: Número da linha no arquivo (para mensagens de erro úteis)
**
** RETORNO:
** - true se o parsing foi bem sucedido
** - false se houve erro (a função já imprimiu a mensagem de erro)
**
** Esta uniformidade de assinatura é o que permite usar a tabela de lookup.
** Todas as funções seguem o mesmo contrato, então podemos tratá-las de
** forma polimórfica.
** ============================================================================
*/

typedef struct s_parse_context	t_parse_context;

typedef bool (*t_parse_function)(t_parse_context *ctx, t_scene *scene, const char *line);

/*
** ============================================================================
** ESTRUTURA: t_element_parser
** ============================================================================
**
** Esta estrutura representa uma entrada na tabela de lookup. Cada entrada
** mapeia um identificador de elemento (como "sp" ou "C") para a função que
** sabe como processar aquele tipo de elemento.
**
** Pense nesta struct como uma linha de um dicionário:
** - A "chave" é o identificador (a string que aparece no arquivo)
** - O "valor" é a função que processa aquele tipo
**
** Quando você recebe uma linha do arquivo, extrai o identificador e procura
** na tabela por uma entrada com aquele identificador. Quando encontra,
** você chama a função associada.
**
** CAMPOS:
**
** - identifier: A string que identifica este tipo de elemento no arquivo.
**   Por exemplo: "A" para luz ambiente, "sp" para esfera, "cy" para cilindro.
**   Este é um ponteiro para uma string constante em memória estática - você
**   não vai alocar ou liberar essas strings, elas apenas existem.
**
** - parse_func: Ponteiro para a função que sabe como processar este tipo.
**   Por exemplo, para "sp", este seria um ponteiro para parse_sphere.
**   Note que este é do tipo t_parse_function que definimos acima, o que
**   garante que todas as funções têm a mesma assinatura.
**
** EXEMPLO DE USO CONCEITUAL:
** Se você tem uma linha "sp 0,0,20 12.6 255,0,0", você:
** 1. Extrai "sp" como identificador
** 2. Procura na tabela por uma entrada onde identifier == "sp"
** 3. Quando encontra, chama entry.parse_func("0,0,20 12.6 255,0,0", scene, line_num)
** 4. A função parse_sphere processa o resto da linha e adiciona a esfera na cena
**
** Esta indireção (mapping identifier -> função) é o que torna fácil adicionar
** novos tipos sem modificar a lógica principal do parser.
** ============================================================================
*/
typedef struct s_light_data
{
	t_point3	position;
	double		brightness;
	t_color		color;
}	t_light_data;

// Para câmera
typedef struct s_camera_data
{
	t_point3	position;
	t_vec3		orientation;
	double		fov;
}	t_camera_data;

// Para esfera
typedef struct s_sphere_data
{
	t_point3	center;
	double		diameter;
	t_color		color;
}	t_sphere_data;

// Para plano
typedef struct s_plane_data
{
	t_point3	point;
	t_vec3		normal;
	t_color		color;
}	t_plane_data;

// Para cilindro
typedef struct s_cylinder_data
{
	t_point3	center;
	t_vec3		axis;
	double		diameter;
	double		height;
	t_color		color;
}	t_cylinder_data;

typedef struct s_element_parser
{
	const char			*identifier;    // "A", "C", "L", "sp", "pl", "cy"
	t_parse_function	parser;         // Ponteiro para a função de parsing
}	t_element_parser;

/*
** ============================================================================
** ESTRUTURA: t_parse_context
** ============================================================================
**
** Esta estrutura mantém o contexto de estado durante o parsing. Ela guarda
** informações que precisam ser rastreadas conforme processamos o arquivo,
** principalmente contadores para validar regras como "exatamente uma câmera".
**
** Por que precisamos desta estrutura? Durante o parsing, você não pode apenas
** processar cada linha isoladamente. Você precisa se lembrar do que já viu.
** Por exemplo, se você encontra uma segunda linha de câmera, precisa saber
** que já viu uma primeira para poder reportar erro.
**
** Esta struct centraliza todo esse estado em um único lugar, em vez de ter
** variáveis soltas espalhadas pelo código.
**
** CAMPOS - CONTADORES:
**
** - camera_count: Quantas câmeras foram encontradas. Deve ser exatamente 1
**   ao final do parsing. Se for 0, falta câmera. Se for 2+, há câmeras
**   duplicadas. Ambos são erros.
**
** - ambient_count: Quantas luzes ambiente foram encontradas. Deve ser
**   exatamente 1. A luz ambiente define a iluminação base da cena.
**
** - light_count: Quantas luzes pontuais foram encontradas. Deve ser pelo
**   menos 1. Uma cena sem nenhuma luz pontual seria completamente escura
**   (exceto pela luz ambiente).
**
** CAMPOS - TRACKING:
**
** - current_line: O número da linha atual sendo processada. Começa em 1
**   (primeira linha do arquivo) e incrementa a cada linha. Usado para
**   mensagens de erro informativas. Quando algo dá errado, você pode dizer
**   exatamente onde: "Error line 5: Invalid sphere diameter".
**
** - filename: Nome do arquivo sendo parseado. Guardamos uma cópia do
**   ponteiro (não uma cópia da string) para poder incluir nas mensagens
**   de erro. Por exemplo: "Error in 'scene.rt' line 5: ..."
**
** CAMPOS - ERROR HANDLING:
**
** - error_occurred: Flag booleana que marca se algum erro já ocorreu.
**   Quando um erro acontece, esta flag é setada para true. Isso permite
**   que você decida se quer continuar tentando parsear (para encontrar
**   múltiplos erros de uma vez) ou abortar imediatamente.
**   Para o miniRT, recomendo abortar no primeiro erro, mas a flag está
**   aqui caso você queira implementar parsing mais robusto depois.
**
** INICIALIZAÇÃO:
** Esta struct deve ser zerada no início do parsing (todos os contadores
** em zero, todas as flags em false, ponteiros em NULL). Você pode ter uma
** função init_parse_context() que faz isso.
**
** VALIDAÇÃO FINAL:
** Após processar todas as linhas, você valida os contadores:
** - Se camera_count != 1, retorna erro
** - Se ambient_count != 1, retorna erro  
** - Se light_count < 1, retorna erro
** Se todos os contadores estão corretos, o parsing foi bem sucedido.
** ============================================================================
*/

struct s_parse_context
{
	int			camera_count;
	int			ambient_count;
	int			light_count;
	int			current_line;
	const char	*filename;
	bool		error_occurred;
};

/*
** ===========================================================================
**                    PROTÓTIPOS - FUNÇÃO PRINCIPAL
** ===========================================================================
*/

/*
** parse_scene - Função principal do parser
**
** Esta é a função pública do módulo de parsing - a única que o resto do
** programa precisa conhecer. Ela coordena todo o processo de parsing.
**
** RESPONSABILIDADES:
** 1. Abre o arquivo e verifica se abriu com sucesso
** 2. Inicializa o contexto de parsing (contadores zerados)
** 3. Lê o arquivo linha por linha
** 4. Para cada linha, identifica o tipo e chama a função apropriada
** 5. Após processar tudo, valida os contadores (uma câmera, uma luz ambiente, etc)
** 6. Libera recursos e fecha arquivo
** 7. Retorna sucesso ou falha
**
** PARÂMETROS:
** - filename: Caminho do arquivo .rt para processar
** - scene: Ponteiro para struct t_scene onde os dados serão armazenados
**          Esta struct deve estar alocada (malloc) antes de chamar parse_scene
**
** RETORNO:
** - true: Parsing bem sucedido, scene está completamente preenchida
** - false: Erro ocorreu, mensagem já foi impressa, scene pode estar
**          parcialmente preenchida (a função de cleanup deve lidar com isso)
**
** TRATAMENTO DE ERRO:
** Em caso de erro, esta função:
** - Imprime uma mensagem clara indicando o problema
** - Fecha o arquivo se estava aberto
** - Libera qualquer memória temporária alocada
** - Retorna false (NÃO libera a struct scene - isso é responsabilidade do caller)
**
** EXEMPLO DE USO:
**     t_scene *scene = malloc(sizeof(t_scene));
**     init_scene(scene);  // Zera todos os campos
**     if (!parse_scene("scene.rt", scene))
**     {
**         free_scene(scene);  // Libera o que foi alocado
**         free(scene);
**         return (error);
**     }
**     // Continua com rendering...
*/

bool	parse_scene(const char *filename, t_scene *scene);

/*
** ===========================================================================
**              PROTÓTIPOS - FUNÇÕES DE PARSING ESPECÍFICAS
** ===========================================================================
**
** Cada uma destas funções processa um tipo específico de elemento da cena.
** Todas seguem a mesma assinatura (t_parse_function) para que possam ser
** usadas na tabela de lookup.
**
** CONVENÇÃO DE NOMENCLATURA:
** parse_<tipo> - por exemplo, parse_ambient, parse_sphere
**
** RESPONSABILIDADES COMUNS:
** Todas estas funções devem:
** 1. Quebrar a linha em tokens (usando ft_split ou similar)
** 2. Converter strings para números (usando atof ou funções custom)
** 3. Validar que há o número correto de parâmetros
** 4. Validar que os valores estão em ranges válidos
** 5. Criar/preencher a struct apropriada
** 6. Adicionar o elemento na cena (na lista ou campo apropriado)
** 7. Incrementar contadores no parse_context se aplicável
** 8. Liberar memória temporária (tokens, etc)
** 9. Retornar true se sucesso, false se erro
** 10. Se erro, imprimir mensagem antes de retornar false
**
** VALIDAÇÕES ESPECÍFICAS:
** - Vetores de direção devem estar normalizados (ou serem normalizados)
** - Intensidades devem estar em [0.0, 1.0]
** - Cores devem estar em [0, 255]
** - Dimensões (diâmetros, alturas) devem ser positivas
** - FOV deve estar em (0, 180]
*/

/*
** parse_ambient - Processa linha de luz ambiente
** Formato: A <intensidade> <cor_R,G,B>
** Exemplo: A 0.2 255,255,255
** Validações: intensidade em [0.0, 1.0], cores em [0, 255]
** Incrementa: ambient_count
*/
bool	parse_ambient(t_parse_context *ctx, t_scene *scene, const char *line);

/*
** parse_camera - Processa linha de câmera
** Formato: C <posição_x,y,z> <orientação_x,y,z> <fov>
** Exemplo: C -50.0,0,20 0,0,1 70
** Validações: orientação normalizada, FOV em (0, 180]
** Incrementa: camera_count
** Nota: Também calcula os vetores forward, right, up da câmera
*/
bool	parse_camera(t_parse_context *ctx, t_scene *scene, const char *line);

/*
** parse_light - Processa linha de luz pontual
** Formato: L <posição_x,y,z> <brilho> <cor_R,G,B>
** Exemplo: L -40.0,50.0,0.0 0.6 255,255,255
** Validações: brilho em [0.0, 1.0], cores em [0, 255]
** Incrementa: light_count
** Nota: Cria novo nó na lista ligada de luzes
*/
bool	parse_light(t_parse_context *ctx, t_scene *scene, const char *line);

/*
** parse_sphere - Processa linha de esfera
** Formato: sp <centro_x,y,z> <diâmetro> <cor_R,G,B>
** Exemplo: sp 0.0,0.0,20.6 12.6 10,0,255
** Validações: diâmetro > 0, cores em [0, 255]
** Nota: Cria novo nó na lista ligada de objetos com type=SPHERE
*/
bool	parse_sphere(t_parse_context *ctx, t_scene *scene, const char *line);

/*
** parse_plane - Processa linha de plano
** Formato: pl <ponto_x,y,z> <normal_x,y,z> <cor_R,G,B>
** Exemplo: pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225
** Validações: normal normalizada, cores em [0, 255]
** Nota: Cria novo nó na lista ligada de objetos com type=PLANE
*/
bool	parse_plane(t_parse_context *ctx, t_scene *scene, const char *line);

/*
** parse_cylinder - Processa linha de cilindro
** Formato: cy <centro_x,y,z> <eixo_x,y,z> <diâmetro> <altura> <cor_R,G,B>
** Exemplo: cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255
** Validações: eixo normalizado, diâmetro > 0, altura > 0, cores em [0, 255]
** Nota: Cria novo nó na lista ligada de objetos com type=CYLINDER
*/
bool	parse_cylinder(t_parse_context *ctx, t_scene *scene, const char *line);

/*
** ===========================================================================
**                    PROTÓTIPOS - FUNÇÕES UTILITÁRIAS
** ===========================================================================
**
** Estas são funções auxiliares usadas pelas funções de parsing específicas.
** Elas fazem tarefas comuns como converter strings para vetores, validar
** ranges, manipular tokens, etc.
*/

/*
** parse_vector - Converte string "x,y,z" para t_vec3
**
** Esta função pega uma string no formato "1.5,2.3,4.1" e converte para
** um vetor t_vec3 com os valores extraídos.
**
** PARÂMETROS:
** - str: String no formato "x,y,z" (números separados por vírgulas)
** - vec: Ponteiro para t_vec3 onde o resultado será armazenado
**
** RETORNO:
** - true se parsing foi bem sucedido
** - false se formato é inválido (faltam componentes, caracteres inválidos, etc)
**
** VALIDAÇÕES:
** - Deve haver exatamente 3 componentes separados por vírgulas
** - Cada componente deve ser um número válido
** - Não deve haver espaços (ou espaços devem ser ignorados)
**
** EXEMPLO:
**     t_vec3 pos;
**     if (parse_vector("10.5,-3.2,15.0", &pos))
**         // pos.x=10.5, pos.y=-3.2, pos.z=15.0
*/
bool	parse_vector(const char *str, t_vec3 *vec);

/*
** parse_color - Converte string "R,G,B" para t_color
**
** Similar a parse_vector, mas para cores. Converte "255,128,0" em um
** vetor onde x=red, y=green, z=blue. Também valida que os valores estão
** no range correto [0, 255].
**
** PARÂMETROS:
** - str: String no formato "R,G,B" (inteiros de 0 a 255 separados por vírgulas)
** - color: Ponteiro para t_color onde o resultado será armazenado
**
** RETORNO:
** - true se parsing foi bem sucedido e valores válidos
** - false se formato inválido ou valores fora do range
**
** VALIDAÇÕES:
** - Deve haver exatamente 3 componentes
** - Cada componente deve ser inteiro em [0, 255]
**
** NOTA SOBRE REPRESENTAÇÃO:
** Você pode escolher representar cores como inteiros [0, 255] ou como
** floats normalizados [0.0, 1.0]. Para ray tracing, floats são mais comuns
** porque facilitam os cálculos. Você pode converter aqui ou depois.
*/
bool	parse_color(const char *str, t_color *color);

/*
** is_valid_identifier - Verifica se string é um identificador válido
**
** Verifica se uma string corresponde a algum identificador conhecido
** (A, C, L, sp, pl, cy). Útil para dar mensagens de erro melhores quando
** encontramos algo inválido.
**
** PARÂMETROS:
** - str: String a verificar
**
** RETORNO:
** - true se é um identificador válido
** - false se não reconhecido
*/
bool	is_valid_identifier(const char *str);

/*
** skip_whitespace - Pula espaços em branco no início de string
**
** Retorna ponteiro para o primeiro caractere não-espaço da string.
** Útil para normalizar linhas que podem ter espaços extras.
**
** PARÂMETROS:
** - str: String possivelmente com espaços no início
**
** RETORNO:
** - Ponteiro para primeiro caractere não-espaço
** - Ou para '\0' se string é só espaços
*/
const char	*skip_whitespace(const char *str);

/*
** is_empty_line - Verifica se linha é vazia ou só tem espaços
**
** Determina se uma linha deve ser ignorada (vazia ou só whitespace).
**
** PARÂMETROS:
** - line: String a verificar
**
** RETORNO:
** - true se linha é vazia ou só tem espaços/tabs/newlines
** - false se tem conteúdo real
*/
bool	is_empty_line(const char *line);

/*
** is_comment_line - Verifica se linha é comentário
**
** Verifica se linha começa com '#' (após ignorar espaços iniciais).
** Comentários são opcionais no miniRT mas úteis para testar.
**
** PARÂMETROS:
** - line: String a verificar
**
** RETORNO:
** - true se é comentário (começa com #)
** - false caso contrário
*/
bool	is_comment_line(const char *line);

/*
** validate_normalized - Verifica se vetor está normalizado
**
** Verifica se o comprimento de um vetor é aproximadamente 1.0.
** Usa EPSILON para tolerância de erro de ponto flutuante.
**
** PARÂMETROS:
** - vec: Vetor a verificar
**
** RETORNO:
** - true se comprimento está entre (1.0 - EPSILON) e (1.0 + EPSILON)
** - false caso contrário
**
** NOTA: Esta função verifica, mas não normaliza. A decisão de normalizar
** automaticamente ou rejeitar fica com a função de parsing específica.
*/
bool	validate_normalized(t_vec3 vec);

/*
** validate_color_component - Verifica se componente de cor é válido
**
** Verifica se um valor numérico está no range válido para cor [0, 255].
**
** PARÂMETROS:
** - value: Valor a verificar
**
** RETORNO:
** - true se value >= 0.0 && value <= 255.0
** - false caso contrário
*/
bool	validate_color_component(double value);

bool	validate_ratio(double value);

/*
** validate_fov - Verifica se FOV está em (0, 180]
**
** Valida campo de visão da câmera. FOV de 0 ou negativo não faz sentido,
** FOV maior que 180 também não (seria ver mais de meio círculo).
**
** PARÂMETROS:
** - fov: Campo de visão em graus
**
** RETORNO:
** - true se fov > 0.0 && fov <= 180.0
** - false caso contrário
*/
bool	validate_fov(double fov);

/*
** ===========================================================================
**                  PROTÓTIPOS - GERENCIAMENTO DE CONTEXTO
** ===========================================================================
*/

/*
** init_parse_context - Inicializa contexto de parsing
**
** Zera todos os contadores e flags do contexto antes de começar o parsing.
**
** PARÂMETROS:
** - ctx: Ponteiro para contexto a inicializar
** - filename: Nome do arquivo sendo parseado (salvo no contexto)
*/
bool	ft_isempty_or_comment(const char *line);
char	*extract_identifier(const char *line);
bool	process_line(t_parse_context *context, t_scene *scene,
			const char *line);

void	print_parse_error(const char *filename, int line_num, 
			const char *message);
void	ft_error_and_free(t_parse_context *ctx, char **tokens,
			const char *message);
/*
** ===========================================================================
**                          TABELA DE LOOKUP
** ===========================================================================
**
** A tabela de lookup é declarada como extern aqui porque ela é definida
** em parser.c. Outros arquivos do módulo de parsing podem referenciá-la
** através deste header.
**
** A tabela é um array de t_element_parser terminado por uma entrada com
** identifier NULL (sentinela). Isso permite iterar pela tabela sem saber
** o tamanho em tempo de compilação.
**
** ESTRUTURA DA TABELA (em parser.c):
**
**     const t_element_parser g_element_parsers[] = {
**         {"A", parse_ambient},
**         {"C", parse_camera},
**         {"L", parse_light},
**         {"sp", parse_sphere},
**         {"pl", parse_plane},
**         {"cy", parse_cylinder},
**         {NULL, NULL}  // Sentinela marca o fim
**     };
**
** COMO USAR:
** Itere pela tabela até encontrar identifier NULL. Para cada entrada,
** compare o identifier com a string que você extraiu da linha. Se bater,
** chame a parse_func associada.
**
** EXEMPLO DE BUSCA:
**     int i = 0;
**     while (g_element_parsers[i].identifier != NULL)
**     {
**         if (strcmp(g_element_parsers[i].identifier, id) == 0)
**         {
**             // Encontrou! Chama a função
**             return g_element_parsers[i].parse_func(line, scene, line_num);
**         }
**         i++;
**     }
**     // Não encontrou - identificador inválido
**
** VANTAGENS DESTA ABORDAGEM:
** - Para adicionar novo tipo: adicione uma linha na tabela
** - Não precisa modificar lógica de busca
** - Fácil de desabilitar temporariamente um tipo (comente a linha)
** - Código de busca é genérico e não muda nunca
*/

extern const t_element_parser	g_element_parsers[];

#endif
