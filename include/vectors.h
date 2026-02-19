#ifndef VECTORS_H
# define VECTORS_H

/*
** ============================================================================
** ESTRUTURA: t_vec3
** ============================================================================
** 
** A estrutura mais fundamental do ray tracing. Um vetor tridimensional pode
** representar três conceitos diferentes dependendo do contexto:
**
** 1. POSIÇÃO/PONTO: Um local no espaço 3D (x, y, z)
**    Exemplo: posição da câmera em (0, 0, -5)
**
** 2. DIREÇÃO: Um vetor que aponta para algum lugar
**    Exemplo: direção que a câmera está olhando (0, 0, 1)
**    Normalmente é normalizado (comprimento = 1)
**
** 3. COR: Valores RGB onde x=Red, y=Green, z=Blue
**    Exemplo: vermelho puro = (255, 0, 0) ou (1.0, 0.0, 0.0)
**
** Esta versatilidade torna o código mais limpo e reutilizável, pois as
** mesmas operações matemáticas (soma, subtração, multiplicação) se aplicam
** a todos esses usos.
**
** Usamos 'double' em vez de 'float' porque precisamos de alta precisão
** para cálculos de interseção. Erros de arredondamento podem causar
** artefatos visuais desagradáveis.
** ============================================================================
*/

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

t_vec3		vec3_create(double x, double y, double z);
t_vec3		vec3_add(t_vec3 a, t_vec3 b);
t_vec3		vec3_subtract(t_vec3 a, t_vec3 b);
t_vec3		vec3_multiply(t_vec3 v, double scalar);
t_vec3		vec3_divide(t_vec3 v, double scalar);
double		vec3_dot(t_vec3 a, t_vec3 b);
t_vec3		vec3_cross(t_vec3 a, t_vec3 b);
double		vec3_length(t_vec3 v);
t_vec3		vec3_normalize(t_vec3 v);

#endif
