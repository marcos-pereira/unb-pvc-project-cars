Princípios de Visão Computacional - 02/2015
Projeto Final da Disciplina
Grupo 2

Detecção e contagem de veículos

- Descrição

Este diretório contém os arquivos referentes à execução dos procedimentos para Grount Truth, ou seja, geração do ground truth em arquivos de texto, redesenho dos ground truths de acordo com as informações presentes nos arquivos de texto, geração de imagens binárias para visualização das marcações realizadas e cálculo do match entre o arquivo de texto obtido no ground truth e de um arquivo de saída de um dos métodos.

- Compilação

Para realizar a compilação dos códigos neste diretório basta executar o seguinte comando terminal dentro deste diretório, os diretórios e links para compilação já estão presentes nesse arquivo

	make

- Execução

Para executar o código que gera o ground truth é necessário que o vídeo a ser analisado esteja na pasta e o video esteja com o seguinte nome: 1.avi, 2.avi, 3.avi até 20.avi por exemplo. Para executar basta:

	./GroundTruth 1.avi

Para executar o código que redesenha o ground truth nos frames do vídeo basta executar o seguinte comando no terminal dentro deste diretório (o arquivo .txt, 1_avi.txt por exemplo, gerado pelo ground truth necessita estar dentro do diretório juntamente com o video):

	./getGT 1.avi

Para executar o código que exibe as seleções feitas no ground truth de forma binária, para que o usuário possa analisar suas regiões de forma mais simples, basta executar o seguinte comando dentro deste diretório (o arquivo .txt gerado pelo ground truth necessita estar dentro deste diretório):

	./DrawBinaries 1.avi

Para executar o código que realiza o match entre os arquivos de texto do ground truth e dos arquivos de saída dos métodos aplicados, basta executar o seguinte comando no terminal dentro deste diretório (os arquivos necessários para executar este código são, por exemplo, 1_avi.txt sendo o arquivo gerado pelo ground truth e 1_avi2.txt sendo o arquivo gerado como saída por um dos métodos de detecção):

	./MatchResult 1.avi
