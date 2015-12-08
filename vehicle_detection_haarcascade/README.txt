--- Informações Gerais ---
Essa pasta contem o exemplo de deteccao de veiculo usando haar cascades. 

TODOS OS DIREITOS AUTORAIS SÃO DO AUTOR DO CÓDIGO, EM NOSSO PROJETO ESTAMOS APENAS PROCURANDO SOLUÇÕES QUE FUNCIONEM PARA CONTAR CARROS

Fonte: https://www.behance.net/gallery/Vehicle-Detection-Tracking-and-Counting/4057777

Baseado no paper: Oliveira, M.; Santos, V. Automatic Detection of Cars in Real Roads using Haar-like Features 
(disponivel em https://sites.google.com/site/andrewssobral/Automatic_Detection_of_Cars_in_Real_Roads_using_Haar-like_Features.pdf?attredirects=0 )

--- Compilando com CMake ---
cd unb-pvc-project-cars/vehicle_detection_haarcascade
mkdir build
cd build
sudo apt-get install cmake

Digite "cmake -h" para ver a lista disponivel
	cmake -G "<generator_name>" ../
generator_name: nome da interface onde voce deseja desenvolver, para gerar makefiles para unix use
	cmake -G "Unix Makefiles" ../

Dentro da pasta build digite:
	make
Para executar ./<nomeexecutavel> <path_to_cars3.xml> <path_to_video>

--- Usando Shell para rodar todos os videos ---
Leia as instruções dentro do run_videos.sh



