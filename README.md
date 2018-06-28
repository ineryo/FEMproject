<!-- $theme: gaia -->
# FEMproject
  Trabaho Final da Disciplina de Introdução ao Método dos Elementos Finitos

# Autores 
  * Igor de Melo Nery de Oliveira
  * Lucas Gouveia Omena Lopes

# Projeto Final

* Implementação dos Elementos Finitos Q4 e T6 para resolução de um problema de uma estrutura submetida ao estado plano de tensões.

# Problema Estudado

Pilar engastado na base, e submetido a uma carga distribuída vertical no topo:

##  ![](/Figuras/problema.png)
@image latex Figuras/problema.png
@image html Figuras/problema.png

# Elementos em Estudo 

##### Elemento Q4:
* Elemento Quadilateral de 4 nós

##  ![](/Figuras/elementQ4.png)
@image latex Figuras/elementQ4.png
@image html Figuras/elementQ4.png

##### Elemento T6:
* Elemento Triangular de 6 nós

##  ![](/Figuras/T6.png)
@image latex Figuras/T6.png
@image html Figuras/T6.png


# Objetivos

* Implementação e uso dos elementos Q4 e T6 na resolução do problema proposto;
* Avaliação dos resultadsos e comparação dos mesmos com os do software Abaqus.


# Resultados

## Implementação Computacional dos Elementos T6 e Q4

* As implementações dos elementos, juntamente com um código de análise de problemas usando o Método dos Elementos Finitos, foram feitas fazendo-se uso da linguagem de programação C++;
* Os códigos e documentações seguem junto a esse PDF, podendo ser acessados de maneira iterativa através do arquivo HTML, onde toda documentação e comentários se encontram.
* A implementação apresentada faz uso do arquivo de entrada do software comercial Abaqus, afim de tirar proveito da ferramenta de geração de malhas do mesmo;
* Os resultados obtidos através dessa implementação são verificados através do Abaqus, e serão apresentados posteriormente.

## Abaqus

* Foram testados exemplos, utilizando como material o aço (Modulo de Young 200 GPa e coeficiente de Poisson 0.3), e estudados os efeitos do tipo de elemento e refinamento da malha nos resultados obtidos. Esses resultados, visulaizados através das ferramentas do software Abaqus. Os campos avaliados foram os deslocamentos (U),tensões (S), reações de apoio (RF) e deformações na estrutura (E).

### Deslocamentos
###  ![](/Figuras/45U.png)
@image latex Figuras/45U.png
@image html Figuras/45U.png

### Tensões
###  ![](/Figuras/45S.png)
@image latex Figuras/45S.png
@image html Figuras/45S.png

### Reações
###  ![](/Figuras/45RF.png)
@image latex Figuras/45RF.png
@image html Figuras/45RF.png

### Deformações
###  ![](/Figuras/45E.png)
@image latex Figuras/45E.png
@image html Figuras/45E.png

## Influência do Número de Elementos
* Para avaliarmos a interferência do refinamento da malha nos resultados obtidos, realizou-se um estudo das tensões e deformações medidas nos elementos. Esse estudo pode ser fisto nas figuras a seguir (simulações do Abqus para variadas divisões de elementos), e no gráfico, que mostra a influência do resultado de acordo com o número de elementos.

###  ![](/Figuras/tabela.png)
@image latex Figuras/tabela.png
@image html Figuras/tabela.png

### Q4
###  ![](/Figuras/stress.png)
@image latex Figuras/stress.png
@image html Figuras/stress.png

###  ![](/Figuras/strain.png)
@image latex Figuras/strain.png
@image html Figuras/strain.png

### T6
###  ![](/Figuras/stress_t6.png)
@image latex Figuras/stress_t6.png
@image html Figuras/stress_t6.png

###  ![](/Figuras/strain_t6.png)
@image latex Figuras/strain_t6.png
@image html Figuras/strain_t6.png

## Gráfico Número de Elementos x Tensões Máximas

### Q4
###  ![](/Figuras/q4_elementxstress.png)
@image latex Figuras/q4_elementxstress.png
@image html Figuras/q4_elementxstress.png


### T6
###  ![](/Figuras/t6_elementxstress.png)
@image latex Figuras/t6_elementxstress.png
@image html Figuras/t6_elementxstress.png

## Gráfico Número de Elementos x Deformações Máximas

### Q4
###  ![](/Figuras/q4_elementxstrain.png)
@image latex Figuras/q4_elementxstrain.png
@image html Figuras/q4_elementxstrain.png


### T6
###  ![](/Figuras/t6_elementxstrain.png)
@image latex Figuras/t6_elementxstrain.png
@image html Figuras/t6_elementxstrain.png


# Análise dos resultados

* A partir dos resultados, avaliou-se a influência do refinamento da malha nos resultados das análises de elementos finitos. Malhas mais refinadas, além de fornecerem resultados com maior grau de suavidade e continuidade, apresentão um comportamento menos rígido em relação as malhas menos refinadas. Apesar de ser uma diferença não tão pronunciada, foi observado que elementos mais refinados tem tanto deformações quanto tensões mais acentuadas.
* Foi constatado também que, para os elementos do tipo T6, a qualidade da resposta é inferior ao Q4, apesar das funções de interpolação quadrádicas utilizadas.


## Referencias até o momento

[Eigen Website - Instalation](http://eigen.tuxfamily.org/index.php?title=IDEs#Visual_Studio)
