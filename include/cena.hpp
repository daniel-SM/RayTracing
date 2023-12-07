#ifndef CENA_HPP
#define CENA_HPP

#include "basics.hpp"
#include "objetos.hpp"
#include "complexos.hpp"
#include "transformacoes.hpp"
#include "info.hpp"
// #include <SDL_image.h>

namespace Cena
{
	// Materiais dos objetos 
	//
	// Material do Plano do Chao ***********************************************
	// Propriedades de reflectividade do plano do chao
	Vetor kaChao { 0.2, 0.7, 0.2 }; // Prop ambiente do material do chao 
	Vetor kdChao = kaChao; // Prop difusa do material do chao 
	Vetor keChao { 0.0, 0.0, 0.0 }; // Prop especular do material do chao 
	double brilhoChao = 1; // Fator de brilho espescular
	// Material do plano do chao
	Material materialChao { kaChao, kdChao, keChao, brilhoChao };

	// Material do Plano de Fundo **********************************************
	// Propriedades de reflectividade do plano de fundo
	Vetor kaFundo { 0.3, 0.3, 0.7 }; // Prop ambiente do material do fundo 
	Vetor kdFundo = kaFundo; // Prop difusa do material do fundo 
	Vetor keFundo { 0.0, 0.0, 0.0 }; // Prop especular do material do fundo 
	double brilhoFundo = 1; // Fator de brilho especular
	// Material do plano de fundo
	Material materialFundo { kaFundo, kdFundo, keFundo, brilhoFundo };

	// Material da Casa ********************************************************
	// Propriedades de reflectividade da Casa
	Vetor kaCasa = { 1.0, 1.0, 1.0 }; // Prop ambiente do material da Casa
	Vetor kdCasa = kaCasa; // Prop difusa do material da Casa
	Vetor keCasa = kaCasa; // Prop especular do material da Casa
	double brilhoCasa = 10; // Fator de brilho especular
	// Material da Casa
	Material materialCasa { kaCasa, kdCasa, keCasa, brilhoCasa };

	// Material do Telhado *****************************************************
	// Propriedades de reflectividade do Telhado
	Vetor kaTelhado = { 0.7, 0.4, 0.3 }; // Prop ambiente do material do Telhado
	Vetor kdTelhado = kaTelhado; // Prop difusa do material do Telhado
	Vetor keTelhado = kaTelhado; // Prop especular do material do Telhado
	double brilhoTelhado = 5; // Fator de brilho especular
	// Material do Telhado
	Material materialTelhado { kaTelhado, kdTelhado, keTelhado, brilhoTelhado };

	// Material do Poste *******************************************************
	// Propriedades de reflectividade do Poste
	Vetor kaPoste = { 0.4, 0.4, 0.4 }; // Prop ambiente do material do Poste
	Vetor kdPoste = kaPoste; // Prop difusa do material do Poste
	Vetor kePoste = { 0.5, 0.5, 0.5 }; // Prop especular do material do Poste
	double brilhoPoste = 5; // Fator de brilho especular
	// Material do Poste
	Material materialPoste { kaPoste, kdPoste, kePoste, brilhoPoste };

	// Material da Arvore ******************************************************
	// Propriedades de reflectividade da Arvore
	Vetor kaArvore = { 0.0, 1.0, 0.0 }; // Prop ambiente do material da Arvore
	Vetor kdArvore = kaArvore; // Prop difusa do material da Arvore
	Vetor keArvore = kaArvore; // Prop especular do material da Arvore
	double brilhoArvore = 5; // Fator de brilho especular
	// Material da Arvore
	Material materialArv { kaArvore, kdArvore, keArvore, brilhoArvore };

	// Material do Tronco ******************************************************
	// Propriedades de reflectividade do Tronco
	Vetor kaTronco = { 0.4, 0.3, 0.1 }; // Prop ambiente do material do Tronco
	Vetor kdTronco = kaTronco; // Prop difusa do material do Tronco
	Vetor keTronco = kaTronco; // Prop especular do material do Tronco
	double brilhoTronco = 5; // Fator de brilho especular
	// Material do Tronco
	Material materialTronco { kaTronco, kdTronco, keTronco, brilhoTronco };

	// *************************************************************************
	// Objetos da Cena *********************************************************
	//
	// Objetos em coordenadas de mundo onde todos objetos estao 
	// no primeiro octante do sistema de coordenadas. O plano do 
	// chao é o plano XZ (y = 0) e os planos XY (z = 0) e YZ (x = 0) 
	// sao delimitantes de fundo para o cenario
	//
	// Informacoes do Plano do Chao ********************************************
	Ponto pontoChao { 0, 0, 0 }; // Ponto presente no plano do chao
	Vetor normalChao { 0, 1, 0 }; // Vetor normal ao plano do chao
	// Objeto do Plano do Chao
	Plano chao (pontoChao, normalChao, materialChao);

	// Informacoes do Plano do Fundo Esquerdo **********************************
	Ponto pontoFundoEsq { 0, 0, 0 }; // Ponto do plano do Fundo Esquerdo
	Vetor normalFundoEsq { 1, 0, 0 }; // Vetor normal ao plano do Fundo Esquerdo
	// Objeto do Plano do Fundo Esquerdo
	Plano fundoesq (pontoFundoEsq, normalFundoEsq, materialFundo);

	// Informacoes do Plano do Fundo Direito ***********************************
	Ponto pontoFundoDir { 0, 0, 0 }; // Ponto presente no plano do fundo Direito
	Vetor normalFundoDir { 0, 0, 1 }; // Vetor normal ao plano do fundo Direito
	// Objeto do Plano do Fundo Direito
	Plano fundodir (pontoFundoDir, normalFundoDir, materialFundo);

	// Objeto Complexo da Casa *************************************************
	// Composto de um cubo e uma piramide
	Casa casa (materialCasa, materialTelhado);
	// Objeto Complexo da Arvore ***********************************************
	double alturaArv = 500;
	double raioFolha = 200;
	double raioTronco = 100;
	// Composto de uma esfera e um cone
	Arvore arvore1 (alturaArv, raioFolha, raioTronco, materialArv, materialTronco);
	Arvore arvore2 (alturaArv, raioFolha, raioTronco, materialArv, materialTronco);
	// Objeto Complexo do Poste ************************************************
	double alturaVer = 500;
	double alturaHor = 100;
	double raioVer = 12;
	double raioHor = 6;
	// Composto de dois cilindro
	Poste poste (alturaVer, alturaHor, raioVer, raioHor, materialPoste);

	// Textura *****************************************************************
	// Carregando a imagens das texturas 
	SDL_Surface* img_door = SDL_LoadBMP("door_1348x1500.bmp"); 
	SDL_Surface* img_sky = SDL_LoadBMP("sky_5960x2800.bmp"); 
	Ponto esq_inf { 0, 0, 0 }; // Define o ponto inferior esquerdo 
	Ponto dir_inf { 1, 0, 0 }; // Define o ponto inferior direito 
	Ponto dir_sup { 1, 1, 0 }; // Define o ponto superior direito 
	Ponto esq_sup { 0, 1, 0 }; // Define o ponto superior esquerdo 
	// Definindo textura da porta
	Textura txt_door (esq_inf, dir_inf, dir_sup, esq_sup, img_door);
	// Definindo textura do ceu
	Textura txt_sky (esq_inf, dir_inf, dir_sup, esq_sup, img_sky);

	// *************************************************************************
	// Fontes de luz ***********************************************************
	//
	// Informacoes da Fonte Pontual ********************************************
	Vetor intensePontual = { 1.0, 1.0, 1.0 }; // Intensidade da fonte pontual
	Ponto posicaoPontual = { 1000, 500, 1000 }; // Posicao da fonte pontual
	// Objeto da fonte de luz Pontual
	Pontual pontual (intensePontual, posicaoPontual);

	// Informacoes da Fonte Spot ***********************************************
	Vetor intenseSpot = { 0.5, 0.5, 0.5 }; 
	Ponto posicaoSpot = { 0, 0, 0 };
	Vetor direcaoSpot = { 0, -1, 0 };
	double anguloSpot = (3 * PI) / 8.0;
	// Objeto da fonte de luz Spot
	Spot spot (intenseSpot, posicaoSpot, direcaoSpot, anguloSpot);

	// Informacoes da Fonte Direcional *****************************************
	Vetor intenseDirecional = { 0.5, 0.5, 0.5 };
	Vetor direcaoDirecional = { -1, -1, -1 };
	// Objeto da fonte de luz Direcional
	Direcional direcional (intenseDirecional, direcaoDirecional);

    // Informacoes da luz Ambiente *********************************************
    Vetor luzAmbiente { 0.3, 0.3, 0.3 };

	// Abrindo arquivo para obter as informacoes
	Info info ("info.txt");

	// Informacoes da Janela ***************************************************
	double xminJanela = info.success ? info.xmin : -72; // Valor da parte de cima da janela em cm
	double xmaxJanela = info.success ? info.xmax : 72; // Valor da parte de baixo da janela em cm
	double yminJanela = info.success ? info.ymin : -40.5; // Valor do lado esquerdo da janela em cm
	double ymaxJanela = info.success ? info.ymax : 40.5; // Valor do lado direito da janela em cm
	double dJanela = info.success ? info.distance : 150; // Distancia da janela em cm
	// Objeto da Janela
	Janela janela (xminJanela, xmaxJanela, yminJanela, ymaxJanela, dJanela); 

	// Informacoes da Camera ***************************************************
	// Vista em Diagonal
	Ponto eye = info.success ? info.eye : Ponto{ 2600, 1000, 10000 };
	Ponto at = info.success ? info.at : Ponto{ 2600, 0, 0 };
	Ponto up = info.success ? info.up : Ponto{ 2600, 500, 0 }; 
	// Objeto da Camera 
	Camera camera (eye, at, up);

	// Lista de Objetos da Cena ************************************************
	Lista<Objeto> cenario;

    // Definindo a lista de objetos da cena
    void definirObjetos() 
    {
		// Realizando transformacoes 
		// Ajustando objeto da casa
		Transformacao::escala(&casa, { 500, 300, 400 });
		Transformacao::translacao(&casa, { 2600, 0, 2200 });
		// Ajustando objeto da arvore
		Transformacao::escala(&arvore1, { 2, 1, 1 });
		Transformacao::translacao(&arvore1, { 2400, 0, 2400 });
		Transformacao::translacao(&arvore2, { 2400, 0, 2400 });
		Transformacao::espelhoArbitrario(&arvore2,  { 2850, 0, 2200 }, { -1, 0, 0 });
		// Ajustando objeto do poste
		Transformacao::rotacaoY(&poste, PI / 4);
		Transformacao::translacao(&poste, { 2400, 0, 2800 });
		// Ajustando objeto da textura
		if (img_door) {
			Transformacao::escala(&txt_door, { 200, 222.5, 0 });
			Transformacao::translacao(&txt_door, { 2750, 0, 2601 });
			cenario.add(&txt_door);
		} else SDL_Log("Erro ao abrir imagem da porta da textura! SDL_Error: %s", SDL_GetError());

		if (img_sky) {
			Transformacao::escala(&txt_sky, { 14900, 7000, 0 });
			cenario.add(&txt_sky);
		} else SDL_Log("Erro ao abrir imagem do ceu da textura! SDL_Error: %s", SDL_GetError());

        // Adicionando os objetos na cena 
		cenario.add(&arvore1);
		cenario.add(&arvore2);
		cenario.add(&poste);
		cenario.add(&casa); 
        cenario.add(&chao);
        cenario.add(&fundoesq);
        // cenario.add(&fundodir);
    }

	// Lista de Fontes de luz **************************************************
	Lista<Fonte> fontes;

    // Definindo a lista de fontes da cena
    void definirFontes() 
    {
		// Realizando transformacoes 
		// Ajustando objeto da luz do poste
		// Posiciona a fonte spot na ponta do poste
		// Transformacao::translacao(&spot, { 2500, 500-13, 2800 });
		Transformacao::translacao(&spot, { 2500, 500-13, 2700 });

        // Adicionando as fontes na lista 
        fontes.add(&spot);
        fontes.add(&pontual);
        fontes.add(&direcional); 
    }
} // namespace Cena

#endif