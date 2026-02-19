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
bool		parse_scene(const char *filename, t_scene *scene);
bool		parse_ambient(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_camera(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_light(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_sphere(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_plane(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_cylinder(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_vector(const char *str, t_vec3 *vec);
bool		parse_color(const char *str, t_color *color);
void		calculate_viewport(t_camera *camera, int width, int height);
bool		is_valid_identifier(const char *str);
const char	*skip_whitespace(const char *str);
bool		is_empty_line(const char *line);
bool		is_comment_line(const char *line);
bool		validate_normalized(t_vec3 vec);
bool		validate_color_component(double value);
bool		validate_ratio(double value);
bool		validate_fov(double fov);
bool		ft_isempty_or_comment(const char *line);
char		*extract_identifier(const char *line);
bool		process_line(t_parse_context *context, t_scene *scene,
				const char *line);
void		print_parse_error(const char *filename, int line_num, 
				const char *message);
void		ft_error_and_free(t_parse_context *ctx, char **tokens,
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
