#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>

//eje y
#define N 15
//eje x
#define M 60
//tamaño de cuadriculas
#define enty 20

typedef struct
{
    char pos;
    char aux;
    int Num_map;
    int nivel;
    int map;
}casilla;

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

typedef struct{
    int x1;
    int x2;
    int y1;
    int y2;
} jugador;

typedef struct {
    bool ciclo;
    casilla mapa[N][M];
    jugador jug;
} game_state;

// Recibe un evento, si es input, cambia input_state. Luego retorna
void leer_input(game_state *game, input_manager *input, ALLEGRO_EVENT ev);

// Es dueño de game_state. Lee input_state, actualiza lógica, posiciones, cambio de nivel, y basicamente todo.
void actualizar(game_state *game, input_manager *input);

// Todo lo referente a dibujar.
void dibujar(game_state* game);

void N_mapa(game_state *game);
void juego(game_state *game,ALLEGRO_EVENT_QUEUE *queue,input_manager *teclado);

int i,j,k;

int main()
{
    game_state game;
    game.jug.x1=0;
    game.jug.y1=0;
    game.mapa[N][M].Num_map=0;
    input_manager teclado;
    teclado.W=false;
    teclado.S=false;
    teclado.A=false;
    teclado.D=false;
    game.ciclo=true;
    game.jug.x1=0;
    game.jug.y1=0;
    game.jug.x2=0;
    game.jug.y2=0;

    al_init();
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY *display=al_create_display(M*enty,N*enty);
    ALLEGRO_EVENT_QUEUE *queue=al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 10); 

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);
    N_mapa(&game);
    while(game.ciclo)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);
        leer_input(&game,&teclado, ev);
        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            actualizar(&game, &teclado);
            dibujar(&game); //Puedes meter al_flip_display() dentro de dibujar.
        }
        //caso moverse
        casilla casilla_jug = game.mapa[game.jug.y1][game.jug.x1];
        if (casilla_jug.nivel != -1)
        {

        }
    }

    al_destroy_display(display);
    al_destroy_event_queue(queue);

    return 0;
}
//gcc Rectangulo.c -o Rectangulo $(pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_primitives-5)

void N_mapa(game_state *game)
{
    FILE *fan;
    switch(game->mapa[N][M].Num_map)
    {
        case 0:
            fan=fopen("assets/mapas/mapa0.txt","r");
            for(i=0;i<N;i++)
            {
                for(j=0;j<M;j++)
                {
                    fscanf(fan,"%c",&game->mapa[i][j].pos);
                }
                fscanf(fan,"%c",&game->mapa[-1][-1].aux);
            }
            fclose(fan);
            break;
        case 1:
            fan=fopen("assets/mapas/mapa1.txt","r"); 
            for(i=0;i<N;i++)
            {
                for(j=0;j<M;j++)
                {
                    fscanf(fan,"%c",&game->mapa[i][j].pos);
                }
                fscanf(fan,"%c",&game->mapa[-1][-1].aux);
            }
            fclose(fan);
            break;
    }

    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
           // game->mapa[i][j].nivel = -1;
            if(game->mapa[i][j].pos=='c' || game->mapa[i][j].pos=='1' || game->mapa[i][j].pos=='o' || game->mapa[i][j].pos=='3' || game->mapa[i][j].pos=='2')
            {
                game->mapa[i][j].map=1;
            }
            else if(game->mapa[i][j].pos=='u')
            {
                game->mapa[i][j].map=1;
                game->jug.y1=i;
                game->jug.x1=j;
            }
            else if(game->mapa[i][j].pos=='a' || game->mapa[i][j].pos=='b')
            {
                game->mapa[i][j].map=0;
            }
            else
            {
                game->mapa[i][j].map=-1;
            }
            // caso nivel
        }
    }

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
                break;
        }
    }
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        game->ciclo=false;
    }

    return;
}

void dibujar(game_state* game)
{
    al_clear_to_color(al_map_rgb(255,255,255));
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            if(game->mapa[i][j].map==0 && game->mapa[i][j].pos=='a')
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(0,0,255));
            }
            else if(game->mapa[i][j].map==1 && (game->mapa[i][j].pos=='c' || game->mapa[i][j].pos=='u'))
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(0,0,0));
            }
            else if(game->mapa[i][j].map==1 && game->mapa[i][j].pos=='o')
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(200,200,255));
            }
            else if(game->mapa[i][j].map==1 && (game->mapa[i][j].pos=='1' || game->mapa[i][j].pos=='2' || game->mapa[i][j].pos=='3'))
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(100,0,200));
            }
            else if(game->mapa[i][j].map==0 && game->mapa[i][j].pos=='b')
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(0,100,255));
            }
        }
    }
    al_draw_filled_rectangle(game->jug.x1*enty,game->jug.y1*enty,(game->jug.x1+1)*enty,(game->jug.y1+1)*enty,al_map_rgb(255,0,0));
    al_flip_display();
    return;
}

void actualizar(game_state *game, input_manager *teclado)
{
    if(teclado->W && game->mapa[game->jug.y2-1][game->jug.x2].map!=0) 
    {
        game->jug.y2--;
    }
    if(teclado->S && game->mapa[game->jug.y2+1][game->jug.x2].map!=0) 
    {
        game->jug.y2++;
    }
    if(teclado->A && game->mapa[game->jug.y2][game->jug.x2-1].map!=0) 
    {
        game->jug.x2--;
    }
    if(teclado->D && game->mapa[game->jug.y2][game->jug.x2+1].map!=0) 
    {
        game->jug.x2++;
    }

    if(game->jug.x2>=0 && game->jug.x2<M && game->jug.y2>=0 && game->jug.y2<N && game->mapa[game->jug.y2][game->jug.x2].map==1)
    {
        game->jug.x1=game->jug.x2; game->jug.y1=game->jug.y2;
    }
    return;
}