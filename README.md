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
* Implementação da integração numérica completa (e reduzida para o elemento quadrático);
* Extrapolação dos valores de tensão e deformação, calculado nos pontos de Gaus, para os valores nodais;
* Avaliação dos resultadsos e comparação dos mesmos com os do software Abaqus.

# Método de Análise

## O procedimento realizado pelo algoritmo desenvolvido pode ser resumido nas seguintes etapas:
* Leitura do arquivo de entrada (proveniente do software Abaqus);
* Calculo das matrizes de rigidez local, e computação das mesmas para matriz global;
* Solução do sistema para o cálculo dos deslocamentos, fezendo uso do método das penalidades;
* Cálculo das forças nodais, fazendo uso dos deslocamentos obtidos;
* Cálculo das tensões e deformações nos pontos de Gauss;
* Estrapolação e suavização dos valores de tensão e deformação para os nós;
* Impressão dos resultados de interesse;
* Comparação dos resultados obtidos com os do software Abaqus.

# Resultados

## Implementação Computacional dos Elementos T6 e Q4

* As implementações dos elementos, juntamente com um código de análise de problemas usando o Método dos Elementos Finitos, foram feitas fazendo-se uso da linguagem de programação C++;
* Estudou-se e fez-se uso da biblioteca Eigen, que possui métodos e ferramentas para usa da algebra linear;
* Estudou-se e fez-se uso do framework Doxygen para geração da documentação da ferramenta computacional desenvolvida, juntamente com o relatório;
* Os códigos e documentações seguem junto a esse PDF, podendo ser acessados de maneira iterativa através do arquivo HTML, onde toda documentação e comentários se encontram;
* Foram implementadas duas classes de elementos (T6 e Q4), uma classe para os nós e uma classe para a análise estrutural;
* A implementação apresentada faz uso do arquivo de entrada do software comercial Abaqus, afim de tirar proveito da ferramenta de geração de malhas do mesmo;
* A ferramenta desenvolvida nesse trabalho faz leitura completa do arquivo de entrada do Abaqus, possibilitando obter todas as informações de interesse (carregamentos, condições de contorno, conectividade e tipo dos elementos, coordenadoas dos nós, etc).;
* Os resultados obtidos através dessa implementação são verificados através do Abaqus, e serão apresentados posteriormente;
* Os resultados dos demais testes realizados na implementação do trabalho encontram-se em um arquivo de texto em anexo;
* Fez-se uso da plataforma Github para desenvolvimento do projeto, que será continuado e melhorado em etapas futuras.

## Resultados da Implementação computacional

* A partir da ferramenta para análise através do Método dos Elementos finitos, resolveu-se o problema proposto com variados valores de refinamento de malha. Os resultados obtidos foram comparados aos do software Abaqus, como mostrado nas Figuras a seguir (comparação entre valores aferidos para os nós engastados e submetidos ao carregamento de pressão). Uma vez que o Abaqus não possibilita a integração reduzida do elementod do tipo T6, foram apenas comparados os resultados de integração completa para ambos os elementos;
* O elemento do tipo Q4 obteve os deslocamentos com erro muito baixo, tomando como referência os resultados do Abaqus;
* O elemento do tipo T6, por sua vez, representou melhor os valores de tensão e deformação nos pontos.


### Elementos do tipo Q4
###  ![](/Figuras/Tab_Q4_16.png)
@image latex Figuras/Tab_Q4_16.png
@image html Figuras/Tab_Q4_16.png

###  ![](/Figuras/Tab_Q4_39.png)
@image latex Figuras/Tab_Q4_39.png
@image html Figuras/Tab_Q4_39.png

###  ![](/Figuras/Tab_Q4_871.png)
@image latex Figuras/Tab_Q4_871.png
@image html Figuras/Tab_Q4_871.png

### Elementos do tipo T6
###  ![](/Figuras/Tab_T6_40.png)
@image latex Figuras/Tab_T6_40.png
@image html Figuras/Tab_T6_40.png

###  ![](/Figuras/Tab_T6_44.png)
@image latex Figuras/Tab_T6_44.png
@image html Figuras/Tab_T6_44.png

## Analises de MEF através do Abaqus

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

* Os objetivos propostos para o trabalho foram alcançados, obtendo assim uma ferramenta computacional capaz de realizar uma análise estrutural utilizando o método dos elementos finitos;
* Dentre as funcionalidades implementadas, se destacam as classes de elementos Q4 e T6, a leitura completa de arquivos gerados pelo software como entrada da simulação, implementação da integração numérica, métodos necssário para os cálculos da análise de MEF (calculo da rigidez local e global, vetor de forças, aplicação das condições de contorno, etc) e extrapolação e suavização dos valores de tensão e deformação (calculado nos pontos de Gauss) para os nós;
* Os resultados obtidos para ambos os elementos implementados se mostraram condizentes com os do software comercial Abaqus, apesar de certas discrepâncias observadas;
* A partir dos resultados, avaliou-se a influência do refinamento da malha nos resultados das análises de elementos finitos. Malhas mais refinadas, além de fornecerem resultados com maior grau de suavidade e continuidade, apresentão um comportamento menos rígido em relação as malhas menos refinadas. Apesar de ser uma diferença não tão pronunciada, foi observado que elementos mais refinados tem tanto deformações quanto tensões mais acentuadas;
* Foi constatado também que, para os elementos do tipo T6, a qualidade da resposta é inferior ao Q4, apesar das funções de interpolação quadrádicas utilizadas;


## Referencias

[Eigen Website - Instalation](http://eigen.tuxfamily.org/index.php?title=IDEs#Visual_Studio)
