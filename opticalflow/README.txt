A pasta contem um arquivo Makefile que primeiro limpa a tela, e depois compila o programa sobre as seguites diretivas:

	g++ `pkg-config --cflags --libs opencv2` -I'include/'  -o OpticalFlowHS OpticalFlowHS.cpp src/HS.cpp src/segmentation.cpp src/flowCore.cpp `pkg-config --libs opencv2` 

O resultado é o executavel "OpticalFlowHS", que deve ser chamado como:
	
	./OpticalFlowHS <path/to/video/file>

-------------------------------------------------------------------- 

Para reproduzir as condições referênciadas no relatório, deve-se abrir o arquivo OpticalFlowHS.cpp e alterar os parâmetros em #define. 
Sendo que:
	LAMBDA    - fator de suavizacao da funcao cvCalcOpticalFlowHS();
    SMOOTH    - Kernel size e desvio padrão da operação GaussianBlur;
    THESH_VEL - Valor mínimo para velocidades.
    MINAREA   - Rectangles with area below won`t be printed
    MAXAREA	  - Rectangles with area above won`t be printed
