#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>

//eje y
#define N 15
//eje x
#define M 60
//tamaño de cuadriculas
#define enty 60

typedef struct 
{
    bool A;
    bool S;
    bool W;
    bool D;
    bool ciclo;
    int x1;
    int x2;
    int y1;
    int y2;
}input_manager;

input_manager juego(char pos[N][M],int map[N][M],ALLEGRO_EVENT_QUEUE *queue,input_manager teclado);

int i,j,k;

int main()
{
    input_manager teclado;
    teclado.W=false;
    teclado.S=false;
    teclado.A=false;
    teclado.D=false;
    teclado.ciclo=true;
    teclado.x1=7;
    teclado.y1=8;
    teclado.x2=2;
    teclado.y2=2;
    char pos[N][M],aux;
    int map[N][M];

    al_init();
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY *display=al_create_display(M*enty,N*enty);
    ALLEGRO_EVENT_QUEUE *queue=al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 5); 

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    FILE *fan;
    fan=fopen("assets/mapas/mapa0.txt","r");

    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            fscanf(fan,"%c",&pos[i][j]);
        }
        fscanf(fan,"%c",&aux);
    }

    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            if(pos[i][j]=='c' || pos[i][j]=='p' || pos[i][j]=='o')
            {
                map[i][j]=1;
            }
            else if(pos[i][j]=='a' || pos[i][j]=='b')
            {
                map[i][j]=0;
            }
            else
            {
                map[i][j]=-1;
            }
        }
    }

    al_start_timer(timer);

    while(teclado.ciclo)
    {
        teclado=juego(pos,map,queue,teclado);
        al_draw_filled_rectangle(teclado.x1*enty,teclado.y1*enty,(teclado.x1+1)*enty,(teclado.y1+1)*enty,al_map_rgb(255,0,0));
        al_flip_display();
    }

    al_destroy_display(display);
    al_destroy_event_queue(queue);

    return 0;
}
//gcc Rectangulo.c -o Rectangulo $(pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_primitives-5)

input_manager juego(char pos[N][M],int map[N][M],ALLEGRO_EVENT_QUEUE *queue,input_manager teclado)
{
    ALLEGRO_EVENT ev;
    al_wait_for_event(queue, &ev);
    int regreso;

    if(ev.type==ALLEGRO_EVENT_KEY_DOWN)
    {
        teclado.y2=teclado.y1;
        teclado.x2=teclado.x1;
        switch(ev.keyboard.keycode)
        {
            case ALLEGRO_KEY_UP:
                    teclado.W=true;
                break;
            case ALLEGRO_KEY_DOWN:
                    teclado.S=true;
                break;
            case ALLEGRO_KEY_LEFT:
                    teclado.A=true;
                break;
            case ALLEGRO_KEY_RIGHT:
                    teclado.D=true;
                break;
            case ALLEGRO_KEY_ESCAPE:
                    teclado.ciclo=false;
                break;
        }
    }
    else if(ev.type==ALLEGRO_EVENT_KEY_UP)
    {
        switch(ev.keyboard.keycode)
        {
            case ALLEGRO_KEY_UP:
                    teclado.W=false;
                    regreso=teclado.W;
                break;
            case ALLEGRO_KEY_DOWN:
                    teclado.S=false;
                    regreso=teclado.S;
                break;
            case ALLEGRO_KEY_LEFT:
                    teclado.A=false;
                    regreso=teclado.A;
                break;
            case ALLEGRO_KEY_RIGHT:
                    teclado.D=false;
                    regreso=teclado.D;
                break;
            case ALLEGRO_KEY_ESCAPE:
                    teclado.ciclo=false;
                break;
        }
    }
    else if(ev.type == ALLEGRO_EVENT_TIMER)
    {
        if(teclado.W && map[teclado.y2-1][teclado.x2]!=0) 
        {
            teclado.y2--;
        }
        if(teclado.S && map[teclado.y2+1][teclado.x2]!=0) 
        {
            teclado.y2++;
        }
        if(teclado.A && map[teclado.y2][teclado.x2-1]!=0) 
        {
            teclado.x2--;
        }
        if(teclado.D && map[teclado.y2][teclado.x2+1]!=0) 
        {
            teclado.x2++;
        }

        if(teclado.x2>=0 && teclado.x2<M && teclado.y2>=0 && teclado.y2<N && map[teclado.y2][teclado.x2]==1)
        {
            teclado.x1=teclado.x2; teclado.y1=teclado.y2;
        }
    }
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        teclado.ciclo=false;
    }
    al_clear_to_color(al_map_rgb(255,255,255));
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            if(map[i][j]==0 && pos[i][j]=='a')
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(0,0,255));
            }
            else if(map[i][j]==1 && pos[i][j]=='c')
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(0,0,0));
            }
            else if(map[i][j]==1 && pos[i][j]=='o')
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(200,200,255));
            }
            else if(map[i][j]==1 && pos[i][j]=='p')
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(100,0,200));
            }
            else if(map[i][j]==0 && pos[i][j]=='b')
            {
                al_draw_filled_rectangle(j*enty,i*enty,(j+1)*enty,(i+1)*enty,al_map_rgb(0,100,255));
            }

        }
    }

    return teclado;
}