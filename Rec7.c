#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>

#define VEN_HOR 1920
#define VEN_VER 1000
//cantidad de mapas
#define K 4
//eje y
#define N 15
//eje x
#define M 61
//tamaño de cuadriculas
#define enty 100

typedef struct{
    int x1;
    int x2;
    int y1;
    int y2;
} jugador;

typedef struct
{
    char tip_res;
    char tipo;
    char Num_sala;
    int sala;
    int caminable;
}casilla;

typedef struct
{
    casilla mapa[N][M];
}nivel;

typedef struct {
    bool ciclo;
    bool ciclo2;
    int sala_a;
    nivel salas[K];
    jugador jug;
    int camY;
    int camX;
} game_state;


int i,j,k;

typedef struct 
{
    bool A;
    bool S;
    bool W;
    bool D;
}input_manager;

typedef struct{
    int posx;
    int posy;
    int llaves_necesarias;
    int peso;
    bool se_puede_mover;
} objeto_interactuable;




/*  casilla mapa[N][M];
    int codmapa;
xxx

xxx pamas[L]*/

// lee los mapas y los guarda
void leer_mapa(game_state *game);

// Recibe un evento, si es input, cambia input_state. Luego retorna
void leer_input(game_state *game, input_manager *input, ALLEGRO_EVENT ev);

// Es dueño de game_state. Lee input_state, actualiza lógica, posiciones, cambio de nivel, y basicamente todo.
void actualizar(game_state *game, input_manager *input);

void act_mapa(game_state *game);

// Todo lo referente a dibujar.
void dibujar(game_state *game);

// cambia el mapa en el que se encuentra el jugador
void N_sala(game_state *game);

void juego(game_state *game,ALLEGRO_EVENT_QUEUE *queue,input_manager *teclado);

int i,j,k;

int main()
{
    game_state game;
    game.jug.x1=0;
    game.jug.y1=0;
    game.sala_a=0;
    input_manager teclado;
    teclado.W=false;
    teclado.S=false;
    teclado.A=false;
    teclado.D=false;
    game.ciclo=true;
    game.ciclo2=true;
    game.jug.x1=0;
    game.jug.y1=0;
    game.jug.x2=0;
    game.jug.y2=0;

    FILE *cop0,*cop1,*cop2,*cop3;
    cop0=fopen("assets/mapas/copy0.txt","w");
    cop1=fopen("assets/mapas/copy1.txt","w");
    cop2=fopen("assets/mapas/copy2.txt","w");
    cop3=fopen("assets/mapas/copy3.txt","w");

    leer_mapa(&game);

    game.salas[game.sala_a].mapa[8][7].tipo='u';
    game.salas[game.sala_a].mapa[6][23].tipo='B';

    al_init();
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY *display=al_create_display(VEN_HOR,VEN_VER);
    ALLEGRO_EVENT_QUEUE *queue=al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 10); 

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);
    while(game.ciclo2)
    {
        N_sala(&game);
        while(game.ciclo)
        {
            ALLEGRO_EVENT ev;
            al_wait_for_event(queue, &ev);
            leer_input(&game,&teclado, ev);
            if(ev.type == ALLEGRO_EVENT_TIMER)
            {
                act_mapa(&game);    
                actualizar(&game, &teclado);
                dibujar(&game);
            }
        }
    }

    al_destroy_display(display);
    al_destroy_event_queue(queue);

    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            fprintf(cop0,"%c",game.salas[0].mapa[i][j].tipo);
            fprintf(cop1,"%c",game.salas[1].mapa[i][j].tipo);
            fprintf(cop2,"%c",game.salas[2].mapa[i][j].tipo);
            fprintf(cop3,"%c",game.salas[3].mapa[i][j].tipo);
        }
    }

    return 0;
}
//gcc Rectangulo.c -o Rectangulo $(pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_primitives-5)

void N_sala(game_state *game)
{
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
           // game->salas[game->sala_a].mapa[i][j].nivel = -1;
            if(
                   game->salas[game->sala_a].mapa[i][j].tipo=='.' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='1' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='o' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='3' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='2' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='0' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='p'
            )
            {
                game->salas[game->sala_a].mapa[i][j].caminable=1;
            }
            else if(game->salas[game->sala_a].mapa[i][j].tipo=='u')
            {
                game->salas[game->sala_a].mapa[i][j].caminable=1;
                game->jug.y1=i;
                game->jug.x1=j;
            }
            else if(game->salas[game->sala_a].mapa[i][j].tipo=='#' || game->salas[game->sala_a].mapa[i][j].tipo=='b' || game->salas[game->sala_a].mapa[i][j].tipo=='B')
            {
                game->salas[game->sala_a].mapa[i][j].caminable=0;
            }
            else
            {
                game->salas[game->sala_a].mapa[i][j].caminable=-1;
            }
        }
    }

    game->ciclo=true;

    return;
}

void leer_input(game_state *game, input_manager *teclado, ALLEGRO_EVENT ev)
{
    int regreso;
    if(ev.type==ALLEGRO_EVENT_KEY_DOWN)
    {
        game->jug.y2=game->jug.y1;
        game->jug.x2=game->jug.x1;
        switch(ev.keyboard.keycode)
        {
            case ALLEGRO_KEY_UP:
                    teclado->W=true;
                break;
            case ALLEGRO_KEY_DOWN:
                    teclado->S=true;
                break;
            case ALLEGRO_KEY_LEFT:
                    teclado->A=true;
                break;
            case ALLEGRO_KEY_RIGHT:
                    teclado->D=true;
                break;
            case ALLEGRO_KEY_ESCAPE:
                    game->ciclo=false;
                    game->ciclo2=false;
                break;
        }
    }
    else if(ev.type==ALLEGRO_EVENT_KEY_UP)
    {
        switch(ev.keyboard.keycode)
        {
            case ALLEGRO_KEY_UP:
                    teclado->W=false;
                    regreso=teclado->W;
                break;
            case ALLEGRO_KEY_DOWN:
                    teclado->S=false;
                    regreso=teclado->S;
                break;
            case ALLEGRO_KEY_LEFT:
                    teclado->A=false;
                    regreso=teclado->A;
                break;
            case ALLEGRO_KEY_RIGHT:
                    teclado->D=false;
                    regreso=teclado->D;
                break;
            case ALLEGRO_KEY_ESCAPE:
                    game->ciclo=false;
                    game->ciclo2=false;
                break;
        }
    }
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        game->ciclo=false;
        game->ciclo2=false;
    }

    return;
}

void dibujar(game_state* game)
{
    ALLEGRO_TRANSFORM camara;
    al_identity_transform(&camara);

    al_translate_transform(&camara,-game->camX,-game->camY);

    al_use_transform(&camara);

    al_clear_to_color(al_map_rgb(255,255,255));
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            casilla tile = game->salas[game->sala_a].mapa[i][j];
            if(tile.caminable==0 && tile.tipo=='#')
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(0,0,255));
            }
            else if(tile.caminable==1 && (tile.tipo=='.' || tile.tipo=='u'))
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(0,0,0));
            }
            else if(tile.caminable==1 && tile.tipo=='o')
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(200,200,255));
            }
            else if(tile.caminable==1 && (tile.tipo=='1' || tile.tipo=='2' || tile.tipo=='3' || tile.tipo=='0'))
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(100,0,200));
            }
            else if(tile.caminable==1 && tile.tipo=='p')
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(100,100,100));
            }
            else if(tile.caminable==0 && (tile.tipo=='B' || tile.tipo=='L'))
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(100,150,255));
            }
            else if(tile.caminable==0 && tile.tipo=='b')
            {
                if(game->salas[game->sala_a].mapa[6][32].tipo=='B' || game->salas[game->sala_a].mapa[6][32].tipo=='L')
                {
                    game->salas[game->sala_a].mapa[8][51].tipo='.';
                    game->salas[game->sala_a].mapa[8][51].caminable=1;    
                    game->salas[game->sala_a].mapa[6][32].tipo='L';  
                    game->salas[game->sala_a].mapa[6][32].caminable=0;
                    al_draw_filled_rectangle(51*enty,8*enty,(51+1)*enty,(8+1)*enty,al_map_rgb(0,0,0));
                    al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(0,100,255));
                }
                else if(game->salas[game->sala_a].mapa[6][32].tipo!='B' || game->salas[game->sala_a].mapa[6][32].tipo!='L')
                {
                    game->salas[game->sala_a].mapa[8][51].caminable=0;    game->salas[game->sala_a].mapa[8][51].tipo='b';
                    al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(0,100,255));
                }
            }
        }
    }
    al_draw_filled_rectangle(game->jug.x1*enty,game->jug.y1*enty,(game->jug.x1+1)*enty,(game->jug.y1+1)*enty,al_map_rgb(255,0,0));
    al_flip_display();
    return;
}

void actualizar(game_state *game, input_manager *teclado)
{
    float pan_an=VEN_HOR;
    float pan_al=VEN_VER;

    float jug_px=game->jug.x1*enty+(enty/2.0);
    float jug_py=game->jug.y1*enty+(enty/2.0);

    game->camX=jug_px-(pan_an/2.0);
    game->camY=jug_py-(pan_al/2.0);

    if ((M*enty)<=pan_an) 
    {
        game->camX=0; 
    } 
    else 
    {
        if(game->camX<0) 
        {
            game->camX=0;
        }
        if(game->camX>(M*enty)-pan_an) 
        {
            game->camX=(M*enty)-pan_an;
        }
    }

    if ((N*enty)<=pan_al) 
    {
        game->camY=0;
    } 
    else 
    {
        if(game->camY<0)game->camY=0;
        if(game->camY>(N*enty)-pan_al) 
        {
            game->camY=(N*enty)-pan_al;
        }
    }

    if(game->camX>(M*enty)-pan_an)
    { 
        game->camX=(M*enty)-pan_an;
    }
    if(game->camY>(N*enty)-pan_al) 
    {
        game->camY=(N*enty)-pan_al;
    }

    if(teclado->W && (game->salas[game->sala_a].mapa[game->jug.y2-1][game->jug.x2].caminable!=0  || (game->salas[game->sala_a].mapa[game->jug.y2-1][game->jug.x2].tipo=='B' && game->salas[game->sala_a].mapa[game->jug.y2-2][game->jug.x2].caminable!=0)) && game->salas[game->sala_a].mapa[game->jug.y2-1][game->jug.x2].caminable!=-1)
    {
        game->jug.y2--;
        if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo=='B')
        {
            game->salas[game->sala_a].mapa[game->jug.y2-1][game->jug.x2].tipo='B';
            game->salas[game->sala_a].mapa[game->jug.y2-1][game->jug.x2].caminable=0;
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo='.';
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].caminable=1;
        }
        if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='0' || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='1' || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='2' || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='3')
        {
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo='u';
            game->salas[game->sala_a].mapa[game->jug.y2+1][game->jug.x2].tipo=game->salas[game->sala_a].mapa[game->jug.y2+1][game->jug.x2].tip_res;
        }
    }
    if(teclado->S && (game->salas[game->sala_a].mapa[game->jug.y2+1][game->jug.x2].caminable!=0 || (game->salas[game->sala_a].mapa[game->jug.y2+1][game->jug.x2].tipo=='B' && game->salas[game->sala_a].mapa[game->jug.y2+2][game->jug.x2].caminable!=0)) && game->salas[game->sala_a].mapa[game->jug.y2+1][game->jug.x2].caminable!=-1)
    {
        game->jug.y2++;
        if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo=='B')
        {
            game->salas[game->sala_a].mapa[game->jug.y2+1][game->jug.x2].tipo='B';
            game->salas[game->sala_a].mapa[game->jug.y2+1][game->jug.x2].caminable=0;
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo='.';
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].caminable=1;
        }
        if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='0' 
            || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='1' 
            || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='2' 
            || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='3')
        {
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo='u';
            game->salas[game->sala_a].mapa[game->jug.y2-1][game->jug.x2].tipo=game->salas[game->sala_a].mapa[game->jug.y2-1][game->jug.x2].tip_res;
        }
    }
    if(teclado->A && (game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2-1].caminable!=0 || (game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2-1].tipo=='B' && game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2-2].caminable!=0)) && game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2-1].caminable!=-1) 
    {
        game->jug.x2--;
        if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo=='B')
        {
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2-1].tipo='B';
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2-1].caminable=0;
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo='.';
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].caminable=1;
        }
        if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='0' || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='1' || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='2' || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='3')
        {
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo='u';
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2+1].tipo=game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2+1].tip_res;
        }
    }
    if(teclado->D && (game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2+1].caminable!=0 || (game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2+1].tipo=='B' && game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2+2].caminable!=0)) && game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2+1].caminable!=-1) 
    {
        game->jug.x2++;
        if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo=='B')
        {
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2+1].tipo='B';
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2+1].caminable=0;
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo='.';
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].caminable=1;
        }
        if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='0' || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='1' || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='2' || game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo!='3')
        {
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tipo='u';
            game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2-1].tipo=game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2-1].tip_res;
        }
    }

    if(game->jug.x2>=0 && game->jug.x2<M && game->jug.y2>=0 && game->jug.y2<N && game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].caminable==1)
    {
        game->jug.x1=game->jug.x2; game->jug.y1=game->jug.y2;
    }
    
//=========================================================================================

    if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tip_res=='0')
    {
        game->sala_a=0;
        game->ciclo=false;
    }
    else if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tip_res=='1')
    {
        game->sala_a=1;
        game->ciclo=false;
    }
    else if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tip_res=='2')
    {
        game->sala_a=2;
        game->ciclo=false;
    }
    else if(game->salas[game->sala_a].mapa[game->jug.y2][game->jug.x2].tip_res=='3')
    {
        game->sala_a=3;
        game->ciclo=false;
    }
    return;
}

void leer_mapa(game_state *game)
{
    char auxelim;
    int ix;
    FILE *fan0,*fan1,*fan2,*fan3;

    //leer el .txt de la sala 0
    fan0=fopen("assets/mapas/mapa0.txt","r");
    //leer el .txt de la sala 1
    fan1=fopen("assets/mapas/mapa1.txt","r");
    //leer el .txt de la sala 2
    fan2=fopen("assets/mapas/mapa2.txt","r");
    //leer el .txt de la sala 3
    fan3=fopen("assets/mapas/mapa3.txt","r");
    
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            fscanf(fan0,"%c",&game->salas[0].mapa[i][j].tipo);
            game->salas[0].mapa[i][j].tip_res=game->salas[0].mapa[i][j].tipo;
            fscanf(fan1,"%c",&game->salas[1].mapa[i][j].tipo);
            game->salas[1].mapa[i][j].tip_res=game->salas[1].mapa[i][j].tipo;
            fscanf(fan2,"%c",&game->salas[2].mapa[i][j].tipo);
            game->salas[2].mapa[i][j].tip_res=game->salas[2].mapa[i][j].tipo;
            fscanf(fan3,"%c",&game->salas[3].mapa[i][j].tipo);
            game->salas[3].mapa[i][j].tip_res=game->salas[3].mapa[i][j].tipo;
        }
    }

    game->salas[1].mapa[i][j].tip_res=game->salas[1].mapa[8][1].tipo='u';
    return;
}

void act_mapa(game_state *game)
{
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            if(
                   game->salas[game->sala_a].mapa[i][j].tipo=='.' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='1' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='o' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='3' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='2' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='0' 
                || game->salas[game->sala_a].mapa[i][j].tipo=='p'
                )
            {
                game->salas[game->sala_a].mapa[i][j].caminable=1;
            }
            else if(game->salas[game->sala_a].mapa[i][j].tipo=='u')
            {
                game->salas[game->sala_a].mapa[i][j].caminable=1;
            }
            else if(game->salas[game->sala_a].mapa[i][j].tipo=='#' 
                 || game->salas[game->sala_a].mapa[i][j].tipo=='b' 
                 || game->salas[game->sala_a].mapa[i][j].tipo=='B' 
                 || game->salas[game->sala_a].mapa[i][j].tipo=='L'
                 )
            {
                game->salas[game->sala_a].mapa[i][j].caminable=0;
            }
            else
            {
                game->salas[game->sala_a].mapa[i][j].caminable=-1;
            }
        }
    }
}