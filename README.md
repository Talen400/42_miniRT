# 42_miniRT

Um ray tracer escrito do zero em C como projeto da 42 School. O miniRT renderiza cenas 3D descritas em arquivos `.rt`, produzindo imagens com iluminação realista usando o modelo de Phong.

---

## O que é Ray Tracing?

Ray tracing é uma técnica de renderização que simula o comportamento da luz de forma física. Para cada pixel da tela, um raio é lançado da câmera em direção à cena. Quando o raio atinge um objeto, calculamos a cor do pixel com base na iluminação, sombras e propriedades do material.

A equação fundamental do raio:

```
P(t) = origin + t * direction
```

Quanto maior `t`, mais distante está o ponto ao longo do raio. O objeto mais próximo (menor `t` positivo) é o que fica visível naquele pixel.

---

## Compilação

```bash
# Versão obrigatória
make

# Versão bonus
make bonus

# Debug
make debug
```

**Dependências:** `cmake`, `make`, `cc`, `libglfw3-dev`

O MLX42 é compilado automaticamente via CMake na primeira vez.

---

## Uso

```bash
./miniRT scenes/complex.rt
./miniRT_bonus scenes/bonus_full.rt
```

- `ESC` ou fechar a janela encerra o programa
- A janela suporta redimensionamento

---

## src — Versão Obrigatória

O `src/` implementa um ray tracer completo com os seguintes módulos:

### Módulo: `main` / `events` / `mlx_utils` / `utils`
Inicialização da aplicação, loop MLX, eventos de teclado/janela e utilitários gerais.

### Módulo: `parser/`
Lê e valida o arquivo `.rt` linha a linha. Usa uma tabela de lookup (`g_element_parsers[]`) que mapeia identificadores (`A`, `C`, `L`, `sp`, `pl`, `cy`) para funções de parse específicas.

Um `t_parse_context` rastreia quantas câmeras e ambientes foram lidos, impedindo duplicatas. Erros abortam com mensagem descritiva.

Arquivos:
- `parser.c` — loop principal de leitura e despacho
- `parser_ambient.c`, `parser_camera.c`, `parser_light.c` — elementos globais
- `parser_sphere.c`, `parser_plane.c`, `parser_cylinder.c` — objetos geométricos
- `parser_camera_utils.c` — cálculo do sistema de coordenadas da câmera
- `parser_error.c`, `parser_utils.c` — erros e utilitários compartilhados

### Módulo: `scene/`
Gerencia a lista ligada de objetos (`t_object`) e inicialização/destruição da cena.

### Módulo: `vectors/`
Biblioteca de vetores 3D (`t_vec3`): soma, subtração, produto escalar, produto vetorial, normalização, comprimento.

### Módulo: `ray/`
Criação de raios (`t_ray = origin + t * direction`).

### Módulo: `intersect/`
Testa colisão de raios com cada tipo de objeto:
- `hit_sphere.c` — equação quadrática `(P-C)·(P-C) = r²`
- `hit_plane.c` — produto escalar `(P - point) · normal = 0`
- `hit_cylinder.c` — corpo cilíndrico + tampas superior e inferior
- `hit_scene.c` — itera todos os objetos, retorna o hit mais próximo
- `hit_record.c` — preenche o `t_hit_record` com ponto, normal, cor, face

### Módulo: `lighting/`
Implementa o modelo de iluminação de Phong:
- **Ambiente:** luz constante em todos os objetos
- **Difusa:** depende do ângulo entre a normal e a direção da luz
- **Especular:** reflexo brilhante dependente do ângulo de visão
- **Sombras:** raio shadow cast para cada luz

### Módulo: `color/`
Operações com `t_color` (RGBA): adição, subtração, escala, clamp, conversão para `uint32_t`.

### Módulo: `draw/`
Loop principal de renderização: para cada pixel `(x, y)`, gera um raio, chama `hit_scene`, e calcula a cor via `calculate_lighting`. Escreve o resultado no buffer de pixels da MLX.

---

## src_bonus — Versão Bonus

O `src_bonus/` expande o ray tracer com novos recursos:

### Diferenças em relação ao obrigatório

| Recurso | `src/` | `src_bonus/` |
|---|---|---|
| Esfera, Plano, Cilindro | ✓ | ✓ |
| Cone | ✗ | ✓ |
| Anti-aliasing (MSAA) | ✗ | ✓ |
| Reflexão recursiva | ✗ | ✓ |
| Propriedades de material por objeto | ✗ | ✓ |
| Múltiplas luzes com acumulação | básico | ✓ |
| Cor do céu (miss ray) | ✗ | ✓ |

### Novos arquivos bonus

- `draw_anti_aliasing.c` — supersampling: múltiplos raios por pixel com média de cor
- `hit_cone.c` — interseção raio-cone com equação quadrática e verificação de altura
- `intersect_utils.c` — utilitários compartilhados de interseção
- `parser_cone.c` — lê objetos `cn` do arquivo `.rt`
- `object_extra_args.c` — lê os parâmetros extras de material: `ks`, `shininess`, `reflectivity`

### Propriedades de material por objeto

Cada objeto pode ter (opcionalmente) parâmetros extras após a cor:

```
# Esfera com material completo
sp <pos> <diâmetro> <R,G,B> <ks> <shininess> <reflectivity>

# Plano com material completo
pl <pos> <normal> <R,G,B> <ka> <shininess> <reflectivity>
```

Defaults aplicados quando omitidos: `KA=0.2`, `KD=0.7`, `KS=0.2`, `SHININESS=30`, `REFLECTIVITY=0`.

### Reflexão recursiva

`calculate_lighting` aceita um parâmetro `depth`. A cada bounce, um novo raio é lançado na direção refletida. A profundidade máxima é `REFLECTION_DEPTH = 5`.

---

## Formato das Cenas (.rt)

O arquivo `.rt` define todos os elementos da cena. Linhas em branco e comentários com `#` são ignorados.

### Elementos globais (exatamente 1 de cada obrigatório)

```
A <intensidade> <R,G,B>
```
Luz ambiente. `intensidade` entre `[0.0, 1.0]`. Cor `RGB` entre `[0, 255]`.

```
C <x,y,z> <dx,dy,dz> <fov>
```
Câmera. Posição `x,y,z`, vetor de orientação normalizado `dx,dy,dz`, campo de visão `fov` em graus `[0, 180]`.

```
L <x,y,z> <brilho> <R,G,B>
```
Luz pontual. Pode haver múltiplas. `brilho` entre `[0.0, 1.0]`.

### Objetos geométricos (0 ou mais de cada)

```
sp <x,y,z> <diâmetro> <R,G,B>
```
Esfera. Centro `x,y,z`, diâmetro positivo.

```
pl <x,y,z> <nx,ny,nz> <R,G,B>
```
Plano infinito. Ponto no plano `x,y,z`, normal normalizada `nx,ny,nz`.

```
cy <x,y,z> <ax,ay,az> <diâmetro> <altura> <R,G,B>
```
Cilindro. Centro `x,y,z`, eixo `ax,ay,az`, diâmetro e altura positivos.

```
cn <x,y,z> <ax,ay,az> <diâmetro> <altura> <R,G,B>
```
Cone (bonus). Vértice `x,y,z`, eixo da ponta para a base.

### Parâmetros extras de material (bonus, opcionais)

Appended após a cor em qualquer objeto:

```
sp ... <R,G,B> <ks> <shininess> <reflectivity>
pl ... <R,G,B> <ka> <shininess> <reflectivity>
cy ... <R,G,B> <ks> <shininess> <reflectivity>
cn ... <R,G,B> <ks> <shininess> <reflectivity>
```

### Validações obrigatórias

- Exatamente 1 câmera (`C`) e 1 luz ambiente (`A`)
- Extensão do arquivo deve ser `.rt`
- Intensidades e brilho entre `[0.0, 1.0]`
- Cores RGB entre `[0, 255]`
- Vetores de orientação/eixo entre `[-1.0, 1.0]` por componente
- FOV entre `[0, 180]`
- Diâmetro e altura positivos

### Exemplo de cena

```
A 0.3 255,255,255

C 0,5,-20 0,-0.2,1 60

L -20,30,-10 0.8 255,255,255

sp 0,0,15 6.0 255,0,0
pl 0,-3,0 0,1,0 200,200,200
cy 0,-3,2 0,1,0 2.0 6.0 255,128,0
```

---

## TODO

### Obrigatório

- [x] Compilação com `make` / `make bonus` / `make clean` / `make fclean` / `make re`
- [x] Norminette (42 coding norm)
- [x] Sem leaks, sem crashes, sem comportamento indefinido
- [x] Parser de arquivo `.rt` com tratamento de erros
- [x] Validação de argumentos (exatamente 1 argumento, extensão `.rt`)
- [x] Elemento câmera (`C`) — exatamente 1 por arquivo
- [x] Elemento luz ambiente (`A`) — exatamente 1 por arquivo
- [x] Elemento luz pontual (`L`) — ao menos 1 por arquivo
- [x] Objeto esfera (`sp`) com intersecção de raio
- [x] Objeto plano (`pl`) com intersecção de raio
- [x] Objeto cilindro (`cy`) com tampas e corpo
- [x] Iluminação difusa (Phong — componente difusa)
- [x] Iluminação especular (Phong — componente especular)
- [x] Luz ambiente aplicada a todos os objetos
- [x] Sombras (shadow rays para cada luz)
- [x] Múltiplas luzes pontuais
- [x] Janela redimensionável
- [x] Fechar com ESC ou clique no X da janela
- [ ] Verificação final da norminette em todos os arquivos
- [ ] Testes de edge cases (valores extremos, arquivos inválidos)
- [ ] Validação com avaliador externo (submissão na Intra)

### Bonus

- [x] Reflexão especular — modelo de Phong completo (componente especular `ks` + `shininess` por objeto)
- [ ] Disrupção de cor — padrão de tabuleiro de xadrez (checkerboard) nos objetos
- [x] Luzes coloridas e multi-spot — múltiplas luzes com cores distintas e acumulação de contribuições
- [x] Segundo objeto de segundo grau — cone (`cn`) com intersecção de raio e verificação de altura
- [ ] Bump mapping — texturas de mapa de relevo para perturbação da normal

---

## Estrutura do Projeto

```
42_miniRT/
├── include/          Headers versão obrigatória
├── include_bonus/    Headers versão bonus
├── src/              Código fonte obrigatório (28 arquivos)
├── src_bonus/        Código fonte bonus (33 arquivos)
├── scenes/           Arquivos .rt de exemplo e teste
├── libft/            Biblioteca C customizada
└── MLX42/            Biblioteca gráfica (submodule)
```

---

## Autores

- **rgregori** — rgregori@student.42sp.org.br
- **tlavared** — tlavared@student.42sp.org.br