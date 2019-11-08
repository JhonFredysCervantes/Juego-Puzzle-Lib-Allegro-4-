#include<iostream>
#include<allegro.h>
#include<time.h>
#include<stdlib.h>
#define tam 3
#define ancho 800
#define alto 600

using namespace std;

int Mat[tam][tam];
int final[tam][tam]={{0,1,2},
					 {3,4,5},
					 {6,7,8}};
int copia[tam][tam];
int cerox,ceroy,movUsuario,movMaquina,tope;

struct nodo{
	int estado[tam][tam];
	int pos[2];
	struct nodo *sig;
	struct nodo *padre;
}*recorrido,*lista,*p,*q,*r,*s,*z, *t, *actual,*cerrado,*Encontrado;

struct img{
	BITMAP *imagen;
	int dato;
}cuadro[tam][tam];

BITMAP *Buffer;
BITMAP *Fondo, *tablero,*miniatura;
BITMAP *m1, *m2, *m3,*cursor, *cargando,*acerca;
BITMAP *ganarImg,*ganarS,*ganarM, *perderImg, *perderS, *perderM;
FONT *snap;

void init();
void definit();

void Desorden(int Matriz[][tam]);
void imprimir(int m[][tam]);
void crearNodo(nodo *&padre);
void copiarVec(int copiar[][tam],int copiando[][tam]);
void cargarMaterial();
void Pintar();
void pintarTabla();
void movimientos();
void derecha();
void izquierda();
void arriba();
void abajo();
void al_vect();
void pintarSolucion();
void limpiarListas(nodo *&lis);
void insertaRecorrido(int v[][tam]);
void asignarImg();
nodo *buscaMenorCoste(nodo *&me);
nodo *evaluar(nodo *&ini);
nodo *moverAcerrada(nodo *&buscado);
nodo *insertalis(nodo *&nod);
nodo *asignarNodo(int m[][tam]);
int reversa(nodo *&nod);
int valornodo(int m[][tam]);
int heuristica(int m[][tam]);
int menu();
int ganarFuncion();
int perderFuncion();
int pausa();
bool encontrado(int vect[][tam]);
bool buscarList(nodo *&dir, nodo *&nod);

int main(){
    int opcion=1,ganar;
	//-----------------------------
	//Desorden(Mat);
	//----Inicio parte Grafica-----
	init();
	set_window_title("PUZZLE 8 ");
	cargarMaterial();
	text_mode(-1);
	//-----------------------------
	while(opcion!=0){
        opcion=menu();
        if(opcion==2){
            movUsuario=0;
            //------ Cargando Resultado ---
            draw_sprite(screen,cargando,0,0);
            limpiarListas(lista);
            limpiarListas(cerrado);
            limpiarListas(recorrido);
            //---asignacion de variables---
            recorrido=NULL;
            lista=NULL;
            cerrado=NULL;
            al_vect();
            copiarVec(copia,Mat);
            q=asignarNodo(copia);
            Encontrado=evaluar(q);
            tope=reversa(Encontrado);
            asignarImg();
            ganar=0;
            //------------------------
            while(!key[KEY_ESC]&&(ganar==0)&&(movUsuario<=(2*tope))){

                Pintar();
                movimientos();
                if(encontrado(copia)){
                    ganar = 1;
                }

            }
            pintarSolucion();
            if(ganar==0&&(movUsuario<=(2*tope))){
                opcion=0;
                ganar = pausa();
            }else{
                ganar = pausa();
                if(ganar==1){
                    opcion=ganarFuncion();
                }else if(ganar==0){
                    opcion=perderFuncion();
                }
            }
        }else{
            draw_sprite(Buffer,acerca,0,0);
            textprintf(Buffer,font,20,580,makecol(255,10,10),"Presione ESC para Salir");
            while(!key[KEY_ESC]){
                draw_sprite(screen,Buffer,0,0);
            }
        }
	}

	definit();

	return 0;
}
END_OF_MAIN();

int pausa(){
    while(!key[KEY_ESC]){
        textprintf(Buffer,font,150,560,makecol(255,10,10),"Presione ESC para Continuar");
        textprintf(Buffer,font,630,195,makecol(240,0,0)," Movimientos Mq: %i",movMaquina);
        draw_sprite(screen,Buffer,0,0);
    }
    if(movUsuario>movMaquina){
        return 0;
    }else{
        return 1;
    }
}

int ganarFuncion(){
    int opc=1;
    while(opc){
        if(mouse_x> 330 && mouse_x< 450 && mouse_y >444 && mouse_y < 485){
            draw_sprite(Buffer,ganarM,0,0);
            if(mouse_b & 1){
                opc=1;
                return opc;
            }
        }else if(mouse_x> 300 && mouse_x< 480 && mouse_y >500 && mouse_y < 540){
            draw_sprite(Buffer,ganarS,0,0);
            if(mouse_b & 1){
                opc=0;
                return opc;
            }
        }else{
            draw_sprite(Buffer,ganarImg,0,0);
        }

        masked_blit(cursor,Buffer,0,0,mouse_x,mouse_y,13,22);
        draw_sprite(screen,Buffer,0,0);
    }
}

int perderFuncion(){
    int opc=1;
    while(opc){
        if(mouse_x> 72 && mouse_x< 215 && mouse_y >325 && mouse_y < 370){
            draw_sprite(Buffer,perderM,0,0);
            if(mouse_b & 1){
                opc=1;
                return opc;
            }
        }else if(mouse_x> 65 && mouse_x< 220 && mouse_y >395 && mouse_y < 450){
            draw_sprite(Buffer,perderS,0,0);
            if(mouse_b & 1){
                opc=0;
                return opc;
            }
        }else{
            draw_sprite(Buffer,perderImg,0,0);
        }

        masked_blit(cursor,Buffer,0,0,mouse_x,mouse_y,13,22);
        draw_sprite(screen,Buffer,0,0);
    }
}

void copiarVec(int copiar[][tam],int copiando[][tam]){
    for(int i=0;i<tam;i++){
        for(int j=0;j<tam;j++){
            copiar[i][j]=copiando[i][j];
            if(copiando[i][j]==0){
                cerox=i;
                ceroy=j;
            }
        }
    }
}

void pintarSolucion(){
    movMaquina=0;
    s=recorrido;
    while(s!=NULL){
        copiarVec(copia,s->estado);
        Pintar();
        rest(80);
        s=s->sig;
        movMaquina++;
    }
}

void al_vect(){
    int randon;
    srand(time(NULL));
    randon = 1+rand()%8;

    if(randon==1){
        int mtr[tam][tam] = { {7,2,0}, {5,3,6}, {1,8,4} };
        for(int i=0;i<tam;i++){
            for(int j=0;j<tam;j++){
                Mat[i][j]=mtr[i][j];
            }
        }
        return ;
    }

    if(randon==2){
        int mtr[tam][tam] = { {3,1,5}, {7,0,8}, {2,4,6} };
        for(int i=0;i<tam;i++){
            for(int j=0;j<tam;j++){
                Mat[i][j]=mtr[i][j];
            }
        }
        return ;
    }

    if(randon==3){
        int mtr[tam][tam] = { {1,7,5}, {3,4,0}, {2,6,8} };
        for(int i=0;i<tam;i++){
            for(int j=0;j<tam;j++){
                Mat[i][j]=mtr[i][j];
            }
        }
        return ;
    }

    if(randon==4){
        int mtr[tam][tam] = { {0,1,6}, {2,7,3}, {5,4,8} };
        for(int i=0;i<tam;i++){
            for(int j=0;j<tam;j++){
                Mat[i][j]=mtr[i][j];
            }
        }
        return ;
    }

    if(randon==5){
        int mtr[tam][tam] = { {2,3,7}, {1,8,4}, {6,0,5} };
        for(int i=0;i<tam;i++){
            for(int j=0;j<tam;j++){
                Mat[i][j]=mtr[i][j];
            }
        }
        return ;
    }
    if(randon==6){
        int mtr[tam][tam] = { {2,4,5}, {0,7,8}, {3,1,6} };
        for(int i=0;i<tam;i++){
            for(int j=0;j<tam;j++){
                Mat[i][j]=mtr[i][j];
            }
        }
        return ;
    }
    if(randon==7){
        int mtr[tam][tam] = { {2,1,8}, {3,4,7}, {0,5,6} };
        for(int i=0;i<tam;i++){
            for(int j=0;j<tam;j++){
                Mat[i][j]=mtr[i][j];
            }
        }
        return ;
    }
    if(randon==8){
        int mtr[tam][tam] = { {6,0,8}, {4,5,3}, {1,7,2} };
        for(int i=0;i<tam;i++){
            for(int j=0;j<tam;j++){
                Mat[i][j]=mtr[i][j];
            }
        }
    }
}

int menu(){
    int opc=1;
    while(opc){
        if(mouse_x> 340 && mouse_x< 488 && mouse_y >315 && mouse_y < 365){
            draw_sprite(Buffer,m2,0,0);
            if(mouse_b & 1){
                opc=2;
                return opc;
            }
        }else if(mouse_x> 300 && mouse_x< 545 && mouse_y >395 && mouse_y < 435){
            draw_sprite(Buffer,m3,0,0);
            if(mouse_b & 1){
                opc=3;
                return opc;
            }
        }else{
            draw_sprite(Buffer,m1,0,0);
        }

        masked_blit(cursor,Buffer,0,0,mouse_x,mouse_y,13,22);
        draw_sprite(screen,Buffer,0,0);
    }
}

void movimientos(){
    derecha();
    izquierda();
    arriba();
    abajo();
}

void derecha(){
    //valido recoger una sola entrada con readkey
    if((readkey()>>8)==KEY_RIGHT){
        if(ceroy!=0){
            copia[cerox][ceroy]=copia[cerox][ceroy-1];
            copia[cerox][ceroy-1]=0;
            ceroy=ceroy-1;
            movUsuario++;
        }
    }
}

void izquierda(){
    if(key[KEY_LEFT]){
        if(ceroy!=2){
            copia[cerox][ceroy]=copia[cerox][ceroy+1];
            copia[cerox][ceroy+1]=0;
            ceroy=ceroy+1;
            movUsuario++;
        }
    }
}

void arriba(){
    if(key[KEY_UP]){
        if(cerox!=2){
            copia[cerox][ceroy]=copia[cerox+1][ceroy];
            copia[cerox+1][ceroy]=0;
            cerox=cerox+1;
            movUsuario++;
        }
    }
}

void abajo(){
    if(key[KEY_DOWN]){
        if(cerox!=0){
            copia[cerox][ceroy]=copia[cerox-1][ceroy];
            copia[cerox-1][ceroy]=0;
            cerox=cerox-1;
            movUsuario++;
        }
    }
}

void Pintar(){
    clear_to_color(Buffer,makecol(255,255,255));
    draw_sprite(Buffer,Fondo,0,0);
    pintarTabla();
    draw_sprite(Buffer,miniatura,650,400);
    textprintf(Buffer,snap,170,15,makecol(31,250,232)," PUZZLE 8 !Diviertete! ");
    textprintf(Buffer,font,10,580,makecol(240,10,22)," ESC para Salir");
    textprintf(Buffer,font,630,165,makecol(240,0,0)," Maximo: %i",2*tope);
    textprintf(Buffer,font,630,180,makecol(240,0,0)," Movimientos Us: %i",movUsuario);
    draw_sprite(screen,Buffer,0,0);

}

void pintarTabla(){
    int x=110,y=50;
    for(int i=0;i<tam;i++){
        for(int j=0;j<tam;j++){

            for(int nn=0;nn<tam;nn++){
                for(int nnn=0;nnn<tam;nnn++){
                    if(copia[i][j]==cuadro[nn][nnn].dato){
                        draw_sprite(Buffer,cuadro[nn][nnn].imagen,x,y);
                        break;
                    }
                }
            }
            x=x+167;
        }
        x=110;
        y=y+167;
    }
}

void cargarMaterial(){
    Buffer = create_bitmap(ancho,alto);
    Fondo = load_bitmap("fondo.bmp",NULL);
    m1 = load_bitmap("menu.bmp",NULL);
    m2 = load_bitmap("jugar.bmp",NULL);
    m3 = load_bitmap("acerca.bmp",NULL);
    cursor = load_bitmap("cursor.bmp",NULL);
    cargando = load_bitmap("cargando.bmp",NULL);
    ganarImg = load_bitmap("ganar.bmp", NULL);
    ganarM = load_bitmap("ganar-m.bmp", NULL);
    ganarS = load_bitmap("ganar-s.bmp", NULL);
    perderImg = load_bitmap("perder.bmp", NULL);
    perderM = load_bitmap("perder-m.bmp", NULL);
    perderS = load_bitmap("perder-s.bmp", NULL);
    acerca = load_bitmap("acercaIntro.bmp",NULL);
    snap = load_font("snap.pcx",NULL,NULL);
}

void asignarImg(){
    int a, x=0,y=0,cont=0;
    srand(time(NULL));
    a=1+rand()%5;
    if(a==1){
        tablero = load_bitmap("sistema.bmp",NULL);
        miniatura = load_bitmap("sistema-m.bmp",NULL);
    }else if( a==2){
        tablero = load_bitmap("agujero.bmp",NULL);
        miniatura = load_bitmap("agujero-m.bmp",NULL);
    }else if( a==3){
        tablero = load_bitmap("planeta.bmp",NULL);
        miniatura = load_bitmap("planeta-m.bmp",NULL);
    }else if( a==4){
        tablero = load_bitmap("nasa.bmp",NULL);
        miniatura = load_bitmap("nasa-m.bmp",NULL);
    }else{
        tablero = load_bitmap("gato.bmp",NULL);
        miniatura = load_bitmap("gato-m.bmp",NULL);
    }
    for(int i=0;i<tam;i++){
        for(int j=0;j<tam;j++){
            cuadro[i][j].imagen = create_sub_bitmap(tablero,x,y,166,166);
            cuadro[i][j].dato=cont;
            x=x+166;
            cont++;
        }
        x=0;
        y=y+166;
    }
    clear_to_color(cuadro[0][0].imagen,makecol(255,255,255));
}

nodo *buscaMenorCoste(nodo *&me){
	int menor,menor2;
	s=me;
	menor=valornodo(s->estado);
	t=s;
	s=s->sig;
	while(s!=NULL){
		menor2=valornodo(s->estado);
		if(menor>menor2){
			t=s;
		}
		s=s->sig;
	}
	return t;
}

nodo *evaluar(nodo *&ini){
	actual = ini;
	actual->padre=NULL;
	lista=actual;
	while(!encontrado(actual->estado)&&lista!=NULL){
		crearNodo(actual);
		cerrado=moverAcerrada(actual);
		actual=buscaMenorCoste(lista);
	}
	return actual;
}

int reversa(nodo *&nod){
    int a=0;
	q=nod;
	while(q!=NULL){
        insertaRecorrido(q->estado);
        q=q->padre;
        a++;
	}
	return a;
}

void insertaRecorrido(int v[][tam]){
    nodo *r = new nodo;
    copiarVec(r->estado,v);
    if(recorrido==NULL){
        recorrido=r;
        r->sig=NULL;
    }else{
        r->sig=recorrido;
        recorrido=r;
    }
}

void limpiarListas(nodo *&lis){
    if(lis!=NULL){
        z=lis;
        s=z->sig;
        while(s!=NULL){
            delete z;
            z=s;
            s=z->sig;
        }
    }
}

bool buscarList(nodo *&dir,nodo *&nod){
	bool std=true;
	z=dir;
	if(z==NULL){
		return false;
	}
	while(z!=NULL){
		for(int i=0;i<tam;i++){
			for(int j=0;j<tam;j++){
				if(nod->estado[i][j]!=z->estado[i][j]){
					std=false;
				}
			}
		}

		if(!std){
			if(z->sig!=NULL){
				std=true;
			}
		}else{
			return std;
		}

		z=z->sig;

	}
	return std;
}

nodo *moverAcerrada(nodo *&buscado){
	int sw=0;
	s=lista;
	if(s==buscado){
		if(lista->sig==NULL){
			lista=NULL;
		}else{
			lista=lista->sig;
		}
		if(cerrado==NULL){
			cerrado=s;
			s->sig=NULL;
		}else{
			s->sig=cerrado;
			cerrado=s;
		}
	}else{
		t=lista->sig;
		while((t!=NULL)&&(s!=NULL)&&(!sw)){
			if(t==buscado){
				s->sig=t->sig;
				if(cerrado==NULL){
					cerrado=t;
					t->sig=NULL;
				}else{
					t->sig=cerrado;
					cerrado=t;
				}
				sw=1;
			}
			t=t->sig;
			s=s->sig;
		}
	}
	return cerrado;
}

nodo *insertalis(nodo *&nod){
	if(lista==NULL){
		lista=nod;
		nod->sig=NULL;
	}else{
		nod->sig=lista;
		lista=nod;
	}
	return lista;
}

void crearNodo(nodo *&padre){

	if(padre->pos[0]!=0){
	 	p=NULL;
		nodo *p = new nodo;
		for(int i=0;i<tam;i++){
			for(int j=0;j<tam;j++){
				p->estado[i][j]=padre->estado[i][j];
			}
		}
		p->estado[padre->pos[0]][padre->pos[1]]=padre->estado[padre->pos[0]-1][padre->pos[1]];
		p->estado[padre->pos[0]-1][padre->pos[1]]=0;
		p->pos[0]=padre->pos[0]-1;
		p->pos[1]=padre->pos[1];
		p->padre=padre;


		if((!buscarList(*&lista,*&p))&&(!buscarList(*&cerrado,*&p))){
			lista=insertalis(p);
		}
	}
	if(padre->pos[0]!=2){
		p=NULL;
		nodo *p = new nodo;
		for(int i=0;i<tam;i++){
			for(int j=0;j<tam;j++){
				p->estado[i][j]=padre->estado[i][j];
			}
		}
		p->estado[padre->pos[0]][padre->pos[1]]=padre->estado[padre->pos[0]+1][padre->pos[1]];
		p->estado[padre->pos[0]+1][padre->pos[1]]=0;
		p->pos[0]=padre->pos[0]+1;
		p->pos[1]=padre->pos[1];
		p->padre=padre;


		if((!buscarList(*&lista,*&p))&&(!buscarList(*&cerrado,*&p))){
			lista=insertalis(p);
		}
	}
	if(padre->pos[1]!=0){
		p=NULL;
		nodo *p = new nodo;
		for(int i=0;i<tam;i++){
			for(int j=0;j<tam;j++){
				p->estado[i][j]=padre->estado[i][j];
			}
		}
		p->estado[padre->pos[0]][padre->pos[1]]=padre->estado[padre->pos[0]][padre->pos[1]-1];
		p->estado[padre->pos[0]][padre->pos[1]-1]=0;
		p->pos[0]=padre->pos[0];
		p->pos[1]=padre->pos[1]-1;
		p->padre=padre;


		if((!buscarList(*&lista,*&p))&&(!buscarList(*&cerrado,*&p))){
			lista=insertalis(p);
		}
	}
	if(padre->pos[1]!=2){
		p=NULL;
		nodo *p = new nodo;
		for(int i=0;i<tam;i++){
			for(int j=0;j<tam;j++){
				p->estado[i][j]=padre->estado[i][j];
			}
		}
		p->estado[padre->pos[0]][padre->pos[1]]=padre->estado[padre->pos[0]][padre->pos[1]+1];
		p->estado[padre->pos[0]][padre->pos[1]+1]=0;
		p->pos[0]=padre->pos[0];
		p->pos[1]=padre->pos[1]+1;
		p->padre=padre;


		if((!buscarList(*&lista,*&p))&&(!buscarList(*&cerrado,*&p))){
			lista=insertalis(p);
		}
	}
}

int valornodo(int m[][tam]){
	int a=0;
	for(int i=0;i<tam;i++){
		for(int j=0;j<tam;j++){
			if(m[i][j]!=final[i][j]){
				a++;
			}
		}
	}
	a = a + heuristica(m);
	return a;
}

int heuristica(int m[][tam]){
	int acum=0,rax=0;
	for(int i=0;i<tam;i++){
		for(int j=0;j<tam;j++){

			for(int g=0;g<tam;g++){
				for(int gg=0;gg<tam;gg++){

					if(m[i][j]==final[g][gg]){
						acum = acum+abs((g-i)+(gg-j));
						rax=1;
					}
					if(rax){
						break;
					}
				}
				if(rax){
					rax=0;
					break;
				}
			}

		}
	}
	return acum;
}

bool encontrado(int vect[][tam]){
	for(int i=0;i<tam;i++){
		for(int j=0;j<tam;j++){
			if(vect[i][j]!=final[i][j]){
				return false;
			}
		}
	}
	return true;
}

void Desorden(int Matriz[][tam]){
	int a=0,b,c;
	int aux[tam][tam];
	bool estado[tam][tam];
	for(int i=0;i<tam;i++){
		for(int j=0;j<tam;j++){
			aux[i][j]=a;
			estado[i][j]=true;
			a++;
		}
	}
	srand(time(NULL));
	for(int i=0;i<tam;i++){
		for(int j=0;j<tam;j++){
			b=rand()%(tam);
			c=rand()%(tam);
			while(!estado[b][c]){
				b=rand()%(tam);
				c=rand()%(tam);
			}
				Matriz[i][j]=aux[b][c];
				estado[b][c]=false;
		}
	}

}

nodo *asignarNodo(int m[][tam]){
	nodo *r = new nodo;
	for(int i=0;i<tam;i++){
		for(int j=0;j<tam;j++){
			r->estado[i][j]=m[i][j];
			if(m[i][j]==0){
				r->pos[0]=i;
				r->pos[1]=j;
			}
		}
	}
	r->sig=NULL;
	return r;
}

void init(){
    int depth, res;
    allegro_init();
    depth = desktop_color_depth();
    if (depth==0) depth = 32;
    set_color_depth(depth);
    res= set_gfx_mode(GFX_AUTODETECT_WINDOWED,ancho, alto, 0, 0);
    if (res != 0){
        allegro_message(allegro_error);
        exit(-1);
    }
    install_timer();
    install_keyboard();
    install_mouse();

}

void definit(){
clear_keybuf();
}

void imprimir(int m[][tam]){
	for(int i=0;i<tam;i++){
		for(int j=0;j<tam;j++){
			cout<<"["<<m[i][j]<<"] ";
		}
		cout<<endl;
	}
}
