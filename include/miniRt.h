/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/02/02 17:04:17 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

/* ========================================================================== */
/*                               BIBLIOTECAS                                  */
/* ========================================================================== */

# include <math.h>       // Funções matemáticas: sqrt, pow, sin, cos, etc
# include <stdlib.h>     // malloc, free, exit
# include <fcntl.h>      // open, close
# include <unistd.h>     // read, write
# include <stdio.h>      // printf (apenas para debugging)
# include <stdbool.h>    // tipo bool (true/false)

#include "vectors.h"   // Operações com vetores 3D

#include "../MLX42/include/MLX42/MLX42.h"	// Lib da mlx
#include "color.h"
#include "../libft/include/libft.h"			// lib da libft
/* ========================================================================== */
/*                              CONSTANTES                                    */
/* ========================================================================== */

/*
** Definições de tamanho da janela e precisão numérica
** Estas constantes controlam aspectos fundamentais da renderização
*/

# define WIDTH 1280          // Largura padrão da janela em pixels
# define HEIGHT 800         // Altura padrão da janela em pixels
# define EPSILON 1e-6        // Valor muito pequeno para comparações de float
                             // Usado para evitar problemas de precisão numérica

/*
** Constantes matemáticas importantes para ray tracing
** PI é usado em cálculos de ângulos e conversões
*/

# define PI 3.14159265358979323846

/*
** Limites para intersecções de raios
** T_MIN evita "shadow acne" (quando um objeto faz sombra em si mesmo)
** T_MAX define a distância máxima que um raio pode viajar
*/

# define T_MIN 0.001         // Distância mínima para considerar uma interseção
# define T_MAX 1e30          // Distância máxima (essencialmente infinito)

/* ========================================================================== */
/*                       ESTRUTURAS FUNDAMENTAIS                              */
/* ========================================================================== */


/*
** Aliases semânticos para tornar o código mais legível
** Mesmo sendo o mesmo tipo de estrutura, usar nomes diferentes
** ajuda a entender a intenção do programador:
*/

typedef t_vec3	t_point3;    // Quando representa uma posição no espaço

/*
** ============================================================================
** ESTRUTURA: t_ray
** ============================================================================
**
** Um raio é a entidade central do ray tracing. Matematicamente, um raio é
** definido pela equação paramétrica:
**
**     P(t) = origin + t * direction
**
** Onde:
** - origin: ponto de partida do raio (t_point3)
** - direction: vetor que indica para onde o raio vai (t_vec3)
** - t: parâmetro que representa a distância ao longo do raio
**
** Para qualquer valor de t positivo, obtemos um ponto ao longo do raio.
** Por exemplo, se t=0, estamos na origem. Se t=1, estamos a uma unidade
** de distância na direção do vetor direction.
**
** É essencial que o vetor direction seja NORMALIZADO (comprimento = 1)
** para que t represente a distância real. Se não for normalizado, t será
** apenas um múltiplo arbitrário.
**
** No ray tracing, lançamos milhões de raios da câmera através dos pixels
** da tela para determinar o que é visível em cada ponto.
** ============================================================================
*/

typedef struct s_ray
{
	t_point3	origin;       // Ponto de onde o raio parte
	t_vec3		direction;    // Direção para onde o raio vai (normalizado)
}	t_ray;

/*
** ============================================================================
** ESTRUTURA: t_camera
** ============================================================================
**
** A câmera define o ponto de vista do observador na cena. Ela funciona como
** uma câmera fotográfica real, com posição, direção e campo de visão.
**
** O campo de visão (FOV) determina quanto da cena é visível. Um FOV menor
** (exemplo: 30°) funciona como um zoom - vemos menos da cena, mas com mais
** detalhes. Um FOV maior (exemplo: 120°) funciona como uma lente grande
** angular - vemos mais da cena, mas com distorção nas bordas.
**
** A câmera tem um sistema de coordenadas próprio formado por três vetores
** perpendiculares entre si (ortogonais):
**
** - forward: para onde a câmera aponta (frente)
** - right: o lado direito da câmera
** - up: o topo da câmera
**
** Estes três vetores formam uma base ortonormal (perpendiculares e de
** comprimento unitário) que nos permite calcular facilmente a direção de
** qualquer raio através de qualquer pixel da tela.
**
** O viewport é um plano imaginário na frente da câmera onde projetamos
** a imagem. Cada pixel da tela corresponde a um pequeno retângulo neste
** viewport, e lançamos raios através do centro destes retângulos.
** ============================================================================
*/

typedef struct s_camera
{
	t_point3	position;           // Onde a câmera está no espaço 3D
	t_vec3		orientation;        // Direção normalizada para onde olha
	double		fov;                // Campo de visão em graus [0, 180]
	
	/* Vetores do sistema de coordenadas da câmera (calculados) */
	t_vec3		forward;            // = orientation (normalizado)
	t_vec3		right;              // Perpendicular a forward e up
	t_vec3		up;                 // Perpendicular a forward e right
	
	/* Dimensões e posição do viewport (plano de projeção) */
	double		viewport_width;     // Largura do viewport em unidades do mundo
	double		viewport_height;    // Altura do viewport em unidades do mundo
	t_point3	lower_left_corner;  // Canto inferior esquerdo do viewport
	t_vec3		horizontal;         // Vetor horizontal do viewport
	t_vec3		vertical;           // Vetor vertical do viewport
}	t_camera;

/* ========================================================================== */
/*                          ESTRUTURAS DE ILUMINAÇÃO                          */
/* ========================================================================== */

/*
** ============================================================================
** ESTRUTURA: t_ambient
** ============================================================================
**
** A luz ambiente representa a iluminação geral e difusa da cena que vem de
** todas as direções. Na realidade, quando acendemos uma luz em uma sala, a
** luz não ilumina apenas os objetos diretamente - ela também reflete nas
** paredes, no teto, no chão, e esses reflexos iluminam indiretamente outros
** objetos.
**
** Simular todos esses reflexos é extremamente computacionalmente caro
** (técnicas como path tracing fazem isso). Para o miniRT, usamos uma
** aproximação simples: uma luz constante que ilumina uniformemente todos
** os objetos, independente de sua posição.
**
** Sem luz ambiente, qualquer parte da cena que não estivesse diretamente
** sob uma luz ficaria completamente preta, o que não é realista. A luz
** ambiente garante que sempre haja alguma iluminação básica.
**
** A intensidade controla quão forte é esta luz ambiente. Um valor de 0.0
** significa nenhuma luz ambiente (apenas iluminação direta). Um valor de
** 1.0 significa que a luz ambiente sozinha já ilumina completamente a cena.
** Valores típicos ficam entre 0.1 e 0.3.
** ============================================================================
*/

typedef struct s_ambient
{
	double		intensity;    // Intensidade da luz [0.0, 1.0]
	t_color		color;        // Cor da luz ambiente (RGB)
}	t_ambient;

/*
** ============================================================================
** ESTRUTURA: t_light
** ============================================================================
**
** Uma fonte de luz pontual que emite luz igualmente em todas as direções
** a partir de um ponto específico no espaço, como uma lâmpada sem abajur
** ou uma vela.
**
** A intensidade (brightness) controla quão forte é a luz. Um valor de 1.0
** significa luz muito brilhante, enquanto 0.5 seria meia intensidade. Este
** valor é multiplicado pela cor da luz e depois pela cor dos objetos para
** determinar a cor final.
**
** A cor da luz afeta a aparência dos objetos. Por exemplo, uma luz vermelha
** fará com que objetos brancos pareçam vermelhos, e objetos azuis pareçam
** escuros (porque azul não reflete luz vermelha).
**
** No ray tracing, para cada ponto de uma superfície que estamos renderizando,
** calculamos:
** 1. A direção da luz até aquele ponto
** 2. A distância da luz até aquele ponto (a luz fica mais fraca com a distância)
** 3. Se há algum objeto bloqueando a luz (criando sombra)
**
** O projeto permite múltiplas luzes, então usamos uma lista ligada.
** ============================================================================
*/

typedef struct s_light
{
	t_point3		position;     // Posição da luz no espaço 3D
	double			brightness;   // Intensidade/brilho [0.0, 1.0]
	t_color			color;        // Cor da luz (RGB)
	struct s_light	*next;        // Próxima luz na lista (pode haver várias)
}	t_light;

/* ========================================================================== */
/*                       ESTRUTURAS DE OBJETOS GEOMÉTRICOS                    */
/* ========================================================================== */

/*
** ============================================================================
** ESTRUTURA: t_sphere
** ============================================================================
**
** Uma esfera é o objeto geométrico mais simples no ray tracing. É definida
** matematicamente por todos os pontos que estão a uma distância fixa (raio)
** de um ponto central.
**
** Equação da esfera: (P - C) · (P - C) = r²
** Onde:
** - P é um ponto qualquer na superfície
** - C é o centro da esfera
** - r é o raio
** - · representa o produto escalar
**
** Para verificar se um raio intersecta a esfera, substituímos P pela
** equação do raio P(t) = O + tD e resolvemos para t. Isso resulta em
** uma equação quadrática (at² + bt + c = 0) que pode ter:
** - 0 soluções: raio não toca a esfera
** - 1 solução: raio é tangente à esfera
** - 2 soluções: raio atravessa a esfera (entrada e saída)
**
** Guardamos diameter em vez de radius diretamente porque é assim que o
** arquivo .rt especifica, mas calculamos o radius (diameter/2) para
** facilitar os cálculos matemáticos.
** ============================================================================
*/

typedef struct s_sphere
{
	t_point3	center;       // Centro da esfera no espaço 3D
	double		diameter;     // Diâmetro (como especificado no .rt)
	double		radius;       // Raio calculado (diameter / 2.0)
	t_color		color;        // Cor da superfície (RGB)
}	t_sphere;

/*
** ============================================================================
** ESTRUTURA: t_plane
** ============================================================================
**
** Um plano infinito é definido por um ponto no plano e um vetor normal
** (perpendicular) ao plano. Matematicamente, um plano é o conjunto de
** todos os pontos P tais que:
**
** (P - point) · normal = 0
**
** Onde:
** - P é qualquer ponto no plano
** - point é um ponto conhecido no plano
** - normal é o vetor perpendicular ao plano
** - · é o produto escalar
**
** Geometricamente, isso significa que o vetor de 'point' até P deve ser
** perpendicular ao vetor normal, o que só acontece se P estiver no plano.
**
** Para calcular a interseção de um raio com um plano:
** 1. Substituímos P pela equação do raio: P(t) = origin + t * direction
** 2. Resolvemos para t
** 3. Se t > 0, o raio intersecta o plano à distância t
**
** O vetor normal DEVE ser normalizado (comprimento = 1) para que os
** cálculos de iluminação funcionem corretamente. O normal determina
** "qual lado" do plano está voltado para cima.
** ============================================================================
*/

typedef struct s_plane
{
	t_point3	point;        // Qualquer ponto no plano
	t_vec3		normal;       // Vetor perpendicular ao plano (normalizado)
	t_color		color;        // Cor da superfície (RGB)
}	t_plane;

/*
** ============================================================================
** ESTRUTURA: t_cylinder
** ============================================================================
**
** O cilindro é o objeto mais complexo da parte obrigatória. Ele é definido
** por:
** - Um ponto central (base do cilindro)
** - Um vetor de eixo (direção ao longo do cilindro)
** - Um diâmetro (espessura)
** - Uma altura (comprimento)
**
** Matematicamente, um cilindro infinito é definido como todos os pontos que
** estão a uma distância fixa (raio) de uma linha (o eixo). Mas no miniRT,
** queremos cilindros finitos, então precisamos:
**
** 1. Verificar interseção com o corpo cilíndrico infinito
** 2. Verificar se o ponto de interseção está entre as duas tampas
** 3. Verificar interseção com as próprias tampas (dois discos circulares)
**
** O cálculo de interseção raio-cilindro envolve:
** - Projetar o raio no plano perpendicular ao eixo do cilindro
** - Resolver uma equação quadrática para encontrar onde o raio projetado
**   intersecta o círculo
** - Calcular a altura do ponto de interseção ao longo do eixo
** - Verificar se essa altura está dentro dos limites [0, height]
**
** Para as tampas, tratamos cada uma como um disco circular (plano finito)
** e verificamos se a interseção com o plano está dentro do raio.
**
** O vetor axis DEVE ser normalizado para que os cálculos funcionem.
** ============================================================================
*/

typedef struct s_cylinder
{
	t_point3	center;       // Centro da base inferior do cilindro
	t_vec3		axis;         // Vetor do eixo central (normalizado)
	double		diameter;     // Diâmetro (como especificado no .rt)
	double		radius;       // Raio calculado (diameter / 2.0)
	double		height;       // Altura do cilindro
	t_color		color;        // Cor da superfície (RGB)
}	t_cylinder;

typedef enum e_cylinder_surface
{
	CYL_BODY,
	CYL_BOTTOM_CAP,
	CYL_TOP_CAP
}	t_cylinder_surface;

/*
** ============================================================================
** ENUMERAÇÃO: t_object_type
** ============================================================================
**
** Identificadores de tipo para os diferentes objetos geométricos.
** Usamos um enum em vez de #define porque o compilador pode fazer
** verificação de tipos, tornando o código mais seguro.
**
** Estes valores são usados para determinar qual função de interseção
** chamar e como interpretar os dados na union da estrutura t_object.
** ============================================================================
*/

typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER
}	t_object_type;

/*
** ============================================================================
** ESTRUTURA: t_object
** ============================================================================
**
** Esta estrutura unifica todos os tipos de objetos em um único tipo,
** permitindo que mantenhamos uma lista heterogênea de objetos diferentes.
**
** A 'union' é uma construção especial em C que permite armazenar diferentes
** tipos no mesmo espaço de memória. Apenas um dos campos da union é válido
** por vez, determinado pelo campo 'type'.
**
** Por exemplo:
** - Se type == SPHERE, então shape.sphere contém dados válidos
** - Se type == PLANE, então shape.plane contém dados válidos
** - Se type == CYLINDER, então shape.cylinder contém dados válidos
**
** A vantagem da union é economia de memória: em vez de ter três ponteiros
** (um para cada tipo), temos apenas o espaço do maior tipo.
**
** A lista ligada (campo 'next') nos permite ter um número arbitrário de
** objetos sem precisar definir um tamanho máximo de array. Cada objeto
** aponta para o próximo, formando uma cadeia. O último objeto tem next = NULL.
**
** Durante o ray tracing, iteramos por esta lista testando interseção do
** raio com cada objeto para encontrar o mais próximo.
** ============================================================================
*/

typedef struct s_object
{
	t_object_type	type;         // Que tipo de objeto é este?
	union u_shape                 // Os dados específicos (apenas um válido)
	{
		t_sphere	sphere;       // Dados se for esfera
		t_plane		plane;        // Dados se for plano
		t_cylinder	cylinder;     // Dados se for cilindro
	}				shape;
	struct s_object	*next;        // Próximo objeto na lista (NULL se último)
}	t_object;

/* ========================================================================== */
/*                     ESTRUTURAS DE INTERSEÇÃO E HIT                         */
/* ========================================================================== */

/*
** ============================================================================
** ESTRUTURA: t_hit_record
** ============================================================================
**
** Quando um raio intersecta um objeto, precisamos guardar muitas informações
** sobre esse ponto de interseção para calcular a cor correta. Esta estrutura
** armazena todos esses dados.
**
** Campos importantes:
**
** - point: O ponto exato no espaço 3D onde o raio tocou a superfície
**
** - normal: O vetor perpendicular à superfície naquele ponto. Usado para
**   calcular como a luz reflete. SEMPRE normalizado (comprimento = 1).
**
** - t: A distância ao longo do raio até o ponto de interseção. Na equação
**   P(t) = origin + t*direction, este é o valor de t. É crucial para
**   determinar qual objeto está mais próximo quando múltiplos objetos
**   são atingidos.
**
** - color: A cor intrínseca do objeto naquele ponto. Esta cor será modulada
**   pela iluminação para produzir a cor final do pixel.
**
** - front_face: Indica se o raio atingiu a superfície por fora (true) ou
**   por dentro (false). Isso é importante porque:
**   * Se atingimos por fora, o normal aponta para fora do objeto
**   * Se atingimos por dentro, o normal deve ser invertido
**   * Afeta cálculos de reflexão e refração
**
** - object: Ponteiro para o objeto que foi atingido. Útil se precisarmos
**   de informações adicionais sobre o objeto (como propriedades de material
**   em implementações mais avançadas).
**
** Esta estrutura é preenchida pela função de interseção e usada pela
** função de iluminação para calcular a cor final do pixel.
** ============================================================================
*/

typedef struct s_hit_record
{
	t_point3			point;
	t_vec3				normal;
	double				t;
	t_color				color;
	bool				front_face;
	t_object			*object;
	t_cylinder_surface	cylinder_surface;
}	t_hit_record;

typedef struct s_quadratic_equation
{
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t;
} t_quadratic_equation;

/* ========================================================================== */
/*                        ESTRUTURA DA CENA COMPLETA                          */
/* ========================================================================== */

/*
** ============================================================================
** ESTRUTURA: t_scene
** ============================================================================
**
** Esta estrutura reúne todos os elementos que compõem uma cena 3D:
** câmera, iluminação e objetos. É a representação completa do mundo
** que queremos renderizar.
**
** De acordo com o formato .rt, algumas restrições se aplicam:
** - Apenas UMA câmera (identificador 'C' no arquivo)
** - Apenas UMA luz ambiente (identificador 'A' no arquivo)
** - Pelo menos UMA luz pontual, mas pode haver várias (identificador 'L')
** - Qualquer número de objetos (sp, pl, cy)
**
** Durante o parsing do arquivo .rt, preenchemos esta estrutura. Durante
** o rendering, percorremos os objetos e luzes para calcular a imagem.
**
** As dimensões width e height definem a resolução da imagem final. Uma
** resolução maior (ex: 1920x1080) produz imagens mais detalhadas mas
** demora mais para renderizar, pois precisamos lançar mais raios.
** ============================================================================
*/

typedef struct s_scene
{
	t_camera	camera;       // A câmera que observa a cena (apenas uma)
	t_ambient	ambient;      // Luz ambiente da cena (apenas uma)
	t_light		*lights;      // Lista ligada de luzes pontuais (uma ou mais)
	t_object	*objects;     // Lista ligada de objetos (zero ou mais)
	int			width;        // Largura da imagem em pixels
	int			height;       // Altura da imagem em pixels
}	t_scene;

/* ========================================================================== */
/*                      ESTRUTURAS DA INTERFACE GRÁFICA                       */
/* ========================================================================== */

/*
** ============================================================================
** ESTRUTURA: t_mlx_data
** ============================================================================
**
** A MiniLibX (MLX) é uma biblioteca gráfica simples usada em muitos projetos
** da 42. Ela fornece funções para criar janelas, manipular imagens e
** gerenciar eventos (teclado, mouse, etc).
**
** Esta estrutura encapsula todos os ponteiros e dados necessários para
** trabalhar com a MLX:
**
** - mlx_ptr: Ponteiro para a instância principal da MLX, obtido via mlx_init()
**
** - win_ptr: Ponteiro para a janela criada com mlx_new_window()
**
** - img_ptr: Ponteiro para a imagem onde desenhamos os pixels
**
** - img_data: Array de chars que contém os dados brutos dos pixels.
**   Cada pixel é representado por 3 ou 4 bytes (RGB ou RGBA).
**   Modificamos este array diretamente para definir cores de pixels.
**
** - bits_per_pixel: Quantos bits são usados por pixel (tipicamente 32)
**
** - line_length: Quantos bytes tem cada linha da imagem. Nem sempre é
**   simplesmente width * bytes_per_pixel porque pode haver padding.
**
** - endian: Indica a ordem dos bytes (big-endian ou little-endian).
**   Afeta como interpretamos valores de cor de múltiplos bytes.
**
** O fluxo típico é:
** 1. mlx_init() -> preenche mlx_ptr
** 2. mlx_new_window() -> preenche win_ptr
** 3. mlx_new_image() -> preenche img_ptr
** 4. mlx_get_data_addr() -> preenche img_data e outros campos
** 5. Modificamos img_data pixel por pixel
** 6. mlx_put_image_to_window() para exibir
** ============================================================================
*/

typedef struct s_mlx_data
{
	mlx_t		*mlx_ptr;         // Ponteiro para instância da MLX
	void		*win_ptr;         // Ponteiro para a janela
	mlx_image_t	*img_ptr;         // Ponteiro para a imagem
	uint8_t		*pixels;        // Array de pixels (dados brutos da imagem)
	int			bits_per_pixel;   // Bits usados por cada pixel
	int			line_length;      // Bytes por linha (com padding possível)
	int			endian;           // Ordem dos bytes (0=little, 1=big endian)
}	t_mlx_data;
typedef struct s_minirt
{
	t_scene		scene;
	t_mlx_data	mlx;
}	t_minirt;

t_ray		ray_create(t_point3 origin, t_vec3 direction);
void		render_scene(t_minirt *rt);
t_color		calculate_lighting(t_scene *scene, t_hit_record *rec);
void		ft_free_split(char **split);
void		error_exit(const char *message);
void		ft_on_close(void *param);
void		ft_on_keypress(mlx_key_data_t keydata, void *param);
void		ft_on_resize(int32_t width, int32_t height, void *param);
#endif
