#ifndef OBJETOS_HPP
#define OBJETOS_HPP

#include "util.hpp"
#include "matriz.hpp"

class Janela
{
private:
    double width, height, distance;
public:
    Janela(double w, double h, double d) : width{w}, height{h}, distance{d} {}

    double getWidth() { return width; }
    double getHeight() { return height; }
    double getDistance() { return distance; }
}; // fim class Janela

class Raio 
{
private:
	Ponto origem; // ponto de origem do raio
	Vetor direcao; // vetor unitario da direcao do raio 
public:
	Raio(Ponto i, Ponto f) : origem{i}, direcao{unitario(f - i)} {}
	Raio(Ponto i, Vetor d) : origem{i}, direcao{unitario(d)} {}

	Vetor getDirecao() { return direcao; }
	Ponto getOrigem() { return origem; }

	// Ponto de intersecao com parametro "t" 
	Ponto pontoIntersecao (double t) { return { origem + (t * direcao) }; }
}; // fim class Raio

// Hierarquia de classes: Objeto
//
class Objeto
{
public:
    Material material;
	virtual double intersecao (Raio) const = 0;
    virtual Vetor obterNormal (Ponto) const = 0;
};

class Esfera : public Objeto
{
private:
    Ponto centro;
    double raio;

public:
    Esfera(Ponto c, double r, Material m) : centro{c}, raio{r} { material = m; }

    double intersecao (Raio r) const override
    {
        Vetor w = r.getOrigem() - centro;

        double b = escalar( w, r.getDirecao() );
        double c = escalar( w, w ) - (raio * raio);

        double delta = (b * b) - c;

        if (delta > 0)
        {
            // c diz a distancia da origem do raio ao centro da esfera
            // se c > 0 entao a origem do raio esta fora da esfera
            if (c < 0) { return -b + sqrt(delta); }
            else       { return -b - sqrt(delta); }
        }
        return -1; // se delta <= 0
    }

    Vetor obterNormal (Ponto p) const override { return (p - centro) / raio; }
}; // fim class Esfera

class Plano : public Objeto
{
    Ponto ponto; // ponto pertencente ao plano
    Vetor normal; // vetor normal ao plano
public:
    Plano(Ponto p, Vetor n, Material m) : ponto{p}, normal{unitario(n)} 
    {
        material = m; 
    }

    double intersecao (Raio raio) const override
	{
		double denominador = escalar(normal, raio.getDirecao());

		if (denominador != 0)
		{
			Vetor w = raio.getOrigem() - ponto;
			double t_int = -1 * (escalar(normal, w) / denominador);
			return t_int;
		}
		else { return -1; }
	}

    Vetor obterNormal (Ponto p) const override { return normal; }
}; // fim class Plano

class Cilindro : public Objeto 
{
private:
    Ponto base; // centro da base do Cilindro
    Ponto topo; // centro do topo do Cilindro
    Vetor direcao; // direcao do eixo do Cilindro
    double raio; // raio do Cilindro
    double altura; // altura do Cilindro 

public:
    Cilindro (Ponto b, Ponto t, double r, Material m) : base{b}, topo{t}, raio{r}
    {
        direcao = (t - b);
        altura = norma(direcao);
        direcao = direcao / altura;
        material = m;
    }

    Cilindro (Ponto b, Vetor d, double r, double h, Material m) 
    : base{b}, direcao{unitario(d)}, raio{r}, altura{h}
    {
        topo = b + (h * direcao);
        material = m;
    } 

    double intersecao (Raio r) const override
    {
        // guarda o valor do menor t que intersecta cilindro
        double menor_t = -1;
        // guarda o valor do t para cada intersecao
        double t_int = -1;

        Vetor w = r.getDirecao() - (escalar(r.getDirecao(), direcao) * direcao);
        // coeficiente "a" da equacao de segundo grau
        double a = escalar(w, w);

        // CHECAR INTERSECAO COM CILINDRO
        // a == 0 significa que o raio é paralelo a superficie cilindrica
        if (a != 0)
        {
            Vetor u = (r.getOrigem() - base);
            Vetor v = u - (escalar(u, direcao) * direcao);

            // coeficiente "b" da equacao de segundo grau 
            double b = escalar(v, w);
            // coeficiente "c" da equacao de segundo grau 
            double c = escalar(v, v) - (raio * raio);
            
            // delta da equacao de segundo grau 
            double delta = (b * b) - (a * c);

            // delta >= 0, significa que tem intersecao
            if (delta >= 0) { 
                // duas raizes da equacao (podem ser a mesma raiz)
                double t1 = (-b - sqrt(delta)) / a;
                double t2 = (-b + sqrt(delta)) / a;

                // obtendo tamanho da projecao do ponto sobre eixo do cilindro
                double escalar1 = escalar(r.pontoIntersecao(t1) - base, direcao);
                double escalar2 = escalar(r.pontoIntersecao(t2) - base, direcao);

                // se ponto de t1 for valido, atualiza o valor de t pois é a menor raiz
                if (escalar1 >= 0 && escalar1 <= altura) { t_int = t1; } 
                // se ponto de t2 for valido, atualiza o valor de t pois é a segunda raiz
                else if (escalar2 >= 0 && escalar2 <= altura) { t_int = t2; }
                // se nao houver ponto valido, deixa t com valor invalido
                else { t_int = -1; }
            }
            else { t_int = -1; } // se delta negativo, deixa t com valor invalido
        } else { t_int = -1; } // como a == 0, deixa t com valor invalido

        // atualiza menor_t com o valor de t encontrado
        if(t_int > 0) menor_t = t_int;

        // denominador no calculo da intersecao com planos
        double denominador;

        // CHECAR INTERSECAO COM BASE
        // escalar entre a normal da base e a direcao do raio
		denominador = escalar((-1 * direcao), r.getDirecao());

        // se denominador != 0 tem intersecao com a base
		if (denominador != 0) {
            // verificando intersecao com base do cilindro
			double t_base = -1 * (escalar((-1 * direcao), (r.getOrigem() - base)) / denominador);

            // se valor de t_base for valido
            if (t_base > 0) {
                // cria vetor do ponto intersectado ate o centro da base
                Vetor v = r.pontoIntersecao(t_base) - base;
                // verificando se o ponto pertence ao circulo da base
                if (escalar(v, v) <= (raio * raio)) 
                    t_int = t_base;
                else // se nao pertencer, deixa t_int com valor invalido
                    t_int = -1;
            // se o valor de t_base nao for valido, deixa t_int com valor invalido
            } else { t_int = -1; } 
        // se denominador == 0 nao tem intersecao com a base
		} else { t_int = -1; }

        // ATUALIZAR O VALOR DE menor_t
        // apenas interessa se o t_int for valido
        if (t_int > 0) {
            // se menor_t for valido, verifica qual o menor valor
            if (menor_t > 0) 
                // menor_t recebe t_int se for menor que o valor atual
                menor_t = (t_int < menor_t ? t_int : menor_t);
            // se for invalido, apenas atualiza o valor
            else menor_t = t_int;
        }

        // CHECAR INTERSECAO COM TOPO
        // escalar entre a normal do topo e a direcao do raio
		denominador = escalar(direcao, r.getDirecao());

        // se denominador != 0 tem intersecao com o topo
		if (denominador != 0) {
            // verificando intersecao com topo do cilindro
            double t_topo = -1 * (escalar(direcao, (r.getOrigem() - topo)) / denominador);

            // se valor de t_topo for valido
            if (t_topo > 0) {
                // cria vetor do ponto intersectado ate o centro do topo
                Vetor v = r.pontoIntersecao(t_topo) - topo;
                // verificando se o ponto pertence ao circulo do topo
                if (escalar(v, v) <= (raio * raio)) 
                    t_int = t_topo;
                else // se nao pertencer, deixa t_int com valor invalido
                    t_int = -1;
            // se o valor de t_topo for invalido, deixa t_int com valor invalido
            } else { t_int = -1; } 
        // se denominador == 0 nao tem intersecao com o topo
		} else { t_int = -1; }

        // ATUALIZAR O VALOR DE menor_t
        // apenas interessa se o t_int for valido
        if (t_int > 0) {
            // se menor_t for valido, verifica qual o menor valor
            if (menor_t > 0) 
                // menor_t recebe t_int se for menor que o valor atual
                menor_t = (t_int < menor_t ? t_int : menor_t);
            // se for invalido, apenas atualiza o valor
            else menor_t = t_int;
        }
        // retorna o menor valor de t encontrado
        return menor_t;
    }

    Vetor obterNormal (Ponto p) const override
    {
        Vetor vetorPonto = (p - base);
        double escalarPonto = escalar(vetorPonto, direcao);

        if (escalarPonto - 0.1 <= 0) return (-1) * direcao;
        else if (escalarPonto + 0.1 >= altura) return direcao;
        else
        {
            Vetor N = vetorPonto - (escalarPonto * direcao);
            return (N / raio);
        }
    }
}; // fim class Cilindro

class Cone : public Objeto 
{
    Ponto base; // centro da base do cone
    Ponto vertice; // vertice do cone 
    Vetor direcao; // direcao do eixo do cone
    double raio; // raio da base do cone
    double altura; // altura do cone 

public:
    Cone (Ponto b, Ponto v, double r, Material m) : base{b}, vertice{v}, raio{r}
    {
        direcao = (v - b); 
        altura = norma(direcao);
        direcao = direcao / altura; 
        material = m;
    }

    Cone (Ponto b, Vetor d, double r, double h, Material m) 
    : base{b}, direcao{unitario(d)}, raio{r}, altura{h} 
    {
        vertice = b + (h * direcao); 
        material = m;
    }

    double intersecao (Raio r) const override 
    {
        // guarda o valor do menor t que intersecta cone
        double menor_t = -1;
        // guarda o valor do t para cada intersecao
        double t_int = -1;

        // variavel B das formulas 
        double beta = (altura*altura) / (raio*raio);
        // variavel w das formulas
        Vetor v = vertice - r.getOrigem();
        // Produto escalar entre direcao do raio e direcao do cone
        double escalarRC = escalar(r.getDirecao(), direcao);
        // Produto escalar entre vetor v e direcao do cone
        double escalarVC = escalar(v, direcao);
        // Produto escalar entre vetor v e direcao do raio
        double escalarVR = escalar(v, r.getDirecao());
        
        // coeficiente "a" da equacao de segundo grau 
        double a = beta - (1 + beta) * (escalarRC * escalarRC);
        // coeficiente "b" da equacao de segundo grau 
        double b = (-beta) * escalarVR + (1 + beta) * escalarVC * escalarRC;
        // coeficiente "c" da equacao de segundo grau 
        double c = beta * escalar(v, v) - (1 + beta) * (escalarVC * escalarVC);

        // CHECAR INTERSECAO COM SUPERFICIE DO CONE
        // delta da equacao de segundo grau 
        double delta = (b * b) - (a * c);

        // delta >= 0, significa que tem intersecao
        if (delta >= 0) 
        {
            // duas raizes da equacao (podem ser a mesma raiz)
            double t1 = (-b - sqrt(delta)) / a;
            double t2 = (-b + sqrt(delta)) / a;

            // obtendo tamanho da projecao do ponto sobre eixo do cilindro
            double escalar1 = escalar(vertice - r.pontoIntersecao(t1), direcao);
            double escalar2 = escalar(vertice - r.pontoIntersecao(t2), direcao);

            // se ponto de t1 for valido, atualiza o valor de t pois é a menor raiz
            if (escalar1 >= 0 && escalar1 <= altura) { t_int = t1; } 
            // se ponto de t2 for valido, atualiza o valor de t pois é a segunda raiz
            else if (escalar2 >= 0 && escalar2 <= altura) { t_int = t2; }
            // se nao houver ponto valido, deixa t com valor invalido
            else { t_int = -1; }            
        } else { t_int = -1; }

        // atualiza menor_t com o valor de t encontrado
        if(t_int > 0) menor_t = t_int;

        // CHECAR INTERSECAO COM BASE
        // escalar entre a normal da base e a direcao do raio
		double denominador = escalar((-1 * direcao), r.getDirecao());

        // se denominador != 0 tem intersecao com a base
		if (denominador != 0) {
            // verificando intersecao com base do cilindro
			double t_base = -1 * (escalar((-1 * direcao), (r.getOrigem() - base)) / denominador);

            // se valor de t_base for valido
            if (t_base > 0) {
                // cria vetor do ponto intersectado ate o centro da base
                Vetor v = r.pontoIntersecao(t_base) - base;
                // verificando se o ponto pertence ao circulo da base
                if (escalar(v, v) <= (raio * raio)) 
                    t_int = t_base;
                else // se nao pertencer, deixa t_int com valor invalido
                    t_int = -1;
            // se o valor de t_base nao for valido, deixa t_int com valor invalido
            } else { t_int = -1; } 
        // se denominador == 0 nao tem intersecao com a base
		} else { t_int = -1; }

        // ATUALIZAR O VALOR DE menor_t
        // apenas interessa se o t_int for positivo
        if (t_int > 0) {
            // se menor_t for valido, verifica qual o menor valor
            if (menor_t > 0) 
                // menor_t recebe t_int se for menor que o valor atual
                menor_t = (t_int < menor_t ? t_int : menor_t);
            // se for invalido, apenas atualiza o valor
            else menor_t = t_int;
        }
        // retorna o menor valor de t encontrado
        return menor_t;
    }

    Vetor obterNormal (Ponto p) const override 
    {
        if (escalar(direcao, (p - base)) - 0.1 <= 0)
        {
            return (-1) * direcao;
        } else
        {
            Vetor w = vetorial(direcao, (p - base));
            Vetor normal = vetorial(w, (vertice - p));
            
            return unitario(normal);
        }
    }
}; // fim class Cone

class Malha : public Objeto
{
private:
    struct Aresta { int v1, v2; };
    struct Face { int a1, a2, a3; };
    int numvertices, numarestas, numfaces;
    Ponto *vertices;
    Aresta *arestas;
    Face *faces;
public:
    Malha(int v, int a, int f) : numvertices{v}, numarestas{a}, numfaces{f}
    {
        vertices = new Ponto[v];
        arestas = new Aresta[a];
        faces = new Face[f];
    }
    ~Malha() 
    {
        delete[] vertices;
        delete[] arestas;
        delete[] faces;
    }

    // recebe id do vertice e posicao do vertice
    void setVertice (int i, Ponto p) { vertices[i] = p; }
    // recebe id da aresta e ids dos vertices
    void setAresta (int i, int v1, int v2) { arestas[i] = { v1, v2 }; }
    // recebe id da face e ids das arestas
    void setFace (int i, int a1, int a2, int a3) { faces[i] = { a1, a2, a3 }; }

    double intersecao (Raio raio) const override {}
    Vetor obterNormal (Ponto ponto) const override {}
}; // fim class Malha
//
// Fim da hierarquia de classes Objeto 

// Gerando um tipo chamado RC que correponde ao tipo da funcao raycast
// Esse tipo sera usado para um ponteiro da função raycast
using RC = double(Lista<Objeto>&, Raio, Objeto*&);

// Hierarquia de classes: Fonte
//
class Fonte 
{
public:
	Vetor intensidade; // intensidade da luz

    virtual Vetor iluminacao(Vetor, Ponto, Vetor, Material) const = 0;
    virtual bool sombra(Ponto, Lista<Objeto>&, RC*) const = 0;
};

class Pontual : public Fonte
{
    Ponto posicao; // local da fonte de luz
public:
    Pontual (Vetor i, Ponto p) : posicao{p} { intensidade = i; }

    bool sombra (Ponto p_int, Lista<Objeto> &cena, RC* raycast) const override 
    {
        // Raio que parte da posicao da fonte ao ponto de intersecao
        Raio raioSombra (posicao, p_int);

        // Ponteiro temporario que vai guardar o objeto atingido
        Objeto* temp; // Necessario na funcao mas nao sera usado 

        // Lancando raio na cena para ver se tem sombra
        double t_sombra = raycast(cena, raioSombra, temp);

        // Obtendo distancia da fonte ao ponto de intersecao
        double distanciaFonte = norma(p_int - posicao);

        // Retorna TRUE se distancia da intersecao for igual ou maior
        return (t_sombra + 0.01 >= distanciaFonte);
    }

    Vetor iluminacao (Vetor normal, Ponto p_int, Vetor dirRaio, Material material) 
    const override
	{
        // Vetor em direcao a fonte de luz  (unitario)
        Vetor luz = unitario(posicao - p_int);
        // Vetor em direcao a origem do raio (unitario)
        Vetor visao = (-1) * dirRaio;
        // Vetor em direcao ao raio refletido (unitario)
        Vetor reflexo = (2 * escalar(luz, normal) * normal) - luz;

        // Fator de Difusao
        double fatorDif = maior(0.0, escalar(luz, normal));
        // Calculo da intensidade da Luz Difusa
        Vetor Id = (material.kd * intensidade) * fatorDif; 

        // Fator Especular
        double fatorEsp = maior(0.0, escalar(reflexo, visao));
        fatorEsp = pow(fatorEsp, material.brilho);
        // Calculo da intensidade da Luz Especular 
        Vetor Ie = (material.ke * intensidade) * fatorEsp; 

        // Retorna a soma das duas intensidades
        return (Id + Ie);
	}
}; // fim class Pontual

class Spot : public Fonte
{
private:
    Ponto posicao; // local da fonte de luz
    Vetor direcao; // direcao da fonte de luz
    double angulo; // angulo de abertura da fonte de luz
public:
    Spot(Vetor i, Ponto p, Vetor d, double a) : 
        posicao{p}, 
        direcao{unitario(d)}, 
        angulo{a} 
        { intensidade = i; }

    bool sombra (Ponto p_int, Lista<Objeto> &cena, RC* raycast) const override
    {
        // Vetor em direcao a fonte de luz  (unitario)
        Vetor luz = unitario(posicao - p_int);
        // Cosseno entre o vetor luz e a direcao da luz spot
        double cosLuzDirecao = escalar((-1 * luz), direcao);

        // Verificando se o ponto intersectado nao esta no foco da luz 
        if (cos(angulo) > cosLuzDirecao) return false;

        // Raio que parte da posicao da fonte ao ponto de intersecao
        Raio raioSombra (posicao, p_int);

        // Ponteiro temporario que vai guardar o objeto atingido
        Objeto* temp; // Necessario na funcao mas nao sera usado 

        // Lancando raio na cena para ver se tem sombra
        double t_sombra = raycast(cena, raioSombra, temp);

        // Obtendo distancia da fonte ao ponto de intersecao
        double distanciaFonte = norma(p_int - posicao);

        // Retorna TRUE se distancia da intersecao for igual ou maior
        return (t_sombra + 0.01 >= distanciaFonte);
    }

    Vetor iluminacao (Vetor normal, Ponto p_int, Vetor dirRaio, Material material)
    const override
    {
        // Vetor em direcao a fonte de luz  (unitario)
        Vetor luz = unitario(posicao - p_int);
        // Vetor em direcao a origem do raio (unitario)
        Vetor visao = (-1) * dirRaio;
        // Vetor em direcao ao raio refletido (unitario)
        Vetor reflexo = (2 * escalar(luz, normal) * normal) - luz;

        // Fator de Difusao e Especular
        double fatorDif, fatorEsp;

        // Cosseno entre o vetor luz e a direcao da luz spot
        double cosLuzDirecao = escalar((-1 * luz), direcao);

        if (cos(angulo) <= cosLuzDirecao)
        {
            // Fator de Difusao
            fatorDif = maior(0.0, escalar(luz, normal));
            // Fator Especular
            fatorEsp = maior(0.0, escalar(reflexo, visao));
            fatorEsp = pow(fatorEsp, material.brilho);
        } else
        {
            fatorDif = 0; // Fator de Difusao
            fatorEsp = 0; // Fator Especular
        }
        
        // Calculo da intensidade da Luz Difusa
        Vetor Id = (material.kd * intensidade) * fatorDif; 
        // Calculo da intensidade da Luz Especular 
        Vetor Ie = (material.ke * intensidade) * fatorEsp; 
        
        return (Id + Ie);// * cosLuzDirecao;
    }
}; // fim class Spot

class Direcional : public Fonte
{
private:
    Vetor direcao;
public:
    Direcional(Vetor i, Vetor d) : direcao{unitario(d)} { intensidade = i; }

    bool sombra (Ponto p_int, Lista<Objeto> &cena, RC* raycast) const override
    {
        // Raio que parte do ponto de intersecao em direcao a fonte
        // Como nao tem posicao da fonte entao gera-se um ponto qualquer 
        // na direcao contraria da direcao da fonte de luz direcional
        Ponto inicio = p_int + (-0.1 * direcao);
        Raio raioSombra (inicio, (-1) * direcao);

        // Ponteiro temporario que vai guardar o objeto atingido
        Objeto* temp; // Necessario na funcao mas nao sera usado 

        // Lancando raio na cena para ver se tem sombra
        double t_sombra = raycast(cena, raioSombra, temp);

        // Retorna TRUE se houve qualquer intersecao no caminho
        // Como nao tem posicao, qualquer intersecao torna-se valida
        return (t_sombra <= 0);
    }

    Vetor iluminacao (Vetor normal, Ponto p_int, Vetor dirRaio, Material material)
    const override
    {
        // Vetor em direcao a fonte de luz  (unitario)
        Vetor luz = (-1) * direcao;
        // Vetor em direcao a origem do raio (unitario)
        Vetor visao = (-1) * dirRaio;
        // Vetor em direcao ao raio refletido (unitario)
        Vetor reflexo = (2 * escalar(luz, normal) * normal) - luz;

        // Fator de Difusao
        double fatorDif = maior(0.0, escalar(luz, normal));
        // Calculo da intensidade da Luz Difusa
        Vetor Id = (material.kd * intensidade) * fatorDif; 

        // Fator Especular
        double fatorEsp = maior(0.0, escalar(reflexo, visao));
        fatorEsp = pow(fatorEsp, material.brilho);
        // Calculo da intensidade da Luz Especular 
        Vetor Ie = (material.ke * intensidade) * fatorEsp; 

        // Retorna a soma das duas intensidades
        return (Id + Ie);
    }
}; // fim class Direcional
//
// Fim da hierarquia de classes Fonte
#endif