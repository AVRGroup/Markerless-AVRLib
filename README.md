# Markerless AVRLib
Desenvolvimento da biblioteca Markerless AVRLib para Realidade Aumentada sem marcadores

## Primeira Versão Funcional

### Sobre
* A biblioteca, em sua primeira versão funcional, é composta por 5 módulos: __Application, Camera, Core, Track e View__;
* Cada projeto possui diferentes __targets__ para criação de _libs_ ou executáveis em modo _debug_ ou _release_;
* Exceto a __Application__, os demais módulos geram _static libraries_ que são _linkadas_ pela __Application__;
* As aplicações no momento devem ser implementadas no projeto __Application__;

### Diretórios
* __data__		Algumas imagens usadas como marcadores, arquivo de configuração da câmera e vídeos das cenas testadas;
* __lib__		_libs_ usadas pela biblioteca e as _libs_ da própria biblioteca;
* __modules__	Código fonte de cada módulo;
* __samples__	No momento possui uma aplicação externa, que utiliza o OpenCV diretamente;
* __include__	_headers_ de bibliotecas externas;
* __bin__		Binários gerados pela compilação do sistema;

### Instruções de Implementação
* Abra o projeto __App Module.cbp__ e edite o arquivo __main.cpp__
* Os passos para criar uma aplicação com a __Markerless AVRLib__ são:
	* Criar uma especificação da classe Model para renderização do modelo virtual
	* Construir um objeto __Application__ utilizando a interface __Application::Builder__
	* Iniciar a aplicação por meio da chamada para o método __Application::Start()__
* Na construção do objeto são definidos os marcadores, arquivo da câmera, algoritmos utilizados e o vídeo (gravado ou webcam)
	* Para os marcadores são necessários o arquivo com sua imagem, preferencialmente ortogonal, e a instância do objeto Model
	* Os algoritmos podem ser definidos criando-se um objeto de __SystemAlgorithms__ ou definindo as _flags_ de otimização
		* Otimização por performance utiliza detector STAR, extrator SURF e _matcher brute force_ L1
		* Otimização por qualidade utiliza detector e extrator SIFT e _matcher brute force_ L2
		* Otimização por balanceamento utiliza detector SIFT, extrator BRISK e _matcher brute force Hamming_
	* Definindo um vídeo pré-gravado a biblioteca fará o precessamento sobre ele, caso contrário, a webcam é aberta
* Mais detalhes podem ser encontrados na documentação do código