--- Informações Gerais ---
Essa pasta conterá o código para criar e definir os arquivos de groundtruth propostos pelo professor e discutidas no final da aula do dia 11nov.

Ela não conterá os vídeos, nela estará presente apenas o código que fará leitura do vídeo, marcará objetos de interesse, armazenará o objeto usando MouseCallBack e bounding boxes e salvará as informações em um arquivo para posteriores comparações e análise de resultados.

--- Compilando com CMake ---
cd unb-pvc-project-cars/groundtruth
mkdir build
cd build
sudo apt-get install cmake

Digite "cmake -h" para ver a lista disponivel
	cmake -G "<generator_name>" ../
generator_name: nome da interface onde voce deseja desenvolver, para gerar makefiles para unix use
	cmake -G "Unix Makefiles" ../

Dentro da pasta build digite:
	make
Para executar ./rect_groundtruth



