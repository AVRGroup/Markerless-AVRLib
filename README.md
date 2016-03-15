# Markerless AVRLib
Desenvolvimento da biblioteca Markerless AVRLib para Realidade Aumentada sem marcadores

* [Primeira Versão Funcional](#primeira-versão-funcional)
	* [Sobre](#sobre)
	* [Diretórios](#diretórios)
	* [Instruções de Implementação](#instruções-de-implementação)
* [Configuração do OpenCV](#configuração-do-opencv)
	* [Diretórios](#diretórios)
	* [Downloads](#downloads)
	* [Instalação](#instalação)
	* [OpenCV Flags](#opencv-flags)

## Primeira Versão Funcional

### Sobre
* A _Markerless AVRLib_ é implementada em C++11. Utiliza os algoritmos de visão computacional presentes no OpenCV. Foi compilada com o _MinGW G++_ 4.9 utilizando a IDE _Code::Blocks_ 16.01;
* A biblioteca, em sua primeira versão, é composta por 5 módulos: __Application, Camera, Core, Track e View__;
* Cada projeto possui diferentes __targets__ para criação de _libs_ ou executáveis em modo _debug_ ou _release_;
* Exceto a __Application__, os demais módulos geram _static libraries_ que são _linkadas_ pela __Application__;
* As aplicações no momento devem ser implementadas no projeto __Application__;

### Diretórios
* __bin__		Binários gerados pela compilação do sistema;
* __data__		Algumas imagens usadas como marcadores, arquivo de configuração da câmera e vídeos das cenas testadas;
* __include__	_headers_ de bibliotecas externas;
* __lib__		_libs_ usadas pela biblioteca e as _libs_ da própria biblioteca;
* __modules__	Código fonte de cada módulo;
* __samples__	No momento possui uma aplicação externa, que utiliza o OpenCV diretamente;

### Instruções de Implementação
* Abra o projeto __App Module.cbp__ e edite o arquivo __main.cpp__
* Os passos para criar uma aplicação com a __Markerless AVRLib__ são:
	* Criar uma especificação da classe Model para renderização do modelo virtual
	* Construir um objeto __Application__ utilizando a interface __Application::Builder__
	* Iniciar a aplicação por meio da chamada para o método __Application::Start()__
* Na construção do objeto são definidos os marcadores, arquivo da câmera, algoritmos, o vídeo entre outros
	* Para os marcadores são necessários o arquivo com sua imagem, preferencialmente ortogonal, e a instância do objeto Model
	* Os algoritmos podem ser definidos criando-se um objeto de __SystemAlgorithms__ ou definindo as _flags_ de otimização
		* Otimização por performance utiliza detector STAR, extrator SURF e _matcher brute force_ L1
		* Otimização por qualidade utiliza detector e extrator SIFT e _matcher brute force_ L2
		* Otimização por balanceamento utiliza detector SIFT, extrator BRISK e _matcher brute force Hamming_
	* Definindo um vídeo pré-gravado a biblioteca fará o precessamento sobre ele, caso contrário, a webcam é aberta
* Mais detalhes podem ser encontrados na documentação do código

## Configuração do OpenCV

### Diretórios
A seguir são listados alguns diretórios utilizados no processo de instação e configuração do opencv. Recomenda-se utilizar os mesmos diretórios sugeridos, dipensando alterações nos projetos da AVRLib, mas pode-se utilizar quaisquer outros caminhos.

* %MINGW_PATH%		Diretório de instalação do compilador MinGW;
* %OPENCV_PATH%		Diretório com os arquivos do opencv baixados e extraídos. Sugestão: __C:\opencv__;
* %OPENCV_CMAKE%	Diretório onde serão gerados os arquivos _cmake_. Sugestão: __%OPENCV_PATH%/build/x86/mingw__
* %OPENCV_INSTALL%	Diretório onde serão gerados os _headers_ e binários do opencv. Sugestão: __%OPENCV_PATH%/install__

### _Downloads_
São necessários os seguintes _softwares_:
* OpenCV superior a 2.4.9 e inferior a 3.0 (a instalação das versões 3.x é diferente) \[[Download Page](http://opencv.org/downloads.html)\];
* CMake 3.0 ou superior \[[Download Page](https://cmake.org/)\];
* MinGW 4.7 ou superior (suporte ao C++11). Recomenda-se usar o compilador que vem junto à IDE [_Code::Blocks_](http://www.codeblocks.org/downloads/26 "Download CodeBlocks") na versão 16.01;

### Instalação
* Adicione o diretório __%MINGW_PATH%/bin__ na variável de ambiente __PATH__;
* Extraia os arquivos do opencv para %OPENCV_PATH%
* Execute o _CMake_ e siga os seguintes passos:
	* Em _where is the source code_ adicione o diretório __%OPENCV_PATH/source%__
	* Em _where to build the binaries_ adicione o diretório __%OPENCV_CMAKE%__
	* Clique em _Configure_ 
	* Selecione:
		* __CodeBlocks - MinGW Makefiles__, se utiliza o mingw que vem junto ao C::B;
		* __MinGW Makefiles__, caso contrário;
	* Mantenha a opção _Use default native compilers_ marcada, clique em _Finish_ e aguarde;
	* Algumas flags de configuração do opencv são exibidas, leia a seção [__OpenCV Flags__](#opencv-flags) antes de proseguir;
	* Clique em _Generate_;
	* Se aparecer a mensagem _Generating done_ então as configurações foram realizadas com sucesso, caso contrário corrija os erros e tente novamente;
* Abra o __prompt de comando__ e navegue até a pasta %OPENCV_CMAKE%;
* Execute o comando __mingw32-make__ e aguarde (pode levar vários minutos);
* Execute o comando __mingw32-make install__ e aguarde alguns segundos;
* Adicione o diretório __%OPENCV_INSTALL/.../bin%__ na variável de ambiente __PATH__;
* Por fim, abra o projeto __App Module.cbp__, edite os caminhos para os arquivos do opencv e tente compilar o projeto;

### OpenCV Flags
Pelo CMake é possível configurar algumas características do OpenCV antes de construi-lo. A seguir são listadas algumas _flags_ mais importantes neste projeto.

* BUILD_EXAMPLES	constroi os binários para os exemplos da biblioteca (desmarque para diminuir o tempo de instalação);
* BUILD_PERF_TESTS	constroi os binários para os testes de performance (desmarque);
* BUILD_opencv_apps	constroi os binários para aplicações utilitárias (desmarque);
* CMAKE_CODEBLOCKS_EXECUTABLE	caminho do executável do codeblocks (adicione se não for encontrado);
* CMAKE_INSTALL_PREFIX			caminho de instalação (altere o valor para __%OPENCV_INSTALL%__);
* WITH_XXXX						suporte à bibliotecas e _frameworks_ externos (marque as opções desejadas ou mantenha o que vier por padrão).

Clique em _Configure_ para aplicar as alterações.
